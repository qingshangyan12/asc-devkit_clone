#!/bin/bash
# 用法：
#   bash test.sh            # 测试 CASE_NUM 1~9 全部场景
#   CASE_NUM=3 bash test.sh # 只测试单个场景

ALL_CASES=${CASE_NUM:-"1 2 3 4 5 6 7 8 9"}
PASS=0
FAIL=0

for c in ${ALL_CASES}; do
    echo "========== CASE_NUM=${c} =========="
    rm -rf build
    mkdir -p build && cd build
    cmake .. -DCASE_NUM=${c} > /dev/null 2>&1
    if ! make -j > /dev/null 2>&1; then
        echo "[BUILD] CASE_NUM=${c} FAILED"
        FAIL=$((FAIL + 1))
        cd ..
        continue
    fi
    python3 ../scripts/gen_data.py --case ${c}
    ./demo
    if python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin; then
        PASS=$((PASS + 1))
    else
        FAIL=$((FAIL + 1))
    fi
    cd ..
    echo ""
done

echo "=============================="
echo "Summary: PASS=${PASS}, FAIL=${FAIL}"
[ ${FAIL} -eq 0 ] && echo "All cases passed!" || echo "Some cases failed!"
