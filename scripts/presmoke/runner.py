from __future__ import annotations

import os
import queue
import shutil
import signal
import subprocess
import concurrent.futures
import threading
import time
from pathlib import Path
from typing import Callable, Dict, Iterable, List, Optional, Union

from .model import Cell, Command, NpuStats, RunCellsResult, RunResult, StepResult
from .pool import NpuSlotPool


NpuGate = Callable[[Cell, Command, Callable[[], int]], tuple[int, float]]

PRESERVE_BUILD_ARTIFACT_CASES = {
    "01_simd_cpp_api/02_features/00_compilation/custom_op",
}


def run_cell(
    cell: Cell,
    log_dir: Path,
    timeout: int,
    host_timeout: int = 900,
    keep_artifacts: bool = False,
    npu_gate: Optional[NpuGate] = None,
) -> RunResult:
    started = time.monotonic()
    log_dir.mkdir(parents=True, exist_ok=True)
    log_file = log_dir / f"{safe_log_name(cell.example.rel_path)}__{cell.mode}.log"
    use_build_dir = any(command.kind in {"cmake", "make", "package_run"} for command in cell.commands)
    if cell.example.source == "case-runner":
        use_build_dir = False
    run_dir = cell.build_dir if use_build_dir else cell.example.path
    if use_build_dir:
        clean_build_dir(cell.build_dir)
        cell.build_dir.mkdir(parents=True, exist_ok=True)
    else:
        clean_build_dir(cell.build_dir)

    status = "PASS"
    reason = ""
    failing_step: Optional[str] = None
    rc: Optional[int] = 0
    steps: List[StepResult] = []

    with log_file.open("w", encoding="utf-8", errors="replace") as log:
        log.write(f"example={cell.example.rel_path}\narch={cell.arch}\nmode={cell.mode}\nrun_dir={run_dir}\n\n")
        for command in cell.commands:
            if not command.raw or command.kind == "skip":
                continue
            log.write(f"\n$ {command.raw}\n")
            log.flush()
            command_started = time.monotonic()
            uses_npu = command_uses_npu(cell, command)
            wait_s = 0.0
            step_timeout = select_timeout(uses_npu, timeout, host_timeout)
            if uses_npu and npu_gate:
                step_rc, wait_s = npu_gate(cell, command, lambda: _run_command(command, run_dir, log, step_timeout))
            else:
                step_rc = _run_command(command, run_dir, log, step_timeout)
            step_duration = time.monotonic() - command_started
            steps.append(
                StepResult(
                    command=command.raw,
                    kind=command.kind,
                    rc=step_rc,
                    duration_s=step_duration,
                    wait_s=wait_s,
                    npu_slot=uses_npu,
                    timeout=step_rc == 124,
                )
            )
            log.write(f"\n[STEP] rc={step_rc} duration_s={step_duration:.3f} wait_s={wait_s:.3f} npu_slot={int(uses_npu)}\n")
            log.flush()
            if step_rc != 0:
                status = "FAIL"
                reason = "timeout" if step_rc == 124 else "nonzero rc"
                failing_step = command.raw
                rc = step_rc
                break

    duration = time.monotonic() - started
    if should_remove_build_dir(cell, use_build_dir, keep_artifacts, status):
        shutil.rmtree(cell.build_dir, ignore_errors=True)
    return RunResult(
        example=cell.example.rel_path,
        arch=cell.arch,
        mode=cell.mode,
        status=status,
        reason=reason,
        failing_step=failing_step,
        rc=rc,
        duration_s=duration,
        log_file=str(log_file),
        source=cell.example.source,
        steps=steps,
    )


