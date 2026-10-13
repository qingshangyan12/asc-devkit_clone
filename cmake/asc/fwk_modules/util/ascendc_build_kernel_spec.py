#!/usr/bin/env python3
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

"""Collect and relocate Kernel Spec resources for RUN package delivery."""

import argparse
import copy
import fcntl
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import subprocess
import tempfile
import sys

MAX_MANIFEST_BYTES = 8 * 1024 * 1024
MAX_RESOURCE_BYTES = 256 * 1024 * 1024
MAX_UNIT_BYTES = 512 * 1024 * 1024
MAX_BUNDLE_BYTES = 1024 * 1024 * 1024
MAX_MANIFESTS = 4096
MAX_UNIT_FILES = 65536
MAX_BUNDLE_FILES = 131072
SOURCE_MARKER = "${source_file_path}"
INCLUDE_FLAGS = ("-isystem", "-iquote", "-idirafter", "-I")


class BundleError(RuntimeError):
    pass


def _checked_path(value):
    path = Path(os.path.abspath(value))
    if any(parent.is_symlink() for parent in (path, *path.parents)):
        raise BundleError(f"symlink is not permitted: {path}")
    return path


def _relative(value):
    if not isinstance(value, str) or not value or "\\" in value or any(c in value for c in "\n\r\0"):
        raise BundleError(f"invalid relative path: {value!r}")
    path = PurePosixPath(value)
    if path.is_absolute() or ".." in path.parts or path == PurePosixPath("."):
        raise BundleError(f"path must stay below its root: {value}")
    return path


def _files(root):
    root = _checked_path(root)
    if not root.is_dir():
        raise BundleError(f"directory is missing: {root}")

    def walk_error(error):
        raise error

    for directory, dirs, files in os.walk(root, onerror=walk_error):
        if len(Path(directory).relative_to(root).parts) > 64:
            raise BundleError(f"directory nesting exceeds 64: {directory}")
        dirs.sort()
        for name in sorted(dirs + files):
            path = _checked_path(Path(directory) / name)
            if name in files:
                if not path.is_file():
                    raise BundleError(f"non-regular file: {path}")
                yield path


def _hash_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _source_maps(values):
    maps = {}
    for source, installed in values:
        root = _checked_path(source)
        target = _relative(installed).as_posix()
        if root in maps and maps[root] != target:
            raise BundleError(f"conflicting source-map: {root}")
        maps[root] = target
    return sorted(maps.items(), key=lambda item: (-len(item[0].parts), str(item[0])))


def _map_absolute(path, maps):
    for root, installed in maps:
        try:
            tail = path.relative_to(root)
        except ValueError:
            continue
        return str(PurePosixPath(installed) / PurePosixPath(tail.as_posix()))
    return None


def _source_index(request, maps):
    files = set()
    for directory in request.source_dir:
        files.update(_files(directory))
    for value in request.source_file:
        path = _checked_path(value)
        if not path.is_file():
            raise BundleError(f"source file is missing: {path}")
        files.add(path)
    installed = {}
    for path in sorted(files):
        relative = _map_absolute(path, maps)
        if relative is None:
            raise BundleError(f"source file has no source-map: {path}")
        installed.setdefault(relative, []).append(path)
    return files, installed


def _resolve_source(value, source_files, installed):
    relative = _relative(value)
    if len(relative.parts) == 1:
        candidates = [p for p in source_files if p.name == value]
    else:
        candidates = installed.get(relative.as_posix(), [])
    if len(candidates) != 1:
        raise BundleError(f"missing or ambiguous source_file {value}: {len(candidates)} candidates")
    source = candidates[0]
    package_path = next(key for key, paths in installed.items() if source in paths)
    return source, package_path


