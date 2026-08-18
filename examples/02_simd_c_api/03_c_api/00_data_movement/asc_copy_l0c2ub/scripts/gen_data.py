#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os

import numpy as np


M = 16
K = 16
N = 64


def pack_signed_int4(values):
    values = np.rint(values).astype(np.int8)
    packed_nibbles = values.astype(np.uint8) & np.uint8(0x0F)
    return packed_nibbles[:, 0::2] | (packed_nibbles[:, 1::2] << np.uint8(4))


def main():
    a = np.eye(M, K, dtype=np.float16)
    b = np.full((K, N), 3.0, dtype=np.float16)
    golden = pack_signed_int4(a.astype(np.float32) @ b.astype(np.float32))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)
    a.tofile("input/x1_gm.bin")
    b.tofile("input/x2_gm.bin")
    golden.tofile("output/golden.bin")


if __name__ == "__main__":
    main()
