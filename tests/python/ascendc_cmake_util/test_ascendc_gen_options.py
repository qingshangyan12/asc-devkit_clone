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

import sys
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[3]
UTIL_DIR = REPO_ROOT / "cmake/asc/fwk_modules/util"
sys.path.insert(0, str(UTIL_DIR))

import ascendc_gen_options


def test_simplified_key_mode_is_kernel_config():
    debug_config = []
    kernel_config = []

    category, value = ascendc_gen_options._parse_single_option(
        "--simplified_key_mode=None", debug_config, kernel_config
    )

    assert category == "kernel_config"
    assert value is None
    assert kernel_config == ["--simplified_key_mode=None"]


def test_simplified_key_mode_is_only_written_to_opc_config(tmp_path):
    compile_options_file = tmp_path / "custom_compile_options.ini"

    ascendc_gen_options.gen_compile_options(
        str(compile_options_file),
        "AddCustomTemplate",
        "ALL",
        ["--simplified_key_mode=None"],
    )

    assert not compile_options_file.exists()
    assert (tmp_path / "custom_opc_options.ini").read_text() == (
        "AddCustomTemplate@ALL@--simplified_key_mode=None\n"
    )


def test_kernel_configs_are_written_as_separate_fields(tmp_path):
    compile_options_file = tmp_path / "custom_compile_options.ini"

    ascendc_gen_options.gen_compile_options(
        str(compile_options_file),
        "AddCustomTemplate",
        "ALL",
        ["--kernel-json-file=/tmp/binary.json", "--simplified_key_mode=None"],
    )

    assert (tmp_path / "custom_opc_options.ini").read_text() == (
        "AddCustomTemplate@ALL@--kernel-json-file=/tmp/binary.json"
        "@--simplified_key_mode=None\n"
    )
