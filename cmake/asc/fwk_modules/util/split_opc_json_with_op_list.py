#!/usr/bin/env python3
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

"""
split_opc_json_with_op_list.py
"""

import sys
import os
import json
import stat


CUSTOM_BINARY_JSON_META = "_ascendc_custom_binary_json"


def wr_json(json_obj, json_file):
    flags = os.O_WRONLY | os.O_CREAT
    modes = stat.S_IWUSR | stat.S_IRUSR
    with os.fdopen(os.open(json_file, flags, modes), "w") as f:
        json.dump(json_obj, f, indent=2)


def _check_binary_json(binary_json, expected_op_type):
    if not isinstance(binary_json, dict):
        raise ValueError("root: expected object")
    if binary_json.get("op_type") != expected_op_type:
        raise ValueError(
            f"op_type: expected '{expected_op_type}', got '{binary_json.get('op_type')}'"
        )
    op_list = binary_json.get("op_list")
    if not isinstance(op_list, list) or not op_list:
        raise ValueError("op_list: expected non-empty array")

    bin_filenames = set()
    for index, op in enumerate(op_list):
        op_path = f"op_list[{index}]"
        if not isinstance(op, dict):
            raise ValueError(f"{op_path}: expected object")
        bin_filename = op.get("bin_filename")
        if (
            not isinstance(bin_filename, str)
            or not bin_filename
            or os.path.basename(bin_filename) != bin_filename
        ):
            raise ValueError(f"{op_path}.bin_filename: invalid value '{bin_filename}'")
        output_name = os.path.splitext(bin_filename)[0]
        if output_name in bin_filenames:
            raise ValueError(f"{op_path}.bin_filename: duplicate output '{output_name}'")
        bin_filenames.add(output_name)
        simplified_key = op.get("simplified_key")
        if not isinstance(simplified_key, str) or not simplified_key:
            raise ValueError(f"{op_path}.simplified_key: expected non-empty string")


def split_json_files(
    ori_json, output_dir, expected_op_type=None, require_simplified_key=False
):
    """
    gen output json by binary_file and opc json file
    """
    if not os.path.exists(ori_json):
        print("[ERROR]the ori_json doesn't exist")
        return []
    if not os.path.exists(output_dir):
        print("[ERROR]the out_dir of split_json doesn't exist")
        return []
    try:
        with open(ori_json, "r") as file_wr:
            binary_json = json.load(file_wr)
    except (OSError, json.JSONDecodeError) as error:
        if require_simplified_key:
            raise ValueError(f"Invalid BINARY_JSON '{ori_json}': {error}") from error
        raise

    if require_simplified_key:
        try:
            _check_binary_json(binary_json, expected_op_type)
        except ValueError as error:
            raise ValueError(f"Invalid BINARY_JSON '{ori_json}': {error}") from error

    op_type = binary_json.get("op_type")
    op_list = binary_json.get("op_list", list())
    generated_files = []

    for idx, op in enumerate(op_list):
        new_binary_json = {"op_type": op_type}
        new_binary_json["op_list"] = [op]
        if require_simplified_key:
            new_binary_json[CUSTOM_BINARY_JSON_META] = [ori_json, idx]

        bin_filename = op.get("bin_filename")
        if not bin_filename:
            print(f"[ERROR]bin_filename field not found in op {idx}")
            return []
        base_name = os.path.splitext(bin_filename)[0]
        new_binary_file = os.path.join(output_dir, f"{base_name}.json")

        generated_files.append(new_binary_file)

        wr_json(new_binary_json, new_binary_file)

    return generated_files


if __name__ == "__main__":
    generated_files = split_json_files(sys.argv[1], sys.argv[2])
