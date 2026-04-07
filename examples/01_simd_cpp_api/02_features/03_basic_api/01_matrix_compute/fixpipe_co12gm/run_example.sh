#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set -e

QUANT_MODE=${1:-"s322s8"}       # 模式 channelsplit, f322bf16, f322f16, f322s8, s322f16, s322s8
OUTPUT_FORMAT=${2:-1}           # 输出格式 1:ND, 2:NZ （channelsplit模式下该参数无效，默认为NZ）
PRE_QUANT_MODE=${3:-1}          # 量化模式 1:Scalar, 2:Vector （channelsplit, f322bf16, f322f16模式下该参数无效）
PRE_RELU_MODE=${4:-0}           # Relu模式 0:disable, 1:NormRelu （channelsplit模式下该参数无效）
EN_UNIT_FLAG=${5:-false}        # 是否使用unitFlag （channelsplit模式下该参数无效）
USE_DATA_COPY=${6:-false}       # 是否使用DataCopy接口代替Fixpipe接口
IS_CHANNEL_SPLIT=${7:-false}    # 是否通道拆分 （仅在QUANT_MODE为channelsplit模式下生效）

case $QUANT_MODE in
    "channelsplit")
        QUANT_MODE_VAL=0
        GEN_SCRIPT="gen_data_channelsplit.py"
        ;;
    "f322bf16")
        QUANT_MODE_VAL=1
        GEN_SCRIPT="gen_data_f322bf16.py"
        ;;
    "f322f16")
        QUANT_MODE_VAL=2
        GEN_SCRIPT="gen_data_f322f16.py"
        ;;
    "f322s8")
        QUANT_MODE_VAL=3
        GEN_SCRIPT="gen_data_f322s8.py"
        ;;
    "s322f16")
        QUANT_MODE_VAL=4
        GEN_SCRIPT="gen_data_s322f16.py"
        ;;
    "s322s8")
        QUANT_MODE_VAL=5
        GEN_SCRIPT="gen_data_s322s8.py"
        ;;
    *)
        echo "Unknown QUANT_MODE: $QUANT_MODE"
        echo "Supported modes: channelsplit, f322bf16, f322f16, f322s8, s322f16, s322s8"
        exit 1
        ;;
esac

echo "========================================"
echo "Running fixpipe_co12gm sample"
echo "QUANT_MODE: $QUANT_MODE (value: $QUANT_MODE_VAL)"
echo "OUTPUT_FORMAT: $OUTPUT_FORMAT (1:ND, 2:NZ)"
echo "PRE_QUANT_MODE: $PRE_QUANT_MODE (1:Scalar, 2:Vector)"
echo "PRE_RELU_MODE: $PRE_RELU_MODE (0:disable, 1:NormRelu)"
echo "EN_UNIT_FLAG: $EN_UNIT_FLAG"
echo "USE_DATA_COPY: $USE_DATA_COPY"
echo "IS_CHANNEL_SPLIT: $IS_CHANNEL_SPLIT"
echo "========================================"

mkdir -p build
cd build

cmake .. \
    -DQUANT_MODE=$QUANT_MODE_VAL \
    -DOUTPUT_FORMAT=$OUTPUT_FORMAT \
    -DPRE_QUANT_MODE=$PRE_QUANT_MODE \
    -DPRE_RELU_MODE=$PRE_RELU_MODE \
    -DEN_UNIT_FLAG=$EN_UNIT_FLAG \
    -DUSE_DATA_COPY=$USE_DATA_COPY \
    -DIS_CHANNEL_SPLIT=$IS_CHANNEL_SPLIT

make -j

if [ "$QUANT_MODE" = "channelsplit" ]; then
    python3 ../scripts/$GEN_SCRIPT -is_channel_split=$IS_CHANNEL_SPLIT
elif [ "$QUANT_MODE" = "f322bf16" ] || [ "$QUANT_MODE" = "f322f16" ]; then
    python3 ../scripts/$GEN_SCRIPT -output_format=$OUTPUT_FORMAT -pre_relu_mode=$PRE_RELU_MODE
elif [ "$QUANT_MODE" = "f322s8" ] || [ "$QUANT_MODE" = "s322f16" ] || [ "$QUANT_MODE" = "s322s8" ]; then
    python3 ../scripts/$GEN_SCRIPT -output_format=$OUTPUT_FORMAT -pre_quant_mode=$PRE_QUANT_MODE -pre_relu_mode=$PRE_RELU_MODE
fi

./demo

python3 ../scripts/verify_result.py output/output.bin output/golden.bin -quant_mode=$QUANT_MODE
