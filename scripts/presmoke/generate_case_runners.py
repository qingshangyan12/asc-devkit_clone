#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

from __future__ import annotations

import argparse
import json
import logging
import re
import shlex
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Iterable, List

from presmoke.discover import discover_examples
from presmoke.model import Cell, Command
from presmoke.planner import is_cmake_configure
from presmoke.planner import build_cells, rewrite_command
from presmoke.readme_spec import parse_readme


LOG = logging.getLogger(__name__)


SPECIAL_PREFIXES = (
    "01_simd_cpp_api/02_features/00_",
    "01_simd_cpp_api/02_features/01_",
    "01_simd_cpp_api/02_features/02_",
)

CUSTOM_OP_PACKAGE_CASE = "01_simd_cpp_api/02_features/00_compilation/custom_op"

CUSTOM_OP_PACKAGE_DEPENDENTS = {
    "01_simd_cpp_api/02_features/01_invocation/aclnn_invocation",
    "01_simd_cpp_api/02_features/01_invocation/aclop_invocation",
    "01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin",
    "01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming",
}

TENSORFLOW_SKIP_REASON = "requires TensorFlow 2.6.5 environment; skipped by presmoke"
ARCH_OVERRIDES = {
    "01_simd_cpp_api/02_features/00_compilation/custom_op": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/01_invocation/aclnn_invocation": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/01_invocation/aclop_invocation": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_builtin": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming": ["dav-2201", "dav-3510"],
    # README "支持的产品" only lists Ascend 950PR/950DT. The option table also
    # mentions dav-2201, but the sample uses FP8/hifloat8 types unavailable on 910B.
    "01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8": ["dav-3510"],
}

ARCH_ENV_KEYS = {
    "ARCH",
    "ASC_ARCH",
    "ASCEND_ARCH",
    "CMAKE_ASC_ARCHITECTURES",
    "NPU_ARCH",
}

BASH_LICENSE_HEADER = [
    "#!/bin/bash",
    "# ----------------------------------------------------------------------------------------------------------",
    "# Copyright (c) 2026 Huawei Technologies Co., Ltd.",
    "# This program is free software, you can redistribute it and/or modify it under the terms and conditions of",
    '# CANN Open Software License Agreement Version 2.0 (the "License").',
    "# Please refer to the License for details. You may not use this file except in compliance with the License.",
    '# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,',
    "# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.",
    "# See LICENSE in the root of the software repository for the full text of the License.",
    "# ----------------------------------------------------------------------------------------------------------",
]


@dataclass
class CaseReport:
    case: str
    runner: str
    source: str
    confidence: str
    supported_archs: List[str] = field(default_factory=list)
    supported_modes: List[str] = field(default_factory=list)
    target_runnable: bool = False
    skip: bool = False
    skip_reason: str = ""
    reasons: List[str] = field(default_factory=list)
    commands: List[str] = field(default_factory=list)


@dataclass
class RunnerRenderSpec:
    rel: str
    build_cmds: List[Command]
    run_cmds: List[Command]
    verify_cmds: List[Command]
    custom_op_dependency: bool = False
    custom_op_package_case: bool = False
    skip_reason: str = ""


def main() -> int:
    logging.basicConfig(level=logging.INFO, format="%(message)s")
    args = parse_args()
    project_root = Path(args.project_root).resolve()
    examples_root = project_root / "examples"
    runners_root = project_root / "scripts" / "presmoke" / "cases"
    reports_root = project_root / "scripts" / "presmoke" / "reports"
    runners_root.mkdir(parents=True, exist_ok=True)
    reports_root.mkdir(parents=True, exist_ok=True)

    specs = [parse_readme(path, examples_root) for path in discover_examples(examples_root)]
    apply_case_overrides(specs)
    cells, _ = build_cells(specs, args.arch, [args.mode])
    runnable_by_rel = {cell.example.rel_path: cell for cell in cells}
    all_cells = [cell_for_spec(spec, args.arch, args.mode) for spec in specs]
    reports: List[CaseReport] = []
    for cell in all_cells:
        report = write_runner(
            project_root,
            runners_root,
            cell,
            runnable_on_target=cell.example.rel_path in runnable_by_rel,
        )
        reports.append(report)

    write_manifest(reports_root, reports)
    LOG.info("generated_runners=%s", len(reports))
    LOG.info("low_confidence=%s", sum(1 for item in reports if item.confidence != "high"))
    LOG.info("reports_dir=%s", reports_root)
    return 0


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate per-case presmoke run.sh wrappers")
    parser.add_argument("--project-root", default=Path(__file__).resolve().parents[2])
    parser.add_argument("--arch", default="dav-2201")
    parser.add_argument("--mode", default="npu")
    return parser.parse_args()


