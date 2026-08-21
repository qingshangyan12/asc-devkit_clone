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


import sys
import argparse
import numpy as np


def verify_result(output, golden):
    output = np.fromfile(output, dtype=np.uint64).reshape(-1)
    golden = np.fromfile(golden, dtype=np.uint64).reshape(-1)
    if output.size != golden.size:
        print(f"size mismatch: output={output.size}, golden={golden.size}")
        return False
    return bool(np.array_equal(output, golden))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("output", type=str)
    parser.add_argument("golden", type=str)
    args = parser.parse_args()
    try:
        res = verify_result(args.output, args.golden)
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)