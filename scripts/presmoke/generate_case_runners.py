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
import os
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
    "01_simd_cpp_api/02_features/99_",
)

CUSTOM_OP_PACKAGE_CASE = "01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/custom_op"
PARALLEL_OPS_PACKAGE_CASE = "01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/parallel_ops_package"
TILING_SINK_PROGRAMMING_CASE = "04_aicpu/02_features/00_framework/00_pytorch/tiling_sink_programming"
TILING_SINK_GENERATE_TASK_PATTERN = (
    "GenerateTaskForSinkOp:Node [AddCustomTilingSink, AddCustomTilingSink] starts to generate tasks "
    "for the tiling sink, sk_flag [0]."
)

CUSTOM_OP_PACKAGE_DEPENDENTS = {
    "01_simd_cpp_api/02_features/99_acl_based/01_acl_invocation/aclnn_invocation",
    "01_simd_cpp_api/02_features/99_acl_based/01_acl_invocation/aclop_invocation",
    "01_simd_cpp_api/02_features/00_framework/02_onnx/onnx_plugin",
    TILING_SINK_PROGRAMMING_CASE,
}

# These projects either build host-only apps, superbuild subprojects, or C API
# samples with their own explicit --npu-arch option. Passing
# CMAKE_ASC_ARCHITECTURES to their top-level CMake only produces an unused
# variable warning.
NO_CMAKE_ARCH_INJECTION_CASES = {
    "01_simd_cpp_api/02_features/00_framework/02_onnx/onnx_plugin",
    "01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/custom_op_static_lib",
    "01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/parallel_ops_package",
    "01_simd_cpp_api/02_features/99_acl_based/01_acl_invocation/aclnn_invocation",
    "01_simd_cpp_api/02_features/99_acl_based/01_acl_invocation/aclop_invocation",
    "02_simd_c_api/00_introduction/00_quickstart/hello_world_npu",
    "02_simd_c_api/00_introduction/01_add/c_api_delicacy_async_add",
    "02_simd_c_api/00_introduction/01_add/c_api_sync_add",
    "02_simd_c_api/02_features/03_c_api/00_vector_compute/00_sync_add",
    "02_simd_c_api/02_features/03_c_api/00_vector_compute/01_async_add",
    "02_simd_c_api/02_features/03_c_api/00_vector_compute/02_c_api_delicacy_async_add",
}

TENSORFLOW_SKIP_REASON = "requires TensorFlow 2.6.5 environment; skipped by presmoke"
MATMUL_L2CACHE_SKIP_REASON = "data size (~308M) causes overtime in cpu mode; cpu mode skipped by presmoke"
LONG_TIME_REASON = "The execution time in CPU mode is too long and needs to be skipped"


@dataclass
class SkipConfig:
    reason: str = ""
    modes: List[str] = field(default_factory=list)


SKIP_CONFIG = {
    "01_simd_cpp_api/02_features/00_framework/01_tensorflow/tensorflow_builtin": SkipConfig(
        reason=TENSORFLOW_SKIP_REASON,
        modes=["npu"],
    ),
    "01_simd_cpp_api/02_features/00_framework/01_tensorflow/tensorflow_custom": SkipConfig(
        reason=TENSORFLOW_SKIP_REASON,
        modes=["npu"],
    ),
    "01_simd_cpp_api/04_advanced_api/00_matmul/matmul_l2cache": SkipConfig(
        reason=MATMUL_L2CACHE_SKIP_REASON,
        modes=["cpu"],
    ),
        "01_simd_cpp_api/03_basic_api/05_sync_control/mutex": SkipConfig(
        reason=LONG_TIME_REASON,
        modes=["cpu"],
    ),
        "01_simd_cpp_api/05_best_practices/00_vector_compute/add_high_performance": SkipConfig(
        reason=LONG_TIME_REASON,
        modes=["cpu"],
    ),
        "01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_eltwise_high_performance": SkipConfig(
        reason=LONG_TIME_REASON,
        modes=["cpu"],
    ),
        "01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_high_performance": SkipConfig(
        reason=LONG_TIME_REASON,
        modes=["cpu"],
    ),
        "01_simd_cpp_api/05_best_practices/04_memory_access/bank_conflict_nd2nz": SkipConfig(
        reason=LONG_TIME_REASON,
        modes=["cpu"],
    ),
}