def _rewrite_include(path, maps, source_parent):
    if path == SOURCE_MARKER or path.startswith(SOURCE_MARKER + "/"):
        suffix = path[len(SOURCE_MARKER) :].lstrip("/")
        if suffix:
            _relative(suffix)
        return SOURCE_MARKER + "/" + str(PurePosixPath(source_parent) / suffix)
    if not os.path.isabs(path):
        return path
    mapped = _map_absolute(_checked_path(path), maps)
    return SOURCE_MARKER + "/" + mapped if mapped is not None else path


def _rewrite_tokens(tokens, maps, source_parent):
    result = []
    index = 0
    while index < len(tokens):
        token = tokens[index]
        if not isinstance(token, str):
            raise BundleError("compile/link options must contain string argv tokens")
        flag = next((flag for flag in INCLUDE_FLAGS if token.startswith(flag)), None)
        if flag and token == flag:
            if index + 1 == len(tokens) or not isinstance(tokens[index + 1], str):
                raise BundleError(f"missing include path after {flag}")
            result.extend([flag, _rewrite_include(tokens[index + 1], maps, source_parent)])
            index += 2
        elif flag:
            suffix = token[len(flag) :]
            separator = "=" if suffix.startswith("=") else ""
            result.append(flag + separator + _rewrite_include(suffix.lstrip("="), maps, source_parent))
            index += 1
        else:
            result.append(token)
            index += 1
    return result


def _relocate(manifest, maps, package_source):
    result = copy.deepcopy(manifest)
    result["source_file"] = package_source
    parent = str(PurePosixPath(package_source).parent)
    for key, tokens in result.get("options", {}).items():
        result["options"][key] = _rewrite_tokens(tokens, maps, parent)
    for kernel in result["kernels"]:
        if "link_options" in kernel:
            kernel["link_options"] = _rewrite_tokens(kernel["link_options"], maps, parent)
        for obj in kernel.get("objects", []):
            for command in obj.get("commands", []):
                command["cmd"] = _rewrite_tokens(command["cmd"], maps, parent)
    return result


def _check_resource_references(value, root, resource_root):
    if isinstance(value, dict):
        for item in value.values():
            _check_resource_references(item, root, resource_root)
    elif isinstance(value, list):
        for item in value:
            _check_resource_references(item, root, resource_root)
    elif isinstance(value, str) and "${resource}/" in value:
        relative = value.split("${resource}/", 1)[1]
        path = _checked_path(root / _relative(relative))
        if resource_root not in path.parents and path != resource_root:
            raise BundleError(f"resource reference is outside resource_path: {path}")
        if not path.exists():
            raise BundleError(f"missing resource reference: {path}")


def _collect(request, maps, sources, installed):
    units = {}
    fingerprints = []
    for directory in sorted(set(request.input_dir)):
        manifests = [p for p in _files(directory) if p.name.endswith("_manifest.json")]
        if not manifests:
            raise BundleError(f"no manifests in input directory: {directory}")
        for path in manifests:
            if path.stat().st_size > MAX_MANIFEST_BYTES:
                raise BundleError(f"manifest is too large: {path}")
            manifest = json.loads(path.read_text(encoding="utf-8"))
            if not isinstance(manifest, dict) or not re.fullmatch(
                r"[0-9a-f]{64}", str(manifest.get("resource_id", ""))
            ):
                raise BundleError(f"invalid resource_id: {path}")
            if not isinstance(manifest.get("kernels"), list) or not manifest["kernels"]:
                raise BundleError(f"manifest has no kernels: {path}")
            source, package_source = _resolve_source(manifest.get("source_file"), sources, installed)
            resource_root = _checked_path(path.parent / _relative(manifest.get("resource_path")))
            files = list(_files(resource_root))
            if len(files) > MAX_UNIT_FILES:
                raise BundleError(f"too many resource files: {path}")
            if any(p.stat().st_size > MAX_RESOURCE_BYTES for p in files):
                raise BundleError(f"resource file is too large: {path}")
            if sum(p.stat().st_size for p in files) > MAX_UNIT_BYTES:
                raise BundleError(f"manifest resources are too large: {path}")
            _check_resource_references(manifest, path.parent, resource_root)
            relocated = _relocate(manifest, maps, package_source)
            encoded = json.dumps(relocated, sort_keys=True, separators=(",", ":")).encode()
            if len(encoded) > MAX_MANIFEST_BYTES:
                raise BundleError(f"relocated manifest is too large: {path}")
            fingerprints.append(
                [
                    str(path),
                    _hash_file(path),
                    str(source),
                    _hash_file(source),
                    package_source,
                    [(str(p.relative_to(path.parent)), _hash_file(p)) for p in files],
                ]
            )
            units.setdefault(manifest["resource_id"], (path, encoded, files))
    ordered = sorted(units.items())
    if len(ordered) > MAX_MANIFESTS:
        raise BundleError("too many distinct manifests")
    if sum(len(unit[2]) for _, unit in ordered) > MAX_BUNDLE_FILES:
        raise BundleError("too many bundle resource files")
    if sum(len(unit[1]) + sum(p.stat().st_size for p in unit[2]) for _, unit in ordered) > MAX_BUNDLE_BYTES:
        raise BundleError("bundle payload is too large")
    return ordered, fingerprints


