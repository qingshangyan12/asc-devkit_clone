#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import hashlib
import os
import re
import shutil
import subprocess
import tempfile


_RESOURCE_ID_SECTION = ".ascend.resource_id"
_RESOURCE_ID_PATTERN = re.compile(r"^[0-9a-f]{64}$")


class ResourceIdError(RuntimeError):
    pass


def _validate_elf(dynamic_object_path):
    if not os.path.isfile(dynamic_object_path) or os.path.islink(dynamic_object_path):
        raise ResourceIdError(
            f"ELF object is not a regular file: {dynamic_object_path}"
        )
    try:
        with open(dynamic_object_path, "rb") as file_obj:
            magic = file_obj.read(4)
    except OSError as error:
        raise ResourceIdError(
            f"failed to read ELF object: {dynamic_object_path}"
        ) from error
    if magic != b"\x7fELF":
        raise ResourceIdError(f"input is not an ELF object: {dynamic_object_path}")


def _find_tool(*names):
    cann_root = os.environ.get("ASCEND_HOME_PATH")
    for name in names:
        if cann_root:
            candidate = os.path.join(cann_root, "bin", name)
            if os.path.isfile(candidate) and os.access(candidate, os.X_OK):
                return candidate
        candidate = shutil.which(name)
        if candidate:
            return candidate
    raise ResourceIdError(f"required ELF tool is unavailable: {', '.join(names)}")


def _run_tool(command):
    try:
        result = subprocess.run(
            command,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            check=False,
            text=True,
        )
    except OSError as error:
        raise ResourceIdError(f"failed to execute ELF tool: {command[0]}") from error
    if result.returncode != 0:
        output = result.stdout.strip()
        raise ResourceIdError(f"ELF tool failed ({command[0]}): {output}")
    return result.stdout


def _resource_id_section_info(dynamic_object_path):
    readelf = _find_tool("llvm-readelf", "readelf")
    output = _run_tool([readelf, "-W", "-S", dynamic_object_path])
    matches = []
    for line in output.splitlines():
        fields = line.split()
        try:
            name_index = fields.index(_RESOURCE_ID_SECTION)
        except ValueError:
            continue
        if len(fields) < name_index + 9:
            raise ResourceIdError("malformed .ascend.resource_id section metadata")
        section_type = fields[name_index + 1]
        size_text = fields[name_index + 4]
        tail = fields[name_index + 6 :]
        flags = tail[0] if len(tail) == 4 else ""
        align_text = tail[-1]
        try:
            size = int(size_text, 16)
            alignment = int(align_text, 10)
        except ValueError as error:
            raise ResourceIdError(
                "malformed .ascend.resource_id section metadata"
            ) from error
        matches.append((section_type, size, flags, alignment))
    return matches


def _validate_section_metadata(dynamic_object_path):
    matches = _resource_id_section_info(dynamic_object_path)
    if len(matches) != 1:
        raise ResourceIdError(
            f"expected one {_RESOURCE_ID_SECTION} section, found {len(matches)}"
        )
    section_type, size, flags, alignment = matches[0]
    if section_type != "PROGBITS":
        raise ResourceIdError(".ascend.resource_id section must be SHT_PROGBITS")
    if size != 64:
        raise ResourceIdError(".ascend.resource_id section must contain 64 bytes")
    if "A" in flags:
        raise ResourceIdError(".ascend.resource_id section must not have SHF_ALLOC")
    if alignment != 1:
        raise ResourceIdError(".ascend.resource_id section alignment must be 1")


def calculate_resource_id(dynamic_object_path: str) -> str:
    _validate_elf(dynamic_object_path)
    digest = hashlib.sha256()
    try:
        with open(dynamic_object_path, "rb") as file_obj:
            for chunk in iter(lambda: file_obj.read(1024 * 1024), b""):
                digest.update(chunk)
    except OSError as error:
        raise ResourceIdError(
            f"failed to hash ELF object: {dynamic_object_path}"
        ) from error
    return digest.hexdigest()


