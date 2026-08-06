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
from collections import defaultdict
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[3]
UTIL_DIR = REPO_ROOT / "cmake/asc/fwk_modules/util"
sys.path.insert(0, str(UTIL_DIR))

import ascendc_compile_kernel


def test_ascendc_gen_param_accepts_six_config_results(tmp_path, monkeypatch):
    builder = ascendc_compile_kernel.CompileKernel.__new__(
        ascendc_compile_kernel.CompileKernel
    )
    builder.working_dir = str(tmp_path)
    builder.op_cfg_ini = str(tmp_path / "aic-ascend910b-ops-info.ini")
    builder.op_soc_ver = "ascend910b"
    builder.op_type = "AddCustomTemplate"
    builder.op_debug_config = set()

    monkeypatch.setattr(
        ascendc_compile_kernel.ascendc_bin_param_build,
        "gen_bin_param_file",
        lambda *args, **kwargs: None,
    )
    monkeypatch.setattr(
        ascendc_compile_kernel.ascendc_bin_param_build,
        "parse_op_debug_config",
        lambda *args, **kwargs: (
            defaultdict(set),
            defaultdict(set),
            defaultdict(dict),
            defaultdict(dict),
            defaultdict(dict),
            defaultdict(dict),
        ),
    )

    def fake_glob(pattern):
        if pattern.endswith("*.json"):
            return [str(tmp_path / "bin_param/AddCustomTemplate_FP16.json")]
        return [str(tmp_path / "bin_param/AddCustomTemplate_FP16.sh")]

    monkeypatch.setattr(ascendc_compile_kernel.glob, "glob", fake_glob)

    builder.ascendc_gen_param()

    assert len(builder.compile_sh) == 1
