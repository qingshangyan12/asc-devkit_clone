from __future__ import annotations

import os
import shutil
import signal
import subprocess
import time
from pathlib import Path
from typing import Callable, Dict, Iterable, List, Optional

from .model import Cell, Command, RunResult, StepResult


NpuGate = Callable[[Cell, Command, Callable[[], int]], tuple[int, float]]


def run_cell(
    cell: Cell,
    log_dir: Path,
    timeout: int,
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
        if cell.build_dir.exists():
            shutil.rmtree(cell.build_dir)
        cell.build_dir.mkdir(parents=True, exist_ok=True)

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
            command_started = time.monotonic()
            uses_npu = command_uses_npu(cell, command)
            wait_s = 0.0
            if uses_npu and npu_gate:
                step_rc, wait_s = npu_gate(cell, command, lambda: _run_command(command, run_dir, log, timeout))
            else:
                step_rc = _run_command(command, run_dir, log, timeout)
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
            if step_rc != 0:
                status = "FAIL"
                reason = "timeout" if step_rc == 124 else "nonzero rc"
                failing_step = command.raw
                rc = step_rc
                break

    duration = time.monotonic() - started
    if use_build_dir and not keep_artifacts and status == "PASS":
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


def _run_command(command: Command, cwd: Path, log, timeout: int) -> int:
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
            return 124
    except OSError as exc:
        log.write(f"[ERROR] failed to start command: {exc}\n")
        return 127
    log.write(stdout or "")
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
