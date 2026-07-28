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
from types import SimpleNamespace

import pytest


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
