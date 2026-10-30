# Summary of Dual-Issue and Single-Issue Reg Vector Computation Interface Instructions<a name="reg-vector-dual-issue"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-29T10:54:19.990Z pushedAt=2026-10-29T11:15:47.689Z nl=2 -->

This section summarizes the dual-issue and single-issue situations of Reg vector computation interface instructions. Single-issue means that one instruction can be executed in one cycle, and dual-issue means that two instructions can be executed in one cycle.
<br>**For registers: only two Reg registers can be written in one cycle. For Unified Buffer (UB): only two VLs can be read in one cycle (one VL is 256B), or one VL can be read and one VL can be written in one cycle.**

This section does not include software simulation interfaces and applies only to the following models:

Ascend 950PR & 950DT products

**Table 1**  Summary of Dual-Issue API Instructions<a id="table-dual-issue"></a>

| API | Hardware Instruction Name | Corresponding API |
| --- | --- | --- |
| register data type | PSET | CreateMask |
| register data type | PLT | UpdateMask |
| Reg data load-in | VLDS/VLD | Continuous alignment load-in (LoadAlign):<br>single load-in mode (except DIST_DS_B8 and DIST_DS_B16 distribution modes) |
| Reg data load-in | VLDAS/VLDA | Continuous non-alignment load-in initialization (LoadUnAlignPre) |
| Reg data load-in | VLDUS/VLDU | Continuous non-alignment load-in (LoadUnAlign) |
| Reg data load-in | PLDS/PLD | MaskReg load-in (LoadAlign) |
| Reg data load-in | VMOV | Move (Reg data load-in) |
| MaskReg computation | PMOV | Move (MaskReg computation)|
| MaskReg computation | PSEL | Select |
| MaskReg computation | MOVP | MoveMask |
| Basic arithmetic | VABS | Abs |
| Basic arithmetic | VEXP | Exp |
| Basic arithmetic | VSQRT | Sqrt |
| Basic arithmetic | VLN | Ln |
| Basic arithmetic | VLN | Log |
| Basic arithmetic | VNEG | Neg |
| Basic arithmetic | VRELU | Relu |
| Basic arithmetic | VPRELU | Prelu |
| Basic arithmetic | VLRELU | LeakyRelu |
| Basic arithmetic | VADD/VADDC | Add |
| Basic arithmetic | VADDCS | AddC |
| Basic arithmetic | VDIV | Div |
| Basic arithmetic | VMAX | Max |
| Basic arithmetic | VMIN | Min |
| Basic arithmetic | VMUL | Mul |
| Basic arithmetic | VMULL | Mull |
| Basic arithmetic | VSUB/VSUBC | Sub |
| Basic arithmetic | VSUBCS | SubC |
| Basic arithmetic | VADDS | Adds |
| Basic arithmetic | VMAXS | Maxs |
| Basic arithmetic | VMINS | Mins |
| Basic arithmetic | VMULS | Muls |
| Logical computation | VNOT/PNOT | Not |
| Logical computation | VAND/PAND | And |
| Logical computation | VOR/POR | Or |
| Logical computation | VXOR/PXOR | Xor |
| Logical computation | VSHL | ShiftLeft |
| Logical computation | VSHR | ShiftRight |
| Logical computation | VSHLS | ShiftLefts |
| Logical computation | VSHRS | ShiftRights |
| Compound computation | VABSDIF | AbsSub |
| Compound computation | VAXPY | Axpy |
| Compound computation | VEXPDIF | ExpSub |
| Compound computation | VMADD | MulDstAdd |
| Compound computation | VMULA | MulAddDst |
| Compound computation | VMULSCVT | MulsCast |
| Comparison and selection | VCMP | Compare |
| Comparison and selection | VCMPS | Compares |
| Comparison and selection | VSEL/PSEL | Select |
| Type conversion | VCVTFI/VCVTFF/VCVTFF2/VCVTIF/VCVTII | Cast |
| Type conversion | VTRC | Truncate |
| Data padding | VBR | Duplicate |
| index operation | VCI | Arange |

**Table 2**  Summary of Single-Issue Interface Instructions<a id="table-single-issue"></a>

| API | Hardware Instruction Name | Corresponding API |
| --- | --- | --- |
| Reg data load-in | VSLDB | Non-continuous alignment load-in (LoadAlign) |
| Reg data load-in | VLDS/VLD | Continuous alignment load-in (LoadAlign):<br>Dual load-in mode and the DIST_DS_B8 and DIST_DS_B16 distribution modes of the single load-in mode |
| Reg data load-in | MOVVP | MaskGenWithRegTensor |
| Reg data load-in | VGATHER2 | Gather |
| Reg data load-in | VGATHERB | GatherB |
| Reg data load-out | VSSTB | Non-continuous alignment load-out (StoreAlign)|
| Reg data load-out | VSTS/VST | Continuous alignment load-out (StoreAlign) |
| Reg data load-out | VSTUS/VSTU/VSTUR | Continuous non-alignment load-out (StoreUnAlign) |
| Reg data load-out | VSTAS/VSTA/VSTAR | Continuous non-alignment load-out post-processing (StoreUnAlignPost) |
| Reg data load-out | PSTS/PST | MaskReg load-out (StoreAlign) |
| Reg data load-out | PSTU | MaskReg non-alignment load-out (StoreUnAlign) |
| Reg data load-out | VSCATTER | Scatter |
| MaskReg computation | PINTLV | Interleave |
| MaskReg computation | PDINTLV | DeInterleave |
| MaskReg computation | PPACK | Pack |
| MaskReg computation | PUNPACK | UnPack |
| Reduction computation | VCADD | Reduce (ReduceType::SUM) |
| Reduction computation | VCMAX | Reduce (ReduceType::MAX) |
| Reduction computation | VCMIN | Reduce (ReduceType::MIN) |
| Reduction computation | VCGADD | ReduceDataBlock (ReduceType::SUM) |
| Reduction computation | VCCGMAX | ReduceDataBlock (ReduceType::MAX) |
| Reduction computation | VCGMIN | ReduceDataBlock (ReduceType::MIN) |
| Reduction computation | VCPADD | PairReduceElem |
| Comparison and selection | VSQZ | Squeeze |
| discrete operation | VSELR | Gather |
| Data rearrangement | VINTLV | Interleave |
| Data rearrangement | VDINTLV | DeInterleave |
| data compression | VUSQZ | Unsqueeze |
| data compression | VPACK | Pack |
| data compression | VUNPACK | UnPack |
| histogram computation | DHISTv2 | Histograms |
