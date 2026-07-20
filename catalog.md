# AscendC API 目录结构比对

---

## 一、基础API（Basic API）

### 1.1 头文件目录树 `include/basic_api/`

```
include/basic_api/
├── core_mng/
│   └── roc/
│       ├── kernel_operator_cube_group_intf.h      # Cube分组管理
│       └── kernel_operator_group_barrier_intf.h    # 分组Barrier
├── op_frame/
│   └── elemwise_frame.h                            # Elemwise框架
├── reg_compute/
│   ├── kernel_reg_compute_addrreg_intf.h           # 地址寄存器
│   ├── kernel_reg_compute_common_intf.h            # 通用接口
│   ├── kernel_reg_compute_copy_intf.h              # Copy
│   ├── kernel_reg_compute_datacopy_intf.h          # DataCopy
│   ├── kernel_reg_compute_gather_mask_intf.h       # GatherMask
│   ├── kernel_reg_compute_histograms_intf.h        # 直方图
│   ├── kernel_reg_compute_intf.h                   # 寄存器计算入口
│   ├── kernel_reg_compute_maskreg_intf.h           # Mask寄存器
│   ├── kernel_reg_compute_membar_intf.h            # 内存屏障
│   ├── kernel_reg_compute_pack_intf.h              # Pack
│   ├── kernel_reg_compute_struct_intf.h            # 结构体
│   ├── kernel_reg_compute_utils.h                  # 工具函数
│   ├── kernel_reg_compute_vec_arange_intf.h        # arange
│   ├── kernel_reg_compute_vec_binary_intf.h        # 二元计算
│   ├── kernel_reg_compute_vec_binary_scalar_intf.h # 二元标量
│   ├── kernel_reg_compute_vec_cmpsel_intf.h        # 比较选择
│   ├── kernel_reg_compute_vec_duplicate_intf.h     # Duplicate
│   ├── kernel_reg_compute_vec_fused_intf.h         # 融合计算
│   ├── kernel_reg_compute_vec_reduce_intf.h        # Reduce
│   ├── kernel_reg_compute_vec_ternary_scalar_intf.h# 三元标量
│   ├── kernel_reg_compute_vec_unary_intf.h         # 一元计算
│   └── kernel_reg_compute_vec_vconv_intf.h         # 类型转换
├── kernel_base_types.h                             # 基础类型
├── kernel_basic_intf.h                             # 基础接口
├── kernel_common.h                                 # 通用定义
├── kernel_cube_intf.h                              # Cube接口
├── kernel_operator_atomic_intf.h                   # 原子操作
├── kernel_operator_block_sync_intf.h               # Block同步
├── kernel_operator_cache_intf.h                    # 缓存控制
├── kernel_operator_common_intf.h                   # 通用算子
├── kernel_operator_conv2d_intf.h                   # Conv2D
├── kernel_operator_data_copy_intf.h                # DataCopy
├── kernel_operator_determine_compute_sync_intf.h   # 计算同步判定
├── kernel_operator_dump_tensor_intf.h              # Dump Tensor
├── kernel_operator_fixpipe_intf.h                  # FixPipe
├── kernel_operator_gemm_intf.h                     # GEMM
├── kernel_operator_intf.h                          # 总入口
├── kernel_operator_limits_intf.h                   # 数值限制
├── kernel_operator_list_tensor_intf.h              # ListTensor
├── kernel_operator_mm_bitmode_intf.h               # MM BitMode
├── kernel_operator_mm_intf.h                       # 矩阵乘
├── kernel_operator_proposal_intf.h                 # Proposal
├── kernel_operator_scalar_intf.h                   # 标量计算
├── kernel_operator_set_atomic_intf.h               # SetAtomic
├── kernel_operator_swap_mem_intf.h                 # SwapMem
├── kernel_operator_sys_var_intf.h                  # 系统变量
├── kernel_operator_utils_intf.h                    # 工具
├── kernel_operator_vec_bilinearinterpolation_intf.h# 双线性插值
├── kernel_operator_vec_binary_intf.h               # 向量二元
├── kernel_operator_vec_binary_scalar_intf.h        # 向量二元标量
├── kernel_operator_vec_brcb_intf.h                 # BRCB
├── kernel_operator_vec_cmpsel_intf.h               # 比较选择
├── kernel_operator_vec_createvecindex_intf.h       # 创建向量索引
├── kernel_operator_vec_duplicate_intf.h            # Duplicate
├── kernel_operator_vec_gather_intf.h               # Gather
├── kernel_operator_vec_gather_mask_intf.h          # GatherMask
├── kernel_operator_vec_mulcast_intf.h              # MulCast
├── kernel_operator_vec_reduce_intf.h               # Reduce
├── kernel_operator_vec_scatter_intf.h              # Scatter
├── kernel_operator_vec_ternary_scalar_intf.h       # 三元标量
├── kernel_operator_vec_transpose_intf.h            # Transpose
├── kernel_operator_vec_unary_intf.h                # 向量一元
├── kernel_operator_vec_vconv_intf.h                # 类型转换
├── kernel_operator_vec_vpadding_intf.h             # VPadding
├── kernel_prof_trace_intf.h                        # ProfTrace
├── kernel_struct_aipp.h                            # AIPP结构体
├── kernel_struct_binary.h                          # 二元结构体
├── kernel_struct_brcb.h                            # BRCB结构体
├── kernel_struct_conv2d.h                          # Conv2D结构体
├── kernel_struct_data_copy.h                       # DataCopy结构体
├── kernel_struct_fixpipe.h                         # FixPipe结构体
├── kernel_struct_gather.h                          # Gather结构体
├── kernel_struct_mm.h                              # MM结构体
├── kernel_struct_proposal.h                        # Proposal结构体
├── kernel_struct_transpose.h                       # Transpose结构体
├── kernel_struct_unary.h                           # 一元结构体
├── kernel_struct_vdeq.h                            # VDEQ结构体
├── kernel_tensor.h                                 # Tensor
├── kernel_tpipe.h                                  # TPipe
├── kernel_type.h                                   # 类型定义
└── kernel_vec_intf.h                               # 向量接口
```

### 1.2 文档目录树 `docs/zh/api/SIMD-API/基础API/`