def run_cells_pipeline(
    cells: Iterable[Cell],
    log_dir: Path,
    timeout: int,
    host_timeout: int = 900,
    keep_artifacts: bool = False,
    jobs: int = 1,
    npu_slots: int = 1,
) -> RunCellsResult:
    cells_list = list(cells)
    if jobs <= 1:
        npu_pool = NpuSlotPool(npu_slots)
        results = [
            run_cell(
                cell,
                log_dir=log_dir,
                timeout=timeout,
                host_timeout=host_timeout,
                keep_artifacts=keep_artifacts,
                npu_gate=npu_pool.gate,
            )
            for cell in cells_list
        ]
        return RunCellsResult(results, NpuStats(slots=npu_slots, queue_model="serial"))

    npu_pool = NpuSlotPool(npu_slots)
    ready: "queue.Queue[Union[_PipelineItem, None]]" = queue.Queue()
    verify_ready: "queue.Queue[Union[_PipelineItem, None]]" = queue.Queue()
    results: List[RunResult] = []
    results_lock = threading.Lock()
    npu_stats = _NpuPipelineStats(npu_slots)

    def append_result(result: RunResult) -> None:
        with results_lock:
            results.append(result)

    def build_worker(cell: Cell) -> None:
        item = run_cell_stage(
            cell,
            log_dir=log_dir,
            timeout=timeout,
            host_timeout=host_timeout,
            commands=build_stage_commands(cell.commands),
            keep_artifacts=True,
            stage_name="build",
        )
        if item.result.status == "PASS":
            ready.put(item)
        else:
            append_result(item.result)

    def npu_worker(slot_idx: int) -> None:
        while True:
            wait_started = time.monotonic()
            item = ready.get()
            npu_stats.add_idle(slot_idx, time.monotonic() - wait_started)
            try:
                if item is None:
                    return
                busy_started = time.monotonic()
                result = finish_cell_after_build(
                    item,
                    timeout=timeout,
                    host_timeout=host_timeout,
                    npu_gate=npu_pool.gate,
                )
                npu_stats.add_busy(slot_idx, time.monotonic() - busy_started)
                if result.status == "PASS":
                    verify_ready.put(item)
                else:
                    append_result(result)
            finally:
                ready.task_done()

    def verify_worker() -> None:
        while True:
            item = verify_ready.get()
            try:
                if item is None:
                    return
                result = finish_cell_verify(item, timeout=timeout, host_timeout=host_timeout, keep_artifacts=keep_artifacts)
                append_result(result)
            finally:
                verify_ready.task_done()

    npu_threads = [
        threading.Thread(target=npu_worker, args=(idx,), name=f"presmoke-npu-{idx}")
        for idx in range(npu_slots)
    ]
    verify_threads = [threading.Thread(target=verify_worker, name=f"presmoke-verify-{idx}") for idx in range(jobs)]
    for thread in npu_threads:
        thread.start()
    for thread in verify_threads:
        thread.start()

    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as executor:
            futures = [executor.submit(build_worker, cell) for cell in cells_list]
            for future in concurrent.futures.as_completed(futures):
                future.result()
        ready.join()
        verify_ready.join()
    finally:
        for _ in npu_threads:
            ready.put(None)
        for _ in verify_threads:
            verify_ready.put(None)
        for thread in npu_threads:
            thread.join()
        for thread in verify_threads:
            thread.join()

    return RunCellsResult(results, npu_stats.snapshot())


class _NpuPipelineStats:
    def __init__(self, slots: int) -> None:
        self._slots = slots
        self._idle_by_slot = [0.0 for _ in range(slots)]
        self._busy_by_slot = [0.0 for _ in range(slots)]
        self._lock = threading.Lock()

    def add_idle(self, slot_idx: int, duration_s: float) -> None:
        with self._lock:
            self._idle_by_slot[slot_idx] += duration_s

    def add_busy(self, slot_idx: int, duration_s: float) -> None:
        with self._lock:
            self._busy_by_slot[slot_idx] += duration_s

    def snapshot(self) -> NpuStats:
        with self._lock:
            idle_by_slot = list(self._idle_by_slot)
            busy_by_slot = list(self._busy_by_slot)
        return NpuStats(
            slots=self._slots,
            queue_model="pipeline",
            idle_s=sum(idle_by_slot),
            busy_s=sum(busy_by_slot),
            max_slot_idle_s=max(idle_by_slot, default=0.0),
            max_slot_busy_s=max(busy_by_slot, default=0.0),
        )


