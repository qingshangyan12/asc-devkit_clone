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

"""ctypes 绑定：加载 libxpuops_exception_dump.so"""

import ctypes
import os

_xpuops_lib = None

# 工程内 lib/ 目录（固定候选）
# XPUOPS_DUMP_LIB_DIR 在 _load_lib 内动态读取（import 之后设置同样生效）
_PROJECT_LIB_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "lib")


def _load_lib():
    # 候选 so 路径：环境变量 > 工程内 lib/
    for d in [os.environ.get("XPUOPS_DUMP_LIB_DIR", ""), _PROJECT_LIB_DIR]:
        if not d:
            continue
        path = os.path.join(d, "libxpuops_exception_dump.so")
        if os.path.exists(path):
            return ctypes.CDLL(path)
    raise FileNotFoundError("libxpuops_exception_dump.so not found, set XPUOPS_DUMP_LIB_DIR or build the project first")


def init(kernels=None):
    """C API xpuopsExceptionDumpInit 的 Python 封装"""
    global _xpuops_lib
    if _xpuops_lib is None:
        _xpuops_lib = _load_lib()
        _xpuops_lib.xpuopsExceptionDumpInit.argtypes = [ctypes.POINTER(ctypes.c_char_p), ctypes.c_size_t]
        _xpuops_lib.xpuopsExceptionDumpInit.restype = ctypes.c_int

    if kernels is None or len(kernels) == 0:
        return _xpuops_lib.xpuopsExceptionDumpInit(None, 0)

    arr = (ctypes.c_char_p * len(kernels))()
    arr[:] = [k.encode() for k in kernels]
    return _xpuops_lib.xpuopsExceptionDumpInit(arr, len(kernels))