```
docs/zh/api/SIMD-API/基础API/
├── 基础API.md
├── 基础API列表.md
├── Cube分组管理（ISASI）/
│   ├── Cube分组管理（ISASI）.md
│   ├── CubeResGroupHandle/
│   │   ├── CubeResGroupHandle.md
│   │   ├── CubeResGroupHandle使用说明.md
│   │   ├── CubeResGroupHandle构造函数.md
│   │   ├── AllocMessage.md
│   │   ├── AssignQueue.md
│   │   ├── CreateCubeResGroup.md
│   │   ├── FreeMessage.md
│   │   ├── PostFakeMsg.md
│   │   ├── PostMessage.md
│   │   ├── SetQuit.md
│   │   ├── SetSkipMsg.md
│   │   └── Wait.md
│   ├── GroupBarrier/
│   │   ├── GroupBarrier.md
│   │   ├── GroupBarrier使用说明.md
│   │   ├── GroupBarrier构造函数.md
│   │   ├── Arrive.md
│   │   ├── GetWorkspaceLen.md
│   │   └── Wait-75.md
│   └── KfcWorkspace/
│       ├── KfcWorkspace.md
│       ├── 构造函数与析构函数.md
│       ├── GetKfcWorkspace.md
│       └── UpdateKfcWorkspace.md
├── Kernel-Tiling/
│   ├── Kernel-Tiling.md
│   ├── COPY_TILING_WITH_ARRAY.md
│   ├── COPY_TILING_WITH_STRUCT.md
│   ├── GET_TILING_DATA.md
│   ├── GET_TILING_DATA_MEMBER.md
│   ├── GET_TILING_DATA_PTR_WITH_STRUCT.md
│   ├── GET_TILING_DATA_WITH_STRUCT.md
│   ├── REGISTER_NONE_TILING.md
│   ├── REGISTER_TILING_DEFAULT.md
│   ├── REGISTER_TILING_FOR_TILINGKEY.md
│   ├── TILING_KEY_IS.md
│   ├── TILING_KEY_LIST.md
│   └── 设置Kernel类型.md
├── Memory矢量计算/
│   ├── Memory矢量计算.md
│   ├── 概述.md
│   ├── 连续计算.md
│   ├── 高维切分.md
│   ├── Vector逻辑架构/
│   │   └── Vector逻辑架构.md
│   ├── SIMD计算说明/
│   │   ├── SIMD计算说明.md
│   │   ├── 概述.md
│   │   ├── 通用说明.md
│   │   └── 掩码/
│   │       ├── 掩码.md
│   │       ├── 接口内设置Mask.md
│   │       └── 接口外设置Mask.md
│   ├── 基础算术/
│   │   ├── 基础算术.md
│   │   ├── 更多样例-9.md
│   │   ├── Abs.md
│   │   ├── Add.md
│   │   ├── Adds.md
│   │   ├── Adds（灵活标量位置）.md
│   │   ├── BilinearInterpolation(ISASI).md
│   │   ├── Div.md
│   │   ├── Divs（灵活标量位置）.md
│   │   ├── Exp.md
│   │   ├── LeakyRelu.md
│   │   ├── Ln.md
│   │   ├── Max.md
│   │   ├── Maxs.md
│   │   ├── Maxs（灵活标量位置）.md
│   │   ├── Min.md
│   │   ├── Mins.md
│   │   ├── Mins（灵活标量位置）.md
│   │   ├── Mull(ISASI).md
│   │   ├── Mul.md
│   │   ├── Muls.md
│   │   ├── Muls（灵活标量位置）.md
│   │   ├── Neg(ISASI).md
│   │   ├── Prelu(ISASI).md
│   │   ├── Reciprocal.md
│   │   ├── Relu.md
│   │   ├── Rsqrt.md
│   │   ├── Sqrt.md
│   │   ├── Sub.md
│   │   └── Subs（灵活标量位置）.md
│   ├── 复合计算/
│   │   ├── 复合计算.md
│   │   ├── AbsSub(ISASI).md
│   │   ├── AddDeqRelu.md
│   │   ├── AddReluCast.md
│   │   ├── AddRelu.md
│   │   ├── Axpy.md
│   │   ├── CastDequant.md
│   │   ├── ExpSub(ISASI).md
│   │   ├── FusedMulAdd.md
│   │   ├── MulAddDst.md
│   │   ├── MulAddRelu.md
│   │   ├── MulCast.md
│   │   ├── MulsCast(ISASI).md
│   │   ├── SubReluCast.md
│   │   └── SubRelu.md
│   ├── 归约计算/
│   │   ├── 归约计算.md
│   │   ├── 概述.md
│   │   ├── Introduction.md
│   │   ├── ReduceDataBlock.md
│   │   ├── ReduceMax.md
│   │   ├── ReduceMin.md
│   │   ├── ReducePairElem.md
│   │   ├── ReduceRepeat.md
│   │   ├── ReduceSum.md
│   │   └── 寄存器辅助接口/
│   │       ├── 寄存器辅助接口.md
│   │       ├── GetReduceRepeatMaxMinSpr(ISASI).md
│   │       └── GetReduceRepeatSumSpr(ISASI).md
│   ├── 排序组合（ISASI）/
│   │   ├── 排序组合（ISASI）.md
│   │   ├── 概述.md
│   │   ├── GetMrgSortResult.md
│   │   ├── MrgSort4.md
│   │   ├── MrgSort.md
│   │   ├── ProposalConcat.md
│   │   ├── ProposalExtract.md
│   │   ├── RpSort16.md
│   │   └── Sort32.md
│   ├── 掩码操作/
│   │   ├── 掩码操作.md
│   │   ├── ResetMask.md
│   │   ├── SetMaskCount.md
│   │   ├── SetMaskNorm.md
│   │   └── SetVectorMask.md
│   ├── 数据填充/
│   │   ├── 数据填充.md
│   │   ├── Brcb.md
│   │   ├── CreateVecIndex.md
│   │   ├── Duplicate.md
│   │   └── VectorPadding(ISASI).md
│   ├── 数据排布转换/
│   │   ├── 数据排布转换.md
│   │   ├── TransDataTo5HD.md
│   │   └── Transpose.md
│   ├── 数据搬运/
│   │   ├── 数据搬运.md
│   │   ├── GM与UB数据搬运/
│   │   │   ├── GM与UB数据搬运.md
│   │   │   ├── GMToUB多维数据搬运NDDMA(DataCopy).md
│   │   │   ├── GMToUB随路转换ND2NZ搬运(DataCopy).md
│   │   │   ├── GMToUB非对齐数据搬运(DataCopyPad).md
│   │   │   ├── GM与UB切片数据搬运(DataCopy).md
│   │   │   ├── GM与UB连续数据搬运(DataCopy).md
│   │   │   ├── GM与UB高维切分数据搬运(DataCopy).md
│   │   │   ├── UBToGM随路转换NZ2ND搬运(DataCopy).md
│   │   │   ├── UBToGM非对齐数据搬运(DataCopyPad).md
│   │   │   └── 寄存器配置说明/
│   │   │       ├── 寄存器配置说明.md
│   │   │       ├── NdDmaDci.md
│   │   │       ├── ResetLoopModePara.md
│   │   │       ├── SetLoopModePara.md
│   │   │       └── SetPadValue(ISASI).md
│   │   └── UB与UB数据搬运/
│   │       ├── UB与UB数据搬运.md
│   │       ├── UBToUB掩码式高维数据搬运(Copy).md
│   │       ├── UBToUB连续数据搬运(Copy).md
│   │       ├── UBToUB连续数据搬运(DataCopy).md
│   │       └── UBToUB高维切分数据搬运(DataCopy).md
│   ├── 数据重排（ISASI）/
│   │   ├── 数据重排（ISASI）.md
│   │   ├── DeInterleave.md
│   │   └── Interleave.md
│   ├── 比较与选择/
│   │   ├── 比较与选择.md
│   │   ├── 概述.md
│   │   ├── Compare.md
│   │   ├── Compares.md
│   │   ├── Compares（灵活标量位置）.md
│   │   ├── Compare（结果存入寄存器）.md
│   │   ├── GatherMask.md
│   │   ├── GetCmpMask(ISASI).md
│   │   ├── Select.md
│   │   ├── Select（灵活标量位置）.md
│   │   └── SetCmpMask(ISASI).md
│   ├── 离散与聚合/
│   │   ├── 离散与聚合.md
│   │   ├── Gatherb(ISASI).md
│   │   ├── Gather.md
│   │   └── Scatter(ISASI).md
│   ├── 类型转换/
│   │   ├── 类型转换.md
│   │   ├── Cast.md
│   │   ├── Truncate(ISASI).md
│   │   └── 寄存器配置说明/
│   │       ├── 寄存器配置说明.md
│   │       └── SetDeqScale.md
│   └── 逻辑计算/
│       ├── 逻辑计算.md
│       ├── And.md
│       ├── Ands.md
│       ├── Not.md
│       ├── Or.md
│       ├── Ors.md
│       ├── ShiftLeft.md
│       ├── ShiftLeft（左移位数为Tensor）.md
│       ├── ShiftRight.md
│       └── ShiftRight（右移位数为Tensor）.md
├── Reg矢量计算/
│   ├── Reg矢量计算.md
│   ├── 概述.md
│   ├── VF调用/
│   │   ├── VF调用.md
│   │   ├── asc_vf_call.md
│   │   └── 关键特性说明.md
│   ├── 同步控制/
│   │   ├── 同步控制.md
│   │   └── LocalMemBar.md
│   ├── 基础算术/
│   │   ├── 基础算术.md
│   │   ├── 概述.md
│   │   ├── Abs.md
│   │   ├── AddC.md
│   │   ├── Add.md
│   │   ├── Adds.md
│   │   ├── Div.md
│   │   ├── Exp.md
│   │   ├── LeakyRelu.md
│   │   ├── Ln.md
│   │   ├── Log10.md
│   │   ├── Log2.md
│   │   ├── Log.md
│   │   ├── Max.md
│   │   ├── Maxs.md
│   │   ├── Min.md
│   │   ├── Mins.md
│   │   ├── Mull.md
│   │   ├── Mul.md
│   │   ├── Muls.md
│   │   ├── Neg.md
│   │   ├── Prelu.md
│   │   ├── Relu.md
│   │   ├── Sqrt.md
│   │   ├── SubC.md
│   │   └── Sub.md
│   ├── 复合计算/
│   │   ├── 复合计算.md
│   │   ├── 概述.md
│   │   ├── AbsSub.md
│   │   ├── Axpy.md
│   │   ├── ExpSub.md
│   │   ├── MulAddDst.md
│   │   ├── MulDstAdd.md
│   │   └── MulsCast.md
│   ├── 寄存器数据类型/
│   │   ├── 寄存器数据类型.md
│   │   ├── AddrReg.md
│   │   ├── MaskReg.md
│   │   ├── RegTensor.md
│   │   └── UnalignRegForLoad-UnalignRegForStore.md
│   ├── 归约计算/
│   │   ├── 归约计算.md
│   │   ├── PairReduceElem.md
│   │   ├── ReduceDataBlock.md
│   │   └── Reduce.md
│   ├── 数据压缩/
│   │   ├── 数据压缩.md
│   │   ├── Pack.md
│   │   ├── UnPack.md
│   │   └── Unsqueeze.md
│   ├── 数据填充/
│   │   ├── 数据填充.md
│   │   └── Duplicate.md
│   ├── 数据重排/
│   │   ├── 数据重排.md
│   │   ├── DeInterleave.md
│   │   └── Interleave.md
│   ├── 比较与选择/
│   │   ├── 比较与选择.md
│   │   ├── 概述.md
│   │   ├── Compare.md
│   │   ├── Compares.md
│   │   ├── Select.md
│   │   └── Squeeze.md
│   ├── 直方图计算/
│   │   ├── 直方图计算.md
│   │   └── Histograms.md
│   ├── 离散操作/
│   │   ├── 离散操作.md
│   │   └── Gather.md
│   ├── 类型转换/
│   │   ├── 类型转换.md
│   │   ├── Cast.md
│   │   └── Truncate.md
│   ├── 系统变量访问/
│   │   ├── 系统变量访问.md
│   │   └── ClearSpr.md
│   ├── 索引操作/
│   │   ├── 索引操作.md
│   │   └── Arange.md
│   ├── 辅助数据类型/
│   │   ├── 辅助数据类型.md
│   │   ├── MaskMergeMode.md
│   │   ├── PostLiteral.md
│   │   ├── RegLayout.md
│   │   ├── RoundMode.md
│   │   └── SatMode.md
│   ├── MaskReg计算/
│   │   ├── MaskReg计算.md
│   │   ├── DeInterleave.md
│   │   ├── Interleave.md
│   │   ├── MoveMask.md
│   │   ├── Move.md
│   │   ├── Pack.md
│   │   ├── Select.md
│   │   └── UnPack.md
│   ├── Reg数据搬入/
│   │   ├── Reg数据搬入.md
│   │   ├── Load.md
│   │   ├── Move.md
│   │   ├── MaskReg搬入（LoadAlign）.md
│   │   ├── MaskReg搬入（MaskGenWithRegTensor）.md
│   │   ├── 连续对齐搬入（LoadAlign）.md
│   │   ├── 连续非对齐搬入（LoadUnAlign）.md
│   │   ├── 非连续对齐搬入（LoadAlign）.md
│   │   ├── 离散搬入（GatherB）.md
│   │   └── 离散搬入（Gather）.md
│   ├── Reg数据搬出/
│   │   ├── Reg数据搬出.md
│   │   ├── Store.md
│   │   ├── MaskReg搬出（StoreAlign）.md
│   │   ├── MaskReg非对齐搬出（StoreUnAlign）.md
│   │   ├── 连续对齐搬出（StoreAlign）.md
│   │   ├── 连续非对齐搬出（StoreUnAlign）.md
│   │   ├── 非连续对齐搬出（StoreAlign）.md
│   │   └── 离散搬出（Scatter）.md
│   └── 逻辑计算/
│       ├── 逻辑计算.md
│       ├── 概述.md
│       ├── And.md
│       ├── Not.md
│       ├── Or.md
│       ├── ShiftLeft.md
│       ├── ShiftLefts.md
│       ├── ShiftRight.md
│       ├── ShiftRights.md
│       └── Xor.md
├── 原子操作/
│   ├── 原子操作.md
│   ├── 原子操作概述.md
│   ├── 关键特性说明.md
│   ├── AtomicAdd.md
│   ├── AtomicCas.md
│   ├── AtomicExch.md
│   ├── AtomicMax.md
│   ├── AtomicMin.md
│   ├── DisableDmaAtomic.md
│   ├── GetStoreAtomicConfig(ISASI).md
│   ├── SetAtomicAdd.md
│   ├── SetAtomicMax(ISASI).md
│   ├── SetAtomicMin(ISASI).md
│   ├── SetAtomicType.md
│   └── SetStoreAtomicConfig(ISASI).md
├── 同步控制/
│   ├── 同步控制.md
│   ├── 系统同步能力概述.md
│   ├── 任务间同步/
│   │   ├── 任务间同步.md
│   │   ├── SetNextTaskStart.md
│   │   └── WaitPreTaskEnd.md
│   ├── 核内同步/
│   │   ├── 核内同步.md
│   │   ├── 核内同步能力概述.md
│   │   ├── 关键特性说明.md
│   │   ├── AllocMutexID-(ISASI).md
│   │   ├── DataSyncBarrier(ISASI).md
│   │   ├── PipeBarrier(ISASI).md
│   │   ├── ReleaseMutexID-(ISASI).md
│   │   ├── SetFlag-WaitFlag(ISASI).md
│   │   ├── Mutex(ISASI)/
│   │   │   ├── Mutex(ISASI).md
│   │   │   ├── 简介.md
│   │   │   ├── Lock.md
│   │   │   └── Unlock.md
│   │   └── TQueSync/
│   │       ├── TQueSync.md
│   │       ├── 模板参数.md
│   │       └── SetFlag-WaitFlag.md
│   └── 核间同步/
│       ├── 核间同步.md
│       ├── 核间同步能力概述.md
│       ├── 关键特性说明.md
│       ├── CrossCoreSetFlag(ISASI).md
│       ├── CrossCoreWaitFlag(ISASI).md
│       ├── IBSet.md
│       ├── IBWait.md
│       ├── InitDetermineComputeWorkspace.md
│       ├── NotifyNextBlock.md
│       ├── SyncAll.md
│       └── WaitPreBlock.md
├── 工具接口/
│   ├── 工具接口.md
│   ├── 执行模式/
│   │   ├── 执行模式.md
│   │   └── Async.md
│   ├── 数学计算/
│   │   ├── 数学计算.md
│   │   ├── CeilDivision.md
│   │   └── NumericLimits/
│   │       ├── NumericLimits.md
│   │       ├── 简介.md
│   │       ├── DeNormMin.md
│   │       ├── Infinity.md
│   │       ├── Lowest.md
│   │       ├── Max.md
│   │       ├── Min.md
│   │       ├── NegativeInfinity.md
│   │       ├── QuietNaN.md
│   │       └── SignalingNaN.md
│   ├── 系统初始化/
│   │   ├── 系统初始化.md
│   │   └── InitSocState.md
│   └── 系统资源与变量/
│       ├── 系统资源与变量.md
│       ├── GetArchVersion.md
│       ├── GetBlockIdx.md
│       ├── GetBlockNum.md
│       ├── GetDataBlockSizeInBytes.md
│       ├── GetProgramCounter(ISASI).md
│       ├── GetRuntimeUBSize.md
│       ├── GetSsbufBaseAddr.md
│       ├── GetSubBlockIdx(ISASI).md
│       ├── GetSubBlockNum(ISASI).md
│       ├── GetSystemCycle(ISASI).md
│       ├── GetTaskRatio.md
│       ├── GetUBSizeInBytes.md
│       └── GetVecLen.md
├── 数据搬运导览/
│   ├── 数据搬运导览.md
│   ├── GM与L1或L0数据搬运.md
│   ├── GM与UB数据搬运.md
│   ├── L1与BiasTable-Buffer数据搬运.md
│   ├── L1与Fixpipe-Buffer数据搬运.md
│   ├── L1与L0数据搬运.md
│   ├── L1或L0C与UB数据搬运.md
│   ├── UB与UB数据搬运.md
│   └── 概览/
│       ├── 概览.md
│       ├── 总体约束说明.md
│       ├── 数据搬运概念.md
│       └── 数据通路.md
├── 数据结构/
│   ├── 数据结构.md
│   ├── 内置数据类型.md
│   ├── precision_conversion.md
│   ├── LocalTensor和GlobalTensor定义/
│   │   ├── LocalTensor和GlobalTensor定义.md
│   │   ├── GlobalTensor/
│   │   │   ├── GlobalTensor.md
│   │   │   ├── GlobalTensor简介.md
│   │   │   ├── GlobalTensor构造函数.md
│   │   │   ├── GetPhyAddr.md
│   │   │   ├── GetShapeInfo.md
│   │   │   ├── GetSize.md
│   │   │   ├── GetValue.md
│   │   │   ├── operator().md
│   │   │   ├── operator.md
│   │   │   ├── ReinterpretCast.md
│   │   │   ├── SetGlobalBuffer.md
│   │   │   ├── SetL2CacheHint.md
│   │   │   ├── SetShapeInfo.md
│   │   │   └── SetValue.md
│   │   └── LocalTensor/
│   │       ├── LocalTensor.md
│   │       ├── LocalTensor简介.md
│   │       ├── LocalTensor构造函数.md
│   │       ├── GetLength.md
│   │       ├── GetPhyAddr.md
│   │       ├── GetPosition.md
│   │       ├── GetShapeInfo.md
│   │       ├── GetSize.md
│   │       ├── GetUserTag.md
│   │       ├── GetValue.md
│   │       ├── operator().md
│   │       ├── operator.md
│   │       ├── Print.md
│   │       ├── ReinterpretCast.md
│   │       ├── SetAddrWithOffset.md
│   │       ├── SetBufferLen.md
│   │       ├── SetShapeInfo.md
│   │       ├── SetSize.md
│   │       ├── SetUserTag.md
│   │       ├── SetValue.md
│   │       └── ToFile.md
│   ├── TensorAPI-BasicStructure/
│   │   ├── TensorAPI-BasicStructure.md
│   │   ├── layout_structure/
│   │   │   ├── layout_structure.md
│   │   │   ├── Layout和层次化表述法.md
│   │   │   ├── Capacity.md
│   │   │   ├── Coord.md
│   │   │   ├── Coshape.md
│   │   │   ├── Cosize.md
│   │   │   ├── Get.md
│   │   │   ├── GetShape.md
│   │   │   ├── GetStride.md
│   │   │   ├── Layout.md
│   │   │   ├── MakeCoord.md
│   │   │   ├── MakeFrameLayout.md
│   │   │   ├── MakeLayout.md
│   │   │   ├── MakePatternLayout.md
│   │   │   ├── MakeShape.md
│   │   │   ├── MakeStride.md
│   │   │   ├── Rank.md
│   │   │   ├── Select.md
│   │   │   ├── Shape.md
│   │   │   ├── Size.md
│   │   │   └── Stride.md
│   │   ├── tensor_structure/
│   │   │   ├── tensor_structure.md
│   │   │   ├── MakeMemPtr.md
│   │   │   ├── MakeTensor.md
│   │   │   ├── Pointer.md
│   │   │   ├── Slice.md
│   │   │   ├── Tensor.md
│   │   │   └── ViewEngine.md
│   │   └── utils/
│   │       ├── tool_functions.md
│   │       └── GetLayoutPattern.md
│   └── 辅助数据结构/
│       ├── 辅助数据结构.md
│       ├── BinaryRepeatParams.md
│       ├── complex32-complex64.md
│       ├── ListTensorDesc.md
│       ├── ShapeInfo.md
│       ├── TPosition.md
│       ├── UnaryRepeatParams.md
│       ├── Coordinate/
│       │   ├── Coordinate.md
│       │   ├── Coordinate简介.md
│       │   ├── Crd2Idx.md
│       │   └── MakeCoord.md
│       ├── Layout/
│       │   ├── Layout.md
│       │   ├── Layout简介.md
│       │   ├── Layout构造函数.md
│       │   ├── 运算符重载.md
│       │   ├── GetShape.md
│       │   ├── GetStride.md
│       │   ├── is_layout.md
│       │   ├── layout1.md
│       │   ├── MakeLayout.md
│       │   ├── MakeShape.md
│       │   └── MakeStride.md
│       ├── TensorDesc/
│       │   ├── TensorDesc.md
│       │   ├── TensorDesc简介.md
│       │   ├── 构造和析构函数.md
│       │   ├── GetDataObj.md
│       │   ├── GetDataPtr.md
│       │   ├── GetDim.md
│       │   ├── GetIndex.md
│       │   ├── GetShape-138.md
│       │   └── SetShapeAddr.md
│       └── TensorTrait/
│           ├── TensorTrait.md
│           ├── TensorTrait简介.md
│           ├── TensorTrait构造函数.md
│           ├── 更多样例.md
│           ├── GetLayout.md
│           ├── is_tensorTrait.md
│           ├── MakeTensorTrait.md
│           └── SetLayout.md
├── 标量计算/
│   ├── 标量计算.md
│   ├── Cast（float转bfloat16_t）.md
│   ├── Cast（float转half-int32_t）.md
│   ├── Cast（多类型转float）.md
│   ├── CountBitsCntSameAsSignBit.md
│   ├── CountLeadingZero.md
│   ├── GetBitCount.md
│   ├── GetSFFValue.md
│   ├── Nop.md
│   ├── ReadGmByPassDCache(ISASI).md
│   └── WriteGmByPassDCache(ISASI).md
├── 特殊寄存器访问/
│   ├── 特殊寄存器访问.md
│   ├── ClearSpr.md
│   ├── GetCtrlSpr(ISASI).md
│   ├── GetSaturationFlag(ISASI).md
│   ├── GetSpr.md
│   ├── ResetCtrlSpr(ISASI).md
│   ├── SetCtrlSpr(ISASI).md
│   └── SetSaturationFlag(ISASI).md
├── 矩阵计算（ISASI）/
│   ├── 矩阵计算（ISASI）.md
│   ├── 概述/
│   │   ├── 概述.md
│   │   ├── 矩阵计算单元.md
│   │   └── 矩阵计算流程.md
│   ├── 矩阵计算分形介绍/
│   │   ├── 矩阵计算分形介绍.md
│   │   ├── 关键分形格式详解.md
│   │   ├── 背景与核心概念.md
│   │   └── 辅助矩阵分形格式详解.md
│   ├── Mmad计算/
│   │   ├── Mmad计算.md
│   │   ├── Conv2D（废弃）.md
│   │   ├── Gemm（废弃）.md
│   │   ├── Mmad.md
│   │   ├── MmadBitMode.md
│   │   ├── MmadMx.md
│   │   ├── MmadWithSparse.md
│   │   ├── 关键特性说明/
│   │   │   ├── 关键特性说明.md
│   │   │   ├── GEMV.md
│   │   │   ├── HF32.md
│   │   │   ├── K-方向对齐约束.md
│   │   │   └── UnitFlag.md
│   │   └── 寄存器配置说明/
│   │       ├── 寄存器配置说明.md
│   │       ├── SetHF32Mode.md
│   │       ├── SetHF32TransMode.md
│   │       └── SetMMColumnMajor-SetMMRowMajor.md
│   ├── 矩阵计算的搬入/
│   │   ├── 矩阵计算的搬入.md
│   │   ├── L1-L0A-B内存结构介绍.md
│   │   ├── 总体说明.md
│   │   ├── 矩阵计算输入搬运约束.md
│   │   ├── 矩阵数据搬入至L0-Buffer/
│   │   │   ├── 矩阵数据搬入至L0-Buffer.md
│   │   │   ├── BroadCastVecToMM(ISASI).md
│   │   │   ├── L1ToBiasTable-Buffer数据搬运（DataCopy）.md
│   │   │   ├── L1ToFixpipe-Buffer数据搬运（DataCopy）.md
│   │   │   ├── LoadData_2D.md
│   │   │   ├── LoadData_2D_BitMode.md
│   │   │   ├── LoadData_2D_MX.md
│   │   │   ├── LoadData_2D_V2.md
│   │   │   ├── LoadData_3D.md
│   │   │   ├── LoadData_3D_BitMode.md
│   │   │   ├── LoadDataWithSparse.md
│   │   │   ├── LoadDataWithStride.md
│   │   │   └── LoadDataWithTranspose.md
│   │   ├── 矩阵数据搬入至L1-Buffer/
│   │   │   ├── 矩阵数据搬入至L1-Buffer.md
│   │   │   ├── GMToL1-2D矩阵搬运（LoadData）.md
│   │   │   ├── GMToL1-2D矩阵搬运V2（LoadData）.md
│   │   │   ├── GMToL1连续数据搬运（DataCopy）.md
│   │   │   ├── GMToL1随路转换-DN2NZ搬运（DataCopy）.md
│   │   │   ├── GMToL1随路转换-ND2NZ搬运（DataCopy）.md
│   │   │   ├── GMToL1非对齐数据搬运（DataCopyPad）.md
│   │   │   ├── GMToL1高维切分数据搬运（DataCopy）.md
│   │   │   ├── UBToL1连续数据搬运（DataCopy）.md
│   │   │   ├── UBToL1随路转换-ND2NZ搬运（DataCopy）.md
│   │   │   ├── UBToL1非对齐数据搬运（DataCopyPad）.md
│   │   │   └── UBToL1高维切分数据搬运（DataCopy）.md
│   │   └── 辅助配置接口/
│   │       ├── 辅助配置接口.md
│   │       ├── Fill.md
│   │       ├── LoadDataUnzip.md
│   │       ├── LoadImageToLocal.md
│   │       ├── LoadUnzipIndex.md
│   │       ├── SetAippFunctions.md
│   │       ├── SetFmatrix.md
│   │       ├── SetFmatrixBitMode.md
│   │       ├── SetLoadDataBoundary.md
│   │       ├── SetLoadDataPaddingValue.md
│   │       ├── SetLoadDataRepeat.md
│   │       └── SetLoadDataRepeatWithStride.md
│   └── 矩阵计算的搬出/
│       ├── 矩阵计算的搬出.md
│       ├── L0C-Buffer内存结构介绍.md
│       ├── 总体说明.md
│       ├── L0C到GM数据搬运（DataCopy）.md
│       ├── L0C到GM数据搬运（Fixpipe）.md
│       ├── L0C到L1数据搬运（DataCopy）.md
│       ├── L0C到L1数据搬运（Fixpipe）.md
│       ├── L0C到UB数据搬运（DataCopy）.md
│       ├── L0C到UB数据搬运（Fixpipe）.md
│       ├── L1到UB数据搬运（DataCopyL1ToUB）.md
│       ├── L1到GM数据搬运/
│       │   ├── L1到GM数据搬运.md
│       │   ├── L1ToGM连续数据搬运（DataCopy）.md
│       │   └── L1ToGM高维切分数据搬运（DataCopy）.md
│       ├── 关键特性说明/
│       │   ├── 关键特性说明.md
│       │   ├── F32-Channel-Split.md
│       │   ├── Int8-Channel-Merge.md
│       │   ├── L0C-Buffer到UB双目标模式.md
│       │   ├── NZ2DN.md
│       │   ├── NZ2ND.md
│       │   ├── NZ2NZ.md
│       │   ├── 卷积格式转换.md
│       │   ├── 随路ReLU.md
│       │   ├── 随路量化.md
│       │   └── 随路量化与随路ReLU场景组合.md
│       └── 寄存器配置说明/
│           ├── 寄存器配置说明.md
│           ├── SetFixPipeAddr.md
│           ├── SetFixPipeClipRelu.md
│           ├── SetFixPipeConfig.md
│           ├── SetFixpipeNz2ndFlag.md
│           └── SetFixpipePreQuantFlag.md
├── 矩阵计算（TensorAPI）/
│   ├── 矩阵计算（TensorAPI）.md
│   ├── 概述/
│   │   ├── 概述.md
│   │   └── 矩阵计算流程.md
│   ├── 矩阵计算分形介绍/
│   │   └── 矩阵计算分形介绍.md
│   ├── Mmad计算/
│   │   ├── Mmad计算.md
│   │   ├── Mmad.md
│   │   ├── 关键特性说明/
│   │   │   ├── 关键特性说明.md
│   │   │   ├── GEMV.md
│   │   │   ├── HF32.md
│   │   │   └── UnitFlag.md
│   │   └── 寄存器配置说明/
│   │       └── 寄存器配置说明.md
│   ├── 矩阵计算的搬入/
│   │   ├── 矩阵计算的搬入.md
│   │   ├── 总体说明.md
│   │   ├── 矩阵数据搬入至L0/
│   │   │   ├── 矩阵数据搬入至L0.md
│   │   │   ├── L1到BiasTable数据搬运（Copy）.md
│   │   │   ├── L1到Fixpipe数据搬运（Copy）.md
│   │   │   ├── L1到L0A数据搬运（Copy）.md
│   │   │   ├── L1到L0B数据搬运（Copy）.md
│   │   │   ├── L1到L0ScaleA数据搬运（Copy）.md
│   │   │   └── L1到L0ScaleB数据搬运（Copy）.md
│   │   └── 矩阵数据搬入至L1/
│   │       ├── 矩阵数据搬入至L1.md
│   │       └── GM到L1数据搬运（Copy）.md
│   └── 矩阵计算的搬出/
│       ├── 矩阵计算的搬出.md
│       ├── 总体说明.md
│       ├── L0C到GM数据搬运（Copy）.md
│       ├── L0C到UB数据搬运（Copy）.md
│       └── 关键特性说明/
│           ├── 关键特性说明.md
│           ├── batch_copy.md
│           ├── f32_channel_split.md
│           ├── int8_channel_merge.md
│           ├── l0c_to_ub_dual_dst.md
│           ├── quant_pre.md
│           └── relu_pre.md
├── 缓存控制/
│   ├── 缓存控制.md
│   ├── 系统缓存概述.md
│   ├── DataCacheCleanAndInvalid.md
│   ├── DataCachePreload.md
│   ├── GetICachePreloadStatus(ISASI).md
│   └── ICachePreLoad(ISASI).md
├── 调试接口/
│   ├── 调试接口.md
│   ├── 关闭ASCENDC_DUMP说明.md
│   ├── CPU孪生调试/
│   │   ├── CPU孪生调试.md
│   │   ├── GmAlloc.md
│   │   ├── GmFree.md
│   │   ├── ICPU_RUN_KF.md
│   │   ├── ICPU_SET_TILING_KEY.md
│   │   └── SetKernelMode.md
│   ├── 上板打印/
│   │   ├── 上板打印.md
│   │   ├── DumpAccChkPoint.md
│   │   ├── DumpTensor.md
│   │   ├── printf.md
│   │   └── PrintTimeStamp.md
│   ├── 异常检测/
│   │   ├── 异常检测.md
│   │   ├── ascendc_assert.md
│   │   ├── assert.md
│   │   ├── CheckLocalMemoryIA(ISASI).md
│   │   └── Trap.md
│   └── 性能统计/
│       ├── 性能统计.md
│       ├── MarkStamp.md
│       ├── MetricsProfStart.md
│       └── MetricsProfStop.md
└── 资源管理/
    ├── 资源管理.md
    ├── Pipe和Que框架/
    │   ├── Pipe和Que框架.md
    │   ├── GetTPipePtr.md
    │   ├── TBuf/
    │   │   ├── TBuf.md
    │   │   ├── TBuf简介.md
    │   │   ├── TBuf构造函数.md
    │   │   ├── Get.md
    │   │   └── GetWithOffset.md
    │   ├── TBufPool/
    │   │   ├── TBufPool.md
    │   │   ├── TBufPool简介.md
    │   │   ├── TBufPool构造函数.md
    │   │   ├── InitBuffer-56.md
    │   │   ├── InitBufPool-55.md
    │   │   └── Reset-57.md
    │   ├── TPipe/
    │   │   ├── TPipe.md
    │   │   ├── TPipe简介.md
    │   │   ├── TPipe构造函数.md
    │   │   ├── AllocEventID.md
    │   │   ├── Destroy.md
    │   │   ├── DestroyWithoutPipeAll.md
    │   │   ├── FetchEventID.md
    │   │   ├── GetBaseAddr.md
    │   │   ├── Init.md
    │   │   ├── InitBuffer.md
    │   │   ├── InitBufPool.md
    │   │   ├── InitSpmBuffer.md
    │   │   ├── ReadSpmBuffer.md
    │   │   ├── ReleaseEventID.md
    │   │   ├── Reset.md
    │   │   └── WriteSpmBuffer.md
    │   ├── TQue/
    │   │   ├── TQue.md
    │   │   ├── TQue简介.md
    │   │   ├── AllocTensor.md
    │   │   ├── DeQue.md
    │   │   ├── EnQue.md
    │   │   ├── FreeTensor.md
    │   │   ├── GetTensorCountInQue.md
    │   │   ├── HasIdleBuffer.md
    │   │   ├── HasTensorInQue.md
    │   │   └── VacantInQue.md
    │   ├── TQueBind/
    │   │   ├── TQueBind.md
    │   │   ├── TQueBind简介.md
    │   │   ├── TQueBind构造函数.md
    │   │   ├── AllocTensor-64.md
    │   │   ├── DeQue-67.md
    │   │   ├── EnQue-66.md
    │   │   ├── FreeAllEvent.md
    │   │   ├── FreeTensor-65.md
    │   │   ├── GetTensorCountInQue-70.md
    │   │   ├── HasIdleBuffer-71.md
    │   │   ├── HasTensorInQue-69.md
    │   │   ├── InitBufHandle.md
    │   │   ├── InitStartBufHandle.md
    │   │   └── VacantInQue-68.md
    │   ├── TSCM/
    │   │   ├── TSCM.md
    │   │   ├── TSCM简介.md
    │   │   ├── AllocTensor-60.md
    │   │   ├── DeQue-63.md
    │   │   ├── EnQue-62.md
    │   │   └── FreeTensor-61.md
    │   └── 自定义TBufPool/
    │       ├── 自定义TBufPool.md
    │       ├── EXTERN_IMPL_BUFPOOL宏.md
    │       ├── GetBufHandle.md
    │       ├── GetCurAddr.md
    │       ├── GetCurBufSize.md
    │       ├── Init-59.md
    │       ├── Reset-58.md
    │       ├── SetCurAddr.md
    │       └── SetCurBufSize.md
    ├── 临时空间管理/
    │   ├── 临时空间管理.md
    │   ├── PopStackBuffer.md
    │   └── workspace/
    │       ├── workspace1.md
    │       ├── GetSysWorkSpacePtr.md
    │       ├── GetUserWorkspace.md
    │       └── SetSysWorkSpace.md
    └── 内存管理/
        ├── 内存管理.md
        └── LocalMemAllocator/
            ├── LocalMemAllocator.md
            ├── LocalMemAllocator简介.md
            ├── LocalMemAllocator构造函数.md
            ├── Alloc.md
            └── GetCurAddr-72.md
```

