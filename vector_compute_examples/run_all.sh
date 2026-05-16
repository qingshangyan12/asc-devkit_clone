#!/bin/bash
set -e
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
echo "========================================="
echo "  vector_compute_examples 全部样例测试"
echo "========================================="
echo ""

SAMPLES=(
    "asc_abs"
    "asc_add"
    "asc_mul"
    "asc_relu"
    "vconv/asc_e4m32float"
)

PASS=0
FAIL=0

for sample in "${SAMPLES[@]}"; do
    SAMPLE_DIR="${ROOT_DIR}/${sample}"
    if [ ! -f "${SAMPLE_DIR}/build_and_run.sh" ]; then
        echo "[跳过] ${sample}: 未找到 build_and_run.sh"
        continue
    fi
    echo ">>> 运行 ${sample} <<<"
    if bash "${SAMPLE_DIR}/build_and_run.sh"; then
        echo "<<< ${sample} 通过 >>>"
        ((PASS++)) || true
    else
        echo "<<< ${sample} 失败 >>>"
        ((FAIL++)) || true
    fi
    echo ""
    rm -rf "${SAMPLE_DIR}/build"
done

echo "========================================="
echo "  结果: ${PASS} 通过, ${FAIL} 失败"
echo "========================================="
