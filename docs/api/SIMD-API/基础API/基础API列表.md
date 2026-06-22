# 基础API列表

## 基础数据结构

| 接口名 | 功能描述 |
| [TPosition](基础API/数据结构/辅助数据结构/TPosition.md) |  |
| --- | --- |
| [LocalTensor](基础API/数据结构/LocalTensor和GlobalTensor定义/LocalTensor/LocalTensor.md) | LocalTensor用于存放AI Core中Local Memory（内部存储）的数据，支持逻辑位置TPosition为VECIN、VECOUT、VECCALC、A1、A2、B1、B2、CO1、CO2。 |
| [GlobalTensor](基础API/数据结构/LocalTensor和GlobalTensor定义/GlobalTensor/GlobalTensor.md) | GlobalTensor用来存放Global Memory（外部存储）的全局数据。 |
| [Coordinate](基础API/数据结构/辅助数据结构/Coordinate/Coordinate.md) | Coordinate本质上是一个元组（tuple），用于表示张量在不同维度的位置信息，即坐标值。 |
| [Layout](基础API/数据结构/辅助数据结构/Layout/Layout.md) | Layout数据结构是描述多维张量内存布局的基础模板类，通过编译时的形状（Shape）和步长（Stride）信息，实现逻辑坐标空间到一维内存地址空间的映射。 |
| [TensorTrait](基础API/数据结构/辅助数据结构/TensorTrait/TensorTrait.md) | TensorTrait数据结构是描述Tensor相关信息的基础模板类，包含Tensor的数据类型、逻辑位置和Layout内存布局。 |

## Memory矢量计算

### 基础算术
| 接口名 | 功能描述 |
| --- | --- |
| [BilinearInterpolation(ISASI)](基础API/Memory矢量计算/基础算术/BilinearInterpolation(ISASI).md) |  |
| [Mull(ISASI)](基础API/Memory矢量计算/基础算术/Mull(ISASI).md) |  |
| [Prelu(ISASI)](基础API/Memory矢量计算/基础算术/Prelu(ISASI).md) |  |
| [Abs](基础API/Memory矢量计算/基础算术/Abs.md) |  |
| [Add](基础API/Memory矢量计算/基础算术/Add.md) |  |
| [Adds](基础API/Memory矢量计算/基础算术/Adds.md) |  |
| [Adds（灵活标量位置）](基础API/Memory矢量计算/基础算术/Adds（灵活标量位置）.md) |  |
| [Div](基础API/Memory矢量计算/基础算术/Div.md) |  |
| [Divs](基础API/Memory矢量计算/基础算术/Divs.md) |  |
| [Exp](基础API/Memory矢量计算/基础算术/Exp.md) |  |
| [LeakyRelu](基础API/Memory矢量计算/基础算术/LeakyRelu.md) |  |
| [Ln](基础API/Memory矢量计算/基础算术/Ln.md) |  |
| [Max](基础API/Memory矢量计算/基础算术/Max.md) |  |
| [Maxs](基础API/Memory矢量计算/基础算术/Maxs.md) |  |
| [Maxs（灵活标量位置）](基础API/Memory矢量计算/基础算术/Maxs（灵活标量位置）.md) |  |
| [Min](基础API/Memory矢量计算/基础算术/Min.md) |  |
| [Mins](基础API/Memory矢量计算/基础算术/Mins.md) |  |
| [Mins（灵活标量位置）](基础API/Memory矢量计算/基础算术/Mins（灵活标量位置）.md) |  |
| [Mul](基础API/Memory矢量计算/基础算术/Mul.md) |  |
| [Muls](基础API/Memory矢量计算/基础算术/Muls.md) |  |
| [Muls（灵活标量位置）](基础API/Memory矢量计算/基础算术/Muls（灵活标量位置）.md) |  |
| [Neg(ISASI)](基础API/Memory矢量计算/基础算术/Neg(ISASI).md) |  |
| [Reciprocal](基础API/Memory矢量计算/基础算术/Reciprocal.md) |  |
| [Relu](基础API/Memory矢量计算/基础算术/Relu.md) |  |
| [Rsqrt](基础API/Memory矢量计算/基础算术/Rsqrt.md) |  |
| [Sqrt](基础API/Memory矢量计算/基础算术/Sqrt.md) |  |
| [Sub](基础API/Memory矢量计算/基础算术/Sub.md) |  |
| [Subs](基础API/Memory矢量计算/基础算术/Subs.md) |  |

### 逻辑计算
| 接口名 | 功能描述 |
| --- | --- |
| [And](基础API/Memory矢量计算/逻辑计算/And.md) |  |
| [Ands](基础API/Memory矢量计算/逻辑计算/Ands.md) |  |
| [Not](基础API/Memory矢量计算/逻辑计算/Not.md) |  |
| [Or](基础API/Memory矢量计算/逻辑计算/Or.md) |  |
| [Ors](基础API/Memory矢量计算/逻辑计算/Ors.md) |  |
| [ShiftLeft](基础API/Memory矢量计算/逻辑计算/ShiftLeft.md) |  |
| [ShiftLeft（左移位数为Tensor）](基础API/Memory矢量计算/逻辑计算/ShiftLeft（左移位数为Tensor）.md) |  |
| [ShiftRight](基础API/Memory矢量计算/逻辑计算/ShiftRight.md) |  |
| [ShiftRight（右移位数为Tensor）](基础API/Memory矢量计算/逻辑计算/ShiftRight（右移位数为Tensor）.md) |  |

### 复合计算
| 接口名 | 功能描述 |
| --- | --- |
| [AbsSub(ISASI)](基础API/Memory矢量计算/复合计算/AbsSub(ISASI).md) |  |
| [ExpSub(ISASI)](基础API/Memory矢量计算/复合计算/ExpSub(ISASI).md) |  |
| [AddDeqRelu](基础API/Memory矢量计算/复合计算/AddDeqRelu.md) |  |
| [AddRelu](基础API/Memory矢量计算/复合计算/AddRelu.md) |  |
| [AddReluCast](基础API/Memory矢量计算/复合计算/AddReluCast.md) |  |
| [Axpy](基础API/Memory矢量计算/复合计算/Axpy.md) |  |
| [CastDequant](基础API/Memory矢量计算/复合计算/CastDequant.md) |  |
| [FusedMulAdd](基础API/Memory矢量计算/复合计算/FusedMulAdd.md) |  |
| [MulAddDst](基础API/Memory矢量计算/复合计算/MulAddDst.md) |  |
| [MulAddRelu](基础API/Memory矢量计算/复合计算/MulAddRelu.md) |  |
| [MulCast](基础API/Memory矢量计算/复合计算/MulCast.md) |  |
| [MulsCast(ISASI)](基础API/Memory矢量计算/复合计算/MulsCast(ISASI).md) |  |
| [SubRelu](基础API/Memory矢量计算/复合计算/SubRelu.md) |  |
| [SubReluCast](基础API/Memory矢量计算/复合计算/SubReluCast.md) |  |

