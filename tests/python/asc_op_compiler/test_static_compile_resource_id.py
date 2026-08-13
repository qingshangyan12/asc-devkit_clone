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

import importlib.util
from contextlib import ExitStack
import os
from pathlib import Path
import tempfile
import unittest
from unittest import mock


MODULE_PATH = os.path.join(
    os.path.dirname(__file__),
    "../../../tools/build/asc_op_compile_base/asc_op_compiler/static_compile_resource_id.py",
)
SPEC = importlib.util.spec_from_file_location("static_compile_resource_id", MODULE_PATH)
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class TestStaticCompileResourceId(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp_dir.cleanup)
        self.object_path = os.path.join(self.temp_dir.name, "kernel.o")
        Path(self.object_path).write_bytes(b"original object")

    def assert_no_resource_id_temporary_files(self):
        temporary_files = [
            name
            for name in os.listdir(self.temp_dir.name)
            if name.startswith(".resource_id.")
        ]
        self.assertEqual(temporary_files, [])

    def test_generate_and_write_does_not_repeat_write_verification(self):
        resource_id = "a" * 64
        with ExitStack() as stack:
            calculate = stack.enter_context(
                mock.patch.object(
                    MODULE, "calculate_resource_id", return_value=resource_id
                )
            )
            write = stack.enter_context(
                mock.patch.object(MODULE, "write_resource_id_section")
            )
            read = stack.enter_context(
                mock.patch.object(
                    MODULE,
                    "read_resource_id_section",
                    side_effect=AssertionError(
                        "write verification must not be repeated"
                    ),
                )
            )

            result = MODULE.generate_and_write_resource_id(self.object_path)

        self.assertEqual(result, resource_id)
        calculate.assert_called_once_with(self.object_path)
        write.assert_called_once_with(self.object_path, resource_id)
        read.assert_not_called()

    def test_invalid_inputs_do_not_modify_object(self):
        before = Path(self.object_path).read_bytes()
        for resource_id in ("A" * 64, "0" * 63, "z" * 64):
            with self.subTest(resource_id=resource_id):
                with mock.patch.object(MODULE, "_validate_elf"):
                    with self.assertRaisesRegex(MODULE.ResourceIdError, "64 lowercase"):
                        MODULE.write_resource_id_section(self.object_path, resource_id)
                self.assertEqual(Path(self.object_path).read_bytes(), before)

        with self.subTest(input_type="non_elf"):
            with self.assertRaisesRegex(MODULE.ResourceIdError, "ELF"):
                MODULE.calculate_resource_id(self.object_path)

        with self.subTest(input_type="symlink"):
            symlink_path = os.path.join(self.temp_dir.name, "kernel-link.o")
            os.symlink(self.object_path, symlink_path)
            with self.assertRaisesRegex(MODULE.ResourceIdError, "regular file"):
                MODULE.calculate_resource_id(symlink_path)

        with self.subTest(input_type="existing_section"):
            section = [("PROGBITS", 64, "", 1)]
            with ExitStack() as stack:
                stack.enter_context(mock.patch.object(MODULE, "_validate_elf"))
                stack.enter_context(
                    mock.patch.object(
                        MODULE, "_resource_id_section_info", return_value=section
                    )
                )
                with self.assertRaisesRegex(MODULE.ResourceIdError, "already exists"):
                    MODULE.write_resource_id_section(self.object_path, "0" * 64)
            self.assertEqual(Path(self.object_path).read_bytes(), before)

    def test_write_failures_preserve_original_and_clean_temporary_files(self):
        resource_id = "a" * 64
        original = Path(self.object_path).read_bytes()

        with self.subTest(stage="objcopy"):
            with ExitStack() as stack:
                stack.enter_context(mock.patch.object(MODULE, "_validate_elf"))
                stack.enter_context(
                    mock.patch.object(
                        MODULE, "_resource_id_section_info", return_value=[]
                    )
                )
                stack.enter_context(
                    mock.patch.object(MODULE, "_find_tool", return_value="llvm-objcopy")
                )
                stack.enter_context(
                    mock.patch.object(
                        MODULE,
                        "_run_tool",
                        side_effect=MODULE.ResourceIdError("objcopy failed"),
                    )
                )
                with self.assertRaisesRegex(MODULE.ResourceIdError, "objcopy failed"):
                    MODULE.write_resource_id_section(self.object_path, resource_id)
            self.assertEqual(Path(self.object_path).read_bytes(), original)
            self.assert_no_resource_id_temporary_files()

        with self.subTest(stage="readback"):
            with ExitStack() as stack:
                stack.enter_context(mock.patch.object(MODULE, "_validate_elf"))
                stack.enter_context(
                    mock.patch.object(
                        MODULE, "_resource_id_section_info", return_value=[]
                    )
                )
                stack.enter_context(
                    mock.patch.object(MODULE, "_find_tool", return_value="llvm-objcopy")
                )
                stack.enter_context(
                    mock.patch.object(MODULE, "_run_tool", return_value="")
                )
                stack.enter_context(
                    mock.patch.object(
                        MODULE, "read_resource_id_section", return_value="b" * 64
                    )
                )
                with self.assertRaisesRegex(
                    MODULE.ResourceIdError, "verification failed"
                ):
                    MODULE.write_resource_id_section(self.object_path, resource_id)
            self.assertEqual(Path(self.object_path).read_bytes(), original)
            self.assert_no_resource_id_temporary_files()

        with self.subTest(stage="replace"):
            with ExitStack() as stack:
                stack.enter_context(mock.patch.object(MODULE, "_validate_elf"))
                stack.enter_context(
                    mock.patch.object(
                        MODULE, "_resource_id_section_info", return_value=[]
                    )
                )
                stack.enter_context(
                    mock.patch.object(MODULE, "_find_tool", return_value="llvm-objcopy")
                )
                stack.enter_context(
                    mock.patch.object(MODULE, "_run_tool", return_value="")
                )
                stack.enter_context(
                    mock.patch.object(
                        MODULE, "read_resource_id_section", return_value=resource_id
                    )
                )
                stack.enter_context(
                    mock.patch.object(
                        MODULE.os,
                        "replace",
                        side_effect=OSError("replace failed"),
                    )
                )
                with self.assertRaisesRegex(OSError, "replace failed"):
                    MODULE.write_resource_id_section(self.object_path, resource_id)
            self.assertEqual(Path(self.object_path).read_bytes(), original)
            self.assert_no_resource_id_temporary_files()

        with self.subTest(stage="payload_creation"):
            real_mkstemp = tempfile.mkstemp
            call_count = 0

            def fail_second_mkstemp(*args, **kwargs):
                nonlocal call_count
                call_count += 1
                if call_count == 2:
                    raise OSError("payload creation failed")
                return real_mkstemp(*args, **kwargs)

            with ExitStack() as stack:
                stack.enter_context(mock.patch.object(MODULE, "_validate_elf"))
                stack.enter_context(
                    mock.patch.object(
                        MODULE, "_resource_id_section_info", return_value=[]
                    )
                )
                stack.enter_context(
                    mock.patch.object(MODULE, "_find_tool", return_value="llvm-objcopy")
                )
                stack.enter_context(
                    mock.patch.object(
                        MODULE.tempfile,
                        "mkstemp",
                        side_effect=fail_second_mkstemp,
                    )
                )
                with self.assertRaisesRegex(OSError, "payload creation failed"):
                    MODULE.write_resource_id_section(self.object_path, resource_id)
            self.assertEqual(Path(self.object_path).read_bytes(), original)
            self.assert_no_resource_id_temporary_files()

    def test_rejects_invalid_resource_id_sections(self):
        metadata_cases = (
            ([], "expected one"),
            ([("PROGBITS", 64, "", 1)] * 2, "expected one"),
            ([("NOBITS", 64, "", 1)], "SHT_PROGBITS"),
            ([("PROGBITS", 63, "", 1)], "64 bytes"),
            ([("PROGBITS", 64, "A", 1)], "SHF_ALLOC"),
            ([("PROGBITS", 64, "", 8)], "alignment"),
        )
        for section_info, error_pattern in metadata_cases:
            with self.subTest(section_info=section_info):
                with ExitStack() as stack:
                    stack.enter_context(mock.patch.object(MODULE, "_validate_elf"))
                    stack.enter_context(
                        mock.patch.object(
                            MODULE,
                            "_resource_id_section_info",
                            return_value=section_info,
                        )
                    )
                    with self.assertRaisesRegex(MODULE.ResourceIdError, error_pattern):
                        MODULE.read_resource_id_section(self.object_path)

        content_cases = (b"A" * 64, b"0" * 63, b"\xff" * 64)
        for content in content_cases:
            with self.subTest(content=content):

                def dump_section(command):
                    dump_path = command[2].split("=", 1)[1]
                    Path(dump_path).write_bytes(content)
                    return ""

                with ExitStack() as stack:
                    stack.enter_context(mock.patch.object(MODULE, "_validate_elf"))
                    stack.enter_context(
                        mock.patch.object(MODULE, "_validate_section_metadata")
                    )
                    stack.enter_context(
                        mock.patch.object(
                            MODULE, "_find_tool", return_value="llvm-objcopy"
                        )
                    )
                    stack.enter_context(
                        mock.patch.object(MODULE, "_run_tool", side_effect=dump_section)
                    )
                    with self.assertRaises(MODULE.ResourceIdError):
                        MODULE.read_resource_id_section(self.object_path)


if __name__ == "__main__":
    unittest.main()
