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

from __future__ import annotations

import os
import shlex
import subprocess
import tempfile
import unittest
from pathlib import Path

from presmoke.generate_case_runners import ONNX_PLUGIN_CASE, RunnerRenderSpec, render_runner
from presmoke.model import Command


class OnnxRunnerTest(unittest.TestCase):
    def test_generated_runner_checks_result_and_preserves_exit_status(self) -> None:
        project_root = Path(__file__).resolve().parents[3]
        for output, program_rc, expected_verify_rc in [
            ("test pass\n", 0, 0),
            ("test failed\n", 0, 1),
            ("result is: 0.0\n", 0, 1),
            ("[ERROR] aclmdlExecute failed\ntest pass\n", 0, 1),
            ("test failed at index 0\ntest pass\n", 0, 1),
            ("test pass\n", 7, 0),
        ]:
            with self.subTest(output=output, program_rc=program_rc), tempfile.TemporaryDirectory() as tmp:
                root = Path(tmp)
                case_dir = root / "examples" / ONNX_PLUGIN_CASE
                build_dir = case_dir / "build_npu"
                build_dir.mkdir(parents=True)
                program = build_dir / "execute_leaky_relu_op"
                program.write_text(f"#!/bin/bash\nprintf %s {shlex.quote(output)}\nexit {program_rc}\n")
                program.chmod(0o755)
                runner = root / "scripts/presmoke/cases" / ONNX_PLUGIN_CASE / "run.sh"
                runner.parent.mkdir(parents=True)
                entry = root / "scripts/presmoke/cases/_case_entry.sh"
                entry.write_text((project_root / "scripts/presmoke/cases/_case_entry.sh").read_text())
                spec = RunnerRenderSpec(
                    rel=ONNX_PLUGIN_CASE,
                    build_cmds=[],
                    run_cmds=[Command("./execute_leaky_relu_op", "run")],
                    verify_cmds=[],
                )
                runner.write_text(render_runner(spec))
                env = dict(os.environ, PRESMOKE_PROJECT_ROOT=str(root), PRESMOKE_MODE="npu")
                common = root / "scripts/presmoke/case_common.sh"
                common.write_text((project_root / "scripts/presmoke/case_common.sh").read_text())
                run = subprocess.run(["bash", str(runner), "run"], env=env, text=True, capture_output=True)
                self.assertEqual(run.returncode, program_rc, run.stdout + run.stderr)
                verify = subprocess.run(["bash", str(runner), "verify"], env=env, text=True, capture_output=True)
                self.assertEqual(verify.returncode, expected_verify_rc, verify.stdout + verify.stderr)
                self.assertEqual((build_dir / "run.log").read_text(), output)


if __name__ == "__main__":
    unittest.main()