### 1.3 基础API 统计

| 项目 | 数量 |
|------|------|
| 文档一级目录（功能域） | 16 个 |
| 文档 .md 文件总计 | 约 420+ 个 |
| 头文件总数 | 约 55 个 (.h) |
| 头文件子目录 | 3 个 (core_mng/, op_frame/, reg_compute/) |

---

## 二、C API

### 2.1 头文件目录树 `include/c_api/`

```
include/c_api/
├── asc_simd.h                          # C API 总入口
├── atomic/
│   └── atomic.h                        # 原子操作
├── cache_ctrl/
│   └── cache_ctrl.h                    # 缓存控制
├── cube_compute/
│   └── cube_compute.h                  # Cube 计算
├── cube_datamove/
│   └── cube_datamove.h                 # Cube 数据搬运
├── misc/
│   └── misc.h                          # 杂项/初始化
├── reg_compute/
│   ├── reg_convert.h                   # 类型转换
│   ├── reg_load.h                      # 加载
│   ├── reg_store.h                     # 存储
│   └── reg_vector.h                    # 向量操作
├── scalar_compute/
│   └── scalar_compute.h               # 标量计算
├── sync/
│   └── sync.h                          # 同步控制
├── sys_var/
│   └── sys_var.h                       # 系统变量
├── utils/
│   └── union.h                         # 工具/Union
├── utils_intf.h                        # 工具接口
├── vector_compute/
│   └── vector_compute.h                # 矢量计算
└── vector_datamove/
    └── vector_datamove.h               # 矢量数据搬运
```

