#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import json
import shutil
import subprocess
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[3]
FWK_MODULES_DIR = REPO_ROOT / "cmake/asc/fwk_modules"


def test_package_add_rejects_multiple_kernel_libraries_per_call(tmp_path):
    (tmp_path / "CMakeLists.txt").write_text(
        f'''cmake_minimum_required(VERSION 3.16)
project(multiple_kernel_libraries NONE)
include("{FWK_MODULES_DIR / "func.cmake"}")
set_property(GLOBAL PROPERTY _ASC_PKG_demo_TYPE RUN)
set_property(GLOBAL PROPERTY ASCENDC_KERNEL_TARGET kernel_a kernel_b)
set_property(GLOBAL PROPERTY ASCENDC_TILING_TARGET tiling)
npu_op_package_add(demo LIBRARY kernel_a tiling kernel_b)
'''
    )
    result = subprocess.run(
        ["cmake", "-S", str(tmp_path), "-B", str(tmp_path / "build")], text=True, capture_output=True
    )
    assert result.returncode != 0
    error = " ".join(result.stderr.split())
    assert "npu_op_package_add accepts only one KERNEL library per call" in error
    assert "Received 'kernel_a' and 'kernel_b'" in error


def test_package_add_rejects_kernel_libraries_across_calls(tmp_path):
    (tmp_path / "CMakeLists.txt").write_text(
        f'''cmake_minimum_required(VERSION 3.16)
project(multiple_kernel_library_calls NONE)
include("{FWK_MODULES_DIR / "func.cmake"}")
set(ASCENDC_CMAKE_SCRIPTS_PATH "{FWK_MODULES_DIR}")
set(ASCEND_PYTHON_EXECUTABLE python3)
set(ASCEND_COMPUTE_UNIT "")
set(ASCEND_SKIP_FAILED_COMPUTE_UNIT OFF)
set(ASCENDC_AUTO_GEN_PATH "${{CMAKE_BINARY_DIR}}/autogen")
set_property(GLOBAL PROPERTY _ASC_PKG_demo_TYPE RUN)
set_property(GLOBAL PROPERTY _ASC_PKG_demo_CODE_GEN_DIRS "${{CMAKE_BINARY_DIR}}/autogen")
set_property(GLOBAL PROPERTY _ASC_PKG_demo_VENDOR_NAME demo)
set_property(GLOBAL PROPERTY ASCENDC_KERNEL_TARGET kernel_a kernel_b)
set_property(GLOBAL PROPERTY _ASC_TGT_KERNEL_kernel_a_BINARY_DIR "${{CMAKE_BINARY_DIR}}/kernel_a")
set_property(GLOBAL PROPERTY _ASC_TGT_KERNEL_kernel_b_BINARY_DIR "${{CMAKE_BINARY_DIR}}/kernel_b")
npu_op_package_add(demo LIBRARY kernel_a)
npu_op_package_add(demo LIBRARY kernel_b)
'''
    )
    result = subprocess.run(
        ["cmake", "-S", str(tmp_path), "-B", str(tmp_path / "build")], text=True, capture_output=True
    )
    assert result.returncode != 0
    error = " ".join(result.stderr.split())
    assert "accepts only one KERNEL library per package" in error
    assert "already contains 'kernel_a'" in error
    assert "cannot add 'kernel_b'" in error


@pytest.mark.skipif(shutil.which("ninja") is None, reason="Ninja is required")
@pytest.mark.parametrize("source_suffix", ["", "/"])
def test_kernel_copy_excludes_nested_build_directory(tmp_path, source_suffix):
    source = tmp_path / "add.cpp"
    source.write_text("int add() { return 0; }\n")
    (tmp_path / "CMakeLists.txt").write_text(
        f'''cmake_minimum_required(VERSION 3.16)
project(kernel_copy NONE)
include("{FWK_MODULES_DIR / "func.cmake"}")
set(ASCENDC_CMAKE_SCRIPTS_PATH "{FWK_MODULES_DIR}")
set(ASCEND_PYTHON_EXECUTABLE python3)
file(MAKE_DIRECTORY "${{CMAKE_BINARY_DIR}}/autogen")
set_property(GLOBAL PROPERTY ASCENDC_AUTO_GEN_PATH "${{CMAKE_BINARY_DIR}}/autogen")
npu_op_kernel_library(kernel SRC_BASE "${{CMAKE_CURRENT_SOURCE_DIR}}{source_suffix}" TILING_LIBRARY tiling)
'''
    )
    build = tmp_path / "build"

    def run(*arguments):
        result = subprocess.run(["cmake", *arguments], text=True, capture_output=True)
        assert result.returncode == 0, result.stdout + result.stderr

    run("-G", "Ninja", "-S", str(tmp_path), "-B", str(build))
    target = ("--build", str(build), "--target", "kernel_copy_kernel_srcs")
    run(*target)
    stamp = build / "autogen/kernel_copy_kernel_srcs.timestamp"
    copied = build / "kernel/binary/dynamic/add.cpp"
    assert copied.read_text() == source.read_text()
    copied_at = stamp.stat().st_mtime_ns
    run(*target)
    assert stamp.stat().st_mtime_ns == copied_at
    source.write_text("int add() { return 1; }\n")
    run(*target)
    assert copied.read_text() == source.read_text()


