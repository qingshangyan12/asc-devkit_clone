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

from types import SimpleNamespace
import pytest
from presmoke import arch


@pytest.mark.parametrize(
    "text,expected",
    [("Atlas A3", "dav-2201"), ("Atlas A2", "dav-2201"), ("Ascend 910_93", "dav-2201"), ("Ascend 950PR", "dav-3510")],
)
def test_product_labels_follow_documented_architectures(monkeypatch, text, expected):
    monkeypatch.delenv("PRESMOKE_ARCH", raising=False)
    monkeypatch.setattr(arch.shutil, "which", lambda name: "/not-executed/npu-smi")
    monkeypatch.setattr(arch.subprocess, "run", lambda *args, **kwargs: SimpleNamespace(stdout=text))
    assert arch.detect_arch() == expected
