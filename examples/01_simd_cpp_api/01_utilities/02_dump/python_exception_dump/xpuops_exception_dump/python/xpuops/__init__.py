# -*- coding: UTF-8 -*-
"""xpuops - NPU 异常自动 dump 与输入/输出 tensor dump 外挂库 Python 接口"""
from .debug import enable_exception_dump, dump_tensor, dump_tensors, run_with_tensor_dump

__all__ = ["enable_exception_dump", "dump_tensor", "dump_tensors", "run_with_tensor_dump"]
