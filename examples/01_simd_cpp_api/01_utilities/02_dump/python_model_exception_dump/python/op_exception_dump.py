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

"""Exception Dump 能力的 Python API：enable_exception_dump / register_input / clear_inputs。

内部经 ctypes 加载回调库（默认 <样例>/runtime/exception_dump_public.so，可用 lib_path
参数或 EXC_DUMP_LIB 环境变量覆盖），库不存在时先执行 scripts/build_callback.sh。"""
import ctypes
import os

_LIB = None


def _default_lib_path():
    return os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        "..", "runtime", "exception_dump_public.so")


def _load_lib(lib_path=None):
    """惰性加载回调库并声明 argtypes（重复调用幂等）。"""
    global _LIB
    if _LIB is not None:
        return _LIB
    path = lib_path or os.environ.get("EXC_DUMP_LIB") or _default_lib_path()
    if not os.path.isfile(path):
        raise FileNotFoundError(
            "exception dump 回调库不存在: %s\n"
            "请先执行 scripts/build_callback.sh 编译回调库，"
            "或通过 lib_path 参数 / EXC_DUMP_LIB 环境变量指定路径。" % path)
    lib = ctypes.CDLL(path)
    try:
        lib.AddInputInfo.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_size_t,
                                     ctypes.c_char_p, ctypes.c_char_p]
        lib.AddInputInfo.restype = ctypes.c_int
        lib.ClearInputInfo.argtypes = []
        lib.ClearInputInfo.restype = None
        lib.RegisterPublicExceptionDump.argtypes = [ctypes.c_char_p]
        lib.RegisterPublicExceptionDump.restype = ctypes.c_int
    except AttributeError as e:
        raise RuntimeError(
            "回调库 %s 缺少 C ABI 符号（%s）；请重新执行 scripts/build_callback.sh "
            "编译与本样例配套的回调库。" % (path, e)) from e
    _LIB = lib
    return lib


def enable_exception_dump(kernel_name, lib_path=None):
    """使能 Exception Dump 并注册目标算子名（算子零改动）。

    经回调库调用 CANN 公开接口 aclrtExceptionInfoCallbackRegister 注册异常回调，
    并按 kernel_name 过滤异常任务；<<<>>> 模板 kernel 直接传函数名本体即可
    （回调内做 mangled 名子串匹配）。首次匹配到目标 kernel 异常时，回调内自动
    挂载 per-binary 回调，当前异常与后续异常均会保存现场。

    Args:
        kernel_name: 目标 kernel 名（str），如 "incre_flash_attention"
        lib_path:    回调库路径，默认 <样例>/runtime/exception_dump_public.so

    Returns:
        0 成功；非 0 为注册失败（CANN 错误码）。
    """
    lib = _load_lib(lib_path)
    ret = lib.RegisterPublicExceptionDump(kernel_name.encode())
    if ret != 0:
        raise RuntimeError(
            "RegisterPublicExceptionDump('%s') 失败，返回 %d（CANN 错误码）；"
            "请检查 CANN 版本是否提供 aclrtExceptionInfoCallbackRegister。"
            % (kernel_name, ret))
    return ret


def register_input(name, tensor, ptr_index, lib_path=None):
    """登记一个输入 tensor 的 dump handle。

    从 tensor 自动读取字节数、shape 与 dtype；meta 类运行时生成的输入（如
    前置算子的输出）必须在其生成之后调用本接口补登记。

    Args:
        name:      输入名（落盘为 <name>.bin）
        tensor:    torch tensor（NPU 侧）
        ptr_index: 该输入 device 指针在 kernel args 区中的槽位（0-based，
                   严格按目标 kernel launch 形参顺序校准）

    Returns:
        0 成功；负值为登记失败（超出容量 64）。
    """
    lib = _load_lib(lib_path)
    size = tensor.numel() * tensor.element_size()
    return lib.AddInputInfo(name.encode(), ptr_index, size,
                            str(tuple(tensor.shape)).encode(),
                            str(tensor.dtype).encode())


def clear_inputs(lib_path=None):
    """清空全部输入登记。开始新一次 model 调用前调用，防止残留上一轮描述。"""
    lib = _load_lib(lib_path)
    lib.ClearInputInfo()
