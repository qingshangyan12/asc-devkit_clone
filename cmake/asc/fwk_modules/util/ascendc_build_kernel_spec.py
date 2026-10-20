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
from dataclasses import dataclass, field
import fcntl
import hashlib
import json
import logging
import os
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import tempfile
import sys

SOURCE_MARKER = "${source_file_path}"
INCLUDE_FLAGS = ("-isystem", "-iquote", "-idirafter", "-include", "-I")


class BundleError(RuntimeError):
    pass


def task_directory(root, current_op_type, soc):
    """Return the resource directory for one operator and chip in this build."""
    if not root or not os.path.isabs(root):
        raise ValueError("kernel-spec-dir must be an absolute path")
    return Path(root) / soc.lower() / current_op_type


def load_build_contexts(config_path):
    """Load one JSON object per line; array configuration is intentionally unsupported."""
    lines = [line for line in Path(config_path).read_text(encoding="utf-8").splitlines() if line.strip()]
    if lines and lines[0].lstrip().startswith("["):
        raise BundleError("JSON array configuration is no longer supported; use JSON Lines")
    try:
        contexts = [json.loads(line) for line in lines]
    except json.JSONDecodeError as error:
        raise BundleError(f"invalid JSON Lines context: {error}") from error
    if any(not isinstance(context, dict) for context in contexts):
        raise BundleError("each JSON Lines context must be an object")
    return contexts


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


class KernelSpecBuildDirectory:
    """Manage one package's build rounds and successful installation entry."""

    def __init__(self, work_dir):
        self.work_dir = _checked_path(work_dir)
        self.install_script = self.work_dir / "install.cmake"

    @property
    def current_directory(self):
        work = self.work_dir
        directory = _checked_path(json.loads((work / "current_build.json").read_text(encoding="utf-8"))["directory"])
        if directory.parent != work or not directory.name.startswith("build-") or not directory.is_dir():
            raise BundleError("Invalid current build directory: " + str(directory))
        return directory

    def prepare(self, contexts):
        work = self.work_dir
        work.mkdir(parents=True, exist_ok=True)
        self.install_script.unlink(missing_ok=True)
        (work / "current_build.json").unlink(missing_ok=True)
        directory = Path(tempfile.mkdtemp(prefix="build-", dir=work))
        for context in contexts:
            for op_type, soc in context["tasks"]:
                task_directory(str(directory), op_type, soc).mkdir(parents=True, exist_ok=True)
        (work / "current_build.json").write_text(json.dumps({"directory": str(directory)}), encoding="utf-8")

    def write_install_entry(self, output=None):
        if output is None:
            command = 'file(REMOVE "$ENV{DESTDIR}${spec_destination}/libcust_compile_resource.so")\n'
        else:
            output_string = str(output).replace("\\", "\\\\").replace('"', '\\"').replace("$", "\\$")
            command = 'file(INSTALL DESTINATION "${spec_destination}" TYPE FILE FILES "' + output_string + '")\n'
        _atomic_write(self.install_script, lambda stream: stream.write(command.encode("utf-8")))


class KernelSpecSourceMapper:
    """Own source candidates and their installed paths for manifest relocation."""

    def __init__(self, source_map, source_file=()):
        self.source_files = source_file
        maps = {}
        for source, installed in source_map:
            root = _checked_path(source)
            target = _relative(installed).as_posix()
            if root in maps and maps[root] != target:
                raise BundleError(f"conflicting source-map: {root}")
            maps[root] = target
        self.maps = sorted(maps.items(), key=lambda item: (-len(item[0].parts), str(item[0])))

    def map_path(self, path):
        for root, installed in self.maps:
            try:
                tail = path.relative_to(root)
            except ValueError:
                continue
            return str(PurePosixPath(installed) / PurePosixPath(tail.as_posix()))
        return None

    def resolve(self, value, bound_source=None):
        """Resolve only the referenced source; unrelated files do not need installation maps."""
        relative = _relative(value)

        def matches(path):
            if len(relative.parts) == 1:
                return path.name == value
            return self.map_path(path) == relative.as_posix()

        if bound_source is not None:
            candidates = {_checked_path(bound_source)}
            if not matches(next(iter(candidates))):
                raise BundleError(f"source_file does not match producer source: {value}: {bound_source}")
        else:
            candidates = set()
            for source_value in self.source_files:
                path = Path(os.path.abspath(source_value))
                if matches(path):
                    candidates.add(path)
        if len(candidates) != 1:
            raise BundleError(f"missing or ambiguous source_file {relative}: {len(candidates)} candidates")
        source = _checked_path(candidates.pop())
        if not source.is_file():
            raise BundleError(f"source file is missing: {source}")
        installed = self.map_path(source)
        if installed is None:
            raise BundleError(f"source file has no source-map: {source}")
        return source, installed

    def _rewrite_include(self, path, source_parent):
        if path == SOURCE_MARKER or path.startswith(SOURCE_MARKER + "/"):
            suffix = path[len(SOURCE_MARKER) :].lstrip("/")
            if suffix:
                _relative(suffix)
            return SOURCE_MARKER + "/" + str(PurePosixPath(source_parent) / suffix)
        if not os.path.isabs(path):
            return path
        mapped = self.map_path(Path(os.path.abspath(path)))
        if mapped is None:
            # External dependencies are supplied by the consumer and keep their original paths.
            return path
        _checked_path(path)
        return SOURCE_MARKER + "/" + mapped

    def _rewrite_tokens(self, tokens, source_parent):
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
                result.extend([flag, self._rewrite_include(tokens[index + 1], source_parent)])
                index += 2
            elif flag:
                suffix = token[len(flag) :]
                separator = "=" if suffix.startswith("=") else ""
                result.append(flag + separator + self._rewrite_include(suffix.lstrip("="), source_parent))
                index += 1
            else:
                result.append(token)
                index += 1
        return result

    def relocate(self, manifest, package_source):
        result = copy.deepcopy(manifest)
        result["source_file"] = package_source
        parent = str(PurePosixPath(package_source).parent)
        for key, tokens in result.get("options", {}).items():
            result["options"][key] = self._rewrite_tokens(tokens, parent)
        for kernel in result["kernels"]:
            if "link_options" in kernel:
                kernel["link_options"] = self._rewrite_tokens(kernel["link_options"], parent)
            for obj in kernel.get("objects", []):
                for command in obj.get("commands", []):
                    command["cmd"] = self._rewrite_tokens(command["cmd"], parent)
        return result