ARCH_OVERRIDES = {
    "01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/custom_op": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/parallel_ops_package": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/custom_op_static_lib": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/99_acl_based/01_acl_invocation/aclnn_invocation": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/99_acl_based/01_acl_invocation/aclop_invocation": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/00_framework/01_tensorflow/tensorflow_builtin": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/00_framework/01_tensorflow/tensorflow_custom": ["dav-2201", "dav-3510"],
    "01_simd_cpp_api/02_features/00_framework/02_onnx/onnx_plugin": ["dav-2201", "dav-3510"],
    "04_aicpu/02_features/00_framework/00_pytorch/tiling_sink_programming": ["dav-2201", "dav-3510"],
    # README "支持的产品" only lists Ascend 950PR/950DT. The option table also
    # mentions dav-2201, but the sample uses FP8/hifloat8 types unavailable on 910B.
    "01_simd_cpp_api/04_advanced_api/00_matmul/matmul_fp8": ["dav-3510"],
}

SCENARIO_NUM_MAP: dict[str, str] = {
    "01_simd_cpp_api/02_features/02_tiling_selector/vector_tiling_strategy": "0 1 2 3",
    "01_simd_cpp_api/03_basic_api/00_data_movement/copy_in_tensor_api": "0 1 2 3",
    "01_simd_cpp_api/03_basic_api/00_data_movement/copy_out_tensor_api": "0 1 2",
    "01_simd_cpp_api/03_basic_api/00_data_movement/copy_ub2ub": "1 2 3",
    "01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2ub_nddma": "1 2 3 4 5",
    "01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_store_mask": "1 2",
    "01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_store_unalign": "1 2",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/cast": "0 1",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_compound_compute": "1 2",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/gather": "1 2 3 4",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/interleave_pair": "1 2",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/mrg_sort": "1 2 3",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/reduce_computation": "1 2 3 4 5 6",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/reduce_data_block": "1 2 3",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/reduce_repeat": "1 2 3 4",
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/transpose": "1 2 3",
    "01_simd_cpp_api/03_basic_api/02_reg_vector_compute/cast": "1 2",
    "01_simd_cpp_api/03_basic_api/02_reg_vector_compute/compare": "1 2",
    "01_simd_cpp_api/03_basic_api/02_reg_vector_compute/data_relayout": "1 2",
    "01_simd_cpp_api/03_basic_api/02_reg_vector_compute/subnormal": "1 2",
    "01_simd_cpp_api/03_basic_api/02_reg_vector_compute/trait": "1 2",
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/fixpipe_l0c2l1": "1 2 3 4",
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/fixpipe_l0c2ub": "1 2 3 4",
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/load_data_2dmx_l12l0": "1 2 3 4 5 6",
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/load_data_2dv2_l12l0": "1 2 3 4 5 6 7",
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/load_data_l12l0": "1 2 3 4 5 6 7 8 9 10 11 12 13 14",
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_load3dv2": "1 2 3 4 5",
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_mx": "1 2 3 4",
    "01_simd_cpp_api/03_basic_api/04_memory_management/tbufpool_management": "1 2 3",
    "01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag": "0 1 2",
    "01_simd_cpp_api/03_basic_api/05_sync_control/reg_sync": "1 2",
    "01_simd_cpp_api/03_basic_api/06_atomic/data_movement_with_atomic_operations": "1 2",
    "01_simd_cpp_api/03_basic_api/06_atomic/scalar_atomic_operations": "1 2",
    "01_simd_cpp_api/03_basic_api/09_utils/get_ub_size": "1 2",
    "01_simd_cpp_api/04_advanced_api/00_matmul/matmul_fp8": "0 1 2 3 4",
    "01_simd_cpp_api/04_advanced_api/00_matmul/matmul_ibshareAB": "1 2",
    "01_simd_cpp_api/04_advanced_api/00_matmul/matmul_ibshareB": "1 2",
    "01_simd_cpp_api/04_advanced_api/08_transpose/transdata": "1 2 3 4",
    "01_simd_cpp_api/04_advanced_api/10_math/clampmin": "0 1",
    "01_simd_cpp_api/04_advanced_api/10_math/power": "0 1 2",
    "01_simd_cpp_api/05_best_practices/00_vector_compute/add_high_performance": "0 1 2 3 4 5 6",
    "01_simd_cpp_api/05_best_practices/01_matrix_compute/matmul_basic_api_high_performance": "1 2",
    "01_simd_cpp_api/05_best_practices/01_matrix_compute/matmul_high_performance": "0 1 2 3 4 5 6 7 8",
    "01_simd_cpp_api/05_best_practices/01_matrix_compute/matmul_mxfp4_high_performance": "1 2",
    "01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_eltwise_high_performance": "0 1 2 3",
    "01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_high_performance": "0 1 2",
    "01_simd_cpp_api/05_best_practices/03_fusion_compute/quant_group_matmul_high_performance": "0 1 2",
    "01_simd_cpp_api/05_best_practices/04_memory_access/bank_conflict_nd2nz": "1 2",
    "01_simd_cpp_api/05_best_practices/04_memory_access/data_copy": "1 2 3 4 5 6 7 8",
    "03_simt_api/03_best_practices/00_memory_optimizations/cache_hint": "0 1",
    "03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice": "0 1 2 3",
    "03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add": "1 2",
    "03_simt_api/03_best_practices/01_execution_conf_optimizations/grid_dim_config": "1 2 3 4 5 6 7 8 9 10 11 12 13",
    # Upstream per-scenario runner cases (not in original MAP)
    "01_simd_cpp_api/02_features/00_framework/04_aclgraph": "2",
    "01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_l0c2gm": "2 3 4 5 6",
    "01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_gather": "2",
    "01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_store_align": "2 3 4 5 6",
    "01_simd_cpp_api/03_basic_api/00_data_movement/reg_move": "2",
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/load_data_with_stride": "2 3 4 5 6",
    "01_simd_cpp_api/03_basic_api/04_memory_management/tensor_allocation": "2 3 4",
    "01_simd_cpp_api/05_best_practices/02_reg_compute/softmax_high_performance": "0 1 2 3 4",
    "01_simd_cpp_api/05_best_practices/04_memory_access/bank_conflict_ub": "2 3 4 5 6 7 8",
    "01_simd_cpp_api/06_compatibility_guide/pattern_transformation": "2",
    "03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence": "2",
    "03_simt_api/03_best_practices/02_control_flow/warp_divergence": "2",
    "05_simd_simt_hybrid/02_best_practices/simd_simt_grid_dim_config": "2 3 4 5 6 7 8 9 10 11 12 13 14 15 16",
    "05_simd_simt_hybrid/02_best_practices/simd_simt_high_performance": "0 2 3",
    "05_simd_simt_hybrid/02_best_practices/simd_simt_integer_fast_div": "1",
    "05_simd_simt_hybrid/02_best_practices/simd_simt_matrix_transpose": "1",
}

