#!/usr/bin/python
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

"""Collect an already completed manifest tree and atomically refresh its resource SO."""

import argparse
import hashlib
import json
import os
import shutil
import re
from pathlib import Path
import subprocess
import tempfile
from contextlib import contextmanager


def safe_source_path(value):
    if (
        not isinstance(value, str)
        or not value
        or "\\" in value
        or "\0" in value
        or value.startswith("/")
        or any(part in ("", ".", "..") for part in value.split("/"))
    ):
        raise ValueError(
            "source_file must be a safe relative POSIX path: " + repr(value)
        )
    return value


def validate_sources(source_root, manifests, source_maps=()):
    """Resolve only referenced sources; preserve the original manifests."""
    roots = [source_root] if isinstance(source_root, Path) else list(source_root)
    files = {}
    for root in roots:
        if not root.exists():
            raise ValueError("Source directory or file does not exist: " + str(root))
        for path in [root] if root.is_file() else sorted(root.rglob("*")):
            if path.is_file():
                files.setdefault(path.name, set()).add(path.resolve())
    installed = {}
    for source, destination in source_maps:
        source = Path(source)
        safe_source_path(destination)
        if not source.exists():
            raise ValueError("Mapped source does not exist: " + str(source))
        for path in [source] if source.is_file() else sorted(source.rglob("*")):
            if path.is_file():
                relative = (
                    destination
                    if source.is_file()
                    else destination + "/" + path.relative_to(source).as_posix()
                )
                installed.setdefault(relative, set()).add(path.resolve())
    resolved = {}
    for manifest in manifests:
        name = safe_source_path(json.loads(manifest.read_text()).get("source_file"))
        # Without an installation map, the caller owns relative source layout.
        if "/" in name and not source_maps:
            continue
        candidates = (
            installed.get(name, set()) if "/" in name else files.get(name, set())
        )
        if not candidates:
            raise ValueError(
                "Expected exactly one source " + name + " in resource " + str(roots)
            )
        if len(candidates) != 1:
            raise ValueError(
                "Ambiguous source_file "
                + name
                + ": "
                + ", ".join(map(str, sorted(candidates)))
                + "; set source_file to a package-relative path"
            )
        selected = next(iter(candidates))
        if "/" in name:
            resolved[manifest] = name
        elif source_maps:
            destinations = sorted(
                key for key, values in installed.items() if selected in values
            )
            if not destinations:
                raise ValueError("Source has no package destination: " + str(selected))
            target = destinations[0]
            if len(installed[target]) != 1:
                raise ValueError("Multiple sources install to " + target)
            resolved[manifest] = target
    return resolved


def relocate_include_paths(document, source_maps):
    """Relocate only explicitly mapped include directories, preserving search order."""
    if not source_maps:
        return
    source_parent = Path(safe_source_path(document["source_file"])).parent
    directories = {}
    for source, destination in source_maps:
        if Path(source).is_dir():
            root = Path(os.path.abspath(source))
            directories.setdefault(root, set()).add(Path(safe_source_path(destination)))

    def relocate(value):
        if not os.path.isabs(value) or "${" in value:
            return value
        path = Path(os.path.abspath(value))
        root = next(
            (parent for parent in (path, *path.parents) if parent in directories), None
        )
        if root is None:
            return value
        destinations = directories[root]
        if len(destinations) != 1:
            raise ValueError("Ambiguous include installation mapping: " + value)
        destination = next(iter(destinations)) / path.relative_to(root)
        relative = os.path.relpath(destination, source_parent)
        return "${source_file_path}" + ("/" + relative if relative != "." else "")

    def rewrite(argv):
        index = 0
        while index < len(argv):
            token = argv[index]
            if token in ("-I", "-isystem", "-iquote", "-idirafter"):
                if index + 1 < len(argv):
                    argv[index + 1] = relocate(argv[index + 1])
                index += 2
                continue
            for prefix in ("-I", "-isystem", "-iquote", "-idirafter"):
                if token.startswith(prefix) and len(token) > len(prefix):
                    argv[index] = prefix + relocate(token[len(prefix) :])
                    break
            index += 1

    for options in document.get("options", {}).values():
        rewrite(options)
    for kernel in document.get("kernels", []):
        for obj in kernel.get("objects", []):
            for command in obj.get("commands", []):
                if command.get("type") == "compile":
                    rewrite(command["cmd"])


def stage_unique_resources(manifests, destination, source_paths=None, source_maps=()):
    selected = {}
    for manifest in sorted(manifests):
        document = json.loads(manifest.read_text())
        if source_paths and manifest in source_paths:
            document["source_file"] = source_paths[manifest]
        relocate_include_paths(document, source_maps)
        resource_id = document.get("resource_id", "")
        if not re.fullmatch(r"[0-9a-f]{64}", resource_id):
            raise ValueError("Invalid resource_id in " + str(manifest))
        if resource_id in selected:
            continue
        selected[resource_id] = manifest
        output = destination / resource_id
        output.mkdir(parents=True)
        resource_path = document.get("resource_path", "")
        if resource_path:
            candidate = manifest.parent / resource_path
            if candidate.is_symlink():
                raise ValueError("Resource path cannot be a symlink: " + str(candidate))
            source = candidate.resolve()
            if (
                not source.is_relative_to(manifest.parent.resolve())
                or not source.is_dir()
            ):
                raise ValueError(
                    "Resource path must be inside manifest directory: " + str(manifest)
                )
            if any(path.is_symlink() for path in source.rglob("*")):
                raise ValueError(
                    "Resource files cannot contain symlinks: " + str(source)
                )
            shutil.copytree(source, output / "resources")
            document["resource_path"] = "resources"
        (output / "kernel_manifest.json").write_text(
            json.dumps(document, indent=2) + "\n"
        )
    return len(selected)