@dataclass
class ResourceUnit:
    manifest_path: Path
    manifest_json: bytes
    resource_root: Path
    installed_source: str
    source_hash: str

    def source_delivery_signature(self):
        # Exporter sees packaged resources, but cannot inspect the separately installed main source.
        return self.installed_source, self.source_hash


@dataclass
class KernelSpecBundleBuilder:
    """Collect one delivery's resources and export its shared library."""

    input_dir: list
    output: str
    work_dir: str
    exporter: str
    cxx: str
    make: str
    source_map: list = field(default_factory=list)
    source_file: list = field(default_factory=list)
    opc_config: str = None
    jobs: int = 8
    source_bindings: dict = field(default_factory=dict)

    def _collect(self, mapper):
        units = []
        sources_by_id = {}
        bindings = self.source_bindings
        for directory in sorted(set(self.input_dir)):
            manifests = [p for p in _files(directory) if p.name.endswith("_manifest.json")]
            if not manifests:
                logging.warning("Kernel Spec: no manifests in input directory, skipping: %s", directory)
                continue
            for path in manifests:
                manifest = json.loads(path.read_text(encoding="utf-8"))
                resource_id = manifest.get("resource_id")
                # Source consistency requires a key; exporter validates the complete manifest.
                if not isinstance(resource_id, str) or not resource_id:
                    raise BundleError(f"missing resource_id for aggregation: {path}")
                source, package_source = mapper.resolve(
                    manifest.get("source_file"), bindings.get(str(_checked_path(directory)))
                )
                resource_root = _checked_path(path.parent / _relative(manifest.get("resource_path")))
                relocated = mapper.relocate(manifest, package_source)
                encoded = json.dumps(relocated, sort_keys=True, separators=(",", ":"), allow_nan=False).encode()
                unit = ResourceUnit(path, encoded, resource_root, package_source, _hash_file(source))
                previous = sources_by_id.get(resource_id)
                if previous is not None and previous.source_delivery_signature() != unit.source_delivery_signature():
                    raise BundleError(f"conflicting resource_id {resource_id}: {previous.manifest_path} and {path}")
                if previous is None:
                    sources_by_id[resource_id] = unit
                # Forward every candidate so exporter owns resource comparison and deduplication.
                units.append((resource_id, unit))
        return sorted(units, key=lambda item: (item[0], str(item[1].manifest_path)))

    def build(self):
        """Return whether a RUN resource SO was exported from the delivery inputs."""
        output = _checked_path(self.output)
        if output.suffix != ".so":
            raise BundleError("RUN resource output must have the .so suffix")
        work = _checked_path(self.work_dir)
        for value in self.input_dir:
            root = _checked_path(value)
            if root == work or root in work.parents or root in output.parents:
                raise BundleError(f"work/output overlap manifest input directory: {root}")
        output.parent.mkdir(parents=True, exist_ok=True)
        work.mkdir(parents=True, exist_ok=True)
        # Output ownership, rather than work-dir ownership, serializes separate callers.
        with open(_checked_path(output.parent / ("." + output.name + ".lock")), "a+") as lock:
            fcntl.flock(lock.fileno(), fcntl.LOCK_EX)
            return self._export(output, work)

    def _export(self, output, work):
        mapper = KernelSpecSourceMapper(self.source_map, self.source_file)
        units = self._collect(mapper)
        if not units:
            output.unlink(missing_ok=True)
            return False
        tools = {name: _tool(getattr(self, name)) for name in ("exporter", "cxx", "make")}
        keep = _keep_temp(self.opc_config)
        if self.jobs < 1:
            raise BundleError("jobs must be positive")
        bundle_dir = Path(tempfile.mkdtemp(prefix=".bundle-", dir=work))
        try:
            collected = bundle_dir / "collected"
            for index, (_resource_id, unit) in enumerate(units):
                # Use private ordinal directories; resource IDs are data, never filesystem paths.
                destination = collected / str(index)
                destination.mkdir(parents=True)
                resource_path = unit.resource_root.relative_to(unit.manifest_path.parent)
                shutil.copytree(unit.resource_root, destination / resource_path, symlinks=True)
                (destination / "resource_manifest.json").write_bytes(unit.manifest_json)
            staged = bundle_dir / "libcust_compile_resource.so"
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
                str(self.jobs),
            ]
            if keep:
                argv.append("--save-temp-files")
            env = dict(os.environ, TMPDIR=str(bundle_dir))
            with open(bundle_dir / "build.log", "wb") as log:
                result = subprocess.run(argv, stdout=log, stderr=subprocess.STDOUT, env=env, check=False)
            if result.returncode:
                with open(bundle_dir / "build.log", "rb") as log:
                    log.seek(max(0, log.seek(0, os.SEEK_END) - 8192))
                    detail = log.read().decode("utf-8", errors="replace")
                raise BundleError(f"exporter failed ({result.returncode}): {detail}")
            with open(_checked_path(staged), "rb") as source:
                _atomic_write(output, lambda stream: shutil.copyfileobj(source, stream))
            return True
        finally:
            if keep:
                print(f"Kernel Spec diagnostics retained: {bundle_dir}")
            else:
                shutil.rmtree(bundle_dir)