### 比较与选择
| 接口名 | 功能描述 |
| --- | --- |
| [GetCmpMask(ISASI)](基础API/Memory矢量计算/比较与选择/GetCmpMask(ISASI).md) |  |
| [SetCmpMask(ISASI)](基础API/Memory矢量计算/比较与选择/SetCmpMask(ISASI).md) |  |
| [Compare](基础API/Memory矢量计算/比较与选择/Compare.md) |  |
| [Compares](基础API/Memory矢量计算/比较与选择/Compares.md) |  |
| [Compares（灵活标量位置）](基础API/Memory矢量计算/比较与选择/Compares（灵活标量位置）.md) |  |
| [Compare（结果存入寄存器）](基础API/Memory矢量计算/比较与选择/Compare（结果存入寄存器）.md) |  |
| [GatherMask](基础API/Memory矢量计算/比较与选择/GatherMask.md) |  |
| [Select](基础API/Memory矢量计算/比较与选择/Select.md) |  |
| [Select（灵活标量位置）](基础API/Memory矢量计算/比较与选择/Select（灵活标量位置）.md) |  |

### 归约计算
| 接口名 | 功能描述 |
| --- | --- |
| [GetReduceRepeatMaxMinSpr(ISASI)](基础API/Memory矢量计算/归约计算/寄存器辅助接口/GetReduceRepeatMaxMinSpr(ISASI).md) |  |
| [GetReduceRepeatSumSpr(ISASI)](基础API/Memory矢量计算/归约计算/寄存器辅助接口/GetReduceRepeatSumSpr(ISASI).md) |  |
| [GetReduceRepeatMaxMinSpr(ISASI)](基础API/Memory矢量计算/归约计算/寄存器辅助接口/GetReduceRepeatMaxMinSpr(ISASI).md) | 本接口用于获取调用ReduceRepeat时所有repeat内的最值及其索引，或获取调用ReduceMax/ReduceMin得到的最值。计算结果以全局变量形式存储，可以随时调用获取。 |
| [GetReduceRepeatSumSpr(ISASI)](基础API/Memory矢量计算/归约计算/寄存器辅助接口/GetReduceRepeatSumSpr(ISASI).md) | 获取ReduceSum接口的计算结果，仅支持tensor前n个数据连续计算接口使用。计算结果以全局变量形式存储，可以随时调用获取。 |
| [ReduceDataBlock](基础API/Memory矢量计算/归约计算/ReduceDataBlock.md) | 如图1所示，`ReduceDataBlock`接口对输入数据以DataBlock为单位进行归约操作，根据模板参数`reduceType`，对每个DataBlock内的数据求和/求最大值/求最小值。 |
| [ReduceMax](基础API/Memory矢量计算/归约计算/ReduceMax.md) | `ReduceMax`接口用于从所有输入数据中找出最大值和最大值索引。 |
| [ReduceMin](基础API/Memory矢量计算/归约计算/ReduceMin.md) | `ReduceMin`接口用于从所有输入数据中找出最小值和最小值索引。 |
| [ReducePairElem](基础API/Memory矢量计算/归约计算/ReducePairElem.md) | `ReducePairElem`接口根据模板参数`reduceType`，对相邻两个（奇偶）元素进行归约操作，结果按顺序写入目标地址。**当前仅支持求和操作。** |
| [ReduceRepeat](基础API/Memory矢量计算/归约计算/ReduceRepeat.md) | `ReduceRepeat`接口用于对每个repeat内所有数据进行归约操作，根据模板参数reduceType进行求和/求最大值/求最小值操作，结果按顺序写入目标地址。 |
| [ReduceSum](基础API/Memory矢量计算/归约计算/ReduceSum.md) | `ReduceSum`接口对所有输入数据进行求和，计算过程如下。 |

### 数据搬运
| 接口名 | 功能描述 |
| --- | --- |
| [GMToUB多维数据搬运NDDMA(DataCopy)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB多维数据搬运NDDMA(DataCopy).md) |  |
| [GMToUB随路转换ND2NZ搬运(DataCopy)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB随路转换ND2NZ搬运(DataCopy).md) |  |
| [GMToUB非对齐数据搬运(DataCopyPad)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB非对齐数据搬运(DataCopyPad).md) |  |
| [GM与UB切片数据搬运(DataCopy)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/GM与UB切片数据搬运(DataCopy).md) |  |
| [GM与UB连续数据搬运(DataCopy)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/GM与UB连续数据搬运(DataCopy).md) |  |
| [GM与UB高维切分数据搬运(DataCopy)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/GM与UB高维切分数据搬运(DataCopy).md) |  |
| [UBToGM随路转换NZ2ND搬运(DataCopy)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/UBToGM随路转换NZ2ND搬运(DataCopy).md) |  |
| [UBToGM非对齐数据搬运(DataCopyPad)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/UBToGM非对齐数据搬运(DataCopyPad).md) |  |
| [SetPadValue(ISASI)](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/寄存器配置说明/SetPadValue(ISASI).md) |  |
| [UBToUB掩码式高维数据搬运(Copy)](基础API/Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB掩码式高维数据搬运(Copy).md) |  |
| [UBToUB连续数据搬运(Copy)](基础API/Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB连续数据搬运(Copy).md) |  |
| [UBToUB连续数据搬运(DataCopy)](基础API/Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB连续数据搬运(DataCopy).md) |  |
| [UBToUB高维切分数据搬运(DataCopy)](基础API/Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB高维切分数据搬运(DataCopy).md) |  |
| [ResetLoopModePara](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/寄存器配置说明/ResetLoopModePara.md) |  |
| [SetLoopModePara](基础API/Memory矢量计算/数据搬运/GM与UB数据搬运/寄存器配置说明/SetLoopModePara.md) |  |

### 数据填充
| 接口名 | 功能描述 |
| --- | --- |
| [VectorPadding(ISASI)](基础API/Memory矢量计算/数据填充/VectorPadding(ISASI).md) |  |
| [Brcb](基础API/Memory矢量计算/数据填充/Brcb.md) |  |
| [CreateVecIndex](基础API/Memory矢量计算/数据填充/CreateVecIndex.md) |  |
| [Duplicate](基础API/Memory矢量计算/数据填充/Duplicate.md) |  |

### 数据排布转换
| 接口名 | 功能描述 |
| --- | --- |
| [TransDataTo5HD](基础API/Memory矢量计算/数据排布转换/TransDataTo5HD.md) |  |
| [Transpose](基础API/Memory矢量计算/数据排布转换/Transpose.md) |  |

### 掩码操作
| 接口名 | 功能描述 |
| --- | --- |
| [ResetMask](基础API/Memory矢量计算/掩码操作/ResetMask.md) |  |
| [SetMaskCount](基础API/Memory矢量计算/掩码操作/SetMaskCount.md) |  |
| [SetMaskNorm](基础API/Memory矢量计算/掩码操作/SetMaskNorm.md) |  |
| [SetVectorMask](基础API/Memory矢量计算/掩码操作/SetVectorMask.md) |  |

### 离散与聚合
| 接口名 | 功能描述 |
| --- | --- |
| [Gatherb(ISASI)](基础API/Memory矢量计算/离散与聚合/Gatherb(ISASI).md) |  |
| [Scatter(ISASI)](基础API/Memory矢量计算/离散与聚合/Scatter(ISASI).md) |  |
| [Gather](基础API/Memory矢量计算/离散与聚合/Gather.md) |  |

### 类型转换
| 接口名 | 功能描述 |
| --- | --- |
| [Truncate(ISASI)](基础API/Memory矢量计算/类型转换/Truncate(ISASI).md) |  |
| [Cast](基础API/Memory矢量计算/类型转换/Cast.md) |  |
| [SetDeqScale](基础API/Memory矢量计算/类型转换/寄存器配置说明/SetDeqScale.md) |  |