class _PipelineItem:
    def __init__(self, cell: Cell, result: RunResult, run_dir: Path, log_file: Path, started: float) -> None:
        self.cell = cell
        self.result = result
        self.run_dir = run_dir
        self.log_file = log_file
        self.started = started
        self.ready_at = time.monotonic()


def run_cell_stage(
    cell: Cell,
    log_dir: Path,
    timeout: int,
    host_timeout: int,
    commands: Iterable[Command],
    keep_artifacts: bool,
    stage_name: str,
) -> _PipelineItem:
    started = time.monotonic()
    log_dir.mkdir(parents=True, exist_ok=True)
    log_file = log_dir / f"{safe_log_name(cell.example.rel_path)}__{cell.mode}.log"
    use_build_dir = any(command.kind in {"cmake", "make", "package_run"} for command in cell.commands)
    if cell.example.source == "case-runner":
        use_build_dir = False
    run_dir = cell.build_dir if use_build_dir else cell.example.path
    if use_build_dir:
        clean_build_dir(cell.build_dir)
        cell.build_dir.mkdir(parents=True, exist_ok=True)
    elif stage_name == "build":
        clean_build_dir(cell.build_dir)

    result = RunResult(
        example=cell.example.rel_path,
        arch=cell.arch,
        mode=cell.mode,
        status="PASS",
        reason="",
        rc=0,
        log_file=str(log_file),
        source=cell.example.source,
        steps=[],
    )
    with log_file.open("w", encoding="utf-8", errors="replace") as log:
        log.write(f"example={cell.example.rel_path}\narch={cell.arch}\nmode={cell.mode}\nrun_dir={run_dir}\n\n")
        run_commands(result, cell, commands, run_dir, log, timeout, host_timeout)
    result.duration_s = time.monotonic() - started
    if should_remove_build_dir(cell, use_build_dir, keep_artifacts, result.status):
        shutil.rmtree(cell.build_dir, ignore_errors=True)
    return _PipelineItem(cell, result, run_dir, log_file, started)


def finish_cell_after_build(
    item: _PipelineItem,
    timeout: int,
    host_timeout: int,
    npu_gate: Optional[NpuGate],
) -> RunResult:
    cell = item.cell
    with item.log_file.open("a", encoding="utf-8", errors="replace") as log:
        if item.result.status == "PASS":
            run_commands(
                item.result,
                cell,
                run_stage_commands(cell.commands),
                item.run_dir,
                log,
                timeout,
                host_timeout,
                npu_gate,
                initial_npu_wait_s=time.monotonic() - item.ready_at,
            )
    item.result.duration_s = time.monotonic() - item.started
    return item.result


def finish_cell_verify(item: _PipelineItem, timeout: int, host_timeout: int, keep_artifacts: bool) -> RunResult:
    cell = item.cell
    with item.log_file.open("a", encoding="utf-8", errors="replace") as log:
        if item.result.status == "PASS":
            run_commands(item.result, cell, verify_stage_commands(cell.commands), item.run_dir, log, timeout, host_timeout)
    item.result.duration_s = time.monotonic() - item.started
    if should_remove_build_dir(cell, cell.build_dir.exists(), keep_artifacts, item.result.status):
        shutil.rmtree(cell.build_dir, ignore_errors=True)
    return item.result


