# API流水类型汇总<a name="ZH-CN_TOPIC_0000002533354434"></a>

<!-- npu="950,A3,910b" id1 -->
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

**表1**  Memory矢量计算API流水类型汇总

<a name="table132070121817"></a>
| API类型 | API接口名称 | 执行核（AIC/AIV） | 流水类型 |
| --- | --- | --- | --- |
| 基础算术 | Exp | AIV | PIPE_V |
| 基础算术 | Ln | AIV | PIPE_V |
| 基础算术 | Abs | AIV | PIPE_V |
| 基础算术 | Reciprocal | AIV | PIPE_V |
| 基础算术 | Sqrt | AIV | PIPE_V |
| 基础算术 | Rsqrt | AIV | PIPE_V |
| 基础算术 | Relu | AIV | PIPE_V |
| 基础算术 | Sub | AIV | PIPE_V |
| 基础算术 | Mul | AIV | PIPE_V |
| 基础算术 | Div | AIV | PIPE_V |
| 基础算术 | Max | AIV | PIPE_V |
| 基础算术 | Min | AIV | PIPE_V |
| 基础算术 | Adds | AIV | PIPE_V |
| <!-- npu="950" id5 --> 基础算术 | Adds（灵活标量位置） | AIV | PIPE_V <!-- end id5 --> |
| 基础算术 | Muls | AIV | PIPE_V |
| <!-- npu="950" id6 --> 基础算术 | Muls（灵活标量位置） | AIV | PIPE_V <!-- end id6 --> |
| 基础算术 | Maxs | AIV | PIPE_V |
| <!-- npu="950" id7 --> 基础算术 | Maxs（灵活标量位置） | AIV | PIPE_V <!-- end id7 --> |
| 基础算术 | Mins | AIV | PIPE_V |
| <!-- npu="950" id8 --> 基础算术 | Mins（灵活标量位置） | AIV | PIPE_V <!-- end id8 --> |
| <!-- npu="950" id9 --> 基础算术 | Subs（灵活标量位置） | AIV | PIPE_V <!-- end id9 --> |
| <!-- npu="950" id10 --> 基础算术 | Divs（灵活标量位置） | AIV | PIPE_V <!-- end id10 --> |
| <!-- npu="950" id11 --> 基础算术 | Neg | AIV | PIPE_V <!-- end id11 --> |
| <!-- npu="950" id12 --> 基础算术 | Mull | AIV | PIPE_V <!-- end id12 --> |
| <!-- npu="950" id13 --> 基础算术 | Prelu | AIV | PIPE_V <!-- end id13 --> |
| 基础算术 | LeakyRelu | AIV | PIPE_V |
| 逻辑计算 | Not | AIV | PIPE_V |
| 逻辑计算 | And | AIV | PIPE_V |
| 逻辑计算 | Or | AIV | PIPE_V |
| 逻辑计算 | ShiftLeft | AIV | PIPE_V |
| 逻辑计算 | ShiftRight | AIV | PIPE_V |
| <!-- npu="950" id14 --> 逻辑计算 | Ands | AIV | PIPE_V <!-- end id14 --> |
| <!-- npu="950" id15 --> 逻辑计算 | Ors | AIV | PIPE_V <!-- end id15 --> |
| 复合计算 | Axpy | AIV | PIPE_V |
| 复合计算 | MulAddDst | AIV | PIPE_V |
| 复合计算 | MulCast | AIV | PIPE_V |
| 复合计算 | AddRelu | AIV | PIPE_V |
| 复合计算 | AddReluCast | AIV | PIPE_V |
| 复合计算 | SubRelu | AIV | PIPE_V |
| 复合计算 | SubReluCast | AIV | PIPE_V |
| 复合计算 | FusedMulAdd | AIV | PIPE_V |
| 复合计算 | MulAddRelu | AIV | PIPE_V |
| 复合计算 | CastDequant | AIV | PIPE_V |
| 复合计算 | AddDeqRelu | AIV | PIPE_V |
| <!-- npu="950" id16 --> 复合计算 | MulsCast | AIV | PIPE_V <!-- end id16 --> |
| <!-- npu="950" id17 --> 复合计算 | AbsSub | AIV | PIPE_V <!-- end id17 --> |
| <!-- npu="950" id18 --> 复合计算 | ExpSub | AIV | PIPE_V <!-- end id18 --> |
| 比较与选择 | Compare | AIV | PIPE_V |
| 比较与选择 | Compare（结果存入寄存器） | AIV | PIPE_V |
| 比较与选择 | Compares | AIV | PIPE_V |
| 比较与选择 | GetCmpMask(ISASI) | AIV | PIPE_V |
| 比较与选择 | SetCmpMask(ISASI) | AIV | PIPE_V |
| 比较与选择 | Select | AIV | PIPE_V |
| 比较与选择 | GatherMask | AIV | PIPE_V |
| 类型转换 | Cast | AIV | PIPE_V |
| <!-- npu="950" id19 --> 类型转换 | Truncate | AIV | PIPE_V <!-- end id19 --> |
| 归约计算 | ReduceDataBlock | AIV | PIPE_V |
| 归约计算 | ReduceRepeat | AIV | PIPE_V |
| 归约计算 | ReducePairElem | AIV | PIPE_V |
| 归约计算 | ReduceMax | AIV | PIPE_V |
| 归约计算 | ReduceMin | AIV | PIPE_V |
| 归约计算 | ReduceSum | AIV | PIPE_V |
| 归约计算 | GetReduceRepeatSumSpr | AIV | PIPE_S |
| <!-- npu="A3,910b" id20 --> 归约计算 | GetReduceRepeatMaxMinSpr | AIV | PIPE_S <!-- end id20 --> |
| 数据排布格式转换 | Transpose | AIV | PIPE_V |
| 数据排布格式转换 | TransDataTo5HD | AIV | PIPE_V |
| 数据填充 | Brcb | AIV | PIPE_V |
| 数据填充 | Duplicate | AIV | PIPE_V |
| 数据填充 | CreateVecIndex | AIV | PIPE_V |
| 排序组合 | Sort32 | AIV | PIPE_V |
| 排序组合 | MrgSort | AIV | PIPE_V |
| 排序组合 | GetMrgSortResult | AIV | PIPE_S |
| 离散与聚合 | Gather | AIV | PIPE_V |
| 离散与聚合 | Gatherb | AIV | PIPE_V |
| <!-- npu="950" id21 --> 离散与聚合 | Scatter | AIV | PIPE_V <!-- end id21 --> |
| 掩码操作 | SetMaskCount | AIV | PIPE_S |
| 掩码操作 | SetMaskNorm | AIV | PIPE_S |
| 掩码操作 | SetVectorMask | AIV | PIPE_S |
| 掩码操作 | ResetMask | AIV | PIPE_S |
| <!-- npu="950" id22 --> 数据重排 | Interleave | AIV | PIPE_V <!-- end id22 --> |
| <!-- npu="950" id23 --> 数据重排 | DeInterleave | AIV | PIPE_V <!-- end id23 --> |
| 数据搬运 | DataCopy(GM->UB) | AIV | PIPE_MTE2 |
| 数据搬运 | DataCopy(UB->GM) | AIV | PIPE_MTE3 |
| 数据搬运 | DataCopyPad(GM->UB) | AIV | PIPE_MTE2 |
| 数据搬运 | DataCopyPad(UB->GM) | AIV | PIPE_MTE3 |
| 数据搬运 | DataCopy(UB->UB) | AIV | PIPE_V |
| 数据搬运 | Copy | AIV | PIPE_V |

