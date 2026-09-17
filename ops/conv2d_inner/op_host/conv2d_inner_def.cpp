/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description: Conv2DInner op def
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

inline const ge::DataType conv2DInnerXDtypeArr[] = {
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

inline const std::vector<ge::DataType> conv2DInnerXDtype =
    Expand(conv2DInnerXDtypeArr, 27);

inline const ge::Format conv2DInnerXFormatArr[] = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0,
    ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW,
    ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC,
};

inline const std::vector<ge::Format> conv2DInnerXFormat =
    Repeat(conv2DInnerXFormatArr, 120);

inline const ge::DataType conv2DInnerFilterDtypeArr[] = {
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

inline const std::vector<ge::DataType> conv2DInnerFilterDtype =
    Expand(conv2DInnerFilterDtypeArr, 27);

inline const std::vector<ge::Format> conv2DInnerFilterFormat(3240, ge::FORMAT_FRACTAL_Z);

inline const ge::DataType conv2DInnerBiasDtypeArr[] = {
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

inline const std::vector<ge::DataType> conv2DInnerBiasDtype =
    Expand(conv2DInnerBiasDtypeArr, 27);

inline const std::vector<ge::Format> conv2DInnerBiasFormat(3240, ge::FORMAT_ND);

inline const std::vector<ge::DataType> conv2DInnerOffsetWDtype(3240, ge::DT_INT8);

inline const std::vector<ge::Format> conv2DInnerOffsetWFormat(3240, ge::FORMAT_ND);

inline const std::vector<ge::DataType> conv2DInnerPreQuantDtype(3240, ge::DT_UINT64);

inline const std::vector<ge::Format> conv2DInnerPreQuantFormat(3240, ge::FORMAT_ND);

inline const std::vector<ge::DataType> conv2DInnerPreReluDtype(3240, ge::DT_UINT32);

inline const std::vector<ge::Format> conv2DInnerPreReluFormat(3240, ge::FORMAT_ND);

inline const std::vector<ge::DataType> conv2DInnerPreClipReluDtype(3240, ge::DT_UINT64);

inline const std::vector<ge::Format> conv2DInnerPreClipReluFormat(3240, ge::FORMAT_ND);

inline const ge::DataType conv2DInnerEltwiseDtypeArr[] = {
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

inline const std::vector<ge::DataType> conv2DInnerEltwiseDtype =
    Expand(conv2DInnerEltwiseDtypeArr, 27);

inline const ge::Format conv2DInnerEltwiseFormatArr[] = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NCHW,
    ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW, ge::FORMAT_NCHW,
    ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC,
    ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC, ge::FORMAT_NHWC,
};

inline const std::vector<ge::Format> conv2DInnerEltwiseFormat =
    Repeat(conv2DInnerEltwiseFormatArr, 120);

inline const ge::DataType conv2DInnerAntiQuantDtypeArr[] = {
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

inline const std::vector<ge::DataType> conv2DInnerAntiQuantDtype =
    Expand(conv2DInnerAntiQuantDtypeArr, 27);

inline const std::vector<ge::Format> conv2DInnerAntiQuantFormat(3240, ge::FORMAT_ND);

inline const std::vector<ge::DataType> conv2DInnerPostQuantDtype(3240, ge::DT_UINT64);

inline const std::vector<ge::Format> conv2DInnerPostQuantFormat(3240, ge::FORMAT_ND);

inline const std::vector<ge::DataType> conv2DInnerPostReluDtype(3240, ge::DT_UINT32);

inline const std::vector<ge::Format> conv2DInnerPostReluFormat(3240, ge::FORMAT_ND);

inline const std::vector<ge::DataType> conv2DInnerPostClipReluDtype(3240, ge::DT_UINT64);

inline const std::vector<ge::Format> conv2DInnerPostClipReluFormat(3240, ge::FORMAT_ND);

inline const std::vector<ge::DataType> conv2DInnerDynamicAippDtype(3240, ge::DT_UINT64);

inline const std::vector<ge::Format> conv2DInnerDynamicAippFormat(3240, ge::FORMAT_ND);

inline const ge::DataType conv2DInnerPaddingDtypeArr[] = {
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

inline const std::vector<ge::DataType> conv2DInnerPaddingDtype =
    Expand(conv2DInnerPaddingDtypeArr, 27);

inline const std::vector<ge::Format> conv2DInnerPaddingFormat(3240, ge::FORMAT_ND);

inline const ge::DataType conv2DInnerYDtypeArr[] = {
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

inline const std::vector<ge::DataType> conv2DInnerYDtype =
    Expand(conv2DInnerYDtypeArr, 27);

inline const ge::Format conv2DInnerYFormatArr[] = {
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC, ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
    ge::FORMAT_NC1HWC0, ge::FORMAT_NCHW, ge::FORMAT_NHWC,
};

inline const std::vector<ge::Format> conv2DInnerYFormat =
    Repeat(conv2DInnerYFormatArr, 120);

class Conv2DInner : public OpDef {
public:
    explicit Conv2DInner(const char* name) : OpDef(name)
    {
        (void)(name);
        this->Input("x")
            .ParamType(REQUIRED)
            .DataType(conv2DInnerXDtype)
            .Format(conv2DInnerXFormat);
        this->Input("filter")
            .ParamType(REQUIRED)
            .DataType(conv2DInnerFilterDtype)
            .Format(conv2DInnerFilterFormat);
        this->Input("bias")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerBiasDtype)
            .Format(conv2DInnerBiasFormat);
        this->Input("offset_w")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerOffsetWDtype)
            .Format(conv2DInnerOffsetWFormat);
        this->Input("pre_quant")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerPreQuantDtype)
            .Format(conv2DInnerPreQuantFormat);
        this->Input("pre_relu")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerPreReluDtype)
            .Format(conv2DInnerPreReluFormat);
        this->Input("pre_clip_relu")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerPreClipReluDtype)
            .Format(conv2DInnerPreClipReluFormat);
        this->Input("eltwise")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerEltwiseDtype)
            .Format(conv2DInnerEltwiseFormat);
        this->Input("anti_quant")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerAntiQuantDtype)
            .Format(conv2DInnerAntiQuantFormat);
        this->Input("post_quant")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerPostQuantDtype)
            .Format(conv2DInnerPostQuantFormat);
        this->Input("post_relu")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerPostReluDtype)
            .Format(conv2DInnerPostReluFormat);
        this->Input("post_clip_relu")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerPostClipReluDtype)
            .Format(conv2DInnerPostClipReluFormat);
        this->Input("dynamic_aipp")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerDynamicAippDtype)
            .Format(conv2DInnerDynamicAippFormat);
        this->Input("padding")
            .ParamType(OPTIONAL)
            .DataType(conv2DInnerPaddingDtype)
            .Format(conv2DInnerPaddingFormat);
        this->Output("y")
            .ParamType(REQUIRED)
            .DataType(conv2DInnerYDtype)
            .Format(conv2DInnerYFormat);

        this->Attr("strides").AttrType(REQUIRED).ListInt();
        this->Attr("pads").AttrType(OPTIONAL).ListInt({0, 0, 0, 0});
        this->Attr("dilations").AttrType(OPTIONAL).ListInt({1, 1});
        this->Attr("groups").AttrType(OPTIONAL).Int(1);
        this->Attr("data_format").AttrType(OPTIONAL).String("NCHW");
        this->Attr("offset_x").AttrType(OPTIONAL).Int(0);
        this->Attr("round_mode").AttrType(OPTIONAL).String("rint");
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
            .ExtendCfgInfo("opFile.value", "conv2d_inner/arch51/conv2d_inner_kernel")
            .ExtendCfgInfo("opInterface.value", "conv2d_inner")
            .ExtendCfgInfo("keepOpInfo.value", "all")
            .ExtendCfgInfo("binaryConfig.value", "conv2d_inner/config/arch51/conv2d_inner_binary.json");

        this->AICore().AddConfig("kirindev0001", gzhCfg);
        // this->AICore().AddConfig("kirindev0002", gzhCfg);
    }
};

OP_ADD_BUILTIN(Conv2DInner);
} // ops
