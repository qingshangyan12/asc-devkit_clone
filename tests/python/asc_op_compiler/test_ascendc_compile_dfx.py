#!/usr/bin/python3
# coding=utf-8
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

from pathlib import Path
import sys
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / "tools/build"))

from asc_op_compile_base.asc_op_compiler import ascendc_compile_dfx
from asc_op_compile_base.adapter import ascendc_compile_dfx as legacy_compile_dfx


class TestDFXTensorShape(unittest.TestCase):
    module = ascendc_compile_dfx

    def setUp(self):
        self.generator = self.module.DFXSectionGenerator()
        with mock.patch.object(self.generator, "is_support_dfx", return_value=True):
            self.generator.dfx_info_reset(None)

    def check_shape(self, shape, dtype, expected_bytes, kind=None):
        name = f"tensor_{self.generator.index}"
        parameter = self.module.DFXArgInfo(name, kind or self.module.DFXParamType.INPUT)
        self.generator.insert_param(parameter)
        self.generator._set_level_1_input_output_dfx_info({"param_name": name, "shape": shape, "dtype": dtype})
        self.assertEqual(parameter.param_size, expected_bytes)
        self.assertEqual(parameter.param_dim, len(shape))
        self.assertEqual(parameter.param_shape, shape)

    def test_rank_zero_is_one_element(self):
        for shape in ([], ()):
            for dtype, size in (("float16", 2), ("bfloat16", 2), ("float32", 4), ("int64", 8)):
                for kind in (self.module.DFXParamType.INPUT, self.module.DFXParamType.OUTPUT):
                    with self.subTest(shape=shape, dtype=dtype, kind=kind):
                        self.check_shape(shape, dtype, size, kind)

    def test_non_scalar_shapes_keep_their_element_count(self):
        for shape, size in (([1], 4), ([2, 3], 24), ([3, 0], 0), ([0, 3], 0)):
            with self.subTest(shape=shape):
                self.check_shape(shape, "float32", size)


class TestLegacyDFXTensorShape(TestDFXTensorShape):
    module = legacy_compile_dfx


if __name__ == "__main__":
    unittest.main()
