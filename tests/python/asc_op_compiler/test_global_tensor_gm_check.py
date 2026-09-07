# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# Licensed under the CANN Open Software License Agreement Version 2.0.
# See LICENSE in the root of the software repository for details.
"""Compile GlobalTensor GM checks for NPU targets and verify header dependencies."""

from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[3]
CHECK = "__ASCENDC_SUPER_KERNEL_CHECK_GM_GET_SET_VALUE_DCCI__"
DCCI = "__ASCENDC_SUPER_KERNEL_ENABLE_GM_GET_SET_VALUE_DCCI__"


class TestGlobalTensorGmCheckDeviceCompile(unittest.TestCase):
    def test_device_objects_and_header_order(self):
        if not shutil.which("bisheng") or not shutil.which("readelf"):
            self.skipTest("bisheng and readelf are required")
        fixture = ROOT / "tests/api/basic_api/ascendc_header_checker/kernel_tensor_gm_check.cpp"
        header = ROOT / "tests/api/basic_api/ascendc_header_checker/kernel_tpipe.cpp"
        with tempfile.TemporaryDirectory(prefix="global-tensor-gm-device-") as temp:
            for arch in ("dav-c220-vec", "dav-c220-cube", "dav-c310-vec", "dav-c310-cube"):
                for check, dcci in ((True, False), (True, True), (False, True)):
                    with self.subTest(arch=arch, check=check, dcci=dcci):
                        output = Path(temp) / f"{arch}-{int(check)}-{int(dcci)}.o"
                        command = [
                            "bisheng", "-x", "cce", "-std=c++17", "--cce-aicore-only",
                            f"--cce-aicore-arch={arch}", "-O2", "-Werror",
                            f"-I{ROOT}", f"-I{ROOT / 'include'}", f"-I{ROOT / 'include/basic_api'}",
                            "-UASCENDC_DEBUG", "-DNDEBUG",
                        ]
                        if check:
                            command.append(f"-D{CHECK}")
                        if dcci:
                            command.append(f"-D{DCCI}")
                        else:
                            command.append("-DASCENDC_DUMP=0")
                        result = subprocess.run(
                            command + ["-c", str(fixture), "-o", str(output)],
                            capture_output=True, text=True, timeout=120,
                        )
                        self.assertEqual(result.returncode, 0, result.stderr)
                        symbols = subprocess.run(
                            ["readelf", "-Ws", str(output)], capture_output=True,
                            text=True, check=True, timeout=10,
                        ).stdout
                        self.assertIn("GmCheckUnboundRead", symbols)
                        self.assertIn("GmCheckUnboundWrite", symbols)
                        if not check:
                            self.assertNotIn("CheckGlobalBuffer", symbols)
                            self.assertNotIn("printf_impl_assert", symbols)
                        # ELF's mandatory null symbol is the only allowed undefined symbol.
                        undefined = [line for line in symbols.splitlines() if " UND " in line]
                        self.assertEqual(len(undefined), 1, symbols)
                        result = subprocess.run(
                            command + ["-DVERIFY_SINGLE_HEADER", "-fsyntax-only", str(header)],
                            capture_output=True, text=True, timeout=120,
                        )
                        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
