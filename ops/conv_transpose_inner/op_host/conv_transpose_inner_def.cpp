/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description: ConvTransposeInner op def
 */

#include "register/op_def_registry.h"

#include <vector>
#include "graph/types.h"

namespace ops {

template<class T, size_t N>
inline std::vector<T> Expand(const T (&p)[N], int rep)
{
    std::vector<T> v;
    v.reserve(N * static_cast<size_t>(rep));
    for (size_t j = 0; j < N; ++j) {
        for (int i = 0; i < rep; ++i) {
            v.push_back(p[j]);
        }
    }
    return v;
}

template<class T, size_t N>
inline std::vector<T> Repeat(const T (&p)[N], int rep)
{
    std::vector<T> v;
    v.reserve(N * static_cast<size_t>(rep));
    for (int i = 0; i < rep; ++i) {
        for (size_t j = 0; j < N; ++j) {
            v.push_back(p[j]);
        }
    }
    return v;
}

static const std::vector<ge::DataType> convTransposeInnerInputSizeDtype(3240, ge::DT_INT64);

static const std::vector<ge::Format> convTransposeInnerInputSizeFormat(3240, ge::FORMAT_ND);

static const ge::DataType convTransposeInnerXDtypeArr[] = {
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN,
};

static const std::vector<ge::DataType> convTransposeInnerXDtype =
    Expand(convTransposeInnerXDtypeArr, 27);

static const ge::Format convTransposeInnerXFormatArr[] = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC,
};

static const std::vector<ge::Format> convTransposeInnerXFormat =
    Repeat(convTransposeInnerXFormatArr, 120);

static const ge::DataType convTransposeInnerFilterDtypeArr[] = {
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
};

static const std::vector<ge::DataType> convTransposeInnerFilterDtype =
    Expand(convTransposeInnerFilterDtypeArr, 27);

static const std::vector<ge::Format> convTransposeInnerFilterFormat(3240, ge::FORMAT_FRACTAL_Z);

static const ge::DataType convTransposeInnerBiasDtypeArr[] = {
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32,
    ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32,
    ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32,
    ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32,
    ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32,
    ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32,
    ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32, ge::DT_INT32,
};

static const std::vector<ge::DataType> convTransposeInnerBiasDtype =
    Expand(convTransposeInnerBiasDtypeArr, 27);

static const std::vector<ge::Format> convTransposeInnerBiasFormat(3240, ge::FORMAT_ND);

static const std::vector<ge::DataType> convTransposeInnerPreQuantDtype(3240, ge::DT_UINT64);

static const std::vector<ge::Format> convTransposeInnerPreQuantFormat(3240, ge::FORMAT_ND);

static const std::vector<ge::DataType> convTransposeInnerPreReluDtype(3240, ge::DT_UINT32);

static const std::vector<ge::Format> convTransposeInnerPreReluFormat(3240, ge::FORMAT_ND);

static const std::vector<ge::DataType> convTransposeInnerPreClipReluDtype(3240, ge::DT_UINT64);

static const std::vector<ge::Format> convTransposeInnerPreClipReluFormat(3240, ge::FORMAT_ND);

static const ge::DataType convTransposeInnerEltwiseDtypeArr[] = {
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT16,
};

static const std::vector<ge::DataType> convTransposeInnerEltwiseDtype =
    Expand(convTransposeInnerEltwiseDtypeArr, 27);

static const ge::Format convTransposeInnerEltwiseFormatArr[] = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NCHW,
    ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW,
    ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC,
    ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC,
};

static const std::vector<ge::Format> convTransposeInnerEltwiseFormat =
    Repeat(convTransposeInnerEltwiseFormatArr, 120);

static const ge::DataType convTransposeInnerAntiQuantDtypeArr[] = {
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT32,
    ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32,
    ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32,
    ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32,
    ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32,
    ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32,
    ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT64,
    ge::DT_UINT64, ge::DT_UINT64, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32,
    ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32,
    ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32, ge::DT_UINT32,
    ge::DT_UINT32,
};

static const std::vector<ge::DataType> convTransposeInnerAntiQuantDtype =
    Expand(convTransposeInnerAntiQuantDtypeArr, 27);

static const std::vector<ge::Format> convTransposeInnerAntiQuantFormat(3240, ge::FORMAT_ND);

static const std::vector<ge::DataType> convTransposeInnerPostQuantDtype(3240, ge::DT_UINT64);

static const std::vector<ge::Format> convTransposeInnerPostQuantFormat(3240, ge::FORMAT_ND);

static const std::vector<ge::DataType> convTransposeInnerPostReluDtype(3240, ge::DT_UINT32);

static const std::vector<ge::Format> convTransposeInnerPostReluFormat(3240, ge::FORMAT_ND);

static const std::vector<ge::DataType> convTransposeInnerPostClipReluDtype(3240, ge::DT_UINT64);

static const std::vector<ge::Format> convTransposeInnerPostClipReluFormat(3240, ge::FORMAT_ND);

static const ge::DataType convTransposeInnerPaddingDtypeArr[] = {
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8,
    ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT8,
    ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT16, ge::DT_INT16,
    ge::DT_INT16, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT8_E4M3FN,
};

static const std::vector<ge::DataType> convTransposeInnerPaddingDtype =
    Expand(convTransposeInnerPaddingDtypeArr, 27);

