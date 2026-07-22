#!/bin/bash
# ============================================================
# pre_smoke.sh - asc-devkit 冒烟测试脚本
# 依赖环境变量：
#   REPOSITORY_NAME - 仓库名 (asc-devkit)
#   pr_id           - PR ID
#   arm_run_url     - (可选) arm 运行包下载地址，传入则替代 OBS 拼接路径
#   WORKSPACE       - 工作目录 (默认当前目录)
#   soc_version     - (可选) SoC 版本，用于 slog 命名
# ============================================================
set +e

shopt -s extglob
WORKSPACE=/home/taskspace
cd /home/taskspace

log() {
  local dt
  dt=$(date '+%Y%m%d.%H%M%S')
  echo "===================================================================="
  echo "$dt : $*"
  echo "===================================================================="
}

log "init test case, please wait ..."
rm -rf /root/ascend/log

# ==============================
# 确定要测试的 ops 列表
# ==============================
declare -a ops
ops=("is_finite")

export ASCEND_GLOBAL_LOG_LEVEL=2
export ASCEND_SLOG_PRINT_TO_STDOUT=0

# ==============================
# 运行 asc-devkit 测试主流程
# ==============================
log "start run test case, please wait ..."

for op in "${ops[@]}"; do
  echo "Processing: $op"
  source /usr/local/Ascend/cann/set_env.sh

  cd ./scripts && bash devkit_dir_check.sh 2>&1 | tee -a ./run_test.log
  cd ${WORKSPACE}
  obs_base="https://ascend-ci.obs.cn-north-4.myhuaweicloud.com/${obs_path}/${package_name}"
  echo ${obs_base}
  arm_package="cann-asc-devkit_9.1.0_linux-aarch64.run"
  if [ -n "${arm_run_url}" ]; then
    wget -nv -O ${arm_package} "${arm_run_url}" 2>/dev/null
  else
    wget -nv -O  ${obs_base} ${arm_package} 2>/dev/null
  fi

  if [ ! -f "${arm_package}" ] || [ ! -s "${arm_package}" ]; then
    echo "No custom package found, This PR no need excute smoke."
    rm -f ${arm_package}
    exit 0
  fi

  chmod +x ${arm_package}
  yes "y" | bash "${arm_package}" --full --install-path=/usr/local/Ascend --quiet
  python3 -m pip install tensorflow -i https://pypi.tuna.tsinghua.edu.cn/simple
  source /usr/local/Ascend/cann/set_env.sh
  bash run_presmoke.sh 2>&1 | tee -a ./run_test.log
done

# 打包plog
mkdir -p /root/ascend
slog_name="slog.tar.gz"
tar -zcf slog.tar.gz -C /root/ascend log
OBS_KEY="${obs_smoke_path}/plog/${slog_name}"
# upload plog
if python3 /home/upload.py --bucket-name "ascend-ci" --action upload  --local-file "slog.tar.gz" --obs-object-key "${OBS_KEY}"; then
  echo "::set-output var=plog_url:https://ascend-ci.obs.cn-north-4.myhuaweicloud.com/${OBS_KEY}"
fi

# ==============================
# 检查 NPU 状态
# ==============================
log "checking NPU status ..."
mkdir -p ./npu_log
npu-smi info 2>&1 | tee ./npu_log/npu_info.log
if grep "dcmi module initialize failed" "./npu_log/npu_info.log"; then
  date_time=$(date '+%Y%m%d.%H%M%S')
  echo "$date_time : ${repo_name}_${pr_id} dcmi module initialize failed" >> ./npu_log/`date +%Y%m%d`.log
  python3 /home/aa/check_npu.py $repo_name $pr_id `cat /home/aa/hostname`
  exit 1
fi

# ==============================
# 检查测试结果
# ==============================
log "checking test results ..."

date_time=`date +%Y%m%d`"."`date +%H%M%S`
if grep -w -e "execute samples success" "./run_test.log"; then
  echo "$date_time : run test case success"
else
  echo "$date_time : run test case failed"
  exit 1
fi
