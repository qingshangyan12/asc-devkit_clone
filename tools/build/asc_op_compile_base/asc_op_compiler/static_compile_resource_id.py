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
"""Build a Resource ID metadata object from the linked object inputs."""

from contextlib import suppress
import hashlib
import os
import shutil
import subprocess
import tempfile


_RESOURCE_ID_TYPE = 6


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


def calculate_resource_id(object_paths):
    """OR the SHA256 digests of objects in their final link order."""
    digest = bytearray(32)
    if not object_paths:
        raise ResourceIdError("object list for resource_id must not be empty")
    for object_path in object_paths:
        _validate_elf(object_path)
        try:
            with open(object_path, "rb") as file_obj:
                current = hashlib.sha256(file_obj.read()).digest()
        except OSError as error:
            raise ResourceIdError(
                f"failed to hash ELF object: {object_path}"
            ) from error
        for index, value in enumerate(current):
            digest[index] |= value
    return digest.hex()


def generate_resource_id_object(object_paths, output_path: str) -> str:
    resource_id = calculate_resource_id(object_paths)
    directory = os.path.dirname(os.path.realpath(output_path))
    os.makedirs(directory, exist_ok=True)
    payload_fd = None
    payload_path = None
    try:
        payload_fd, payload_path = tempfile.mkstemp(
            prefix=".resource_id.payload.", dir=directory
        )
        payload_file = os.fdopen(payload_fd, "wb")
        payload_fd = None
        with payload_file:
            payload_file.write(
                (
                    _RESOURCE_ID_TYPE.to_bytes(2, "little")
                    + len(resource_id).to_bytes(2, "little")
                    + resource_id.encode("ascii")
                )
            )
        linker = _find_tool("ld.lld")
        objcopy = _find_tool("llvm-objcopy")
        _run_tool(
            [
                linker,
                "-m",
                "aicorelinux",
                "-r",
                "-b",
                "binary",
                payload_path,
                "-o",
                output_path,
            ]
        )
        _run_tool(
            [objcopy, "--rename-section", ".data=.ascend.meta,readonly", output_path]
        )
        return resource_id
    finally:
        for file_descriptor in (payload_fd,):
            if file_descriptor is None:
                continue
            with suppress(OSError):
                os.close(file_descriptor)
        for path in (payload_path,):
            if path is None:
                continue
            try:
                os.unlink(path)
            except FileNotFoundError:
                pass
