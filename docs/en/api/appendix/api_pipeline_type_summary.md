# API Pipeline Type Summary<a name="ZH-CN_TOPIC_0000002533354434"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-29T10:32:49.084Z pushedAt=2026-10-29T10:52:37.446Z nl=2 -->

This section summarizes the pipeline types of some major APIs. The following content applies to the following models:

<!-- npu="950" id2 -->
- Ascend 950PR & 950DT products
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 products
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 products
<!-- end id4 -->

<!-- npu="950" id5 -->
For Ascend 950PR&950DT products:<br>A single VF runs on the PIPE_V pipeline. You do not need to manage synchronization between VFs. When different Reg vector APIs within a VF have data dependencies, call [LocalMemBar](../SIMD-API/basic_api/reg_vector_compute/sync_control/LocalMemBar.md) for synchronization.
<!-- end id5 -->

**Table 1**  Memory vector computation API pipeline type summary

<a name="table132070121817"></a>

| API Type | API | Execution Core (AIC/AIV) | Pipeline Type |
| --- | --- | --- | --- |
| Basic arithmetic | Exp | AIV | PIPE_V |
| Basic arithmetic | Ln | AIV | PIPE_V |
| Basic arithmetic | Abs | AIV | PIPE_V |
| Basic arithmetic | Reciprocal | AIV | PIPE_V |
| Basic arithmetic | Sqrt | AIV | PIPE_V |
| Basic arithmetic | Rsqrt | AIV | PIPE_V |
| Basic arithmetic | Relu | AIV | PIPE_V |
| Basic arithmetic | Add | AIV | PIPE_V |
| Basic arithmetic | Sub | AIV | PIPE_V |
| Basic arithmetic | Mul | AIV | PIPE_V |
| Basic arithmetic | Div | AIV | PIPE_V |
| Basic arithmetic | Max | AIV | PIPE_V |
| Basic arithmetic | Min | AIV | PIPE_V |
| Basic arithmetic | Adds | AIV | PIPE_V |
| Basic arithmetic | Adds (flexible scalar position) | AIV | PIPE_V |
| Basic arithmetic | Muls | AIV | PIPE_V |
| Basic arithmetic | Muls (flexible scalar position) | AIV | PIPE_V |
| Basic arithmetic | Maxs | AIV | PIPE_V |
| Basic arithmetic | Maxs (flexible scalar position) | AIV | PIPE_V |
| Basic arithmetic | Mins | AIV | PIPE_V |
| Basic arithmetic | Mins (flexible scalar position) | AIV | PIPE_V |
| Basic arithmetic | Subs (flexible scalar position) | AIV | PIPE_V |
| Basic arithmetic | Divs (flexible scalar position) | AIV | PIPE_V |
| Basic arithmetic | Neg | AIV | PIPE_V |
| Basic arithmetic | Mull | AIV | PIPE_V |
| Basic arithmetic | Prelu | AIV | PIPE_V |
| Basic arithmetic | LeakyRelu | AIV | PIPE_V |
| Basic arithmetic | BilinearInterpolation | AIV | PIPE_V |
| Basic arithmetic | Reciprocal | AIV | PIPE_V |
| Logical computation | Not | AIV | PIPE_V |
| Logical computation | And | AIV | PIPE_V |
| Logical computation | Or | AIV | PIPE_V |
| Logical computation | ShiftLeft | AIV | PIPE_V |
| Logical computation | ShiftRight | AIV | PIPE_V |
| Logical computation | Ands | AIV | PIPE_V |
| Logical computation | Ors | AIV | PIPE_V |
| Logical computation | ShiftLeft (left shift count is a Tensor) | AIV | PIPE_V |
| Logical computation | ShiftRights (right shift count is a Tensor) | AIV | PIPE_V |
| Compound computation | Axpy | AIV | PIPE_V |
| Compound computation | MulAddDst | AIV | PIPE_V |
| Compound computation | MulCast | AIV | PIPE_V |
| Compound computation | AddRelu | AIV | PIPE_V |
| Compound computation | AddReluCast | AIV | PIPE_V |
| Compound computation | SubRelu | AIV | PIPE_V |
| Compound computation | SubReluCast | AIV | PIPE_V |
| Compound computation | FusedMulAdd | AIV | PIPE_V |
| Compound computation | MulAddRelu | AIV | PIPE_V |
| Compound computation | CastDequant | AIV | PIPE_V |
| Compound computation | AddDeqRelu | AIV | PIPE_V |
| Compound computation | MulsCast | AIV | PIPE_V |
| Compound computation | AbsSub | AIV | PIPE_V |
| Compound computation | ExpSub | AIV | PIPE_V |
| Comparison and selection | Compare | AIV | PIPE_V |
| Comparison and selection | Compare (result stored in a register) | AIV | PIPE_V |
| Comparison and selection | Compares | AIV | PIPE_V |
| Comparison and selection | Compares (flexible scalar position) | AIV | PIPE_V |
| Comparison and selection | GetCmpMask(ISASI) | AIV | PIPE_V |
| Comparison and selection | SetCmpMask(ISASI) | AIV | PIPE_V |
| Comparison and selection | Select | AIV | PIPE_V |
| Comparison and selection | Select (flexible scalar position) | AIV | PIPE_V |
| Comparison and selection | GatherMask | AIV | PIPE_V |
| Type conversion | Cast | AIV | PIPE_V |
| Type conversion | Truncate | AIV | PIPE_V |
| Reduction computation | ReduceDataBlock | AIV | PIPE_V |
| Reduction computation | ReduceRepeat | AIV | PIPE_V |
| Reduction computation | ReducePairElem | AIV | PIPE_V |
| Reduction computation | ReduceMax | AIV | PIPE_V |
| Reduction computation | ReduceMin | AIV | PIPE_V |
| Reduction computation | ReduceSum | AIV | PIPE_V |
| Reduction computation | GetReduceRepeatSumSpr | AIV | PIPE_S |
| Reduction computation | GetReduceRepeatMaxMinSpr | AIV | PIPE_S |
| Data layout conversion | Transpose | AIV | PIPE_V |
| Data layout conversion | TransDataTo5HD | AIV | PIPE_V |
| Data padding | Brcb | AIV | PIPE_V |
| Data padding | Duplicate | AIV | PIPE_V |
| Data padding | CreateVecIndex | AIV | PIPE_V |
| Sorting and combination | Sort32 | AIV | PIPE_V |
| Sorting and combination | MrgSort | AIV | PIPE_V |
| Sorting and combination | GetMrgSortResult | AIV | PIPE_S |
| Discrete and aggregate | Gather | AIV | PIPE_V |
| Discrete and aggregate | Gatherb | AIV | PIPE_V |
| Discrete and aggregate | Scatter | AIV | PIPE_V |
| Mask operation | SetMaskCount | AIV | PIPE_S |
| Mask operation | SetMaskNorm | AIV | PIPE_S |
| Mask operation | SetVectorMask | AIV | PIPE_S |
| Mask operation | ResetMask | AIV | PIPE_S |
| Data rearrangement | Interleave | AIV | PIPE_V |
| Data rearrangement | DeInterleave | AIV | PIPE_V |
| data movement | DataCopy(GM->UB) | AIV | PIPE_MTE2 |
| data movement | DataCopy(UB->GM) | AIV | PIPE_MTE3 |
| data movement | DataCopyPad(GM->UB) | AIV | PIPE_MTE2 |
| data movement | DataCopyPad(UB->GM) | AIV | PIPE_MTE3 |
| data movement | DataCopy(UB->UB) | AIV | PIPE_V |
| data movement | Copy | AIV | PIPE_V |

