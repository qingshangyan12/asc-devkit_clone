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

"""Real CMake build/install checks using simulated producer and exporter executables."""

import json
import os
import shutil
import subprocess
from pathlib import Path

import pytest

FWK_MODULES_DIR = Path(__file__).resolve().parents[3] / "cmake/asc/fwk_modules"


@pytest.mark.parametrize("script", ["install.sh", "upgrade.sh"])
def test_run_scripts_deliver_and_remove_stale_resource_so(tmp_path, script):
    for name in ("install.sh", "upgrade.sh", "uninstall.sh"):
        shutil.copy2(FWK_MODULES_DIR / "scripts" / name, tmp_path / name)
    target = tmp_path / "installed"
    vendor = Path("vendors/customize")
    (target / vendor).mkdir(parents=True)
    (target / "vendors/config.ini").write_text("load_priority=customize\n")
    payload = tmp_path / "packages" / vendor
    resource = payload / "op_spec/libcust_compile_resource.so"
    resource.parent.mkdir(parents=True)
    (payload / "version.info").write_text("version=1\n")
    command = ["bash", str(tmp_path / script)]
    if script == "install.sh":
        # This fixture tests directory delivery and carries no host library/version metadata.
        command.extend(["--quiet", "--force", "--install-path=" + str(target)])
    for content in (b"first resource", None, b"new resource"):
        if content is None:
            resource.unlink()
        else:
            resource.write_bytes(content)
        result = subprocess.run(
            command,
            cwd=tmp_path,
            env={**os.environ, "ASCEND_OPP_PATH": str(target)},
            input="o\n" * 20,
            text=True,
            capture_output=True,
            timeout=30,
        )
        assert result.returncode == 0, result.stdout + result.stderr
        installed = target / vendor / "op_spec/libcust_compile_resource.so"
        assert installed.exists() == (content is not None)
        if content is not None:
            assert installed.read_bytes() == content


PROJECT = r"""
cmake_minimum_required(VERSION 3.16)
project(spec_graph CXX)
set(ASCEND_PYTHON_EXECUTABLE python3)
set(ASCENDC_CMAKE_SCRIPTS_PATH "${FWK_MODULES_DIR}")
include(${ASCENDC_CMAKE_SCRIPTS_PATH}/func.cmake)
set(_ASC_KERNEL_SPEC_EXPORTER "${CMAKE_CURRENT_SOURCE_DIR}/exporter" CACHE FILEPATH "")
set(ASCEND_COMPUTE_UNIT "ascend910b;ascend950" CACHE STRING "Selected chips")
option(ASCEND_SKIP_FAILED_COMPUTE_UNIT "Allow failed producer tasks" ON)
set_property(GLOBAL PROPERTY _ASC_PKG_demo_KERNEL_SPEC_BUILD_DIR "${CMAKE_BINARY_DIR}/demo/kernel_spec_bundle")
set_property(GLOBAL PROPERTY _ASC_PKG_demo_ENABLE_CPACK TRUE)
add_custom_target(binary)
_ascendc_remove_kernel_spec_install(demo packages/vendors/demo/op_spec)
function(add_kernel kernel)
  set(auto_gen_path "${CMAKE_BINARY_DIR}/${kernel}/autogen")
  set(KERNEL_SRC_BASE "${CMAKE_CURRENT_SOURCE_DIR}/${kernel}")
  set(KERNEL_BINARY_PATH "${CMAKE_BINARY_DIR}/${kernel}")
  set(KERNEL_DYNAMIC_PATH "${KERNEL_BINARY_PATH}/binary/dynamic")
  file(MAKE_DIRECTORY "${auto_gen_path}")
  file(WRITE "${auto_gen_path}/custom_opc_options.ini" "${SPEC_OPC_OPTIONS}")
  set(kernel_spec_mode "${SPEC_MODE}")
  set(ASCEND_AUTOGEN_PATH "${auto_gen_path}")
  set_property(GLOBAL PROPERTY ASCENDC_AUTO_GEN_PATH "${auto_gen_path}")
  ascend_copy_kernel_source_file(TARGET ${kernel}_copy_kernel_srcs PACKAGE_TARGET ${kernel}
    KERNEL_BASE_DIR "${KERNEL_SRC_BASE}" DST_DIR "${KERNEL_DYNAMIC_PATH}")
  set(KERNEL_COMPILE_TARGETS)
  set(kernel_spec_tasks)
  foreach(soc IN LISTS ASCEND_COMPUTE_UNIT)
    add_custom_target(${kernel}_ascendc_bin_${soc}_gen_ops_config)
    foreach(op IN ITEMS Add Mul)
      string(TOLOWER "${op}" op_file)
      add_custom_target(${kernel}_${soc}_${op}
        COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/producer.py" "${ASCENDC_CMAKE_SCRIPTS_PATH}/util"
          "${kernel_spec_mode}" "${CMAKE_BINARY_DIR}/demo/kernel_spec_bundle" "${op}" "${soc}"
          "${KERNEL_SRC_BASE}/${op_file}.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/fail"
        VERBATIM)
      list(APPEND KERNEL_COMPILE_TARGETS ${kernel}_${soc}_${op})
      list(APPEND kernel_spec_tasks "[\"${op}\",\"${soc}\"]")
      add_dependencies(${kernel}_ascendc_bin_${soc}_gen_ops_config ${kernel}_${soc}_${op})
    endforeach()
  endforeach()
  if(NOT SPEC_MODE STREQUAL "None")
    _ascendc_kernel_spec_layout(demo "packages/vendors/demo/op_impl/dynamic/${kernel}")
    _ascendc_add_kernel_spec_target(demo ${kernel})
  else()
    foreach(target IN LISTS KERNEL_COMPILE_TARGETS)
      add_dependencies(binary ${target})
    endforeach()
  endif()
  install(DIRECTORY "${KERNEL_DYNAMIC_PATH}/" DESTINATION "packages/vendors/demo/op_impl/dynamic/${kernel}" OPTIONAL)
endfunction()
add_kernel(kernel_a)
"""

