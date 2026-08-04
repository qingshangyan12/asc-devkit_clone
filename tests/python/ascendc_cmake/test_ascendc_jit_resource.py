#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import subprocess
from collections import defaultdict
from pathlib import Path
from types import SimpleNamespace

REPO_ROOT = Path(__file__).resolve().parents[3]
FUNC_CMAKE = REPO_ROOT / "cmake/asc/fwk_modules/func.cmake"


def _write(path, content="{}"):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content)


def _compile_args(tmp_path, **jit_options):
    defaults = {
        "jit_collection_root": None,
    }
    defaults.update(jit_options)
    return SimpleNamespace(
        op_type="AddCustom",
        src_file=str(tmp_path / "custom op" / "op_kernel" / "add_custom.cpp"),
        compute_unit="ascend910b",
        compile_options="",
        debug_config="",
        config_ini=str(tmp_path / "aic-ascend910b-ops-info.ini"),
        tiling_lib=str(tmp_path / "liboptiling.so"),
        output_path=str(tmp_path / "binary"),
        dynamic_dir=None,
        json_file=None,
        target_name="ascendc_kernels",
        auto_gen_path=str(tmp_path / "auto_gen"),
        **defaults
    )


def test_compile_kernel_exports_collection_root_for_compile_op(monkeypatch, tmp_path):
    import ascendc_compile_kernel

    monkeypatch.chdir(tmp_path)
    monkeypatch.delenv("ASCENDC_JIT_COLLECTION_ROOT", raising=False)
    ascendc_compile_kernel.CompileKernel(_compile_args(tmp_path))
    assert "ASCENDC_JIT_COLLECTION_ROOT" not in os.environ

    collection_root = tmp_path / "collection root"
    ascendc_compile_kernel.CompileKernel(_compile_args(
        tmp_path, jit_collection_root=str(collection_root)))
    assert os.environ["ASCENDC_JIT_COLLECTION_ROOT"] == str(collection_root.resolve())


def test_compile_kernel_does_not_preserve_kernel_meta_for_external_collection(monkeypatch, tmp_path):
    import ascendc_compile_kernel

    monkeypatch.chdir(tmp_path)
    recorded = []

    def fake_gen_bin_param_file(*args, **kwargs):
        recorded.append(kwargs)
        out_dir = Path(args[1])
        _write(out_dir / "input.json")
        _write(out_dir / "compile.sh", "#!/bin/sh\n")

    monkeypatch.setattr(
        ascendc_compile_kernel.ascendc_bin_param_build,
        "gen_bin_param_file",
        fake_gen_bin_param_file,
    )
    monkeypatch.setattr(
        ascendc_compile_kernel.ascendc_bin_param_build,
        "parse_op_debug_config",
        lambda *args: (defaultdict(set), defaultdict(set), {}, {}, {}),
    )

    kernel = ascendc_compile_kernel.CompileKernel(_compile_args(
        tmp_path, jit_collection_root="/collection"))
    kernel.ascendc_gen_param()

    assert recorded == [{}]


def test_bin_param_preserves_existing_debug_config_when_jit_keeps_kernel_meta(
    monkeypatch, tmp_path
):
    import ascendc_bin_param_build

    class FakeOpDesc:
        def __init__(self):
            self.op_type = "AddCustom"
            self.json_file = ""
            self.debug_config = set()

        def set_soc_version(self, _soc):
            pass

        def set_out_path(self, _out_dir):
            pass

        def set_op_debug_config(self, config):
            self.debug_config = set(config)

        def set_op_super_config(self, _config):
            pass

        def set_tiling_key(self, _config):
            pass

        def set_kernel_template_input(self, _config):
            pass

        def gen_input_json(self, _auto_gen_path):
            pass

    op_desc = FakeOpDesc()
    monkeypatch.setattr(
        ascendc_bin_param_build.opdesc_parser,
        "get_op_desc",
        lambda *args: [op_desc],
    )
    monkeypatch.setattr(
        ascendc_bin_param_build,
        "parse_op_debug_config",
        lambda *args: (
            defaultdict(set),
            defaultdict(set, {"AddCustom": {"ccec_g"}}),
            {},
            {},
            {},
        ),
    )
    cfg_file = tmp_path / "ops-info.ini"
    _write(cfg_file, "")

    ascendc_bin_param_build.gen_bin_param_file(
        str(cfg_file),
        str(tmp_path / "bin_param"),
        "ascend910b",
        preserve_kernel_meta=True,
    )

    assert op_desc.debug_config == {"ccec_g", "dump_cce"}


