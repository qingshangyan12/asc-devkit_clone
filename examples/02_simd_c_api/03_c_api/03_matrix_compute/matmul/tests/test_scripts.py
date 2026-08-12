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
import subprocess
import sys
import tempfile
import unittest

from pathlib import Path

import numpy as np


ROOT = Path(__file__).resolve().parents[1]
GEN_DATA = ROOT / "scripts" / "gen_data.py"
VERIFY_RESULT = ROOT / "scripts" / "verify_result.py"
RUN_CASES = ROOT / "scripts" / "run_cases.py"


class DataGenerationTests(unittest.TestCase):
    def run_generator(
        self, case_name: str, output_dir: Path
    ) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [
                sys.executable,
                str(GEN_DATA),
                "--case",
                case_name,
                "--output-dir",
                str(output_dir),
            ],
            cwd=ROOT,
            capture_output=True,
            text=True,
            check=False,
        )

    def test_p01_writes_expected_shapes_and_bias_golden(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output_dir = Path(directory)
            result = self.run_generator("P01", output_dir)

            self.assertEqual(result.returncode, 0, result.stderr)
            a = np.fromfile(output_dir / "x1_gm.bin", dtype=np.float32).reshape(30, 40)
            b = np.fromfile(output_dir / "x2_gm.bin", dtype=np.float32).reshape(40, 70)
            bias = np.fromfile(output_dir / "bias_gm.bin", dtype=np.float32)
            golden = np.fromfile(output_dir / "golden.bin", dtype=np.float64).reshape(
                30, 70
            )

            self.assertTrue(np.array_equal(a, np.zeros((30, 40), dtype=np.float32)))
            self.assertTrue(np.array_equal(b, np.zeros((40, 70), dtype=np.float32)))
            self.assertEqual(bias.shape, (70,))
            self.assertTrue(
                np.array_equal(
                    golden, np.broadcast_to(bias.astype(np.float64), (30, 70))
                )
            )

    def test_seeded_random_case_is_reproducible_and_bounded(self) -> None:
        with tempfile.TemporaryDirectory() as first, tempfile.TemporaryDirectory() as second:
            first_dir = Path(first)
            second_dir = Path(second)
            first_result = self.run_generator("P06", first_dir)
            second_result = self.run_generator("P06", second_dir)

            self.assertEqual(first_result.returncode, 0, first_result.stderr)
            self.assertEqual(second_result.returncode, 0, second_result.stderr)
            for filename in ("x1_gm.bin", "x2_gm.bin", "bias_gm.bin", "golden.bin"):
                self.assertEqual(
                    (first_dir / filename).read_bytes(),
                    (second_dir / filename).read_bytes(),
                )

            for filename in ("x1_gm.bin", "x2_gm.bin", "bias_gm.bin"):
                values = np.fromfile(first_dir / filename, dtype=np.float32)
                self.assertGreaterEqual(float(values.min()), -100.0)
                self.assertLessEqual(float(values.max()), 100.0)

    def test_all_cases_write_exact_files_and_metadata(self) -> None:
        expected_sizes = {
            "x1_gm.bin": 30 * 40 * np.dtype(np.float32).itemsize,
            "x2_gm.bin": 40 * 70 * np.dtype(np.float32).itemsize,
            "bias_gm.bin": 70 * np.dtype(np.float32).itemsize,
            "golden.bin": 30 * 70 * np.dtype(np.float64).itemsize,
        }

        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for case_number in range(1, 9):
                case_name = f"P{case_number:02d}"
                output_dir = root / case_name
                result = self.run_generator(case_name, output_dir)

                self.assertEqual(result.returncode, 0, result.stderr)
                for filename, expected_size in expected_sizes.items():
                    self.assertEqual(
                        (output_dir / filename).stat().st_size, expected_size
                    )

                metadata = json.loads(
                    (output_dir / "case.json").read_text(encoding="utf-8")
                )
                self.assertEqual(metadata["case"], case_name)
                self.assertEqual(metadata["a_shape"], [30, 40])
                self.assertEqual(metadata["b_shape"], [40, 70])
                self.assertEqual(metadata["bias_shape"], [1, 70])
                self.assertEqual(metadata["output_shape"], [30, 70])

    def test_cancellation_case_reduces_to_bias(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output_dir = Path(directory)
            result = self.run_generator("P04", output_dir)

            self.assertEqual(result.returncode, 0, result.stderr)
            bias = np.fromfile(output_dir / "bias_gm.bin", dtype=np.float32)
            golden = np.fromfile(output_dir / "golden.bin", dtype=np.float64).reshape(
                30, 70
            )
            self.assertTrue(np.all(np.abs(bias) >= 10.0))
            self.assertTrue(
                np.array_equal(
                    golden, np.broadcast_to(bias.astype(np.float64), (30, 70))
                )
            )

    def test_unknown_case_fails_without_writing_data(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output_dir = Path(directory)
            result = self.run_generator("P99", output_dir)

            self.assertNotEqual(result.returncode, 0)
            self.assertFalse((output_dir / "x1_gm.bin").exists())


class ResultVerificationTests(unittest.TestCase):
    def generate_case(self, case_name: str, output_dir: Path) -> None:
        result = subprocess.run(
            [
                sys.executable,
                str(GEN_DATA),
                "--case",
                case_name,
                "--output-dir",
                str(output_dir),
            ],
            cwd=ROOT,
            capture_output=True,
            text=True,
            check=False,
        )
        self.assertEqual(result.returncode, 0, result.stderr)

    def run_verifier(
        self, actual: Path, golden: Path
    ) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [sys.executable, str(VERIFY_RESULT), str(actual), str(golden)],
            cwd=ROOT,
            capture_output=True,
            text=True,
            check=False,
        )

    def test_exact_float32_result_passes_and_reports_metrics(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output_dir = Path(directory)
            self.generate_case("P06", output_dir)
            golden = np.fromfile(output_dir / "golden.bin", dtype=np.float64)
            actual_path = output_dir / "actual.bin"
            golden.astype(np.float32).tofile(actual_path)

            result = self.run_verifier(actual_path, output_dir / "golden.bin")

            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("matched_ratio=1.000000", result.stdout)
            self.assertIn("max_abs_error=", result.stdout)
            self.assertIn("max_ulp_error=", result.stdout)

    def test_single_large_error_fails_maximum_error_guard(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output_dir = Path(directory)
            self.generate_case("P02", output_dir)
            actual = np.zeros(30 * 70, dtype=np.float32)
            actual[137] = np.float32(1.0)
            actual_path = output_dir / "actual.bin"
            actual.tofile(actual_path)

            result = self.run_verifier(actual_path, output_dir / "golden.bin")

            self.assertNotEqual(result.returncode, 0)
            self.assertIn("status=FAILED", result.stdout)
            self.assertIn("worst_index=137", result.stdout)

    def test_maximum_error_ulp_is_measured_at_worst_absolute_error(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output_dir = Path(directory)
            golden = np.zeros(30 * 70, dtype=np.float64)
            golden[100] = 50_000.0
            golden_path = output_dir / "golden.bin"
            golden.tofile(golden_path)

            actual = golden.astype(np.float32)
            for _ in range(4):
                actual[100] = np.nextafter(actual[100], np.float32(np.inf))
            actual[101] = np.float32(1.0e-6)
            actual_path = output_dir / "actual.bin"
            actual.tofile(actual_path)

            result = self.run_verifier(actual_path, golden_path)

            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("worst_index=100", result.stdout)
            self.assertIn("max_abs_error_ulp=4", result.stdout)

    def test_wrong_output_size_fails_with_clear_message(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output_dir = Path(directory)
            self.generate_case("P02", output_dir)
            actual_path = output_dir / "actual.bin"
            np.zeros(10, dtype=np.float32).tofile(actual_path)

            result = self.run_verifier(actual_path, output_dir / "golden.bin")

            self.assertNotEqual(result.returncode, 0)
            self.assertIn("expected 2100 float32 values", result.stderr)

    def test_non_finite_output_fails_and_reports_count(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output_dir = Path(directory)
            self.generate_case("P02", output_dir)
            actual = np.zeros(30 * 70, dtype=np.float32)
            actual[17] = np.nan
            actual[23] = np.inf
            actual_path = output_dir / "actual.bin"
            actual.tofile(actual_path)

            result = self.run_verifier(actual_path, output_dir / "golden.bin")

            self.assertNotEqual(result.returncode, 0)
            self.assertIn("non_finite_count=2", result.stdout)
            self.assertIn("status=FAILED", result.stdout)


class CaseRunnerTests(unittest.TestCase):
    def test_runner_executes_selected_cases_and_repeats(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            work_dir = Path(directory)
            fake_binary = work_dir / "fake_demo.py"
            fake_binary.write_text(
                f"#!{sys.executable}\n"
                "from pathlib import Path\n"
                "import numpy as np\n"
                "Path('output').mkdir(exist_ok=True)\n"
                "golden = np.fromfile('input/golden.bin', dtype=np.float64)\n"
                "golden.astype(np.float32).tofile('output/output.bin')\n",
                encoding="utf-8",
            )
            fake_binary.chmod(0o755)

            result = subprocess.run(
                [
                    sys.executable,
                    str(RUN_CASES),
                    "--binary",
                    str(fake_binary),
                    "--work-dir",
                    str(work_dir),
                    "--repeat",
                    "2",
                    "--cases",
                    "P01",
                    "P02",
                ],
                cwd=ROOT,
                capture_output=True,
                text=True,
                check=False,
            )

            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertEqual(result.stdout.count("status=PASSED"), 4)
            self.assertIn("summary=4/4 passed", result.stdout)


if __name__ == "__main__":
    unittest.main()
