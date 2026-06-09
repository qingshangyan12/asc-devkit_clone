/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "graph/operator.h"
#include "register/register.h"

#include <cstdlib>
#include <string>

using namespace ge;

namespace domi {
namespace {
bool GetAlphaAttrValue(const std::string& attrs_json, std::string& alpha_value)
{
  const size_t alpha_pos = attrs_json.find("\"alpha\"");
  if (alpha_pos == std::string::npos) {
    return false;
  }

  const size_t attr_begin = attrs_json.rfind('{', alpha_pos);
  const size_t attr_end = attrs_json.find('}', alpha_pos);
  if (attr_begin == std::string::npos || attr_end == std::string::npos) {
    return false;
  }

  const std::string attr = attrs_json.substr(attr_begin, attr_end - attr_begin + 1);
  if (attr.find("\"type\":1") == std::string::npos && attr.find("\"type\": 1") == std::string::npos) {
    return false;
  }

  std::string f_key = "\"f\":\"";
  size_t value_begin = attr.find("\"f\":\"");
  if (value_begin == std::string::npos) {
    f_key = "\"f\": \"";
    value_begin = attr.find("\"f\": \"");
  }
  if (value_begin == std::string::npos) {
    return false;
  }

  value_begin += f_key.size();
  const size_t value_end = attr.find('"', value_begin);
  if (value_end == std::string::npos) {
    return false;
  }

  alpha_value = attr.substr(value_begin, value_end - value_begin);
  return true;
}
}
Status ParseOnnxParamsLeakyReluCustom(const ge::Operator& op_src, ge::Operator& op_dest) {
  // trans op_src to op_dest
  // if op_src get required attr failed, need to return Failed
  // if op_src get optional attr failed, need to return Failed or set a default value
  float negative_slope = 0.01f;
  string negative_slope_str;
  AscendString attrs_string;
  if (ge::GRAPH_SUCCESS == op_src.GetAttr("attribute", attrs_string)) {
    if (GetAlphaAttrValue(attrs_string.GetString(), negative_slope_str)) {
      negative_slope = std::atof(negative_slope_str.c_str());
    }
  }

  op_dest.SetAttr("negative_slope", negative_slope);
  return SUCCESS;
}

REGISTER_CUSTOM_OP("LeakyReluCustom")
    .FrameworkType(ONNX)
    .OriginOpType({ge::AscendString("ai.onnx::8::LeakyRelu"),
                   ge::AscendString("ai.onnx::9::LeakyRelu"),
                   ge::AscendString("ai.onnx::10::LeakyRelu"),
                   ge::AscendString("ai.onnx::11::LeakyRelu"),
                   ge::AscendString("ai.onnx::12::LeakyRelu"),
                   ge::AscendString("ai.onnx::13::LeakyRelu")})
    .ParseParamsByOperatorFn(ParseOnnxParamsLeakyReluCustom)
    .ImplyType(ImplyType::TVM);
}  // namespace domi