### 2.2 文档目录树 `docs/zh/api/SIMD-API/C-API/`

```
docs/zh/api/SIMD-API/C-API/
├── C-API.md
├── README.md
├── 通用说明和约束.md
├── cache_ctrl/
│   ├── 缓存控制.md
│   ├── asc_datacache_preload.md
│   ├── asc_dcci.md
│   ├── asc_dci.md
│   ├── asc_get_icache_preload_status.md
│   └── asc_icache_preload.md
├── cube_compute/
│   ├── 矩阵计算.md
│   ├── asc_enable_fp8.md
│   ├── asc_enable_hf32.md
│   ├── asc_enable_hf32_trans.md
│   ├── asc_enable_hif8.md
│   ├── asc_get_l0c2gm_prequant.md
│   ├── asc_get_l0c2gm_relu.md
│   ├── asc_get_l0c2gm_unitflag.md
│   ├── asc_mmad.md
│   ├── asc_mmad_mx.md
│   ├── asc_mmad_sparse.md
│   ├── asc_set_fp32_mode.md
│   ├── asc_set_l0c2gm_config.md
│   ├── asc_set_l0c2gm_nz2nd.md
│   ├── asc_set_mmad_direction_m.md
│   └── asc_set_mmad_direction_n.md
├── cube_datamove/
│   ├── 矩阵数据搬运.md
│   ├── asc_copy_gm2l0a.md
│   ├── asc_copy_gm2l0b.md
│   ├── asc_copy_gm2l1/
│   │   ├── asc_copy_gm2l1.md
│   │   ├── asc_copy_gm2l1_arch_2201.md
│   │   └── asc_copy_gm2l1_arch_3510.md
│   ├── asc_copy_gm2l1_align.md
│   ├── asc_copy_gm2l1_dn2nz.md
│   ├── asc_copy_gm2l1_nd2nz/
│   │   ├── asc_copy_gm2l1_nd2nz.md
│   │   ├── asc_copy_gm2l1_nd2nz_arch_2201.md
│   │   └── asc_copy_gm2l1_nd2nz_arch_3510.md
│   ├── asc_copy_l0c2gm/
│   │   ├── asc_copy_l0c2gm.md
│   │   ├── asc_copy_l0c2gm_arch_2201.md
│   │   └── asc_copy_l0c2gm_arch_3510.md
│   ├── asc_copy_l0c2l1/
│   │   ├── asc_copy_l0c2l1.md
│   │   ├── asc_copy_l0c2l1_arch_2201.md
│   │   └── asc_copy_l0c2l1_arch_3510.md
│   ├── asc_copy_l0c2ub.md
│   ├── asc_copy_l12bt/
│   │   ├── asc_copy_l12bt.md
│   │   ├── asc_copy_l12bt_arch_2201.md
│   │   └── asc_copy_l12bt_arch_3510.md
│   ├── asc_copy_l12fb/
│   │   ├── asc_copy_l12fb.md
│   │   ├── asc_copy_l12fb_arch_2201.md
│   │   └── asc_copy_l12fb_arch_3510.md
│   ├── asc_copy_l12gm.md
│   ├── asc_copy_l12l0a/
│   │   ├── asc_copy_l12l0a.md
│   │   ├── asc_copy_l12l0a_arch_2201.md
│   │   └── asc_copy_l12l0a_arch_3510.md
│   ├── asc_copy_l12l0a_mx.md
│   ├── asc_copy_l12l0a_trans.md
│   ├── asc_copy_l12l0b/
│   │   ├── asc_copy_l12l0b.md
│   │   ├── asc_copy_l12l0b_arch_2201.md
│   │   └── asc_copy_l12l0b_arch_3510.md
│   ├── asc_copy_l12l0b_mx.md
│   ├── asc_copy_l12l0b_sparse.md
│   ├── asc_copy_l12l0b_trans/
│   │   ├── asc_copy_l12l0b_trans.md
│   │   ├── asc_copy_l12l0b_trans_arch_2201.md
│   │   └── asc_copy_l12l0b_trans_arch_3510.md
│   ├── asc_copy_l12l0c.md
│   ├── asc_copy_l12ub.md
│   ├── asc_fill_l0a.md
│   ├── asc_fill_l0b.md
│   ├── asc_fill_l1.md
│   ├── asc_load_image_to_cbuf.md
│   ├── asc_set_gm2l1_loop1_stride.md
│   ├── asc_set_gm2l1_loop2_stride.md
│   ├── asc_set_gm2l1_loop_size.md
│   ├── asc_set_gm2l1_pad.md
│   ├── asc_set_l0c2gm_lrelu_alpha.md
│   ├── asc_set_l0c_copy_params.md
│   ├── asc_set_l0c_copy_prequant.md
│   ├── asc_set_l13d_fmatrix_b.md
│   ├── asc_set_l13d_fmatrix.md
│   ├── asc_set_l13d_padding.md
│   ├── asc_set_l13d_rpt.md
│   └── asc_set_l13d_size.md
├── misc/
│   ├── 其他操作.md
│   └── asc_init.md
├── reg/
│   ├── reg.md
│   ├── arithmetic_compute/
│   │   ├── arithmetic_compute.md
│   │   ├── asc_abs.md
│   │   ├── asc_add.md
│   │   ├── asc_addc.md
│   │   ├── asc_add_scalar.md
│   │   ├── asc_div.md
│   │   ├── asc_exp.md
│   │   ├── asc_leakyrelu.md
│   │   ├── asc_ln.md
│   │   ├── asc_max.md
│   │   ├── asc_max_scalar.md
│   │   ├── asc_min.md
│   │   ├── asc_min_scalar.md
│   │   ├── asc_mul.md
│   │   ├── asc_mull.md
│   │   ├── asc_mul_scalar.md
│   │   ├── asc_muls_deprecated.md
│   │   ├── asc_neg.md
│   │   ├── asc_prelu.md
│   │   ├── asc_relu.md
│   │   ├── asc_sqrt.md
│   │   ├── asc_sub.md
│   │   └── asc_subc.md
│   ├── compare_and_select/
│   │   ├── compare_and_select.md
│   │   ├── asc_eq.md
│   │   ├── asc_eq_scalar.md
│   │   ├── asc_ge.md
│   │   ├── asc_ge_scalar.md
│   │   ├── asc_gt.md
│   │   ├── asc_gt_scalar.md
│   │   ├── asc_le.md
│   │   ├── asc_le_scalar.md
│   │   ├── asc_lt.md
│   │   ├── asc_lt_scalar.md
│   │   ├── asc_ne.md
│   │   ├── asc_ne_scalar.md
│   │   ├── asc_select.md
│   │   ├── asc_squeeze.md
│   │   ├── asc_squeeze_deprecated.md
│   │   └── asc_squeeze_with_status.md
│   ├── compound_compute/
│   │   ├── compound_compute.md
│   │   ├── asc_abs_sub.md
│   │   ├── asc_axpy.md
│   │   ├── asc_exp_sub_deprecated.md
│   │   └── asc_madd.md
│   ├── data_compress/
│   │   ├── data_compress.md
│   │   ├── asc_pack_deprecated.md
│   │   ├── asc_unpack.md
│   │   └── asc_unsqueeze.md
│   ├── data_fill/
│   │   ├── data_fill.md
│   │   ├── asc_duplicate.md
│   │   └── asc_duplicate_scalar.md
│   ├── data_rearrange/
│   │   ├── data_rearrange.md
│   │   ├── asc_deintlv.md
│   │   └── asc_intlv.md
│   ├── data_type_convert/
│   │   ├── data_type_convert.md
│   │   ├── rounding_mode.md
│   │   ├── asc_bfloat162e1m2x2_deprecated.md
│   │   ├── asc_bfloat162e2m1x2_deprecated.md
│   │   ├── asc_bfloat162float.md
│   │   ├── asc_bfloat162half.md
│   │   ├── asc_bfloat162int32.md
│   │   ├── asc_e1m2x22bfloat16_deprecated.md
│   │   ├── asc_e2m1x22bfloat16_deprecated.md
│   │   ├── asc_e4m32float_deprecated.md
│   │   ├── asc_e5m22float_deprecated.md
│   │   ├── asc_float2bfloat16.md
│   │   ├── asc_float2e4m3_deprecated.md
│   │   ├── asc_float2e5m2_deprecated.md
│   │   ├── asc_float2half.md
│   │   ├── asc_float2hif8_deprecated.md
│   │   ├── asc_float2int16.md
│   │   ├── asc_float2int32.md
│   │   ├── asc_float2int64.md
│   │   ├── asc_half2bfloat16.md
│   │   ├── asc_half2float.md
│   │   ├── asc_half2hif8.md
│   │   ├── asc_half2int16.md
│   │   ├── asc_half2int32.md
│   │   ├── asc_half2int4x2_deprecated.md
│   │   ├── asc_half2int8.md
│   │   ├── asc_half2uint8.md
│   │   ├── asc_hif82float_deprecated.md
│   │   ├── asc_hif82half.md
│   │   ├── asc_int162float.md
│   │   ├── asc_int162half.md
│   │   ├── asc_int162int32.md
│   │   ├── asc_int162uint32.md
│   │   ├── asc_int162uint8.md
│   │   ├── asc_int322float.md
│   │   ├── asc_int322int16.md
│   │   ├── asc_int322int64.md
│   │   ├── asc_int322uint16.md
│   │   ├── asc_int322uint8_deprecated.md
│   │   ├── asc_int4x22bfloat16_deprecated.md
│   │   ├── asc_int4x22half_deprecated.md
│   │   ├── asc_int4x22int16_deprecated.md
│   │   ├── asc_int642float.md
│   │   ├── asc_int642int32.md
│   │   ├── asc_int82half.md
│   │   ├── asc_int82int16.md
│   │   ├── asc_int82int32_deprecated.md
│   │   ├── asc_truncate.md
│   │   ├── asc_uint162uint32.md
│   │   ├── asc_uint162uint8.md
│   │   ├── asc_uint322int16.md
│   │   ├── asc_uint322uint16.md
│   │   ├── asc_uint322uint8_deprecated.md
│   │   ├── asc_uint82half.md
│   │   ├── asc_uint82uint16.md
│   │   └── asc_uint82uint32_deprecated.md
│   ├── histogram_compute/
│   │   ├── histogram_compute.md
│   │   ├── asc_cumulative_histogram.md
│   │   └── asc_frequency_histogram.md
│   ├── index_operate/
│   │   ├── index_operate.md
│   │   └── asc_arange.md
│   ├── logic_compute/
│   │   ├── logic_compute.md
│   │   ├── asc_and.md
│   │   ├── asc_not.md
│   │   ├── asc_or.md
│   │   ├── asc_shiftleft.md
│   │   ├── asc_shiftleft_scalar.md
│   │   ├── asc_shiftright.md
│   │   ├── asc_shiftright_scalar.md
│   │   └── asc_xor.md
│   ├── reduce_compute/
│   │   ├── reduce_compute.md
│   │   ├── asc_pair_reduce_sum.md
│   │   ├── asc_reduce_max.md
│   │   ├── asc_reduce_max_datablock.md
│   │   ├── asc_reduce_min.md
│   │   ├── asc_reduce_min_datablock.md
│   │   ├── asc_reduce_sum.md
│   │   └── asc_reduce_sum_datablock.md
│   ├── reg_data_types/
│   │   ├── reg_data_types.md
│   │   ├── data_type_definition.md
│   │   ├── asc_create_iter_reg_deprecated.md
│   │   ├── asc_create_mask.md
│   │   ├── asc_get_mask_spr.md
│   │   ├── asc_update_addr_reg.md
│   │   └── asc_update_mask.md
│   ├── reg_load/
│   │   ├── reg_load.md
│   │   ├── asc_copy.md
│   │   ├── asc_gather.md
│   │   ├── asc_gather_datablock.md
│   │   ├── asc_load.md
│   │   ├── asc_loadalign/
│   │   │   ├── asc_loadalign.md
│   │   │   ├── asc_loadalign_v1_deprecated.md
│   │   │   ├── asc_loadalign_v2_deprecated.md
│   │   │   ├── asc_loadalign_v3_deprecated.md
│   │   │   ├── asc_loadalign_v4_deprecated.md
│   │   │   ├── asc_loadalign_v5_deprecated.md
│   │   │   └── asc_loadalign_v6_deprecated.md
│   │   ├── asc_loadalign.md
│   │   ├── asc_loadalign_postupdate.md
│   │   ├── asc_loadalign_brc_datablock.md
│   │   ├── asc_loadalign_brc_datablock_postupdate.md
│   │   ├── asc_loadalign_brc_elem.md
│   │   ├── asc_loadalign_brc_elem_postupdate.md
│   │   ├── asc_loadalign_brc_elem2datablock.md
│   │   ├── asc_loadalign_brc_elem2datablock_postupdate.md
│   │   ├── asc_loadalign_deintlv.md
│   │   ├── asc_loadalign_deintlv_postupdate.md
│   │   ├── asc_loadalign_downsample.md
│   │   ├── asc_loadalign_downsample_postupdate.md
│   │   ├── asc_loadalign_unpack.md
│   │   ├── asc_loadalign_unpack_postupdate.md
│   │   ├── asc_loadalign_unpack4.md
│   │   ├── asc_loadalign_unpack4_postupdate.md
│   │   ├── asc_loadalign_upsample.md
│   │   ├── asc_loadalign_upsample_postupdate.md
│   │   ├── asc_loadunalign.md
│   │   ├── asc_loadunalign_postupdate.md
│   │   └── asc_loadunalign_pre.md
│   ├── reg_store/
│   │   ├── reg_store.md
│   │   ├── asc_scatter.md
│   │   ├── asc_scatter_deprecated.md
│   │   ├── asc_store.md
│   │   ├── asc_storealign/
│   │   │   ├── asc_storealign.md
│   │   │   ├── asc_storealign_v1_deprecated.md
│   │   │   ├── asc_storealign_v2_deprecated.md
│   │   │   ├── asc_storealign_v3_deprecated.md
│   │   │   ├── asc_storealign_v4_deprecated.md
│   │   │   └── asc_storealign_v5_deprecated.md
│   │   ├── asc_storealign.md
│   │   ├── asc_storealign_postupdate.md
│   │   ├── asc_storealign_1st.md
│   │   ├── asc_storealign_1st_postupdate.md
│   │   ├── asc_storealign_intlv.md
│   │   ├── asc_storealign_pack.md
│   │   ├── asc_storealign_pack_postupdate.md
│   │   ├── asc_storealign_pack_quarter.md
│   │   ├── asc_storealign_pack_quarter_postupdate.md
│   │   ├── asc_storeunalign.md
│   │   ├── asc_storeunalign_postupdate.md
│   │   ├── asc_storeunalign_post.md
│   │   └── asc_storeunalign_post_postupdate.md
│   ├── sync_control/
│   │   ├── sync_control.md
│   │   └── asc_mem_bar.md
│   └── system_variables_access/
│       ├── system_variables_access.md
│       └── asc_clear_ar_spr.md
├── scalar_compute/
│   ├── 标量计算.md
│   ├── asc_atomic_add.md
│   ├── asc_atomic_and.md
│   ├── asc_atomic_cas.md
│   ├── asc_atomic_dec.md
│   ├── asc_atomic_exch.md
│   ├── asc_atomic_inc.md
│   ├── asc_atomic_max.md
│   ├── asc_atomic_min.md
│   ├── asc_atomic_or.md
│   ├── asc_atomic_sub.md
│   ├── asc_atomic_xor.md
│   ├── asc_clear_nthbit.md
│   ├── asc_clz.md
│   ├── asc_ffs.md
│   ├── asc_ffz.md
│   ├── asc_float2int32.md
│   ├── asc_popc.md
│   ├── asc_set_nthbit.md
│   ├── asc_sflbits.md
│   ├── asc_store_dev.md
│   └── asc_zero_bits_cnt.md
├── simd_atomic/
│   ├── 原子操作.md
│   ├── asc_get_store_atomic_config.md
│   ├── asc_set_atomic_add.md
│   ├── asc_set_atomic_max.md
│   ├── asc_set_atomic_min.md
│   ├── asc_set_atomic_none.md
│   ├── asc_set_store_atomic_config_v1.md
│   └── asc_set_store_atomic_config_v2_deprecated.md
├── struct/
│   ├── 数据结构.md
│   ├── asc_fill_value_config.md
│   ├── asc_l13d_fmatrix_config.md
│   ├── asc_load3d_v2_config.md
│   ├── asc_ndim_pad_count_config.md
│   └── asc_store_atomic_config.md
├── sync/
│   ├── 同步控制.md
│   ├── asc_lock.md
│   ├── asc_sync.md
│   ├── asc_sync_block_arrive.md
│   ├── asc_sync_block_wait.md
│   ├── asc_sync_data_barrier.md
│   ├── asc_sync_inter_arrive.md
│   ├── asc_sync_inter_wait.md
│   ├── asc_sync_intra_arrive.md
│   ├── asc_sync_intra_wait.md
│   ├── asc_sync_mte2.md
│   ├── asc_sync_mte3.md
│   ├── asc_sync_notify.md
│   ├── asc_sync_pipe.md
│   ├── asc_sync_subblock_arrive.md
│   ├── asc_sync_subblock_wait.md
│   ├── asc_sync_vec.md
│   ├── asc_sync_wait.md
│   └── asc_unlock.md
├── sys_var/
│   ├── 系统变量.md
│   ├── asc_get_arch_ver.md
│   ├── asc_get_ar_spr_deprecated.md
│   ├── asc_get_block_idx.md
│   ├── asc_get_block_num.md
│   ├── asc_get_core_id.md
│   ├── asc_get_ctrl.md
│   ├── asc_get_ffts_base_addr.md
│   ├── asc_get_phy_buf_addr.md
│   ├── asc_get_phy_stack_base.md
│   ├── asc_get_program_counter.md
│   ├── asc_get_smmu_tag_version.md
│   ├── asc_get_squeeze_status.md
│   ├── asc_get_status.md
│   ├── asc_get_sub_block_id.md
│   ├── asc_get_sub_block_num.md
│   ├── asc_get_system_cycle.md
│   ├── asc_get_sys_virtual_base.md
│   ├── asc_get_vf_len.md
│   ├── asc_set_ctrl.md
│   ├── asc_set_ffts_base_addr.md
│   ├── asc_set_gm2l1_nz_para.md
│   ├── asc_set_l0c2gm_channel_para.md
│   ├── asc_set_l0c2gm_quant_post.md
│   ├── asc_set_l0c2gm_relu_alpha.md
│   ├── asc_set_l12l0_padding_val.md
│   └── asc_set_l3d_rpt_b.md
├── vector_compute/
│   ├── Memory矢量计算.md
│   ├── asc_abs.md
│   ├── asc_add.md
│   ├── asc_add_relu.md
│   ├── asc_add_scalar.md
│   ├── asc_and.md
│   ├── asc_axpy.md
│   ├── asc_bfloat162float.md
│   ├── asc_bfloat162int32.md
│   ├── asc_bitsort.md
│   ├── asc_brcb.md
│   ├── asc_copy.md
│   ├── asc_datablock_reduce_max.md
│   ├── asc_datablock_reduce_min.md
│   ├── asc_datablock_reduce_sum.md
│   ├── asc_deq_int162b8.md
│   ├── asc_deq_int322half.md
│   ├── asc_div.md
│   ├── asc_duplicate.md
│   ├── asc_eq.md
│   ├── asc_eq_scalar.md
│   ├── asc_exp.md
│   ├── asc_float2bfloat16.md
│   ├── asc_float2float.md
│   ├── asc_float2half.md
│   ├── asc_float2int16.md
│   ├── asc_float2int32.md
│   ├── asc_float2int64.md
│   ├── asc_fma.md
│   ├── asc_gather.md
│   ├── asc_gather_datablock.md
│   ├── asc_ge.md
│   ├── asc_ge_scalar.md
│   ├── asc_get_cmp_mask.md
│   ├── asc_get_reduce_max_cnt.md
│   ├── asc_get_reduce_min_cnt.md
│   ├── asc_get_rsvd_count.md
│   ├── asc_get_vms4_sr.md
│   ├── asc_gt.md
│   ├── asc_gt_scalar.md
│   ├── asc_half2float.md
│   ├── asc_half2int16.md
│   ├── asc_half2int32.md
│   ├── asc_half2int4.md
│   ├── asc_half2int8.md
│   ├── asc_half2uint8.md
│   ├── asc_int162float.md
│   ├── asc_int162half.md
│   ├── asc_int322float.md
│   ├── asc_int322int16.md
│   ├── asc_int322int64.md
│   ├── asc_int42half.md
│   ├── asc_int642float.md
│   ├── asc_int642int32.md
│   ├── asc_int82half.md
│   ├── asc_le.md
│   ├── asc_le_scalar.md
│   ├── asc_leakyrelu.md
│   ├── asc_log.md
│   ├── asc_lt.md
│   ├── asc_lt_scalar.md
│   ├── asc_max.md
│   ├── asc_max_scalar.md
│   ├── asc_min.md
│   ├── asc_min_scalar.md
│   ├── asc_mrgsort4.md
│   ├── asc_mul.md
│   ├── asc_mul_add.md
│   ├── asc_mul_add_relu.md
│   ├── asc_mul_cast_half2int8.md
│   ├── asc_mul_cast_half2uint8.md
│   ├── asc_mul_scalar.md
│   ├── asc_ne.md
│   ├── asc_ne_scalar.md
│   ├── asc_not.md
│   ├── asc_or.md
│   ├── asc_pair_reduce_sum.md
│   ├── asc_rcp.md
│   ├── asc_reduce.md
│   ├── asc_relu.md
│   ├── asc_repeat_reduce_max.md
│   ├── asc_repeat_reduce_min.md
│   ├── asc_repeat_reduce_sum.md
│   ├── asc_rsqrt.md
│   ├── asc_select.md
│   ├── asc_set_cmp_mask.md
│   ├── asc_set_deq_scale.md
│   ├── asc_set_mask_count.md
│   ├── asc_set_mask_norm.md
│   ├── asc_set_va_reg.md
│   ├── asc_set_vector_mask.md
│   ├── asc_shiftleft.md
│   ├── asc_shiftright.md
│   ├── asc_sqrt.md
│   ├── asc_squeeze.md
│   ├── asc_sub.md
│   ├── asc_sub_relu.md
│   ├── asc_sub_scalar.md
│   ├── asc_transpose.md
│   ├── asc_transto5hd.md
│   ├── asc_uint82half.md
│   └── asc_vdeq_int162b8.md
└── vector_datamove/
    ├── 矢量数据搬运.md
    ├── asc_copy_gm2ub/
    │   ├── asc_copy_gm2ub.md
    │   ├── asc_copy_gm2ub_arch_2201.md
    │   └── asc_copy_gm2ub_arch_3510.md
    ├── asc_copy_gm2ub_align/
    │   ├── asc_copy_gm2ub_align.md
    │   ├── asc_copy_gm2ub_align_arch_2201.md
    │   └── asc_copy_gm2ub_align_arch_3510.md
    ├── asc_copy_ub2gm/
    │   ├── asc_copy_ub2gm.md
    │   ├── asc_copy_ub2gm_arch_2201.md
    │   └── asc_copy_ub2gm_arch_3510.md
    ├── asc_copy_ub2gm_align/
    │   ├── asc_copy_ub2gm_align.md
    │   ├── asc_copy_ub2gm_align_arch_2201.md
    │   └── asc_copy_ub2gm_align_arch_3510.md
    ├── asc_copy_ub2l1.md
    ├── asc_copy_ub2ub.md
    ├── asc_ndim_copy_gm2ub.md
    ├── asc_set_copy_pad_val.md
    ├── asc_set_gm2ub_loop1_stride.md
    ├── asc_set_gm2ub_loop2_stride.md
    ├── asc_set_gm2ub_loop_size.md
    ├── asc_set_gm2ub_pad_deprecated.md
    ├── asc_set_ndim_loop_stride.md
    ├── asc_set_ndim_pad_count.md
    ├── asc_set_ndim_pad_value.md
    ├── asc_set_ub2gm_loop1_stride.md
    ├── asc_set_ub2gm_loop2_stride.md
    └── asc_set_ub2gm_loop_size.md
```

