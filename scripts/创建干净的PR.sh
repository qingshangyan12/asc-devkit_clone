#!/bin/bash

# sourceRoot="/c/Users/guore/Desktop/蓝区样例开发/资料回合/原子操作/asc-devkit_8937"
# targetRoot="/c/Users/guore/Desktop/蓝区样例开发/资料回合/asc-devkit_4489"

# # Markdown 文件列表（源路径相对于 sourceRoot）
# mdFiles=(
"docs/api/SIMD-API/基础API/原子操作/DisableDmaAtomic.md"
"docs/api/SIMD-API/基础API/原子操作/GetStoreAtomicConfig(ISASI).md"
"docs/api/SIMD-API/基础API/原子操作/SetAtomicAdd.md"
"docs/api/SIMD-API/基础API/原子操作/SetAtomicMax(ISASI).md"
"docs/api/SIMD-API/基础API/原子操作/SetAtomicMin(ISASI).md"
"docs/api/SIMD-API/基础API/原子操作/SetAtomicType.md"
"docs/api/SIMD-API/基础API/原子操作/SetStoreAtomicConfig(ISASI).md"
"docs/api/SIMD-API/基础API/原子操作/关键特性说明.md"
"docs/api/SIMD-API/基础API/原子操作/原子操作.md"
"docs/api/SIMD-API/基础API/原子操作/原子操作概述.md"
# )

# # PNG 文件列表（源路径相对于 sourceRoot）
# pngFiles=(
#     "docs/api/figures/atomic_operation_pipelined_data_movement_effect.png"
#     "docs/api/figures/deterministic_computation_gm_data_change_process.png"
#     "docs/api/figures/non_deterministic_computation_1_gm_data_change_process.png"
#     "docs/api/figures/non_deterministic_computation_2_gm_data_change_process.png"
#     "docs/api/figures/software_synchronization_between_two_cores_flowchart.png"
# )


sourceRoot="/c/Users/guore/Desktop/蓝区样例开发/资料回合/同步控制/asc-devkit_sync_contrl"
targetRoot="/c/Users/guore/Desktop/蓝区样例开发/资料回合/asc-devkit_4489"

# Markdown 文件列表（源路径相对于 sourceRoot）
mdFiles=(
"docs/api/SIMD-API/基础API/同步控制/同步控制-73.md"
"docs/api/SIMD-API/基础API/同步控制/核内同步/DataSyncBarrier(ISASI).md"
"docs/api/SIMD-API/基础API/同步控制/核内同步/PipeBarrier(ISASI).md"
"docs/api/SIMD-API/基础API/同步控制/核内同步/SetFlag-WaitFlag(ISASI).md"
"docs/api/SIMD-API/基础API/同步控制/核内同步/同步控制简介.md"
"docs/api/SIMD-API/基础API/同步控制/核内同步/核内同步.md"
"docs/api/SIMD-API/基础API/同步控制/核内同步/核内同步能力概述.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/关键特性说明.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/核间同步.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/核间同步能力概述.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/系统同步能力概述.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/IBSet.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/IBWait.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/InitDetermineComputeWorkspace.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/NotifyNextBlock.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/SyncAll.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/WaitPreBlock.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/关键特性说明.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/核间同步.md"
"docs/api/SIMD-API/基础API/同步控制/核间同步/核间同步能力概述.md"
"docs/api/SIMD-API/基础API/同步控制/系统同步能力概述.md"
)

# PNG 文件列表（源路径相对于 sourceRoot）
pngFiles=(
"docs/api/figures/ascend_950pr_950dt_architecture.png"
"docs/api/figures/atlas_a2_a3_architecture.png"
"docs/api/figures/block_subblock_relationship.png"
"docs/api/figures/cross_wait_flag_multi_mode_error.png"
"docs/api/figures/datacopy_address_overlap_pipebarrier.png"
"docs/api/figures/flag_id_counter_reuse_example.png"
"docs/api/figures/flag_id_switch_mode_error.png"
"docs/api/figures/inter_core_aic_all_sync.png"
"docs/api/figures/inter_core_sync_scenario_example.png"
"docs/api/figures/intra_core_parallel_pipeline_timing.png"
"docs/api/figures/mul_add_pipebarrier_sync_diagram.png" 
"docs/api/figures/pipebarrier_function_diagram.png"
"docs/api/figures/setflag_waitflag_multi_pipeline_sync.png"
"docs/api/figures/single_core_aic_aiv_sync_aiv_setflag.png"
"docs/api/figures/single_core_aiv_all_sync.png"
"docs/api/figures/sync_control_mode_diagram.png"
"docs/api/figures/vector_compute_data_flow-43.png"
"docs/api/figures/vector_compute_data_flow.png"
)

echo "开始拷贝 Markdown 文件..."
for file in "${mdFiles[@]}"; do
    src="${sourceRoot}/${file}"
    dst="${targetRoot}/${file}"
    dstDir=$(dirname "$dst")
    if [ ! -d "$dstDir" ]; then
        mkdir -p "$dstDir"
    fi
    if [ -f "$src" ]; then
        cp -f "$src" "$dst"
        echo "  [OK] $file"
    else
        echo "  [缺失] 源文件不存在: $src"
    fi
done

echo ""
echo "开始拷贝 PNG 文件..."
for file in "${pngFiles[@]}"; do
    src="${sourceRoot}/${file}"
    dst="${targetRoot}/${file}"
    dstDir=$(dirname "$dst")
    if [ ! -d "$dstDir" ]; then
        mkdir -p "$dstDir"
    fi
    if [ -f "$src" ]; then
        cp -f "$src" "$dst"
        echo "  [OK] $file"
    else
        echo "  [缺失] 源文件不存在: $src"
    fi
done

echo ""
echo "全部拷贝完成！"
