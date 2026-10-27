# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
"""Render real cmark/Pygments output and preserve literal math-like sentinels."""

from html.parser import HTMLParser
from pathlib import Path
import sys
import unittest


SCRIPTS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SCRIPTS))
from mdparser.parser import parse_string  # noqa: E402


class RenderedText(HTMLParser):
    def __init__(self):
        super().__init__(convert_charrefs=True)
        self.parts = []

    def handle_data(self, data):
        self.parts.append(data)


def text_from_html(markdown):
    reader = RenderedText()
    reader.feed(parse_string(markdown))
    return "".join(reader.parts)


class MathPlaceholderCollisionTests(unittest.TestCase):
    def assert_preserved(self, markdown, literals):
        text = text_from_html(markdown)
        for literal in literals:
            self.assertIn(literal, text)
        self.assertEqual(text.count("$x$"), 1)

    def test_plain_literal_is_not_restored_as_another_formula(self):
        self.assert_preserved("Literal @@MATH0@@ alongside $x$.", ["@@MATH0@@"])

    def test_inline_code_literal_is_preserved(self):
        self.assert_preserved("`@@MATH0@@` alongside $x$.", ["@@MATH0@@"])

    def test_fenced_text_literal_is_preserved(self):
        self.assert_preserved("```text\n@@MATH0@@\n```\n\n$x$", ["@@MATH0@@"])

    def test_entity_encoded_literal_is_preserved(self):
        self.assert_preserved("&#64;&#64;MATH0@@ alongside $x$.", ["@@MATH0@@"])

    def test_markdown_escaped_literal_is_preserved(self):
        self.assert_preserved(r"@\@MATH0@@ alongside $x$.", ["@@MATH0@@"])

    def test_repeated_prefixes_and_multiple_formulas_are_preserved(self):
        text = text_from_html("@@MATH0@@ @@@MATH0@@ @@@@MATH1@@\n\n$x$ then $y$")
        for literal in ["@@MATH0@@", "@@@MATH0@@", "@@@@MATH1@@"]:
            self.assertIn(literal, text)
        self.assertEqual(text.count("$x$"), 1)
        self.assertEqual(text.count("$y$"), 1)

    def test_existing_formula_rendering_without_collision_is_unchanged(self):
        self.assertEqual(text_from_html("$x$ and $$y$$").strip(), "$x$ and $$y$$")

    def test_literal_without_any_formula_remains_literal(self):
        self.assertEqual(text_from_html("@@MATH0@@").strip(), "@@MATH0@@")


if __name__ == "__main__":
    unittest.main()
