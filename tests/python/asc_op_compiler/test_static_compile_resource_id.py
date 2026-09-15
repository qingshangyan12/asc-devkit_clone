#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
import hashlib
import importlib.util
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
        self.object_paths = []
        for index, content in enumerate((b"\x7fELFfirst", b"\x7fELFsecond")):
            path = os.path.join(self.temp_dir.name, f"kernel{index}.o")
            Path(path).write_bytes(content)
            self.object_paths.append(path)

    def test_resource_id_is_bytewise_or_of_object_digests(self):
        expected = bytearray(32)
        for path in self.object_paths:
            for index, value in enumerate(
                hashlib.sha256(Path(path).read_bytes()).digest()
            ):
                expected[index] |= value
        self.assertEqual(
            MODULE.calculate_resource_id(self.object_paths), bytes(expected).hex()
        )

    def test_empty_or_non_elf_inputs_are_rejected(self):
        with self.assertRaisesRegex(MODULE.ResourceIdError, "must not be empty"):
            MODULE.calculate_resource_id([])
        bad_path = os.path.join(self.temp_dir.name, "bad.o")
        Path(bad_path).write_bytes(b"not elf")
        with self.assertRaisesRegex(MODULE.ResourceIdError, "not an ELF"):
            MODULE.calculate_resource_id([bad_path])

    def test_generates_metadata_object_and_keeps_inputs_unchanged(self):
        output_path = os.path.join(self.temp_dir.name, "resource_id.o")
        commands = []
        sources = []
        original = [Path(path).read_bytes() for path in self.object_paths]

        def record(command):
            commands.append(command)
            source_index = command.index("-xcce") + 1
            sources.append(Path(command[source_index]).read_text(encoding="utf-8"))

        with mock.patch.object(MODULE, "_run_tool", side_effect=record):
            resource_id = MODULE.generate_resource_id_object(
                self.object_paths,
                output_path,
                ["fake-ccec"],
                ["-DTEST_OPTION"],
                "dav-c220-cube",
            )
        self.assertEqual(len(resource_id), 64)
        self.assertEqual(
            commands[0][:5],
            ["fake-ccec", "-DTEST_OPTION", "-c", "-O3", "-xcce"],
        )
        self.assertIn("--cce-aicore-arch=dav-c220-cube", commands[0])
        self.assertIn("-std=c++17", commands[0])
        self.assertEqual(commands[0][-1], output_path)
        source = sources[0]
        self.assertIn('__attribute__((used, section(".ascend.meta")))', source)
        self.assertIn('#include "kernel_utils_macros.h"', source)
        self.assertIn(
            "static const struct BinaryMetaResourceId g_ascend_resource_id_section = "
            "{{{%d, %d}, {%s}}};"
            % (
                MODULE._RESOURCE_ID_VALUE_SIZE,
                MODULE._RESOURCE_ID_TYPE,
                ", ".join(f"'{char}'" for char in resource_id),
            ),
            source,
        )
        self.assertEqual(Path(self.object_paths[0]).read_bytes(), original[0])
        self.assertEqual(Path(self.object_paths[1]).read_bytes(), original[1])
        self.assertEqual(Path(output_path).exists(), False)

    def test_missing_compiler_or_arch_is_rejected(self):
        output_path = os.path.join(self.temp_dir.name, "resource_id.o")
        for compile_command, cce_arch in (
            ([], "dav-c220-cube"),
            ([None], "dav-c220-cube"),
            (["fake-ccec"], ""),
        ):
            with self.subTest(compile_command=compile_command, cce_arch=cce_arch):
                with self.assertRaisesRegex(
                    MODULE.ResourceIdError, "compile command and cce arch"
                ):
                    MODULE.generate_resource_id_object(
                        self.object_paths,
                        output_path,
                        compile_command,
                        ["-DTEST_OPTION"],
                        cce_arch,
                    )


if __name__ == "__main__":
    unittest.main()