def _tool(value):
    found = shutil.which(value)
    if not found:
        raise BundleError(f"executable tool is missing: {value}")
    return Path(found).resolve()


def _keep_temp(config):
    if not config:
        return False
    path = _checked_path(config)
    if not path.is_file():
        raise BundleError(f"OPC config is missing: {path}")
    for line in path.read_text(encoding="utf-8").splitlines():
        for option in line.split("@")[2:]:
            if option.startswith("--op_debug_config=") and "dump_cce" in option.split("=", 1)[1].split(";"):
                return True
    return False


def _atomic_write(output, write):
    descriptor, name = tempfile.mkstemp(prefix="." + output.name + ".", dir=output.parent)
    try:
        with os.fdopen(descriptor, "wb") as stream:
            write(stream)
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(name, output)
        directory_fd = os.open(output.parent, os.O_RDONLY | os.O_DIRECTORY)
        try:
            os.fsync(directory_fd)
        finally:
            os.close(directory_fd)
    finally:
        if os.path.exists(name):
            os.unlink(name)


def build_bundle(request):
    """Validate inputs, reuse a verified cache or publish a newly exported RUN SO."""
    output = _checked_path(request.output)
    if output.suffix != ".so":
        raise BundleError("RUN resource output must have the .so suffix")
    work = _checked_path(request.work_dir)
    for value in request.input_dir:
        root = _checked_path(value)
        if root == work or root in work.parents or root in output.parents:
            raise BundleError(f"work/output overlap manifest input directory: {root}")
    output.parent.mkdir(parents=True, exist_ok=True)
    work.mkdir(parents=True, exist_ok=True)
    lock_key = hashlib.sha256(str(output).encode()).hexdigest()
    # Output ownership, rather than work-dir ownership, serializes separate callers.
    with open(_checked_path(output.parent / ("." + output.name + ".lock")), "a+") as lock:
        fcntl.flock(lock.fileno(), fcntl.LOCK_EX)
        return _build_locked(request, output, work, lock_key)


