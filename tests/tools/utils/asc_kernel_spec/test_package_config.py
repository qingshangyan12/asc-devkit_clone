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

import json
import sys
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

    def test_existing_command_gets_isolated_kernel_spec_environment(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "probe.py").write_text(
                "import json,os,sys\nfrom pathlib import Path\n"
                "Path(sys.argv[1]).write_text(json.dumps([os.getenv('ASCENDC_KERNEL_SPEC_MODE'),"
                "os.getenv('ASCENDC_KERNEL_SPEC_DIR')]))\n"
            )
            (
                root / "CMakeLists.txt"
            ).write_text(f'''cmake_minimum_required(VERSION 3.16)
project(command_scope NONE)
include("{FUNCTIONS}")
npu_op_kernel_options(first ALL OPTIONS --kernel-spec=Normal "--kernel-spec-dir=${{CMAKE_BINARY_DIR}}/with space"
  OUTPUT_COMMAND first_command)
npu_op_kernel_options(second ALL OPTIONS --kernel-spec=None OUTPUT_COMMAND second_command)
add_custom_target(first ALL COMMAND ${{first_command}} "{sys.executable}" "{root}/probe.py" "{root}/first.json" VERBATIM)
add_custom_target(second ALL COMMAND ${{second_command}} "{sys.executable}" "{root}/probe.py" "{root}/second.json" VERBATIM)
''')
            result = subprocess.run(
                ["cmake", "-S", str(root), "-B", str(root / "build")],
                capture_output=True,
                text=True,
            )
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            result = subprocess.run(
                ["cmake", "--build", str(root / "build"), "--parallel", "2"],
                capture_output=True,
                text=True,
            )
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertEqual(
                json.loads((root / "first.json").read_text()),
                ["Normal", str(root / "build/with space")],
            )
            self.assertEqual(
                json.loads((root / "second.json").read_text()), ["None", ""]
            )

    def test_command_configuration_rejects_unsupported_options(self):
        for options, message in [
            ("ALL OPTIONS --kernel-spec=All", "KERNEL_SPEC_MODE must be"),
            (
                "ALL OPTIONS --kernel-spec=Normal --kernel-spec-dir=relative",
                "absolute path",
            ),
            ("Add OPTIONS --kernel-spec=None", "whole command"),
            ("ALL OPTIONS -O2", "only supports"),
        ]:
            with self.subTest(options=options):
                result = self.configure(
                    f"npu_op_kernel_options(demo {options} OUTPUT_COMMAND prefix)\n"
                )
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(message, result.stderr)

    def test_command_rejects_malformed_arguments(self):
        for arguments in (
            "OPTIONS --kernel-spec=Normal --kernel-spec=None OUTPUT_COMMAND prefix",
            "OPTIONS --kernel-spec=None OUTPUT_COMMAND prefix TYPO value",
            "OPTIONS --kernel-spec=None COMPUTE_UNIT OUTPUT_COMMAND prefix",
            "OPTIONS --kernel-spec=None OUTPUT_COMMAND",
            "OPTIONS OUTPUT_COMMAND prefix",
            "OPTIONS --kernel-spec=None --kernel-spec-dir=/one --kernel-spec-dir=/two OUTPUT_COMMAND prefix",
        ):
            with self.subTest(arguments=arguments):
                result = self.configure(f"npu_op_kernel_options(k ALL {arguments})\n")
                self.assertNotEqual(result.returncode, 0, result.stdout)

    def test_command_options_ignore_project_cache_variables(self):
        result = self.configure("""
set(spec_mode SK CACHE STRING "Project setting")
set(spec_dir /unrelated CACHE STRING "Project setting")
npu_op_kernel_options(k ALL OPTIONS --kernel-spec=Normal --kernel-spec-dir=/resources OUTPUT_COMMAND prefix)
if(NOT "${prefix}" MATCHES "ASCENDC_KERNEL_SPEC_MODE=Normal;ASCENDC_KERNEL_SPEC_DIR=/resources$")
  message(FATAL_ERROR "Project cache affected command options")
endif()
""")
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

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
