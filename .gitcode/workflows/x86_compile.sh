#!/bin/bash

cd ${WORKSPACE}
echo $(grep -E "^VERSION_ID=" /etc/os-release | cut -d'"' -f2)
if [[ "${task_name}" == *ubuntu24* || "${task_name}" == *24* ]]; then
    sudo update-alternatives --set gcc /usr/bin/gcc-14
else
    if [[ -f "/opt/rh/devtoolset-7/enable" ]]; then
        echo "source devtoolset"
        source /opt/rh/devtoolset-7/enable
    fi
fi
gcc --version
source /home/jenkins/Ascend/cann/bin/setenv.bash
set +e
case "${task_name}" in
    x86_compile_ubuntu24)
        sed -i "1i set(CMAKE_EXPORT_COMPILE_COMMANDS ON)" "CMakeLists.txt"
        ;;
esac
if [ "${GIT_TARGET_BRANCH}" == master ]; then
        bash build.sh --pkg --cann_3rd_lib_path="/home/jenkins/opensource" -f "pr_filelist.txt"
        echo "exec cmd: [bash build.sh --pkg --cann_3rd_lib_path="/home/jenkins/opensource" -f "pr_filelist.txt"]"
elif [ "${GIT_TARGET_BRANCH}" == exp/kadc] || [ "${GIT_TARGET_BRANCH}" == feature/tensor_api_from_9.0.0 ]; then
        echo "not need build"
        mkdir -p build_out
        touch ${WORKSPACE}/build_out/cann-asc-devkit.run
else
        bash build.sh --pkg --cann_3rd_lib_path="/home/jenkins/opensource"
fi

if [ ! -f "build_out/"*.run ]; then
    mkdir -p build_out
    touch build_out/cann-ops-math-test_linux-x86_64.run
fi
