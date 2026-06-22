# 基础API列表

## 基础数据结构

| 接口名 | 功能描述 |
| --- | --- |
| [TPosition](数据结构/辅助数据结构/TPosition.md) |  |
| [LocalTensor](数据结构/LocalTensor和GlobalTensor定义/LocalTensor/LocalTensor.md) | LocalTensor用于存放AI Core中Local Memory（内部存储）的数据，支持逻辑位置TPosition为VECIN、VECOUT、VECCALC、A1、A2、B1、B2、CO1、CO2。 |
| [GlobalTensor](数据结构/LocalTensor和GlobalTensor定义/GlobalTensor/GlobalTensor.md) | GlobalTensor用来存放Global Memory（外部存储）的全局数据。 |
| [Coordinate](数据结构/辅助数据结构/Coordinate/Coordinate.md) | Coordinate本质上是一个元组（tuple），用于表示张量在不同维度的位置信息，即坐标值。 |
| [Layout](数据结构/辅助数据结构/Layout/Layout.md) | Layout数据结构是描述多维张量内存布局的基础模板类，通过编译时的形状（Shape）和步长（Stride）信息，实现逻辑坐标空间到一维内存地址空间的映射。 |
| [TensorTrait](数据结构/辅助数据结构/TensorTrait/TensorTrait.md) | TensorTrait数据结构是描述Tensor相关信息的基础模板类，包含Tensor的数据类型、逻辑位置和Layout内存布局。 |

## Memory矢量计算

### 基础算术
| 接口名 | 功能描述 |
| --- | --- |
| [BilinearInterpolation(ISASI)](Memory矢量计算/基础算术/BilinearInterpolation(ISASI).md) |  |
| [Mull(ISASI)](Memory矢量计算/基础算术/Mull(ISASI).md) |  |
| [Prelu(ISASI)](Memory矢量计算/基础算术/Prelu(ISASI).md) |  |
| [Abs](Memory矢量计算/基础算术/Abs.md) |  |
| [Add](Memory矢量计算/基础算术/Add.md) |  |
| [Adds](Memory矢量计算/基础算术/Adds.md) |  |
| [Adds（灵活标量位置）](Memory矢量计算/基础算术/Adds（灵活标量位置）.md) |  |
| [Div](Memory矢量计算/基础算术/Div.md) |  |
| [Divs](Memory矢量计算/基础算术/Divs.md) |  |
| [Exp](Memory矢量计算/基础算术/Exp.md) |  |
| [LeakyRelu](Memory矢量计算/基础算术/LeakyRelu.md) |  |
| [Ln](Memory矢量计算/基础算术/Ln.md) |  |
| [Max](Memory矢量计算/基础算术/Max.md) |  |
| [Maxs](Memory矢量计算/基础算术/Maxs.md) |  |
| [Maxs（灵活标量位置）](Memory矢量计算/基础算术/Maxs（灵活标量位置）.md) |  |
| [Min](Memory矢量计算/基础算术/Min.md) |  |
| [Mins](Memory矢量计算/基础算术/Mins.md) |  |
| [Mins（灵活标量位置）](Memory矢量计算/基础算术/Mins（灵活标量位置）.md) |  |
| [Mul](Memory矢量计算/基础算术/Mul.md) |  |
| [Muls](Memory矢量计算/基础算术/Muls.md) |  |
| [Muls（灵活标量位置）](Memory矢量计算/基础算术/Muls（灵活标量位置）.md) |  |
| [Neg(ISASI)](Memory矢量计算/基础算术/Neg(ISASI).md) |  |
| [Reciprocal](Memory矢量计算/基础算术/Reciprocal.md) |  |
| [Relu](Memory矢量计算/基础算术/Relu.md) |  |
| [Rsqrt](Memory矢量计算/基础算术/Rsqrt.md) |  |
| [Sqrt](Memory矢量计算/基础算术/Sqrt.md) |  |
| [Sub](Memory矢量计算/基础算术/Sub.md) |  |
| [Subs](Memory矢量计算/基础算术/Subs.md) |  |

### 逻辑计算
| 接口名 | 功能描述 |
| --- | --- |
| [And](Memory矢量计算/逻辑计算/And.md) |  |
| [Ands](Memory矢量计算/逻辑计算/Ands.md) |  |
| [Not](Memory矢量计算/逻辑计算/Not.md) |  |
| [Or](Memory矢量计算/逻辑计算/Or.md) |  |
| [Ors](Memory矢量计算/逻辑计算/Ors.md) |  |
| [ShiftLeft](Memory矢量计算/逻辑计算/ShiftLeft.md) |  |
| [ShiftLeft（左移位数为Tensor）](Memory矢量计算/逻辑计算/ShiftLeft（左移位数为Tensor）.md) |  |
| [ShiftRight](Memory矢量计算/逻辑计算/ShiftRight.md) |  |
| [ShiftRight（右移位数为Tensor）](Memory矢量计算/逻辑计算/ShiftRight（右移位数为Tensor）.md) |  |

### 复合计算
| 接口名 | 功能描述 |
| --- | --- |
| [AbsSub(ISASI)](Memory矢量计算/复合计算/AbsSub(ISASI).md) |  |
| [ExpSub(ISASI)](Memory矢量计算/复合计算/ExpSub(ISASI).md) |  |
| [AddDeqRelu](Memory矢量计算/复合计算/AddDeqRelu.md) |  |
| [AddRelu](Memory矢量计算/复合计算/AddRelu.md) |  |
| [AddReluCast](Memory矢量计算/复合计算/AddReluCast.md) |  |
| [Axpy](Memory矢量计算/复合计算/Axpy.md) |  |
| [CastDequant](Memory矢量计算/复合计算/CastDequant.md) |  |
| [FusedMulAdd](Memory矢量计算/复合计算/FusedMulAdd.md) |  |
| [MulAddDst](Memory矢量计算/复合计算/MulAddDst.md) |  |
| [MulAddRelu](Memory矢量计算/复合计算/MulAddRelu.md) |  |
| [MulCast](Memory矢量计算/复合计算/MulCast.md) |  |
| [MulsCast(ISASI)](Memory矢量计算/复合计算/MulsCast(ISASI).md) |  |
| [SubRelu](Memory矢量计算/复合计算/SubRelu.md) |  |
| [SubReluCast](Memory矢量计算/复合计算/SubReluCast.md) |  |

### 比较与选择
| 接口名 | 功能描述 |
| --- | --- |
| [GetCmpMask(ISASI)](Memory矢量计算/比较与选择/GetCmpMask(ISASI).md) |  |
| [SetCmpMask(ISASI)](Memory矢量计算/比较与选择/SetCmpMask(ISASI).md) |  |
| [Compare](Memory矢量计算/比较与选择/Compare.md) |  |
| [Compares](Memory矢量计算/比较与选择/Compares.md) |  |
| [Compares（灵活标量位置）](Memory矢量计算/比较与选择/Compares（灵活标量位置）.md) |  |
| [Compare（结果存入寄存器）](Memory矢量计算/比较与选择/Compare（结果存入寄存器）.md) |  |
| [GatherMask](Memory矢量计算/比较与选择/GatherMask.md) |  |
| [Select](Memory矢量计算/比较与选择/Select.md) |  |
| [Select（灵活标量位置）](Memory矢量计算/比较与选择/Select（灵活标量位置）.md) |  |

