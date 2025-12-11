/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef TOOLS_BUILD_ASC_OP_COMPILE_BASE_C_API_ASC_PLATFORM_CONSTANT_H
#define TOOLS_BUILD_ASC_OP_COMPILE_BASE_C_API_ASC_PLATFORM_CONSTANT_H

namespace tvm {
namespace cceconf {
constexpr const char* L0A_Buffer = "L0A_Buffer";
constexpr const char* L0B_Buffer = "L0B_Buffer";
constexpr const char* L0C_Buffer = "L0C_Buffer";
constexpr const char* L1_Buffer = "L1_Buffer";
constexpr const char* L2_Buffer = "L2_Buffer";
constexpr const char* FB_Buffer = "FB_Buffer";
constexpr const char* FB0_Buffer = "FB0_Buffer";
constexpr const char* FB1_Buffer = "FB1_Buffer";
constexpr const char* FB2_Buffer = "FB2_Buffer";
constexpr const char* FB3_Buffer = "FB3_Buffer";
constexpr const char* BT_Buffer = "BT_Buffer";
constexpr const char* Unified_Buffer = "Unified_Buffer";
constexpr const char* SMASK_Buffer = "SMASK_Buffer";

constexpr const char* Ascend_310 = "Ascend310";
constexpr const char* Ascend_910 = "Ascend910";
constexpr const char* Ascend_910A = "Ascend910A";
constexpr const char* Ascend_910B = "Ascend910B";
constexpr const char* Ascend_910B1 = "Ascend910B1";
constexpr const char* Ascend_910B2 = "Ascend910B2";
constexpr const char* Ascend_910B2C = "Ascend910B2C";
constexpr const char* Ascend_910B3 = "Ascend910B3";
constexpr const char* Ascend_910B4 = "Ascend910B4";
constexpr const char* Ascend_910B4_1 = "Ascend910B4-1";
constexpr const char* Ascend_910_93 = "Ascend910_93";
constexpr const char* Ascend_910_9391 = "Ascend910_9391";
constexpr const char* Ascend_910_9392 = "Ascend910_9392";
constexpr const char* Ascend_910_9381 = "Ascend910_9381";
constexpr const char* Ascend_910_9382 = "Ascend910_9382";
constexpr const char* Ascend_910_9372 = "Ascend910_9372";
constexpr const char* Ascend_910_9362 = "Ascend910_9362";
constexpr const char* Ascend_910_95 = "Ascend910_95";
constexpr const char* MC62CM12AA = "MC62CM12AA";
constexpr const char* Ascend_910_950y = "Ascend910_950y";
constexpr const char* Ascend_910_950z = "Ascend910_950z";
constexpr const char* Ascend_910_957b = "Ascend910_957b";
constexpr const char* Ascend_910_957d = "Ascend910_957d";
constexpr const char* Ascend_910_957c = "Ascend910_957c";
constexpr const char* Ascend_910_9589 = "Ascend910_9589";
constexpr const char* Ascend_910_958a = "Ascend910_958a";
constexpr const char* Ascend_910_958b = "Ascend910_958b";
constexpr const char* Ascend_910_9599 = "Ascend910_9599";
constexpr const char* Ascend_910_9579 = "Ascend910_9579";
constexpr const char* Ascend_910_9591 = "Ascend910_9591";
constexpr const char* Ascend_910_9592 = "Ascend910_9592";
constexpr const char* Ascend_910_9595 = "Ascend910_9595";
constexpr const char* Ascend_910_9596 = "Ascend910_9596";
constexpr const char* Ascend_910_9581 = "Ascend910_9581";
constexpr const char* Ascend_910_9582 = "Ascend910_9582";
constexpr const char* Ascend_910_9583 = "Ascend910_9583";
constexpr const char* Ascend_910_9584 = "Ascend910_9584";
constexpr const char* Ascend_910_9585 = "Ascend910_9585";
constexpr const char* Ascend_910_9586 = "Ascend910_9586";
constexpr const char* Ascend_910_9587 = "Ascend910_9587";
constexpr const char* Ascend_910_9588 = "Ascend910_9588";
constexpr const char* Ascend_910_9571 = "Ascend910_9571";
constexpr const char* Ascend_910_9572 = "Ascend910_9572";
constexpr const char* Ascend_910_9573 = "Ascend910_9573";
constexpr const char* Ascend_910_9574 = "Ascend910_9574";
constexpr const char* Ascend_910_9575 = "Ascend910_9575";
constexpr const char* Ascend_910_9576 = "Ascend910_9576";
constexpr const char* Ascend_910_9577 = "Ascend910_9577";
constexpr const char* Ascend_910_9578 = "Ascend910_9578";
constexpr const char* Ascend_910_95A1 = "Ascend910_95A1";
constexpr const char* Ascend_910_95A2 = "Ascend910_95A2";
constexpr const char* Ascend_910_55 = "Ascend910_55";
constexpr const char* Ascend_910_5591 = "Ascend910_5591";
constexpr const char* Ascend_910ProA = "Ascend910ProA";
constexpr const char* Ascend_910ProB = "Ascend910ProB";
constexpr const char* Ascend_910PremiumA = "Ascend910PremiumA";
constexpr const char* Ascend_310P = "Ascend310P";
constexpr const char* Ascend_310P1 = "Ascend310P1";
constexpr const char* Ascend_310P2 = "Ascend310P2";
constexpr const char* Ascend_310P3 = "Ascend310P3";
constexpr const char* Ascend_310P4 = "Ascend310P4";
constexpr const char* Ascend_310P5 = "Ascend310P5";
constexpr const char* Ascend_310P7 = "Ascend310P7";
constexpr const char* Ascend_610 = "Ascend610";
constexpr const char* kAscend610Lite = "Ascend610Lite";
constexpr const char* BS9SX2A = "BS9SX2A";
constexpr const char* BS9SX2AA = "BS9SX2AA";
constexpr const char* BS9SX2AB = "BS9SX2AB";
constexpr const char* MC61AM21A = "MC61AM21A";
constexpr const char* MC61AM21AA = "MC61AM21AA";
constexpr const char* MC61AM21AB = "MC61AM21AB";
constexpr const char* BS9SX1A = "BS9SX1A";
constexpr const char* BS9SX1AA = "BS9SX1AA";
constexpr const char* BS9SX1AB = "BS9SX1AB";
constexpr const char* BS9SX1AC = "BS9SX1AC";
constexpr const char* Ascend_610B = "Ascend610B";
constexpr const char* Ascend_610B1 = "Ascend610B1";
constexpr const char* Ascend_610B2 = "Ascend610B2";
constexpr const char* Ascend_310B = "Ascend310B";
constexpr const char* Ascend_310B1 = "Ascend310B1";
constexpr const char* Ascend_310B2 = "Ascend310B2";
constexpr const char* Ascend_310B3 = "Ascend310B3";
constexpr const char* Ascend_310B4 = "Ascend310B4";
constexpr const char* AS31XM1 = "AS31XM1";
constexpr const char* AS31XM1X = "AS31XM1X";
constexpr const char* Ascend_031 = "Ascend031";
constexpr const char* Ascend_035 = "Ascend035";
constexpr const char* Ascend_035A = "Ascend035A";
constexpr const char* Ascend_035B = "Ascend035B";
constexpr const char* Hi3796CV300ES = "Hi3796CV300ES";
constexpr const char* Hi3796CV300CS = "Hi3796CV300CS";
constexpr const char* SD3403 = "SD3403";
constexpr const char* Hi3519AV200 = "Hi3519AV200";
constexpr const char* HI19__DC = "Hi19__DC";
constexpr const char* HI19__MDC = "Hi19__MDC";
constexpr const char* HI19__VEC = "Hi19__VEC";

constexpr const char* DEVICE_CORE_NUM = "Device_core_num";
constexpr const char* CORE_NUM = "CORE_NUM";
constexpr const char* CUBE_CORE_CNT = "CUBE_CORE_CNT";
constexpr const char* VECTOR_CORE_CNT = "VECTOR_CORE_CNT";
constexpr const char* MAX_CORE_NUM = "MAX_CORE_NUM";
constexpr const char* SOC_VERSION = "SOC_VERSION";
constexpr const char* AICORE_NUM = "AICORE_NUM";
constexpr const char* MIX = "MIX";
constexpr const char* VECTORCORE_NUM = "VECTORCORE_NUM";
constexpr const char* AICORE_TYPE = "AICORE_TYPE";
constexpr const char* UB_SIZE = "UB_SIZE";
constexpr const char* L2_SIZE = "L2_SIZE";
constexpr const char* L1_SIZE = "L1_SIZE";
constexpr const char* CUBE_SIZE = "CUBE_SIZE";
constexpr const char* UNZIP = "UNZIP";
constexpr const char* L0A_LAYOUT_IS_zN = "L0A_LAYOUT_IS_zN";
constexpr const char* L0A_SIZE = "L0A_SIZE";
constexpr const char* L0B_SIZE = "L0B_SIZE";
constexpr const char* L0C_SIZE = "L0C_SIZE";
constexpr const char* SMASK_SIZE = "SMASK_SIZE";
constexpr const char* VECTOR_REG_WIDTH = "VECTOR_REG_WIDTH";
constexpr const char* PRIDCATE_REG_WIDTH = "PRIDCATE_REG_WIDTH";
constexpr const char* WIDE_REG_WIDTH = "WIDE_REG_WIDTH";
constexpr const char* COMPILER_ARCH = "Compiler_arch";
constexpr const char* COMPILER_AICPU_SUPPORT_OS = "Compiler_aicpu_support_os";
constexpr const char* FULL_SOC_VERSION = "FULL_SOC_VERSION";
constexpr const char* SHORT_SOC_VERSION = "SHORT_SOC_VERSION";
constexpr const char* L2_CAPABILITY = "Device_L2_capability";
constexpr const char* CORE_TYPE = "coreType";
constexpr const char* CUBE_VECTOR_SPLIT = "CUBE_VECTOR_SPLIT";
constexpr const char* CORE_TYPE_LIST = "CORE_TYPE_LIST";
constexpr const char* LOCAL_UB = "local.UB";
constexpr const char* LOCAL_L1 = "local.L1";
constexpr const char* LOCAL_L0A = "local.L0A";
constexpr const char* LOCAL_L0B = "local.L0B";
constexpr const char* LOCAL_L0C = "local.L0C";
constexpr const char* LOCAL_REG = "local.REG";
constexpr const char* LOCAL_SMASK = "local.SMASK";
constexpr const char* LOCAL_L1_FUSION = "local.L1_Fusion";
constexpr const char* LOCAL_UB_FUSION = "local.UB_Fusion";
constexpr const char* LOCAL_FB = "local.FB";
constexpr const char* LOCAL_FB0 = "local.FB0";
constexpr const char* LOCAL_FB1 = "local.FB1";
constexpr const char* LOCAL_FB2 = "local.FB2";
constexpr const char* LOCAL_FB3 = "local.FB3";
constexpr const char* FB_SIZE = "FB_SIZE";
constexpr const char* FB0_SIZE = "FB0_SIZE";
constexpr const char* FB1_SIZE = "FB1_SIZE";
constexpr const char* FB2_SIZE = "FB2_SIZE";
constexpr const char* FB3_SIZE = "FB3_SIZE";
constexpr const char* LOCAL_BT = "local.BT";
constexpr const char* BT_SIZE = "BT_SIZE";
constexpr const char* CUBE_FREQ = "CUBE_FREQ";
}  // namespace cceconf
}  // namespace tvm
#endif  // TOOLS_BUILD_ASC_OP_COMPILE_BASE_C_API_ASC_PLATFORM_CONSTANT_H