def _valid_binary_json(op_type="AddCustomTemplate"):
    return {
        "op_type": op_type,
        "op_list": [
            {
                "bin_filename": "AddCustomTemplate_FP16",
                "simplified_key": "custom_fp16",
                "inputs": [{"dtype": "float16", "format": "ND", "shape": [-2]}],
                "outputs": [{"dtype": "float16", "format": "ND", "shape": [-2]}],
            }
        ],
    }


def _configure(tmp_path, kernel_args, options="", binary_json=None):
    source_dir = tmp_path / "source"
    build_dir = tmp_path / "build"
    source_dir.mkdir()
    (source_dir / "kernel.cpp").write_text("", encoding="utf-8")
    content = _valid_binary_json() if binary_json is None else binary_json
    (source_dir / "binary.json").write_text(json.dumps(content), encoding="utf-8")
    (source_dir / "CMakeLists.txt").write_text(
        f'''cmake_minimum_required(VERSION 3.16)
project(binary_json_test NONE)
set(ASCEND_PYTHON_EXECUTABLE "python3")
set(ASCENDC_CMAKE_SCRIPTS_PATH "{FWK_MODULES_DIR.as_posix()}")
set(CUSTOM_OPC_OPTIONS "custom_opc_options.ini")
file(MAKE_DIRECTORY "${{CMAKE_BINARY_DIR}}/autogen")
file(WRITE "${{CMAKE_BINARY_DIR}}/autogen/custom_opc_options.ini" "")
set_property(GLOBAL PROPERTY ASCENDC_AUTO_GEN_PATH "${{CMAKE_BINARY_DIR}}/autogen")
include("{(FWK_MODULES_DIR / "func.cmake").as_posix()}")
npu_op_kernel_sources(ascendc_kernels {kernel_args})
{options}
''',
        encoding="utf-8",
    )
    result = subprocess.run(
        ["cmake", "-S", str(source_dir), "-B", str(build_dir)],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    return result, source_dir, build_dir


def test_kernel_json_option_writes_opc_kernel_config(tmp_path):
    result, source_dir, build_dir = _configure(
        tmp_path,
        "OP_TYPE AddCustomTemplate KERNEL_FILE kernel.cpp",
        """npu_op_kernel_options(ascendc_kernels AddCustomTemplate
    OPTIONS --kernel-json-file=${CMAKE_CURRENT_SOURCE_DIR}/binary.json
)""",
    )

    assert result.returncode == 0, result.stdout
    assert (build_dir / "autogen/custom_opc_options.ini").read_text() == (
        f"AddCustomTemplate@@--kernel-json-file={source_dir / 'binary.json'}\n"
    )


def test_kernel_sources_no_longer_handles_binary_json():
    source = (FWK_MODULES_DIR / "func.cmake").read_text(encoding="utf-8")
    kernel_sources = source.split("function(npu_op_kernel_sources target_name)", 1)[1].split("endfunction()", 1)[0]

    assert "BINARY_JSON" not in kernel_sources
    assert "--simplified_key_mode=None" not in kernel_sources


def test_without_kernel_json_option_keeps_existing_path(tmp_path):
    result, _, build_dir = _configure(tmp_path, "OP_TYPE AddCustomTemplate KERNEL_FILE kernel.cpp")

    assert result.returncode == 0, result.stdout
    assert (build_dir / "autogen/custom_opc_options.ini").read_text() == ""


def test_custom_codegen_options_follow_current_configuration(tmp_path):
    opbuild = tmp_path / "cann/tools/opbuild/op_build"
    opbuild.parent.mkdir(parents=True)
    opbuild.write_text("#!/bin/sh\nexit 0\n")
    opbuild.chmod(0o755)
    (tmp_path / "CMakeLists.txt").write_text(
        f'''cmake_minimum_required(VERSION 3.16)
project(codegen_options NONE)
include("{FWK_MODULES_DIR / "func.cmake"}")
set(ASCENDC_CMAKE_SCRIPTS_PATH "{FWK_MODULES_DIR}")
set(ASCEND_CANN_PACKAGE_PATH "{tmp_path}/cann")
set(CUSTOM_COMPILE_OPTIONS custom_compile_options.ini)
set(CUSTOM_OPC_OPTIONS custom_opc_options.ini)
set(custom_out "${{CMAKE_BINARY_DIR}}/custom autogen")
file(MAKE_DIRECTORY "${{custom_out}}")
set(ASCEND_PYTHON_EXECUTABLE /bin/true)
npu_op_code_gen(SRC unused.cpp PACKAGE demo OUT_DIR "${{custom_out}}")
set(ASCEND_PYTHON_EXECUTABLE python3)
if(OPTION_VALUE)
  npu_op_kernel_options(kernel ALL OPTIONS "-DVALUE=${{OPTION_VALUE}}" "--kernel-json-file=${{OPTION_VALUE}}.json")
endif()
set(ASCEND_PYTHON_EXECUTABLE /bin/true)
npu_op_code_gen(SRC unused.cpp PACKAGE demo OUT_DIR "${{custom_out}}/.")
'''
    )
    build = tmp_path / "build"
    for value in ("first", "second", ""):
        result = subprocess.run(
            ["cmake", "-S", str(tmp_path), "-B", str(build), f"-DOPTION_VALUE={value}"], text=True, capture_output=True
        )
        assert result.returncode == 0, result.stdout + result.stderr
        output = build / "custom autogen"
        assert (output / "custom_compile_options.ini").read_text() == (f"ALL,,-DVALUE={value}\n" if value else "")
        assert (output / "custom_opc_options.ini").read_text() == (
            f"ALL@@--kernel-json-file={value}.json\n" if value else ""
        )


@pytest.mark.parametrize("cross_compile,skip_failed", [(False, False), (False, True), (True, False), (True, True)])
@pytest.mark.parametrize("use_ccache", [False, True])
def test_kernel_spec_parameters_reach_compile_command_directly(tmp_path, cross_compile, skip_failed, use_ccache):
    scripts = tmp_path / "scripts"
    (scripts / "util").mkdir(parents=True)
    launcher = scripts / "ccache"
    launcher.write_text('#!/bin/sh\nexec "$@"\n')
    launcher.chmod(0o755)
    (scripts / "util/ascendc_compile_kernel.py").write_text(
        "import json,os,sys\nfrom pathlib import Path\n"
        "Path(__file__).with_name('arguments.json').write_text(json.dumps(\n"
        "    {'args': sys.argv[1:], 'ccache': os.environ.get('ASCENDC_CCACHE_EXECUTABLE')}))\n"
    )
    root = tmp_path / "resource root"
    (tmp_path / "stub.cpp").write_text("int stub() { return 0; }\n")
    (tmp_path / "CMakeLists.txt").write_text(
        f'''cmake_minimum_required(VERSION 3.16)
project(direct_spec_options CXX)
set(CMAKE_CXX_COMPILER_LAUNCHER "{launcher if use_ccache else ""}")
include("{FWK_MODULES_DIR / "func.cmake"}")
set(ASCENDC_CMAKE_SCRIPTS_PATH "{scripts}")
set(ASCEND_PYTHON_EXECUTABLE python3)
set(ENABLE_CROSS_COMPILE {"ON" if cross_compile else "OFF"})
set(ASCEND_SKIP_FAILED_COMPUTE_UNIT {"ON" if skip_failed else "OFF"})
set(HOST_NATIVE_TILING_LIB "${{CMAKE_BINARY_DIR}}/unused.so")
set_property(GLOBAL PROPERTY _ASC_PKG_ERROR_FILE "${{CMAKE_BINARY_DIR}}/compile_error.log")
set_property(GLOBAL PROPERTY _ASC_PKG_demo_KERNEL_SPEC_MODE SK)
set_property(GLOBAL PROPERTY _ASC_PKG_demo_KERNEL_SPEC_BUILD_DIR "{root}")
set_property(GLOBAL PROPERTY _ASC_PKG_demo_CODE_GEN_DIRS "${{CMAKE_BINARY_DIR}}/autogen")
set_property(GLOBAL PROPERTY _ASC_PKG_demo_ENABLE_BINARY_PACKAGE ON)
add_library(demo_ascendc_tiling SHARED stub.cpp)
add_custom_target(kernel_ops_info_gen_ascend910b)
simple_kernel_compile(OP_TYPE AddCustom SRC "${{CMAKE_CURRENT_SOURCE_DIR}}/add.cpp"
    COMPUTE_UNIT ascend910b TARGET_NAME kernel PACKAGE_NAME demo TILING_LIB tiling)
'''
    )
    build = tmp_path / "build"
    for command in [
        ["cmake", "-S", str(tmp_path), "-B", str(build)],
        ["cmake", "--build", str(build), "--target", "AddCustom_ascend910b"],
    ]:
        result = subprocess.run(command, text=True, capture_output=True)
        assert result.returncode == 0, result.stdout + result.stderr
    recorded = json.loads((scripts / "util/arguments.json").read_text())
    if use_ccache:
        assert recorded["ccache"] == str(launcher)
    arguments = recorded["args"]
    assert "--kernel-spec-mode=SK" in arguments
    assert f"--kernel-spec-build={root}" in arguments
