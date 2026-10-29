# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

"""Observable keyword, path-output, coverage and index provenance contracts."""

import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

SCRIPT = Path(__file__).resolve().parents[1] / "scripts/keyword_search.py"
spec = importlib.util.spec_from_file_location("keywords", SCRIPT)
engine = importlib.util.module_from_spec(spec)
spec.loader.exec_module(engine)


class KeywordTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name) / "corpus"
        for path, body in {
            "docs/abs.md": "# asc_abs\nfloat absolute value",
            "docs/copy.md": "# asc_copy_gm2ub\nGM UB 搬运",
            "include/abs.h": "void asc_abs(float x);",
            "examples/abs.asc": "asc_abs(x);",
            ".agent/skills/helper/SKILL.md": "# helper\nasc_abs usage",
        }.items():
            file = self.root / path
            file.parent.mkdir(parents=True, exist_ok=True)
            file.write_text(body)
        self.index = Path(self.temp.name) / "index.sqlite3"
        engine.build(self.root, self.index, "fixture-v1")

    def test_literal_no_semantic_expansion(self):
        self.assertEqual(engine.search(self.index, "绝对值")["hits"], [])
        self.assertEqual(engine.search(self.index, "GM UB")["hits"][0]["path"], "docs/copy.md")
        self.assertEqual(engine.search(self.index, "GM absent")["hits"], [])

    def test_resources_and_stable_ranking(self):
        first = engine.search(self.index, "asc_abs")
        self.assertEqual({x["kind"] for x in first["hits"]}, {"doc", "include", "example", "skill"})
        self.assertEqual(first, engine.search(self.index, "ASC_ABS"))

    def test_paths_only(self):
        out = subprocess.run(
            [sys.executable, str(SCRIPT), "search", "--index", str(self.index), "--query", "asc_abs", "--json"],
            capture_output=True,
            text=True,
            check=True,
        )
        self.assertTrue(all(isinstance(x, str) for x in json.loads(out.stdout)))

    def test_structured_cli_also_returns_only_paths(self):
        directory = Path(self.temp.name) / "structured"
        directory.mkdir()
        (directory / "index.json").write_text(json.dumps({
            "documents": {"docs/abs.md": {"title": "asc_abs", "description": "absolute"}},
            "views": {},
        }))
        command = [sys.executable, str(SCRIPT.with_name("query.py")),
                   "--index-dir", str(directory), "--kw", "absolute", "--json"]
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        self.assertEqual(json.loads(result.stdout), ["docs/abs.md"])

    def test_empty_and_uncovered_are_distinct(self):
        self.assertEqual(engine.search(self.index, "absent")["status"], "no_match")
        self.assertEqual(engine.search(self.index, "asc_abs", prefix="docs/absent/")["status"], "no_coverage")

    def test_changed_candidate_fails(self):
        (self.root / "docs/abs.md").write_text("changed")
        with self.assertRaisesRegex(ValueError, "changed"):
            engine.search(self.index, "asc_abs")

    def test_cannot_write_into_corpus(self):
        with self.assertRaisesRegex(ValueError, "outside"):
            engine.build(self.root, self.root / "index.db", "fixture")

    def test_missing_index_not_created(self):
        missing = self.index.with_name("missing.db")
        with self.assertRaises(OSError):
            engine.search(missing, "asc_abs")
        self.assertFalse(missing.exists())

    def test_symlink_not_indexed(self):
        (self.root / "docs/link.md").symlink_to(self.root / "docs/abs.md")
        engine.build(self.root, self.index, "fixture")
        self.assertNotIn("docs/link.md", [x["path"] for x in engine.search(self.index, "asc_abs")["hits"]])


if __name__ == "__main__":
    unittest.main()
