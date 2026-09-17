#!/usr/bin/env bash
# check_env.sh — Exception Dump 样例环境检查
#
# 检查项：CANN 安装与版本、异常回调注册新接口存在性（aclrtExceptionInfoCallbackRegister）、
# NPU 在位、Python 栈（torch / torch_npu / ascend_ops）。
# 任一检查失败以非 0 退出并给出明确原因。
set -u

SAMPLE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
fail=0

echo "== 1. CANN 环境 =="
if [ -z "${ASCEND_HOME_PATH:-}" ]; then
    echo "[MISS] ASCEND_HOME_PATH 未设置，请先 source <CANN>/bin/setenv.bash 或 ascend-toolkit/set_env.sh"
    fail=1
else
    echo "[OK]   ASCEND_HOME_PATH = ${ASCEND_HOME_PATH}"
    if [ -f "${ASCEND_HOME_PATH}/aarch64-linux/ascend_toolkit_install.info" ]; then
        grep -i version "${ASCEND_HOME_PATH}/aarch64-linux/ascend_toolkit_install.info" || true
    fi
fi

echo "== 2. 异常回调注册接口存在性（旧 CANN 只有已废弃的 aclrtSetExceptionInfoCallback，不支持本样例）=="
if [ -n "${ASCEND_HOME_PATH:-}" ]; then
    header="$(ls "${ASCEND_HOME_PATH}"/aarch64-linux/include/acl/acl_rt.h 2>/dev/null || \
              ls "${ASCEND_HOME_PATH}"/include/acl/acl_rt.h 2>/dev/null || true)"
    if [ -n "${header}" ] && grep -q "aclrtExceptionInfoCallbackRegister" "${header}"; then
        echo "[OK]   头文件含 aclrtExceptionInfoCallbackRegister: ${header}"
    else
        echo "[MISS] CANN 头文件无 aclrtExceptionInfoCallbackRegister（${header:-未找到 acl_rt.h}）"
        echo "       本样例要求 CANN 提供新的回调注册/注销接口；旧 CANN 不支持端到端用例"
        fail=1
    fi
else
    echo "[SKIP] 未设置 ASCEND_HOME_PATH，跳过"
fi

echo "== 3. NPU 在位 =="
if command -v npu-smi >/dev/null 2>&1 && npu-smi info 2>/dev/null | grep -q "910"; then
    npu-smi info | head -6
else
    echo "[MISS] npu-smi 不可用或无 NPU 设备；端到端用例必须在 NPU 实机执行"
    fail=1
fi

echo "== 4. Python 栈（端到端用例必需；仅静态检查可跳过）==="
python3 - <<'EOF'
import sys
missing = []
try:
    import torch  # noqa: F401
except ImportError:
    missing.append("torch")
try:
    import torch_npu  # noqa: F401
except ImportError:
    missing.append("torch_npu")
try:
    import ascend_ops  # noqa: F401
except ImportError:
    missing.append("ascend_ops（先执行 scripts/build_ifa.sh 构建安装）")
if missing:
    print("[MISS] 缺少: %s" % ", ".join(missing))
    sys.exit(1)
import torch, torch_npu  # noqa: F401
print("[OK]   python %s, torch %s, torch_npu %s, ascend_ops 已安装"
      % (sys.version.split()[0], torch.__version__, torch_npu.__version__))
EOF
if [ $? -ne 0 ]; then
    fail=1
fi

echo "== 5. 回调库 =="
if [ -f "${SAMPLE_DIR}/runtime/exception_dump_public.so" ]; then
    echo "[OK]   ${SAMPLE_DIR}/runtime/exception_dump_public.so 已存在"
else
    echo "[TODO] 回调库未编译，请执行 scripts/build_callback.sh（普通仓库 CI 的静态检查到此为止）"
fi

if [ ${fail} -ne 0 ]; then
    echo "\n[RESULT] 环境检查未通过"
    exit 1
fi
echo "\n[RESULT] 环境检查通过"
