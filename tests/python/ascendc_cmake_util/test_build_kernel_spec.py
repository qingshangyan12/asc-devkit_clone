# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import ctypes
import json
import os
from pathlib import Path
import sys
import tempfile
import subprocess

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / "cmake/asc/fwk_modules/util"))
import ascendc_build_kernel_spec as bundle


@pytest.mark.parametrize("destdir", [False, True])
@pytest.mark.parametrize("install_always", [None, "0", "1"])
def test_install_replaces_changed_resource_with_same_timestamp(tmp_path, destdir, install_always):
    output = tmp_path / "build with spaces/libcust_compile_resource.so"
    output.parent.mkdir()
    output.write_bytes(b"new-resource")
    destination = tmp_path / "installed with spaces/op_spec"
    stage = tmp_path / "stage" if destdir else None
    installed = Path(str(stage) + str(destination)) if stage else destination
    installed.mkdir(parents=True)
    target = installed / output.name
    target.write_bytes(b"old-resource")
    for path in (output, target):
        os.utime(path, ns=(1_600_000_000_400_000_000,) * 2)
    script = tmp_path / "install.cmake"
    bundle.KernelSpecBundleBuilder.write_install_entry(script, output)
    driver = tmp_path / "driver.cmake"
    driver.write_text(
        f'set(spec_destination "{destination}")\ninclude("{script}")\n'
        'if(NOT CMAKE_INSTALL_MANIFEST_FILES)\n  message(FATAL_ERROR "missing install manifest")\nendif()\n'
        f'if(NOT "$ENV{{CMAKE_INSTALL_ALWAYS}}" STREQUAL "{install_always or ""}")\n'
        '  message(FATAL_ERROR "installation environment changed")\nendif()\n'
        + (
            'if(DEFINED ENV{CMAKE_INSTALL_ALWAYS})\n  message(FATAL_ERROR "environment leaked")\nendif()\n'
            if install_always is None
            else ""
        )
    )
    env = {key: value for key, value in os.environ.items() if key not in {"CMAKE_INSTALL_ALWAYS", "DESTDIR"}}
    if install_always is not None:
        env["CMAKE_INSTALL_ALWAYS"] = install_always
    if stage:
        env["DESTDIR"] = str(stage)
    result = subprocess.run(["cmake", "-P", str(driver)], env=env, text=True, capture_output=True)
    assert result.returncode == 0, result.stdout + result.stderr
    assert target.read_bytes() == output.read_bytes(), result.stdout


@pytest.mark.parametrize("changed", ["source", "mapping", "tool", "output"])
def test_export_cache_invalidates_changed_inputs_and_output(bundle_request, changed):
    bundle_request.build()
    output = Path(bundle_request.output)
    previous = output.stat().st_mtime_ns
    if changed == "source":
        Path(bundle_request.source_file[0]).write_text("void add() { /* modified */ }\n")
    elif changed == "mapping":
        bundle_request.source_map[0][1] = "op_impl/other"
    elif changed == "tool":
        exporter = Path(bundle_request.exporter)
        exporter.write_text(exporter.read_text() + "\n# changed exporter\n")
    else:
        output.write_bytes(b"invalid cached output")
    bundle_request.build()
    assert output.stat().st_mtime_ns != previous
    assert output.read_bytes().startswith(b"\x7fELF")


@pytest.fixture
def bundle_request(tmp_path):
    source = tmp_path / "src"
    source.mkdir()
    (source / "add.cpp").write_text("void add() {}\n")
    exporter = tmp_path / "exporter"
    exporter.write_text(
        "#!/usr/bin/env python3\n"
        "import json,sys,pathlib\n"
        "args=sys.argv[1:]\n"
        "root=pathlib.Path(args[args.index('--input-dir')+1])\n"
        "out=pathlib.Path(args[args.index('--output')+1])\n"
        "data=[json.loads(p.read_text()) for p in sorted(root.rglob('*_manifest.json'))]\n"
        "out.write_bytes(b'\\x7fELF'+json.dumps(data,sort_keys=True).encode())\n"
    )
    exporter.chmod(0o755)
    inp = tmp_path / "input"
    inp.mkdir()
    _unit(inp / "910b", "a" * 64)
    return bundle.KernelSpecBundleBuilder(
        input_dir=[str(inp)],
        source_file=[str(source / "add.cpp")],
        source_map=[[str(source), "op_impl/dynamic"]],
        output=str(tmp_path / "output/libcust_compile_resource.so"),
        work_dir=str(tmp_path / "work"),
        exporter=str(exporter),
        cxx="c++",
        make="make",
        opc_config=None,
        jobs=2,
    )


