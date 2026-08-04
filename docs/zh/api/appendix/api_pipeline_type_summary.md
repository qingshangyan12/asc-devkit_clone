# api_pipeline_type_summary<a name="ZH-CN_TOPIC_0000002533354434"></a>

本节汇总介绍了一些主要的API的流水类型，以下内容针对如下型号生效：

<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
<!-- end id4 -->

<!-- npu="950" id5 -->
针对Ascend 950PR/Ascend 950DT：<br>单个VF是PIPE_V流水，用户不需要管理VF和VF之间的同步，VF内的不同的Reg矢量接口存在数据依赖时需要调用[LocalMemBar](../SIMD-API/basic_api/reg_vector_compute/sync_control/LocalMemBar.md)进行同步。
<!-- end id5 -->

**表1**  memory_vector_computeapi_pipeline_type_summary

<a name="table132070121817"></a>
| API类型 | API接口名称 | 执行核（AIC/AIV） | 流水类型 |
| --- | --- | --- | --- |
| basic_arithmetic | Exp | AIV | PIPE_V |
| basic_arithmetic | Ln | AIV | PIPE_V |
| basic_arithmetic | Abs | AIV | PIPE_V |
| basic_arithmetic | Reciprocal | AIV | PIPE_V |
| basic_arithmetic | Sqrt | AIV | PIPE_V |
| basic_arithmetic | Rsqrt | AIV | PIPE_V |
| basic_arithmetic | Relu | AIV | PIPE_V |
| basic_arithmetic | Add | AIV | PIPE_V |
| basic_arithmetic | Sub | AIV | PIPE_V |
| basic_arithmetic | Mul | AIV | PIPE_V |
| basic_arithmetic | Div | AIV | PIPE_V |
| basic_arithmetic | Max | AIV | PIPE_V |
| basic_arithmetic | Min | AIV | PIPE_V |
| basic_arithmetic | Adds | AIV | PIPE_V |
| basic_arithmetic | Adds（灵活标量位置） | AIV | PIPE_V |
| basic_arithmetic | Muls | AIV | PIPE_V |
| basic_arithmetic | Muls（灵活标量位置） | AIV | PIPE_V |
| basic_arithmetic | Maxs | AIV | PIPE_V |
| basic_arithmetic | Maxs（灵活标量位置） | AIV | PIPE_V |
| basic_arithmetic | Mins | AIV | PIPE_V |
| basic_arithmetic | Mins（灵活标量位置） | AIV | PIPE_V |
| basic_arithmetic | Subs（灵活标量位置） | AIV | PIPE_V |
| basic_arithmetic | Divs（灵活标量位置） | AIV | PIPE_V |
| basic_arithmetic | Neg | AIV | PIPE_V |
| basic_arithmetic | Mull | AIV | PIPE_V |
| basic_arithmetic | Prelu | AIV | PIPE_V |
| basic_arithmetic | LeakyRelu | AIV | PIPE_V |
| basic_arithmetic | BilinearInterpolation | AIV | PIPE_V |
| basic_arithmetic | Reciprocal | AIV | PIPE_V |
| logical_compute | Not | AIV | PIPE_V |
| logical_compute | And | AIV | PIPE_V |
| logical_compute | Or | AIV | PIPE_V |
| logical_compute | ShiftLeft | AIV | PIPE_V |
| logical_compute | ShiftRight | AIV | PIPE_V |
| logical_compute | Ands | AIV | PIPE_V |
| logical_compute | Ors | AIV | PIPE_V |
| logical_compute | ShiftLeft（左移位数为Tensor） | AIV | PIPE_V |
| logical_compute | ShiftRights（左移位数为Tensor） | AIV | PIPE_V |
| composite_compute | Axpy | AIV | PIPE_V |
| composite_compute | MulAddDst | AIV | PIPE_V |
| composite_compute | MulCast | AIV | PIPE_V |
| composite_compute | AddRelu | AIV | PIPE_V |
| composite_compute | AddReluCast | AIV | PIPE_V |
| composite_compute | SubRelu | AIV | PIPE_V |
| composite_compute | SubReluCast | AIV | PIPE_V |
| composite_compute | FusedMulAdd | AIV | PIPE_V |
| composite_compute | MulAddRelu | AIV | PIPE_V |
| composite_compute | CastDequant | AIV | PIPE_V |
| composite_compute | AddDeqRelu | AIV | PIPE_V |
| composite_compute | MulsCast | AIV | PIPE_V |
| composite_compute | AbsSub | AIV | PIPE_V |
| composite_compute | ExpSub | AIV | PIPE_V |
| compare_and_select | Compare | AIV | PIPE_V |
| compare_and_select | Compare（结果存入寄存器） | AIV | PIPE_V |
| compare_and_select | Compares | AIV | PIPE_V |
| compare_and_select | Compares（灵活标量位置） | AIV | PIPE_V |
| compare_and_select | GetCmpMask(ISASI) | AIV | PIPE_V |
| compare_and_select | SetCmpMask(ISASI) | AIV | PIPE_V |
| compare_and_select | Select | AIV | PIPE_V |
| compare_and_select | Select（灵活标量位置） | AIV | PIPE_V |
| compare_and_select | GatherMask | AIV | PIPE_V |
| type_conversion | Cast | AIV | PIPE_V |
| type_conversion | Truncate | AIV | PIPE_V |
| reduction_compute | ReduceDataBlock | AIV | PIPE_V |
| reduction_compute | ReduceRepeat | AIV | PIPE_V |
| reduction_compute | ReducePairElem | AIV | PIPE_V |
| reduction_compute | ReduceMax | AIV | PIPE_V |
| reduction_compute | ReduceMin | AIV | PIPE_V |
| reduction_compute | ReduceSum | AIV | PIPE_V |
| reduction_compute | GetReduceRepeatSumSpr | AIV | PIPE_S |
| reduction_compute | GetReduceRepeatMaxMinSpr | AIV | PIPE_S |
| data_layout_conversion | Transpose | AIV | PIPE_V |
| data_layout_conversion | TransDataTo5HD | AIV | PIPE_V |
| data_padding | Brcb | AIV | PIPE_V |
| data_padding | Duplicate | AIV | PIPE_V |
| data_padding | CreateVecIndex | AIV | PIPE_V |
| 排序组合 | Sort32 | AIV | PIPE_V |
| 排序组合 | MrgSort | AIV | PIPE_V |
| 排序组合 | GetMrgSortResult | AIV | PIPE_S |
| scatter_gather | Gather | AIV | PIPE_V |
| scatter_gather | Gatherb | AIV | PIPE_V |
| scatter_gather | Scatter | AIV | PIPE_V |
| mask_operations | SetMaskCount | AIV | PIPE_S |
| mask_operations | SetMaskNorm | AIV | PIPE_S |
| mask_operations | SetVectorMask | AIV | PIPE_S |
| mask_operations | ResetMask | AIV | PIPE_S |
| data_reorder | Interleave | AIV | PIPE_V |
| data_reorder | DeInterleave | AIV | PIPE_V |
| data_move | DataCopy(GM->UB) | AIV | PIPE_MTE2 |
| data_move | DataCopy(UB->GM) | AIV | PIPE_MTE3 |
| data_move | DataCopyPad(GM->UB) | AIV | PIPE_MTE2 |
| data_move | DataCopyPad(UB->GM) | AIV | PIPE_MTE3 |
| data_move | DataCopy(UB->UB) | AIV | PIPE_V |
| data_move | Copy | AIV | PIPE_V |

