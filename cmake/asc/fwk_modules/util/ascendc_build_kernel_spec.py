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


def load_build_context(config_path):
    """Load the package's single kernel build configuration."""
    context = json.loads(Path(config_path).read_text(encoding="utf-8"))
    if not isinstance(context, dict):
        raise BundleError("build configuration must be a JSON object")
    return context


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


def _files(root, include_directories=False):
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
            if name in dirs and include_directories:
                yield path
            elif name in files:
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
            if option.startswith("--op_debug_config=") and "dump_cce" in option.split("=", 1)[1].replace(
                ",", ";"
            ).split(";"):
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
            bound = _checked_path(bound_source)
            candidates = {path for path in bound.rglob(relative.name) if matches(path)} if bound.is_dir() else {bound}
            if not bound.is_dir() and not matches(bound):
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
        source_hashes = {}
        bindings = self.source_bindings
        for directory in sorted(set(self.input_dir)):
            manifests = [p for p in _files(directory) if p.name.endswith("_manifest.json")]
            if not manifests:
                logging.warning("Kernel Spec: no manifests in input directory, skipping: %s", directory)
                continue
            for path in manifests:
                manifest = json.loads(path.read_text(encoding="utf-8"))
                resource_id = manifest.get("resource_id")
                source, package_source = mapper.resolve(
                    manifest.get("source_file"), bindings.get(str(_checked_path(directory)))
                )
                resource_root = _checked_path(path.parent / _relative(manifest.get("resource_path")))
                relocated = mapper.relocate(manifest, package_source)
                encoded = json.dumps(relocated, sort_keys=True, separators=(",", ":"), allow_nan=False).encode()
                if source not in source_hashes:
                    source_hashes[source] = _hash_file(source)
                unit = ResourceUnit(path, encoded, resource_root, package_source, source_hashes[source])
                # Forward every candidate so exporter owns duplicate-ID selection.
                units.append((resource_id, unit))
        # Preserve valid-ID ordering and let exporter diagnose missing or malformed IDs.
        return sorted(units, key=lambda item: (item[0] if isinstance(item[0], str) else "", str(item[1].manifest_path)))

    @staticmethod
    def write_install_entry(install_script, output=None):
        if output is None:
            command = 'file(REMOVE "$ENV{DESTDIR}${spec_destination}/libcust_compile_resource.so")\n'
        else:
            output_string = str(output).replace("\\", "\\\\").replace('"', '\\"').replace("$", "\\$")
            command = 'file(INSTALL DESTINATION "${spec_destination}" TYPE FILE FILES "' + output_string + '")\n'
        install_script.parent.mkdir(parents=True, exist_ok=True)
        _atomic_write(install_script, lambda stream: stream.write(command.encode("utf-8")))

    def build(self, install_script=None):
        """Export resources and publish the optional installation entry only on success."""
        install = _checked_path(install_script) if install_script else None
        if install:
            install.unlink(missing_ok=True)
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
        lock_root = output.parent
        lock_root.mkdir(parents=True, exist_ok=True)
        with open(_checked_path(lock_root / ("." + output.name + ".lock")), "a+") as lock:
            fcntl.flock(lock.fileno(), fcntl.LOCK_EX)
            exported = self._export(output, work)
            if install:
                self.write_install_entry(install, output if exported else None)
            return exported

    def _export(self, output, work):
        mapper = KernelSpecSourceMapper(self.source_map, self.source_file)
        units = self._collect(mapper)
        if not units:
            output.unlink(missing_ok=True)
            output.with_suffix(output.suffix + ".cache.json").unlink(missing_ok=True)
            return False
        tools = {name: _tool(getattr(self, name)) for name in ("exporter", "cxx", "make")}
        keep = _keep_temp(self.opc_config)
        if self.jobs < 1:
            raise BundleError("jobs must be positive")
        fingerprint = hashlib.sha256()
        for name, tool in sorted(tools.items()):
            fingerprint.update((name + str(tool) + _hash_file(tool)).encode())
        for resource_id, unit in units:
            fingerprint.update(json.dumps([resource_id, unit.installed_source, unit.source_hash]).encode())
            fingerprint.update(unit.manifest_json)
            for path in _files(unit.resource_root, include_directories=True):
                relative = str(path.relative_to(unit.resource_root))
                if path.is_dir():
                    fingerprint.update(json.dumps(["directory", relative]).encode())
                else:
                    fingerprint.update(json.dumps(["file", relative, _hash_file(path)]).encode())
        cache = _checked_path(output.with_suffix(output.suffix + ".cache.json"))
        signature = fingerprint.hexdigest()
        if not keep and cache.is_file() and output.is_file():
            try:
                saved = json.loads(cache.read_text(encoding="utf-8"))
            except json.JSONDecodeError:
                saved = None
            if saved == {"input": signature, "output": _hash_file(output)}:
                return True
        bundle_dir = Path(tempfile.mkdtemp(prefix=".bundle-", dir=work))
        try:
            collected = bundle_dir / "collected"
            for index, (_resource_id, unit) in enumerate(units):
                # Use private ordinal directories; resource IDs are data, never filesystem paths.
                destination = collected / f"{index:08d}"
                destination.mkdir(parents=True)
                resource_path = unit.resource_root.relative_to(unit.manifest_path.parent)
                shutil.copytree(unit.resource_root, destination / resource_path, symlinks=True)
                (destination / "resource_manifest.json").write_bytes(unit.manifest_json)
            argv = [
                str(tools["exporter"]),
                "--input-dir",
                str(collected),
                "--output",
                str(output),
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
            # asc_compile_exporter owns atomic publication and cross-filesystem handling.
            if not output.is_file():
                raise BundleError(f"exporter succeeded without publishing output: {output}")
            contents = json.dumps({"input": signature, "output": _hash_file(output)}).encode()
            _atomic_write(cache, lambda stream: stream.write(contents))
            return True
        finally:
            if keep:
                print(f"Kernel Spec diagnostics retained: {bundle_dir}")
            else:
                shutil.rmtree(bundle_dir)


def bundle_resources(build_config, work_dir, exporter, cxx, make):
    context = load_build_context(build_config)
    work = _checked_path(work_dir)
    install_script = work / "install.cmake"
    install_script.unlink(missing_ok=True)
    directory = work / "input"
    output = work / "op_spec/libcust_compile_resource.so"
    inputs = []
    source_bindings = {}
    source_root = Path(context["source_root"]).resolve()
    copied_root = Path(context["copied_source_root"])
    for op_type, soc in context["tasks"]:
        task_root = task_directory(str(directory), op_type, soc)
        receipt = task_root / ".producer-result.json"
        if not receipt.is_file():
            raise BundleError(f"Kernel Spec producer result is missing: {op_type}/{soc}")
        result = json.loads(receipt.read_text(encoding="utf-8"))
        status = result.get("status")
        if status == "filtered" or (status == "failed" and context.get("allow_failed") is True):
            logging.warning("Kernel Spec producer %s, skipping %s/%s: %s", status, op_type, soc, result["reason"])
            continue
        if status != "completed":
            raise BundleError(f"Kernel Spec producer {op_type}/{soc}: {status}: {result.get('reason', '')}")
        if not any(task_root.rglob("*_manifest.json")):
            raise BundleError(f"Kernel Spec producer completed without manifests: {op_type}/{soc}")
        source = Path(result["source"])
        relative = source.relative_to(source_root)
        # The source map describes the existing source installation, not a guessed new layout.
        if not (copied_root / relative).is_file():
            raise ValueError("Kernel Spec source is missing from the install input: " + str(source))
        input_root = str(_checked_path(task_root))
        source_bindings[input_root] = str(source)
        inputs.append(input_root)
    KernelSpecBundleBuilder(
        input_dir=inputs,
        source_map=[(context["source_root"], context["source_destination"])],
        output=str(output),
        work_dir=str(work / "work"),
        exporter=exporter,
        cxx=cxx,
        make=make,
        opc_config=context.get("opc_config"),
        source_bindings=source_bindings,
    ).build(install_script)


def main():
    """Build the resources produced by the package's kernel targets."""
    if len(sys.argv) <= 1 or sys.argv[1] != "bundle":
        raise SystemExit("usage: ascendc_build_kernel_spec.py bundle ...")
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-config", required=True)
    parser.add_argument("--work-dir", required=True)
    for name in ("exporter", "cxx", "make"):
        parser.add_argument("--" + name, required=True)
    request = parser.parse_args(sys.argv[2:])
    try:
        bundle_resources(request.build_config, request.work_dir, request.exporter, request.cxx, request.make)
    except (BundleError, OSError, ValueError, KeyError, TypeError, AttributeError) as error:
        parser.exit(1, f"Kernel Spec build failed: {error}\n")


if __name__ == "__main__":
    main()