SCENARIO_NUM_ARCH_MAP: dict[str, dict[str, str]] = {
    "01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2l1": {
        "dav-3510": "1 2 3 4 5",
        "default": "1 2 4 5",
    },
    "01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm": {
        "dav-3510": "1 2 3 4 5 6",
        "default": "1 2",
    },
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/compare": {
        "dav-3510": "1 2 3 4",
        "default": "1 2 3",
    },
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_logic": {
        "dav-3510": "1 2 3 4",
        "default": "1 4",
    },
    "01_simd_cpp_api/03_basic_api/01_memory_vector_compute/select": {
        "dav-3510": "1 2 3 4",
        "default": "1 2 3",
    },
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/fixpipe_l0c2gm": {
        "dav-3510": "1 2 3 4 5 6 7",
        "default": "1 2 4 5 6 7",
    },
    "01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad": {
        "dav-2201": "1 2 3 4",
        "default": "1 2 3",
    },
    "01_simd_cpp_api/05_best_practices/03_fusion_compute/matmul_gelu_high_performance": {
        "dav-3510": "1 2",
        "default": "1",
    },
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
    skip_modes: List[str] = field(default_factory=list)
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
    skip_modes: List[str] = field(default_factory=list)
    arch: str = ""
    scenarios: str = ""



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

    remove_stale_runners(runners_root, reports)
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
    confidence, reasons = runner_confidence(cell, runnable_on_target)
    render_spec, confidence = build_runner_render_spec(cell, confidence, reasons)
    scenarios = get_all_scenario_values(rel)
    if scenarios:
        # Multi-scenario case: generate per-scenario runners (__scenario_N/run.sh)
        # Skip the first scenario (it's the default, handled by main run.sh)
        scenario_nums = scenarios.split()
        examples_root = project_root / "examples"
        for scenario_num in scenario_nums[1:]:
            scenario_dir = runners_root / f"{rel}__scenario_{scenario_num}"
            scenario_dir.mkdir(parents=True, exist_ok=True)
            runner = scenario_dir / "run.sh"
            if runner.exists():
                continue
            text = render_per_scenario_runner(render_spec, scenario_num)
            runner.write_text(text, encoding="utf-8")
            runner.chmod(0o755)
        # Ensure examples/ symlinks for each scenario, regardless of
        # whether the runner already existed (needed by presmoke driver)
        for scenario_num in scenario_nums[1:]:
            example_scenario = examples_root / f"{rel}__scenario_{scenario_num}"
            if not example_scenario.exists():
                example_scenario.parent.mkdir(parents=True, exist_ok=True)
                target = os.path.relpath(
                    examples_root / rel, example_scenario.parent
                )
                example_scenario.symlink_to(target)
        # Report uses the main runner (which already exists from upstream)
        runner_path = runner_dir / "run.sh"
    else:
        # Single-scenario case: generate main run.sh
        runner_dir.mkdir(parents=True, exist_ok=True)
        runner = runner_dir / "run.sh"
        text = render_runner(render_spec)
        runner.write_text(text, encoding="utf-8")
        runner.chmod(0o755)
        runner_path = runner
    skip_reason = render_spec.skip_reason
    skip_modes = render_spec.skip_modes
    all_modes_skipped = bool(skip_reason and all(mode in skip_modes for mode in cell.example.modes))
    return CaseReport(
        case=rel,
        runner=runner_path.relative_to(project_root).as_posix(),
        source=cell.example.source,
        confidence=confidence,
        supported_archs=cell.example.archs,
        supported_modes=cell.example.modes,
        target_runnable=runnable_on_target,
        skip=all_modes_skipped,
        skip_reason=skip_reason,
        skip_modes=skip_modes,
        reasons=reasons,
        commands=[command.raw for command in cell.commands if command.raw],
    )