def _configure_project(tmp_path, body):
    source = tmp_path / "source"
    build = tmp_path / "build"
    source.mkdir(parents=True)
    (source / "CMakeLists.txt").write_text(
        "cmake_minimum_required(VERSION 3.16)\n"
        "project(jit_package_api NONE)\n"
        'include("{}")\n'.format(FUNC_CMAKE.as_posix()) + body,
        encoding="utf-8",
    )
    return subprocess.run(
        ["cmake", "-S", str(source), "-B", str(build)],
        text=True,
        capture_output=True,
        check=False,
    )


def _fake_generator(tmp_path, executable=True):
    generator = tmp_path / "tools with spaces" / "manifest_generator"
    _write(generator, "#!/bin/sh\nexit 0\n")
    generator.chmod(0o755 if executable else 0o644)
    return generator


def _package_call(generator=None, package_type="RUN", package="demo", vendor="customize"):
    jit_config = ""
    if generator is not None:
        jit_config = '    JIT_GENERATOR "{}"\n'.format(generator.as_posix())
    return (
        "npu_op_package({0}\n"
        "  TYPE {1}\n"
        "  CONFIG\n"
        "    VENDOR_NAME {2}\n"
        "    ENABLE_CPACK False\n"
        "{3}"
        ")\n"
    ).format(package, package_type, vendor, jit_config)


def test_jit_config_rejects_non_run_package(tmp_path):
    body = _package_call(_fake_generator(tmp_path), package_type="SHARED")
    result = _configure_project(tmp_path / "non_run", body)
    assert result.returncode != 0
    assert "only supports RUN" in result.stdout + result.stderr


def test_jit_package_rejects_missing_or_non_executable_generator(tmp_path):
    missing = tmp_path / "missing generator"
    missing_result = _configure_project(
        tmp_path / "missing", _package_call(missing)
    )
    assert missing_result.returncode != 0
    assert "does not exist" in missing_result.stdout + missing_result.stderr

    non_executable = _fake_generator(tmp_path, executable=False)
    non_executable_result = _configure_project(
        tmp_path / "non executable", _package_call(non_executable)
    )
    assert non_executable_result.returncode != 0
    assert "not executable" in non_executable_result.stdout + non_executable_result.stderr


def test_jit_package_stores_normalized_properties(tmp_path):
    generator = _fake_generator(tmp_path)
    expected_root = (tmp_path / "valid" / "build" / "jit_resources" / "demo").as_posix()
    body = _package_call(generator) + (
        "get_property(enabled GLOBAL PROPERTY _ASC_PKG_demo_ENABLE_JIT_PACKAGE)\n"
        "get_property(actual_generator GLOBAL PROPERTY _ASC_PKG_demo_JIT_GENERATOR)\n"
        "get_property(root GLOBAL PROPERTY _ASC_PKG_demo_JIT_ROOT)\n"
        "if(NOT enabled)\n"
        '  message(FATAL_ERROR "JIT package property was not enabled")\n'
        "endif()\n"
        'if(NOT actual_generator STREQUAL "{}")\n'
        '  message(FATAL_ERROR "unexpected generator: ${{actual_generator}}")\n'
        "endif()\n"
        'if(NOT root STREQUAL "{}")\n'
        '  message(FATAL_ERROR "unexpected JIT root: ${{root}}")\n'
        "endif()\n"
    ).format(generator.resolve().as_posix(), expected_root)
    result = _configure_project(tmp_path / "valid", body)
    assert result.returncode == 0, result.stdout + result.stderr


def test_package_without_jit_generator_keeps_jit_disabled(tmp_path):
    body = _package_call() + (
        "get_property(enabled GLOBAL PROPERTY _ASC_PKG_demo_ENABLE_JIT_PACKAGE)\n"
        "if(enabled)\n"
        '  message(FATAL_ERROR "JIT package property was unexpectedly enabled")\n'
        "endif()\n"
    )
    result = _configure_project(tmp_path / "disabled", body)
    assert result.returncode == 0, result.stdout + result.stderr


def test_jit_bundle_accumulates_dependencies_from_each_package_add():
    function_source = FUNC_CMAKE.read_text(encoding="utf-8")

    assert "function(npu_op_package_jit" not in function_source
    assert 'set(JIT_SOC_ROOT "${JIT_ROOT}/${compute_unit}")' in function_source
    assert 'lib${current_target_vendor_name}_compile_database.so' in function_source
    assert (
        "op_impl/ai_core/tbe/kernel/jit/${compute_unit}"
    ) in function_source
    assert (
        "add_dependencies(${target_package_name}_jit_bundle ${JIT_BUNDLE_TARGET})"
    ) in function_source
    assert (
        "add_dependencies(${JIT_BUNDLE_TARGET} ${SOC_KERNEL_COMPILE_TARGETS})"
    ) in function_source
