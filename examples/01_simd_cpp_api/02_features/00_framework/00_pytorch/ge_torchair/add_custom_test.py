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

import os
from typing import Any

import torch
import torch_npu
import torchair
from torchair.ge import Tensor
from torch_npu.testing.testcase import TestCase, run_tests


LIB_PATH = os.environ.get("ASCENDC_OPS_LIB", os.path.join(os.path.dirname(__file__), "build/libascendc_ops.so"))
torch.ops.load_library(LIB_PATH)


@torchair.register_fx_node_ge_converter(torch.ops.ascendc_ops.ascendc_add.default)
def convert_ascendc_add(x: Tensor, y: Tensor, z: Tensor = None, meta_outputs: Any = None):
    return torchair.ge.custom_op(
        "AddCustom",
        inputs={
            "x": x,
            "y": y,
        },
        outputs=["z"],
    )


class AddCustomModel(torch.nn.Module):
    def forward(self, x, y):
        return torch.ops.ascendc_ops.ascendc_add(x, y)


class TestGeTorchairAdd(TestCase):
    def test_add_custom_graph_mode(self):
        shape = [8, 2048]
        torch.manual_seed(0)
        x_cpu = torch.rand(shape, device="cpu", dtype=torch.float32)
        y_cpu = torch.rand(shape, device="cpu", dtype=torch.float32)

        model = AddCustomModel().npu()
        config = torchair.CompilerConfig()
        npu_backend = torchair.get_npu_backend(compiler_config=config)
        opt_model = torch.compile(model, fullgraph=True, backend=npu_backend, dynamic=False)

        output = opt_model(x_cpu.npu(), y_cpu.npu()).cpu()
        golden = torch.add(x_cpu, y_cpu)
        torch.testing.assert_close(output, golden, rtol=1e-4, atol=1e-4)


if __name__ == "__main__":
    run_tests()