### 数据重排（ISASI）
| 接口名 | 功能描述 |
| --- | --- |
| [DeInterleave](基础API/Memory矢量计算/数据重排（ISASI）/DeInterleave.md) |  |
| [Interleave](基础API/Memory矢量计算/数据重排（ISASI）/Interleave.md) |  |

### 排序组合（ISASI）
| 接口名 | 功能描述 |
| --- | --- |
| [GetMrgSortResult](基础API/Memory矢量计算/排序组合（ISASI）/GetMrgSortResult.md) |  |
| [MrgSort](基础API/Memory矢量计算/排序组合（ISASI）/MrgSort.md) |  |
| [MrgSort4](基础API/Memory矢量计算/排序组合（ISASI）/MrgSort4.md) |  |
| [ProposalConcat](基础API/Memory矢量计算/排序组合（ISASI）/ProposalConcat.md) |  |
| [ProposalExtract](基础API/Memory矢量计算/排序组合（ISASI）/ProposalExtract.md) |  |
| [RpSort16](基础API/Memory矢量计算/排序组合（ISASI）/RpSort16.md) |  |
| [Sort32](基础API/Memory矢量计算/排序组合（ISASI）/Sort32.md) |  |

## Reg矢量计算

### MaskReg计算
| 接口名 | 功能描述 |
| --- | --- |
| [DeInterleave](基础API/Reg矢量计算/MaskReg计算/DeInterleave.md) |  |
| [Interleave](基础API/Reg矢量计算/MaskReg计算/Interleave.md) |  |
| [Move](基础API/Reg矢量计算/MaskReg计算/Move.md) |  |
| [MoveMask](基础API/Reg矢量计算/MaskReg计算/MoveMask.md) |  |
| [Pack](基础API/Reg矢量计算/MaskReg计算/Pack.md) |  |
| [Select](基础API/Reg矢量计算/MaskReg计算/Select.md) |  |
| [UnPack](基础API/Reg矢量计算/MaskReg计算/UnPack.md) |  |

### Reg数据搬运
| 接口名 | 功能描述 |
| --- | --- |
| [MaskReg搬入](基础API/Reg矢量计算/Reg数据搬运/MaskReg搬入.md) |  |
| [MaskReg搬出](基础API/Reg矢量计算/Reg数据搬运/MaskReg搬出.md) |  |
| [Move](基础API/Reg矢量计算/Reg数据搬运/Move.md) |  |
| [连续对齐搬入](基础API/Reg矢量计算/Reg数据搬运/连续对齐搬入.md) |  |
| [连续对齐搬出](基础API/Reg矢量计算/Reg数据搬运/连续对齐搬出.md) |  |
| [连续非对齐搬入](基础API/Reg矢量计算/Reg数据搬运/连续非对齐搬入.md) |  |
| [连续非对齐搬出](基础API/Reg矢量计算/Reg数据搬运/连续非对齐搬出.md) |  |
| [非连续对齐搬入](基础API/Reg矢量计算/Reg数据搬运/非连续对齐搬入.md) |  |
| [非连续对齐搬出](基础API/Reg矢量计算/Reg数据搬运/非连续对齐搬出.md) |  |

### 同步控制
| 接口名 | 功能描述 |
| --- | --- |
| [AllocMutexID (ISASI)](基础API/同步控制/核内同步/AllocMutexID-(ISASI).md) |  |
| [DataSyncBarrier(ISASI)](基础API/同步控制/核内同步/DataSyncBarrier(ISASI).md) |  |
| [Mutex(ISASI)](基础API/同步控制/核内同步/Mutex(ISASI)/Mutex(ISASI).md) |  |
| [PipeBarrier(ISASI)](基础API/同步控制/核内同步/PipeBarrier(ISASI).md) |  |
| [ReleaseMutexID (ISASI)](基础API/同步控制/核内同步/ReleaseMutexID-(ISASI).md) |  |
| [SetFlag/WaitFlag(ISASI)](基础API/同步控制/核内同步/SetFlag-WaitFlag(ISASI).md) |  |
| [CrossCoreSetFlag(ISASI)](基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md) |  |
| [CrossCoreWaitFlag(ISASI)](基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md) |  |
| [LocalMemBar](基础API/Reg矢量计算/同步控制/LocalMemBar.md) |  |

### 基础算术
| 接口名 | 功能描述 |
| --- | --- |
| [Abs](基础API/Reg矢量计算/基础算术/Abs.md) |  |
| [Add](基础API/Reg矢量计算/基础算术/Add.md) |  |
| [AddC](基础API/Reg矢量计算/基础算术/AddC.md) |  |
| [Adds](基础API/Reg矢量计算/基础算术/Adds.md) |  |
| [Div](基础API/Reg矢量计算/基础算术/Div.md) |  |
| [Exp](基础API/Reg矢量计算/基础算术/Exp.md) |  |
| [LeakyRelu](基础API/Reg矢量计算/基础算术/LeakyRelu.md) |  |
| [Ln](基础API/Reg矢量计算/基础算术/Ln.md) |  |
| [Log](基础API/Reg矢量计算/基础算术/Log.md) |  |
| [Log10](基础API/Reg矢量计算/基础算术/Log10.md) |  |
| [Log2](基础API/Reg矢量计算/基础算术/Log2.md) |  |
| [Max](基础API/Reg矢量计算/基础算术/Max.md) |  |
| [Maxs](基础API/Reg矢量计算/基础算术/Maxs.md) |  |
| [Min](基础API/Reg矢量计算/基础算术/Min.md) |  |
| [Mins](基础API/Reg矢量计算/基础算术/Mins.md) |  |
| [Mul](基础API/Reg矢量计算/基础算术/Mul.md) |  |
| [Mull](基础API/Reg矢量计算/基础算术/Mull.md) |  |
| [Muls](基础API/Reg矢量计算/基础算术/Muls.md) |  |
| [Neg](基础API/Reg矢量计算/基础算术/Neg.md) |  |
| [Prelu](基础API/Reg矢量计算/基础算术/Prelu.md) |  |
| [Relu](基础API/Reg矢量计算/基础算术/Relu.md) |  |
| [Sqrt](基础API/Reg矢量计算/基础算术/Sqrt.md) |  |
| [Sub](基础API/Reg矢量计算/基础算术/Sub.md) |  |
| [SubC](基础API/Reg矢量计算/基础算术/SubC.md) |  |

### 复合计算
| 接口名 | 功能描述 |
| --- | --- |
| [AbsSub](基础API/Reg矢量计算/复合计算/AbsSub.md) |  |
| [Axpy](基础API/Reg矢量计算/复合计算/Axpy-38.md) |  |
| [ExpSub](基础API/Reg矢量计算/复合计算/ExpSub.md) |  |
| [MulAddDst](基础API/Reg矢量计算/复合计算/MulAddDst-39.md) |  |
| [MulDstAdd](基础API/Reg矢量计算/复合计算/MulDstAdd.md) |  |
| [MulsCast](基础API/Reg矢量计算/复合计算/MulsCast.md) |  |

### 寄存器数据类型
| 接口名 | 功能描述 |
| --- | --- |
| [AddrReg](基础API/Reg矢量计算/寄存器数据类型/AddrReg.md) |  |
| [MaskReg](基础API/Reg矢量计算/寄存器数据类型/MaskReg.md) |  |
| [RegTensor](基础API/Reg矢量计算/寄存器数据类型/RegTensor.md) |  |
| [UnalignRegForLoad & UnalignRegForStore](基础API/Reg矢量计算/寄存器数据类型/UnalignRegForLoad-UnalignRegForStore.md) |  |