def cell_for_spec(spec, arch: str, mode: str) -> Cell:
    commands = [rewrite_command(command, arch, mode) for command in spec.commands]
    build_dir = spec.path / f"build_{mode}"
    return Cell(spec, arch, mode, commands, build_dir)


def apply_case_overrides(specs) -> None:
    for spec in specs:
        if spec.rel_path in ARCH_OVERRIDES:
            spec.archs = ARCH_OVERRIDES[spec.rel_path]


def write_runner(project_root: Path, runners_root: Path, cell, runnable_on_target: bool) -> CaseReport:
    rel = cell.example.rel_path
    runner_dir = runners_root / rel
    runner_dir.mkdir(parents=True, exist_ok=True)
    runner = runner_dir / "run.sh"
    confidence, reasons = classify_confidence(cell)
    if not runnable_on_target:
        reasons.append("unsupported_on_target_arch_or_mode")
        confidence = downgrade(confidence)
    build_cmds = [
        command
        for command in cell.commands
        if command.raw and (
            command.kind in {"cmake", "make", "gen_data", "shell"}
            or is_atc_prerequisite_command(command, cell.commands)
        )
    ]
    run_cmds = [
        command
        for command in cell.commands
        if command.raw
        and command.kind in {"run", "package_run"}
        and command not in build_cmds
    ]
    verify_cmds = [command for command in cell.commands if command.raw and command.kind == "verify"]

    if not run_cmds:
        run_cmds = [Command(":", "run")]
        if "no_run_step" not in reasons:
            reasons.append("no_run_step")
            confidence = downgrade(confidence)
    if not verify_cmds:
        verify_cmds = [Command(":", "verify")]

    custom_op_package_case = rel == CUSTOM_OP_PACKAGE_CASE
    custom_op_dependency = requires_custom_op_package(rel)
    skip_reason = explicit_skip_reason(rel)
    if skip_reason:
        reasons.append("explicit_skip")
        confidence = "high"
        build_cmds = [Command(":", "build")]
        run_cmds = [Command(":", "run")]
        verify_cmds = [Command(":", "verify")]
    elif custom_op_package_case:
        reasons.append("custom_op_package_provider")
        build_cmds = [Command(":", "build")]
        run_cmds = [Command(":", "run")]
        verify_cmds = [Command(":", "verify")]
    elif custom_op_dependency:
        reasons.append("requires_custom_op_package")

    text = _render_runner_from_spec(
        RunnerRenderSpec(
            rel=rel,
            build_cmds=build_cmds,
            run_cmds=run_cmds,
            verify_cmds=verify_cmds,
            custom_op_dependency=custom_op_dependency,
            custom_op_package_case=custom_op_package_case,
            skip_reason=skip_reason,
        )
    )
    runner.write_text(text, encoding="utf-8")
    runner.chmod(0o755)
    return CaseReport(
        case=rel,
        runner=runner.relative_to(project_root).as_posix(),
        source=cell.example.source,
        confidence=confidence,
        supported_archs=cell.example.archs,
        supported_modes=cell.example.modes,
        target_runnable=runnable_on_target,
        skip=bool(skip_reason),
        skip_reason=skip_reason,
        reasons=reasons,
        commands=[command.raw for command in cell.commands if command.raw],
    )


