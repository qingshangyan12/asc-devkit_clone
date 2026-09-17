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

"""IFA（incre_flash_attention + meta）Exception Dump Python model：固定 seed 构造输入，
使能能力并登记输入，按场景注入故障执行 kernel（scene1/scene2/baseline）。"""
import torch
import torch_npu  # noqa: F401
import ascend_ops  # noqa: F401

from op_exception_dump import clear_inputs, enable_exception_dump, register_input

KERNEL_NAME = "incre_flash_attention"

# 8 输入 ptrIndex 布局表（0-based，与 IFA LAUNCH_INCRE_FA 宏形参序一致）。
# 新算子接入时必须按目标 kernel launch 形参序重新校准，不得复用本表
# （校准方法见 README"新算子接入指南"）。
INPUT_PTR_INDEX = {
    "query":              0,
    "key_cache":          1,
    "value_cache":        2,
    "actual_seq_kvlen":   6,
    "block_table":       14,
    "dequant_scale_key": 17,
    "dequant_scale_value": 19,
    "metadata":          28,
}


def build_and_register():
    """构造固定 seed 输入，使能 Exception Dump 并登记全部 8 个输入。

    Returns:
        (tensors, fa_param)：输入 tensor 字典与主算子实参
    """
    torch.npu.set_device(0)
    torch.manual_seed(42)

    B, QH, KVH, QS, BS, D, KV_SEQ = 18, 64, 1, 1, 128, 128, 8192
    MAXB = KV_SEQ // BS + 1
    BLOCK_NUM = B * MAXB
    SCALE = 1.0 / (D ** 0.5)

    q = torch.randn(B, QH, QS, D, dtype=torch.bfloat16).npu()
    key_cache = torch.randint(0, 100, (BLOCK_NUM, KVH, D // 32, BS, 32), dtype=torch.int8).npu()
    value_cache = torch.randint(0, 100, (BLOCK_NUM, KVH, D // 32, BS, 32), dtype=torch.int8).npu()
    block_table = torch.arange(B * MAXB, dtype=torch.int32).view(B, MAXB).npu()
    actual_seq_kvlen = torch.tensor([KV_SEQ] * B, dtype=torch.int64).npu()
    deq_k = torch.randn(KVH, 1, D, dtype=torch.bfloat16).npu()
    deq_v = torch.randn(KVH, 1, D, dtype=torch.bfloat16).npu()

    # Step 1: 使能 + 注册算子名（引导链路，算子零改动）
    enable_exception_dump(KERNEL_NAME)

    # Step 2: 登记输入 handle（每轮调用前清理防残留）
    clear_inputs()
    tensors = {
        "query": q, "key_cache": key_cache, "value_cache": value_cache,
        "actual_seq_kvlen": actual_seq_kvlen, "block_table": block_table,
        "dequant_scale_key": deq_k, "dequant_scale_value": deq_v,
    }
    for name, t in tensors.items():
        register_input(name, t, INPUT_PTR_INDEX[name])
    print("[INFO] Input info set: %d inputs registered" % len(tensors))

    # Step 3: meta 前置算子先生成 metadata（运行时输出），生成后补登记
    meta_param = {
        "batch_size": B, "query_seq_size": QS, "query_head_num": QH, "key_head_num": KVH,
        "head_dim": D, "block_size": BS, "max_block_num_per_batch": MAXB,
        "actual_seq_lengths_kv": actual_seq_kvlen, "layout_query": "BNSD",
    }
    metadata = torch.ops.ascend_ops.npu_fused_infer_attention_score_metadata(**meta_param)
    register_input("metadata", metadata, INPUT_PTR_INDEX["metadata"])
    print("[INFO] data_ptr(metadata) = 0x%016x, usedCoreNum=%d"
          % (metadata.data_ptr(), int(metadata.cpu().numpy()[0])))

    fa_param = {
        "query": q, "key": key_cache, "value": value_cache,
        "actual_seq_kvlen": actual_seq_kvlen, "block_table": block_table,
        "dequant_scale_key": deq_k, "dequant_scale_value": deq_v,
        "num_query_heads": QH, "num_key_value_heads": KVH,
        "softmax_scale": SCALE, "block_size": BS,
        "input_layout": "BNSD", "sparse_mode": 0, "inner_precise": 1,
        "key_quant_mode": 0, "value_quant_mode": 0,
        "metadata": metadata,
    }
    return tensors, fa_param


def launch_and_check(scene, tensors, fa_param):
    """按场景注入并执行 kernel，异常视为预期路径。

    Returns:
        True 表示发生异常且属注入场景（预期失败路径）
    """
    if scene == "scene1":
        tensors["block_table"][0, 0] = 0x7FFFFFFF
        print("[INJECT] scene1: block_table[0,0] = 0x7FFFFFFF (块表损坏)")
    elif scene == "scene2":
        tensors["actual_seq_kvlen"][0] = 0x7FFFFFFF
        print("[INJECT] scene2: actual_seq_kvlen[0] = 0x7FFFFFFF (序列长度损坏)")
    else:
        print("[INJECT] no inject (正常基线，验证回调不误触发)")
    print("[INFO] launching incre_flash_attention kernel ...")
    try:
        out = torch.ops.ascend_ops.npu_fused_infer_attention_score(**fa_param)
        torch.npu.synchronize()
        print("[INFO] kernel 正常结束, output shape = %s" % str(tuple(out[0].shape)))
        return False
    except RuntimeError as e:
        print("[CAUGHT] kernel 执行异常(预期): %s" % str(e)[-80:])
        try:
            torch.npu.synchronize()
        except Exception:
            pass
        return scene != "baseline"


def run(scenes):
    """依次执行给定场景，返回是否存在预期的注入异常。"""
    tensors, fa_param = build_and_register()
    expected_fail = False
    for scene in scenes:
        if launch_and_check(scene, tensors, fa_param):
            expected_fail = True
    return expected_fail