PRODUCER = r"""
import hashlib
import json
import sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from ascendc_build_kernel_spec import task_directory
mode, root, op, soc, source, fail = sys.argv[2:]
if mode != "None":
    assert (Path(root) / 'input').is_dir(), 'CMake must prepare inputs before producers'
    directory = task_directory(str(Path(root) / 'input'), op, soc)
    directory.mkdir(parents=True, exist_ok=True)
    failure = Path(fail).read_text().strip() if Path(fail).exists() else None
    filtered = failure in ('filtered', 'filtered:' + soc)
    failed = failure is not None and failure in ('', soc)
    if not filtered:
        resources = directory / 'resources'
        resources.mkdir()
        (resources / 'wrapper.cpp').write_text('wrapper')
        (directory / 'add_manifest.json').write_text(json.dumps({
            'resource_id': hashlib.sha256((soc + op).encode()).hexdigest(),
            'soc_version': soc, 'source_file': Path(source).name, 'resource_path': 'resources',
            'options': {'common_compile': ['-I${source_file_path}']},
            'kernels': [{'kernel_name': op, 'objects': []}]
        }))
    if failure != 'missing':
        (directory / '.producer-result.json').write_text(json.dumps({
            'source': source, 'status': 'filtered' if filtered else ('failed' if failed else 'completed'),
            'reason': 'simulated compile failure' if failed else ''
        }))
"""

EXPORTER = r"""
#!/usr/bin/env python3
import json
import sys
from pathlib import Path
args = sys.argv[1:]
root = Path(args[args.index('--input-dir') + 1])
out = Path(args[args.index('--output') + 1])
out.write_bytes(b'\x7fELF' + json.dumps([json.loads(p.read_text()) for p in sorted(root.rglob('*_manifest.json'))]).encode())
"""


def _run(*arguments, success=True):
    result = subprocess.run([str(argument) for argument in arguments], text=True, capture_output=True, check=False)
    assert (result.returncode == 0) == success, result.stdout + result.stderr
    return result


@pytest.fixture(params=["Unix Makefiles", "Ninja"])
def project(tmp_path, request):
    source = tmp_path / "source with spaces"
    source.mkdir()
    (source / "CMakeLists.txt").write_text(PROJECT)
    (source / "producer.py").write_text(PRODUCER)
    exporter = source / "exporter"
    exporter.write_text(EXPORTER.lstrip())
    exporter.chmod(0o755)
    (source / "kernel_a").mkdir()
    (source / "kernel_a/add.cpp").write_text("void kernel_a() {}\n")
    (source / "kernel_a/mul.cpp").write_text("void kernel_mul() {}\n")
    build = tmp_path / "build with spaces"
    _run(
        "cmake",
        "-G",
        request.param,
        "-S",
        source,
        "-B",
        build,
        f"-DFWK_MODULES_DIR={FWK_MODULES_DIR}",
        "-DSPEC_MODE=Normal",
    )
    return source, build, tmp_path / "installed"


def _build(build, success=True):
    return _run("cmake", "--build", build, "--target", "binary", "-j4", success=success)


def _install(build, prefix, success=True):
    return _run("cmake", "--install", build, "--prefix", prefix, success=success)


def _configure(source, build, *options):
    _run("cmake", "-S", source, "-B", build, *options)


def test_package_reuses_fixed_paths_and_replaces_resource_inputs(project):
    source, build, prefix = project
    _build(build)
    _install(build, prefix)
    work = build / "demo/kernel_spec_bundle"
    inputs = work / "input"
    output = work / "op_spec/libcust_compile_resource.so"
    installed = prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so"
    assert inputs.is_dir()
    assert output.read_bytes() == installed.read_bytes()
    assert not (work / "current_build.json").exists()
    assert not list(work.glob("build-*"))
    data = json.loads(installed.read_bytes()[4:])
    assert len(data) == 4
    assert {m["source_file"] for m in data} == {"op_impl/dynamic/kernel_a/add.cpp", "op_impl/dynamic/kernel_a/mul.cpp"}
    assert all(
        (inputs / soc / op / ".producer-result.json").is_file()
        for soc in ("ascend910b", "ascend950")
        for op in ("Add", "Mul")
    )
    stale = inputs / "obsolete/resources/old.cpp"
    stale.parent.mkdir(parents=True)
    stale.write_text("old resource")
    timestamp = output.stat().st_mtime_ns
    _build(build)
    assert not stale.exists()
    assert output.stat().st_mtime_ns == timestamp