def classify_confidence(cell) -> tuple[str, List[str]]:
    reasons: List[str] = []
    confidence = "high"
    rel = cell.example.rel_path
    commands = [command.raw for command in cell.commands if command.raw]
    joined = "\n".join(commands)
    if cell.example.source != "readme":
        reasons.append(f"source_{cell.example.source}")
        confidence = downgrade(confidence)
    if rel.startswith(SPECIAL_PREFIXES):
        reasons.append("special_02_features_00_01_02")
        confidence = "low"
    if re.search(r"\$[A-Za-z_][A-Za-z0-9_]*|\$\{[^}]+}", joined):
        reasons.append("shell_variable_reference")
        confidence = "low"
    if re.search(r"(^|[;\s])(for|while|if|case|function)\b", joined):
        reasons.append("shell_control_flow")
        confidence = "low"
    if "\\\n" in joined or any(command.rstrip().endswith("\\") for command in commands):
        reasons.append("line_continuation")
        confidence = "low"
    if any("atc " in command or command.startswith("atc") for command in commands):
        reasons.append("atc_command")
        confidence = downgrade(confidence)
    if any("tensorflow" in command.lower() or "onnx" in command.lower() for command in commands):
        reasons.append("framework_command")
        confidence = downgrade(confidence)
    if not any(command.startswith("cmake") for command in commands):
        reasons.append("no_cmake_step")
        confidence = downgrade(confidence)
    if not any(command.startswith("make") for command in commands):
        reasons.append("no_make_step")
        confidence = downgrade(confidence)
    return confidence, sorted(set(reasons))


def is_atc_prerequisite_command(command: Command, commands: Iterable[Command]) -> bool:
    if command.kind != "run" or not command.raw.startswith("python3 "):
        return False
    command_list = list(commands)
    try:
        index = command_list.index(command)
    except ValueError:
        return False
    return any(item.raw.startswith("atc ") for item in command_list[index + 1:])


def downgrade(confidence: str) -> str:
    if confidence == "high":
        return "medium"
    return confidence


def requires_custom_op_package(rel: str) -> bool:
    if rel == CUSTOM_OP_PACKAGE_CASE:
        return True
    if rel in CUSTOM_OP_PACKAGE_DEPENDENTS:
        return True
    if "/02_framework/01_tensorflow/" in rel:
        return True
    return False


def explicit_skip_reason(rel: str) -> str:
    if "/02_framework/01_tensorflow/" in rel:
        return TENSORFLOW_SKIP_REASON
    return ""


def render_runner(
    rel: str,
    build_cmds: List[Command],
    run_cmds: List[Command],
    verify_cmds: List[Command],
    custom_op_dependency: bool = False,
    custom_op_package_case: bool = False,
    skip_reason: str = "",
) -> str:
    return _render_runner_from_spec(
        RunnerRenderSpec(
            rel=rel,
            build_cmds=build_cmds,
            run_cmds=run_cmds,
            verify_cmds=verify_cmds,
            custom_op_dependency=custom_op_dependency,
            custom_op_package_case=custom_op_package_case,
            skip_reason=skip_reason,
        )
    )


def _render_runner_from_spec(spec: RunnerRenderSpec) -> str:
    pre_build_lines = (
        ['    presmoke_ensure_custom_op_package'] if spec.custom_op_dependency and not spec.skip_reason else []
    )
    pre_run_lines = (
        ['    presmoke_ensure_custom_op_package'] if spec.custom_op_package_case and not spec.skip_reason else []
    )
    skip_lines = []
    if spec.skip_reason:
        quoted_reason = shlex.quote(spec.skip_reason)
        skip_lines = [
            f"SKIP_REASON={quoted_reason}",
            "",
        ]
    return "\n".join(
        [
            *BASH_LICENSE_HEADER,
            "",
            "set -euo pipefail",
            "",
            f'CASE_REL={shlex.quote(spec.rel)}',
            *skip_lines,
            'SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"',
            f'source "$SCRIPT_DIR/{case_entry_relative_path(spec.rel)}"',
            'presmoke_case_init "$CASE_REL"',
            "",
            "case_build() {",
            '    mkdir -p "$BUILD_DIR"',
            *pre_build_lines,
            *indent_commands(spec.build_cmds),
            "}",
            "",
            "case_run() {",
            '    mkdir -p "$BUILD_DIR"',
            *pre_run_lines,
            *indent_commands(spec.run_cmds, default_cd_build=True),
            "}",
            "",
            "case_verify() {",
            '    mkdir -p "$BUILD_DIR"',
            *indent_commands(spec.verify_cmds, default_cd_build=True),
            "}",
            "",
            "case_clean() {",
            "    presmoke_default_clean",
            "}",
            "",
            'presmoke_case_main "$@"',
            "",
        ]
    )


def case_entry_relative_path(rel: str) -> str:
    depth = len(Path(rel).parts)
    return "../" * depth + "_case_entry.sh"