def _unit(root, resource_id):
    resources = root / "resources"
    resources.mkdir(parents=True)
    (resources / "wrapper.cpp").write_text('#include "add.cpp"\n')
    manifest = {
        "resource_id": resource_id,
        "soc_version": "Ascend910B1",
        "source_file": "add.cpp",
        "resource_path": "resources",
        "options": {"common_compile": ["-I${source_file_path}"]},
        "kernels": [
            {
                "kernel_name": "add",
                "objects": [
                    {
                        "object_name": "basic",
                        "object_type": "basic",
                        "commands": [
                            {
                                "type": "compile",
                                "cmd": [
                                    "c++",
                                    "${options:common_compile}",
                                    "${resource}/resources/wrapper.cpp",
                                    "-o",
                                    "${output}/add.o",
                                ],
                            }
                        ],
                        "outputs": ["${output}/add.o"],
                    }
                ],
            }
        ],
    }
    path = root / "add_manifest.json"
    path.write_text(json.dumps(manifest))
    return path


def _published(bundle_request):
    return json.loads(Path(bundle_request.output).read_bytes()[4:])


@pytest.mark.parametrize("status", [None, "completed", "filtered", "failed", "invalid"])
def test_bundle_requires_explicit_producer_result(bundle_request, tmp_path, status):
    source = Path(bundle_request.source_file[0])
    context = {
        "tasks": [["Add", "ascend910b"]],
        "opc_config": str(tmp_path / "options.ini"),
        "source_root": str(source.parent),
        "copied_source_root": str(source.parent),
        "source_destination": "op_impl/dynamic",
        "allow_failed": False,
    }
    Path(context["opc_config"]).write_text("")
    config = tmp_path / "config.json"
    config.write_text(json.dumps(context) + "\n")
    work = tmp_path / "bundle"
    task = work / "input/ascend910b/Add"
    task.mkdir(parents=True)
    if status is not None:
        (task / ".producer-result.json").write_text(
            json.dumps({"status": status, "source": str(source), "reason": "test result"})
        )
    if status == "filtered":
        bundle.bundle_resources(config, work, "unused", "unused", "unused")
        assert "file(REMOVE" in (work / "install.cmake").read_text()
    else:
        with pytest.raises(bundle.BundleError):
            bundle.bundle_resources(config, work, "unused", "unused", "unused")
        assert not (work / "install.cmake").exists()


def test_passes_sorted_duplicate_ids_to_exporter_and_preserves_input(bundle_request, monkeypatch):
    root = Path(bundle_request.input_dir[0])
    original = (root / "910b/add_manifest.json").read_bytes()
    _unit(root / "950", "a" * 64)
    _unit(root / "other", "b" * 64)
    hashed_sources = []
    hash_file = bundle._hash_file

    def record_hash(path):
        hashed_sources.append(path)
        return hash_file(path)

    monkeypatch.setattr(bundle, "_hash_file", record_hash)
    bundle_request.build()
    assert hashed_sources.count(Path(bundle_request.source_file[0])) == 1
    data = _published(bundle_request)
    assert [m["resource_id"] for m in data] == ["a" * 64, "a" * 64, "b" * 64]
    assert data[0]["source_file"] == "op_impl/dynamic/add.cpp"
    assert data[0]["options"]["common_compile"] == ["-I${source_file_path}"]
    assert (root / "910b/add_manifest.json").read_bytes() == original


def test_duplicate_id_order_is_preserved_past_ten_candidates(bundle_request):
    root = Path(bundle_request.input_dir[0])
    bundle_request.input_dir = []
    for index in range(11):
        candidate = root / f"candidate_{index:02d}"
        resource_id = str(index) * 64 if index < 2 else "a" * 64
        manifest_path = _unit(candidate, resource_id)
        manifest = json.loads(manifest_path.read_text())
        manifest["options"]["common_compile"].append(f"-DREVIEW_CANDIDATE={index}")
        manifest_path.write_text(json.dumps(manifest))
        bundle_request.input_dir.append(str(candidate))

    bundle_request.build()

    published = _published(bundle_request)
    duplicate = next(manifest for manifest in published if manifest["resource_id"] == "a" * 64)
    assert "-DREVIEW_CANDIDATE=2" in duplicate["options"]["common_compile"]


