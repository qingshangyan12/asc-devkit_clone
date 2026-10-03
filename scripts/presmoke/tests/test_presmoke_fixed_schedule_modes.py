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

from pathlib import Path
from presmoke.model import Cell, ExampleSpec
from presmoke.scheduler import apply_fixed_schedule


def cell(name, mode):
    spec = ExampleSpec(Path(name), name, [], ["dav-2201"], ["cpu", "npu"], "readme")
    return Cell(spec, "dav-2201", mode, [], Path(name) / mode)


def test_fixed_schedule_keeps_all_modes_in_case_order(tmp_path):
    cells = [cell("a", "cpu"), cell("a", "npu"), cell("b", "cpu"), cell("b", "npu")]
    schedule = tmp_path / "schedule.txt"
    schedule.write_text("b\na\nb\n")
    result = apply_fixed_schedule(cells, schedule)
    assert [c.key for c in result] == [cells[i].key for i in [2, 3, 0, 1]]
    assert len(result) == len(cells)


def test_unscheduled_cells_are_preserved(tmp_path):
    cells = [cell("a", "cpu"), cell("b", "cpu")]
    schedule = tmp_path / "schedule.txt"
    schedule.write_text("b\nunknown\n")
    assert apply_fixed_schedule(cells, schedule) == [cells[1], cells[0]]
