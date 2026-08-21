#!/bin/bash
# 通过跳板机 ipA 访问 ipB，在 ipB 上打包指定目录，然后回传到 ipA，再拉回本地。
# 依赖：本地与 ipA 上都需要安装 sshpass。

set -euo pipefail

# ==================== 配置区 ====================
# 跳板机 ipA
IP_A="192.168.1.10"
USER_A="root"
PASS_A="passwordA"
PORT_A="22"
DIR_A="/tmp/mc2_pkg"            # ipA 上的中转目录

# 目标机 ipB
IP_B="192.168.1.20"
USER_B="root"
PASS_B="passwordB"
PORT_B="22"
DIR_B="/home/xxx/MC2"           # ipB 上待打包目录的父路径

# 需要打包的子目录名
PKGS=("jjc_old" "jjc_new" "wzj_new")

# 本地保存目录
LOCAL_DIR="./mc2_pkg"
# ================================================

SSH_OPTS="-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR"

command -v sshpass >/dev/null 2>&1 || { echo "[ERROR] 本地未安装 sshpass"; exit 1; }

# 生成的文件名列表
FINAL_NAMES=()
for p in "${PKGS[@]}"; do
    FINAL_NAMES+=("${p}.tar.gz.txt")
done

# ---- 1. 在 ipB 上执行的打包命令 ----
TAR_CMD="cd '${DIR_B}'"
for p in "${PKGS[@]}"; do
    TAR_CMD+=" && tar -zcvf ${p}.tar.gz ${p} && mv -f ${p}.tar.gz ${p}.tar.gz.txt"
done
TAR_CMD+=" && ls -lh"
for n in "${FINAL_NAMES[@]}"; do
    TAR_CMD+=" ${n}"
done

# ---- 2. 在 ipA 上执行的脚本：调用 ipB 打包 + 拉取文件到 ipA ----
SCP_B_SRC=""
for n in "${FINAL_NAMES[@]}"; do
    SCP_B_SRC+=" ${USER_B}@${IP_B}:${DIR_B}/${n}"
done

A_SCRIPT="set -e
command -v sshpass >/dev/null 2>&1 || { echo '[ERROR] ipA 未安装 sshpass'; exit 1; }
export SSHPASS='${PASS_B}'
mkdir -p '${DIR_A}'
echo '===> [ipB] 开始打包'
sshpass -e ssh ${SSH_OPTS} -p ${PORT_B} ${USER_B}@${IP_B} \"${TAR_CMD}\"
echo '===> [ipB -> ipA] 拷贝压缩包'
sshpass -e scp ${SSH_OPTS} -P ${PORT_B}${SCP_B_SRC} '${DIR_A}/'
ls -lh '${DIR_A}'
"

echo "===> 连接跳板机 ${IP_A}"
export SSHPASS="${PASS_A}"
sshpass -e ssh ${SSH_OPTS} -p "${PORT_A}" "${USER_A}@${IP_A}" bash -s <<< "${A_SCRIPT}"

# ---- 3. 从 ipA 拉回本地 ----
echo "===> [ipA -> 本地] 拷贝压缩包到 ${LOCAL_DIR}"
mkdir -p "${LOCAL_DIR}"
SCP_A_SRC=()
for n in "${FINAL_NAMES[@]}"; do
    SCP_A_SRC+=("${USER_A}@${IP_A}:${DIR_A}/${n}")
done
sshpass -e scp ${SSH_OPTS} -P "${PORT_A}" "${SCP_A_SRC[@]}" "${LOCAL_DIR}/"

echo "===> 完成"
ls -lh "${LOCAL_DIR}"