def read_resource_id_section(dynamic_object_path: str) -> str:
    _validate_elf(dynamic_object_path)
    _validate_section_metadata(dynamic_object_path)
    objcopy = _find_tool("llvm-objcopy", "objcopy")
    directory = os.path.dirname(os.path.realpath(dynamic_object_path))
    dump_fd, dump_path = tempfile.mkstemp(prefix=".resource_id.dump.", dir=directory)
    os.close(dump_fd)
    try:
        _run_tool(
            [
                objcopy,
                "--dump-section",
                f"{_RESOURCE_ID_SECTION}={dump_path}",
                dynamic_object_path,
            ]
        )
        with open(dump_path, "rb") as file_obj:
            content = file_obj.read()
    except OSError as error:
        raise ResourceIdError("failed to read .ascend.resource_id section") from error
    finally:
        try:
            os.unlink(dump_path)
        except FileNotFoundError:
            pass
    try:
        resource_id = content.decode("ascii")
    except UnicodeDecodeError as error:
        raise ResourceIdError(".ascend.resource_id is not ASCII") from error
    if not _RESOURCE_ID_PATTERN.fullmatch(resource_id):
        raise ResourceIdError(".ascend.resource_id must be 64 lowercase hex characters")
    return resource_id


def write_resource_id_section(dynamic_object_path: str, resource_id: str) -> None:
    _validate_elf(dynamic_object_path)
    if not isinstance(resource_id, str) or not _RESOURCE_ID_PATTERN.fullmatch(
        resource_id
    ):
        raise ResourceIdError("resource_id must be 64 lowercase hex characters")
    if _resource_id_section_info(dynamic_object_path):
        raise ResourceIdError(".ascend.resource_id section already exists")

    objcopy = _find_tool("llvm-objcopy", "objcopy")
    directory = os.path.dirname(os.path.realpath(dynamic_object_path))
    object_fd = None
    payload_fd = None
    temporary_object = None
    payload_path = None
    try:
        object_fd, temporary_object = tempfile.mkstemp(
            prefix=".resource_id.object.", suffix=".o", dir=directory
        )
        os.close(object_fd)
        object_fd = None
        payload_fd, payload_path = tempfile.mkstemp(
            prefix=".resource_id.payload.", dir=directory
        )
        payload_file = os.fdopen(payload_fd, "wb")
        payload_fd = None
        with payload_file:
            payload_file.write(resource_id.encode("ascii"))
            payload_file.flush()
            os.fsync(payload_file.fileno())
        shutil.copy2(dynamic_object_path, temporary_object)
        _run_tool(
            [
                objcopy,
                "--add-section",
                f"{_RESOURCE_ID_SECTION}={payload_path}",
                "--set-section-flags",
                f"{_RESOURCE_ID_SECTION}=readonly",
                temporary_object,
            ]
        )
        actual_resource_id = read_resource_id_section(temporary_object)
        if actual_resource_id != resource_id:
            raise ResourceIdError("resource_id verification failed")
        with open(temporary_object, "rb") as temporary_file:
            os.fsync(temporary_file.fileno())
        os.replace(temporary_object, dynamic_object_path)
    finally:
        for file_descriptor in (object_fd, payload_fd):
            if file_descriptor is not None:
                try:
                    os.close(file_descriptor)
                except OSError:
                    pass
        for path in (temporary_object, payload_path):
            if path is None:
                continue
            try:
                os.unlink(path)
            except FileNotFoundError:
                pass


def generate_and_write_resource_id(dynamic_object_path: str) -> str:
    resource_id = calculate_resource_id(dynamic_object_path)
    write_resource_id_section(dynamic_object_path, resource_id)
    actual_resource_id = read_resource_id_section(dynamic_object_path)
    if actual_resource_id != resource_id:
        raise ResourceIdError("resource_id verification failed")
    return resource_id
