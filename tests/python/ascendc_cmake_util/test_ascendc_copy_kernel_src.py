#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License"). Please refer to the License for details.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from typing import Tuple

REPO_ROOT = Path(__file__).resolve().parents[3]
COPY_KERNEL_SRC = REPO_ROOT / "cmake/asc/fwk_modules/util/ascendc_copy_kernel_src.py"


class TestCopyKernelSource(unittest.TestCase):
    def _copy_and_check(self, kernel_base: Path) -> None:
        with tempfile.TemporaryDirectory(prefix="ascendc-copy-kernel-src-") as temporary:
            root = Path(temporary)
            build_dir = root / "build" / "project"
            auto_gen_dir = build_dir / "autogen"
            dst_dir = build_dir / "demo" / "binary" / "dynamic"
            build_dir.mkdir(parents=True)
            auto_gen_dir.mkdir()
            (auto_gen_dir / "demo_custom_source_files.ini").write_text(
                "[deadbeef_AddCustom]\nkernel_dir = .\n", encoding="utf-8"
            )

            result = subprocess.run(
                [
                    sys.executable,
                    str(COPY_KERNEL_SRC),
                    f"--auto-gen-path={auto_gen_dir}",
                    "--target-name=demo",
                    f"--kernel-base-dir={kernel_base}{os.sep}",
                    f"--copy-dst-dir={dst_dir}",
                ],
                cwd=build_dir,
                text=True,
                capture_output=True,
                check=False,
            )
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            copied_file = dst_dir / "nested" / "kernel.cpp"
            self.assertTrue(copied_file.is_file(), f"kernel source was not copied from {kernel_base}")
            self.assertEqual(copied_file.read_text(encoding="utf-8"), "kernel-source\n")

    def _make_source_tree(self, root: Path) -> Tuple[Path, Path]:
        source_root = root / "source"
        source_dir = source_root / "real"
        nested_dir = source_dir / "nested"
        nested_dir.mkdir(parents=True)
        (nested_dir / "kernel.cpp").write_text("kernel-source\n", encoding="utf-8")
        return source_root, source_dir

    def test_regular_kernel_source_directory_is_copied(self) -> None:
        with tempfile.TemporaryDirectory(prefix="ascendc-copy-kernel-src-regular-") as temporary:
            _source_root, source_dir = self._make_source_tree(Path(temporary))
            self._copy_and_check(source_dir)

    def test_absolute_target_symlink_is_copied(self) -> None:
        with tempfile.TemporaryDirectory(prefix="ascendc-copy-kernel-src-absolute-") as temporary:
            source_root, source_dir = self._make_source_tree(Path(temporary))
            link = source_root / "absolute-target"
            link.symlink_to(source_dir, target_is_directory=True)
            self._copy_and_check(link)

    def test_absolute_symlink_path_with_relative_target_is_copied(self) -> None:
        with tempfile.TemporaryDirectory(prefix="ascendc-copy-kernel-src-relative-") as temporary:
            source_root, source_dir = self._make_source_tree(Path(temporary))
            link = source_root / "relative-target"
            link.symlink_to(Path("real"), target_is_directory=True)
            self.assertTrue(link.is_absolute())
            self.assertEqual(link.resolve(), source_dir.resolve())
            self._copy_and_check(link)


if __name__ == "__main__":
    unittest.main()
