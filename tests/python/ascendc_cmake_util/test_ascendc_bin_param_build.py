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

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
UTIL_DIR = REPO_ROOT / "cmake/asc/fwk_modules/util"
sys.path.insert(0, str(UTIL_DIR))

import ascendc_bin_param_build


def test_all_compute_unit_matches_target_soc():
    assert ascendc_bin_param_build._is_soc_match("ALL", "ascend910b")
    assert ascendc_bin_param_build._is_soc_match("", "ascend910b")
    assert ascendc_bin_param_build._is_soc_match("ascend910b", "ascend910")
    assert not ascendc_bin_param_build._is_soc_match("ascend950", "ascend910b")


def test_parse_simplified_key_mode_none(tmp_path):
    opc_config = tmp_path / "custom_opc_options.ini"
    opc_config.write_text(
        "AddCustomTemplate@ALL@--simplified_key_mode=None\n", encoding="utf-8"
    )

    result = ascendc_bin_param_build.parse_op_debug_config(
        str(opc_config), "ascend910"
    )

    assert len(result) == 6
    assert result[5]["AddCustomTemplate"] is None


def test_parse_simplified_key_mode_integer(tmp_path):
    opc_config = tmp_path / "custom_opc_options.ini"
    opc_config.write_text(
        "AddCustomTemplate@ascend910b@--simplified_key_mode=0\n",
        encoding="utf-8",
    )

    result = ascendc_bin_param_build.parse_op_debug_config(
        str(opc_config), "ascend910"
    )

    assert result[5]["AddCustomTemplate"] == 0


def test_parse_missing_config_returns_six_items(tmp_path):
    result = ascendc_bin_param_build.parse_op_debug_config(
        str(tmp_path / "missing.ini"), "ascend910"
    )

    assert len(result) == 6


def test_parse_invalid_simplified_key_mode_raises(tmp_path):
    opc_config = tmp_path / "custom_opc_options.ini"
    opc_config.write_text(
        "AddCustomTemplate@ALL@--simplified_key_mode=invalid\n", encoding="utf-8"
    )

    with pytest.raises(ValueError):
        ascendc_bin_param_build.parse_op_debug_config(
            str(opc_config), "ascend910"
        )


def _new_builder(tmp_path, simplified_key_mode):
    builder = ascendc_bin_param_build.BinParamBuilder("AddCustomTemplate")
    builder.soc = "ascend910b"
    builder.out_path = str(tmp_path)
    builder.op_file = "add_custom_template"
    builder.op_intf = "add_custom_template"
    builder.simplified_key_mode = simplified_key_mode
    return builder


def test_write_build_cmd_omits_none_simplified_key_mode(tmp_path):
    builder = _new_builder(tmp_path, None)

    builder._write_build_cmd(
        "AddCustomTemplate_FP16.json",
        "AddCustomTemplate_FP16",
        0,
        str(tmp_path),
    )

    script = (tmp_path / "AddCustomTemplate-add_custom_template-0.sh").read_text()
    assert "--simplified_key_mode" not in script


def test_write_build_cmd_keeps_mode_zero_by_default(tmp_path):
    builder = _new_builder(tmp_path, 0)

    builder._write_build_cmd(
        "AddCustomTemplate_FP16.json",
        "AddCustomTemplate_FP16",
        0,
        str(tmp_path),
    )

    script = (tmp_path / "AddCustomTemplate-add_custom_template-0.sh").read_text()
    assert "--simplified_key_mode=0" in script


def test_gen_bin_param_applies_all_simplified_key_mode(tmp_path, monkeypatch):
    cfg_file = tmp_path / "aic-ascend910b-ops-info.ini"
    cfg_file.write_text("[AddCustomTemplate]\n", encoding="utf-8")
    opc_config = tmp_path / "custom_opc_options.ini"
    opc_config.write_text(
        "AddCustomTemplate@ALL@--simplified_key_mode=None\n", encoding="utf-8"
    )

    class FakeOpDesc:
        op_type = "AddCustomTemplate"
        json_file = ""
        simplified_key_mode = 0

        def set_soc_version(self, soc):
            self.soc = soc

        def set_out_path(self, out_path):
            self.out_path = out_path

        def gen_input_json(self, auto_gen_path):
            self.generated = auto_gen_path

    op_desc = FakeOpDesc()
    monkeypatch.setattr(
        ascendc_bin_param_build.opdesc_parser,
        "get_op_desc",
        lambda *args, **kwargs: [op_desc],
    )

    ascendc_bin_param_build.gen_bin_param_file(
        str(cfg_file), str(tmp_path / "out"), "ascend910b", str(opc_config)
    )

    assert op_desc.simplified_key_mode is None
