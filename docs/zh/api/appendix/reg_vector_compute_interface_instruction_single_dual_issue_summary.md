# reg_vector_compute_interface_instruction_single_dual_issue_summary<a name="reg-vector-dual-issue"></a>

本节汇总介绍了reg_vector_compute接口指令单双发的情况，指令单发指1个Cycle能够执行一条指令，指令双发指1个Cycle能够执行两条指令。
<br>**针对寄存器：1个Cycle只能写两个Reg寄存器；针对UB：1个Cycle只能读两个VL（一个VL为256B），或者1个Cycle读一个VL和写一个VL。**

本节内容不包含软仿类接口，仅针对如下型号生效：

Ascend 950PR/Ascend 950DT

**表 1**  接口指令双发汇总<a id="table-dual-issue"></a>

| 接口类型 | 硬件指令名称 | 对应接口名称 |
| --- | --- | --- |
| register_data_types | PSET | CreateMask |
| register_data_types | PLT | UpdateMask |
| reg_data_load | VLDS/VLD | 连续对齐搬入（LoadAlign）：<br>单搬入模式（除了DIST_DS_B8、DIST_DS_B16分布模式） |
| reg_data_load | VLDAS/VLDA | 连续非对齐搬入初始化（LoadUnAlignPre） |
| reg_data_load | VLDUS/VLDU | 连续非对齐搬入（LoadUnAlign） |
| reg_data_load | PLDS/PLD | MaskReg搬入（LoadAlign） |
| reg_data_load | VMOV | Move（reg_data_load） |
| MaskReg_compute | PMOV | Move（MaskReg_compute）|
| MaskReg_compute | PSEL | Select |
| MaskReg_compute | MOVP | MoveMask |
| basic_arithmetic | VABS | Abs |
| basic_arithmetic | VEXP | Exp |
| basic_arithmetic | VSQRT | Sqrt |
| basic_arithmetic | VLN | Ln |
| basic_arithmetic | VLN | Log |
| basic_arithmetic | VNEG | Neg |
| basic_arithmetic | VRELU | Relu |
| basic_arithmetic | VPRELU | Prelu |
| basic_arithmetic | VLRELU | LeakyRelu |
| basic_arithmetic | VADD/VADDC | Add |
| basic_arithmetic | VADDCS | AddC |
| basic_arithmetic | VDIV | Div |
| basic_arithmetic | VMAX | Max |
| basic_arithmetic | VMIN | Min |
| basic_arithmetic | VMUL | Mul |
| basic_arithmetic | VMULL | Mull |
| basic_arithmetic | VSUB/VSUBC | Sub |
| basic_arithmetic | VSUBCS | SubC |
| basic_arithmetic | VADDS | Adds |
| basic_arithmetic | VMAXS | Maxs |
| basic_arithmetic | VMINS | Mins |
| basic_arithmetic | VMULS | Muls |
| logical_compute | VNOT/PNOT | Not |
| logical_compute | VAND/PAND | And |
| logical_compute | VOR/POR | Or |
| logical_compute | VXOR/PXOR | Xor |
| logical_compute | VSHL | ShiftLeft |
| logical_compute | VSHR | ShiftRight |
| logical_compute | VSHLS | ShiftLefts |
| logical_compute | VSHRS | ShiftRights |
| composite_compute | VABSDIF | AbsSub |
| composite_compute | VAXPY | Axpy |
| composite_compute | VEXPDIF | ExpSub |
| composite_compute | VMADD | MulDstAdd |
| composite_compute | VMULA | MulAddDst |
| composite_compute | VMULSCVT | MulsCast |
| compare_and_select | VCMP | Compare |
| compare_and_select | VCMPS | Compares |
| compare_and_select | VSEL/PSEL | Select |
| type_conversion | VCVTFI/VCVTFF/VCVTFF2/VCVTIF/VCVTII | Cast |
| type_conversion | VTRC | Truncate |
| data_padding | VBR | Duplicate |
| index_operations | VCI | Arange |

**表 2**  接口指令单发汇总<a id="table-single-issue"></a>

| 接口类型 | 硬件指令名称 | 对应接口名称 |
| --- | --- | --- |
| reg_data_load | VSLDB | 非连续对齐搬入（LoadAlign） |
| reg_data_load | VLDS/VLD | 连续对齐搬入（LoadAlign）：<br>双搬入模式和单搬入模式的DIST_DS_B8、DIST_DS_B16分布模式 |
| reg_data_load | MOVVP | MaskGenWithRegTensor |
| reg_data_load | VGATHER2 | Gather |
| reg_data_load | VGATHERB | GatherB |
| reg_data_store | VSSTB | 非连续对齐搬出（StoreAlign）|
| reg_data_store | VSTS/VST | 连续对齐搬出（StoreAlign） |
| reg_data_store | VSTUS/VSTU/VSTUR | 连续非对齐搬出（StoreUnAlign） |
| reg_data_store | VSTAS/VSTA/VSTAR | 连续非对齐搬出后处理（StoreUnAlignPost） |
| reg_data_store | PSTS/PST | MaskReg搬出（StoreAlign） |
| reg_data_store | PSTU | MaskReg非对齐搬出（StoreUnAlign） |
| reg_data_store | VSCATTER | Scatter |
| MaskReg_compute | PINTLV | Interleave |
| MaskReg_compute | PDINTLV | DeInterleave |
| MaskReg_compute | PPACK | Pack |
| MaskReg_compute | PUNPACK | UnPack |
| reduction_compute | VCADD | Reduce（ReduceType::SUM） |
| reduction_compute | VCMAX | Reduce（ReduceType::MAX） |
| reduction_compute | VCMIN | Reduce（ReduceType::MIN） |
| reduction_compute | VCGADD | ReduceDataBlock（ReduceType::SUM） |
| reduction_compute | VCCGMAX | ReduceDataBlock（ReduceType::MAX） |
| reduction_compute | VCGMIN | ReduceDataBlock（ReduceType::MIN） |
| reduction_compute | VCPADD | PairReduceElem |
| compare_and_select | VSQZ | Squeeze |
| scatter_operations | VSELR | Gather |
| data_reorder | VINTLV | Interleave |
| data_reorder | VDINTLV | DeInterleave |
| data_compression | VUSQZ | Unsqueeze |
| data_compression | VPACK | Pack |
| data_compression | VUNPACK | UnPack |
| histogram_compute | DHISTv2 | Histograms |