def _build_locked(request, output, work, lock_key):
    maps = _source_maps(request.source_map)
    sources, installed = _source_index(request, maps)
    units, fingerprint = _collect(request, maps, sources, installed)
    tools = {name: _tool(getattr(request, name)) for name in ("exporter", "cxx", "make")}
    keep = _keep_temp(request.opc_config)
    if request.jobs < 1:
        raise BundleError("jobs must be positive")
    key_data = {
        "inputs": fingerprint,
        "maps": [(str(p), dst) for p, dst in maps],
        "keep": keep,
        "jobs": request.jobs,
        "script": _hash_file(Path(__file__)),
        "tools": {name: [str(path), _hash_file(path)] for name, path in tools.items()},
    }
    key = hashlib.sha256(json.dumps(key_data, sort_keys=True).encode()).hexdigest()
    stamp = _checked_path(work / (lock_key + ".stamp.json"))
    try:
        cached = json.loads(stamp.read_text(encoding="utf-8"))
        if cached.get("key") == key and output.is_file() and cached.get("output_hash") == _hash_file(output):
            return False
    except (OSError, ValueError, AttributeError):
        pass
    workspace = Path(tempfile.mkdtemp(prefix=".bundle-", dir=work))
    try:
        collected = workspace / "collected"
        for resource_id, (path, encoded, _files_to_copy) in units:
            destination = collected / resource_id
            destination.mkdir(parents=True)
            resource_path = json.loads(encoded)["resource_path"]
            shutil.copytree(path.parent / resource_path, destination / resource_path, symlinks=True)
            (destination / "resource_manifest.json").write_bytes(encoded)
        staged = workspace / "libcust_compile_db.so"
        argv = [
            str(tools["exporter"]),
            "--input-dir",
            str(collected),
            "--output",
            str(staged),
            "--cxx",
            str(tools["cxx"]),
            "--make",
            str(tools["make"]),
            "--jobs",
            str(request.jobs),
        ]
        if keep:
            argv.append("--save-temp-files")
        env = dict(os.environ, TMPDIR=str(workspace))
        with open(workspace / "build.log", "wb") as log:
            result = subprocess.run(argv, stdout=log, stderr=subprocess.STDOUT, env=env, check=False)
        if result.returncode:
            with open(workspace / "build.log", "rb") as log:
                log.seek(max(0, log.seek(0, os.SEEK_END) - 8192))
                detail = log.read().decode("utf-8", errors="replace")
            raise BundleError(f"exporter failed ({result.returncode}): {detail}")
        with open(_checked_path(staged), "rb") as source:
            if source.read(4) != b"\x7fELF":
                raise BundleError("exporter did not produce an ELF resource bundle")
            source.seek(0)
            _atomic_write(output, lambda stream: shutil.copyfileobj(source, stream))
        stamp_data = json.dumps({"key": key, "output_hash": _hash_file(output)}, sort_keys=True).encode()
        _atomic_write(stamp, lambda stream: stream.write(stamp_data))
        return True
    finally:
        if keep:
            print(f"Kernel Spec diagnostics retained: {workspace}")
        else:
            shutil.rmtree(workspace)


def reset_tasks(request):
    from kernel_spec_options import task_directory

    state = Path(request.state)
    state.parent.mkdir(parents=True, exist_ok=True)
    Path(request.install_status).unlink(missing_ok=True)
    previous = json.loads(state.read_text(encoding="utf-8")) if state.is_file() else []
    tasks = []
    for op_type, soc in request.task:
        directory = task_directory(request.opc_config, op_type, soc, request.task_id)
        if directory is not None:
            tasks.append({"op_type": op_type, "soc": soc, "directory": str(directory)})
    # All producer targets run on every build. Clear only this package/kernel's namespaces.
    for value in {task["directory"] for task in previous + tasks}:
        directory = Path(value)
        if directory.parent.parent.name != ".ascendc-" + request.task_id or any(
            parent.is_symlink() for parent in (directory, *directory.parents)
        ):
            raise ValueError("Invalid producer cleanup directory: " + value)
        if directory.exists():
            shutil.rmtree(directory)
    state.write_text(json.dumps(tasks, sort_keys=True), encoding="utf-8")


