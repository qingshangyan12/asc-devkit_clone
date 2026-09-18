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
import tempfile


_RESOURCE_ID_TYPE = 6
_RESOURCE_ID_VALUE_SIZE = 64
_TLV_HEAD_SIZE = 4
_HASH_CHUNK_SIZE = 1024 * 1024


class ResourceIdError(RuntimeError):
    pass


def _validate_elf(dynamic_object_path):
    if not isinstance(dynamic_object_path, (str, os.PathLike)):
        raise ResourceIdError(f"ELF object path is not a path: {dynamic_object_path!r}")
    if not os.path.isfile(dynamic_object_path) or os.path.islink(dynamic_object_path):
        raise ResourceIdError(
            f"ELF object is not a regular file: {dynamic_object_path}"
        )


def _hash_elf(object_path, digest):
    """Check the ELF magic and fold the whole object into digest in one read."""
    _validate_elf(object_path)
    try:
        with open(object_path, "rb") as file_obj:
            chunk = file_obj.read(_HASH_CHUNK_SIZE)
            if not chunk.startswith(b"\x7fELF"):
                raise ResourceIdError(f"input is not an ELF object: {object_path}")
            while chunk:
                digest.update(chunk)
                chunk = file_obj.read(_HASH_CHUNK_SIZE)
    except OSError as error:
        raise ResourceIdError(f"failed to read ELF object: {object_path}") from error


def calculate_resource_id(object_paths):
    """SHA256 of the linked objects' contents concatenated in link order.

    Hashing the merged stream rather than combining per-object digests keeps the
    link order significant and leaves no way for two objects to cancel out.
    """
    if not object_paths:
        raise ResourceIdError("object list for resource_id must not be empty")
    digest = hashlib.sha256()
    for object_path in object_paths:
        _hash_elf(object_path, digest)
    return digest.hexdigest()


def _render_resource_id_source(resource_id):
    """Render the C++ source that places the Resource ID into .ascend.meta."""
    # Initialize the value array character by character: a string literal of
    # exactly 64 chars carries an implicit NUL, which would overflow the array.
    value = ", ".join(f"'{char}'" for char in resource_id)
    return (
        "struct BinaryMetaSpecializationResourceId {\n"
        "    unsigned short type;\n"
        "    unsigned short len;\n"
        "    char value[%d];\n"
        "};\n"
        "static_assert(sizeof(BinaryMetaSpecializationResourceId) == %d,\n"
        '              "BinaryMetaSpecializationResourceId must stay packed as type|len|value");\n'
        '__attribute__((used, section(".ascend.meta")))\n'
        "static const struct BinaryMetaSpecializationResourceId g_ascend_resource_id_section = "
        "{%d, %d, {%s}};\n"
        % (
            _RESOURCE_ID_VALUE_SIZE,
            _TLV_HEAD_SIZE + _RESOURCE_ID_VALUE_SIZE,
            _RESOURCE_ID_TYPE,
            _RESOURCE_ID_VALUE_SIZE,
            value,
        )
    )


def generate_resource_id_object(
    object_paths,
    output_path,
    compile_command,
    cce_arch,
    compile_log_path=None,
):
    """Compile the Resource ID into a .ascend.meta metadata object."""
    from .ascendc_common_utility import CommonUtility, CompileStage

    resource_id = calculate_resource_id(object_paths)
    if not compile_command or not all(compile_command) or not cce_arch:
        raise ResourceIdError("compile command and cce arch must be provided")
    directory = os.path.dirname(os.path.realpath(output_path))
    os.makedirs(directory, exist_ok=True)
    source_fd = None
    source_path = None
    try:
        source_fd, source_path = tempfile.mkstemp(
            prefix=".resource_id.source.", suffix=".cpp", dir=directory
        )
        source_file = os.fdopen(source_fd, "w")
        source_fd = None
        with source_file:
            source_file.write(_render_resource_id_source(resource_id))
        with suppress(FileNotFoundError):
            os.unlink(output_path)
        CommonUtility.run_cmd_inner(
            list(compile_command)
            + [
                "-c",
                "-O3",
                "-xcce",
                source_path,
                "--cce-aicore-arch=" + cce_arch,
                "--cce-aicore-only",
                "-std=c++17",
                "-o",
                output_path,
            ],
            CompileStage.SPECIALIZATION,
            compile_log_path,
        )
        if not os.path.isfile(output_path):
            raise ResourceIdError(f"compile resource id object failed: {output_path}")
        return resource_id
    finally:
        if source_fd is not None:
            with suppress(OSError):
                os.close(source_fd)
        if source_path is not None:
            with suppress(FileNotFoundError):
                os.unlink(source_path)
