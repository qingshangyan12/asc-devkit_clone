# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
"""Run the real Markdown CLI on real input files, using cmark-gfm."""

import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


SCRIPTS = Path(__file__).resolve().parents[1]


class TestBatchInputPaths(unittest.TestCase):
    def run_batch(self, kinds):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            inputs = []
            for name, kind in zip(["first", "second"], kinds):
                source = root / name / "page.md"
                source.parent.mkdir()
                source.write_text("# " + name + "\n\nActual Markdown content.\n", encoding="utf-8")
                inputs.append(str(source if kind == "absolute" else source.relative_to(root)))
            output = root / "output"
            env = dict(os.environ)
            env["PYTHONPATH"] = str(SCRIPTS) + os.pathsep + env.get("PYTHONPATH", "")
            result = subprocess.run(
                [sys.executable, "-c", "from mdparser.cli import main; main()", *inputs, "-o", str(output)],
                cwd=root,
                env=env,
                capture_output=True,
                text=True,
                timeout=3,
                check=False,
            )
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            for name in ["first", "second"]:
                html = (output / name / "page.html").read_text(encoding="utf-8")
                self.assertIn("Actual Markdown content.", html)
                self.assertIn("<h1>", html)
                self.assertIn(name, html)

    def test_relative_then_absolute_finishes(self):
        self.run_batch(("relative", "absolute"))

    def test_absolute_then_relative_finishes(self):
        self.run_batch(("absolute", "relative"))

    def test_all_relative_paths_keep_working(self):
        self.run_batch(("relative", "relative"))

    def test_all_absolute_paths_keep_working(self):
        self.run_batch(("absolute", "absolute"))