def build_package(request):
    status = Path(request.install_status)
    status.unlink(missing_ok=True)
    contexts = json.loads(Path(request.package_state).read_text(encoding="utf-8"))
    output = Path(request.output)
    inputs = []
    maps = []
    sources = set()
    configs = []
    for context in contexts:
        tasks = json.loads(Path(context["state"]).read_text(encoding="utf-8"))
        if not tasks:
            continue
        configs.append(context["opc_config"])
        maps.append((context["source_root"], context["source_destination"]))
        source_root = Path(context["source_root"]).resolve()
        copied_root = Path(context["copied_source_root"])
        for task in tasks:
            directory = Path(task["directory"])
            receipt = directory / ".producer-success"
            if not receipt.is_file():
                raise ValueError("Kernel Spec producer did not complete: " + task["op_type"] + "/" + task["soc"])
            source = Path(receipt.read_text(encoding="utf-8"))
            relative = source.relative_to(source_root)
            # The source map describes the existing source installation, not a guessed new layout.
            if not (copied_root / relative).is_file():
                raise ValueError("Kernel Spec source is missing from the install input: " + str(source))
            sources.add(str(source))
            inputs.append(str(directory))
    if not configs:
        output.unlink(missing_ok=True)
        for stamp in Path(request.work_dir).glob("*.stamp.json"):
            stamp.unlink()
        status.write_text(
            "set(ASCENDC_KERNEL_SPEC_READY TRUE)\nset(ASCENDC_KERNEL_SPEC_HAS_RESOURCES FALSE)\n", encoding="utf-8"
        )
        return

    # Retain temporary resources if any contributing kernel enables the existing debug setting.
    combined_config = Path(request.package_state).with_name("bundle_options.ini")
    combined_config.write_text(
        "\n".join(Path(config).read_text(encoding="utf-8") for config in sorted(set(configs))), encoding="utf-8"
    )
    build_bundle(
        argparse.Namespace(
            input_dir=inputs,
            source_dir=[],
            source_file=sorted(sources),
            source_map=maps,
            output=str(output),
            work_dir=request.work_dir,
            exporter=request.exporter,
            cxx=request.cxx,
            make=request.make,
            opc_config=str(combined_config),
            jobs=8,
        )
    )
    status.write_text(
        "set(ASCENDC_KERNEL_SPEC_READY TRUE)\nset(ASCENDC_KERNEL_SPEC_HAS_RESOURCES TRUE)\n", encoding="utf-8"
    )


def engineering_main(action, arguments):
    """Internal CMake entry points; direct aggregation keeps its existing CLI."""
    parser = argparse.ArgumentParser(description=__doc__)
    if action == "reset":
        for name in ("state", "install-status", "opc-config", "task-id"):
            parser.add_argument("--" + name, required=True)
        parser.add_argument("--task", nargs=2, action="append", default=[], metavar=("OP", "SOC"))
    else:
        for name in ("package-state", "install-status", "output", "work-dir", "exporter", "cxx", "make"):
            parser.add_argument("--" + name, required=True)
    request = parser.parse_args(arguments)
    if action == "reset" and not re.fullmatch(r"[0-9a-f]{64}", request.task_id):
        parser.error("invalid internal task ID")
    try:
        if action == "reset":
            reset_tasks(request)
        else:
            build_package(request)
    except (BundleError, OSError, ValueError, KeyError, TypeError) as error:
        parser.exit(1, f"Kernel Spec build failed: {error}\n")


def main():
    if len(sys.argv) > 1 and sys.argv[1] in {"reset", "bundle"}:
        engineering_main(sys.argv[1], sys.argv[2:])
        return
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input-dir", action="append", required=True)
    parser.add_argument("--source-dir", action="append", default=[])
    parser.add_argument("--source-file", action="append", default=[])
    parser.add_argument("--source-map", action="append", nargs=2, default=[], metavar=("BUILD_ROOT", "PACKAGE_PATH"))
    for option in ("output", "work-dir", "exporter", "cxx", "make"):
        parser.add_argument("--" + option, required=True)
    parser.add_argument("--opc-config")
    parser.add_argument("--jobs", type=int, default=8)
    args = parser.parse_args()
    try:
        changed = build_bundle(args)
    except (BundleError, OSError, ValueError, KeyError, TypeError) as error:
        parser.exit(1, f"Kernel Spec aggregation failed: {error}\n")
    print("Kernel Spec bundle published" if changed else "Kernel Spec bundle cache hit")


if __name__ == "__main__":
    main()
