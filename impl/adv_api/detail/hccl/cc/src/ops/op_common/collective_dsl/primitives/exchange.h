// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#pragma once
#include "program.h"
#include <functional>

namespace asc { namespace ir {
using Slices = std::vector<Slice>;
using MeshSlices = std::map<Rank, std::vector<Transfer>>;
struct NhrSlices {
    Op direction; // Read 或 Write；不让 engine 擅自更改两端的匹配协议
    // block 是初始数据块的 rank 标记；holder 是此时存放该块的 rank。
    // 回调只在构图时生成地址列表，绝不作为设备端函数运行。
    // Mesh→NHR 时一个 block 可包括整个 Mesh group 的 owner slices。
    std::function<Slices(Rank holder, Rank block)> addresses;
};
Exchange mesh(Rank p, const Ranks& ranks, const MeshSlices& slices);
Exchange nhr(Rank p, const Ranks& ranks, const NhrSlices& slices);
} }
