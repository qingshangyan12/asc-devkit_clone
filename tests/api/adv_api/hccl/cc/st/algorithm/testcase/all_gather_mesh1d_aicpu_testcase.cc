    u64 sendCount = (512 * 1024 - 1) / sizeof(float);        // 边界：512KiB-1（selector阈值）
    HcclDataType dataType = HcclDataType::HCCL_DATA_TYPE_FP32;
    RunAllGatherMesh1DAicpuA5(topoMeta, sendCount, dataType);
}

// ========== 3 Rank测试 ==========
TEST_F(ST_ALL_GATHER_MESH1D_AICPU_TEST, mesh1d_3rank_128kib_int32)
{
    TopoMeta topoMeta{{{0, 1, 2}}};
    u64 sendCount = (128 * 1024) / sizeof(int32_t);
    HcclDataType dataType = HcclDataType::HCCL_DATA_TYPE_INT32;
    RunAllGatherMesh1DAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_MESH1D_AICPU_TEST, mesh1d_3rank_512kib_fp16)
{
    TopoMeta topoMeta{{{0, 1, 2}}};
    u64 sendCount = (512 * 1024) / 2;                        // 边界：正好512KiB
    HcclDataType dataType = HcclDataType::HCCL_DATA_TYPE_FP16;
    RunAllGatherMesh1DAicpuA5(topoMeta, sendCount, dataType);
}

// ========== 4 Rank测试 ==========
TEST_F(ST_ALL_GATHER_MESH1D_AICPU_TEST, mesh1d_4rank_4kib_int16)
{
    TopoMeta topoMeta{{{0, 1, 2, 3}}};
    u64 sendCount = (4 * 1024) / sizeof(int16_t);
    HcclDataType dataType = HcclDataType::HCCL_DATA_TYPE_INT16;
    RunAllGatherMesh1DAicpuA5(topoMeta, sendCount, dataType);
}

TEST_F(ST_ALL_GATHER_MESH1D_AICPU_TEST, mesh1d_4rank_256kib_fp32)
{
    TopoMeta topoMeta{{{0, 1, 2, 3}}};
    u64 sendCount = (256 * 1024) / sizeof(float);
    HcclDataType dataType = HcclDataType::HCCL_DATA_TYPE_FP32;
    RunAllGatherMesh1DAicpuA5(topoMeta, sendCount, dataType);
}

// ========== 多loop场景（验证skipCclCopy行为，文档8.1节） ==========
// UB_MAX_DATA_SIZE通常为512MB，这里构造超过它的单Rank数据量触发多loop
TEST_F(ST_ALL_GATHER_MESH1D_AICPU_TEST, mesh1d_2rank_large_data_multi_loop_fp32)
{
    TopoMeta topoMeta{{{0, 1}}};
    u64 sendCount = (600 * 1024 * 1024) / sizeof(float);    // 600MB单Rank，触发loopTimes>1
    HcclDataType dataType = HcclDataType::HCCL_DATA_TYPE_FP32;
    RunAllGatherMesh1DAicpuA5(topoMeta, sendCount, dataType);
}