def runner_confidence(cell, runnable_on_target: bool) -> tuple[str, List[str]]:
    confidence, reasons = classify_confidence(cell)
    if not runnable_on_target:
        reasons.append("unsupported_on_target_arch_or_mode")
        confidence = downgrade(confidence)
    return confidence, reasons


@dataclass
class SkipStrategy:
    confidence: str
    skip_reason: str
    skip_modes: List[str]
    noop_needed: bool
    custom_op_package_case: bool
    custom_op_dependency: bool


def _resolve_skip_strategy(
    rel: str, modes: List[str], reasons: List[str], confidence: str
) -> SkipStrategy:
    custom_op_package_case = rel == CUSTOM_OP_PACKAGE_CASE
    custom_op_dependency = requires_custom_op_package(rel)
    skip_reason, skip_modes = explicit_skip_config(rel)
    all_modes_skipped = skip_reason and all(mode in skip_modes for mode in modes)
    noop_needed = all_modes_skipped or custom_op_package_case
    if all_modes_skipped:
        reasons.append("explicit_skip")
        confidence = "high"
    elif skip_reason:
        reasons.append("partial_mode_skip")
        confidence = "high"
    elif custom_op_package_case:
        reasons.append("custom_op_package_provider")
    elif custom_op_dependency:
        reasons.append("requires_custom_op_package")
    return SkipStrategy(
        confidence=confidence,
        skip_reason=skip_reason,
        skip_modes=skip_modes,
        noop_needed=noop_needed,
        custom_op_package_case=custom_op_package_case,
        custom_op_dependency=custom_op_dependency,
    )


def _auto_append_scripts(cell, build_cmds: List[Command], verify_cmds: List[Command]) -> List[Command]:
    scripts_dir = cell.example.path / "scripts"
    has_gen_data = any(c.kind == "gen_data" for c in build_cmds)
    if (scripts_dir / "gen_data.py").exists() and not has_gen_data:
        build_cmds.append(Command("python3 ../scripts/gen_data.py", "gen_data"))
    has_verify = any(c.kind == "verify" for c in verify_cmds)
    if (scripts_dir / "verify_result.py").exists() and not has_verify:
        verify_cmds.append(Command("python3 ../scripts/verify_result.py output/output.bin output/golden.bin", "verify"))
    elif not has_verify:
        verify_cmds[:] = [Command(":", "verify")]
    return verify_cmds


