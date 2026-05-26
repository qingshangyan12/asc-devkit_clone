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

# 功能：监控两个目录一级结构变化

# 定义监控目录和快照文件（可根据实际情况修改）
DIR_LIST=(
    "/usr/local/Ascend/cann/|./monitor_snapshot_1.txt"
    "/usr/local/Ascend/cann/asc/|./monitor_snapshot_2.txt"
)

cat << 'EOF' > monitor_snapshot_1.txt
d aarch64-linux
d combo_script
d compat
d compiler
d fwkacllib
d opp
d python
d runtime
d share
d toolkit
d tools
d var
f cann_uninstall.sh
f ops_uninstall.sh
f set_env.sh
l arm64-linux
l asc
l bin
l conf
l devlib
l include
l lib64
l pkg_inc
EOF

cat << 'EOF' > monitor_snapshot_2.txt
d impl
d include
EOF

# 遍历监控每个目录
for item in "${DIR_LIST[@]}"; do
    # 分割目录和快照路径
    TARGET_DIR=$(echo "$item" | cut -d'|' -f1)
    SNAPSHOT_FILE=$(echo "$item" | cut -d'|' -f2)

    # 校验目录是否存在
    if [ ! -d "$TARGET_DIR" ]; then
        echo -e "\n【错误】目录不存在：$TARGET_DIR" 2>&1 | tee -a result_check.txt
        continue
    fi

    # 创建临时文件
    CURRENT_SNAP=$(mktemp)
    # 生成当前目录一级结构快照
    find "$TARGET_DIR" -maxdepth 1 -mindepth 1 -printf '%y %f\n' 2>/dev/null | sort > "$CURRENT_SNAP"
    echo -e "\n========== 监控目录：$TARGET_DIR =========="

    # 对比快照
    DIFF=$(diff "$SNAPSHOT_FILE" "$CURRENT_SNAP" 2>/dev/null)
    if [ -z "$DIFF" ]; then
        echo "✔ 目录结构未变动" 2>&1 | tee -a result_check.txt
    else
        echo "✘ 目录结构已变动，差异如下：" 2>&1 | tee -a result_check.txt
        echo "$DIFF"
    fi

    # 清理临时文件
    rm "$CURRENT_SNAP"
done
echo -e "\n========== 目录对比完成 =========="

# 结构判断
COUNT=$(grep -c -F "目录结构未变动" result_check.txt)
if [ "$COUNT" -eq 2 ]; then
    echo "check pass"
else
    echo "check fail"
fi
