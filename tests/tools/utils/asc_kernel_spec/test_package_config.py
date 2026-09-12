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

    def test_invalid_mode(self):
        result = self.configure(
            "npu_op_package(demo TYPE RUN CONFIG KERNEL_SPEC_MODE Wrong)\n"
        )
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("KERNEL_SPEC_MODE must be None, Normal, SK or All", result.stderr)

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