### 归约计算
| 接口名 | 功能描述 |
| --- | --- |
| [GetReduceRepeatMaxMinSpr(ISASI)](Memory矢量计算/归约计算/寄存器辅助接口/GetReduceRepeatMaxMinSpr(ISASI).md) |  |
| [GetReduceRepeatSumSpr(ISASI)](Memory矢量计算/归约计算/寄存器辅助接口/GetReduceRepeatSumSpr(ISASI).md) |  |
| [GetReduceRepeatMaxMinSpr(ISASI)](Memory矢量计算/归约计算/寄存器辅助接口/GetReduceRepeatMaxMinSpr(ISASI).md) | 本接口用于获取调用ReduceRepeat时所有repeat内的最值及其索引，或获取调用ReduceMax/ReduceMin得到的最值。计算结果以全局变量形式存储，可以随时调用获取。 |
| [GetReduceRepeatSumSpr(ISASI)](Memory矢量计算/归约计算/寄存器辅助接口/GetReduceRepeatSumSpr(ISASI).md) | 获取ReduceSum接口的计算结果，仅支持tensor前n个数据连续计算接口使用。计算结果以全局变量形式存储，可以随时调用获取。 |
| [ReduceDataBlock](Memory矢量计算/归约计算/ReduceDataBlock.md) | 如图1所示，`ReduceDataBlock`接口对输入数据以DataBlock为单位进行归约操作，根据模板参数`reduceType`，对每个DataBlock内的数据求和/求最大值/求最小值。 |
| [ReduceMax](Memory矢量计算/归约计算/ReduceMax.md) | `ReduceMax`接口用于从所有输入数据中找出最大值和最大值索引。 |
| [ReduceMin](Memory矢量计算/归约计算/ReduceMin.md) | `ReduceMin`接口用于从所有输入数据中找出最小值和最小值索引。 |
| [ReducePairElem](Memory矢量计算/归约计算/ReducePairElem.md) | `ReducePairElem`接口根据模板参数`reduceType`，对相邻两个（奇偶）元素进行归约操作，结果按顺序写入目标地址。**当前仅支持求和操作。** |
| [ReduceRepeat](Memory矢量计算/归约计算/ReduceRepeat.md) | `ReduceRepeat`接口用于对每个repeat内所有数据进行归约操作，根据模板参数reduceType进行求和/求最大值/求最小值操作，结果按顺序写入目标地址。 |
| [ReduceSum](Memory矢量计算/归约计算/ReduceSum.md) | `ReduceSum`接口对所有输入数据进行求和，计算过程如下。 |

### 数据搬运
| 接口名 | 功能描述 |
| --- | --- |
| [GMToUB多维数据搬运NDDMA(DataCopy)](Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB多维数据搬运NDDMA(DataCopy).md) |  |
| [GMToUB随路转换ND2NZ搬运(DataCopy)](Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB随路转换ND2NZ搬运(DataCopy).md) |  |
| [GMToUB非对齐数据搬运(DataCopyPad)](Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB非对齐数据搬运(DataCopyPad).md) |  |
| [GM与UB切片数据搬运(DataCopy)](Memory矢量计算/数据搬运/GM与UB数据搬运/GM与UB切片数据搬运(DataCopy).md) |  |
| [GM与UB连续数据搬运(DataCopy)](Memory矢量计算/数据搬运/GM与UB数据搬运/GM与UB连续数据搬运(DataCopy).md) |  |
| [GM与UB高维切分数据搬运(DataCopy)](Memory矢量计算/数据搬运/GM与UB数据搬运/GM与UB高维切分数据搬运(DataCopy).md) |  |
| [UBToGM随路转换NZ2ND搬运(DataCopy)](Memory矢量计算/数据搬运/GM与UB数据搬运/UBToGM随路转换NZ2ND搬运(DataCopy).md) |  |
| [UBToGM非对齐数据搬运(DataCopyPad)](Memory矢量计算/数据搬运/GM与UB数据搬运/UBToGM非对齐数据搬运(DataCopyPad).md) |  |
| [SetPadValue(ISASI)](Memory矢量计算/数据搬运/GM与UB数据搬运/寄存器配置说明/SetPadValue(ISASI).md) |  |
| [UBToUB掩码式高维数据搬运(Copy)](Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB掩码式高维数据搬运(Copy).md) |  |
| [UBToUB连续数据搬运(Copy)](Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB连续数据搬运(Copy).md) |  |
| [UBToUB连续数据搬运(DataCopy)](Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB连续数据搬运(DataCopy).md) |  |
| [UBToUB高维切分数据搬运(DataCopy)](Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB高维切分数据搬运(DataCopy).md) |  |
| [ResetLoopModePara](Memory矢量计算/数据搬运/GM与UB数据搬运/寄存器配置说明/ResetLoopModePara.md) |  |
| [SetLoopModePara](Memory矢量计算/数据搬运/GM与UB数据搬运/寄存器配置说明/SetLoopModePara.md) |  |

### 数据填充
| 接口名 | 功能描述 |
| --- | --- |
| [VectorPadding(ISASI)](Memory矢量计算/数据填充/VectorPadding(ISASI).md) |  |
| [Brcb](Memory矢量计算/数据填充/Brcb.md) |  |
| [CreateVecIndex](Memory矢量计算/数据填充/CreateVecIndex.md) |  |
| [Duplicate](Memory矢量计算/数据填充/Duplicate.md) |  |

### 数据排布转换
| 接口名 | 功能描述 |
| --- | --- |
| [TransDataTo5HD](Memory矢量计算/数据排布转换/TransDataTo5HD.md) |  |
| [Transpose](Memory矢量计算/数据排布转换/Transpose.md) |  |

### 掩码操作
| 接口名 | 功能描述 |
| --- | --- |
| [ResetMask](Memory矢量计算/掩码操作/ResetMask.md) |  |
| [SetMaskCount](Memory矢量计算/掩码操作/SetMaskCount.md) |  |
| [SetMaskNorm](Memory矢量计算/掩码操作/SetMaskNorm.md) |  |
| [SetVectorMask](Memory矢量计算/掩码操作/SetVectorMask.md) |  |

### 离散与聚合
| 接口名 | 功能描述 |
| --- | --- |
| [Gatherb(ISASI)](Memory矢量计算/离散与聚合/Gatherb(ISASI).md) |  |
| [Scatter(ISASI)](Memory矢量计算/离散与聚合/Scatter(ISASI).md) |  |
| [Gather](Memory矢量计算/离散与聚合/Gather.md) |  |

