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

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / "cmake/asc/fwk_modules/util"))
import ascendc_build_kernel_spec as bundle


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


def test_build_config_is_json_lines_only(tmp_path):
    config = tmp_path / "kernel_spec_build_config.jsonl"
    contexts = [{"tasks": [["Add", "ascend910b"]]}]
    config.write_text("\n".join(json.dumps(context) for context in contexts) + "\n", encoding="utf-8")
    assert bundle.load_build_contexts(config) == contexts

    config.write_text(json.dumps(contexts), encoding="utf-8")
    with pytest.raises(bundle.BundleError, match="JSON Lines"):
        bundle.load_build_contexts(config)


@pytest.mark.parametrize("keep", [False, True])
def test_old_rounds_are_removed_only_after_publication(tmp_path, keep):
    opc = tmp_path / "options.ini"
    opc.write_text("ALL@ALL@--op_debug_config=dump_cce,oom\n" if keep else "")
    context = {"tasks": [["Add", "ascend910b"]], "opc_config": str(opc)}
    rounds = bundle.KernelSpecBuildDirectory(tmp_path / "rounds")
    rounds.prepare([context])
    first = rounds.current_directory
    opc.write_text("")
    rounds.prepare([context])
    assert first.is_dir()
    rounds.write_install_entry()
    assert first.exists() is keep
    assert rounds.current_directory.is_dir()


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
    config = tmp_path / "config.jsonl"
    config.write_text(json.dumps(context) + "\n")
    rounds = bundle.KernelSpecBuildDirectory(tmp_path / "rounds")
    rounds.prepare([context])
    task = rounds.current_directory / "ascend910b/Add"
    if status is not None:
        (task / ".producer-result.json").write_text(
            json.dumps({"status": status, "source": str(source), "reason": "test result"})
        )
    if status == "filtered":
        bundle.bundle_resources(config, rounds.work_dir, "unused", "unused", "unused")
        assert "file(REMOVE" in rounds.install_script.read_text()
    else:
        with pytest.raises(bundle.BundleError):
            bundle.bundle_resources(config, rounds.work_dir, "unused", "unused", "unused")
        assert not rounds.install_script.exists()


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
    assert hashed_sources == [Path(bundle_request.source_file[0])]
    data = _published(bundle_request)
    assert [m["resource_id"] for m in data] == ["a" * 64, "a" * 64, "b" * 64]
    assert data[0]["source_file"] == "op_impl/dynamic/add.cpp"
    assert data[0]["options"]["common_compile"] == ["-I${source_file_path}/op_impl/dynamic"]
    assert (root / "910b/add_manifest.json").read_bytes() == original


def test_include_mapping_uses_longest_prefix_and_preserves_order(bundle_request):
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
    assert _published(bundle_request)[0]["options"]["common_compile"] == [
        "-I",
        "${source_file_path}/op_impl/dynamic",
        "-isystem${source_file_path}/extra/include",
        "-iquote",
        "${source_file_path}/op_impl/dynamic/not_created",
        "-idirafter${source_file_path}/op_impl/dynamic",
        "-I/untouched",
    ]


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


def test_publish_failure_keeps_previous_output(bundle_request, monkeypatch):
    bundle_request.build()
    before = Path(bundle_request.output).read_bytes()
    (Path(bundle_request.input_dir[0]) / "910b/resources/wrapper.cpp").write_text("changed")
    original = os.replace

    def fail_output(source, dest):
        if Path(dest) == Path(bundle_request.output):
            raise OSError("publish failed")
        return original(source, dest)

    monkeypatch.setattr(bundle.os, "replace", fail_output)
    with pytest.raises(OSError, match="publish failed"):
        bundle_request.build()
    assert Path(bundle_request.output).read_bytes() == before


def test_opc_dump_cce_preserves_exporter_diagnostics(bundle_request, tmp_path):
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