### 归约计算
| 接口名 | 功能描述 |
| --- | --- |
| [PairReduceElem](基础API/Reg矢量计算/归约计算/PairReduceElem.md) |  |
| [Reduce](基础API/Reg矢量计算/归约计算/Reduce.md) |  |
| [ReduceDataBlock](基础API/Reg矢量计算/归约计算/ReduceDataBlock.md) |  |

### 数据压缩
| 接口名 | 功能描述 |
| --- | --- |
| [Pack](基础API/Reg矢量计算/数据压缩/Pack-53.md) |  |
| [UnPack](基础API/Reg矢量计算/数据压缩/UnPack-54.md) |  |
| [Unsqueeze](基础API/Reg矢量计算/数据压缩/Unsqueeze.md) |  |

### 数据填充
| 接口名 | 功能描述 |
| --- | --- |
| [Duplicate](基础API/Reg矢量计算/数据填充/Duplicate.md) |  |

### 数据类型
| 接口名 | 功能描述 |
| --- | --- |
| [CastTrait](基础API/Reg矢量计算/数据类型/CastTrait.md) |  |
| [MaskMergeMode](基础API/Reg矢量计算/数据类型/MaskMergeMode.md) |  |
| [PostLiteral](基础API/Reg矢量计算/数据类型/PostLiteral.md) |  |
| [RegLayout](基础API/Reg矢量计算/数据类型/RegLayout.md) |  |
| [RoundMode](基础API/Reg矢量计算/数据类型/RoundMode.md) |  |
| [SatMode](基础API/Reg矢量计算/数据类型/SatMode.md) |  |

### 数据重排
| 接口名 | 功能描述 |
| --- | --- |
| [DeInterleave](基础API/Reg矢量计算/数据重排/DeInterleave.md) |  |
| [Interleave](基础API/Reg矢量计算/数据重排/Interleave.md) |  |

### 比较与选择
| 接口名 | 功能描述 |
| --- | --- |
| [Compare](基础API/Reg矢量计算/比较与选择/Compare-41.md) |  |
| [Compares](基础API/Reg矢量计算/比较与选择/Compares-42.md) |  |
| [Select](基础API/Reg矢量计算/比较与选择/Select-43.md) |  |
| [Squeeze](基础API/Reg矢量计算/比较与选择/Squeeze.md) |  |

### 直方图计算
| 接口名 | 功能描述 |
| --- | --- |
| [Histograms](基础API/Reg矢量计算/直方图计算/Histograms.md) |  |

### 离散操作
| 接口名 | 功能描述 |
| --- | --- |
| [Gather](基础API/Reg矢量计算/离散操作/Gather.md) |  |

### 类型转换
| 接口名 | 功能描述 |
| --- | --- |
| [Cast](基础API/Reg矢量计算/类型转换/Cast.md) |  |
| [Truncate](基础API/Reg矢量计算/类型转换/Truncate.md) |  |

### 系统变量访问
| 接口名 | 功能描述 |
| --- | --- |
| [ClearSpr](基础API/Reg矢量计算/系统变量访问/ClearSpr.md) |  |
| [GetSpr](基础API/Reg矢量计算/系统变量访问/GetSpr.md) |  |

### 索引操作
| 接口名 | 功能描述 |
| --- | --- |
| [Arange](基础API/Reg矢量计算/索引操作/Arange.md) |  |

### 逻辑计算
| 接口名 | 功能描述 |
| --- | --- |
| [And](基础API/Reg矢量计算/逻辑计算/And-33.md) |  |
| [Not](基础API/Reg矢量计算/逻辑计算/Not-32.md) |  |
| [Or](基础API/Reg矢量计算/逻辑计算/Or-34.md) |  |
| [ShiftLeft](基础API/Reg矢量计算/逻辑计算/ShiftLeft-35.md) |  |
| [ShiftLefts](基础API/Reg矢量计算/逻辑计算/ShiftLefts.md) |  |
| [ShiftRight](基础API/Reg矢量计算/逻辑计算/ShiftRight-36.md) |  |
| [ShiftRights](基础API/Reg矢量计算/逻辑计算/ShiftRights.md) |  |
| [Xor](基础API/Reg矢量计算/逻辑计算/Xor.md) |  |

## Cube分组管理（ISASI）
| 接口名 | 功能描述 |
| --- | --- |
| [AllocMessage](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/AllocMessage.md) |  |
| [Arrive](基础API/Cube分组管理（ISASI）/GroupBarrier/Arrive.md) |  |
| [AssignQueue](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/AssignQueue.md) |  |
| [CreateCubeResGroup](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/CreateCubeResGroup.md) |  |
| [CubeResGroupHandle](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/CubeResGroupHandle.md) |  |
| [FreeMessage](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/FreeMessage.md) |  |
| [GetKfcWorkspace](基础API/Cube分组管理（ISASI）/KfcWorkspace/GetKfcWorkspace.md) |  |
| [GetWorkspaceLen](基础API/Cube分组管理（ISASI）/GroupBarrier/GetWorkspaceLen.md) |  |
| [GroupBarrier](基础API/Cube分组管理（ISASI）/GroupBarrier/GroupBarrier.md) |  |
| [KfcWorkspace](基础API/Cube分组管理（ISASI）/KfcWorkspace/KfcWorkspace.md) |  |
| [PostFakeMsg](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/PostFakeMsg.md) |  |
| [PostMessage](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/PostMessage.md) |  |
| [SetQuit](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/SetQuit.md) |  |
| [SetSkipMsg](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/SetSkipMsg.md) |  |
| [UpdateKfcWorkspace](基础API/Cube分组管理（ISASI）/KfcWorkspace/UpdateKfcWorkspace.md) |  |
| [Wait](基础API/Cube分组管理（ISASI）/GroupBarrier/Wait-75.md) |  |
| [Wait](基础API/Cube分组管理（ISASI）/CubeResGroupHandle/Wait.md) |  |

## Kernel-Tiling
| 接口名 | 功能描述 |
| --- | --- |
| [COPY_TILING_WITH_ARRAY](基础API/Kernel-Tiling/COPY_TILING_WITH_ARRAY.md) |  |
| [COPY_TILING_WITH_STRUCT](基础API/Kernel-Tiling/COPY_TILING_WITH_STRUCT.md) |  |
| [GET_TILING_DATA](基础API/Kernel-Tiling/GET_TILING_DATA.md) |  |
| [GET_TILING_DATA_MEMBER](基础API/Kernel-Tiling/GET_TILING_DATA_MEMBER.md) |  |
| [GET_TILING_DATA_PTR_WITH_STRUCT](基础API/Kernel-Tiling/GET_TILING_DATA_PTR_WITH_STRUCT.md) |  |
| [GET_TILING_DATA_WITH_STRUCT](基础API/Kernel-Tiling/GET_TILING_DATA_WITH_STRUCT.md) |  |
| [Kernel Tiling](基础API/Kernel-Tiling/Kernel-Tiling.md) |  |
| [REGISTER_NONE_TILING](基础API/Kernel-Tiling/REGISTER_NONE_TILING.md) |  |
| [REGISTER_TILING_DEFAULT](基础API/Kernel-Tiling/REGISTER_TILING_DEFAULT.md) |  |
| [REGISTER_TILING_FOR_TILINGKEY](基础API/Kernel-Tiling/REGISTER_TILING_FOR_TILINGKEY.md) |  |
| [TILING_KEY_IS](基础API/Kernel-Tiling/TILING_KEY_IS.md) |  |
| [TILING_KEY_LIST](基础API/Kernel-Tiling/TILING_KEY_LIST.md) |  |
| [设置Kernel类型](基础API/Kernel-Tiling/设置Kernel类型.md) |  |