def test_bundle_preserves_include_options(bundle_request):
    root = Path(bundle_request.source_file[0]).parent
    bundle_request.source_map.append([str(root / "nested"), "extra"])
    path = Path(bundle_request.input_dir[0]) / "910b/add_manifest.json"
    manifest = json.loads(path.read_text())
    manifest["options"]["common_compile"] = [
        "-I",
        str(root),
        "-isystem" + str(root / "nested/include"),
        "-iquote",
        str(root / "not_created"),
        "-idirafter" + str(root),
        "-I/untouched",
    ]
    path.write_text(json.dumps(manifest))
    bundle_request.build()
    assert _published(bundle_request)[0]["options"] == manifest["options"]


def test_exporter_failure_preserves_output_and_cleans_bundle_dir(bundle_request):
    bundle_request.build()
    before = Path(bundle_request.output).read_bytes()
    Path(bundle_request.exporter).write_text("#!/bin/sh\necho failed\nexit 1\n")
    with pytest.raises(bundle.BundleError, match="exporter"):
        bundle_request.build()
    assert Path(bundle_request.output).read_bytes() == before
    assert not list(Path(bundle_request.work_dir).glob(".bundle-*"))


@pytest.mark.parametrize("empty_only", [False, True])
def test_empty_resource_directories_are_skipped(bundle_request, tmp_path, caplog, empty_only):
    empty = tmp_path / "empty"
    empty.mkdir()
    bundle_request.input_dir = [str(empty)] if empty_only else bundle_request.input_dir + [str(empty)]
    output = Path(bundle_request.output)
    if empty_only:
        output.parent.mkdir(parents=True)
        output.write_bytes(b"stale")
        bundle_request.exporter = "missing-exporter"
    assert bundle_request.build() is (not empty_only)
    assert "no manifests" in caplog.text
    if empty_only:
        assert not output.exists()
    else:
        assert len(_published(bundle_request)) == 1


@pytest.mark.parametrize("case", ["missing", "resource_escape", "source_escape", "symlink"])
def test_rejects_invalid_inputs(bundle_request, case, tmp_path):
    path = Path(bundle_request.input_dir[0]) / "910b/add_manifest.json"
    manifest = json.loads(path.read_text())
    if case == "missing":
        bundle_request.input_dir.append(str(tmp_path / "missing"))
    elif case == "resource_escape":
        manifest["resource_path"] = "../resources"
    elif case == "source_escape":
        manifest["source_file"] = "../add.cpp"
    elif case == "symlink":
        (path.parent / "resources/link").symlink_to(bundle_request.source_file[0])
    path.write_text(json.dumps(manifest))
    with pytest.raises(bundle.BundleError):
        bundle_request.build()
    assert not Path(bundle_request.output).exists()


def test_opc_dump_cce_preserves_exporter_diagnostics(bundle_request, tmp_path):
    bundle_request.build()
    config = tmp_path / "custom_opc_options.ini"
    config.write_text("ALL@@--op_debug_config=dump_cce\n")
    bundle_request.opc_config = str(config)
    bundle_request.build()
    folders = list(Path(bundle_request.work_dir).glob(".bundle-*"))
    assert len(folders) == 1
    assert (folders[0] / "build.log").is_file()
    assert (folders[0] / "collected").is_dir()


def test_work_directory_cannot_contaminate_manifest_inputs(bundle_request):
    bundle_request.work_dir = str(Path(bundle_request.input_dir[0]) / "work")
    with pytest.raises(bundle.BundleError, match="overlap"):
        bundle_request.build()


def test_unmapped_source_and_bad_package_map_fail(bundle_request):
    bundle_request.source_map = []
    with pytest.raises(bundle.BundleError, match="source-map"):
        bundle_request.build()
    bundle_request.source_map = [[Path(bundle_request.source_file[0]).parent, "../escape"]]
    with pytest.raises(bundle.BundleError, match="root"):
        bundle_request.build()


def test_publishes_across_filesystems(bundle_request):
    if (
        not Path("/dev/shm").is_dir()
        or Path("/dev/shm").stat().st_dev == Path(bundle_request.work_dir).parent.stat().st_dev
    ):
        pytest.skip("requires writable separate /dev/shm filesystem")
    with tempfile.TemporaryDirectory(prefix="kernel-spec-stage2-", dir="/dev/shm") as output_dir:
        bundle_request.output = str(Path(output_dir) / "libcust_compile_resource.so")
        bundle_request.build()
        assert _published(bundle_request)[0]["source_file"] == "op_impl/dynamic/add.cpp"


