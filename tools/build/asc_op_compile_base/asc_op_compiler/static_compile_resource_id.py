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
"""Derive a Resource ID from the linked object inputs and publish it as metadata."""

from contextlib import suppress
import hashlib
import os
import subprocess
import tempfile


_RESOURCE_ID_TYPE = 6
_RESOURCE_ID_VALUE_SIZE = 64
_HASH_CHUNK_SIZE = 1024 * 1024
_META_SECTION = ".ascend.meta"
_OBJCOPY = "llvm-objcopy"


class ResourceIdError(RuntimeError):
    pass


def _validate_elf(dynamic_object_path):
    if not isinstance(dynamic_object_path, (str, os.PathLike)):
        raise ResourceIdError(f"ELF object path is not a path: {dynamic_object_path!r}")
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


def _hash_object(object_path):
    _validate_elf(object_path)
    digest = hashlib.sha256()
    try:
        with open(object_path, "rb") as file_obj:
            for chunk in iter(lambda: file_obj.read(_HASH_CHUNK_SIZE), b""):
                digest.update(chunk)
    except OSError as error:
        raise ResourceIdError(f"failed to hash ELF object: {object_path}") from error
    return digest.digest()


def calculate_resource_id(object_paths):
    """XOR the SHA256 digests of linked objects."""
    if not object_paths:
        raise ResourceIdError("object list for resource_id must not be empty")
    digest = bytearray(hashlib.sha256().digest_size)
    for object_path in object_paths:
        for index, value in enumerate(_hash_object(object_path)):
            digest[index] ^= value
    if not any(digest):
        raise ResourceIdError(
            "resource_id collapsed to zero: object digests cancelled out"
        )
    return digest.hex()


def _dump_meta_section(object_path, dump_path):
    """Return the raw .ascend.meta bytes, or None when the object has no such section."""
    try:
        result = subprocess.run(
            [
                _OBJCOPY,
                "--dump-section",
                f"{_META_SECTION}={dump_path}",
                object_path,
                os.devnull,
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            check=False,
        )
    except OSError as error:
        raise ResourceIdError(f"failed to execute {_OBJCOPY}") from error
    # objcopy fails when the section is absent, which simply means nothing is there yet.
    if result.returncode != 0:
        return None
    try:
        with open(dump_path, "rb") as dump_file:
            return dump_file.read()
    except OSError as error:
        raise ResourceIdError(
            f"failed to read {_META_SECTION} of {object_path}"
        ) from error


def attach_resource_id_entry(object_path, resource_id, compile_log_path=None):
    """Append the Resource ID TLV entry to an already linked object.

    .ascend.meta is a plain concatenation of TLV entries, so the entry is built
    by hand and appended to whatever the link produced. The layout must stay in
    step with BinaryMetaResourceId in impl/basic_api/utils/kernel_utils_macros.h.
    """
    from .ascendc_common_utility import CommonUtility, CompileStage

    if len(resource_id) != _RESOURCE_ID_VALUE_SIZE:
        raise ResourceIdError(
            "resource_id must be %d characters, got %d"
            % (_RESOURCE_ID_VALUE_SIZE, len(resource_id))
        )
    entry = (
        _RESOURCE_ID_TYPE.to_bytes(2, "little")
        + _RESOURCE_ID_VALUE_SIZE.to_bytes(2, "little")
        + resource_id.encode("ascii")
    )
    directory = os.path.dirname(os.path.realpath(object_path))
    try:
        payload_fd, payload_path = tempfile.mkstemp(
            prefix=".ascend.meta.", dir=directory
        )
    except OSError as error:
        raise ResourceIdError(f"failed to stage {_META_SECTION} payload") from error
    os.close(payload_fd)
    try:
        existing = _dump_meta_section(object_path, payload_path)
        try:
            with open(payload_path, "wb") as payload_file:
                payload_file.write((existing or b"") + entry)
        except OSError as error:
            raise ResourceIdError(f"failed to stage {_META_SECTION} payload") from error
        if existing is None:
            command = [
                _OBJCOPY,
                "--add-section",
                f"{_META_SECTION}={payload_path}",
                "--set-section-flags",
                f"{_META_SECTION}=readonly",
                object_path,
            ]
        else:
            command = [
                _OBJCOPY,
                "--update-section",
                f"{_META_SECTION}={payload_path}",
                object_path,
            ]
        CommonUtility.run_cmd_inner(
            command, CompileStage.SPECIALIZATION, compile_log_path
        )
        written = _dump_meta_section(object_path, payload_path)
        if written is None or entry not in written:
            raise ResourceIdError(
                f"failed to attach resource id to {_META_SECTION} of {object_path}"
            )
    finally:
        with suppress(OSError):
            os.unlink(payload_path)