### 类型转换
| 接口名 | 功能描述 |
| --- | --- |
| [Truncate(ISASI)](Memory矢量计算/类型转换/Truncate(ISASI).md) |  |
| [Cast](Memory矢量计算/类型转换/Cast.md) |  |
| [SetDeqScale](Memory矢量计算/类型转换/寄存器配置说明/SetDeqScale.md) |  |

### 数据重排（ISASI）
| 接口名 | 功能描述 |
| --- | --- |
| [DeInterleave](Memory矢量计算/数据重排（ISASI）/DeInterleave.md) |  |
| [Interleave](Memory矢量计算/数据重排（ISASI）/Interleave.md) |  |

### 排序组合（ISASI）
| 接口名 | 功能描述 |
| --- | --- |
| [GetMrgSortResult](Memory矢量计算/排序组合（ISASI）/GetMrgSortResult.md) |  |
| [MrgSort](Memory矢量计算/排序组合（ISASI）/MrgSort.md) |  |
| [MrgSort4](Memory矢量计算/排序组合（ISASI）/MrgSort4.md) |  |
| [ProposalConcat](Memory矢量计算/排序组合（ISASI）/ProposalConcat.md) |  |
| [ProposalExtract](Memory矢量计算/排序组合（ISASI）/ProposalExtract.md) |  |
| [RpSort16](Memory矢量计算/排序组合（ISASI）/RpSort16.md) |  |
| [Sort32](Memory矢量计算/排序组合（ISASI）/Sort32.md) |  |

## Reg矢量计算

### MaskReg计算
| 接口名 | 功能描述 |
| --- | --- |
| [DeInterleave](Reg矢量计算/MaskReg计算/DeInterleave.md) |  |
| [Interleave](Reg矢量计算/MaskReg计算/Interleave.md) |  |
| [Move](Reg矢量计算/MaskReg计算/Move.md) |  |
| [MoveMask](Reg矢量计算/MaskReg计算/MoveMask.md) |  |
| [Pack](Reg矢量计算/MaskReg计算/Pack.md) |  |
| [Select](Reg矢量计算/MaskReg计算/Select.md) |  |
| [UnPack](Reg矢量计算/MaskReg计算/UnPack.md) |  |

### Reg数据搬运
| 接口名 | 功能描述 |
| --- | --- |
| [MaskReg搬入](Reg矢量计算/Reg数据搬运/MaskReg搬入.md) |  |
| [MaskReg搬出](Reg矢量计算/Reg数据搬运/MaskReg搬出.md) |  |
| [Move](Reg矢量计算/Reg数据搬运/Move.md) |  |
| [连续对齐搬入](Reg矢量计算/Reg数据搬运/连续对齐搬入.md) |  |
| [连续对齐搬出](Reg矢量计算/Reg数据搬运/连续对齐搬出.md) |  |
| [连续非对齐搬入](Reg矢量计算/Reg数据搬运/连续非对齐搬入.md) |  |
| [连续非对齐搬出](Reg矢量计算/Reg数据搬运/连续非对齐搬出.md) |  |
| [非连续对齐搬入](Reg矢量计算/Reg数据搬运/非连续对齐搬入.md) |  |
| [非连续对齐搬出](Reg矢量计算/Reg数据搬运/非连续对齐搬出.md) |  |

### 同步控制
| 接口名 | 功能描述 |
| --- | --- |
| [AllocMutexID (ISASI)](同步控制/核内同步/AllocMutexID-(ISASI).md) |  |
| [DataSyncBarrier(ISASI)](同步控制/核内同步/DataSyncBarrier(ISASI).md) |  |
| [Mutex(ISASI)](同步控制/核内同步/Mutex(ISASI)/Mutex(ISASI).md) |  |
| [PipeBarrier(ISASI)](同步控制/核内同步/PipeBarrier(ISASI).md) |  |
| [ReleaseMutexID (ISASI)](同步控制/核内同步/ReleaseMutexID-(ISASI).md) |  |
| [SetFlag/WaitFlag(ISASI)](同步控制/核内同步/SetFlag-WaitFlag(ISASI).md) |  |
| [CrossCoreSetFlag(ISASI)](同步控制/核间同步/CrossCoreSetFlag(ISASI).md) |  |
| [CrossCoreWaitFlag(ISASI)](同步控制/核间同步/CrossCoreWaitFlag(ISASI).md) |  |
| [LocalMemBar](Reg矢量计算/同步控制/LocalMemBar.md) |  |

### 基础算术
| 接口名 | 功能描述 |
| --- | --- |
| [Abs](Reg矢量计算/基础算术/Abs.md) |  |
| [Add](Reg矢量计算/基础算术/Add.md) |  |
| [AddC](Reg矢量计算/基础算术/AddC.md) |  |
| [Adds](Reg矢量计算/基础算术/Adds.md) |  |
| [Div](Reg矢量计算/基础算术/Div.md) |  |
| [Exp](Reg矢量计算/基础算术/Exp.md) |  |
| [LeakyRelu](Reg矢量计算/基础算术/LeakyRelu.md) |  |
| [Ln](Reg矢量计算/基础算术/Ln.md) |  |
| [Log](Reg矢量计算/基础算术/Log.md) |  |
| [Log10](Reg矢量计算/基础算术/Log10.md) |  |
| [Log2](Reg矢量计算/基础算术/Log2.md) |  |
| [Max](Reg矢量计算/基础算术/Max.md) |  |
| [Maxs](Reg矢量计算/基础算术/Maxs.md) |  |
| [Min](Reg矢量计算/基础算术/Min.md) |  |
| [Mins](Reg矢量计算/基础算术/Mins.md) |  |
| [Mul](Reg矢量计算/基础算术/Mul.md) |  |
| [Mull](Reg矢量计算/基础算术/Mull.md) |  |
| [Muls](Reg矢量计算/基础算术/Muls.md) |  |
| [Neg](Reg矢量计算/基础算术/Neg.md) |  |
| [Prelu](Reg矢量计算/基础算术/Prelu.md) |  |
| [Relu](Reg矢量计算/基础算术/Relu.md) |  |
| [Sqrt](Reg矢量计算/基础算术/Sqrt.md) |  |
| [Sub](Reg矢量计算/基础算术/Sub.md) |  |
| [SubC](Reg矢量计算/基础算术/SubC.md) |  |

### 复合计算
| 接口名 | 功能描述 |
| --- | --- |
| [AbsSub](Reg矢量计算/复合计算/AbsSub.md) |  |
| [Axpy](Reg矢量计算/复合计算/Axpy-38.md) |  |
| [ExpSub](Reg矢量计算/复合计算/ExpSub.md) |  |
| [MulAddDst](Reg矢量计算/复合计算/MulAddDst-39.md) |  |
| [MulDstAdd](Reg矢量计算/复合计算/MulDstAdd.md) |  |
| [MulsCast](Reg矢量计算/复合计算/MulsCast.md) |  |

### 寄存器数据类型
| 接口名 | 功能描述 |
| --- | --- |
| [AddrReg](Reg矢量计算/寄存器数据类型/AddrReg.md) |  |
| [MaskReg](Reg矢量计算/寄存器数据类型/MaskReg.md) |  |
| [RegTensor](Reg矢量计算/寄存器数据类型/RegTensor.md) |  |
| [UnalignRegForLoad & UnalignRegForStore](Reg矢量计算/寄存器数据类型/UnalignRegForLoad-UnalignRegForStore.md) |  |

