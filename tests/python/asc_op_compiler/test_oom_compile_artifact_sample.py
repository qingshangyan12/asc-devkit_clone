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
import subprocess
import sys
import unittest
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[3]
SAMPLE = (
    REPO_ROOT
    / "examples/01_simd_cpp_api/02_features/04_compile"
    / "04_oom_compile_artifact_check/check_oom_artifacts.py"
)
CANN_PATH = Path("/data/chu/Ascend/cann")


class TestOomCompileArtifactSample(unittest.TestCase):
    def run_sample(self, devkit_path):
        return subprocess.run(
            [
                sys.executable,
                str(SAMPLE),
                "--devkit-path",
                str(devkit_path),
                "--cann-path",
                str(CANN_PATH),
            ],
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            check=False,
        )

    def test_all_artifact_contracts_pass(self):
        result = self.run_sample(REPO_ROOT)

        self.assertEqual(result.returncode, 0, result.stdout)
        expected_lines = (
            "PASS dynamic tensor keeps legacy fallback",
            "PASS dynamic tensor emits storage-shape registration",
            "PASS dynamic TensorList keeps legacy fallback",
            "PASS dynamic TensorList emits storage-shape registration",
            "PASS static shape keeps legacy OOM generation",
            "PASS non-OOM artifacts remain unchanged",
            "PASS OOM JSON capability and opParaSize",
            "All OOM compile artifact checks passed.",
        )
        for line in expected_lines:
            self.assertIn(line, result.stdout)

    def test_invalid_devkit_path_is_an_environment_error(self):
        result = self.run_sample("/tmp/not-a-devkit")

        self.assertEqual(result.returncode, 2, result.stdout)
        self.assertIn("ERROR", result.stdout)


if __name__ == "__main__":
    unittest.main()