## Memory矢量计算/Memory矢量计算.md
| 接口名 | 功能描述 |
| --- | --- |
| [Memory矢量计算](基础API/Memory矢量计算/Memory矢量计算.md) |  |

## 原子操作
| 接口名 | 功能描述 |
| --- | --- |
| [GetStoreAtomicConfig(ISASI)](基础API/原子操作/GetStoreAtomicConfig(ISASI).md) |  |
| [SetAtomicMax(ISASI)](基础API/原子操作/SetAtomicMax(ISASI).md) |  |
| [SetAtomicMin(ISASI)](基础API/原子操作/SetAtomicMin(ISASI).md) |  |
| [SetStoreAtomicConfig(ISASI)](基础API/原子操作/SetStoreAtomicConfig(ISASI).md) |  |
| [AtomicAdd](基础API/原子操作/AtomicAdd.md) |  |
| [AtomicCas](基础API/原子操作/AtomicCas.md) |  |
| [AtomicExch](基础API/原子操作/AtomicExch.md) |  |
| [AtomicMax](基础API/原子操作/AtomicMax.md) |  |
| [AtomicMin](基础API/原子操作/AtomicMin.md) |  |
| [DisableDmaAtomic](基础API/原子操作/DisableDmaAtomic.md) |  |
| [SetAtomicAdd](基础API/原子操作/SetAtomicAdd.md) |  |
| [SetAtomicType](基础API/原子操作/SetAtomicType.md) |  |

## 同步控制
| 接口名 | 功能描述 |
| --- | --- |
| [IBSet](基础API/同步控制/核间同步/IBSet.md) |  |
| [IBWait](基础API/同步控制/核间同步/IBWait.md) |  |
| [InitDetermineComputeWorkspace](基础API/同步控制/核间同步/InitDetermineComputeWorkspace.md) |  |
| [Lock](基础API/同步控制/核内同步/Mutex(ISASI)/Lock.md) |  |
| [NotifyNextBlock](基础API/同步控制/核间同步/NotifyNextBlock.md) |  |
| [SetFlag/WaitFlag](基础API/同步控制/核内同步/TQueSync/SetFlag-WaitFlag.md) |  |
| [SetNextTaskStart](基础API/同步控制/任务间同步/SetNextTaskStart.md) |  |
| [SyncAll](基础API/同步控制/核间同步/SyncAll.md) |  |
| [TQueSync](基础API/同步控制/核内同步/TQueSync/TQueSync.md) |  |
| [Unlock](基础API/同步控制/核内同步/Mutex(ISASI)/Unlock.md) |  |
| [WaitPreBlock](基础API/同步控制/核间同步/WaitPreBlock.md) |  |
| [WaitPreTaskEnd](基础API/同步控制/任务间同步/WaitPreTaskEnd.md) |  |

