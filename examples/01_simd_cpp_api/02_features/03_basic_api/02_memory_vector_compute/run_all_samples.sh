#!/bin/bash

# ==============================================================================
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# ==============================================================================
# Description: Run all samples in NPU/CPU/NPU-Sim modes (without verification)
# ==============================================================================

set -euf  # Exit on error, undefined variable check

# Configure environment variables
if [ -f "/usr/local/Ascend/cann/set_env.sh" ]; then
    source /usr/local/Ascend/cann/set_env.sh
elif [ -f "$HOME/Ascend/cann/set_env.sh" ]; then
    source "$HOME/Ascend/cann/set_env.sh"
else
    echo "Error: CANN not found. Please set CANN environment manually."
    exit 1
fi

# Sample list and scenario count configuration (hardcoded)
declare -A SAMPLE_SCENARIOS
SAMPLE_SCENARIOS[block_reduce_min_max_sum]=3
SAMPLE_SCENARIOS[mrg_sort]=3
SAMPLE_SCENARIOS[pair_reduce_sum]=1
SAMPLE_SCENARIOS[whole_reduce_min_max_sum]=7
SAMPLE_SCENARIOS[region_proposal_sort]=1

# Test results statistics
TOTAL_TESTS=0

# Define build and run function
build_and_run() {
    local sample_dir=$1
    local scenario_num=$2
    local mode=$3
    local arch=$4
    local cmake_args=$5
    
    echo ""
    echo "========================================="
    echo "Running ${sample_dir}, scenario ${scenario_num}, ${mode} mode (${arch})..."
    echo "========================================="
    
    cd "${sample_dir}"
    
    rm -rf build
    mkdir -p build && cd build
    
    if [ "${scenario_num}" -ne 0 ]; then
        cmake .. ${cmake_args} -DSCENARIO_NUM=${scenario_num}
    else
        cmake .. ${cmake_args}
    fi
    
    make -j
    cd ..
    
    if [ "${scenario_num}" -ne 0 ]; then
        python3 scripts/gen_data.py -scenarioNum=${scenario_num}
    else
    python3 scripts/gen_data.py
    fi
    
    cd build
    ./demo
    cd ..
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
}

# Iterate through all samples and scenarios
for sample_dir in block_reduce_min_max_sum mrg_sort pair_reduce_sum whole_reduce_min_max_sum region_proposal_sort; do
    if [ ! -d "${sample_dir}" ]; then
        echo "Warning: Sample directory '${sample_dir}' not found, skipping..."
        continue
    fi
    
    scenario_count=${SAMPLE_SCENARIOS[$sample_dir]}
    
    echo ""
    echo "================================================================================"
    echo "Sample: ${sample_dir} (${scenario_count} scenarios)"
    echo "================================================================================"
    
    # Iterate through all scenarios (scenario=0 means not using SCENARIO_NUM)
    for ((scenario = 0; scenario <= scenario_count; scenario++)); do
        # NPU mode (dav-2201)
        build_and_run ${sample_dir} ${scenario} "NPU" "dav-2201" "-DNPU_ARCH=dav-2201"
        
        # CPU mode (dav-2201)
        build_and_run ${sample_dir} ${scenario} "CPU" "dav-2201" "-DRUN_MODE=cpu -DNPU_ARCH=dav-2201"
        
        # NPU-Sim mode (dav-2201)
        build_and_run ${sample_dir} ${scenario} "NPU-Sim" "dav-2201" "-DRUN_MODE=sim -DNPU_ARCH=dav-2201"
        
        # NPU-Sim mode (dav-3510)
        build_and_run ${sample_dir} ${scenario} "NPU-Sim" "dav-3510" "-DRUN_MODE=sim -DNPU_ARCH=dav-3510"
    done
done

# Output test results summary
echo ""
echo "================================================================================"
echo "Test Results Summary"
echo "================================================================================"
echo ""
echo "Total tests: ${TOTAL_TESTS}"
echo ""

echo "All tests completed!"