def build_runner_render_spec(cell, confidence: str, reasons: List[str]) -> tuple[RunnerRenderSpec, str]:
    rel = cell.example.rel_path
    build_cmds = build_commands_for_runner(cell.commands)
    run_cmds = run_commands_for_runner(cell.commands, build_cmds)
    build_cmds, run_cmds = merge_export_commands_into_run(build_cmds, run_cmds)
    verify_cmds = [command for command in cell.commands if command.raw and command.kind == "verify"]
    if not run_cmds:
        run_cmds = [Command(":", "run")]
        if "no_run_step" not in reasons:
            reasons.append("no_run_step")
            confidence = downgrade(confidence)
    if not verify_cmds:
        verify_cmds = [Command(":", "verify")]
    s = _resolve_skip_strategy(rel, cell.example.modes, reasons, confidence)
    if s.noop_needed:
        build_cmds, run_cmds, verify_cmds = [Command(":", "build")], [Command(":", "run")], [Command(":", "verify")]
    else:
        verify_cmds = _auto_append_scripts(cell, build_cmds, verify_cmds)
    arch = cell.arch
    scenarios = get_scenario_values(rel, arch)
    return (
        RunnerRenderSpec(
            rel=rel,
            build_cmds=build_cmds,
            run_cmds=run_cmds,
            verify_cmds=verify_cmds,
            custom_op_dependency=s.custom_op_dependency,
            custom_op_package_case=s.custom_op_package_case,
            skip_reason=s.skip_reason,
            skip_modes=s.skip_modes,
            arch=arch,
            scenarios=scenarios,
        ),
        s.confidence,
    )


def noop_stage_commands() -> tuple[List[Command], List[Command], List[Command]]:
    return [Command(":", "build")], [Command(":", "run")], [Command(":", "verify")]


def build_commands_for_runner(commands: Iterable[Command]) -> List[Command]:
    command_list = list(commands)
    result: List[Command] = []
    for command in command_list:
        if not command.raw:
            continue
        if command.kind in {"cmake", "make", "gen_data", "shell"}:
            result.append(command)
            continue
        if is_atc_prerequisite_command(command, command_list):
            result.append(command)
    return result


def run_commands_for_runner(commands: Iterable[Command], build_cmds: List[Command]) -> List[Command]:
    result: List[Command] = []
    for command in commands:
        if not command.raw or command.kind not in {"run", "package_run"}:
            continue
        if command not in build_cmds:
            result.append(command)
    return result


def merge_export_commands_into_run(
    build_cmds: List[Command],
    run_cmds: List[Command],
) -> tuple[List[Command], List[Command]]:
    export_cmds: List[str] = []
    remaining_build_cmds: List[Command] = []
    for command in build_cmds:
        if is_export_command(command.raw):
            export_cmds.append(command.raw)
        else:
            remaining_build_cmds.append(command)
    if not export_cmds or not run_cmds:
        return build_cmds, run_cmds

    first_run = run_cmds[0]
    merged_run = Command("; ".join([*export_cmds, first_run.raw]), first_run.kind, dict(first_run.env))
    return remaining_build_cmds, [merged_run, *run_cmds[1:]]


def is_export_command(command: str) -> bool:
    return bool(re.match(r"^export\s+[A-Za-z_][A-Za-z0-9_]*=", command.strip()))


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
    if "/00_framework/01_tensorflow/" in rel:
        return True
    return False


def explicit_skip_config(rel: str) -> tuple[str, List[str]]:
    config = SKIP_CONFIG.get(rel, SkipConfig())
    return config.reason, config.modes


def has_scenario(rel: str) -> bool:
    return rel in SCENARIO_NUM_MAP or rel in SCENARIO_NUM_ARCH_MAP


def get_scenario_values(rel: str, arch: str) -> str:
    if rel in SCENARIO_NUM_ARCH_MAP:
        arch_map = SCENARIO_NUM_ARCH_MAP[rel]
        return arch_map.get(arch, arch_map.get("default", "1"))
    if rel in SCENARIO_NUM_MAP:
        return SCENARIO_NUM_MAP[rel]
    return ""


def get_all_scenario_values(rel: str) -> str:
    """Return union of all scenario values across all architectures."""
    if rel in SCENARIO_NUM_ARCH_MAP:
        arch_map = SCENARIO_NUM_ARCH_MAP[rel]
        all_nums = set()
        for nums in arch_map.values():
            all_nums.update(nums.split())
        return " ".join(sorted(all_nums, key=int))
    if rel in SCENARIO_NUM_MAP:
        return SCENARIO_NUM_MAP[rel]
    return ""


