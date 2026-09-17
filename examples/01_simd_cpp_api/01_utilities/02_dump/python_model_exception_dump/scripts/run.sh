#!/usr/bin/env bash
# run.sh — Exception Dump 看护用例执行入口（分进程执行，退出码语义见 README）
#
# 用法: ./run.sh <baseline|scene1|scene2|all|full>
#   scene1 / scene2 / baseline   单场景独立进程执行
#   all                          同进程 scene1 -> scene2 -> baseline（D3：验证 per-binary 接管）
#   full                         完整回归：scene1 -> verify -> parse -> all -> baseline
#
# 判定原则：以日志与 dump 产物为准；注入场景 test_public_dump.py 退出码 2 属预期成功路径。
set -u

SAMPLE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
TEST_DIR="${SAMPLE_DIR}/test"
MODE="${1:-scene1}"

run_one() {
    echo "===== test_public_dump.py $1 ====="
    python3 "${TEST_DIR}/test_public_dump.py" "$1"
    rc=$?
    if [ "$1" = "baseline" ]; then
        [ ${rc} -eq 0 ] && echo "[OK] baseline 正常结束（退出码 0）" || { echo "[FAIL] baseline 退出码 ${rc}"; return 1; }
    else
        { [ ${rc} -eq 0 ] || [ ${rc} -eq 2 ]; } && echo "[OK] 注入场景完成（退出码 ${rc}，2 为预期路径）" || { echo "[FAIL] 注入场景退出码 ${rc}"; return 1; }
    fi
}

case "${MODE}" in
    scene1|scene2|baseline|all)
        run_one "${MODE}"
        ;;
    full)
        # 场景分进程执行（D3 的 all 例外：同进程验证 per-binary 接管）
        run_one scene1   || exit 1
        echo "===== verify_dump.py ====="
        python3 "${SAMPLE_DIR}/python/verify_dump.py" || exit 1
        echo "===== parse_tiling.py ====="
        python3 "${SAMPLE_DIR}/python/parse_tiling.py" || exit 1
        run_one all      || exit 1
        run_one baseline || exit 1
        echo "[RESULT] 完整回归执行完成；产物与判定明细见上方日志"
        ;;
    *)
        echo "用法: $0 <baseline|scene1|scene2|all|full>" >&2
        exit 1
        ;;
esac
