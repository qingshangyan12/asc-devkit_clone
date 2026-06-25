# Reg矢量计算接口指令单双发汇总<a name="reg-vector-dual-issue"></a>

<!-- npu="950" id1 -->
本节汇总介绍了Reg矢量计算接口指令单双发的情况，以下内容仅针对如下型号生效：

Ascend 950PR/Ascend 950DT

**表 1**  接口指令双发汇总<a id="table-dual-issue"></a>

| 硬件指令名称 | 对应接口名称 |
| --- | --- |
| VADD/VADDC | Add |
| VSUB/VSUBC | Sub |
| VMAX | Max |
| VMIN | Min |
| VABSDIF | AbsSub |
| VCMP | Compare |
| VADDS | Adds |
| VADDCS | AddC |
| VSUBCS | SubC |
| VMAXS | Maxs |
| VMINS | Mins |
| VCMPS | Compares |
| VABS | Abs |
| VNEG | Neg |
| VCVTFI/VCVTFF/VCVTFF2/VCVTIF/VCVTII | Cast |
| VTRC | Truncate |
| VCI | Arange |
| VDIV | Div |
| VSQRT | Sqrt |
| VLN | Ln |
| VEXP | Exp |
| VEXPDIF | ExpSub |
| VRELU | Relu |
| VAND/PAND | And |
| VOR/POR | Or |
| VXOR/PXOR | Xor |
| VNOT/PNOT | Not |
| VMOV | Move（Reg数据搬入） |
| VSEL/PSEL | Select |
| VBR | Duplicate |
| PSET | CreateMask |
| PLT | UpdateMask |
| PMOV | Move（MaskReg计算）|
| VMUL | Mul |
| VMULL | Mull |
| VMULA | MulAddDst |
| VMADD | MulDstAdd |
| VAXPY | Axpy |
| VSHL | Shiftleft |
| VSHR | ShiftRight |
| VLRELU | LeakyRelu |
| VPRELU | Prelu |
| VMULS | Muls |
| VMULSCVT | MulsCast |
| VSHLS | ShiftLefts |
| VSHRS | ShiftRights |
| VLDS/VLD | 连续对齐搬入（LoadAlign），仅单搬入（除了DIST_DS_B8、DIST_DS_B16分布模式）。 |
| VLDAS/VLDA | 连续非对齐搬入初始化（LoadUnAlignPre） |
| VLDUS/VLDU | 连续非对齐搬入（LoadUnAlign） |
| PLDS/PLD | MaskReg搬入（LoadAlign） |

**表 2**  接口指令单发汇总<a id="table-single-issue"></a>

| 硬件指令名称 | 对应接口名称 |
| --- | --- |
| VCADD | Reduce（ReduceType::SUM） |
| VCMAX | Reduce（ReduceType::MAX） |
| VCMIN | Reduce（ReduceType::MIN） |
| VCGADD | ReduceDataBlock（ReduceType::SUM） |
| VCCGMAX | ReduceDataBlock（ReduceType::MAX） |
| VCGMIN | ReduceDataBlock（ReduceType::MIN） |
| VCPADD | PairReduceElem |
| VSQZ | Squeeze |
| VPACK | Pack |
| VUNPACK | UnPack |
| VUSQZ | Unsqueeze |
| VINTLV | Interleave |
| VDINTLV | DeInterleave |
| VGATHER2 | Gather |
| VGATHERB | GatherB |
| VSCATTER | Scatter |
| VSELR | Gather |
| VSLDB | 非连续对齐搬入（LoadAlign） |
| VSSTB | 非连续对齐搬出（StoreAlign）|
| VLDS/VLD | 连续对齐搬入（LoadAlign）的双搬入和单搬入的DIST_DS_B8、DIST_DS_B16分布模式 |
| VSTS/VST | 连续对齐搬出（StoreAlign） |
| VSTUS/VSTU/VSTUR | 连续非对齐搬出（StoreUnAlign） |
| VSTAS/VSTA/VSTAR | 连续非对齐搬出后处理（StoreUnAlignPost） |
| PSTS/PST | MaskReg搬出（StoreAlign） |
| PSTU | MaskReg非对齐搬出（StoreUnAlign） |

针对寄存器：1个Cycle只能写两个Reg。

针对UB：1个Cycle只能读两个VL，或者1个Cycle读一个VL和写一个VL。
<!-- end id1 -->