def render_runner(
    spec: RunnerRenderSpec,
) -> str:
    if spec.rel == PARALLEL_OPS_PACKAGE_CASE:
        return render_parallel_ops_package_runner(spec)
    if spec.rel == TILING_SINK_PROGRAMMING_CASE:
        return render_tiling_sink_programming_runner(spec)
    build_prefix = custom_op_guard(spec.custom_op_dependency, spec.skip_reason)
    run_prefix = custom_op_guard(spec.custom_op_package_case, spec.skip_reason)
    inject_cmake_arch = spec.rel not in NO_CMAKE_ARCH_INJECTION_CASES
    lines = [
        *runner_header(spec),
        *runner_function(
            "case_build",
            [*build_prefix, *indent_commands(spec.build_cmds, inject_cmake_arch=inject_cmake_arch)],
        ),
        *runner_function("case_run", [*run_prefix, *indent_commands(spec.run_cmds, default_cd_build=True)]),
        *runner_function("case_verify", indent_commands(spec.verify_cmds, default_cd_build=True)),
        "case_clean() {",
        "    presmoke_default_clean",
        "}",
        "",
        'presmoke_case_main "$@"',
        "",
    ]
    return "\n".join(lines)


def rewrite_scenario_number(command: str, scenario_num: str) -> str:
    """Replace scenario number placeholders with a concrete scenario number."""
    command = re.sub(
        r"-DSCENARIO_NUM=(?:'[^']*'|\"[^\"]*\"|\$[A-Za-z_][A-Za-z0-9_]*|\$\{[^}]+}|[^ \t;]+)",
        f"-DSCENARIO_NUM={scenario_num}",
        command,
    )
    command = re.sub(
        r"-scenarioNum=(?:'[^']*'|\"[^\"]*\"|\$[A-Za-z_][A-Za-z0-9_]*|\$\{[^}]+}|[^ \t;]+)",
        f"-scenarioNum={scenario_num}",
        command,
    )
    return command


def indent_commands_for_scenario(
    commands: Iterable[Command],
    scenario_num: str,
    default_cd_build: bool = False,
    inject_cmake_arch: bool = True,
) -> List[str]:
    lines: List[str] = []
    for command in commands:
        if isinstance(command, str):
            command = Command(command, "shell")
        raw = command.raw
        if not raw or raw == ":":
            lines.append("    :")
            continue
        raw = rewrite_runtime_arch_options(raw)
        raw = rewrite_runtime_cmake_options(raw, inject_cmake_arch)
        raw = rewrite_scenario_number(raw, scenario_num)
        quoted = shlex.quote(raw)
        env_prefix = command_env_prefix(command)
        # Rewrite scenario env vars to the concrete number
        env_prefix = re.sub(r'\bSCENARIO_NUM=\S+', f'SCENARIO_NUM={scenario_num}', env_prefix)
        env_prefix = re.sub(r'\bSCENARIO=\S+', f'SCENARIO={scenario_num}', env_prefix)
        if 'SCENARIO_NUM=' not in env_prefix:
            env_prefix = f'SCENARIO_NUM={scenario_num} {env_prefix}'
        runner = f"{env_prefix}bash -lc {quoted}"
        lines.append(f'    (cd "{command_workdir(raw, default_cd_build)}" && {runner})')
    return lines


def render_per_scenario_runner(spec: RunnerRenderSpec, scenario_num: str) -> str:
    """Generate a per-scenario runner (case__scenario_N/run.sh) with hardcoded SCENARIO_NUM."""
    build_prefix = custom_op_guard(spec.custom_op_dependency, spec.skip_reason)
    run_prefix = custom_op_guard(spec.custom_op_package_case, spec.skip_reason)
    inject_cmake_arch = spec.rel not in NO_CMAKE_ARCH_INJECTION_CASES
    lines = [
        *runner_header(spec),
        f'BUILD_DIR="$CASE_DIR/build_${{MODE}}_scenario_{scenario_num}"',
        "export BUILD_DIR",
        "",
        *runner_function(
            "case_build",
            [
                *build_prefix,
                *indent_commands_for_scenario(
                    spec.build_cmds, scenario_num, inject_cmake_arch=inject_cmake_arch
                ),
            ],
        ),
        *runner_function(
            "case_run",
            [*run_prefix, *indent_commands_for_scenario(spec.run_cmds, scenario_num, default_cd_build=True)],
        ),
        *runner_function(
            "case_verify",
            indent_commands_for_scenario(spec.verify_cmds, scenario_num, default_cd_build=True),
        ),
        "case_clean() {",
        "    presmoke_default_clean",
        "}",
        "",
        'presmoke_case_main "$@"',
        "",
    ]
    return "\n".join(lines)


