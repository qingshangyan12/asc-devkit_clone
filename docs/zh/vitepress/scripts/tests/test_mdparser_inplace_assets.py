# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
"""Run actual Markdown CLI output against real adjacent static files."""

import base64
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


SCRIPTS = Path(__file__).resolve().parents[1]
PNG = base64.b64decode("iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mP8/x8AAwMCAO+aD1cAAAAASUVORK5CYII=")


class TestInPlaceStaticAssets(unittest.TestCase):
    def run_output(self, output_kind):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for name in ["first", "second"]:
                (root / (name + ".md")).write_text("# " + name + "\n\n![pixel](pixel.png)\n", encoding="utf-8")
            asset = root / "pixel.png"
            asset.write_bytes(PNG)
            args = ["first.md", "second.md"]
            if output_kind == "explicit_same":
                output = root
                args += ["-o", str(output)]
            elif output_kind == "separate":
                output = root / "output"
                args += ["-o", str(output)]
            else:
                output = root
            env = dict(os.environ)
            env["PYTHONPATH"] = str(SCRIPTS) + os.pathsep + env.get("PYTHONPATH", "")
            result = subprocess.run(
                [sys.executable, "-c", "from mdparser.cli import main; main()", *args],
                cwd=root,
                env=env,
                capture_output=True,
                text=True,
                timeout=5,
                check=False,
            )
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            for name in ["first", "second"]:
                self.assertIn("pixel.png", (output / (name + ".html")).read_text())
            self.assertEqual((output / "pixel.png").read_bytes(), PNG)
            self.assertEqual(asset.read_bytes(), PNG)

    def test_default_batch_output_keeps_adjacent_asset_in_place(self):
        self.run_output("default")

    def test_explicit_source_directory_output_keeps_asset_in_place(self):
        self.run_output("explicit_same")

    def test_separate_output_directory_still_copies_assets(self):
        self.run_output("separate")
