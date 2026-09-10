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
"""Check compatibility contracts in generated OOM compile artifacts."""

import argparse
import importlib.util
import os
import sys
from pathlib import Path


EXIT_OK = 0
EXIT_CHECK_FAILED = 1
EXIT_ENVIRONMENT_ERROR = 2


def parse_args():
    default_devkit = Path(__file__).resolve().parents[6]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--devkit-path", type=Path, default=default_devkit)
    parser.add_argument(
        "--cann-path",
        type=Path,
        default=os.environ.get("ASCEND_HOME_PATH"),
    )
    return parser.parse_args()


def validate_environment(devkit_path, cann_path):
    required_paths = (
        devkit_path
        / "tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py",
        devkit_path / "tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py",
    )
    if cann_path is None:
        raise ValueError("CANN path is required; use --cann-path or ASCEND_HOME_PATH")
    required_paths += (
        cann_path
        / "python/site-packages/asc_op_compile_base/c_api/libasc_platform.so",
    )
    missing = [str(path) for path in required_paths if not path.is_file()]
    if missing:
        raise ValueError("required path does not exist: " + ", ".join(missing))


def bootstrap_environment(cann_path):
    if os.environ.get("ASCENDC_OOM_SAMPLE_BOOTSTRAPPED") == "1":
        return

    python_path = cann_path / "python/site-packages"
    library_paths = (
        cann_path / "aarch64-linux/lib64",
        cann_path / "aarch64-linux/devlib/linux/x86_64",
    )
    environment = os.environ.copy()
    environment["PYTHONPATH"] = os.pathsep.join(
        [str(python_path), environment.get("PYTHONPATH", "")]
    ).rstrip(os.pathsep)
    environment["LD_LIBRARY_PATH"] = os.pathsep.join(
        [*(str(path) for path in library_paths), environment.get("LD_LIBRARY_PATH", "")]
    ).rstrip(os.pathsep)
    environment["ASCENDC_OOM_SAMPLE_BOOTSTRAPPED"] = "1"
    os.execve(
        sys.executable,
        [sys.executable, str(Path(__file__).resolve()), *sys.argv[1:]],
        environment,
    )


def load_repo_module(module_name, module_path):
    spec = importlib.util.spec_from_file_location(module_name, module_path)
    if spec is None or spec.loader is None:
        raise ImportError(f"cannot create an import specification for {module_path}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def tensor(name, shape):
    return {
        "shape": shape,
        "ori_shape": shape,
        "format": "ND",
        "ori_format": "ND",
        "dtype": "float32",
        "range": [(0, None)],
        "param_name": name,
    }


def make_fixtures(generator, static_shape):
    shape = [16] if static_shape else [-2]
    input_tensor = tensor("x_in__", shape)
    tensor_list = tensor("xs_in__", shape)
    output_tensor = tensor("z_out_", shape)
    op_info = generator.OpInfo(
        kernel_name="OomCompileArtifactSample",
        op_type="OomCompileArtifactSample",
        inputs=[input_tensor, tensor_list],
        outputs=[output_tensor],
        attrs=[],
        impl_mode="high_performance",
        origin_inputs=[input_tensor, tensor_list],
        origin_outputs=[output_tensor],
        param_type_dynamic=False,
        mc2_ctx=[],
        param_type_list=["required", "dynamic", "required"],
        init_value_list=[None],
        output_shape_depend_on_compute=[],
    )
    tiling_info = generator.TilingInfo()
    tiling_info.tiling_data_size = 64
    tiling_info.tiling_key_list = ["1"]
    tiling_info.tiling_key_data_size = {}
    tiling_info.static_shape_flag = static_shape
    tiling_info.static_workspace_size = 0
    compile_info = generator.CompileInfo()
    compile_info.tiling_key_struct_map = {}
    return op_info, tiling_info, compile_info


def generate_source(generator, buildcfg, static_shape, oom_enabled):
    op_info, tiling_info, compile_info = make_fixtures(generator, static_shape)
    with buildcfg.build_config() as config:
        config.current()["tir.op_debug_config"] = ["oom"] if oom_enabled else []
        return generator.add_op_param_to_workspace(
            op_info,
            tiling_info,
            "",
            ["-DDORIG_DTYPE_XS=DT_FLOAT"],
            compile_info,
        )


def report_check(name, condition, detail):
    if condition:
        print(f"PASS {name}")
        return True
    print(f"FAIL {name}: {detail}")
    return False


def check_source_artifacts(generator, buildcfg):
    dynamic_source = generate_source(generator, buildcfg, False, True)
    static_source = generate_source(generator, buildcfg, True, True)
    non_oom_source = generate_source(generator, buildcfg, False, False)

    tensor_new = "OOMTryRegisterTensorWithStorageShape"
    tensor_legacy = "OOMCheckAddrRange(x_in__"
    list_new = "OOMTryRegisterTensorListWithStorageShape"
    list_legacy = "OOMCheckTensorListRange(xs_in__"
    checks = [
        report_check(
            "dynamic tensor keeps legacy fallback",
            tensor_legacy in dynamic_source,
            "OOMCheckAddrRange fallback for x_in__ is missing",
        ),
        report_check(
            "dynamic tensor emits storage-shape registration",
            all(
                marker in dynamic_source
                for marker in (
                    "OOMHasStorageShapeHeader",
                    "oomStorageShapeCursor += 2",
                    tensor_new,
                )
            )
            and dynamic_source.index(tensor_new) < dynamic_source.index(list_new),
            "header handling or Tensor registration order is incorrect",
        ),
        report_check(
            "dynamic TensorList keeps legacy fallback",
            list_legacy in dynamic_source,
            "OOMCheckTensorListRange fallback for xs_in__ is missing",
        ),
        report_check(
            "dynamic TensorList emits storage-shape registration",
            list_new in dynamic_source,
            "TensorList storage-shape registration is missing",
        ),
        report_check(
            "static shape keeps legacy OOM generation",
            "OOMCheckAddrRange(x_in__, 64)" in static_source
            and "OOMHasStorageShapeHeader" not in static_source
            and tensor_new not in static_source
            and list_new not in static_source,
            "static OOM source unexpectedly uses storage-shape metadata",
        ),
        report_check(
            "non-OOM artifacts remain unchanged",
            non_oom_source.startswith(
                "#if defined(ASCENDC_OOM) && ASCENDC_OOM == 1\n"
            )
            and non_oom_source.endswith("#endif\n"),
            "OOM code is not fully protected by the existing compile-time guard",
        ),
    ]
    return checks


def main():
    args = parse_args()
    try:
        devkit_path = args.devkit_path.resolve()
        if args.cann_path is None:
            raise ValueError(
                "CANN path is required; use --cann-path or ASCEND_HOME_PATH"
            )
        cann_path = args.cann_path.resolve()
        validate_environment(devkit_path, cann_path)
        bootstrap_environment(cann_path)

        from asc_op_compile_base.common import buildcfg

        generator = load_repo_module(
            "asc_op_compile_base.asc_op_compiler.oom_sample_gen_code",
            devkit_path
            / "tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py",
        )
        checks = check_source_artifacts(generator, buildcfg)
    except (ImportError, OSError, ValueError) as error:
        print(f"ERROR {error}")
        return EXIT_ENVIRONMENT_ERROR

    print("FAIL OOM JSON capability and opParaSize: check is not implemented")
    checks.append(False)
    failed = checks.count(False)
    print(f"{failed} OOM compile artifact check(s) failed.")
    return EXIT_CHECK_FAILED


if __name__ == "__main__":
    raise SystemExit(main())
