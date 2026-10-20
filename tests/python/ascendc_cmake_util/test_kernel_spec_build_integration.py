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
set(ASCEND_COMPUTE_UNIT ascend910b ascend950)
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
  file(WRITE "${auto_gen_path}/custom_opc_options.ini" "")
  set(kernel_spec_mode "${SPEC_MODE}")
  if(DISABLE_EXPORT)
    set(kernel_spec_mode None)
  endif()
  set_property(GLOBAL PROPERTY ASCENDC_AUTO_GEN_PATH "${auto_gen_path}")
  ascend_copy_kernel_source_file(TARGET ${kernel}_copy_kernel_srcs PACKAGE_TARGET ${kernel}
    KERNEL_BASE_DIR "${KERNEL_SRC_BASE}" DST_DIR "${KERNEL_DYNAMIC_PATH}")
  set(KERNEL_COMPILE_TARGETS)
  set(kernel_spec_tasks)
  foreach(soc IN LISTS ASCEND_COMPUTE_UNIT)
    add_custom_target(${kernel}_${soc}
      COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/producer.py" "${ASCENDC_CMAKE_SCRIPTS_PATH}/util"
        "${kernel_spec_mode}" "${CMAKE_BINARY_DIR}/demo/kernel_spec_bundle" Add "${soc}"
        "${KERNEL_SRC_BASE}/add.cpp" "${KERNEL_DYNAMIC_PATH}/add.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/fail"
      VERBATIM)
    add_dependencies(${kernel}_${soc} ${kernel}_copy_kernel_srcs)
    list(APPEND KERNEL_COMPILE_TARGETS ${kernel}_${soc})
    list(APPEND kernel_spec_tasks "[\"Add\",\"${soc}\"]")
    add_custom_target(${kernel}_ascendc_bin_${soc}_gen_ops_config)
    add_dependencies(${kernel}_ascendc_bin_${soc}_gen_ops_config ${kernel}_${soc})
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
from ascendc_build_kernel_spec import KernelSpecBuildDirectory, task_directory
mode, root, op, soc, source, copied_source, fail = sys.argv[2:]
assert Path(copied_source).read_text() == Path(source).read_text(), 'compile must see the latest copied source'
if mode != "None":
    build_directory = KernelSpecBuildDirectory(root)
    directory = task_directory(str(build_directory.current_directory), op, soc)
    assert directory.is_dir(), 'prepare must create the directory before parallel OPC variants'
    resources = directory / 'resources'
    resources.mkdir()
    (resources / 'wrapper.cpp').write_text('wrapper')
    (directory / 'add_manifest.json').write_text(json.dumps({
        'resource_id': hashlib.sha256(soc.encode()).hexdigest(),
        'soc_version': soc, 'source_file': 'add.cpp', 'resource_path': 'resources',
        'options': {'common_compile': ['-I${source_file_path}']},
        'kernels': [{'kernel_name': op, 'objects': []}]
    }))
    failure = Path(fail).read_text().strip() if Path(fail).exists() else None
    failed = failure is not None and failure in ('', soc)
    if failure != 'missing':
        (directory / '.producer-result.json').write_text(json.dumps({
            'source': source, 'status': 'failed' if failed else 'completed',
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


@pytest.fixture
def project(tmp_path):
    source = tmp_path / "source with spaces"
    source.mkdir()
    (source / "CMakeLists.txt").write_text(PROJECT)
    (source / "producer.py").write_text(PRODUCER)
    exporter = source / "exporter"
    exporter.write_text(EXPORTER.lstrip())
    exporter.chmod(0o755)
    (source / "kernel_a").mkdir()
    (source / "kernel_a/add.cpp").write_text("void kernel_a() {}\n")
    build = tmp_path / "build with spaces"
    _run("cmake", "-S", source, "-B", build, f"-DFWK_MODULES_DIR={FWK_MODULES_DIR}", "-DSPEC_MODE=Normal")
    return source, build, tmp_path / "installed"


def _build(build, success=True):
    return _run("cmake", "--build", build, "--target", "binary", "-j4", success=success)


def _install(build, prefix, success=True):
    return _run("cmake", "--install", build, "--prefix", prefix, success=success)


def _configure(source, build, *options):
    _run("cmake", "-S", source, "-B", build, *options)


def test_package_builds_use_fresh_directories_and_install_resources(project):
    source, build, prefix = project
    _build(build)
    _install(build, prefix)
    work = build / "demo/kernel_spec_bundle"

    def current():
        return Path(json.loads((work / "current_build.json").read_text())["directory"])

    output = current() / "op_spec/libcust_compile_resource.so"
    installed = prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so"
    data = json.loads(installed.read_bytes()[4:])
    assert len(data) == 2
    assert all("kernel-spec" not in path.read_text() for path in build.glob("*/autogen/custom_opc_options.ini"))
    assert {manifest["source_file"] for manifest in data} == {"op_impl/dynamic/kernel_a/add.cpp"}
    first = current()
    assert len(list(work.glob("build-*"))) == 1
    assert all((first / soc / "Add/.producer-result.json").is_file() for soc in ("ascend910b", "ascend950"))
    _build(build)
    second = current()
    assert second != first
    assert not first.exists()
    assert (second / "op_spec/libcust_compile_resource.so").is_file()
    assert not list(work.glob("*.tasks.json"))
    (source / "kernel_a/add.cpp").write_text("changed source")
    _build(build)
    assert current() not in {first, second}
    assert len(list(work.glob("build-*"))) == 1
    assert (build / "kernel_a/binary/dynamic/add.cpp").read_text() == "changed source"


def test_skipped_producers_remove_old_result_and_reconfigure_requires_build(project):
    source, build, prefix = project
    _build(build)
    _install(build, prefix)
    installed = prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so"
    previous = installed.read_bytes()
    work = build / "demo/kernel_spec_bundle"
    previous_build = json.loads((work / "current_build.json").read_text())["directory"]
    (source / "fail").touch()
    result = _build(build)
    assert "producer failed" in result.stderr
    skipped_build = json.loads((work / "current_build.json").read_text())["directory"]
    assert skipped_build != previous_build
    assert not (Path(skipped_build) / "op_spec/libcust_compile_resource.so").exists()
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
    previous = Path(json.loads((work / "current_build.json").read_text())["directory"])
    installed = prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so"
    data = installed.read_bytes()
    (source / "fail").write_text(failure)
    _configure(source, build, "-DASCEND_SKIP_FAILED_COMPUTE_UNIT=OFF")
    result = _build(build, success=False)
    assert ("producer result is missing" if failure == "missing" else "producer Add/ascend950: failed") in result.stderr
    assert previous.is_dir()
    assert installed.read_bytes() == data
    _install(build, prefix, success=False)
    if failure != "missing":
        _configure(source, build, "-DASCEND_SKIP_FAILED_COMPUTE_UNIT=ON")
        _build(build)
        _install(build, prefix)
        resources = json.loads(installed.read_bytes()[4:])
        assert [resource["soc_version"] for resource in resources] == ["ascend910b"]
        assert not previous.exists()
    else:
        _configure(source, build, "-DASCEND_SKIP_FAILED_COMPUTE_UNIT=ON")
        assert "producer result is missing" in _build(build, success=False).stderr
        assert previous.is_dir()


def test_global_none_and_package_none_remove_stale_delivery(project):
    source, build, prefix = project
    _build(build)
    _install(build, prefix)
    work = build / "demo/kernel_spec_bundle"
    previous_build = Path(json.loads((work / "current_build.json").read_text())["directory"])
    installed = prefix / "packages/vendors/demo/op_spec/libcust_compile_resource.so"
    _configure(source, build, "-DDISABLE_EXPORT=ON", "-D_ASC_KERNEL_SPEC_EXPORTER=missing-exporter")
    _build(build)
    _install(build, prefix)
    current = Path(json.loads((work / "current_build.json").read_text())["directory"])
    assert current != previous_build
    assert not (current / "op_spec/libcust_compile_resource.so").exists()
    assert not previous_build.exists()
    assert not installed.exists()
    _configure(source, build, "-DDISABLE_EXPORT=OFF")
    _build(build, success=False)
    _configure(source, build, "-DSPEC_MODE=None")
    _build(build)
    installed.parent.mkdir(parents=True, exist_ok=True)
    installed.write_bytes(b"stale")
    _install(build, prefix)
    assert not installed.exists()
