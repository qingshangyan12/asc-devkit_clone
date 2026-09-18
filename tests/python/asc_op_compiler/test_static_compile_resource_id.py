#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import hashlib
import os
from pathlib import Path
import re
import sys
import tempfile
import unittest
from unittest import mock

TOP_PATH = os.path.join(os.path.dirname(os.path.realpath(__file__)), "../../../")
FRAMEWORK_PATH = os.path.join(TOP_PATH, "tools/build/")
if FRAMEWORK_PATH not in sys.path:
    sys.path.insert(0, FRAMEWORK_PATH)

from asc_op_compile_base.asc_op_compiler import static_compile_resource_id as MODULE
from asc_op_compile_base.asc_op_compiler.ascendc_common_utility import (
    CommonUtility,
    CompileStage,
)


class TestStaticCompileResourceId(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp_dir.cleanup)
        self.object_paths = []
        for index, content in enumerate((b"\x7fELFfirst", b"\x7fELFsecond")):
            path = os.path.join(self.temp_dir.name, f"kernel{index}.o")
            Path(path).write_bytes(content)
            self.object_paths.append(path)

    def test_resource_id_xors_object_digests(self):
        expected = bytearray(hashlib.sha256().digest_size)
        for path in self.object_paths:
            for index, value in enumerate(
                hashlib.sha256(Path(path).read_bytes()).digest()
            ):
                expected[index] ^= value
        self.assertEqual(
            MODULE.calculate_resource_id(self.object_paths), expected.hex()
        )

    def test_cancelled_out_digests_are_rejected(self):
        # XOR is self-inverse: identical contents cancel and would otherwise
        # produce 64 zeros, which is a well-formed but meaningless identity.
        twin = os.path.join(self.temp_dir.name, "twin.o")
        Path(twin).write_bytes(Path(self.object_paths[0]).read_bytes())
        for paths in ([self.object_paths[0]] * 2, [self.object_paths[0], twin]):
            with self.subTest(paths=paths):
                with self.assertRaisesRegex(
                    MODULE.ResourceIdError, "collapsed to zero"
                ):
                    MODULE.calculate_resource_id(paths)

    def test_resource_id_depends_on_content_but_not_link_order(self):
        baseline = MODULE.calculate_resource_id(self.object_paths)
        self.assertEqual(
            MODULE.calculate_resource_id(list(reversed(self.object_paths))), baseline
        )
        Path(self.object_paths[0]).write_bytes(b"\x7fELFfirst-rebuilt")
        self.assertNotEqual(MODULE.calculate_resource_id(self.object_paths), baseline)

    def test_resource_id_does_not_saturate_as_objects_are_added(self):
        # XOR keeps each output bit balanced as more object digests are merged.
        paths = list(self.object_paths)
        for index in range(32):
            path = os.path.join(self.temp_dir.name, f"extra{index}.o")
            Path(path).write_bytes(b"\x7fELF" + str(index).encode("ascii"))
            paths.append(path)
        resource_id = MODULE.calculate_resource_id(paths)
        self.assertEqual(len(resource_id), MODULE._RESOURCE_ID_VALUE_SIZE)
        self.assertLess(resource_id.count("f"), 32)

    def test_metadata_layout_matches_kernel_utils_macros_header(self):
        header = Path(
            os.path.join(TOP_PATH, "impl/basic_api/utils/kernel_utils_macros.h")
        ).read_text(encoding="utf-8")
        self.assertIn(
            f"B_TYPE_SPECIALIZATION_RESOURCE_ID = {MODULE._RESOURCE_ID_TYPE}",
            header,
        )
        base_tlv = re.search(r"struct BaseTlv \{(.*?)\}", header, re.S)
        self.assertIsNotNone(base_tlv)
        self.assertEqual(
            re.findall(r"unsigned short (\w+);", base_tlv.group(1)), ["type", "len"]
        )
        entry = re.search(r"struct BinaryMetaResourceId \{(.*?)\}", header, re.S)
        self.assertIsNotNone(entry)
        self.assertIn("BaseTlv head;", entry.group(1))
        self.assertIn(f"char value[{MODULE._RESOURCE_ID_VALUE_SIZE}];", entry.group(1))

    def test_adds_section_when_absent(self):
        object_path = os.path.join(self.temp_dir.name, "kernel.o")
        Path(object_path).write_bytes(b"\x7fELFobject")
        resource_id = "a" * MODULE._RESOURCE_ID_VALUE_SIZE
        commands, stages = [], []
        section = {"data": None}

        def fake_dump(obj_path, dump_path):
            return section["data"]

        def fake_run_cmd_inner(cmds, stage, compile_log_path=None):
            commands.append(cmds)
            stages.append(stage)
            payload = cmds[cmds.index("--add-section") + 1].split("=", 1)[1]
            section["data"] = Path(payload).read_bytes()

        with mock.patch.object(MODULE, "_dump_meta_section", side_effect=fake_dump):
            with mock.patch.object(
                CommonUtility, "run_cmd_inner", side_effect=fake_run_cmd_inner
            ):
                MODULE.attach_resource_id_entry(object_path, resource_id, "compile.log")

        # Nothing to preserve, so the section is created outright.
        self.assertEqual(len(commands), 1)
        self.assertIn("--add-section", commands[0])
        self.assertNotIn("--update-section", commands[0])
        self.assertEqual(stages, [CompileStage.SPECIALIZATION])
        expected = (
            MODULE._RESOURCE_ID_TYPE.to_bytes(2, "little")
            + MODULE._RESOURCE_ID_VALUE_SIZE.to_bytes(2, "little")
            + resource_id.encode("ascii")
        )
        self.assertEqual(section["data"], expected)
        self.assertEqual(len(expected), 4 + MODULE._RESOURCE_ID_VALUE_SIZE)

    def test_updates_section_and_keeps_existing_entries(self):
        object_path = os.path.join(self.temp_dir.name, "kernel.o")
        Path(object_path).write_bytes(b"\x7fELFobject")
        resource_id = "b" * MODULE._RESOURCE_ID_VALUE_SIZE
        feature = (
            (4).to_bytes(2, "little") + (4).to_bytes(2, "little") + b"\x03\x00\x00\x00"
        )
        section = {"data": feature}
        commands = []

        def fake_dump(obj_path, dump_path):
            return section["data"]

        def fake_run_cmd_inner(cmds, stage, compile_log_path=None):
            commands.append(cmds)
            payload = cmds[cmds.index("--update-section") + 1].split("=", 1)[1]
            section["data"] = Path(payload).read_bytes()

        with mock.patch.object(MODULE, "_dump_meta_section", side_effect=fake_dump):
            with mock.patch.object(
                CommonUtility, "run_cmd_inner", side_effect=fake_run_cmd_inner
            ):
                MODULE.attach_resource_id_entry(object_path, resource_id)

        # One call swaps the whole section, so it is never missing in between.
        self.assertEqual(len(commands), 1)
        self.assertIn("--update-section", commands[0])
        self.assertNotIn("--remove-section", commands[0])
        self.assertTrue(section["data"].startswith(feature))
        self.assertTrue(section["data"].endswith(resource_id.encode("ascii")))

    def test_attach_rejects_wrong_length_missing_tool_and_missing_result(self):
        object_path = os.path.join(self.temp_dir.name, "kernel.o")
        Path(object_path).write_bytes(b"\x7fELFobject")
        with self.assertRaisesRegex(MODULE.ResourceIdError, "must be 64 characters"):
            MODULE.attach_resource_id_entry(object_path, "short")
        resource_id = "c" * MODULE._RESOURCE_ID_VALUE_SIZE
        # A missing objcopy must surface as a ResourceIdError, not a bare OSError.
        with mock.patch.object(
            MODULE.subprocess, "run", side_effect=FileNotFoundError(2, "no such file")
        ):
            with self.assertRaisesRegex(MODULE.ResourceIdError, "failed to execute"):
                MODULE.attach_resource_id_entry(object_path, resource_id)
        # objcopy reporting success while writing nothing must still be caught.
        with mock.patch.object(MODULE, "_dump_meta_section", return_value=None):
            with mock.patch.object(
                CommonUtility, "run_cmd_inner", side_effect=lambda *a, **k: None
            ):
                with self.assertRaisesRegex(
                    MODULE.ResourceIdError, "failed to attach resource id"
                ):
                    MODULE.attach_resource_id_entry(object_path, resource_id)


if __name__ == "__main__":
    unittest.main()