**表2**  Cube api_pipeline_type_summary

<a name="table12311219174812"></a>
| API接口名称 | 执行核（AIC/AIV） | 流水类型 |
| --- | --- | --- |
| DataCopy(GM->L1) | AIC | PIPE_MTE2 |
| DataCopyPad(GM->L1) | AIC | PIPE_MTE2 |
| LoadData(GM->L1) | AIC | PIPE_MTE2 |
| LoadData(GM->L0) | AIC | PIPE_MTE2 |
| LoadData(L1->L0) | AIC | PIPE_MTE1 |
| LoadDataWithTranspose(L1->L0) | AIC | PIPE_MTE1 |
| LoadDataWithSparse(L1->L0) | AIC | PIPE_MTE1 |
| DataCopy(L1->BT) | AIC | PIPE_MTE1 |
| DataCopy(L1->FB) | AIC | PIPE_FIX |
| DataCopyPad(UB->L1) | AIC | <!-- npu="950" id6 -->针对Ascend 950PR/Ascend 950DT：<br>PIPE_MTE3。<br><!-- end id6 --><!-- npu="A3" id7 -->针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：<br>PIPE_MTE3、PIPE_MTE2。<br><!-- end id7 --><!-- npu="910b" id8 -->针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：<br>PIPE_MTE3、PIPE_MTE2。<br><!-- end id8 --> |
| Fill(L1) | AIC | PIPE_MTE2 |
| Fill(L0) | AIC | PIPE_MTE1 |
| SetFmatrix | AIC | PIPE_S |
| SetLoadDataBoundary | AIC | PIPE_S |
| SetLoadDataRepeat | AIC | PIPE_S |
| SetLoadDataPaddingValue | AIC | PIPE_S |
| Mmad | AIC | PIPE_M |
| MmadMx | AIC | PIPE_M |
| MmadWithSparse | AIC | PIPE_M |
| SetMMColumnMajor/SetMMRowMajor | AIC | PIPE_S |
| SetHF32Mode | AIC | PIPE_S |
| SetHF32TransMode | AIC | PIPE_S |
| DataCopy(L0C->GM) | AIC | PIPE_FIX |
| DataCopy(L0C->L1) | AIC | PIPE_FIX |
| DataCopy(L1->GM) | AIC | PIPE_MTE3 |
| Fixpipe(L0C->GM) | AIC | PIPE_FIX |
| Fixpipe(L0C->L1) | AIC | PIPE_FIX |
| Fixpipe(L0C->UB) | AIC | PIPE_FIX |
| DataCopyL1ToUB(L1->UB) | AIC | PIPE_MTE1 |
| SetFixPipeConfig | AIC | PIPE_S |
| SetFixpipePreQuantFlag | AIC | PIPE_S |
| SetFixpipeNz2ndFlag | AIC | PIPE_S |
| SetLreluALPHA | AIC | PIPE_S |