def render_tiling_sink_programming_runner(spec: RunnerRenderSpec) -> str:
    build_prefix = custom_op_guard(spec.custom_op_dependency, spec.skip_reason)
    lines = [
        *runner_header(spec),
        *runner_function("case_build", [*build_prefix, *indent_commands(spec.build_cmds)]),
        *runner_function(
            "case_run",
            [
                "    presmoke_clear_plog",
                *indent_tiling_sink_run_commands(spec.run_cmds),
                f"    presmoke_verify_tiling_sink_task_log {shlex.quote(TILING_SINK_GENERATE_TASK_PATTERN)}",
            ],
        ),
        *runner_function("case_verify", indent_commands(spec.verify_cmds, default_cd_build=True)),
        "case_clean() {",
        "    presmoke_default_clean",
        "}",
        "",
        'presmoke_case_main "$@"',
        "",
    ]
    return "\n".join(lines)


def render_parallel_ops_package_runner(spec: RunnerRenderSpec) -> str:
    lines = [
        *runner_header(spec),
        "case_build() {",
        "    local cmake_args=(cmake -S . -B build)",
        '    if [[ -n "${NLOHMANN_JSON_URL:-}" ]]; then',
        '        cmake_args+=("-DNLOHMANN_JSON_URL=$NLOHMANN_JSON_URL")',
        "    fi",
        '    if [[ -n "${RUN_MODE_ARG:-}" ]]; then',
        '        cmake_args+=("$RUN_MODE_ARG")',
        "    fi",
        "",
        '    mkdir -p "$BUILD_DIR"',
        '    (cd "$CASE_DIR" && soc_version=$SOC_VERSION presmoke_run_command "${cmake_args[@]}")',
        '    (cd "$CASE_DIR" && soc_version=$SOC_VERSION presmoke_run_command cmake --build build -j)',
        "}",
        "",
        *runner_function("case_run", indent_commands(spec.run_cmds, default_cd_build=True)),
        *runner_function("case_verify", indent_commands(spec.verify_cmds, default_cd_build=True)),
        "case_clean() {",
        '    rm -rf "$CASE_DIR/build" "$BUILD_DIR"',
        "}",
        "",
        'presmoke_case_main "$@"',
        "",
    ]
    return "\n".join(lines)


def custom_op_guard(enabled: bool, skip_reason: str) -> List[str]:
    return ['    presmoke_ensure_custom_op_package'] if enabled and not skip_reason else []


def runner_header(spec: RunnerRenderSpec) -> List[str]:
    return [
        *BASH_LICENSE_HEADER,
        "",
        "set -euo pipefail",
        "",
        f'CASE_REL={shlex.quote(spec.rel)}',
        *skip_reason_lines(spec.skip_reason),
        *skip_modes_lines(spec.skip_modes),
        'SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"',
        f'source "$SCRIPT_DIR/{case_entry_relative_path(spec.rel)}"',
        'presmoke_case_init "$CASE_REL"',
        "",
    ]


def skip_reason_lines(skip_reason: str) -> List[str]:
    if not skip_reason:
        return []
    return [f"SKIP_REASON={shlex.quote(skip_reason)}", ""]


def skip_modes_lines(skip_modes: List[str]) -> List[str]:
    if not skip_modes:
        return []
    modes_str = " ".join(skip_modes)
    return [f"SKIP_MODES=({modes_str})", ""]


def runner_function(name: str, body: List[str]) -> List[str]:
    return [
        f"{name}() {{",
        '    mkdir -p "$BUILD_DIR"',
        *body,
        "}",
        "",
    ]


def case_entry_relative_path(rel: str) -> str:
    depth = len(Path(rel).parts)
    return "../" * depth + "_case_entry.sh"


def indent_commands(
    commands: Iterable[Command],
    default_cd_build: bool = False,
    inject_cmake_arch: bool = True,
) -> List[str]:
    lines: List[str] = []
    for command in commands:
        if isinstance(command, str):
            command = Command(command, "shell")
        lines.append(indent_command(command, default_cd_build, inject_cmake_arch))
    return lines