### 归约计算
| 接口名 | 功能描述 |
| --- | --- |
| [PairReduceElem](Reg矢量计算/归约计算/PairReduceElem.md) |  |
| [Reduce](Reg矢量计算/归约计算/Reduce.md) |  |
| [ReduceDataBlock](Reg矢量计算/归约计算/ReduceDataBlock.md) |  |

### 数据压缩
| 接口名 | 功能描述 |
| --- | --- |
| [Pack](Reg矢量计算/数据压缩/Pack-53.md) |  |
| [UnPack](Reg矢量计算/数据压缩/UnPack-54.md) |  |
| [Unsqueeze](Reg矢量计算/数据压缩/Unsqueeze.md) |  |

### 数据填充
| 接口名 | 功能描述 |
| --- | --- |
| [Duplicate](Reg矢量计算/数据填充/Duplicate.md) |  |

### 数据类型
| 接口名 | 功能描述 |
| --- | --- |
| [CastTrait](Reg矢量计算/数据类型/CastTrait.md) |  |
| [MaskMergeMode](Reg矢量计算/数据类型/MaskMergeMode.md) |  |
| [PostLiteral](Reg矢量计算/数据类型/PostLiteral.md) |  |
| [RegLayout](Reg矢量计算/数据类型/RegLayout.md) |  |
| [RoundMode](Reg矢量计算/数据类型/RoundMode.md) |  |
| [SatMode](Reg矢量计算/数据类型/SatMode.md) |  |

### 数据重排
| 接口名 | 功能描述 |
| --- | --- |
| [DeInterleave](Reg矢量计算/数据重排/DeInterleave.md) |  |
| [Interleave](Reg矢量计算/数据重排/Interleave.md) |  |

### 比较与选择
| 接口名 | 功能描述 |
| --- | --- |
| [Compare](Reg矢量计算/比较与选择/Compare-41.md) |  |
| [Compares](Reg矢量计算/比较与选择/Compares-42.md) |  |
| [Select](Reg矢量计算/比较与选择/Select-43.md) |  |
| [Squeeze](Reg矢量计算/比较与选择/Squeeze.md) |  |

### 直方图计算
| 接口名 | 功能描述 |
| --- | --- |
| [Histograms](Reg矢量计算/直方图计算/Histograms.md) |  |

### 离散操作
| 接口名 | 功能描述 |
| --- | --- |
| [Gather](Reg矢量计算/离散操作/Gather.md) |  |

### 类型转换
| 接口名 | 功能描述 |
| --- | --- |
| [Cast](Reg矢量计算/类型转换/Cast.md) |  |
| [Truncate](Reg矢量计算/类型转换/Truncate.md) |  |

### 系统变量访问
| 接口名 | 功能描述 |
| --- | --- |
| [ClearSpr](Reg矢量计算/系统变量访问/ClearSpr.md) |  |
| [GetSpr](Reg矢量计算/系统变量访问/GetSpr.md) |  |

### 索引操作
| 接口名 | 功能描述 |
| --- | --- |
| [Arange](Reg矢量计算/索引操作/Arange.md) |  |

### 逻辑计算
| 接口名 | 功能描述 |
| --- | --- |
| [And](Reg矢量计算/逻辑计算/And-33.md) |  |
| [Not](Reg矢量计算/逻辑计算/Not-32.md) |  |
| [Or](Reg矢量计算/逻辑计算/Or-34.md) |  |
| [ShiftLeft](Reg矢量计算/逻辑计算/ShiftLeft-35.md) |  |
| [ShiftLefts](Reg矢量计算/逻辑计算/ShiftLefts.md) |  |
| [ShiftRight](Reg矢量计算/逻辑计算/ShiftRight-36.md) |  |
| [ShiftRights](Reg矢量计算/逻辑计算/ShiftRights.md) |  |
| [Xor](Reg矢量计算/逻辑计算/Xor.md) |  |

## Cube分组管理（ISASI）
| 接口名 | 功能描述 |
| --- | --- |
| [AllocMessage](Cube分组管理（ISASI）/CubeResGroupHandle/AllocMessage.md) |  |
| [Arrive](Cube分组管理（ISASI）/GroupBarrier/Arrive.md) |  |
| [AssignQueue](Cube分组管理（ISASI）/CubeResGroupHandle/AssignQueue.md) |  |
| [CreateCubeResGroup](Cube分组管理（ISASI）/CubeResGroupHandle/CreateCubeResGroup.md) |  |
| [CubeResGroupHandle](Cube分组管理（ISASI）/CubeResGroupHandle/CubeResGroupHandle.md) |  |
| [FreeMessage](Cube分组管理（ISASI）/CubeResGroupHandle/FreeMessage.md) |  |
| [GetKfcWorkspace](Cube分组管理（ISASI）/KfcWorkspace/GetKfcWorkspace.md) |  |
| [GetWorkspaceLen](Cube分组管理（ISASI）/GroupBarrier/GetWorkspaceLen.md) |  |
| [GroupBarrier](Cube分组管理（ISASI）/GroupBarrier/GroupBarrier.md) |  |
| [KfcWorkspace](Cube分组管理（ISASI）/KfcWorkspace/KfcWorkspace.md) |  |
| [PostFakeMsg](Cube分组管理（ISASI）/CubeResGroupHandle/PostFakeMsg.md) |  |
| [PostMessage](Cube分组管理（ISASI）/CubeResGroupHandle/PostMessage.md) |  |
| [SetQuit](Cube分组管理（ISASI）/CubeResGroupHandle/SetQuit.md) |  |
| [SetSkipMsg](Cube分组管理（ISASI）/CubeResGroupHandle/SetSkipMsg.md) |  |
| [UpdateKfcWorkspace](Cube分组管理（ISASI）/KfcWorkspace/UpdateKfcWorkspace.md) |  |
| [Wait](Cube分组管理（ISASI）/GroupBarrier/Wait-75.md) |  |
| [Wait](Cube分组管理（ISASI）/CubeResGroupHandle/Wait.md) |  |

