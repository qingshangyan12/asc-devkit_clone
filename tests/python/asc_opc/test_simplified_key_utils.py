#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import sys
import unittest


FILE_PATH = os.path.dirname(os.path.realpath(__file__))
TOP_PATH = os.path.join(FILE_PATH, "../../../")
FRAMEWORK_PATH = os.path.join(TOP_PATH, "tools/build/")
ASC_OPC_PATH = os.path.join(TOP_PATH, "tools/build/asc_opc/python/asc_opc_tool/")
sys.path.insert(0, FRAMEWORK_PATH)
sys.path.insert(0, ASC_OPC_PATH)

from simplified_key_utils import (
    generate_custom_mode_simplified_key,
    infer_simplified_key_mode,
)


class TestCustomSimplifiedKeyLength(unittest.TestCase):
    def test_mode_inference_rejects_oversized_custom_fragment(self):
        with self.assertRaisesRegex(
            ValueError, "custom simplified_key length must be less than 256 bytes"
        ):
            infer_simplified_key_mode({"simplified_key": "k" * 256}, {})

    def test_custom_fragment_accepts_255_bytes(self):
        fragment = "k" * 255

        simplified_keys = generate_custom_mode_simplified_key(
            "AddCustom", fragment, ["d=0"], ["p=0"]
        )

        self.assertEqual(simplified_keys, [f"AddCustom/d=0,p=0/{fragment}"])

    def test_custom_fragment_rejects_256_bytes(self):
        fragment = "k" * 256

        with self.assertRaisesRegex(
            ValueError, "custom simplified_key length must be less than 256 bytes"
        ):
            generate_custom_mode_simplified_key(
                "AddCustom", fragment, ["d=0"], ["p=0"]
            )

    def test_custom_fragment_uses_utf8_byte_length(self):
        with self.assertRaisesRegex(
            ValueError, "custom simplified_key length must be less than 256 bytes"
        ):
            generate_custom_mode_simplified_key(
                "AddCustom", "中" * 86, ["d=0"], ["p=0"]
            )

    def test_complete_key_accepts_1023_bytes(self):
        fragment = "k"
        public_fields = "/d=0,p=0/"
        op_type = "O" * (1023 - len(public_fields) - len(fragment))

        simplified_keys = generate_custom_mode_simplified_key(
            op_type, fragment, ["d=0"], ["p=0"]
        )

        self.assertEqual(len(simplified_keys[0].encode("utf-8")), 1023)

    def test_complete_key_rejects_1024_bytes(self):
        fragment = "k"
        public_fields = "/d=0,p=0/"
        op_type = "O" * (1024 - len(public_fields) - len(fragment))

        with self.assertRaisesRegex(
            ValueError, "simplified_key length must be less than 1024 bytes"
        ):
            generate_custom_mode_simplified_key(
                op_type, fragment, ["d=0"], ["p=0"]
            )


if __name__ == "__main__":
    unittest.main()
