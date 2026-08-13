#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
import json
import re
import sys


CUSTOM_SIMPLIFIED_KEY_MAX_LEN = 256
KERNEL_NAME_PATTERN = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


def _validate_non_empty_string(value, field_path, errors):
    if not isinstance(value, str) or not value:
        errors.append(f"{field_path}: expected non-empty string")
        return False
    return True


def _validate_tensor(tensor, field_path, errors):
    if tensor is None:
        return
    if isinstance(tensor, list):
        for index, item in enumerate(tensor):
            _validate_tensor(item, f"{field_path}[{index}]", errors)
        return
    if not isinstance(tensor, dict):
        errors.append(f"{field_path}: expected object, array, or null")
        return
    if not tensor:
        return
    _validate_non_empty_string(tensor.get("dtype"), f"{field_path}.dtype", errors)
    _validate_non_empty_string(tensor.get("format"), f"{field_path}.format", errors)


def _validate_tensor_list(op, field, op_path, errors):
    tensors = op.get(field)
    field_path = f"{op_path}.{field}"
    if not isinstance(tensors, list):
        errors.append(f"{field_path}: expected array")
        return
    for index, tensor in enumerate(tensors):
        _validate_tensor(tensor, f"{field_path}[{index}]", errors)


def _validate_op(op, index, bin_filenames, errors):
    op_path = f"op_list[{index}]"
    if not isinstance(op, dict):
        errors.append(f"{op_path}: expected object")
        return

    bin_filename = op.get("bin_filename")
    if _validate_non_empty_string(bin_filename, f"{op_path}.bin_filename", errors):
        if KERNEL_NAME_PATTERN.fullmatch(bin_filename) is None:
            errors.append(f"{op_path}.bin_filename: invalid kernel name '{bin_filename}'")
        elif bin_filename in bin_filenames:
            errors.append(f"{op_path}.bin_filename: duplicate value '{bin_filename}'")
        else:
            bin_filenames.add(bin_filename)

    simplified_key = op.get("simplified_key")
    if _validate_non_empty_string(simplified_key, f"{op_path}.simplified_key", errors):
        key_len = len(simplified_key.encode("utf-8"))
        if key_len >= CUSTOM_SIMPLIFIED_KEY_MAX_LEN:
            errors.append(
                f"{op_path}.simplified_key: UTF-8 length must be less than "
                f"{CUSTOM_SIMPLIFIED_KEY_MAX_LEN} bytes, but got {key_len} bytes"
            )

    _validate_tensor_list(op, "inputs", op_path, errors)
    _validate_tensor_list(op, "outputs", op_path, errors)


def _classify_simplified_keys(op_list, errors):
    has_nonempty_key = []
    for index, op in enumerate(op_list):
        if not isinstance(op, dict):
            errors.append(f"op_list[{index}]: expected object")
            has_nonempty_key.append(False)
            continue
        value = op.get("simplified_key")
        if value not in (None, "") and not isinstance(value, str):
            errors.append(f"op_list[{index}].simplified_key: expected string")
        has_nonempty_key.append(isinstance(value, str) and bool(value))

    if any(has_nonempty_key) and not all(has_nonempty_key):
        for index, present in enumerate(has_nonempty_key):
            if not present:
                errors.append(
                    f"op_list[{index}].simplified_key: cannot mix custom and "
                    "standard binaries"
                )
    return bool(has_nonempty_key) and all(has_nonempty_key)


def validate_custom_binary_json(json_path: str, expected_op_type: str) -> bool:
    try:
        with open(json_path, "r", encoding="utf-8") as file:
            binary_json = json.load(file)
    except OSError as error:
        raise ValueError(
            f"Invalid BINARY_JSON '{json_path}': failed to read file: {error}"
        ) from error
    except json.JSONDecodeError as error:
        raise ValueError(
            f"Invalid BINARY_JSON '{json_path}': invalid JSON: {error}"
        ) from error

    errors = []
    is_custom = False
    if not isinstance(binary_json, dict):
        errors.append("root: expected object")
    else:
        op_type = binary_json.get("op_type")
        if _validate_non_empty_string(op_type, "op_type", errors):
            if op_type != expected_op_type:
                errors.append(
                    f"op_type: expected '{expected_op_type}', got '{op_type}'"
                )

        op_list = binary_json.get("op_list")
        if not isinstance(op_list, list) or not op_list:
            errors.append("op_list: expected non-empty array")
        else:
            is_custom = _classify_simplified_keys(op_list, errors)
            if is_custom:
                bin_filenames = set()
                for index, op in enumerate(op_list):
                    _validate_op(op, index, bin_filenames, errors)

    if errors:
        details = "\n".join(f"  - {error}" for error in errors)
        raise ValueError(f"Invalid BINARY_JSON '{json_path}':\n{details}")
    return is_custom


def main():
    parser = argparse.ArgumentParser(description="Validate custom Binary JSON")
    parser.add_argument("--json", required=True, help="Path to Binary JSON")
    parser.add_argument("--op-type", required=True, help="Expected operator type")
    args = parser.parse_args()

    try:
        validate_custom_binary_json(args.json, args.op_type)
    except ValueError as error:
        print(error, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