def indent_commands(commands: Iterable[Command], default_cd_build: bool = False) -> List[str]:
    lines: List[str] = []
    for command in commands:
        if isinstance(command, str):
            command = Command(command, "shell")
        raw = command.raw
        if not raw or raw == ":":
            lines.append("    :")
            continue
        raw = rewrite_runtime_arch_options(raw)
        raw = rewrite_runtime_cmake_options(raw)
        quoted = shlex.quote(raw)
        env_prefix = command_env_prefix(command)
        runner = f"{env_prefix}bash -lc {quoted}"
        if raw.startswith("cmake "):
            normalized = raw
            quoted = shlex.quote(normalized)
            runner = f"{env_prefix}bash -lc {quoted}"
            if is_cmake_configure(normalized) and (" .." in normalized or normalized.endswith("..")):
                lines.append(f'    (cd "$BUILD_DIR" && {runner})')
            elif is_cmake_configure(normalized):
                lines.append(f'    (cd "$CASE_DIR" && {runner})')
            else:
                lines.append(f'    (cd "$CASE_DIR" && {runner})')
        elif raw.startswith("make"):
            lines.append(f'    (cd "$BUILD_DIR" && {runner})')
        elif raw.startswith("atc "):
            lines.append(f'    (cd "$BUILD_DIR" && {runner})')
        elif command_runs_from_build_dir(raw, default_cd_build):
            lines.append(f'    (cd "$BUILD_DIR" && {runner})')
        else:
            lines.append(f'    (cd "$CASE_DIR" && {runner})')
    return lines


def command_runs_from_build_dir(command: str, default_cd_build: bool = False) -> bool:
    if command.startswith("python3 ../"):
        return True
    if 'python3 ../' in command and ("--application=" in command or command.startswith("msprof ")):
        return True
    return default_cd_build and command.startswith("./") and not command.startswith("./build/")


def rewrite_runtime_cmake_options(command: str) -> str:
    if not command.startswith("cmake ") or not is_cmake_configure(command):
        return command
    command = re.sub(
        r"-DCMAKE_ASC_ARCHITECTURES=(?:'[^']*'|\"[^\"]*\"|[^ \t;]+)",
        '-DCMAKE_ASC_ARCHITECTURES="$ARCH"',
        command,
    )
    command = re.sub(
        r"-DCMAKE_ASC_RUN_MODE=(?:'[^']*'|\"[^\"]*\"|[^ \t;]+)",
        "",
        command,
    )
    if "-DCMAKE_ASC_ARCHITECTURES=" not in command:
        command = f'{command} -DCMAKE_ASC_ARCHITECTURES="$ARCH"'
    return f"{command} $RUN_MODE_ARG"


def rewrite_runtime_arch_options(command: str) -> str:
    command = re.sub(
        r"--npu-arch=(?:'[^']*'|\"[^\"]*\"|\$[A-Za-z_][A-Za-z0-9_]*|\$\{[^}]+}|[^ \t;]+)",
        "--npu-arch=$ARCH",
        command,
    )
    command = re.sub(
        r"-DNPU_ARCH=(?:'[^']*'|\"[^\"]*\"|\$[A-Za-z_][A-Za-z0-9_]*|\$\{[^}]+}|[^ \t;]+)",
        '-DNPU_ARCH="$ARCH"',
        command,
    )
    return command


def command_env_prefix(command: Command) -> str:
    env = dict(command.env)
    env.setdefault("soc_version", "$SOC_VERSION")
    for key in ARCH_ENV_KEYS:
        if key in env:
            env[key] = "$ARCH"
    if not env:
        return ""
    parts = [f"{key}={quote_env_value(str(value))}" for key, value in sorted(env.items())]
    return " ".join(parts) + " "


def quote_env_value(value: str) -> str:
    if value in {"$ARCH", "$SOC_VERSION"}:
        return value
    if re.search(r"(^|[^\\])\$[A-Za-z_{]", value):
        return value
    return shlex.quote(value)


def write_manifest(reports_root: Path, reports: List[CaseReport]) -> None:
    (reports_root / "case_runner_manifest.json").write_text(
        json.dumps([asdict(report) for report in reports], ensure_ascii=False, indent=2),
        encoding="utf-8",
    )


if __name__ == "__main__":
    raise SystemExit(main())