@contextmanager
def bundle_work_directory(work, keep):
    directory = Path(tempfile.mkdtemp(prefix=".bundle-", dir=work))
    try:
        yield directory
    finally:
        if keep:
            print("JIT temporary files and build log:", directory)
        else:
            shutil.rmtree(directory, ignore_errors=True)


def keep_temporary_files(explicit, opc_config):
    if explicit:
        return True
    if not opc_config:
        return False
    content = Path(opc_config).read_text()
    # The existing --save-temp-files option is written as dump_cce in OPC configuration.
    return (
        re.search(r"(?:^|[,@=; ])dump_cce(?:$|[,@; \n])", content, re.MULTILINE)
        is not None
    )


def main():
    parser = argparse.ArgumentParser()
    for key in ("output", "exporter", "cxx", "make"):
        parser.add_argument("--" + key, required=True)
    parser.add_argument("--input-dir", action="append", required=True)
    parser.add_argument("--save-temp-files", action="store_true")
    parser.add_argument(
        "--opc-config", help="Existing custom_opc_options.ini for save-temp-files"
    )
    parser.add_argument(
        "--source-map",
        nargs=2,
        action="append",
        default=[],
        metavar=("BUILD_SOURCE", "PACKAGE_PATH"),
    )
    parser.add_argument("--source-dir", action="append", default=[])
    parser.add_argument("--source-file", action="append", default=[])
    parser.add_argument(
        "--work-dir",
        help="Cache and compiler work directory, outside the binary package",
    )
    args = parser.parse_args()
    keep_temp = keep_temporary_files(args.save_temp_files, args.opc_config)
    roots = [Path(path) for path in args.input_dir]
    output = Path(args.output)
    manifests = []
    digest = hashlib.sha256()
    digest.update(str(keep_temp).encode())
    for root in roots:
        current = sorted(root.rglob("*_manifest.json"))
        if not current:
            raise RuntimeError("No generated manifest under " + str(root))
        manifests.extend(current)
        digest.update(str(root.resolve()).encode() + b"\0")
        for path in sorted(root.rglob("*")):
            if path.is_file():
                name = path.relative_to(root).as_posix().encode()
                data = path.read_bytes()
                digest.update(
                    len(name).to_bytes(8, "big")
                    + name
                    + len(data).to_bytes(8, "big")
                    + data
                )
    if not args.source_dir and not args.source_file:
        raise ValueError("At least one source directory or file is required")
    source_paths = validate_sources(
        [Path(path) for path in args.source_dir + args.source_file],
        manifests,
        args.source_map,
    )
    digest.update(json.dumps(args.source_map, sort_keys=True).encode())
    # Source layout can change without changing the manifest or mapping roots.
    digest.update(
        json.dumps(
            sorted((str(path.resolve()), name) for path, name in source_paths.items())
        ).encode()
    )
    digest.update(Path(__file__).read_bytes())

    for tool in (args.exporter, args.cxx, args.make):
        digest.update(tool.encode() + b"\0")
        digest.update(Path(tool).read_bytes())
    key = digest.hexdigest()
    output_key = hashlib.sha256(str(output.resolve()).encode()).hexdigest()[:16]
    work = (
        Path(args.work_dir)
        if args.work_dir
        else roots[0].parent / ".kernel_spec_cache" / output_key
    )
    work.mkdir(parents=True, exist_ok=True)
    stamp = work / (output.name + ".stamp.json")
    if output.is_file() and stamp.is_file():
        try:
            previous = json.loads(stamp.read_text())
        except (OSError, ValueError):
            previous = None
        if previous == {
            "inputs": key,
            "output": hashlib.sha256(output.read_bytes()).hexdigest(),
        }:
            print("JIT bundle unchanged:", output)
            return

    output.parent.mkdir(parents=True, exist_ok=True)
    with bundle_work_directory(work, keep_temp) as temporary:
        result = Path(temporary) / output.name
        collected = Path(temporary) / "inputs"
        count = stage_unique_resources(
            manifests, collected, source_paths, args.source_map
        )
        command = [
            args.exporter,
            "--input-dir",
            str(collected),
            "--output",
            str(result),
            "--cxx",
            args.cxx,
            "--make",
            args.make,
        ]
        if keep_temp:
            command.append("--save-temp-files")
        environment = os.environ.copy()
        environment["TMPDIR"] = str(temporary)
        with (Path(temporary) / "build.log").open("w") as log:
            result_code = subprocess.run(
                command, env=environment, stdout=log, stderr=subprocess.STDOUT
            )
        if result_code.returncode:
            print((Path(temporary) / "build.log").read_text())
            result_code.check_returncode()
        record = {
            "inputs": key,
            "output": hashlib.sha256(result.read_bytes()).hexdigest(),
        }
        # Publish from the output filesystem even when the work directory is on another mount.
        descriptor, pending = tempfile.mkstemp(
            prefix="." + output.name + ".", suffix=".tmp", dir=output.parent
        )
        try:
            with os.fdopen(descriptor, "wb") as destination, result.open(
                "rb"
            ) as source:
                shutil.copyfileobj(source, destination)
                destination.flush()
                os.fsync(destination.fileno())
            os.chmod(pending, result.stat().st_mode & 0o777)
            os.replace(pending, output)
        finally:
            Path(pending).unlink(missing_ok=True)
        marker = Path(temporary) / "stamp.json"
        marker.write_text(json.dumps(record, indent=2) + "\n")
        os.replace(marker, stamp)
    print(
        "JIT bundle published:",
        output,
        "manifests:",
        len(manifests),
        "unique resources:",
        count,
    )


if __name__ == "__main__":
    main()
