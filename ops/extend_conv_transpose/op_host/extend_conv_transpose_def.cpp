/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description: ExtendConvTranspose op def
 */

#include "register/op_def_registry.h"

#include <vector>
#include "graph/types.h"

namespace ops {

static const std::vector<ge::DataType> extendConvTransposeInputSizeDtype = {
    ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64,
    ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64,
    ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64,
    ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64,
    ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64,
    ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64,
    ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64,
    ge::DT_INT64, ge::DT_INT64, ge::DT_INT64, ge::DT_INT64,
};

static const std::vector<ge::DataType> extendConvTransposeXDtype = {
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

static const std::vector<ge::DataType> extendConvTransposeFilterDtype = {
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

static const std::vector<ge::DataType> extendConvTransposeBiasDtype = {
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

static const std::vector<ge::DataType> extendConvTransposeScaleDtype = {
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
};

static const std::vector<ge::DataType> extendConvTransposeYDtype = {
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

static const std::vector<ge::Format> extendConvTransposeInputSizeFormat = {
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

static const std::vector<ge::Format> extendConvTransposeXFormat = {
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

static const std::vector<ge::Format> extendConvTransposeFilterFormat = {
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

static const std::vector<ge::Format> extendConvTransposeBiasFormat = {
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

static const std::vector<ge::Format> extendConvTransposeScaleFormat = {
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

static const std::vector<ge::Format> extendConvTransposeYFormat = {
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

class ExtendConvTranspose : public OpDef {
public:
    explicit ExtendConvTranspose(const char* name) : OpDef(name)
    {
        (void)(name);
        this->Input("input_size")
            .ParamType(REQUIRED)
            .DataType(extendConvTransposeInputSizeDtype)
            .Format(extendConvTransposeInputSizeFormat);
        this->Input("x")
            .ParamType(REQUIRED)
            .DataType(extendConvTransposeXDtype)
            .Format(extendConvTransposeXFormat);
        this->Input("filter")
            .ParamType(REQUIRED)
            .DataType(extendConvTransposeFilterDtype)
            .Format(extendConvTransposeFilterFormat);
        this->Input("bias")
            .ParamType(OPTIONAL)
            .DataType(extendConvTransposeBiasDtype)
            .Format(extendConvTransposeBiasFormat);
        this->Input("scale")
            .ParamType(OPTIONAL)
            .DataType(extendConvTransposeScaleDtype)
            .Format(extendConvTransposeScaleFormat);
        this->Output("y")
            .ParamType(REQUIRED)
            .DataType(extendConvTransposeYDtype)
            .Format(extendConvTransposeYFormat);

        this->Attr("strides").AttrType(REQUIRED).ListInt();
        this->Attr("pads").AttrType(REQUIRED).ListInt();
        this->Attr("dilations").AttrType(OPTIONAL).ListInt({1, 1, 1, 1, 1});
        this->Attr("groups").AttrType(OPTIONAL).Int(1);
        this->Attr("data_format").AttrType(OPTIONAL).String("NDHWC");
        this->Attr("output_padding").AttrType(OPTIONAL).ListInt({0, 0, 0, 0, 0});
        this->Attr("offset_x").AttrType(OPTIONAL).Int(0);
        this->Attr("fusion_mode").AttrType(OPTIONAL).Int(0);
        this->Attr("y_quant_mode").AttrType(OPTIONAL).Int(0);
        this->Attr("pad_mode").AttrType(OPTIONAL).String("SPECIFIC");

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

OP_ADD_BUILTIN(ExtendConvTranspose);
} // ops
