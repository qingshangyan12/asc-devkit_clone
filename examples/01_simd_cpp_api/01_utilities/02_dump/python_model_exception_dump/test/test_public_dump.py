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

"""Exception Dump 看护用例入口：scene1/scene2/baseline/all 四场景。
注入场景 kernel 异常被捕获后退出码 2 属预期成功路径；产物落盘 <脚本目录>/task_fail_dump/。"""
import glob
import os
import sys

# model 与 Python API 层位于 ../python/
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "python"))
# dump 产物按 CWD 落盘，统一切换到脚本目录，保证任意位置执行产物路径一致
os.chdir(os.path.dirname(os.path.abspath(__file__)))

from ifa_exception_dump import run  # noqa: E402

SCENE = sys.argv[1] if len(sys.argv) > 1 else "all"
SCENES = ["scene1", "scene2", "baseline"] if SCENE == "all" else [SCENE]

dirs_before = set(glob.glob("task_fail_dump/dev*"))
print("[INFO] 运行前已有 dump 目录: %d 个" % len(dirs_before))

expected_fail = run(SCENES)

new_dirs = sorted(set(glob.glob("task_fail_dump/dev*")) - dirs_before)
print("[RESULT] dump 目录存在: %s" % os.path.isdir("task_fail_dump"))
print("[RESULT] 本轮新增 dump 目录数: %d" % len(new_dirs))
for d in new_dirs:
    files = os.listdir(d)
    bins = [f for f in files if f.endswith(".bin")]
    print("  %s: %d 个文件 (%s ...)" % (d, len(files), bins[0] if bins else "-"))
# 注入场景 kernel 异常被捕获属预期成功路径，以退出码 2 标记；baseline 正常结束退出 0
sys.exit(2 if expected_fail else 0)