### 2.3 C API 统计

| 项目 | 数量 |
|------|------|
| 文档一级目录（功能域） | 12 个 (不含 figures) |
| 文档 .md 文件总计 | 约 280+ 个 |
| 头文件总数 | 约 18 个 (.h) |
| 头文件子目录 | 11 个 |

---

## 三、头文件 vs 文档 分类比对

### 3.1 C API：头文件与文档一级目录映射

| 头文件目录 | 文档目录 | 命名一致性 | 备注 |
|-----------|---------|:---:|------|
| `atomic/` | `simd_atomic/` | ❌ | 命名不一致：头文件 `atomic`，文档 `simd_atomic` |
| `cache_ctrl/` | `cache_ctrl/` | ✅ | |
| `cube_compute/` | `cube_compute/` | ✅ | |
| `cube_datamove/` | `cube_datamove/` | ✅ | |
| `misc/` | `misc/` | ✅ | |
| `reg_compute/` | `reg/` | ❌ | 命名不一致：头文件 `reg_compute`，文档 `reg` |
| `scalar_compute/` | `scalar_compute/` | ✅ | |
| `sync/` | `sync/` | ✅ | |
| `sys_var/` | `sys_var/` | ✅ | |
| `utils/` | — | ❌ | 文档无独立 `utils/` 章节 |
| `vector_compute/` | `vector_compute/` | ✅ | |
| `vector_datamove/` | `vector_datamove/` | ✅ | |
| — | `struct/` | ❌ | 文档有，头文件无对应目录（结构体分散在各模块 .h 中） |
| — | `figures/` | — | 图片资源，非功能目录 |