def test_exporter_writes_final_output_directly(bundle_request, tmp_path):
    output_record = tmp_path / "exporter-output.txt"
    exporter = tmp_path / "direct-exporter"
    exporter.write_text(
        "#!/usr/bin/env python3\n"
        "import json, pathlib, sys\n"
        "args=sys.argv[1:]\n"
        "output=pathlib.Path(args[args.index('--output') + 1])\n"
        f"pathlib.Path({str(output_record)!r}).write_text(str(output))\n"
        "output.write_bytes(b'\\x7fELF' + json.dumps([]).encode())\n"
    )
    exporter.chmod(0o755)
    bundle_request.exporter = str(exporter)
    bundle_request.build()
    assert Path(output_record.read_text()) == Path(bundle_request.output)


def test_preserves_empty_include_directories_in_resource_copy(bundle_request, tmp_path):
    path = Path(bundle_request.input_dir[0]) / "910b/add_manifest.json"
    (path.parent / "resources/empty_include").mkdir()
    manifest = json.loads(path.read_text())
    manifest["options"]["common_compile"].append("-I${resource}/resources/empty_include")
    path.write_text(json.dumps(manifest))
    config = tmp_path / "custom_opc_options.ini"
    config.write_text("ALL@@--op_debug_config=dump_cce\n")
    bundle_request.opc_config = str(config)
    bundle_request.build()
    bundle_dir = next(Path(bundle_request.work_dir).glob(".bundle-*"))
    assert (bundle_dir / "collected" / "00000000" / "resources/empty_include").is_dir()


@pytest.mark.skipif(not os.environ.get("ASCEND_TEST_COMPILE_EXPORTER"), reason="requires built asc_compile_exporter")
def test_real_exporter_builds_loadable_bundle_with_large_resource(bundle_request):
    bundle_request.exporter = os.environ["ASCEND_TEST_COMPILE_EXPORTER"]
    large = Path(bundle_request.input_dir[0]) / "910b/resources/large.h"
    large.write_bytes(b"x" * (1024 * 1024 + 1))
    bundle_request.build()

    class String(ctypes.Structure):
        _fields_ = [("data", ctypes.c_void_p), ("size", ctypes.c_uint64)]

    class File(ctypes.Structure):
        _fields_ = [("name", String), ("path", String), ("data", ctypes.c_void_p), ("size", ctypes.c_uint64)]

    class Manifest(ctypes.Structure):
        _fields_ = [("json", String), ("files", ctypes.POINTER(File)), ("count", ctypes.c_uint64)]

    class Header(ctypes.Structure):
        _fields_ = [
            ("magic", ctypes.c_uint32),
            ("version", ctypes.c_uint32),
            ("size", ctypes.c_uint32),
            ("flags", ctypes.c_uint32),
        ]

    class Bundle(ctypes.Structure):
        _fields_ = [
            ("header", Header),
            ("manifests", ctypes.POINTER(Manifest)),
            ("count", ctypes.c_uint64),
            ("extensions", ctypes.c_void_p),
            ("extension_count", ctypes.c_uint64),
        ]

    library = ctypes.CDLL(bundle_request.output)
    library.AscendcGetCompileResourceBundle.restype = ctypes.POINTER(Bundle)
    exported = library.AscendcGetCompileResourceBundle().contents
    assert exported.header.magic == 0x41435253
    assert exported.header.version == 1
    assert exported.count == 1
    manifest = exported.manifests[0]
    data = json.loads(ctypes.string_at(manifest.json.data, manifest.json.size))
    assert data["source_file"] == "op_impl/dynamic/add.cpp"
    assert data["options"]["common_compile"] == ["-I${source_file_path}"]
    resources = {ctypes.string_at(f.path.data, f.path.size).decode(): f for f in manifest.files[: manifest.count]}
    assert resources["resources/large.h"].size == 1024 * 1024 + 1
    assert ctypes.string_at(resources["resources/large.h"].data, 1) == b"x"