def indent_tiling_sink_run_commands(commands: Iterable[Command]) -> List[str]:
    lines: List[str] = []
    for command in commands:
        raw = rewrite_runtime_arch_options(command.raw)
        raw = rewrite_runtime_cmake_options(raw)
        quoted = shlex.quote(raw)
        env_prefix = command_env_prefix(command)
        runner = f"ASCEND_GLOBAL_LOG_LEVEL=1 {env_prefix}bash -lc {quoted}"
        lines.append(f'    (cd "{command_workdir(raw, default_cd_build=True)}" && {runner})')
    return lines


def indent_command(command: Command, default_cd_build: bool = False, inject_cmake_arch: bool = True) -> str:
    raw = command.raw
    if not raw or raw == ":":
        return "    :"
    raw = rewrite_runtime_arch_options(raw)
    raw = rewrite_runtime_cmake_options(raw, inject_cmake_arch)
    quoted = shlex.quote(raw)
    env_prefix = command_env_prefix(command)
    runner = f"{env_prefix}bash -lc {quoted}"
    return f'    (cd "{command_workdir(raw, default_cd_build)}" && {runner})'


def command_workdir(command: str, default_cd_build: bool = False) -> str:
    if command.startswith("cmake "):
        if is_cmake_configure(command) and (" .." in command or command.endswith("..")):
            return "$BUILD_DIR"
        return "$CASE_DIR"
    if command.startswith("make") or command.startswith("atc "):
        return "$BUILD_DIR"
    if command_runs_from_build_dir(command, default_cd_build):
        return "$BUILD_DIR"
    return "$CASE_DIR"


def command_runs_from_build_dir(command: str, default_cd_build: bool = False) -> bool:
    if re.match(r"^(export\s+[A-Za-z_][A-Za-z0-9_]*=.*;\s*)+\./", command.strip()):
        return True
    if command.startswith("python3 ../"):
        return True
    if 'python3 ../' in command and ("--application=" in command or command.startswith("msprof ")):
        return True
    return default_cd_build and command.startswith("./") and not command.startswith("./build/")


def rewrite_runtime_cmake_options(command: str, inject_cmake_arch: bool = True) -> str:
    if not command.startswith("cmake ") or not is_cmake_configure(command):
        return command
    command = re.sub(
        r"-DCMAKE_ASC_ARCHITECTURES=(?:'[^']*'|\"[^\"]*\"|[^ \t;]+)",
        '-DCMAKE_ASC_ARCHITECTURES="$ARCH"' if inject_cmake_arch else "",
        command,
    )
    command = re.sub(
        r"-DCMAKE_ASC_RUN_MODE=(?:'[^']*'|\"[^\"]*\"|[^ \t;]+)",
        "",
        command,
    )
    command = re.sub(r"[ \t]+", " ", command).strip()
    if inject_cmake_arch and "-DCMAKE_ASC_ARCHITECTURES=" not in command:
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


def remove_stale_runners(runners_root: Path, reports: List[CaseReport]) -> None:
    expected_cases = {item.case for item in reports}
    multi_scenario_cases = {c for c in expected_cases if has_scenario(c)}
    examples_root = runners_root.parent.parent.parent / "examples"
    stale_links: List[Path] = []
    for runner in list(runners_root.rglob("run.sh")):
        rel = runner.parent.relative_to(runners_root).as_posix()
        scenario_match = re.match(r'^(.+)__scenario_(\d+)$', rel)
        if scenario_match:
            base_case = scenario_match.group(1)
            if base_case in multi_scenario_cases:
                continue
            LOG.info("remove_stale_runner=%s", rel)
            runner.unlink()
            remove_empty_parents(runner.parent, runners_root)
            stale_links.append(examples_root / rel)
        else:
            if rel in expected_cases:
                continue
            LOG.info("remove_stale_runner=%s", rel)
            runner.unlink()
            remove_empty_parents(runner.parent, runners_root)
            stale_links.append(examples_root / rel)
    # Clean up stale examples/ symlinks
    for link in stale_links:
        if link.is_symlink():
            LOG.info("remove_stale_example_symlink=%s", link.relative_to(examples_root.parent))
            link.unlink()
            remove_empty_parents(link.parent, examples_root)


def remove_empty_parents(path: Path, stop: Path) -> None:
    while path != stop:
        try:
            path.rmdir()
        except OSError:
            return
        path = path.parent


if __name__ == "__main__":
    raise SystemExit(main())