**Table 2**  Cube API pipeline type summary

<a name="table12311219174812"></a>

| API | Execution Core (AIC/AIV) | Pipeline Type |
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
| DataCopyPad(UB->L1) | AIC | <!-- npu="950" id6 -->For Ascend 950PR&950DT products:<br>PIPE_MTE3.<br><!-- end id6 --><!-- npu="A3" id7 -->For Atlas A3 products:<br>PIPE_MTE3, PIPE_MTE2.<br><!-- end id7 --><!-- npu="910b" id8 -->For Atlas A2 products:<br>PIPE_MTE3, PIPE_MTE2.<br><!-- end id8 --> |
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

**Table 3**  Scalar computation instruction pipeline type summary

| API | Pipeline Type |
| --- | --- |
| GetBitCount | PIPE_S |
| CountLeadingZero | PIPE_S |
| GetSFFValue | PIPE_S |
| CountBitsCntSameAsSignBit | PIPE_S |
| Cast (float to half, int32_t) | PIPE_S |
| Cast (float to bfloat16_t) | PIPE_S |
| Cast (bfloat16_t to float) | PIPE_S |
| Nop | PIPE_S |

**Table 4**  Summary of Synchronization Instruction Pipeline Types

<a name="table1054353913238"></a>

| API | Pipeline Type |
| --- | --- |
| SetFlag/WaitFlag | PIPE_S |
| PipeBarrier | PIPE_S |
| DataSyncBarrier | PIPE_S |
| CrossCoreSetFlag | PIPE_S |
| CrossCoreWaitFlag | PIPE_S |
| SyncAll (hardware synchronization API) | PIPE_S |
| SetNextTaskStart | PIPE_S |
| WaitPreTaskEnd | PIPE_S |
| Mutex::Lock | PIPE_S |
| Mutex::UnLock | PIPE_S |

**Table 5**  Summary of Atomic Operation Pipeline Types

<a name="table25871912193814"></a>

| API | Pipeline Type |
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

**Table 6**  Summary of Tool API Pipeline Types

<a name="table1147331317457"></a>

| API | Pipeline Type |
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

**Table 7**  Summary of pipeline types for cache control APIs

| API | Pipeline Type |
| --- | --- |
| ReadGmBypassDCache(ISASI) | PIPE_S |
| WriteGmBypassDCache(ISASI) | PIPE_S |
| DataCachePreload | PIPE_S |
| DataCacheCleanAndInvalid | PIPE_S |
| ICachePreLoad(ISASI) | PIPE_S |
| GetICachePreloadStatus(ISASI) | PIPE_S |
