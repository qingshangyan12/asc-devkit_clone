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
_HASH_CHUNK_SIZE = 1024 * 1024


class ResourceIdError(RuntimeError):
    pass


def calculate_resource_id(object_path):
    """SHA256 of the kernel object as it stands before the Resource ID is embedded."""
    if os.path.islink(object_path):
        raise ResourceIdError(f"link input must not be a symlink: {object_path}")
    digest = hashlib.sha256()
    try:
        with open(object_path, "rb") as file_obj:
            chunk = file_obj.read(_HASH_CHUNK_SIZE)
            while chunk:
                digest.update(chunk)
                chunk = file_obj.read(_HASH_CHUNK_SIZE)
    except OSError as error:
        raise ResourceIdError(f"failed to read link input: {object_path}") from error
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
        '__attribute__((used, section(".ascend.meta")))\n'
        "static const struct BinaryMetaSpecializationResourceId g_ascend_resource_id_section = "
        "{%d, %d, {%s}};\n"
        % (
            _RESOURCE_ID_VALUE_SIZE,
            _RESOURCE_ID_TYPE,
            _RESOURCE_ID_VALUE_SIZE,
            value,
        )
    )


def generate_resource_id_object(
    object_path,
    output_path,
    compile_command,
    cce_arch,
    compile_log_path=None,
):
    """Compile the Resource ID into a .ascend.meta metadata object."""
    from .ascendc_common_utility import CommonUtility, CompileStage

    resource_id = calculate_resource_id(object_path)
    directory = os.path.dirname(os.path.realpath(output_path))
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
        return resource_id
    finally:
        if source_fd is not None:
            with suppress(OSError):
                os.close(source_fd)
        if source_path is not None:
            with suppress(FileNotFoundError):
                os.unlink(source_path)


def embed_resource_id(dst_file, is_debug, compile_log_path=None):
    """Compile the Resource ID of dst_file and merge it back into dst_file."""
    from .ascendc_compile_base import (
        COMPILER_ARCH,
        get_soc_spec,
        link_resource_id_obj,
    )
    from .global_storage import global_var_storage

    resource_object = "%s.resource_id.o" % dst_file
    compile_command = [global_var_storage.get_variable("ascendc_compiler_path")]
    if global_var_storage.get_variable("ascendc_enable_ccache"):
        compile_command.insert(0, os.environ.get("ASCENDC_CCACHE_EXECUTABLE"))
    resource_id = generate_resource_id_object(
        dst_file,
        resource_object,
        compile_command,
        get_soc_spec(COMPILER_ARCH),
        compile_log_path,
    )
    link_resource_id_obj(dst_file, resource_object, is_debug, compile_log_path)
    return resource_id
