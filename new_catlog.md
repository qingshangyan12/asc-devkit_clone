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