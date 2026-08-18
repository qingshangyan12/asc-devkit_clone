# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# 推荐使用 SOC_SERIES 指定 kernel 支持的芯片系列；不指定时，kernel 默认适用于
# ASCEND_SOC_SERIES 中配置的所有芯片系列。
npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustom
    KERNEL_DIR add_custom
    KERNEL_FILE add_custom_kernel.cpp
    # SOC_SERIES ${ASCEND_SOC_SERIES}
)

npu_op_kernel_sources(ascendc_kernels
    OP_TYPE LeakyReluCustom
    KERNEL_DIR leaky_relu_custom
    KERNEL_FILE leaky_relu_custom_kernel.cpp
    # SOC_SERIES ${ASCEND_SOC_SERIES}
)

npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustomTilingSink
    KERNEL_DIR add_custom_tiling_sink
    KERNEL_FILE add_custom_tiling_sink_kernel.cpp
    # SOC_SERIES ${ASCEND_SOC_SERIES}
)

npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustomTemplate
    KERNEL_DIR add_custom_template
    KERNEL_FILE add_custom_template.cpp
    # SOC_SERIES ${ASCEND_SOC_SERIES}
)

npu_op_kernel_library(ascendc_kernels
    SRC_BASE ${CMAKE_CURRENT_SOURCE_DIR}
    TILING_LIBRARY cust_optiling
)

npu_op_package_add(${package_name}
    LIBRARY
        ascendc_kernels
)
