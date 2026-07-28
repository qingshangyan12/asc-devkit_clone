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

import subprocess
from collections import defaultdict
from pathlib import Path
from types import SimpleNamespace

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
FUNC_CMAKE = REPO_ROOT / "cmake/asc/fwk_modules/func.cmake"


def _write(path, content="{}"):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content)


def test_collects_sorted_recursive_manifests_with_argv(monkeypatch, tmp_path):
    from ascendc_jit_resource import collect_custom_op_resources

    generator = str(tmp_path / "generator path" / "manifest_generator")
    custom_op_root = str(tmp_path / "custom op")
    collection_root = str(tmp_path / "collected resources")
    working_dir = tmp_path / "AddCustom_ascend910b"
    manifest1 = working_dir / "kernel_1" / "nested" / "z_manifest.json"
    manifest0 = working_dir / "kernel_0" / "a_manifest.json"
    _write(manifest1)
    _write(manifest0)
    calls = []

    def fake_run(command, **kwargs):
        calls.append((command, kwargs))

    monkeypatch.setattr(subprocess, "run", fake_run)
    collect_custom_op_resources(
        generator, custom_op_root, collection_root, str(working_dir)
    )

    assert calls == [
        ([
            generator,
            "--collect_custom_op_resources",
            custom_op_root,
            collection_root,
            str(manifest0),
            str(manifest1),
        ], {"check": True})
    ]


def test_collection_requires_at_least_one_manifest(tmp_path):
    from ascendc_jit_resource import collect_custom_op_resources

    working_dir = tmp_path / "NoManifest_ascend910b"
    working_dir.mkdir()
    with pytest.raises(RuntimeError, match="found no manifest"):
        collect_custom_op_resources(
            "/tool/manifest_generator", "/custom op", "/collection", str(working_dir)
        )


def test_collection_wraps_generator_failure(monkeypatch, tmp_path):
    from ascendc_jit_resource import collect_custom_op_resources

    working_dir = tmp_path / "FailedOp_ascend910b"
    _write(working_dir / "kernel_0" / "failed_manifest.json")

    def fail(command, **kwargs):
        raise subprocess.CalledProcessError(7, command)

    monkeypatch.setattr(subprocess, "run", fail)
    with pytest.raises(RuntimeError, match=str(working_dir)):
        collect_custom_op_resources(
            "/tool/manifest_generator", "/custom op", "/collection", str(working_dir)
        )