def test_successful_exporter_with_invalid_output_is_rejected(bundle_request):
    bundle_request.build()
    old = Path(bundle_request.output).read_bytes()
    Path(bundle_request.exporter).write_text("#!/bin/sh\nexit 0\n")
    with pytest.raises(OSError):
        bundle_request.build()
    assert Path(bundle_request.output).read_bytes() == old


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
    assert (bundle_dir / "collected" / "0" / "resources/empty_include").is_dir()


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
    assert data["options"]["common_compile"] == ["-I${source_file_path}/op_impl/dynamic"]
    resources = {ctypes.string_at(f.path.data, f.path.size).decode(): f for f in manifest.files[: manifest.count]}
    assert resources["resources/large.h"].size == 1024 * 1024 + 1
    assert ctypes.string_at(resources["resources/large.h"].data, 1) == b"x"


@pytest.mark.parametrize("difference", ["source", "installed_path"])
def test_same_id_source_conflict_preserves_published_bundle(bundle_request, tmp_path, difference):
    bundle_request.build()
    published = Path(bundle_request.output).read_bytes()
    root = Path(bundle_request.input_dir[0])
    path = _unit(root / "duplicate", "a" * 64)
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
    with pytest.raises(bundle.BundleError, match="conflicting resource_id") as error:
        bundle_request.build()
    assert str(path) in str(error.value)
    assert str(root / "910b/add_manifest.json") in str(error.value)
    assert Path(bundle_request.output).read_bytes() == published


@pytest.mark.skipif(not os.environ.get("ASCEND_TEST_COMPILE_EXPORTER"), reason="requires built asc_compile_exporter")
@pytest.mark.parametrize("difference", ["equivalent", "manifest", "resource", "empty_directory", "empty_file"])
def test_real_exporter_owns_id_deduplication_and_preserves_output_on_conflict(bundle_request, difference):
    bundle_request.exporter = os.environ["ASCEND_TEST_COMPILE_EXPORTER"]
    bundle_request.build()
    published = Path(bundle_request.output).read_bytes()
    path = _unit(Path(bundle_request.input_dir[0]) / "duplicate", "a" * 64)
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
    if difference != "equivalent":
        with pytest.raises(bundle.BundleError, match="exporter failed"):
            bundle_request.build()
        assert Path(bundle_request.output).read_bytes() == published
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


def test_bundle_round_relocates_shared_include_outside_kernel_root(bundle_request, tmp_path):
    work = tmp_path / "rounds"
    source = Path(bundle_request.source_file[0])
    shared = tmp_path / "shared"
    shared.mkdir()
    config = tmp_path / "build.jsonl"
    opc = tmp_path / "options.ini"
    opc.write_text("")
    context = {
        "tasks": [["Add", "ascend910b"]],
        "opc_config": str(opc),
        "source_root": str(source.parent),
        "copied_source_root": str(source.parent),
        "source_destination": "op_impl/dynamic",
    }
    config.write_text(
        json.dumps(context) + "\n" + json.dumps({"tasks": [], "source_maps": [[str(shared), "op_impl/common"]]}) + "\n"
    )
    directory = bundle.KernelSpecBuildDirectory(work)
    directory.prepare([context])
    task = bundle.task_directory(str(directory.current_directory), "Add", "ascend910b")
    manifest_path = _unit(task, "a" * 64)
    manifest = json.loads(manifest_path.read_text())
    manifest["options"]["common_compile"] = ["-I" + str(shared)]
    manifest_path.write_text(json.dumps(manifest))
    (task / ".producer-result.json").write_text(
        json.dumps({"status": "completed", "source": str(source), "reason": ""})
    )
    bundle.bundle_resources(str(config), str(work), bundle_request.exporter, "c++", "make")
    output = directory.current_directory / "op_spec/libcust_compile_resource.so"
    published = json.loads(output.read_bytes()[4:])
    assert published[0]["options"]["common_compile"] == ["-I${source_file_path}/op_impl/common"]
