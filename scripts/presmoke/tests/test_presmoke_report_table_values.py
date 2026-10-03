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

import pytest
from markdown_it import MarkdownIt
from presmoke.model import RunResult, Suggestion
from presmoke.report import markdown_result_table, markdown_suggestions, write_suggestions


def cell_counts(text):
    tokens = MarkdownIt().enable("table").parse(text)
    return [sum(t.type == "td_open" for t in tokens)]


def test_result_values_remain_in_eleven_columns():
    result = RunResult(
        "case|one",
        "dav-2201",
        "cpu",
        "FAIL",
        reason="first|second\nthird",
        log_file="out|run.log",
        stage_log_files=["stage|one.log"],
    )
    table = "\n".join(markdown_result_table([result]))
    assert cell_counts(table) == [11]
    assert "third" in table
    assert "first\\|second<br>third" in table


@pytest.mark.parametrize("writer", ["inline", "file"])
def test_suggestions_preserve_all_five_fields(tmp_path, writer):
    item = Suggestion("example|one", "category", "warn", "one|two\nthree", "hint|tail")
    if writer == "inline":
        table = "\n".join(markdown_suggestions([item]))
    else:
        path = tmp_path / "suggestions.md"
        write_suggestions([item], path)
        table = path.read_text()
    assert cell_counts(table) == [5]
    assert "one\\|two<br>three" in table
    assert "hint\\|tail" in table
