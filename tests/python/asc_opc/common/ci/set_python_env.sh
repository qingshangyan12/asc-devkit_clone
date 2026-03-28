#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set -e

#current dir
cur_dir=$1
test_type=$2
product=$3
product_model=${product}
cmake_binary_dir=$4
version=$5

echo "$(date +"%Y-%m-%d %H:%M:%S") exec [set_python_env.sh] $cur_dir $test_type $product $cmake_binary_dir version: ${version}."

if [ "onetrack" = "${product}" ]; then
    product_model="mini"
fi

if [ "Xpy_ut" = "X$test_type" ];then
    test_type="ut"
    # fix set ddk version issue
    export PYTHONPATH=$cur_dir/llt/opc/ut/testcase:$PYTHONPATH
    export PYTHONPATH=$cur_dir/llt/opc/st/testcase:$PYTHONPATH
elif [ "Xpy_st" = "X$test_type" ];then
    test_type="st"
fi

#PYTHONPATH
export PYTHONPATH=$cur_dir/atc/opcompiler/opc/python/opc_tool:$PYTHONPATH
export PYTHONPATH=$cur_dir/atc/opcompiler/ascendc_compiler/framework/tools/build/asc_opc/python/asc_opc_tool:$PYTHONPATH
export PYTHONPATH=$cur_dir/llt/atc/opcompiler/opc/stub:$PYTHONPATH
export PYTHONPATH=$PYTHONPATH:cur_dir/open_source/tvm/tiling/:$cur_dir/open_source/tvm/tiling/auto_tune/:$cur_dir/asl/ops/cann/ops/built-in/tbe
export PYTHONPATH=$PYTHONPATH:$cur_dir/open_source/tvm/python:$cur_dir/open_source/tvm/python/te:$cur_dir/open_source/tvm/topi/python