**表3**  scalar_compute指令流水类型汇总

| API接口名称 | 流水类型 |
| --- | --- |
| GetBitCount | PIPE_S |
| CountLeadingZero | PIPE_S |
| GetSFFValue | PIPE_S |
| CountBitsCntSameAsSignBit | PIPE_S |
| Cast（float转half、int32_t） | PIPE_S |
| Cast（float转bfloat16_t） | PIPE_S |
| Cast（bfloat16_t转float） | PIPE_S |
| Nop | PIPE_S |

**表4**  同步指令流水类型汇总

<a name="table1054353913238"></a>
| API接口名称 | 流水类型 |
| --- | --- |
| SetFlag/WaitFlag | PIPE_S |
| PipeBarrier | PIPE_S |
| DataSyncBarrier | PIPE_S |
| CrossCoreSetFlag | PIPE_S |
| CrossCoreWaitFlag | PIPE_S |
| SyncAll（硬件sync_interface） | PIPE_S |
| SetNextTaskStart | PIPE_S |
| WaitPreTaskEnd | PIPE_S |
| Mutex::Lock | PIPE_S |
| Mutex::UnLock | PIPE_S |

**表5**  atomic_operations流水类型汇总

<a name="table25871912193814"></a>
| API接口名称 | 流水类型 |
| --- | --- |
| SetAtomicAdd | PIPE_S |
| SetAtomicMax | PIPE_S |
| SetAtomicMin | PIPE_S |
| SetAtomicType | PIPE_S |
| DisableDmaAtomic | PIPE_S |
| GetStoreAtomicConfig(ISASI) | PIPE_S |
| SetStoreAtomicConfig(ISASI) | PIPE_S |
| AtomicAdd | PIPE_S |
| AtomicMax | PIPE_S |
| AtomicMin | PIPE_S |
| AtomicCas | PIPE_S |
| AtomicExch | PIPE_S |

**表6**  tool_interface流水类型汇总

<a name="table1147331317457"></a>
| API接口名称 | 流水类型 |
| --- | --- |
| InitSocState | PIPE_S |
| GetBlockNum | PIPE_S |
| GetBlockIdx | PIPE_S |
| GetTaskRatio | PIPE_S |
| GetSubBlockNum | PIPE_S |
| GetSubBlockIdx | PIPE_S |
| GetDataBlockSizeInBytes | PIPE_S |
| GetSystemCycle | PIPE_S |
| GetProgramCounter | PIPE_S |
| GetArchVersion | PIPE_S |
| GetUBSizeInBytes | PIPE_S |
| GetRuntimeUBSize | PIPE_S |
| GetVecLen | PIPE_S |
| CeilDivision | PIPE_S |
| GetSsbufBaseAddr | PIPE_S |

**表7**  cache_control接口流水类型汇总

| API接口名称 | 流水类型 |
| --- | --- |
| ReadGmByPassDCache(ISASI) | PIPE_S |
| WriteGmByPassDCache(ISASI) | PIPE_S |
| DataCachePreload | PIPE_S |
| DataCacheCleanAndInvalid | PIPE_S |
| ICachePreLoad(ISASI) | PIPE_S |
| GetICachePreloadStatus(ISASI) | PIPE_S |