### 3.2 基础 API：头文件 vs 文档功能域映射

| 文档一级目录（16个） | 对应头文件 | 对应状态 |
|---------------------|-----------|:---:|
| `Cube分组管理（ISASI）/` | `core_mng/` | ✅ |
| `Kernel-Tiling/` | kernel 基类 + tiling 相关宏（无独立头文件） | ✅ |
| `Memory矢量计算/` | `kernel_operator_vec_*.h` 系列 (+ `kernel_vec_intf.h`) | ✅ |
| `Reg矢量计算/` | `reg_compute/` 目录 | ✅ |
| `原子操作/` | `kernel_operator_atomic_intf.h` + `kernel_operator_set_atomic_intf.h` | ✅ |
| `同步控制/` | `kernel_operator_block_sync_intf.h` + `kernel_operator_determine_compute_sync_intf.h` | ✅ |
| `工具接口/` | `kernel_operator_utils_intf.h` + `kernel_operator_limits_intf.h` | ✅ |
| `数据搬运导览/` | `kernel_operator_data_copy_intf.h` | ✅ |
| `数据结构/` | `kernel_tensor.h` + `kernel_type.h` + `kernel_base_types.h` + 各 struct 头文件 | ✅ |
| `标量计算/` | `kernel_operator_scalar_intf.h` | ✅ |
| `特殊寄存器访问/` | `kernel_operator_sys_var_intf.h` | ✅ |
| `矩阵计算（ISASI）/` | `kernel_cube_intf.h` + `kernel_operator_mm_intf.h` + `kernel_operator_gemm_intf.h` + `kernel_operator_mm_bitmode_intf.h` + `kernel_operator_conv2d_intf.h` + `kernel_operator_fixpipe_intf.h` | ✅ |
| `矩阵计算（TensorAPI）/` | 同上 + `kernel_struct_*.h` | ✅ |
| `缓存控制/` | `kernel_operator_cache_intf.h` | ✅ |
| `调试接口/` | `kernel_operator_dump_tensor_intf.h` + `kernel_prof_trace_intf.h` | ✅ |
| `资源管理/` | `kernel_tpipe.h` | ✅ |
| — | `op_frame/elemwise_frame.h` | ❌ 缺文档 |

