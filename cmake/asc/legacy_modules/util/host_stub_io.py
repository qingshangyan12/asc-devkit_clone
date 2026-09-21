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
"""Host stub output paths and file IO."""

import io
import json
import os
import stat
from typing import Dict, List, Tuple


def get_stub_impl_cpp_filepath(dst_dir: str) -> str:
    """Get host_stub.cpp destination file path."""
    return os.path.join(dst_dir, "host_stub.cpp")


def get_triple_chevrons_impl_cpp_filepath(dst_dir: str) -> str:
    """Get host_stub.cpp destination file path."""
    return os.path.join(dst_dir, "triple_chevrons_config.cmake")


def get_normal_config_filepath(dst_dir: str) -> str:
    """Get normal_config.cmake destination file path."""
    return os.path.join(dst_dir, "normal_config.cmake")


def get_host_config_filepath(dst_dir: str) -> str:
    """Get normal_config.cmake destination file path."""
    return os.path.join(dst_dir, "host_config.cmake")


def get_aic_config_filepath(dst_dir: str) -> str:
    """Get aic_config.cmake destination file path."""
    return os.path.join(dst_dir, "aic_config.cmake")


def get_aiv_config_filepath(dst_dir: str) -> str:
    """Get aiv_config.cmake destination file path."""
    return os.path.join(dst_dir, "aiv_config.cmake")


def save_file(filepath: str, content: str):
    """Save file."""
    try:
        with open(filepath, "w", encoding="utf-8") as file:
            os.chmod(filepath, stat.S_IRUSR + stat.S_IWUSR)
            file.write(content)
    except Exception as err:
        print("[ERROR]: write file failed, filename is {}".format(filepath))
        raise err


def get_preprocessed_source_filepath(filepath: str, source_mapping: Dict) -> str:
    """Get preprocessed file's source file path."""
    absolute_filepath = os.path.abspath(filepath)
    return source_mapping[absolute_filepath]


def trans_device_cpp_filename(filename: str) -> str:
    """Transform device cpp filename."""
    return f"auto_gen_{filename}"


def generate_config_content(
    all_sources: List[Tuple[str, List[str]]],
    all_definitions: List[Tuple[str, List[str]]],
) -> str:
    """Generate config.cmake content by all sources and all definitions."""
    buff = io.StringIO()

    for name, sources in all_sources:
        buff.write(f"set({name}\n")
        for source in sources:
            buff.write(f"    {source}\n")
        buff.write(")\n")

    all_definitions.sort()
    for source, definitions in all_definitions:
        if definitions:
            buff.write(f"set_source_files_properties({source}\n")
            definitions_str = ";".join(definitions)
            buff.write(f'    PROPERTIES COMPILE_DEFINITIONS "{definitions_str}"\n')
            buff.write(")\n")

    return buff.getvalue()


def load_compile_commands(filepath: str) -> Tuple[Dict[str, str], Dict[str, bool]]:
    """Load compile_commands.json."""
    source_mapping = {}
    enable_ascendc_time_stamp = {}
    try:
        with open(filepath, encoding="utf-8") as file:
            compile_commands = json.load(file)
    except Exception as err:
        print("[ERROR]: read json file failed, json file is {}".format(filepath))
        raise err

    for compile_info in compile_commands:
        command_list = compile_info["command"].split()
        if "-o" in command_list:
            directory = compile_info["directory"]
            source = compile_info["file"]

            index = command_list.index("-o")
            output_file = command_list[index + 1]
            absolute_obj = os.path.join(directory, output_file)

            source_mapping[absolute_obj] = source
            if "-DASCENDC_TIME_STAMP_ON" in command_list:
                enable_ascendc_time_stamp[absolute_obj] = True
            else:
                enable_ascendc_time_stamp[absolute_obj] = False
    return source_mapping, enable_ascendc_time_stamp
