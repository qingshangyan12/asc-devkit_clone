/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description: Conv2DV2 op def
 */

#include "register/op_def_registry.h"

namespace ops {
static const std::vector<ge::DataType> conv2DV2XDtype = {
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,
    ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,
    ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
};

static const std::vector<ge::DataType> conv2DV2FilterDtype = {
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
};

static const std::vector<ge::DataType> conv2DV2BiasDtype = {
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,
    ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,
    ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,
    ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,
    ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,
    ge::DT_INT32,         ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT32,         ge::DT_INT32,
    ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,
    ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,         ge::DT_INT32,
};

static const std::vector<ge::DataType> conv2DV2OffsetWDtype = {
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
};

static const std::vector<ge::DataType> conv2DV2YDtype = {
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_INT16,         ge::DT_INT16,
    ge::DT_INT16,         ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,
    ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT8,          ge::DT_INT8,
    ge::DT_INT8,          ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,
    ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,          ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_FLOAT16,       ge::DT_INT16,
    ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT8,          ge::DT_INT8,          ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,       ge::DT_FLOAT16,
    ge::DT_FLOAT16,       ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT16,         ge::DT_INT8,
    ge::DT_INT8,          ge::DT_INT8,          ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
};

static const std::vector<ge::Format> conv2DV2XFormat = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
};

static const std::vector<ge::Format> conv2DV2FilterFormat = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
};

static const std::vector<ge::Format> conv2DV2BiasFormat = {
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
};

static const std::vector<ge::Format> conv2DV2OffsetWFormat = {
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
    ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND,
};

static const std::vector<ge::Format> conv2DV2YFormat = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW,    ge::FORMAT_NHWC,    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW,    ge::FORMAT_NHWC,
};

class Conv2DV2 : public OpDef {
public:
    explicit Conv2DV2(const char* name) : OpDef(name)
    {
        (void)(name);
        this->Input("x")
            .ParamType(REQUIRED)
            .DataType(conv2DV2XDtype)
            .Format(conv2DV2XFormat);
        this->Input("filter")
            .ParamType(REQUIRED)
            .DataType(conv2DV2FilterDtype)
            .Format(conv2DV2FilterFormat);
        this->Input("bias")
            .ParamType(OPTIONAL)
            .DataType(conv2DV2BiasDtype)
            .Format(conv2DV2BiasFormat);
        this->Input("offset_w")
            .ParamType(OPTIONAL)
            .DataType(conv2DV2OffsetWDtype)
            .Format(conv2DV2OffsetWFormat);
        this->Output("y")
            .ParamType(REQUIRED)
            .DataType(conv2DV2YDtype)
            .Format(conv2DV2YFormat);

        this->Attr("strides").AttrType(REQUIRED).ListInt();
        this->Attr("pads").AttrType(OPTIONAL).ListInt({0, 0, 0, 0});
        this->Attr("dilations").AttrType(OPTIONAL).ListInt({1, 1, 1, 1});
        this->Attr("groups").AttrType(OPTIONAL).Int(1);
        this->Attr("data_format").AttrType(OPTIONAL).String("NCHW");
        this->Attr("offset_x").AttrType(OPTIONAL).Int(0);
        this->Attr("pad_mode").AttrType(OPTIONAL).String("SPECIFIC");
        this->Attr("enable_hf32").AttrType(OPTIONAL).Bool(false);

        OpAICoreConfig gzhCfg;
        gzhCfg.DynamicCompileStaticFlag(false)
            .DynamicFormatFlag(false)
            .DynamicRankSupportFlag(true)
            .DynamicShapeSupportFlag(false)
            .NeedCheckSupportFlag(false)
            .ExtendCfgInfo("keepOpInfo.value", "all");

        this->AICore().AddConfig("kirindev0001", gzhCfg);
    }
};

OP_ADD_BUILTIN(Conv2DV2);
} // ops
