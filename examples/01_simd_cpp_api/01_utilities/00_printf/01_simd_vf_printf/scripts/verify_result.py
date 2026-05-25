#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


import re
import os
import sys


def validate_log_file(log_file_path):
    """校验simd_vf打印输出的日志内容是否正确"""
    if not os.path.exists(log_file_path):
        print(f"Error: File not found - {log_file_path}")
        return False

    with open(log_file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    passed = 0
    total = 0

    # 1. 检查aicore侧printf输出
    total += 1
    if "[aicore] simd_vf printf demo start ..." in content:
        print("Pass: aicore printf start marker")
        passed += 1
    else:
        print("[Failed]: aicore printf start marker not found")

    total += 1
    if "[aicore] simd_vf printf demo end ..." in content:
        print("Pass: aicore printf end marker")
        passed += 1
    else:
        print("[Failed]: aicore printf end marker not found")

    # 2. 检查simd_vf printf格式打印 (int=10, uint=20, float=3.14, string=hello)
    total += 1
    simd_match = re.search(r'\[simd_vf\]\s*int=10,\s*uint=20,\s*float=3\.14[0]*,\s*string=hello', content)
    if simd_match:
        print("Pass: simd_vf printf format (int/uint/float/string)")
        passed += 1
    else:
        print("[Failed]: simd_vf printf format not found or incorrect")

    # 3. 检查simd_vf printf hex/ptr打印
    total += 1
    hex_ptr_match = re.search(r'\[simd_vf\]\s*hex=ff,\s*ptr=', content)
    if hex_ptr_match:
        print("Pass: simd_vf printf hex/ptr format")
        passed += 1
    else:
        print("[Failed]: simd_vf printf hex/ptr format not found or incorrect")

    # 4. 检查simd_vf纯字符串打印
    total += 1
    if "[simd_vf] This is a simd_vf printf demo string." in content:
        print("Pass: simd_vf printf string literal")
        passed += 1
    else:
        print("[Failed]: simd_vf printf string literal not found")

    # 5. 检查simt_vf printf输出
    total += 1
    simt_match = re.findall(r'\[simt_vf\]\s*threadIdx=\d+,\s*blockIdx=\d+', content)
    if simt_match and len(simt_match) > 0:
        print(f"Pass: simt_vf printf ({len(simt_match)} threads)")
        passed += 1
    else:
        print("[Failed]: simt_vf printf output not found")

    # 6. 检查dump输出包含数据标记
    total += 1
    if "DumpTensor" in content or "Dump" in content:
        print("Pass: simd_vf dump output found")
        passed += 1
    else:
        print("[Failed]: simd_vf dump output not found (may need ASCENDC_SIMD_VF_DEBUG=ON)")

    # 最终汇总
    if passed == total:
        print(f"[Success]! All validations passed. ({passed}/{total})")
        return True
    else:
        print(f"[Failed]: {passed}/{total} checks passed")
        return False


if __name__ == "__main__":
    if len(sys.argv) < 2:
        log_filename = 'output/log.txt'
    else:
        log_filename = sys.argv[1]
    sys.exit(not validate_log_file(log_filename))
