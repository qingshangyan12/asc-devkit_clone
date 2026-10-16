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

"""Minimal ctypes bridge to the locally built AddCustom operator."""

import ctypes as C
import os

# A non-null, aligned, deliberately invalid device output address, for this demo only.
FAULT_OUTPUT_PTR = 0x100000000000


class AddExecutionError(RuntimeError):
    """An error during operator launch or stream synchronization."""


class AclnnAdd:
    def __init__(self):
        self.acl = C.CDLL("libascendcl.so", mode=C.RTLD_GLOBAL)
        self.op = C.CDLL(os.environ.get("NPUOPS_ADD_LIB", "libcust_opapi.so"), mode=C.RTLD_GLOBAL)
        ptr, u64, i64 = C.c_void_p, C.c_uint64, C.c_int64
        dims = C.POINTER(i64)
        self._bind(self.op, "aclCreateTensor", ptr, [dims, u64, C.c_int, dims, i64, C.c_int, dims, u64, ptr])
        self._bind(self.op, "aclDestroyTensor", C.c_int, [ptr])
        self._bind(self.op, "aclnnAddCustomGetWorkspaceSize", C.c_int, [ptr, ptr, ptr, C.POINTER(u64), C.POINTER(ptr)])
        self._bind(self.op, "aclnnAddCustom", C.c_int, [ptr, u64, ptr, ptr])
        self._bind(self.acl, "aclrtSynchronizeStream", C.c_int, [ptr])

    @staticmethod
    def _bind(lib, name, result, args):
        function = getattr(lib, name)
        function.restype = result
        function.argtypes = args

    def __call__(self, x, y, *, fault=False):
        import torch

        for tensor in (x, y):
            if tensor.dtype != torch.float16 or tensor.device.type != "npu" or not tensor.is_contiguous():
                raise ValueError("Add demo expects contiguous float16 NPU tensors")
        if x.numel() != 16384:
            raise ValueError("AddCustom demo requires 16384 elements")
        if x.shape != y.shape or x.device != y.device:
            raise ValueError("Add demo expects identical input shapes and devices")
        # Keep inputs, output and workspace alive until the stream has synchronized.
        out = torch.empty(x.shape, dtype=x.dtype, device=x.device)
        shape = (C.c_int64 * x.ndim)(*x.shape)
        strides = (C.c_int64 * x.ndim)(*x.stride())
        descriptors = []
        try:
            for address in (x.data_ptr(), y.data_ptr(), FAULT_OUTPUT_PTR if fault else out.data_ptr()):
                # ACL_FLOAT16=1, ACL_FORMAT_ND=2. These descriptors borrow the tensor memory.
                descriptor = self.op.aclCreateTensor(shape, x.ndim, 1, strides, 0, 2, shape, x.ndim, address)
                if not descriptor:
                    raise RuntimeError("aclCreateTensor failed")
                descriptors.append(descriptor)
            size, executor = C.c_uint64(), C.c_void_p()
            print("[ADD] prepare workspace/tiling", flush=True)
            ret = self.op.aclnnAddCustomGetWorkspaceSize(*descriptors, C.byref(size), C.byref(executor))
            if ret:
                raise RuntimeError(f"aclnnAddCustomGetWorkspaceSize failed: {ret}")
            print(f"[ADD] workspace bytes: {size.value}", flush=True)
            workspace = torch.empty(size.value, dtype=torch.uint8, device=x.device) if size.value else None
            stream = torch.npu.current_stream(x.device).npu_stream
            print("[ADD] launch AddCustom", flush=True)
            ret = self.op.aclnnAddCustom(
                workspace.data_ptr() if workspace is not None else None, size.value, executor, stream
            )
            if ret:
                raise AddExecutionError(f"aclnnAddCustom failed: {ret}")
            print("[ADD] synchronize stream", flush=True)
            ret = self.acl.aclrtSynchronizeStream(stream)
            if ret:
                raise AddExecutionError(f"aclrtSynchronizeStream failed: {ret}")
            if fault:
                raise RuntimeError("invalid output address did not trigger an Add exception")
            return out
        finally:
            # Destroy host-side descriptors only; their device memory belongs to PyTorch.
            for descriptor in descriptors:
                self.op.aclDestroyTensor(descriptor)