def run_commands(
    result: RunResult,
    cell: Cell,
    commands: Iterable[Command],
    run_dir: Path,
    log,
    timeout: int,
    host_timeout: int,
    npu_gate: Optional[NpuGate] = None,
    initial_npu_wait_s: float = 0.0,
) -> None:
    pending_npu_wait_s = initial_npu_wait_s
    for command in commands:
        if not command.raw or command.kind == "skip":
            continue
        log.write(f"\n$ {command.raw}\n")
        log.flush()
        command_started = time.monotonic()
        uses_npu = command_uses_npu(cell, command)
        step_timeout = select_timeout(uses_npu, timeout, host_timeout)
        wait_s = 0.0
        if uses_npu and npu_gate:
            step_rc, wait_s = npu_gate(cell, command, lambda: _run_command(command, run_dir, log, step_timeout))
        else:
            step_rc = _run_command(command, run_dir, log, step_timeout)
        queued_wait_s = 0.0
        if uses_npu and pending_npu_wait_s:
            queued_wait_s = pending_npu_wait_s
            wait_s += queued_wait_s
            pending_npu_wait_s = 0.0
        step_duration = time.monotonic() - command_started
        result.steps.append(
            StepResult(
                command=command.raw,
                kind=command.kind,
                rc=step_rc,
                duration_s=step_duration,
                wait_s=wait_s,
                npu_slot=uses_npu,
                timeout=step_rc == 124,
            )
        )
        log.write(f"\n[STEP] rc={step_rc} duration_s={step_duration:.3f} wait_s={wait_s:.3f} npu_slot={int(uses_npu)}\n")
        log.flush()
        if step_rc != 0:
            result.status = "FAIL"
            result.reason = "timeout" if step_rc == 124 else "nonzero rc"
            result.failing_step = command.raw
            result.rc = step_rc
            return


def build_stage_commands(commands: Iterable[Command]) -> List[Command]:
    return [command for command in commands if command.kind not in {"run", "package_run", "verify"}]


def run_stage_commands(commands: Iterable[Command]) -> List[Command]:
    return [command for command in commands if command.kind in {"run", "package_run"}]


def verify_stage_commands(commands: Iterable[Command]) -> List[Command]:
    return [command for command in commands if command.kind == "verify"]


def clean_build_dir(build_dir: Path) -> None:
    if build_dir.exists():
        shutil.rmtree(build_dir, ignore_errors=True)


def should_remove_build_dir(cell: Cell, use_build_dir: bool, keep_artifacts: bool, status: str) -> bool:
    if not use_build_dir or keep_artifacts or status != "PASS":
        return False
    return cell.example.rel_path not in PRESERVE_BUILD_ARTIFACT_CASES


def command_uses_npu(cell: Cell, command: Command) -> bool:
    if cell.mode != "npu":
        return False
    if command.kind in {"run", "package_run"}:
        return True
    stripped = command.raw.strip()
    if stripped.startswith("./") or stripped.startswith("msprof "):
        return True
    if "mssanitizer" in stripped:
        return True
    return False


def select_timeout(uses_npu: bool, npu_timeout: int, host_timeout: int) -> Optional[int]:
    return npu_timeout if uses_npu else None


def _run_command(command: Command, cwd: Path, log, timeout: Optional[int]) -> int:
    env = os.environ.copy()
    env.update(command.env)
    try:
        proc = subprocess.Popen(
            command.raw,
            shell=True,
            cwd=str(cwd),
            env=env,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            preexec_fn=os.setsid if hasattr(os, "setsid") else None,
        )
        try:
            stdout, _ = proc.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            _kill_process_group(proc)
            stdout, _ = proc.communicate(timeout=10)
            log.write(stdout or "")
            log.write(f"\n[TIMEOUT] after {timeout}s\n")
            log.flush()
            return 124
    except OSError as exc:
        log.write(f"[ERROR] failed to start command: {exc}\n")
        return 127
    log.write(stdout or "")
    log.flush()
    return proc.returncode


def _kill_process_group(proc: subprocess.Popen) -> None:
    try:
        if hasattr(os, "killpg") and proc.pid:
            os.killpg(proc.pid, signal.SIGTERM)
        else:
            proc.terminate()
    except OSError:
        return
    time.sleep(1)
    if proc.poll() is None:
        try:
            if hasattr(os, "killpg") and proc.pid:
                os.killpg(proc.pid, signal.SIGKILL)
            else:
                proc.kill()
        except OSError:
            pass


def safe_log_name(rel_path: str) -> str:
    return rel_path.replace("/", "__")