### 3.3 C API `reg/` 文档子模块展开 vs 头文件

C API 文档 `reg/` 下有 14 个子模块，但头文件 `reg_compute/` 仅 4 个文件：

| 文档 reg/ 子模块 | 头文件 reg_compute/ | 粒度对比 |
|------------------|---------------------|---------|
| `arithmetic_compute/` | `reg_vector.h` | 文档更细则 |
| `compare_and_select/` | `reg_vector.h` | |
| `compound_compute/` | `reg_vector.h` | |
| `data_compress/` | `reg_vector.h` | |
| `data_fill/` | `reg_vector.h` | |
| `data_rearrange/` | `reg_vector.h` | |
| `data_type_convert/` | `reg_convert.h` | |
| `histogram_compute/` | `reg_vector.h` | |
| `index_operate/` | `reg_vector.h` | |
| `logic_compute/` | `reg_vector.h` | |
| `reduce_compute/` | `reg_vector.h` | |
| `reg_data_types/` | 无独立头文件 | |
| `reg_load/` | `reg_load.h` | |
| `reg_store/` | `reg_store.h` | |
| `sync_control/` | 无独立头文件 | |
| `system_variables_access/` | 无独立头文件 | |

### 3.4 总体评估与问题总结

| 问题 | 分类 | 严重度 |
|------|------|:---:|
| `atomic` vs `simd_atomic` 命名不一致 | C API 头文件 vs 文档 | ⚠️ 中 |
| `reg_compute` vs `reg` 命名不一致 | C API 头文件 vs 文档 | ⚠️ 中 |
| 头文件 `utils/` 无对应文档章节 | C API 文档缺失 | ⚠️ 中 |
| 文档 `struct/` 无对应头文件目录 | C API 头文件缺失 | 🔵 低 |
| 头文件 `op_frame/` 无对应文档 | 基础 API 文档缺失 | ⚠️ 中 |
| 基础 API 头文件扁平化（~50文件）vs 文档层级化（16 域） | 组织粒度差异 | 🔵 低 |
| C API `reg/` 文档 14 子模块 vs 头文件 4 文件 | 粒度差异 | 🔵 低 |
| C API `scalar_compute/` 文档含大量原子操作函数 | 分类交叉 | 🔵 低 |

**结论**：基础 API 整体对齐度较好，主要缺口是 `op_frame/` 缺少文档。C API 存在 2 处命名不一致（`reg_compute`/`reg`、`atomic`/`simd_atomic`）和 `utils/` 头文件无对应文档的问题；文档 `struct/` 结构体在头文件中无独立目录也需关注。
