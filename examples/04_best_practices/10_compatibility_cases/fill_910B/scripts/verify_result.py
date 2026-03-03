#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import sys
from pathlib import Path
import numpy as np

CUR = Path(__file__).resolve()
REPO_ROOT = next(parent for parent in CUR.parents if (parent / ".git").exists())
sys.path.insert(0, str(REPO_ROOT / "examples"))
from common_verify import verify_result_file

RELATIVE_TOL = 1e-3
ABSOLUTE_TOL = 1e-5
ERROR_TOL = 1e-3
DATA_TYPE = np.float32


def verify_result(output, golden):
    return verify_result_file(output, golden, DATA_TYPE, RELATIVE_TOL, ABSOLUTE_TOL, ERROR_TOL, print_arrays=True)


if __name__ == '__main__':
    try:
        res = verify_result(sys.argv[1], sys.argv[2])
        if not res:
            raise ValueError("[ERROR] result error")
        print("test pass!")
    except Exception as e:
        print(e)
        sys.exit(1)