## Kernel-Tiling
| 接口名 | 功能描述 |
| --- | --- |
| [COPY_TILING_WITH_ARRAY](Kernel-Tiling/COPY_TILING_WITH_ARRAY.md) |  |
| [COPY_TILING_WITH_STRUCT](Kernel-Tiling/COPY_TILING_WITH_STRUCT.md) |  |
| [GET_TILING_DATA](Kernel-Tiling/GET_TILING_DATA.md) |  |
| [GET_TILING_DATA_MEMBER](Kernel-Tiling/GET_TILING_DATA_MEMBER.md) |  |
| [GET_TILING_DATA_PTR_WITH_STRUCT](Kernel-Tiling/GET_TILING_DATA_PTR_WITH_STRUCT.md) |  |
| [GET_TILING_DATA_WITH_STRUCT](Kernel-Tiling/GET_TILING_DATA_WITH_STRUCT.md) |  |
| [Kernel Tiling](Kernel-Tiling/Kernel-Tiling.md) |  |
| [REGISTER_NONE_TILING](Kernel-Tiling/REGISTER_NONE_TILING.md) |  |
| [REGISTER_TILING_DEFAULT](Kernel-Tiling/REGISTER_TILING_DEFAULT.md) |  |
| [REGISTER_TILING_FOR_TILINGKEY](Kernel-Tiling/REGISTER_TILING_FOR_TILINGKEY.md) |  |
| [TILING_KEY_IS](Kernel-Tiling/TILING_KEY_IS.md) |  |
| [TILING_KEY_LIST](Kernel-Tiling/TILING_KEY_LIST.md) |  |
| [设置Kernel类型](Kernel-Tiling/设置Kernel类型.md) |  |

## Memory矢量计算/Memory矢量计算.md
| 接口名 | 功能描述 |
| --- | --- |
| [Memory矢量计算](Memory矢量计算/Memory矢量计算.md) |  |

## 原子操作
| 接口名 | 功能描述 |
| --- | --- |
| [GetStoreAtomicConfig(ISASI)](原子操作/GetStoreAtomicConfig(ISASI).md) |  |
| [SetAtomicMax(ISASI)](原子操作/SetAtomicMax(ISASI).md) |  |
| [SetAtomicMin(ISASI)](原子操作/SetAtomicMin(ISASI).md) |  |
| [SetStoreAtomicConfig(ISASI)](原子操作/SetStoreAtomicConfig(ISASI).md) |  |
| [AtomicAdd](原子操作/AtomicAdd.md) |  |
| [AtomicCas](原子操作/AtomicCas.md) |  |
| [AtomicExch](原子操作/AtomicExch.md) |  |
| [AtomicMax](原子操作/AtomicMax.md) |  |
| [AtomicMin](原子操作/AtomicMin.md) |  |
| [DisableDmaAtomic](原子操作/DisableDmaAtomic.md) |  |
| [SetAtomicAdd](原子操作/SetAtomicAdd.md) |  |
| [SetAtomicType](原子操作/SetAtomicType.md) |  |

## 同步控制
| 接口名 | 功能描述 |
| --- | --- |
| [IBSet](同步控制/核间同步/IBSet.md) |  |
| [IBWait](同步控制/核间同步/IBWait.md) |  |
| [InitDetermineComputeWorkspace](同步控制/核间同步/InitDetermineComputeWorkspace.md) |  |
| [Lock](同步控制/核内同步/Mutex(ISASI)/Lock.md) |  |
| [NotifyNextBlock](同步控制/核间同步/NotifyNextBlock.md) |  |
| [SetFlag/WaitFlag](同步控制/核内同步/TQueSync/SetFlag-WaitFlag.md) |  |
| [SetNextTaskStart](同步控制/任务间同步/SetNextTaskStart.md) |  |
| [SyncAll](同步控制/核间同步/SyncAll.md) |  |
| [TQueSync](同步控制/核内同步/TQueSync/TQueSync.md) |  |
| [Unlock](同步控制/核内同步/Mutex(ISASI)/Unlock.md) |  |
| [WaitPreBlock](同步控制/核间同步/WaitPreBlock.md) |  |
| [WaitPreTaskEnd](同步控制/任务间同步/WaitPreTaskEnd.md) |  |

