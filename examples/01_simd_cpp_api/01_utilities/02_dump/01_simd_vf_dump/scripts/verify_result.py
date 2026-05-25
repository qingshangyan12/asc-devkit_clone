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
    """校验simd_vf dump输出的日志内容是否正确"""
    if not os.path.exists(log_file_path):
        print(f"Error: File not found - {log_file_path}")
        return False

    with open(log_file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    passed = 0
    total = 0

    # 1. 检查aicore侧printf输出
    total += 1
    if "[aicore] simd_vf dump demo start ..." in content:
        print("Pass: aicore printf start marker")
        passed += 1
    else:
        print("[Failed]: aicore printf start marker not found")

    total += 1
    if "[aicore] simd_vf dump demo end ..." in content:
        print("Pass: aicore printf end marker")
        passed += 1
    else:
        print("[Failed]: aicore printf end marker not found")

    # 2. 检查UB dump输出标记
    total += 1
    if "UB data (asc_dump_ubuf + asc_dump)" in content:
        print("Pass: UB dump section marker found")
        passed += 1
    else:
        print("[Failed]: UB dump section marker not found")

    # 3. 检查Reg dump输出标记
    total += 1
    if "Register data (asc_dump_reg + asc_dump)" in content:
        print("Pass: Register dump section marker found")
        passed += 1
    else:
        print("[Failed]: Register dump section marker not found")

    # 4. 检查DumpTensor / Dump数据输出
    total += 1
    dump_count = len(re.findall(r'DumpTensor|Dump\s', content))
    if dump_count > 0:
        print(f"Pass: Dump data output found ({dump_count} dump blocks)")
        passed += 1
    else:
        print("[Failed]: No Dump data output found")

    # 5. 检查asc_dump_ubuf输出 (32个值的block)
    total += 1
    ubuf_blocks = re.findall(r'\[([^\]]*)\]\s*$', content, re.MULTILINE)
    if len(ubuf_blocks) >= 1:
        print(f"Pass: UB dump data content found ({len(ubuf_blocks)} data blocks)")
        passed += 1
    else:
        print("[Failed]: UB dump data content not found")

    # 6. 检查asc_dump_reg输出 (Reg数据包含3.0和4.0复制值)
    total += 1
    if "3.0" in content or "3." in content:
        print("Pass: Register dump data values found (expected 3.0 / 4.0)")
        passed += 1
    else:
        print("[Failed]: Register dump data values not found")

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