static const std::vector<ge::Format> convTransposeInnerPaddingFormat(3240, ge::FORMAT_ND);

static const ge::DataType convTransposeInnerYDtypeArr[] = {
    ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16,
    ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,
    ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16,
    ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,
    ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16,
    ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,
    ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16,
    ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16,
    ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN,
    ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8, ge::DT_FLOAT8_E4M3FN, ge::DT_FLOAT16, ge::DT_INT16, ge::DT_INT8,
    ge::DT_FLOAT8_E4M3FN,
};

static const std::vector<ge::DataType> convTransposeInnerYDtype =
    Expand(convTransposeInnerYDtypeArr, 27);

static const ge::Format convTransposeInnerYFormatArr[] = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
};

static const std::vector<ge::Format> convTransposeInnerYFormat =
    Repeat(convTransposeInnerYFormatArr, 120);

class ConvTransposeInner : public OpDef {
public:
    explicit ConvTransposeInner(const char* name) : OpDef(name)
    {
        (void)(name);
        this->Input("input_size")
            .ParamType(REQUIRED)
            .DataType(convTransposeInnerInputSizeDtype)
            .Format(convTransposeInnerInputSizeFormat);
        this->Input("x")
            .ParamType(REQUIRED)
            .DataType(convTransposeInnerXDtype)
            .Format(convTransposeInnerXFormat);
        this->Input("filter")
            .ParamType(REQUIRED)
            .DataType(convTransposeInnerFilterDtype)
            .Format(convTransposeInnerFilterFormat);
        this->Input("bias")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerBiasDtype)
            .Format(convTransposeInnerBiasFormat);
        this->Input("pre_quant")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerPreQuantDtype)
            .Format(convTransposeInnerPreQuantFormat);
        this->Input("pre_relu")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerPreReluDtype)
            .Format(convTransposeInnerPreReluFormat);
        this->Input("pre_clip_relu")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerPreClipReluDtype)
            .Format(convTransposeInnerPreClipReluFormat);
        this->Input("eltwise")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerEltwiseDtype)
            .Format(convTransposeInnerEltwiseFormat);
        this->Input("anti_quant")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerAntiQuantDtype)
            .Format(convTransposeInnerAntiQuantFormat);
        this->Input("post_quant")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerPostQuantDtype)
            .Format(convTransposeInnerPostQuantFormat);
        this->Input("post_relu")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerPostReluDtype)
            .Format(convTransposeInnerPostReluFormat);
        this->Input("post_clip_relu")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerPostClipReluDtype)
            .Format(convTransposeInnerPostClipReluFormat);
        this->Input("padding")
            .ParamType(OPTIONAL)
            .DataType(convTransposeInnerPaddingDtype)
            .Format(convTransposeInnerPaddingFormat);
        this->Output("y")
            .ParamType(REQUIRED)
            .DataType(convTransposeInnerYDtype)
            .Format(convTransposeInnerYFormat);

        this->Attr("strides").AttrType(REQUIRED).ListInt();
        this->Attr("pads").AttrType(REQUIRED).ListInt();
        this->Attr("dilations").AttrType(OPTIONAL).ListInt({1, 1, 1, 1});
        this->Attr("groups").AttrType(OPTIONAL).Int(1);
        this->Attr("data_format").AttrType(OPTIONAL).String("NCHW");
        this->Attr("output_padding").AttrType(OPTIONAL).ListInt({0, 0, 0, 0});
        this->Attr("offset_x").AttrType(OPTIONAL).Int(0);
        this->Attr("fusion_mode").AttrType(OPTIONAL).Int(0);
        this->Attr("y_quant_mode").AttrType(OPTIONAL).Int(0);
        this->Attr("pad_mode").AttrType(OPTIONAL).String("SPECIFIC");
        this->Attr("pre_dtype").AttrType(OPTIONAL).Int(-1);
        this->Attr("post_dtype").AttrType(OPTIONAL).Int(-1);
        this->Attr("pre_relu_mode").AttrType(OPTIONAL).String("no");
        this->Attr("post_relu_mode").AttrType(OPTIONAL).String("no");
        this->Attr("eltwise_op").AttrType(OPTIONAL).String("no");
        this->Attr("bit_mask").AttrType(OPTIONAL).Int(0);

        OpAICoreConfig gzhCfg;
        gzhCfg.DynamicCompileStaticFlag(false)
            .DynamicFormatFlag(false)
            .DynamicRankSupportFlag(true)
            .DynamicShapeSupportFlag(false)
            .NeedCheckSupportFlag(false)
            .ExtendCfgInfo("opFile.value", "conv_transpose_inner/arch51/conv_transpose_inner_kernel")
            .ExtendCfgInfo("opInterface.value", "conv_transpose_inner")
            .ExtendCfgInfo("keepOpInfo.value", "all")
            .ExtendCfgInfo("binaryConfig.value", "conv_transpose_inner/config/arch51/conv_transpose_inner_binary.json");

        this->AICore().AddConfig("kirindev0001", gzhCfg);
        // this->AICore().AddConfig("kirindev0002", gzhCfg);
    }
};

OP_ADD_BUILTIN(ConvTransposeInner);
} // ops