## 工具接口
| 接口名 | 功能描述 |
| --- | --- |
| [GetProgramCounter(ISASI)](工具接口/系统资源与变量/GetProgramCounter(ISASI).md) |  |
| [GetSubBlockIdx(ISASI)](工具接口/系统资源与变量/GetSubBlockIdx(ISASI).md) |  |
| [GetSubBlockNum(ISASI)](工具接口/系统资源与变量/GetSubBlockNum(ISASI).md) |  |
| [GetSystemCycle(ISASI)](工具接口/系统资源与变量/GetSystemCycle(ISASI).md) |  |
| [Async](工具接口/执行模式/Async.md) | 基于分离模式（AIC和AIV分离）开发融合算子时，算子逻辑中通常同时包含AIV和AIC的处理逻辑，此时需要使用Ascend C提供的宏`ASCEND_IS_AIV`/`ASCEND_IS_AIC... |
| [CeilDivision](工具接口/数学计算/CeilDivision.md) | 计算两个整数num1和num2相除后向上取整结果。 |
| [DeNormMin](工具接口/数学计算/NumericLimits/DeNormMin.md) | 返回指定数据类型的最小正的非正规值。 |
| [GetArchVersion](工具接口/系统资源与变量/GetArchVersion.md) | 获取当前AI处理器架构版本号。 |
| [GetBlockIdx](工具接口/系统资源与变量/GetBlockIdx.md) | 获取当前逻辑AI Core的索引，用于代码内部的多核逻辑控制及多核偏移量计算等。 |
| [GetBlockNum](工具接口/系统资源与变量/GetBlockNum.md) | 获取当前任务配置的逻辑AI Core的数量，用于代码内部的多核逻辑控制等。 |
| [GetDataBlockSizeInBytes](工具接口/系统资源与变量/GetDataBlockSizeInBytes.md) | 获取当前芯片版本一个DataBlock的大小，单位为Byte。 |
| [GetProgramCounter(ISASI)](工具接口/系统资源与变量/GetProgramCounter(ISASI).md) | 获取程序计数器的值，以追踪当前程序执行的位置。 |
| [GetRuntimeUBSize](工具接口/系统资源与变量/GetRuntimeUBSize.md) | 获取运行时UB空间的大小，单位为Byte。开发者根据UB的大小来计算循环次数等参数值。 |
| [GetSsbufBaseAddr](工具接口/系统资源与变量/GetSsbufBaseAddr.md) | 该接口用于获取SSBuffer的基地址。 |
| [GetSubBlockIdx(ISASI)](工具接口/系统资源与变量/GetSubBlockIdx(ISASI).md) | 在分离模式架构下，获取逻辑AI Core上Cube Core（AIC）或者Vector Core（AIV）的逻辑索引。 |
| [GetSubBlockNum(ISASI)](工具接口/系统资源与变量/GetSubBlockNum(ISASI).md) | 在分离模式架构下，获取当前配置中，一个逻辑AI Core上Cube Core（AIC）或者Vector Core（AIV）的数量。 |
| [GetSystemCycle(ISASI)](工具接口/系统资源与变量/GetSystemCycle(ISASI).md) | 获取系统计数器当前的cycle值。通过计算代码段前后cycle值的差值，可得到代码段消耗的系统cycle数，再通过换算即可获得代码段的执行时间。 |
| [GetTaskRatio](工具接口/系统资源与变量/GetTaskRatio.md) | 分离模式下，获取任务启动的Cube Core（AIC）或者Vector Core（AIV）的数量与逻辑AI Core数量的比例。 |
| [GetUBSizeInBytes](工具接口/系统资源与变量/GetUBSizeInBytes.md) | 获取UB空间的大小，单位为Byte。开发者根据UB的大小来计算循环次数等参数值。 |
| [GetVecLen](工具接口/系统资源与变量/GetVecLen.md) | 获取RegTensor位宽VL（Vector Length）的大小。 |
| [Infinity](工具接口/数学计算/NumericLimits/Infinity.md) | 返回指定数据类型的正无穷大值。 |
| [InitSocState](工具接口/系统初始化/InitSocState.md) | 本接口对AI Core的全局状态进行初始化，包括AIC（Cube Core）与AIV（Vector Core）的公共状态及各自特有状态。由于不同产品的实现存在差异，实际执行的初始化项也有所不同。 |
| [Lowest](工具接口/数学计算/NumericLimits/Lowest.md) | 返回指定数据类型的最低有限值，即满足无其他有限值y符合y < x的有限值x。 |
| [Max](工具接口/数学计算/NumericLimits/Max-77.md) | 返回指定数据类型的最大有限值。 |
| [Min](工具接口/数学计算/NumericLimits/Min-78.md) | 返回指定数据类型的最小有限值。 |
| [NegativeInfinity](工具接口/数学计算/NumericLimits/NegativeInfinity.md) | 返回指定数据类型的负无穷大值。 |
| [NumericLimits](工具接口/数学计算/NumericLimits/NumericLimits.md) |  |
| [QuietNaN](工具接口/数学计算/NumericLimits/QuietNaN.md) | 返回指定数据类型的安静NaN值（浮点尾数最高位为1）。 |
| [SignalingNaN](工具接口/数学计算/NumericLimits/SignalingNaN.md) | 返回指定数据类型的发信NaN值（浮点尾数最高位为0）。 |

## 标量计算
| 接口名 | 功能描述 |
| --- | --- |
| [ReadGmByPassDCache(ISASI)](标量计算/ReadGmByPassDCache(ISASI).md) |  |
| [WriteGmByPassDCache(ISASI)](标量计算/WriteGmByPassDCache(ISASI).md) |  |
| [Cast（float转bfloat16_t）](标量计算/Cast（float转bfloat16_t）.md) |  |
| [Cast（float转half/int32_t）](标量计算/Cast（float转half-int32_t）.md) |  |
| [Cast（多类型转float）](标量计算/Cast（多类型转float）.md) |  |
| [CountBitsCntSameAsSignBit](标量计算/CountBitsCntSameAsSignBit.md) |  |
| [CountLeadingZero](标量计算/CountLeadingZero.md) |  |
| [GetBitCount](标量计算/GetBitCount.md) |  |
| [GetSFFValue](标量计算/GetSFFValue.md) |  |
| [Nop](标量计算/Nop.md) | 用户输入延迟的个数，实现延迟若干个cycle。 |

## 特殊寄存器访问
| 接口名 | 功能描述 |
| --- | --- |
| [GetCtrlSpr(ISASI)](特殊寄存器访问/GetCtrlSpr(ISASI).md) |  |
| [ResetCtrlSpr(ISASI)](特殊寄存器访问/ResetCtrlSpr(ISASI).md) |  |
| [SetCtrlSpr(ISASI)](特殊寄存器访问/SetCtrlSpr(ISASI).md) |  |
| [GetCtrlSpr(ISASI)](特殊寄存器访问/GetCtrlSpr(ISASI).md) | 读取CTRL寄存器（控制寄存器）特定比特位上的值。 |
| [GetSaturationFlag(ISASI)](特殊寄存器访问/GetSaturationFlag(ISASI).md) | 获取计算/精度转换时饱和模式的开启状态。 |
| [ResetCtrlSpr(ISASI)](特殊寄存器访问/ResetCtrlSpr(ISASI).md) | 重置CTRL寄存器（控制寄存器）的特定比特位。 |
| [SetCtrlSpr(ISASI)](特殊寄存器访问/SetCtrlSpr(ISASI).md) | 对CTRL寄存器（控制寄存器）的特定比特位进行设置。 |
| [SetSaturationFlag(ISASI)](特殊寄存器访问/SetSaturationFlag(ISASI).md) | 设置饱和开关的标志位，以控制计算/精度转换时饱和模式的开启与关闭。 |

## 矩阵计算（ISASI）
| 接口名 | 功能描述 |
| --- | --- |
| [Mmad](矩阵计算（ISASI）/Mmad计算/Mmad.md) |  |
| [SetHF32Mode](矩阵计算（ISASI）/Mmad计算/寄存器配置说明/SetHF32Mode.md) |  |
| [SetHF32TransMode](矩阵计算（ISASI）/Mmad计算/寄存器配置说明/SetHF32TransMode.md) |  |
| [Load3D](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/Load3D.md) |  |
| [LoadDataWithSparse](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/LoadDataWithSparse.md) |  |
| [LoadDataWithTranspose](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/LoadDataWithTranspose.md) |  |
| [Fill](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/Fill.md) |  |
| [LoadDataUnzip](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/LoadDataUnzip.md) |  |
| [LoadImageToLocal](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/LoadImageToLocal.md) |  |
| [SetFmatrix](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetFmatrix.md) |  |
| [SetLoadDataBoundary](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetLoadDataBoundary.md) |  |
| [SetLoadDataPaddingValue](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetLoadDataPaddingValue.md) |  |
| [SetFixPipeAddr](矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixPipeAddr.md) |  |
| [SetFixPipeClipRelu](矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixPipeClipRelu.md) |  |
| [SetFixPipeConfig](矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixPipeConfig.md) |  |
| [SetFixpipePreQuantFlag](矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixpipePreQuantFlag.md) |  |
| [GMToL1 Load2D指令搬运](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1-Load2D指令搬运.md) |  |
| [GMToL1连续数据搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1连续数据搬运（DataCopy）.md) |  |
| [GMToL1随路转换-ND2NZ搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1随路转换-ND2NZ搬运（DataCopy）.md) |  |
| [GMToL1高维切分数据搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1高维切分数据搬运（DataCopy）.md) |  |
| [L0C Buffer到UB数据搬运（Fixpipe）](矩阵计算（ISASI）/矩阵计算的搬出/L0C到UB数据搬运（Fixpipe）.md) |  |
| [L0C到GM数据搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬出/L0C到GM数据搬运（DataCopy）.md) |  |
| [L0C到GM数据搬运（Fixpipe）](矩阵计算（ISASI）/矩阵计算的搬出/L0C到GM数据搬运（Fixpipe）.md) |  |
| [L0C到L1数据搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬出/L0C到L1数据搬运（DataCopy）.md) |  |
| [L0C到L1数据搬运（Fixpipe）](矩阵计算（ISASI）/矩阵计算的搬出/L0C到L1数据搬运（Fixpipe）.md) |  |
| [L1ToGM连续数据搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬出/L1到GM数据搬运/L1ToGM连续数据搬运（DataCopy）.md) |  |
| [L1ToGM高维切分数据搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬出/L1到GM数据搬运/L1ToGM高维切分数据搬运（DataCopy）.md) |  |
| [L1到GM数据搬运（仅Atlas A2/A3 系列产品支持）](矩阵计算（ISASI）/矩阵计算的搬出/L1到GM数据搬运/L1到GM数据搬运.md) |  |
| [LoadUnzipIndex](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/LoadUnzipIndex.md) |  |
| [MmadBitMode](矩阵计算（ISASI）/矩阵计算/MmadBitMode.md) | MmadBitMode对于MmadParams结构体构造进行了优化，本接口适用于scalar流水成为性能优化瓶颈的场景，支持基础Mmad/MmadMx计算功能。本接口与Mmad/MmadMx接口... |
| [MmadMx](矩阵计算（ISASI）/矩阵计算/MmadMx.md) | MmadMx（全称Microscaling Mmad）为带有量化系数的矩阵乘法，即左矩阵和右矩阵均有对应的量化系数矩阵，左量化系数矩阵scaleA和右量化系数矩阵scaleB。MmadMx场景中... |
| [MmadWithSparse](矩阵计算（ISASI）/矩阵计算/MmadWithSparse.md) | MmadWithSparse接口负责完成特殊稀疏矩阵乘加操作。稀疏矩阵是一种特殊类型的矩阵，即矩阵中包含较多的零元素。4：2结构化稀疏要求一个连续的4个权重或激活值的组（通常是张量中的一行或一列... |
| [SetAippFunctions](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetAippFunctions.md) |  |
| [SetFixpipeNz2ndFlag](矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixpipeNz2ndFlag.md) |  |
| [SetLoadDataRepeat](矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetLoadDataRepeat.md) |  |
| [SetMMColumnMajor/SetMMRowMajor](矩阵计算（ISASI）/Mmad计算/寄存器配置说明/SetMMColumnMajor-SetMMRowMajor.md) | 控制Mmad/MmadWithSparse优先通过M/N的哪个方向。 |
| [UBToL1连续数据搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1连续数据搬运（DataCopy）.md) | 该接口实现将矩阵从Unified Buffer（UB，TPosition为VECIN/VECCALC/VECOUT）搬运至L1 Buffer，搬运方式为连续搬运，数据搬运时格式和内容保持不变。 |
| [UBToL1随路转换-ND2NZ搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1随路转换-ND2NZ搬运（DataCopy）.md) |  |
| [UBToL1非对齐数据搬运（DataCopyPad）](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1非对齐数据搬运（DataCopyPad）.md) |  |
| [UBToL1高维切分数据搬运（DataCopy）](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1高维切分数据搬运（DataCopy）.md) | 该接口实现将矩阵从Unified Buffer（UB，TPosition为VECIN/VECCALC/VECOUT）搬运至L1 Buffer，支持非连续搬运和连续搬运，数据搬运时格式和内容保持不变。 |
| [矩阵计算（ISASI）](矩阵计算（ISASI）/矩阵计算（ISASI）.md) |  |
| [矩阵数据搬入至L0-Buffer](矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/矩阵数据搬入至L0-Buffer.md) |  |

## 矩阵计算（TensorAPI）
| 接口名 | 功能描述 |
| --- | --- |
| [Global Memory到L1 Buffer数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L1/GM到L1数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到L1 Buffer。`Copy`接口根据源张量和目的张量的存储位置、数据类型... |
| [L0C Buffer到Global Memory数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬出/L0C到GM数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L0C Buffer中的矩阵计算结果搬运到Global Memory。L0C Buffer中的数据通常为`Mmad`... |
| [L0C Buffer到Unified Buffer数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬出/L0C到UB数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L0C Buffer中的矩阵计算结果搬运到Unified Buffer。L0C Buffer中的数据通常为`Mmad... |
| [L1 Buffer到BiasTable Buffer数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到BiasTable数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的bias数据搬运到BiasTable Buffer，作为Mmad计算中的bias输入。 |
| [L1 Buffer到Fixpipe Buffer数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到Fixpipe数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的量化数据搬运到Fixpipe Buffer。量化数据可用于L0C输出到GM/UB时做随路量化。 |
| [L1 Buffer到L0A Buffer数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到L0A数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的左矩阵数据搬运到L0A Buffer。 |
| [L1 Buffer到L0B Buffer数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到L0B数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的右矩阵数据搬运到L0B Buffer。 |
| [L1 Buffer到L0ScaleA Buffer数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到L0ScaleA数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的左矩阵缩放数据搬运到L0ScaleA Buffer。左矩阵缩放数据在L0ScaleA Buff... |
| [L1 Buffer到L0ScaleB Buffer数据搬运](矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到L0ScaleB数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的右矩阵缩放数据搬运到L0ScaleB Buffer。右矩阵缩放数据在L0ScaleB Buff... |
| [Mmad](矩阵计算（TensorAPI）/Mmad计算/Mmad.md) | `Mmad`接口用于完成L0A Buffer上左矩阵A和L0B Buffer上右矩阵B的矩阵乘加，结果写入L0C Buffer上结果矩阵C。默认模式为普通矩阵计算。数学表达式为： |
| [矩阵计算（TensorAPI）](矩阵计算（TensorAPI）/矩阵计算（TensorAPI）.md) |  |

## 缓存控制
| 接口名 | 功能描述 |
| --- | --- |
| [GetICachePreloadStatus(ISASI)](缓存控制/GetICachePreloadStatus(ISASI).md) |  |
| [ICachePreLoad(ISASI)](缓存控制/ICachePreLoad(ISASI).md) |  |
| [DataCacheCleanAndInvalid](缓存控制/DataCacheCleanAndInvalid.md) |  |
| [DataCachePreload](缓存控制/DataCachePreload.md) |  |

## 调试接口
| 接口名 | 功能描述 |
| --- | --- |
| [CheckLocalMemoryIA(ISASI)](调试接口/异常检测/CheckLocalMemoryIA(ISASI).md) |  |
| [ascendc_assert](调试接口/异常检测/ascendc_assert.md) |  |
| [assert](调试接口/异常检测/assert.md) |  |
| [DumpAccChkPoint](调试接口/上板打印/DumpAccChkPoint.md) |  |
| [DumpTensor](调试接口/上板打印/DumpTensor.md) |  |
| [GmAlloc](调试接口/CPU孪生调试/GmAlloc.md) |  |
| [GmFree](调试接口/CPU孪生调试/GmFree.md) |  |
| [ICPU_RUN_KF](调试接口/CPU孪生调试/ICPU_RUN_KF.md) |  |
| [ICPU_SET_TILING_KEY](调试接口/CPU孪生调试/ICPU_SET_TILING_KEY.md) |  |
| [MarkStamp](调试接口/性能统计/MarkStamp.md) |  |
| [MetricsProfStart](调试接口/性能统计/MetricsProfStart.md) |  |
| [MetricsProfStop](调试接口/性能统计/MetricsProfStop.md) |  |
| [printf](调试接口/上板打印/printf.md) |  |
| [PrintTimeStamp](调试接口/上板打印/PrintTimeStamp.md) |  |
| [SetKernelMode](调试接口/CPU孪生调试/SetKernelMode.md) |  |
| [Trap](调试接口/异常检测/Trap.md) |  |

## 资源管理
| 接口名 | 功能描述 |
| --- | --- |
| [Alloc](资源管理/内存管理/LocalMemAllocator/Alloc.md) |  |
| [AllocEventID](资源管理/Pipe和Que框架/TPipe/AllocEventID.md) |  |
| [AllocTensor](资源管理/Pipe和Que框架/TSCM/AllocTensor-60.md) |  |
| [AllocTensor](资源管理/Pipe和Que框架/TQueBind/AllocTensor-64.md) |  |
| [AllocTensor](资源管理/Pipe和Que框架/TQue/AllocTensor.md) |  |
| [DeQue](资源管理/Pipe和Que框架/TSCM/DeQue-63.md) |  |
| [DeQue](资源管理/Pipe和Que框架/TQueBind/DeQue-67.md) |  |
| [DeQue](资源管理/Pipe和Que框架/TQue/DeQue.md) |  |
| [Destroy](资源管理/Pipe和Que框架/TPipe/Destroy.md) |  |
| [DestroyWithoutPipeAll](资源管理/Pipe和Que框架/TPipe/DestroyWithoutPipeAll.md) |  |
| [EnQue](资源管理/Pipe和Que框架/TSCM/EnQue-62.md) |  |
| [EnQue](资源管理/Pipe和Que框架/TQueBind/EnQue-66.md) |  |
| [EnQue](资源管理/Pipe和Que框架/TQue/EnQue.md) |  |
| [FetchEventID](资源管理/Pipe和Que框架/TPipe/FetchEventID.md) |  |
| [FreeAllEvent](资源管理/Pipe和Que框架/TQueBind/FreeAllEvent.md) |  |
| [FreeTensor](资源管理/Pipe和Que框架/TSCM/FreeTensor-61.md) |  |
| [FreeTensor](资源管理/Pipe和Que框架/TQueBind/FreeTensor-65.md) |  |
| [FreeTensor](资源管理/Pipe和Que框架/TQue/FreeTensor.md) |  |
| [Get](资源管理/Pipe和Que框架/TBuf/Get.md) |  |
| [GetBaseAddr](资源管理/Pipe和Que框架/TPipe/GetBaseAddr.md) |  |
| [GetBufHandle](资源管理/Pipe和Que框架/自定义TBufPool/GetBufHandle.md) |  |
| [GetCurAddr](资源管理/Pipe和Que框架/自定义TBufPool/GetCurAddr.md) |  |
| [GetCurAddr](资源管理/内存管理/LocalMemAllocator/GetCurAddr-72.md) |  |
| [GetCurBufSize](资源管理/Pipe和Que框架/自定义TBufPool/GetCurBufSize.md) |  |
| [GetSysWorkSpacePtr](资源管理/临时空间管理/workspace/GetSysWorkSpacePtr.md) |  |
| [GetTensorCountInQue](资源管理/Pipe和Que框架/TQueBind/GetTensorCountInQue-70.md) |  |
| [GetTensorCountInQue](资源管理/Pipe和Que框架/TQue/GetTensorCountInQue.md) |  |
| [GetTPipePtr](资源管理/Pipe和Que框架/GetTPipePtr.md) |  |
| [GetUserWorkspace](资源管理/临时空间管理/workspace/GetUserWorkspace.md) |  |
| [GetWithOffset](资源管理/Pipe和Que框架/TBuf/GetWithOffset.md) |  |
| [HasIdleBuffer](资源管理/Pipe和Que框架/TQueBind/HasIdleBuffer-71.md) |  |
| [HasIdleBuffer](资源管理/Pipe和Que框架/TQue/HasIdleBuffer.md) |  |
| [HasTensorInQue](资源管理/Pipe和Que框架/TQueBind/HasTensorInQue-69.md) |  |
| [HasTensorInQue](资源管理/Pipe和Que框架/TQue/HasTensorInQue.md) |  |
| [Init](资源管理/Pipe和Que框架/TPipe/Init.md) |  |
| [Init](资源管理/Pipe和Que框架/自定义TBufPool/Init-59.md) |  |
| [InitBuffer](资源管理/Pipe和Que框架/TPipe/InitBuffer.md) |  |
| [InitBuffer](资源管理/Pipe和Que框架/TBufPool/InitBuffer-56.md) |  |
| [InitBufHandle](资源管理/Pipe和Que框架/TQueBind/InitBufHandle.md) |  |
| [InitBufPool](资源管理/Pipe和Que框架/TPipe/InitBufPool.md) |  |
| [InitBufPool](资源管理/Pipe和Que框架/TBufPool/InitBufPool-55.md) |  |
| [InitSpmBuffer](资源管理/Pipe和Que框架/TPipe/InitSpmBuffer.md) |  |
| [InitStartBufHandle](资源管理/Pipe和Que框架/TQueBind/InitStartBufHandle.md) |  |
| [LocalMemAllocator](资源管理/内存管理/LocalMemAllocator/LocalMemAllocator.md) |  |
| [PopStackBuffer](资源管理/临时空间管理/PopStackBuffer.md) |  |
| [ReadSpmBuffer](资源管理/Pipe和Que框架/TPipe/ReadSpmBuffer.md) |  |
| [ReleaseEventID](资源管理/Pipe和Que框架/TPipe/ReleaseEventID.md) |  |
| [Reset](资源管理/Pipe和Que框架/TPipe/Reset.md) |  |
| [Reset](资源管理/Pipe和Que框架/TBufPool/Reset-57.md) |  |
| [Reset](资源管理/Pipe和Que框架/自定义TBufPool/Reset-58.md) |  |
| [SetCurAddr](资源管理/Pipe和Que框架/自定义TBufPool/SetCurAddr.md) |  |
| [SetCurBufSize](资源管理/Pipe和Que框架/自定义TBufPool/SetCurBufSize.md) |  |
| [SetSysWorkSpace](资源管理/临时空间管理/workspace/SetSysWorkSpace.md) |  |
| [TBuf](资源管理/Pipe和Que框架/TBuf/TBuf.md) |  |
| [TBufPool](资源管理/Pipe和Que框架/TBufPool/TBufPool.md) |  |
| [TPipe](资源管理/Pipe和Que框架/TPipe/TPipe.md) |  |
| [TQue](资源管理/Pipe和Que框架/TQue/TQue.md) |  |
| [TQueBind](资源管理/Pipe和Que框架/TQueBind/TQueBind.md) |  |
| [TSCM](资源管理/Pipe和Que框架/TSCM/TSCM.md) |  |
| [VacantInQue](资源管理/Pipe和Que框架/TQueBind/VacantInQue-68.md) |  |
| [VacantInQue](资源管理/Pipe和Que框架/TQue/VacantInQue.md) |  |
| [WriteSpmBuffer](资源管理/Pipe和Que框架/TPipe/WriteSpmBuffer.md) |  |
