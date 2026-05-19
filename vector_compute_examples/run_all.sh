#!/bin/bash
set -e
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
echo "========================================="
echo "  vector_compute_examples 全部样例测试"
echo "========================================="
echo ""

SAMPLES=(
    "asc_abs"
    "asc_abs_sub"
    "asc_add"
    "asc_axpy"
    "asc_leakyrelu"
    "asc_mul"
    "asc_reduce_max"
    "asc_relu"
    "asc_select"
    "asc_sub"
    "asc_neg"
    "asc_max"
    "asc_min"
    "asc_div"
    "vconv/asc_e4m32float"
    "asc_sqrt"
    "asc_exp"
    "asc_ln"
    "asc_and"
    "asc_or"
    "asc_reduce_min"
    "asc_xor"
    "asc_shiftleft"
    "asc_ge"
    "asc_mul_scalar"
    "asc_eq"
    "asc_lt"
    "asc_gt"
    "asc_not"
    "asc_madd"
    "asc_shiftright"
    "asc_deintlv"
    "asc_intlv"
    "asc_transpose"
    "asc_add_scalar"
    "asc_le"
    "asc_prelu"
    "asc_reduce_sum"
    "asc_ne"
    "asc_pair_reduce_sum"
    "asc_mull"
    "asc_arange"
    "asc_squeeze"
    "asc_unpack_lower"
    "asc_pack"
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
