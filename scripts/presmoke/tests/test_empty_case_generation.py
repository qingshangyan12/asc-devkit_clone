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

import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

from presmoke.case_runners import CaseRunnerOptions, build_case_runner_cells_with_skips
from presmoke.generate_case_runners import (
    CUSTOM_OP_PACKAGE_CASE,
    PARALLEL_OPS_PACKAGE_CASE,
    TILING_SINK_PROGRAMMING_CASE,
    build_runner_render_spec,
    cell_for_spec,
)
from presmoke.model import ExampleSpec


class EmptyCaseGenerationTest(unittest.TestCase):
    def test_empty_case_is_skipped_by_generated_manifest_and_consumer(self):
        scripts_root = Path(__file__).resolve().parents[2]
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            for name, body in (
                ("empty", "No executable commands."),
                ("runnable", "- 样例执行\n```bash\n./run.sh\n```"),
            ):
                example = root / "examples" / name
                example.mkdir(parents=True)
                (example / "README.md").write_text(
                    f"## 支持的产品\nAtlas A2\n\n## 编译运行\n{body}\n", encoding="utf-8"
                )
            env = dict(os.environ, PYTHONPATH=str(scripts_root), PYTHONDONTWRITEBYTECODE="1")
            subprocess.run(
                [sys.executable, "-m", "presmoke.generate_case_runners", "--project-root", str(root)],
                check=True,
                capture_output=True,
                text=True,
                encoding="utf-8",
                env=env,
            )
            manifest = json.loads(
                (root / "scripts/presmoke/reports/case_runner_manifest.json").read_text(encoding="utf-8")
            )
            items = {item["case"]: item for item in manifest}
            self.assertTrue(items["empty"]["skip"])
            self.assertIn("no runnable commands", items["empty"]["skip_reason"])
            self.assertNotEqual(items["empty"]["confidence"], "high")
            self.assertIn("no_runnable_commands", items["empty"]["reasons"])
            self.assertFalse(items["runnable"]["skip"])
            runner = (root / "scripts/presmoke/cases/empty/run.sh").read_text(encoding="utf-8")
            self.assertIn("SKIP_REASON='no runnable commands'", runner)
            cells, _, skips = build_case_runner_cells_with_skips(root, CaseRunnerOptions("dav-2201", ["npu"], [], []))
            self.assertEqual([cell.example.rel_path for cell in cells], ["runnable"])
            self.assertEqual([(item.example, item.status) for item in skips], [("empty", "SKIP")])
            _, _, unsupported_arch = build_case_runner_cells_with_skips(
                root, CaseRunnerOptions("dav-3510", ["npu"], [], [])
            )
            self.assertEqual(unsupported_arch, [])
            _, _, unsupported_mode = build_case_runner_cells_with_skips(
                root, CaseRunnerOptions("dav-2201", ["sim"], [], [])
            )
            self.assertEqual(unsupported_mode, [])
            if os.name != "nt":
                runner_dir = root / "scripts/presmoke/cases"
                for source, dest in (
                    (scripts_root / "presmoke/cases/_case_entry.sh", runner_dir / "_case_entry.sh"),
                    (scripts_root / "presmoke/case_common.sh", runner_dir.parent / "case_common.sh"),
                ):
                    dest.write_text(source.read_text(encoding="utf-8"), encoding="utf-8")
                result = subprocess.run(
                    ["bash", str(runner_dir / "empty/run.sh"), "all"],
                    capture_output=True,
                    text=True,
                    env=dict(env, PRESMOKE_PROJECT_ROOT=str(root), PRESMOKE_MODE="npu"),
                )
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertIn("SKIP: no runnable commands", result.stdout)

    def test_custom_runners_are_not_skipped_just_because_readme_has_no_commands(self):
        for rel in (CUSTOM_OP_PACKAGE_CASE, PARALLEL_OPS_PACKAGE_CASE, TILING_SINK_PROGRAMMING_CASE):
            with self.subTest(rel=rel):
                spec = ExampleSpec(Path("/tmp") / rel, rel, [], ["dav-2201"], ["npu"], "convention")
                render_spec, _ = build_runner_render_spec(cell_for_spec(spec, "dav-2201", "npu"), "low", [])
                self.assertNotIn("no runnable commands", render_spec.skip_reason)


if __name__ == "__main__":
    unittest.main()