## 工具接口
| 接口名 | 功能描述 |
| --- | --- |
| [GetProgramCounter(ISASI)](基础API/工具接口/系统资源与变量/GetProgramCounter(ISASI).md) |  |
| [GetSubBlockIdx(ISASI)](基础API/工具接口/系统资源与变量/GetSubBlockIdx(ISASI).md) |  |
| [GetSubBlockNum(ISASI)](基础API/工具接口/系统资源与变量/GetSubBlockNum(ISASI).md) |  |
| [GetSystemCycle(ISASI)](基础API/工具接口/系统资源与变量/GetSystemCycle(ISASI).md) |  |
| [Async](基础API/工具接口/执行模式/Async.md) | 基于分离模式（AIC和AIV分离）开发融合算子时，算子逻辑中通常同时包含AIV和AIC的处理逻辑，此时需要使用Ascend C提供的宏`ASCEND_IS_AIV`/`ASCEND_IS_AIC... |
| [CeilDivision](基础API/工具接口/数学计算/CeilDivision.md) | 计算两个整数num1和num2相除后向上取整结果。 |
| [DeNormMin](基础API/工具接口/数学计算/NumericLimits/DeNormMin.md) | 返回指定数据类型的最小正的非正规值。 |
| [GetArchVersion](基础API/工具接口/系统资源与变量/GetArchVersion.md) | 获取当前AI处理器架构版本号。 |
| [GetBlockIdx](基础API/工具接口/系统资源与变量/GetBlockIdx.md) | 获取当前逻辑AI Core的索引，用于代码内部的多核逻辑控制及多核偏移量计算等。 |
| [GetBlockNum](基础API/工具接口/系统资源与变量/GetBlockNum.md) | 获取当前任务配置的逻辑AI Core的数量，用于代码内部的多核逻辑控制等。 |
| [GetDataBlockSizeInBytes](基础API/工具接口/系统资源与变量/GetDataBlockSizeInBytes.md) | 获取当前芯片版本一个DataBlock的大小，单位为Byte。 |
| [GetProgramCounter(ISASI)](基础API/工具接口/系统资源与变量/GetProgramCounter(ISASI).md) | 获取程序计数器的值，以追踪当前程序执行的位置。 |
| [GetRuntimeUBSize](基础API/工具接口/系统资源与变量/GetRuntimeUBSize.md) | 获取运行时UB空间的大小，单位为Byte。开发者根据UB的大小来计算循环次数等参数值。 |
| [GetSsbufBaseAddr](基础API/工具接口/系统资源与变量/GetSsbufBaseAddr.md) | 该接口用于获取SSBuffer的基地址。 |
| [GetSubBlockIdx(ISASI)](基础API/工具接口/系统资源与变量/GetSubBlockIdx(ISASI).md) | 在分离模式架构下，获取逻辑AI Core上Cube Core（AIC）或者Vector Core（AIV）的逻辑索引。 |
| [GetSubBlockNum(ISASI)](基础API/工具接口/系统资源与变量/GetSubBlockNum(ISASI).md) | 在分离模式架构下，获取当前配置中，一个逻辑AI Core上Cube Core（AIC）或者Vector Core（AIV）的数量。 |
| [GetSystemCycle(ISASI)](基础API/工具接口/系统资源与变量/GetSystemCycle(ISASI).md) | 获取系统计数器当前的cycle值。通过计算代码段前后cycle值的差值，可得到代码段消耗的系统cycle数，再通过换算即可获得代码段的执行时间。 |
| [GetTaskRatio](基础API/工具接口/系统资源与变量/GetTaskRatio.md) | 分离模式下，获取任务启动的Cube Core（AIC）或者Vector Core（AIV）的数量与逻辑AI Core数量的比例。 |
| [GetUBSizeInBytes](基础API/工具接口/系统资源与变量/GetUBSizeInBytes.md) | 获取UB空间的大小，单位为Byte。开发者根据UB的大小来计算循环次数等参数值。 |
| [GetVecLen](基础API/工具接口/系统资源与变量/GetVecLen.md) | 获取RegTensor位宽VL（Vector Length）的大小。 |
| [Infinity](基础API/工具接口/数学计算/NumericLimits/Infinity.md) | 返回指定数据类型的正无穷大值。 |
| [InitSocState](基础API/工具接口/系统初始化/InitSocState.md) | 本接口对AI Core的全局状态进行初始化，包括AIC（Cube Core）与AIV（Vector Core）的公共状态及各自特有状态。由于不同产品的实现存在差异，实际执行的初始化项也有所不同。 |
| [Lowest](基础API/工具接口/数学计算/NumericLimits/Lowest.md) | 返回指定数据类型的最低有限值，即满足无其他有限值y符合y < x的有限值x。 |
| [Max](基础API/工具接口/数学计算/NumericLimits/Max-77.md) | 返回指定数据类型的最大有限值。 |
| [Min](基础API/工具接口/数学计算/NumericLimits/Min-78.md) | 返回指定数据类型的最小有限值。 |
| [NegativeInfinity](基础API/工具接口/数学计算/NumericLimits/NegativeInfinity.md) | 返回指定数据类型的负无穷大值。 |
| [NumericLimits](基础API/工具接口/数学计算/NumericLimits/NumericLimits.md) |  |
| [QuietNaN](基础API/工具接口/数学计算/NumericLimits/QuietNaN.md) | 返回指定数据类型的安静NaN值（浮点尾数最高位为1）。 |
| [SignalingNaN](基础API/工具接口/数学计算/NumericLimits/SignalingNaN.md) | 返回指定数据类型的发信NaN值（浮点尾数最高位为0）。 |

## 标量计算
| 接口名 | 功能描述 |
| --- | --- |
| [ReadGmByPassDCache(ISASI)](基础API/标量计算/ReadGmByPassDCache(ISASI).md) |  |
| [WriteGmByPassDCache(ISASI)](基础API/标量计算/WriteGmByPassDCache(ISASI).md) |  |
| [Cast（float转bfloat16_t）](基础API/标量计算/Cast（float转bfloat16_t）.md) |  |
| [Cast（float转half/int32_t）](基础API/标量计算/Cast（float转half-int32_t）.md) |  |
| [Cast（多类型转float）](基础API/标量计算/Cast（多类型转float）.md) |  |
| [CountBitsCntSameAsSignBit](基础API/标量计算/CountBitsCntSameAsSignBit.md) |  |
| [CountLeadingZero](基础API/标量计算/CountLeadingZero.md) |  |
| [GetBitCount](基础API/标量计算/GetBitCount.md) |  |
| [GetSFFValue](基础API/标量计算/GetSFFValue.md) |  |
| [Nop](基础API/标量计算/Nop.md) | 用户输入延迟的个数，实现延迟若干个cycle。 |

## 特殊寄存器访问
| 接口名 | 功能描述 |
| --- | --- |
| [GetCtrlSpr(ISASI)](基础API/特殊寄存器访问/GetCtrlSpr(ISASI).md) |  |
| [ResetCtrlSpr(ISASI)](基础API/特殊寄存器访问/ResetCtrlSpr(ISASI).md) |  |
| [SetCtrlSpr(ISASI)](基础API/特殊寄存器访问/SetCtrlSpr(ISASI).md) |  |
| [GetCtrlSpr(ISASI)](基础API/特殊寄存器访问/GetCtrlSpr(ISASI).md) | 读取CTRL寄存器（控制寄存器）特定比特位上的值。 |
| [GetSaturationFlag(ISASI)](基础API/特殊寄存器访问/GetSaturationFlag(ISASI).md) | 获取计算/精度转换时饱和模式的开启状态。 |
| [ResetCtrlSpr(ISASI)](基础API/特殊寄存器访问/ResetCtrlSpr(ISASI).md) | 重置CTRL寄存器（控制寄存器）的特定比特位。 |
| [SetCtrlSpr(ISASI)](基础API/特殊寄存器访问/SetCtrlSpr(ISASI).md) | 对CTRL寄存器（控制寄存器）的特定比特位进行设置。 |
| [SetSaturationFlag(ISASI)](基础API/特殊寄存器访问/SetSaturationFlag(ISASI).md) | 设置饱和开关的标志位，以控制计算/精度转换时饱和模式的开启与关闭。 |

## 矩阵计算（ISASI）
| 接口名 | 功能描述 |
| --- | --- |
| [Mmad](基础API/矩阵计算（ISASI）/Mmad计算/Mmad.md) |  |
| [SetHF32Mode](基础API/矩阵计算（ISASI）/Mmad计算/寄存器配置说明/SetHF32Mode.md) |  |
| [SetHF32TransMode](基础API/矩阵计算（ISASI）/Mmad计算/寄存器配置说明/SetHF32TransMode.md) |  |
| [Load3D](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/Load3D.md) |  |
| [LoadDataWithSparse](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/LoadDataWithSparse.md) |  |
| [LoadDataWithTranspose](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/LoadDataWithTranspose.md) |  |
| [Fill](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/Fill.md) |  |
| [LoadDataUnzip](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/LoadDataUnzip.md) |  |
| [LoadImageToLocal](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/LoadImageToLocal.md) |  |
| [SetFmatrix](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetFmatrix.md) |  |
| [SetLoadDataBoundary](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetLoadDataBoundary.md) |  |
| [SetLoadDataPaddingValue](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetLoadDataPaddingValue.md) |  |
| [SetFixPipeAddr](基础API/矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixPipeAddr.md) |  |
| [SetFixPipeClipRelu](基础API/矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixPipeClipRelu.md) |  |
| [SetFixPipeConfig](基础API/矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixPipeConfig.md) |  |
| [SetFixpipePreQuantFlag](基础API/矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixpipePreQuantFlag.md) |  |
| [GMToL1 Load2D指令搬运](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1-Load2D指令搬运.md) |  |
| [GMToL1连续数据搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1连续数据搬运（DataCopy）.md) |  |
| [GMToL1随路转换-ND2NZ搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1随路转换-ND2NZ搬运（DataCopy）.md) |  |
| [GMToL1高维切分数据搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1高维切分数据搬运（DataCopy）.md) |  |
| [L0C Buffer到UB数据搬运（Fixpipe）](基础API/矩阵计算（ISASI）/矩阵计算的搬出/L0C到UB数据搬运（Fixpipe）.md) |  |
| [L0C到GM数据搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬出/L0C到GM数据搬运（DataCopy）.md) |  |
| [L0C到GM数据搬运（Fixpipe）](基础API/矩阵计算（ISASI）/矩阵计算的搬出/L0C到GM数据搬运（Fixpipe）.md) |  |
| [L0C到L1数据搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬出/L0C到L1数据搬运（DataCopy）.md) |  |
| [L0C到L1数据搬运（Fixpipe）](基础API/矩阵计算（ISASI）/矩阵计算的搬出/L0C到L1数据搬运（Fixpipe）.md) |  |
| [L1ToGM连续数据搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬出/L1到GM数据搬运/L1ToGM连续数据搬运（DataCopy）.md) |  |
| [L1ToGM高维切分数据搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬出/L1到GM数据搬运/L1ToGM高维切分数据搬运（DataCopy）.md) |  |
| [L1到GM数据搬运（仅Atlas A2/A3 系列产品支持）](基础API/矩阵计算（ISASI）/矩阵计算的搬出/L1到GM数据搬运/L1到GM数据搬运.md) |  |
| [LoadUnzipIndex](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/LoadUnzipIndex.md) |  |
| [MmadBitMode](基础API/矩阵计算（ISASI）/矩阵计算/MmadBitMode.md) | MmadBitMode对于MmadParams结构体构造进行了优化，本接口适用于scalar流水成为性能优化瓶颈的场景，支持基础Mmad/MmadMx计算功能。本接口与Mmad/MmadMx接口... |
| [MmadMx](基础API/矩阵计算（ISASI）/矩阵计算/MmadMx.md) | MmadMx（全称Microscaling Mmad）为带有量化系数的矩阵乘法，即左矩阵和右矩阵均有对应的量化系数矩阵，左量化系数矩阵scaleA和右量化系数矩阵scaleB。MmadMx场景中... |
| [MmadWithSparse](基础API/矩阵计算（ISASI）/矩阵计算/MmadWithSparse.md) | MmadWithSparse接口负责完成特殊稀疏矩阵乘加操作。稀疏矩阵是一种特殊类型的矩阵，即矩阵中包含较多的零元素。4：2结构化稀疏要求一个连续的4个权重或激活值的组（通常是张量中的一行或一列... |
| [SetAippFunctions](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetAippFunctions.md) |  |
| [SetFixpipeNz2ndFlag](基础API/矩阵计算（ISASI）/矩阵计算的搬出/寄存器配置说明/SetFixpipeNz2ndFlag.md) |  |
| [SetLoadDataRepeat](基础API/矩阵计算（ISASI）/矩阵计算的搬入/辅助配置接口/SetLoadDataRepeat.md) |  |
| [SetMMColumnMajor/SetMMRowMajor](基础API/矩阵计算（ISASI）/Mmad计算/寄存器配置说明/SetMMColumnMajor-SetMMRowMajor.md) | 控制Mmad/MmadWithSparse优先通过M/N的哪个方向。 |
| [UBToL1连续数据搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1连续数据搬运（DataCopy）.md) | 该接口实现将矩阵从Unified Buffer（UB，TPosition为VECIN/VECCALC/VECOUT）搬运至L1 Buffer，搬运方式为连续搬运，数据搬运时格式和内容保持不变。 |
| [UBToL1随路转换-ND2NZ搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1随路转换-ND2NZ搬运（DataCopy）.md) |  |
| [UBToL1非对齐数据搬运（DataCopyPad）](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1非对齐数据搬运（DataCopyPad）.md) |  |
| [UBToL1高维切分数据搬运（DataCopy）](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1高维切分数据搬运（DataCopy）.md) | 该接口实现将矩阵从Unified Buffer（UB，TPosition为VECIN/VECCALC/VECOUT）搬运至L1 Buffer，支持非连续搬运和连续搬运，数据搬运时格式和内容保持不变。 |
| [矩阵计算（ISASI）](基础API/矩阵计算（ISASI）/矩阵计算（ISASI）.md) |  |
| [矩阵数据搬入至L0-Buffer](基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/矩阵数据搬入至L0-Buffer.md) |  |

## 矩阵计算（TensorAPI）
| 接口名 | 功能描述 |
| --- | --- |
| [Global Memory到L1 Buffer数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L1/GM到L1数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到L1 Buffer。`Copy`接口根据源张量和目的张量的存储位置、数据类型... |
| [L0C Buffer到Global Memory数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬出/L0C到GM数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L0C Buffer中的矩阵计算结果搬运到Global Memory。L0C Buffer中的数据通常为`Mmad`... |
| [L0C Buffer到Unified Buffer数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬出/L0C到UB数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L0C Buffer中的矩阵计算结果搬运到Unified Buffer。L0C Buffer中的数据通常为`Mmad... |
| [L1 Buffer到BiasTable Buffer数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到BiasTable数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的bias数据搬运到BiasTable Buffer，作为Mmad计算中的bias输入。 |
| [L1 Buffer到Fixpipe Buffer数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到Fixpipe数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的量化数据搬运到Fixpipe Buffer。量化数据可用于L0C输出到GM/UB时做随路量化。 |
| [L1 Buffer到L0A Buffer数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到L0A数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的左矩阵数据搬运到L0A Buffer。 |
| [L1 Buffer到L0B Buffer数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到L0B数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的右矩阵数据搬运到L0B Buffer。 |
| [L1 Buffer到L0ScaleA Buffer数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到L0ScaleA数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的左矩阵缩放数据搬运到L0ScaleA Buffer。左矩阵缩放数据在L0ScaleA Buff... |
| [L1 Buffer到L0ScaleB Buffer数据搬运](基础API/矩阵计算（TensorAPI）/矩阵计算的搬入/矩阵数据搬入至L0/L1到L0ScaleB数据搬运（Copy）.md) | Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的右矩阵缩放数据搬运到L0ScaleB Buffer。右矩阵缩放数据在L0ScaleB Buff... |
| [Mmad](基础API/矩阵计算（TensorAPI）/Mmad计算/Mmad.md) | `Mmad`接口用于完成L0A Buffer上左矩阵A和L0B Buffer上右矩阵B的矩阵乘加，结果写入L0C Buffer上结果矩阵C。默认模式为普通矩阵计算。数学表达式为： |
| [矩阵计算（TensorAPI）](基础API/矩阵计算（TensorAPI）/矩阵计算（TensorAPI）.md) |  |

## 缓存控制
| 接口名 | 功能描述 |
| --- | --- |
| [GetICachePreloadStatus(ISASI)](基础API/缓存控制/GetICachePreloadStatus(ISASI).md) |  |
| [ICachePreLoad(ISASI)](基础API/缓存控制/ICachePreLoad(ISASI).md) |  |
| [DataCacheCleanAndInvalid](基础API/缓存控制/DataCacheCleanAndInvalid.md) |  |
| [DataCachePreload](基础API/缓存控制/DataCachePreload.md) |  |

## 调试接口
| 接口名 | 功能描述 |
| --- | --- |
| [CheckLocalMemoryIA(ISASI)](基础API/调试接口/异常检测/CheckLocalMemoryIA(ISASI).md) |  |
| [ascendc_assert](基础API/调试接口/异常检测/ascendc_assert.md) |  |
| [assert](基础API/调试接口/异常检测/assert.md) |  |
| [DumpAccChkPoint](基础API/调试接口/上板打印/DumpAccChkPoint.md) |  |
| [DumpTensor](基础API/调试接口/上板打印/DumpTensor.md) |  |
| [GmAlloc](基础API/调试接口/CPU孪生调试/GmAlloc.md) |  |
| [GmFree](基础API/调试接口/CPU孪生调试/GmFree.md) |  |
| [ICPU_RUN_KF](基础API/调试接口/CPU孪生调试/ICPU_RUN_KF.md) |  |
| [ICPU_SET_TILING_KEY](基础API/调试接口/CPU孪生调试/ICPU_SET_TILING_KEY.md) |  |
| [MarkStamp](基础API/调试接口/性能统计/MarkStamp.md) |  |
| [MetricsProfStart](基础API/调试接口/性能统计/MetricsProfStart.md) |  |
| [MetricsProfStop](基础API/调试接口/性能统计/MetricsProfStop.md) |  |
| [printf](基础API/调试接口/上板打印/printf.md) |  |
| [PrintTimeStamp](基础API/调试接口/上板打印/PrintTimeStamp.md) |  |
| [SetKernelMode](基础API/调试接口/CPU孪生调试/SetKernelMode.md) |  |
| [Trap](基础API/调试接口/异常检测/Trap.md) |  |

## 资源管理
| 接口名 | 功能描述 |
| --- | --- |
| [Alloc](基础API/资源管理/内存管理/LocalMemAllocator/Alloc.md) |  |
| [AllocEventID](基础API/资源管理/Pipe和Que框架/TPipe/AllocEventID.md) |  |
| [AllocTensor](基础API/资源管理/Pipe和Que框架/TSCM/AllocTensor-60.md) |  |
| [AllocTensor](基础API/资源管理/Pipe和Que框架/TQueBind/AllocTensor-64.md) |  |
| [AllocTensor](基础API/资源管理/Pipe和Que框架/TQue/AllocTensor.md) |  |
| [DeQue](基础API/资源管理/Pipe和Que框架/TSCM/DeQue-63.md) |  |
| [DeQue](基础API/资源管理/Pipe和Que框架/TQueBind/DeQue-67.md) |  |
| [DeQue](基础API/资源管理/Pipe和Que框架/TQue/DeQue.md) |  |
| [Destroy](基础API/资源管理/Pipe和Que框架/TPipe/Destroy.md) |  |
| [DestroyWithoutPipeAll](基础API/资源管理/Pipe和Que框架/TPipe/DestroyWithoutPipeAll.md) |  |
| [EnQue](基础API/资源管理/Pipe和Que框架/TSCM/EnQue-62.md) |  |
| [EnQue](基础API/资源管理/Pipe和Que框架/TQueBind/EnQue-66.md) |  |
| [EnQue](基础API/资源管理/Pipe和Que框架/TQue/EnQue.md) |  |
| [FetchEventID](基础API/资源管理/Pipe和Que框架/TPipe/FetchEventID.md) |  |
| [FreeAllEvent](基础API/资源管理/Pipe和Que框架/TQueBind/FreeAllEvent.md) |  |
| [FreeTensor](基础API/资源管理/Pipe和Que框架/TSCM/FreeTensor-61.md) |  |
| [FreeTensor](基础API/资源管理/Pipe和Que框架/TQueBind/FreeTensor-65.md) |  |
| [FreeTensor](基础API/资源管理/Pipe和Que框架/TQue/FreeTensor.md) |  |
| [Get](基础API/资源管理/Pipe和Que框架/TBuf/Get.md) |  |
| [GetBaseAddr](基础API/资源管理/Pipe和Que框架/TPipe/GetBaseAddr.md) |  |
| [GetBufHandle](基础API/资源管理/Pipe和Que框架/自定义TBufPool/GetBufHandle.md) |  |
| [GetCurAddr](基础API/资源管理/Pipe和Que框架/自定义TBufPool/GetCurAddr.md) |  |
| [GetCurAddr](基础API/资源管理/内存管理/LocalMemAllocator/GetCurAddr-72.md) |  |
| [GetCurBufSize](基础API/资源管理/Pipe和Que框架/自定义TBufPool/GetCurBufSize.md) |  |
| [GetSysWorkSpacePtr](基础API/资源管理/临时空间管理/workspace/GetSysWorkSpacePtr.md) |  |
| [GetTensorCountInQue](基础API/资源管理/Pipe和Que框架/TQueBind/GetTensorCountInQue-70.md) |  |
| [GetTensorCountInQue](基础API/资源管理/Pipe和Que框架/TQue/GetTensorCountInQue.md) |  |
| [GetTPipePtr](基础API/资源管理/Pipe和Que框架/GetTPipePtr.md) |  |
| [GetUserWorkspace](基础API/资源管理/临时空间管理/workspace/GetUserWorkspace.md) |  |
| [GetWithOffset](基础API/资源管理/Pipe和Que框架/TBuf/GetWithOffset.md) |  |
| [HasIdleBuffer](基础API/资源管理/Pipe和Que框架/TQueBind/HasIdleBuffer-71.md) |  |
| [HasIdleBuffer](基础API/资源管理/Pipe和Que框架/TQue/HasIdleBuffer.md) |  |
| [HasTensorInQue](基础API/资源管理/Pipe和Que框架/TQueBind/HasTensorInQue-69.md) |  |
| [HasTensorInQue](基础API/资源管理/Pipe和Que框架/TQue/HasTensorInQue.md) |  |
| [Init](基础API/资源管理/Pipe和Que框架/TPipe/Init.md) |  |
| [Init](基础API/资源管理/Pipe和Que框架/自定义TBufPool/Init-59.md) |  |
| [InitBuffer](基础API/资源管理/Pipe和Que框架/TPipe/InitBuffer.md) |  |
| [InitBuffer](基础API/资源管理/Pipe和Que框架/TBufPool/InitBuffer-56.md) |  |
| [InitBufHandle](基础API/资源管理/Pipe和Que框架/TQueBind/InitBufHandle.md) |  |
| [InitBufPool](基础API/资源管理/Pipe和Que框架/TPipe/InitBufPool.md) |  |
| [InitBufPool](基础API/资源管理/Pipe和Que框架/TBufPool/InitBufPool-55.md) |  |
| [InitSpmBuffer](基础API/资源管理/Pipe和Que框架/TPipe/InitSpmBuffer.md) |  |
| [InitStartBufHandle](基础API/资源管理/Pipe和Que框架/TQueBind/InitStartBufHandle.md) |  |
| [LocalMemAllocator](基础API/资源管理/内存管理/LocalMemAllocator/LocalMemAllocator.md) |  |
| [PopStackBuffer](基础API/资源管理/临时空间管理/PopStackBuffer.md) |  |
| [ReadSpmBuffer](基础API/资源管理/Pipe和Que框架/TPipe/ReadSpmBuffer.md) |  |
| [ReleaseEventID](基础API/资源管理/Pipe和Que框架/TPipe/ReleaseEventID.md) |  |
| [Reset](基础API/资源管理/Pipe和Que框架/TPipe/Reset.md) |  |
| [Reset](基础API/资源管理/Pipe和Que框架/TBufPool/Reset-57.md) |  |
| [Reset](基础API/资源管理/Pipe和Que框架/自定义TBufPool/Reset-58.md) |  |
| [SetCurAddr](基础API/资源管理/Pipe和Que框架/自定义TBufPool/SetCurAddr.md) |  |
| [SetCurBufSize](基础API/资源管理/Pipe和Que框架/自定义TBufPool/SetCurBufSize.md) |  |
| [SetSysWorkSpace](基础API/资源管理/临时空间管理/workspace/SetSysWorkSpace.md) |  |
| [TBuf](基础API/资源管理/Pipe和Que框架/TBuf/TBuf.md) |  |
| [TBufPool](基础API/资源管理/Pipe和Que框架/TBufPool/TBufPool.md) |  |
| [TPipe](基础API/资源管理/Pipe和Que框架/TPipe/TPipe.md) |  |
| [TQue](基础API/资源管理/Pipe和Que框架/TQue/TQue.md) |  |
| [TQueBind](基础API/资源管理/Pipe和Que框架/TQueBind/TQueBind.md) |  |
| [TSCM](基础API/资源管理/Pipe和Que框架/TSCM/TSCM.md) |  |
| [VacantInQue](基础API/资源管理/Pipe和Que框架/TQueBind/VacantInQue-68.md) |  |
| [VacantInQue](基础API/资源管理/Pipe和Que框架/TQue/VacantInQue.md) |  |
| [WriteSpmBuffer](基础API/资源管理/Pipe和Que框架/TPipe/WriteSpmBuffer.md) |  |