**表2**  Cube API流水类型汇总

<a name="table12311219174812"></a>
| API接口名称 | 执行核（AIC/AIV） | 流水类型 |
| --- | --- | --- |
| DataCopy(GM->L1) | AIC | PIPE_MTE2 |
| <!-- npu="950" id24 --> DataCopyPad(GM->L1) | AIC | PIPE_MTE2 <!-- end id24 --> |
| LoadData(GM->L1) | AIC | PIPE_MTE2 |
| <!-- npu="A3,910b" id25 --> LoadData(GM->L0) | AIC | PIPE_MTE2 <!-- end id25 --> |
| LoadData(L1->L0) | AIC | PIPE_MTE1 |
| LoadDataWithTranspose(L1->L0) | AIC | PIPE_MTE1 |
| <!-- npu="A3,910b" id26 --> LoadDataWithSparse(L1->L0) | AIC | PIPE_MTE1 <!-- end id26 --> |
| DataCopy(L1->BT) | AIC | PIPE_MTE1 |
| DataCopy(L1->FB) | AIC | PIPE_FIX |
| <!-- npu="A3,910b" id27 --> 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：<br>DataCopyPad(UB->L1) | AIC | PIPE_MTE3、PIPE_MTE2 <!-- end id27 --> |
| <!-- npu="950" id28 --> 针对Ascend 950PR/Ascend 950DT：<br>DataCopyPad(UB->L1) | AIC | PIPE_MTE3 <!-- end id28 --> |
| Fill(L1) | AIC | PIPE_MTE2 |
| Fill(L0) | AIC | PIPE_MTE1 |
| SetFmatrix | AIC | PIPE_S |
| SetLoadDataBoundary | AIC | PIPE_S |
| SetLoadDataRepeat | AIC | PIPE_S |
| SetLoadDataPaddingValue | AIC | PIPE_S |
| Mmad | AIC | PIPE_M |
| <!-- npu="950" id29 --> MmadMx | AIC | PIPE_M <!-- end id29 --> |
| <!-- npu="A3,910b" id30 --> MmadWithSparse | AIC | PIPE_M <!-- end id30 --> |
| SetMMColumnMajor/SetMMRowMajor | AIC | PIPE_S |
| SetHF32Mode | AIC | PIPE_S |
| SetHF32TransMode | AIC | PIPE_S |
| DataCopy(L0C->GM) | AIC | PIPE_FIX |
| DataCopy(L0C->L1) | AIC | PIPE_FIX |
| <!-- npu="A3,910b" id31 --> DataCopy(L1->GM) | AIC | PIPE_MTE3 <!-- end id31 --> |
| Fixpipe(L0C->GM) | AIC | PIPE_FIX |
| Fixpipe(L0C->L1) | AIC | PIPE_FIX |
| <!-- npu="950" id32 --> Fixpipe(L0C->UB) | AIC | PIPE_FIX <!-- end id32 --> |
| <!-- npu="950" id33 --> DataCopyL1ToUB(L1->UB) | AIC | PIPE_MTE1 <!-- end id33 --> |
| SetFixPipeConfig | AIC | PIPE_S |
| SetFixpipePreQuantFlag | AIC | PIPE_S |
| SetFixpipeNz2ndFlag | AIC | PIPE_S |
| <!-- npu="A3,910b" id34 --> SetLreluALPHA | AIC | PIPE_S <!-- end id34 --> |

