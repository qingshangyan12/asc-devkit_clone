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

"""CPU tests for the Add demo's offline dump validation."""
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest

DEMO = Path(__file__).resolve().parents[1] / "examples" / "demo_add.py"
spec = importlib.util.spec_from_file_location("demo_add", DEMO)
demo = importlib.util.module_from_spec(spec)
spec.loader.exec_module(demo)


class DumpValidationTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for stage, name, values in [
            ("input", "add_normal_x", [0., 1., 2., 3.]),
            ("input", "add_normal_y", [1., 1., 1., 1.]),
            ("output", "add_normal_ret", [1., 2., 3., 4.]),
            ("input", "add_fault_x", [0., 1., 2., 3.]),
            ("input", "add_fault_y", [1., 1., 1., 1.]),
        ]:
            stem = f"{stage}_{name}_dev0_20261016"
            (self.root / f"{stem}.bin").write_bytes(struct.pack("<4f", *values))
            meta = dict(name=name, stage=stage, shape=[4], dtype="torch.float32",
                        itemsize=4, numel=4, nbytes=16, device="npu:0",
                        data_ptr="0x200000004000" if name.endswith("_y") else "0x200000000000",
                        bin_file=f"{stem}.bin")
            (self.root / f"{stem}.json").write_text(json.dumps(meta))
        (self.root / "fault.json").write_text(json.dumps({"output_ptr": "0x100000000000"}))
        self.args = self.root / "add_dev0_task1_time_args.bin"
        self.args.write_bytes(struct.pack("<QQQ", 0x200000000000, 0x200000004000, 0x100000000000))
        self.info = self.root / "add_dev0_task1_time_info.txt"
        self.info.write_text("kernel_name : add\nerror_code : 0x123\nargs_bytes : 24\nargs_dumped : yes\n")

    def check(self):
        with contextlib.redirect_stdout(io.StringIO()):
            return demo.check_dump_products(self.root)

    def test_valid_run_accepts_dynamic_args_size(self):
        self.assertTrue(self.check())
        self.args.write_bytes(self.args.read_bytes() + bytes(64))
        self.info.write_text(self.info.read_text().replace("24", "88"))
        self.assertTrue(self.check())

    def test_rejects_args_length_mismatch(self):
        self.args.write_bytes(b"short")
        self.assertFalse(self.check())

    def test_rejects_zero_error(self):
        self.info.write_text(self.info.read_text().replace("0x123", "0x0"))
        self.assertFalse(self.check())

    def test_rejects_unrelated_kernel(self):
        self.info.write_text(self.info.read_text().replace(" : add", " : mul"))
        self.assertFalse(self.check())

    def test_rejects_missing_fault_pointer_in_args(self):
        self.args.write_bytes(bytes(24))
        self.assertFalse(self.check())

    def test_rejects_missing_input_pointers_in_args(self):
        self.args.write_bytes(struct.pack("<QQQ", 0, 0, 0x100000000000))
        self.assertFalse(self.check())

    def test_rejects_wrong_add_result(self):
        output = next(self.root.glob("output_*.bin"))
        output.write_bytes(bytes(16))
        self.assertFalse(self.check())

    def test_rejects_missing_metadata(self):
        next(self.root.glob("output_*.json")).unlink()
        self.assertFalse(self.check())

    def test_rejects_missing_fault_inputs(self):
        for path in self.root.glob("input_add_fault_x*"):
            path.unlink()
        self.assertFalse(self.check())

    def test_rejects_empty_directory(self):
        with tempfile.TemporaryDirectory() as directory:
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertFalse(demo.check_dump_products(directory))


if __name__ == "__main__":
    unittest.main()