def bundle_resources(build_config, work_dir, exporter, cxx, make):
    contexts = load_build_contexts(build_config)
    build_directory = KernelSpecBuildDirectory(work_dir)
    build_directory.install_script.unlink(missing_ok=True)
    directory = build_directory.current_directory
    output = directory / "op_spec/libcust_compile_resource.so"
    inputs = []
    maps = []
    sources = set()
    source_bindings = {}
    configs = []
    for context in contexts:
        maps.extend(context.get("source_maps", ()))
        tasks = context["tasks"]
        if not tasks:
            continue
        configs.append(context["opc_config"])
        maps.append((context["source_root"], context["source_destination"]))
        source_root = Path(context["source_root"]).resolve()
        copied_root = Path(context["copied_source_root"])
        for op_type, soc in tasks:
            task_root = task_directory(str(directory), op_type, soc)
            receipt = task_root / ".producer-success"
            if not receipt.is_file():
                logging.warning("Kernel Spec producer did not complete, skipping: %s/%s", op_type, soc)
                continue
            source = Path(receipt.read_text(encoding="utf-8"))
            relative = source.relative_to(source_root)
            # The source map describes the existing source installation, not a guessed new layout.
            if not (copied_root / relative).is_file():
                raise ValueError("Kernel Spec source is missing from the install input: " + str(source))
            sources.add(str(source))
            input_root = str(_checked_path(task_root))
            if input_root in source_bindings and source_bindings[input_root] != str(source):
                raise BundleError(f"conflicting producer source for {input_root}")
            source_bindings[input_root] = str(source)
            inputs.append(input_root)
    if not inputs:
        build_directory.write_install_entry()
        return

    # Retain temporary resources if any contributing kernel enables the existing debug setting.
    combined_config = directory / "bundle_options.ini"
    combined_config.write_text(
        "\n".join(Path(config).read_text(encoding="utf-8") for config in sorted(set(configs))), encoding="utf-8"
    )
    exported = KernelSpecBundleBuilder(
        input_dir=inputs,
        source_file=sorted(sources),
        source_map=maps,
        output=str(output),
        work_dir=str(directory / "work"),
        exporter=exporter,
        cxx=cxx,
        make=make,
        opc_config=str(combined_config),
        source_bindings=source_bindings,
    ).build()
    build_directory.write_install_entry(output if exported else None)


def main():
    """Handle the internal CMake prepare and bundle entry points."""
    if len(sys.argv) <= 1 or sys.argv[1] not in {"prepare", "bundle"}:
        raise SystemExit("usage: ascendc_build_kernel_spec.py {prepare|bundle} ...")
    action = sys.argv[1]
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("build-config", "work-dir"):
        parser.add_argument("--" + name, required=True)
    if action == "bundle":
        for name in ("exporter", "cxx", "make"):
            parser.add_argument("--" + name, required=True)
    request = parser.parse_args(sys.argv[2:])
    try:
        if action == "prepare":
            KernelSpecBuildDirectory(request.work_dir).prepare(load_build_contexts(request.build_config))
        else:
            bundle_resources(request.build_config, request.work_dir, request.exporter, request.cxx, request.make)
    except (BundleError, OSError, ValueError, KeyError, TypeError, AttributeError) as error:
        parser.exit(1, f"Kernel Spec build failed: {error}\n")


if __name__ == "__main__":
    main()