def _compile_args(tmp_path, **jit_options):
    defaults = {
        "jit_resource_generator": None,
        "jit_custom_op_root": None,
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


def test_compile_kernel_requires_all_jit_options(monkeypatch, tmp_path):
    from ascendc_compile_kernel import CompileKernel

    monkeypatch.chdir(tmp_path)
    args = _compile_args(
        tmp_path,
        jit_resource_generator="/tool/manifest_generator",
        jit_custom_op_root="/custom op",
    )
    with pytest.raises(ValueError, match="all be provided"):
        CompileKernel(args)


def test_compile_kernel_collection_is_optional(monkeypatch, tmp_path):
    import ascendc_compile_kernel

    monkeypatch.chdir(tmp_path)
    calls = []
    monkeypatch.setattr(
        ascendc_compile_kernel,
        "collect_custom_op_resources",
        lambda **kwargs: calls.append(kwargs),
    )

    disabled = ascendc_compile_kernel.CompileKernel(_compile_args(tmp_path))
    disabled.collect_jit_resources()
    assert calls == []

    enabled_args = _compile_args(
        tmp_path,
        jit_resource_generator=str(tmp_path / "generator with spaces"),
        jit_custom_op_root=str(tmp_path / "custom op"),
        jit_collection_root=str(tmp_path / "collection root"),
    )
    enabled = ascendc_compile_kernel.CompileKernel(enabled_args)
    enabled.collect_jit_resources()
    assert calls == [{
        "generator": enabled_args.jit_resource_generator,
        "custom_op_root": enabled_args.jit_custom_op_root,
        "collection_root": enabled_args.jit_collection_root,
        "working_dir": enabled.working_dir,
    }]


@pytest.mark.parametrize("jit_enabled", [False, True])
def test_compile_kernel_requests_kernel_meta_for_jit(monkeypatch, tmp_path, jit_enabled):
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

    jit_options = {}
    if jit_enabled:
        jit_options = {
            "jit_resource_generator": "/tool/manifest_generator",
            "jit_custom_op_root": "/custom op",
            "jit_collection_root": "/collection",
        }
    kernel = ascendc_compile_kernel.CompileKernel(_compile_args(tmp_path, **jit_options))
    kernel.ascendc_gen_param()

    assert recorded == [{"preserve_kernel_meta": jit_enabled}]


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


def _jit_call(generator, package_type="RUN", package="demo", vendor="customize"):
    return (
        'set_property(GLOBAL PROPERTY _ASC_PKG_{0}_TYPE {1})\n'
        'set_property(GLOBAL PROPERTY _ASC_PKG_{0}_VENDOR_NAME {2})\n'
        'npu_op_package_jit({0} GENERATOR "{3}")\n'
    ).format(package, package_type, vendor, generator.as_posix())


@pytest.mark.parametrize(
    "name,body,error",
    [
        ("unknown", 'npu_op_package_jit(missing GENERATOR "/missing")\n', "not specified"),
        ("non_run", None, "only supports RUN"),
        ("missing_generator", (
            "set_property(GLOBAL PROPERTY _ASC_PKG_demo_TYPE RUN)\n"
            "set_property(GLOBAL PROPERTY _ASC_PKG_demo_VENDOR_NAME customize)\n"
            "npu_op_package_jit(demo)\n"
        ), "must provide GENERATOR"),
    ],
)
def test_jit_package_rejects_invalid_package_contract(tmp_path, name, body, error):
    if name == "non_run":
        body = _jit_call(_fake_generator(tmp_path), package_type="SHARED")
    result = _configure_project(tmp_path / name, body)
    assert result.returncode != 0
    assert error in result.stdout + result.stderr


def test_jit_package_rejects_missing_or_non_executable_generator(tmp_path):
    missing = tmp_path / "missing generator"
    missing_result = _configure_project(
        tmp_path / "missing", _jit_call(missing)
    )
    assert missing_result.returncode != 0
    assert "does not exist" in missing_result.stdout + missing_result.stderr

    non_executable = _fake_generator(tmp_path, executable=False)
    non_executable_result = _configure_project(
        tmp_path / "non executable", _jit_call(non_executable)
    )
    assert non_executable_result.returncode != 0
    assert "not executable" in non_executable_result.stdout + non_executable_result.stderr


def test_jit_package_stores_normalized_properties(tmp_path):
    generator = _fake_generator(tmp_path)
    expected_root = (tmp_path / "valid" / "build" / "jit_resources" / "demo").as_posix()
    body = _jit_call(generator) + (
        "get_property(enabled GLOBAL PROPERTY _ASC_PKG_demo_ENABLE_JIT_PACKAGE)\n"
        "get_property(actual_generator GLOBAL PROPERTY _ASC_PKG_demo_JIT_GENERATOR)\n"
        "get_property(collection GLOBAL PROPERTY _ASC_PKG_demo_JIT_COLLECTION_ROOT)\n"
        "get_property(work GLOBAL PROPERTY _ASC_PKG_demo_JIT_WORK_DIR)\n"
        "get_property(output GLOBAL PROPERTY _ASC_PKG_demo_JIT_OUTPUT_SO)\n"
        "if(NOT enabled)\n"
        '  message(FATAL_ERROR "JIT package property was not enabled")\n'
        "endif()\n"
        'if(NOT actual_generator STREQUAL "{}")\n'
        '  message(FATAL_ERROR "unexpected generator: ${{actual_generator}}")\n'
        "endif()\n"
        'if(NOT collection STREQUAL "{}/collected")\n'
        '  message(FATAL_ERROR "unexpected collection: ${{collection}}")\n'
        "endif()\n"
        'if(NOT work STREQUAL "{}/work")\n'
        '  message(FATAL_ERROR "unexpected work: ${{work}}")\n'
        "endif()\n"
        'if(NOT output STREQUAL "{}/libcustomize_compile_resource.so")\n'
        '  message(FATAL_ERROR "unexpected output: ${{output}}")\n'
        "endif()\n"
    ).format(generator.resolve().as_posix(), expected_root, expected_root, expected_root)
    result = _configure_project(tmp_path / "valid", body)
    assert result.returncode == 0, result.stdout + result.stderr