**表3**  标量计算指令流水类型汇总

| API接口名称 | 流水类型 |
| --- | --- |
| GetBitCount | PIPE_S |
| CountLeadingZero | PIPE_S |
| GetSFFValue | PIPE_S |
| CountBitsCntSameAsSignBit | PIPE_S |
| Cast（float转half、int32_t） | PIPE_S |
| Cast（float转bfloat16_t） | PIPE_S |
| Cast（bfloat16_t转float） | PIPE_S |
| ReadGmByPassDCache(ISASI) | PIPE_S |
| WriteGmByPassDCache(ISASI) | PIPE_S |
| <!-- npu="950" id40 --> Nop | PIPE_S <!-- end id40 --> |

**表4**  同步指令流水类型汇总

<a name="table1054353913238"></a>
| API接口名称 | 流水类型 |
| --- | --- |
| SetFlag/WaitFlag | PIPE_S |
| PipeBarrier | PIPE_S |
| DataSyncBarrier | PIPE_S |
| CrossCoreSetFlag | PIPE_S |
| CrossCoreWaitFlag | PIPE_S |
| SyncAll（硬件同步接口） | PIPE_S |
| SetNextTaskStart | PIPE_S |
| WaitPreTaskEnd | PIPE_S |
| <!-- npu="950" id41 --> Mutex::Lock | PIPE_S <!-- end id41 --> |
| <!-- npu="950" id42 --> Mutex::UnLock | PIPE_S <!-- end id42 --> |

**表5**  原子操作流水类型汇总

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
| <!-- npu="950" id35 --> AtomicAdd | PIPE_S <!-- end id35 --> |
| <!-- npu="950" id36 --> AtomicMax | PIPE_S <!-- end id36 --> |
| <!-- npu="950" id37 --> AtomicMin | PIPE_S <!-- end id37 --> |
| <!-- npu="950" id38 --> AtomicCas | PIPE_S <!-- end id38 --> |
| <!-- npu="950" id39 --> AtomicExch | PIPE_S <!-- end id39 --> |

**表6**  工具接口流水类型汇总

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
| <!-- npu="950" id43 --> GetUBSizeInBytes | PIPE_S <!-- end id43 --> |
| <!-- npu="950" id44 --> GetRuntimeUBSize | PIPE_S <!-- end id44 --> |
| <!-- npu="950" id45 --> GetVecLen | PIPE_S <!-- end id45 --> |
| <!-- npu="950" id46 --> CeilDivision | PIPE_S <!-- end id46 --> |
| <!-- npu="950" id47 --> GetSsbufBaseAddr | PIPE_S <!-- end id47 --> |

**表7**  缓存控制接口流水类型汇总

| API接口名称 | 流水类型 |
| --- | --- |
| DataCachePreload | PIPE_S |
| DataCacheCleanAndInvalid | PIPE_S |
| ICachePreLoad(ISASI) | PIPE_S |
| GetICachePreloadStatus(ISASI) | PIPE_S |
<!-- end id1 -->