def test_skipped_producers_remove_old_result_and_reconfigure_requires_build(project):
    source, build, prefix = project
    _build(build)
    _install(build, prefix)
    installed = prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so"
    previous = installed.read_bytes()
    work = build / "demo/kernel_spec_bundle"
    (source / "fail").touch()
    result = _build(build)
    assert "producer failed" in result.stdout + result.stderr
    assert not (work / "op_spec/libcust_compile_resource.so").exists()
    _install(build, prefix)
    assert not installed.exists()
    (source / "fail").unlink()
    _build(build)
    _install(build, prefix)
    assert installed.read_bytes() == previous
    _configure(source, build, "-DSPEC_MODE=Normal")
    _install(build, prefix, success=False)
    assert installed.read_bytes() == previous


@pytest.mark.parametrize("failure", ["ascend950", "missing"])
def test_producer_result_failures_preserve_previous_delivery(project, failure):
    source, build, prefix = project
    _build(build)
    _install(build, prefix)
    work = build / "demo/kernel_spec_bundle"
    output = work / "op_spec/libcust_compile_resource.so"
    previous = output.read_bytes()
    installed = prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so"
    data = installed.read_bytes()
    (source / "fail").write_text(failure)
    _configure(source, build, "-DASCEND_SKIP_FAILED_COMPUTE_UNIT=OFF")
    result = _build(build, success=False)
    message = "producer result is missing" if failure == "missing" else "producer Add/ascend950: failed"
    assert message in result.stdout + result.stderr
    assert output.read_bytes() == previous
    assert installed.read_bytes() == data
    _install(build, prefix, success=False)
    if failure != "missing":
        _configure(source, build, "-DASCEND_SKIP_FAILED_COMPUTE_UNIT=ON")
        _build(build)
        _install(build, prefix)
        resources = json.loads(installed.read_bytes()[4:])
        assert len(resources) == 2
        assert {resource["soc_version"] for resource in resources} == {"ascend910b"}
        assert output.is_file()
    else:
        _configure(source, build, "-DASCEND_SKIP_FAILED_COMPUTE_UNIT=ON")
        result = _build(build, success=False)
        assert "producer result is missing" in result.stdout + result.stderr
        assert output.read_bytes() == previous


def test_package_none_removes_stale_delivery(project):
    source, build, prefix = project
    _build(build)
    _install(build, prefix)
    installed = prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so"
    assert installed.is_file()
    _configure(source, build, "-DSPEC_MODE=None", "-D_ASC_KERNEL_SPEC_EXPORTER=missing-exporter")
    _build(build)
    _install(build, prefix)
    assert not installed.exists()


def test_fixed_inputs_handle_filtering_and_chip_selection_changes(project):
    source, build, prefix = project
    _build(build)
    (source / "fail").write_text("filtered:ascend950")
    _build(build)
    _install(build, prefix)
    work = build / "demo/kernel_spec_bundle"
    output = work / "op_spec/libcust_compile_resource.so"
    assert {m["soc_version"] for m in json.loads(output.read_bytes()[4:])} == {"ascend910b"}
    (source / "fail").write_text("filtered")
    _build(build)
    _install(build, prefix)
    assert not output.exists()
    assert not output.with_suffix(".so.cache.json").exists()
    assert not (prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so").exists()
    (source / "fail").unlink()
    _configure(source, build, "-DASCEND_COMPUTE_UNIT=ascend910b", "-DSPEC_MODE=SK")
    _build(build)
    assert not (work / "input/ascend950").exists()
    assert len(json.loads(output.read_bytes()[4:])) == 2


def test_fixed_output_preserves_diagnostics_and_blocks_failed_export_install(project):
    source, build, prefix = project
    _build(build)
    _install(build, prefix)
    work = build / "demo/kernel_spec_bundle"
    output = work / "op_spec/libcust_compile_resource.so"
    previous = output.read_bytes()
    exporter = source / "exporter"
    original = exporter.read_text()
    exporter.write_text("#!/bin/sh\nexit 1\n")
    _build(build, success=False)
    assert output.read_bytes() == previous
    _install(build, prefix, success=False)
    exporter.write_text(original)
    _configure(source, build, "-DSPEC_OPC_OPTIONS=ALL@@--op_debug_config=dump_cce\n")
    _build(build)
    diagnostics = list((work / "work").glob(".bundle-*"))
    assert len(diagnostics) == 1
    retained = diagnostics[0] / "collected/00000000/resource_manifest.json"
    manifest = retained.read_bytes()
    _build(build)
    assert retained.read_bytes() == manifest
    assert len(list((work / "work").glob(".bundle-*"))) == 2
    _install(build, prefix)
