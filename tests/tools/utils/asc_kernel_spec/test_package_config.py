#!/usr/bin/python
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

from pathlib import Path
import subprocess
import shlex
import sys
import tempfile
import unittest

FUNCTIONS = Path(__file__).resolve().parents[4] / "cmake/asc/fwk_modules/func.cmake"


class ExistingPackageConfig(unittest.TestCase):
    def configure(self, body):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "CMakeLists.txt").write_text(
                f'cmake_minimum_required(VERSION 3.16)\nproject(contract NONE)\ninclude("{FUNCTIONS}")\n'
                + body
            )
            return subprocess.run(
                ["cmake", "-S", str(root), "-B", str(root / "build")],
                text=True,
                capture_output=True,
            )

    def test_existing_options_route_to_opc_not_device_compiler(self):
        util = FUNCTIONS.parent / "util"
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (
                root / "CMakeLists.txt"
            ).write_text(f'''cmake_minimum_required(VERSION 3.16)
project(options NONE)
include("{FUNCTIONS}")
set(ASCEND_PYTHON_EXECUTABLE "{sys.executable}")
set(ASCENDC_CMAKE_SCRIPTS_PATH "{FUNCTIONS.parent}")
set(CUSTOM_COMPILE_OPTIONS custom_compile_options.ini)
set_property(GLOBAL PROPERTY ASCENDC_AUTO_GEN_PATH "{root}")
npu_op_kernel_options(kernel ALL OPTIONS "--kernel-spec=Normal" "--kernel-spec-dir={root}/manifest with space" -O2 --save-temp-files)
npu_op_kernel_options(kernel AddCustom COMPUTE_UNIT ascend910b OPTIONS --kernel-spec=None)
''')
            configured = subprocess.run(
                ["cmake", "-S", str(root), "-B", str(root / "build")],
                capture_output=True,
                text=True,
            )
            self.assertEqual(
                configured.returncode, 0, configured.stdout + configured.stderr
            )
            device_options = (root / "custom_compile_options.ini").read_text()
            self.assertIn("-O2", device_options)
            self.assertNotIn("kernel-spec", device_options)
            self.assertIn("dump_cce", (root / "custom_opc_options.ini").read_text())
            for op_type, expected in (
                (
                    "Other",
                    [
                        "--kernel-spec=Normal",
                        f"--kernel-spec-dir={root}/manifest with space/ascend910b",
                    ],
                ),
                ("AddCustom", []),
            ):
                result = subprocess.run(
                    [
                        sys.executable,
                        str(util / "kernel_spec_options.py"),
                        "--config-file",
                        str(root / "custom_opc_options.ini"),
                        "--op-type",
                        op_type,
                        "--soc",
                        "Ascend910B1",
                    ],
                    check=True,
                    capture_output=True,
                    text=True,
                )
                self.assertEqual(shlex.split(result.stdout), expected)

    def test_invalid_mode(self):
        for mode in ("Wrong", "All"):
            with self.subTest(mode=mode):
                result = self.configure(
                    f"npu_op_package(demo TYPE RUN CONFIG KERNEL_SPEC_MODE {mode})\n"
                )
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(
                    "KERNEL_SPEC_MODE must be None, Normal or SK", result.stderr
                )

    def test_shared_configuration_is_rejected_in_both_orders(self):
        for mode, owner in [("Normal", ""), ("None", "other")]:
            with self.subTest(mode=mode):
                result = self.configure(f'''
set_property(GLOBAL PROPERTY _ASC_PKG_demo_TYPE RUN)
set_property(GLOBAL PROPERTY _ASC_PKG_demo_CODE_GEN_DIRS "${{CMAKE_BINARY_DIR}}/autogen")
set_property(GLOBAL PROPERTY _ASC_PKG_demo_KERNEL_SPEC_MODE {mode})
set_property(GLOBAL PROPERTY _ASC_KERNEL_PACKAGE_PARTICIPANTS other)
set_property(GLOBAL PROPERTY _ASC_KERNEL_SPEC_AUTOGEN_OWNER "{owner}")
set_property(GLOBAL PROPERTY _ASC_TGT_KERNEL_kernels_TYPE KERNEL)
set_property(GLOBAL PROPERTY ASCENDC_KERNEL_TARGET kernels)
set_property(GLOBAL PROPERTY _ASC_TGT_KERNEL_kernels_BINARY_DIR "${{CMAKE_BINARY_DIR}}/kernels")
npu_op_package_add(demo LIBRARY kernels)
''')
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(
                    "KERNEL_SPEC_MODE cannot share a CMake project", result.stderr
                )
