#!/bin/bash

SRC_DIR="/c/Users/guore/Desktop/蓝区样例开发/资料回合/doc_test/top5/Ascend  C API参考(markdown)ALL/figures"
DST_DIR="/c/Users/guore/Desktop/蓝区样例开发/资料回合/同步控制/asc-devkit_sync_contrl/docs/api/figures"

mkdir -p "$DST_DIR"

declare -a FILES=(
  "核间同步业务场景示例图(AIC-AIV为1-2).png"
  "block和subblock之间关系(灰色部分表示一个group-即1个block和N个subblock).png"
  "多AI-Core中AIC全核同步.png"
  "单AI-Core中AIV全核同步.png"
  "单AI-Core中AIC与AIV全核同步(AIV进行CrossCoreSetFlag).png"
  "flagId计数器反复设置的示例(计数器的值范围为0-15).png"
  "同步控制模式示意图.png"
  "2-8-3-3-6.png"
  "2-8-3-4-6.png"
)

for file in "${FILES[@]}"; do
  if [ -f "$SRC_DIR/$file" ]; then
    cp "$SRC_DIR/$file" "$DST_DIR/$file"
    echo "OK: $file"
  else
    echo "NOT FOUND: $SRC_DIR/$file"
  fi
done

echo "Done."
