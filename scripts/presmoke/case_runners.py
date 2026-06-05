from __future__ import annotations

import json
from pathlib import Path
from typing import Iterable, List

from .discover import filter_examples
from .model import Cell, Command, ExampleSpec, RunResult, Suggestion


def build_case_runner_cells(
    project_root: Path,
    arch: str,
    modes: Iterable[str],
    includes: Iterable[str],
    excludes: Iterable[str],
) -> tuple[List[Cell], List[Suggestion]]:
    cells, suggestions, _ = build_case_runner_cells_with_skips(project_root, arch, modes, includes, excludes)
    return cells, suggestions


def build_case_runner_cells_with_skips(
    project_root: Path,
    arch: str,
    modes: Iterable[str],
    includes: Iterable[str],
    excludes: Iterable[str],
) -> tuple[List[Cell], List[Suggestion], List[RunResult]]:
    runners_root = project_root / "scripts" / "presmoke" / "cases"
    manifest = load_manifest(project_root)
    runner_dirs = sorted(path.parent for path in runners_root.rglob("run.sh"))
    runner_dirs = filter_examples(runner_dirs, includes, excludes)
    modes_list = list(modes)
    cells: List[Cell] = []
    suggestions: List[Suggestion] = []
    skipped: List[RunResult] = []
    for runner_dir in runner_dirs:
        rel_path = runner_dir.relative_to(runners_root).as_posix()
        info = manifest.get(rel_path, {})
        supported_archs = list(info.get("supported_archs") or [arch])
        supported_modes = list(info.get("supported_modes") or modes_list or ["npu"])
        confidence = info.get("confidence", "")
        reasons = ", ".join(info.get("reasons", []))
        if info.get("skip"):
            skip_reason = info.get("skip_reason") or "explicit skip"
            for mode in modes_list:
                skipped.append(
                    RunResult(
                        rel_path,
                        arch,
                        mode,
                        "SKIP",
                        skip_reason,
                        duration_s=0,
                        source="case-runner",
                    )
                )
            suggestions.append(
                Suggestion(
                    rel_path,
                    "case-runner",
                    "info",
                    "Explicitly skipped",
                    skip_reason,
                )
            )
            continue
        if confidence and confidence != "high":
            suggestions.append(
                Suggestion(
                    rel_path,
                    "case-runner",
                    "warn" if confidence == "low" else "info",
                    f"Generated runner confidence is {confidence}",
                    reasons,
                )
            )
        for mode in modes_list:
            if arch not in supported_archs:
                suggestions.append(
                    Suggestion(
                        rel_path,
                        "case-runner",
                        "info",
                        f"Excluded on unsupported arch {arch}",
                        f"Supported archs: {', '.join(supported_archs)}",
                    )
                )
                continue
            if mode not in supported_modes:
                suggestions.append(
                    Suggestion(
                        rel_path,
                        "case-runner",
                        "info",
                        f"Excluded on unsupported mode {mode}",
                        f"Supported modes: {', '.join(supported_modes)}",
                    )
                )
                continue
            runner = runner_dir / "run.sh"
            commands = [
                Command(f"bash {runner} clean", "clean", {"PRESMOKE_PROJECT_ROOT": str(project_root), "PRESMOKE_ARCH": arch, "PRESMOKE_MODE": mode}),
                Command(f"bash {runner} build", "build", {"PRESMOKE_PROJECT_ROOT": str(project_root), "PRESMOKE_ARCH": arch, "PRESMOKE_MODE": mode}),
                Command(f"bash {runner} run", "run", {"PRESMOKE_PROJECT_ROOT": str(project_root), "PRESMOKE_ARCH": arch, "PRESMOKE_MODE": mode}),
                Command(f"bash {runner} verify", "verify", {"PRESMOKE_PROJECT_ROOT": str(project_root), "PRESMOKE_ARCH": arch, "PRESMOKE_MODE": mode}),
            ]
            spec = ExampleSpec(
                path=project_root / "examples" / rel_path,
                rel_path=rel_path,
                commands=commands,
                archs=supported_archs,
                modes=supported_modes,
                source="case-runner",
                suggestions=[],
            )
            cells.append(Cell(spec, arch, mode, commands, spec.path / f"build_{mode}"))
    return cells, suggestions, skipped


def load_manifest(project_root: Path) -> dict[str, dict]:
    manifest = project_root / "scripts" / "presmoke" / "reports" / "case_runner_manifest.json"
    if not manifest.exists():
        return {}
    data = json.loads(manifest.read_text(encoding="utf-8"))
    return {item["case"]: item for item in data}
