/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#include "register/op_def_registry.h"

namespace ops {
class ExtendConv2D : public OpDef {
public:
    explicit ExtendConv2D(const char* name) : OpDef(name)
    {
    }
};

OP_ADD_BUILTIN(ExtendConv2D);
} // ops