@pytest.mark.parametrize("difference", ["source", "installed_path"])
def test_same_id_source_keeps_first_candidate(bundle_request, tmp_path, difference):
    root = Path(bundle_request.input_dir[0])
    _unit(root / "duplicate", "a" * 64)
    source = tmp_path / "other_source/add.cpp"
    source.parent.mkdir()
    source.write_text("different source" if difference == "source" else "void add() {}\n")
    bundle_request.source_map.append(
        [str(source.parent), "op_impl/dynamic" if difference == "source" else "other_layout"]
    )
    bundle_request.source_bindings = {
        str(root / "910b"): bundle_request.source_file[0],
        str(root / "duplicate"): str(source),
    }
    bundle_request.input_dir = list(bundle_request.source_bindings)

    exporter = tmp_path / "dedup_exporter"
    exporter.write_text(
        "#!/usr/bin/env python3\n"
        "import json\n"
        "import sys\n"
        "from pathlib import Path\n"
        "args = sys.argv[1:]\n"
        "root = Path(args[args.index('--input-dir') + 1])\n"
        "output = Path(args[args.index('--output') + 1])\n"
        "seen = set()\n"
        "manifests = []\n"
        "for path in sorted(root.rglob('*_manifest.json')):\n"
        "    manifest = json.loads(path.read_text())\n"
        "    if manifest['resource_id'] not in seen:\n"
        "        seen.add(manifest['resource_id'])\n"
        "        manifests.append(manifest)\n"
        "output.write_bytes(b'\\x7fELF' + json.dumps(manifests).encode())\n"
    )
    exporter.chmod(0o755)
    bundle_request.exporter = str(exporter)
    bundle_request.build()
    published = _published(bundle_request)
    assert len(published) == 1
    assert published[0]["source_file"] == "op_impl/dynamic/add.cpp"


@pytest.mark.skipif(not os.environ.get("ASCEND_TEST_COMPILE_EXPORTER"), reason="requires built asc_compile_exporter")
@pytest.mark.parametrize("difference", ["equivalent", "manifest", "resource", "empty_directory", "empty_file"])
def test_real_exporter_keeps_first_duplicate_id(bundle_request, difference):
    bundle_request.exporter = os.environ["ASCEND_TEST_COMPILE_EXPORTER"]
    bundle_request.build()
    published = Path(bundle_request.output).read_bytes()
    path = _unit(Path(bundle_request.input_dir[0]) / "duplicate", "a" * 64)
    bundle_request.build()
    if difference == "manifest":
        manifest = json.loads(path.read_text())
        manifest["soc_version"] = "Ascend950"
        path.write_text(json.dumps(manifest))
    elif difference == "resource":
        (path.parent / "resources/wrapper.cpp").write_text("different wrapper")
    elif difference == "empty_directory":
        (path.parent / "resources/include").mkdir()
    elif difference == "empty_file":
        (path.parent / "resources/empty.h").write_text("")
    bundle_request.build()
    assert Path(bundle_request.output).read_bytes() == published
    if difference != "equivalent":
        return
    manifest = json.loads(path.read_text())
    path.write_text(json.dumps(dict(reversed(list(manifest.items()))), indent=4))
    bundle_request.build()

    class Header(ctypes.Structure):
        _fields_ = [
            ("magic", ctypes.c_uint32),
            ("version", ctypes.c_uint32),
            ("size", ctypes.c_uint32),
            ("flags", ctypes.c_uint32),
        ]

    class Bundle(ctypes.Structure):
        _fields_ = [("header", Header), ("manifests", ctypes.c_void_p), ("count", ctypes.c_uint64)]

    library = ctypes.CDLL(bundle_request.output)
    library.AscendcGetCompileResourceBundle.restype = ctypes.POINTER(Bundle)
    assert library.AscendcGetCompileResourceBundle().contents.count == 1


def test_producer_binding_selects_actual_source_among_same_names(bundle_request, tmp_path):
    other = tmp_path / "other/add.cpp"
    other.parent.mkdir()
    other.write_text("other source")
    bundle_request.source_file.append(str(other))
    bundle_request.source_bindings = {str(Path(bundle_request.input_dir[0])): bundle_request.source_file[0]}
    bundle_request.build()
    assert _published(bundle_request)[0]["source_file"] == "op_impl/dynamic/add.cpp"


def test_preserves_shared_include_outside_kernel_root(bundle_request, tmp_path):
    shared = tmp_path / "shared"
    shared.mkdir()
    path = Path(bundle_request.input_dir[0]) / "910b/add_manifest.json"
    manifest = json.loads(path.read_text())
    manifest["options"]["common_compile"] = ["-I" + str(shared)]
    path.write_text(json.dumps(manifest))
    bundle_request.source_map.append([str(shared), "op_impl/common"])
    bundle_request.build()
    assert _published(bundle_request)[0]["options"]["common_compile"] == manifest["options"]["common_compile"]
