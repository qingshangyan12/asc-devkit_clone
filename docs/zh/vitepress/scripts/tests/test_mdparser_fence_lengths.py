# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
"""Compare the real preprocessor's code literals with cmark-gfm output."""

from html.parser import HTMLParser
from pathlib import Path
import sys
import unittest

import cmarkgfm


sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from mdparser import parser


class CodeText(HTMLParser):
    def __init__(self):
        super().__init__()
        self.inside = False
        self.parts = []

    def handle_starttag(self, tag, attrs):
        if tag == "code":
            self.inside = True

    def handle_endtag(self, tag):
        if tag == "code":
            self.inside = False

    def handle_data(self, data):
        if self.inside:
            self.parts.append(data)


def code_text(html):
    collector = CodeText()
    collector.feed(html)
    return "".join(collector.parts)


class TestClosingFenceLength(unittest.TestCase):
    def check_code_literals(self, source):
        expected = cmarkgfm.github_flavored_markdown_to_html(source)
        actual = parser.parse_string(source)
        self.assertEqual(code_text(actual), code_text(expected))

    def test_four_backticks_keep_three_backticks_inside_code(self):
        self.check_code_literals("````text\n```\n~literal~\n````\n")

    def test_four_tildes_keep_three_tildes_inside_code(self):
        self.check_code_literals("~~~~text\n~~~\n~literal~\n~~~~\n")

    def test_five_backticks_keep_four_backticks_inside_code(self):
        self.check_code_literals("`````text\n````\n~literal~\n`````\n")

    def test_math_extraction_stays_outside_the_long_code_fence(self):
        source = "````text\n```\n$inside$\n````\n\n$outside$\n"
        extract = getattr(parser, "_extract_math_blocks")
        _, blocks = extract(source)
        self.assertEqual(blocks, ["$outside$"])

    def test_matching_three_character_fence_keeps_existing_behavior(self):
        self.check_code_literals("```text\n~literal~\n```\n")

    def test_longer_closing_fence_is_still_accepted(self):
        self.check_code_literals("```text\n~literal~\n````\n")
