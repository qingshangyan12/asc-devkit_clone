# Reg矢量计算接口理论性能汇总

本节汇总介绍了Reg矢量计算接口的理论性能，本节内容仅针对如下型号生效：

<!-- npu="960" id1 -->
Ascend 960PR/Ascend 960DT
<!-- end id1 -->
<!-- npu="950" id2 -->
Ascend 950PR/Ascend 950DT
<!-- end id2 -->

本节汇总的理论性能均为单指令理论性能，实际性能还需考虑指令单双发情况，请参考[Reg矢量计算接口指令单双发汇总](../appendix/reg_vector_compute_interface_instruction_single_dual_issue_summary.md)。软仿实现的接口和数据类型不提供理论性能。

**表1**  RegTensor数据搬入

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| 连续对齐搬入（LoadAlign） | b8 | b8 | 256 |
| 连续对齐搬入（LoadAlign） | b16 | b16 | 128 |
| 连续对齐搬入（LoadAlign） | b32 | b32 | 64 |
| 非连续对齐搬入（LoadAlign） | b8 | b8 | 128 |
| 非连续对齐搬入（LoadAlign） | b16 | b16 | 64 |
| 非连续对齐搬入（LoadAlign） | b32 | b32 | 32 |
| 连续非对齐搬入（LoadUnAlign） | b8 | b8 | 256（LoadUnAlign）/32（LoadUnAlignPre） |
| 连续非对齐搬入（LoadUnAlign） | b16 | b16 | 128（LoadUnAlign）/16（LoadUnAlignPre） |
| 连续非对齐搬入（LoadUnAlign） | b32 | b32 | 64（LoadUnAlign）/8（LoadUnAlignPre） |
| 离散搬入（Gather） | b8 | b16 | 64 |
| 离散搬入（Gather） | b16 | b16 | 64 |
| 离散搬入（Gather） | b32 | b32 | 32 |
| 离散搬入（GatherB） | b8 | b8 | 128 |
| 离散搬入（GatherB） | b16 | b16 | 64 |
| 离散搬入（GatherB） | b32 | b32 | 32 |
| 离散搬入（GatherB） | b64 | b64 | 16 |
| Move | b8 | b8 | 256 |
| Move | b16 | b16 | 128 |
| Move | b32 | b32 | 64 |
| Move | b64 | b64 | 32 |

**表2**  RegTensor数据搬出

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| 连续对齐搬出（StoreAlign） | b8 | b8 | 256 |
| 连续对齐搬出（StoreAlign） | b16 | b16 | 128 |
| 连续对齐搬出（StoreAlign） | b32 | b32 | 64 |
| 非连续对齐搬出（StoreAlign） | b8 | b8 | 128 |
| 非连续对齐搬出（StoreAlign） | b16 | b16 | 64 |
| 非连续对齐搬出（StoreAlign） | b32 | b32 | 32 |
| 连续非对齐搬出（StoreUnAlign） | b8 | b8 | 256（StoreUnAlign）/32（StoreUnAlignPost） |
| 连续非对齐搬出（StoreUnAlign） | b16 | b16 | 128（StoreUnAlign）/16（StoreUnAlignPost） |
| 连续非对齐搬出（StoreUnAlign） | b32 | b32 | 64（StoreUnAlign）/8（StoreUnAlignPost） |
| 离散搬出（Scatter） | b8 | b8 | 128 |
| 离散搬出（Scatter） | b16 | b16 | 64 |
| 离散搬出（Scatter） | b32 | b32 | 32 |

**表3**  MaskReg数据搬入

| 接口 | 理论耗时（cycle） |
| -------- | ---------------- |
| MaskReg搬入（LoadAlign） | 1 |
| MaskReg搬入（MaskGenWithRegTensor） | 4 |

**表4**  MaskReg数据搬出

| 接口 | 理论耗时（cycle） |
| -------- | ---------------- |
| MaskReg搬出（StoreAlign） | 1 |
| MaskReg非对齐搬出（StoreUnAlign） | 1 |

**表5**  MaskReg计算

| 接口 | 理论耗时（cycle） |
| -------- | ---------------- |
| Move | 1 |
| Interleave | 1 |
| DeInterleave | 1 |
| Select | 1 |
| Pack | 1 |
| UnPack | 1 |
| MoveMask | 1 |

**表6**  基础算术

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Abs | int8_t | int8_t | <!-- npu="950" id3 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id3 --><!-- npu="960" id4 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id4 --> |
| Abs | int16_t | int16_t | <!-- npu="950" id5 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id5 --><!-- npu="960" id6 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id6 --> |
| Abs | half | half | 128 |
| Abs | int32_t | int32_t | <!-- npu="950" id7 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id7 --><!-- npu="960" id8 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id8 --> |
| Abs | float | float | 64 |
| Exp | half | half | <!-- npu="950" id9 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id9 --><!-- npu="960" id10 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id10 --> |
| Exp | float | float | <!-- npu="950" id11 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id11 --><!-- npu="960" id12 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id12 --> |
| Sqrt | half | half | <!-- npu="950" id13 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id13 --><!-- npu="960" id14 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id14 --> |
| Sqrt | float | float | <!-- npu="950" id15 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id15 --><!-- npu="960" id16 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id16 --> |
| Ln | half | half | <!-- npu="950" id17 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id17 --><!-- npu="960" id18 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id18 --> |
| Ln | float | float | <!-- npu="950" id19 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id19 --><!-- npu="960" id20 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id20 --> |
| Log | half | half | 16 |
| Log | float | float | 16 |
| Neg | int8_t | int8_t | <!-- npu="950" id21 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id21 --><!-- npu="960" id22 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id22 --> |
| Neg | int16_t | int16_t | <!-- npu="950" id23 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id23 --><!-- npu="960" id24 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id24 --> |
| Neg | half | half | 128 |
| Neg | int32_t | int32_t | <!-- npu="950" id25 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id25 --><!-- npu="960" id26 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id26 --> |
| Neg | float | float | 64 |
| Relu | half | half | 128 |
| Relu | int32_t | int32_t | 64 |
| Relu | float | float | 64 |
| Prelu | half | half | 128 |
| Prelu | float | float | 64 |
| LeakyRelu | half | half | 128 |
| LeakyRelu | float | float | 64 |
| Add | int8_t | int8_t | <!-- npu="950" id27 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id27 --><!-- npu="960" id28 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id28 --> |
| Add | uint8_t | uint8_t | <!-- npu="950" id29 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id29 --><!-- npu="960" id30 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id30 --> |
| Add | int16_t | int16_t | <!-- npu="950" id31 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id31 --><!-- npu="960" id32 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id32 --> |
| Add | uint16_t | uint16_t | <!-- npu="950" id33 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id33 --><!-- npu="960" id34 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id34 --> |
| Add | half | half | 128 |
| Add | bfloat16_t | bfloat16_t | 128 |
| Add | int32_t | int32_t | <!-- npu="950" id35 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id35 --><!-- npu="960" id36 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id36 --> |
| Add | uint32_t | uint32_t | <!-- npu="950" id37 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id37 --><!-- npu="960" id38 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id38 --> |
| Add | float | float | 64 |
| AddC | int32_t | int32_t | 32 |
| AddC | uint32_t | uint32_t | <!-- npu="950" id39 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id39 --><!-- npu="960" id40 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id40 --> |
| Div | half | half | <!-- npu="950" id41 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id41 --><!-- npu="960" id42 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id42 --> |
| Div | float | float | <!-- npu="950" id43 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id43 --><!-- npu="960" id44 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id44 --> |
| Max | int8_t | int8_t | 256 |
| Max | uint8_t | uint8_t | 256 |
| Max | int16_t | int16_t | 128 |
| Max | uint16_t | uint16_t | 128 |
| Max | half | half | 128 |
| Max | bfloat16_t | bfloat16_t | 128 |
| Max | int32_t | int32_t | 64 |
| Max | uint32_t | uint32_t | 64 |
| Max | float | float | 64 |
| Min | int8_t | int8_t | 256 |
| Min | uint8_t | uint8_t | 256 |
| Min | int16_t | int16_t | 128 |
| Min | uint16_t | uint16_t | 128 |
| Min | half | half | 128 |
| Min | bfloat16_t | bfloat16_t | 128 |
| Min | int32_t | int32_t | 64 |
| Min | uint32_t | uint32_t | 64 |
| Min | float | float | 64 |
| Mul | int16_t | int16_t | 64 |
| Mul | uint16_t | uint16_t | 64 |
| Mul | half | half | 128 |
| Mul | int32_t | int32_t | 32 |
| Mul | uint32_t | uint32_t | 32 |
| Mul | float | float | 64 |
| Mull | int32_t | int32_t | 32 |
| Mull | uint32_t | uint32_t | 32 |
| Sub | int8_t | int8_t | <!-- npu="950" id45 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id45 --><!-- npu="960" id46 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id46 --> |
| Sub | uint8_t | uint8_t | <!-- npu="950" id47 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id47 --><!-- npu="960" id48 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id48 --> |
| Sub | int16_t | int16_t | <!-- npu="950" id49 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id49 --><!-- npu="960" id50 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id50 --> |
| Sub | uint16_t | uint16_t | <!-- npu="950" id51 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id51 --><!-- npu="960" id52 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id52 --> |
| Sub | half | half | 128 |
| Sub | bfloat16_t | bfloat16_t | 128 |
| Sub | int32_t | int32_t | <!-- npu="950" id53 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id53 --><!-- npu="960" id54 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id54 --> |
| Sub | uint32_t | uint32_t | <!-- npu="950" id55 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id55 --><!-- npu="960" id56 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id56 --> |
| Sub | float | float | 64 |
| SubC | int32_t | int32_t | 32 |
| SubC | uint32_t | uint32_t | <!-- npu="950" id57 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id57 --><!-- npu="960" id58 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id58 --> |
| Adds | int8_t | int8_t | <!-- npu="950" id59 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id59 --><!-- npu="960" id60 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id60 --> |
| Adds | uint8_t | uint8_t | <!-- npu="950" id61 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id61 --><!-- npu="960" id62 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id62 --> |
| Adds | int16_t | int16_t | <!-- npu="950" id63 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id63 --><!-- npu="960" id64 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id64 --> |
| Adds | uint16_t | uint16_t | <!-- npu="950" id65 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id65 --><!-- npu="960" id66 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id66 --> |
| Adds | half | half | 128 |
| Adds | bfloat16_t | bfloat16_t | 128 |
| Adds | int32_t | int32_t | <!-- npu="950" id67 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id67 --><!-- npu="960" id68 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id68 --> |
| Adds | uint32_t | uint32_t | <!-- npu="950" id69 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id69 --><!-- npu="960" id70 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id70 --> |
| Adds | float | float | 64 |
| Maxs | int8_t | int8_t | 256 |
| Maxs | uint8_t | uint8_t | 256 |
| Maxs | int16_t | int16_t | 128 |
| Maxs | uint16_t | uint16_t | 128 |
| Maxs | half | half | 128 |
| Maxs | bfloat16_t | bfloat16_t | 128 |
| Maxs | int32_t | int32_t | 64 |
| Maxs | uint32_t | uint32_t | 64 |
| Maxs | float | float | 64 |
| Mins | int8_t | int8_t | 256 |
| Mins | uint8_t | uint8_t | 256 |
| Mins | int16_t | int16_t | 128 |
| Mins | uint16_t | uint16_t | 128 |
| Mins | half | half | 128 |
| Mins | bfloat16_t | bfloat16_t | 128 |
| Mins | int32_t | int32_t | 64 |
| Mins | uint32_t | uint32_t | 64 |
| Mins | float | float | 64 |
| Muls | int16_t | int16_t | 64 |
| Muls | uint16_t | uint16_t | 64 |
| Muls | half | half | 128 |
| Muls | int32_t | int32_t | 32 |
| Muls | uint32_t | uint32_t | 32 |
| Muls | float | float | 64 |

**表7**  逻辑计算

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Not | int8_t | int8_t | 256 |
| Not | uint8_t | uint8_t | 256 |
| Not | int16_t | int16_t | 128 |
| Not | uint16_t | uint16_t | 128 |
| Not | half | half | 128 |
| Not | int32_t | int32_t | 64 |
| Not | uint32_t | uint32_t | 64 |
| Not | float | float | 64 |
| And | int8_t | int8_t | 256 |
| And | uint8_t | uint8_t | 256 |
| And | int16_t | int16_t | 128 |
| And | uint16_t | uint16_t | 128 |
| And | int32_t | int32_t | 64 |
| And | uint32_t | uint32_t | 64 |
| Or | int8_t | int8_t | 256 |
| Or | uint8_t | uint8_t | 256 |
| Or | int16_t | int16_t | 128 |
| Or | uint16_t | uint16_t | 128 |
| Or | int32_t | int32_t | 64 |
| Or | uint32_t | uint32_t | 64 |
| Xor | int8_t | int8_t | 256 |
| Xor | uint8_t | uint8_t | 256 |
| Xor | int16_t | int16_t | 128 |
| Xor | uint16_t | uint16_t | 128 |
| Xor | int32_t | int32_t | 64 |
| Xor | uint32_t | uint32_t | 64 |
| ShiftLeft | int8_t | int8_t | 256 |
| ShiftLeft | uint8_t | uint8_t | 256 |
| ShiftLeft | int16_t | int16_t | 128 |
| ShiftLeft | uint16_t | uint16_t | 128 |
| ShiftLeft | int32_t | int32_t | 64 |
| ShiftLeft | uint32_t | uint32_t | 64 |
| ShiftRight | int8_t | int8_t | 256 |
| ShiftRight | uint8_t | uint8_t | 256 |
| ShiftRight | int16_t | int16_t | 128 |
| ShiftRight | uint16_t | uint16_t | 128 |
| ShiftRight | int32_t | int32_t | 64 |
| ShiftRight | uint32_t | uint32_t | 64 |
| ShiftLefts | int8_t | int8_t | 256 |
| ShiftLefts | uint8_t | uint8_t | 256 |
| ShiftLefts | int16_t | int16_t | 128 |
| ShiftLefts | uint16_t | uint16_t | 128 |
| ShiftLefts | int32_t | int32_t | 64 |
| ShiftLefts | uint32_t | uint32_t | 64 |
| ShiftRights | int8_t | int8_t | 256 |
| ShiftRights | uint8_t | uint8_t | 256 |
| ShiftRights | int16_t | int16_t | 128 |
| ShiftRights | uint16_t | uint16_t | 128 |
| ShiftRights | int32_t | int32_t | 64 |
| ShiftRights | uint32_t | uint32_t | 64 |

**表8**  复合计算

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| AbsSub | half | half | 128 |
| AbsSub | float | float | 64 |
| Axpy | half | half | 128 |
| Axpy | float | float | 64 |
| ExpSub | half | float | <!-- npu="950" id71 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id71 --><!-- npu="960" id72 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id72 --> |
| ExpSub | float | float | <!-- npu="950" id73 -->针对Ascend 950PR/Ascend 950DT：<br>16<br><!-- end id73 --><!-- npu="960" id74 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id74 --> |
| MulDstAdd | half | half | 128 |
| MulDstAdd | bfloat16_t | bfloat16_t | 128 |
| MulDstAdd | float | float | 64 |
| MulAddDst | int16_t | int16_t | 64 |
| MulAddDst | uint16_t | uint16_t | 64 |
| MulAddDst | half | half | 128 |
| MulAddDst | bfloat16_t | bfloat16_t | 128 |
| MulAddDst | int32_t | int32_t | 32 |
| MulAddDst | uint32_t | uint32_t | 32 |
| MulAddDst | float | float | 64 |
| MulsCast | float | half | 64 |

**表9**  比较与选择

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Compare | int8_t | MaskReg | 256 |
| Compare | uint8_t | MaskReg | 256 |
| Compare | int16_t | MaskReg | 128 |
| Compare | uint16_t | MaskReg | 128 |
| Compare | half | MaskReg | 128 |
| Compare | bfloat16_t | MaskReg | 128 |
| Compare | int32_t | MaskReg | 64 |
| Compare | uint32_t | MaskReg | 64 |
| Compare | float | MaskReg | 64 |
| Compares | int8_t | MaskReg | 256 |
| Compares | uint8_t | MaskReg | 256 |
| Compares | int16_t | MaskReg | 128 |
| Compares | uint16_t | MaskReg | 128 |
| Compares | half | MaskReg | 128 |
| Compares | bfloat16_t | MaskReg | 128 |
| Compares | int32_t | MaskReg | 64 |
| Compares | uint32_t | MaskReg | 64 |
| Compares | float | MaskReg | 64 |
| Select | int8_t | MaskReg | 256 |
| Select | uint8_t | MaskReg | 256 |
| Select | int16_t | MaskReg | 128 |
| Select | uint16_t | MaskReg | 128 |
| Select | half | MaskReg | 128 |
| Select | bfloat16_t | MaskReg | 128 |
| Select | int32_t | MaskReg | 64 |
| Select | uint32_t | MaskReg | 64 |
| Select | float | MaskReg | 64 |
| Squeeze | int8_t | int8_t | <!-- npu="950" id75 -->针对Ascend 950PR/Ascend 950DT：<br>51.2<br><!-- end id75 --><!-- npu="960" id76 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id76 --> |
| Squeeze | uint8_t | uint8_t | <!-- npu="950" id77 -->针对Ascend 950PR/Ascend 950DT：<br>51.2<br><!-- end id77 --><!-- npu="960" id78 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id78 --> |
| Squeeze | int16_t | int16_t | <!-- npu="950" id79 -->针对Ascend 950PR/Ascend 950DT：<br>25.6<br><!-- end id79 --><!-- npu="960" id80 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id80 --> |
| Squeeze | uint16_t | uint16_t | <!-- npu="950" id81 -->针对Ascend 950PR/Ascend 950DT：<br>25.6<br><!-- end id81 --><!-- npu="960" id82 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id82 --> |
| Squeeze | half | half | <!-- npu="950" id83 -->针对Ascend 950PR/Ascend 950DT：<br>25.6<br><!-- end id83 --><!-- npu="960" id84 -->针对Ascend 960PR/Ascend 960DT：<br>32<br><!-- end id84 --> |
| Squeeze | int32_t | int32_t | <!-- npu="950" id85 -->针对Ascend 950PR/Ascend 950DT：<br>12.8<br><!-- end id85 --><!-- npu="960" id86 -->针对Ascend 960PR/Ascend 960DT：<br>16<br><!-- end id86 --> |
| Squeeze | uint32_t | uint32_t | <!-- npu="950" id87 -->针对Ascend 950PR/Ascend 950DT：<br>12.8<br><!-- end id87 --><!-- npu="960" id88 -->针对Ascend 960PR/Ascend 960DT：<br>16<br><!-- end id88 --> |
| Squeeze | float | float | <!-- npu="950" id89 -->针对Ascend 950PR/Ascend 950DT：<br>12.8<br><!-- end id89 --><!-- npu="960" id90 -->针对Ascend 960PR/Ascend 960DT：<br>16<br><!-- end id90 --> |

**表10**  类型转换

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Cast | float | int64_t | 16 |
| Cast | float | int32_t | 32 |
| Cast | float | int16_t | 32 |
| Cast | half | int32_t | 32 |
| Cast | half | int16_t | 64 |
| Cast | half | int8_t | 64 |
| Cast | float | half | 64 |
| Cast | float | bfloat16_t | 64 |
| Cast | half | float | 64 |
| Cast | bfloat16_t | float | 64 |
| Cast | float | hifloat8_t | 32 |
| Cast | hifloat8_t | float | 32 |
| Cast | half | hifloat8_t | 64 |
| Cast | hifloat8_t | half | 64 |
| Cast | float | fp8_e4m3fn_t | 32 |
| Cast | float | fp8_e5m2_t | 32 |
| Cast | fp4×2_e1m2_t | bfloat16_t | 64 |
| Cast | fp4×2_e2m1_t | bfloat16_t | 64 |
| Cast | bfloat16_t | fp4×2_e1m2_t | 64 |
| Cast | bfloat16_t | fp4×2_e2m1_t | 64 |
| Cast | uint8_t | half | 64 |
| Cast | int8_t | half | 64 |
| Cast | int16_t | half | 64 |
| Cast | int16_t | float | 32 |
| Cast | int32_t | float | 32 |
| Cast | int4×2_t | half | 64 |
| Cast | int4×2_t | int16_t | 64 |
| Cast | uint8_t | uint16_t | 64 |
| Cast | uint8_t | uint32_t | 32 |
| Cast | int8_t | int16_t | 64 |
| Cast | int8_t | int32_t | 32 |
| Cast | uint16_t | uint8_t | 64 |
| Truncate | half | half | 64 |
| Truncate | bfloat16_t | bfloat16_t | 64 |
| Truncate | float | float | 32 |

**表11**  归约计算

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Reduce（ReduceType::SUM） | half | half | 128 |
| Reduce（ReduceType::SUM） | int16_t | int32_t | 64 |
| Reduce（ReduceType::SUM） | uint16_t | uint32_t | 64 |
| Reduce（ReduceType::SUM） | int32_t | int32_t | 64 |
| Reduce（ReduceType::SUM） | uint32_t | uint32_t | 64 |
| Reduce（ReduceType::SUM） | float | float | 64 |
| Reduce（ReduceType::MAX） | int16_t | int16_t | 128 |
| Reduce（ReduceType::MAX） | uint16_t | uint16_t | 128 |
| Reduce（ReduceType::MAX） | half | half | 128 |
| Reduce（ReduceType::MAX） | int32_t | int32_t | 64 |
| Reduce（ReduceType::MAX） | uint32_t | uint32_t | 64 |
| Reduce（ReduceType::MAX） | float | float | 64 |
| Reduce（ReduceType::MIN） | int16_t | int16_t | 128 |
| Reduce（ReduceType::MIN） | uint16_t | uint16_t | 128 |
| Reduce（ReduceType::MIN） | half | half | 128 |
| Reduce（ReduceType::MIN） | int32_t | int32_t | 64 |
| Reduce（ReduceType::MIN） | uint32_t | uint32_t | 64 |
| Reduce（ReduceType::MIN） | float | float | 64 |
| ReduceDataBlock（ReduceType::SUM） | half | half | 128 |
| ReduceDataBlock（ReduceType::SUM） | int32_t | int32_t | 64 |
| ReduceDataBlock（ReduceType::SUM） | uint32_t | uint32_t | 64 |
| ReduceDataBlock（ReduceType::SUM） | float | float | 64 |
| ReduceDataBlock（ReduceType::MAX） | int16_t | int16_t | 128 |
| ReduceDataBlock（ReduceType::MAX） | uint16_t | uint16_t | 128 |
| ReduceDataBlock（ReduceType::MAX） | half | half | 128 |
| ReduceDataBlock（ReduceType::MAX） | int32_t | int32_t | 64 |
| ReduceDataBlock（ReduceType::MAX） | uint32_t | uint32_t | 64 |
| ReduceDataBlock（ReduceType::MAX） | float | float | 64 |
| ReduceDataBlock（ReduceType::MIN） | int16_t | int16_t | 128 |
| ReduceDataBlock（ReduceType::MIN） | uint16_t | uint16_t | 128 |
| ReduceDataBlock（ReduceType::MIN） | half | half | 128 |
| ReduceDataBlock（ReduceType::MIN） | int32_t | int32_t | 64 |
| ReduceDataBlock（ReduceType::MIN） | uint32_t | uint32_t | 64 |
| ReduceDataBlock（ReduceType::MIN） | float | float | 64 |
| PairReduceElem | half | half | 128 |
| PairReduceElem | float | float | 64 |

**表12**  数据填充

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Duplicate | int8_t | int8_t | 256 |
| Duplicate | uint8_t | uint8_t | 256 |
| Duplicate | int16_t | int16_t | 128 |
| Duplicate | uint16_t | uint16_t | 128 |
| Duplicate | half | half | 128 |
| Duplicate | bfloat16_t | bfloat16_t | 128 |
| Duplicate | int32_t | int32_t | 64 |
| Duplicate | uint32_t | uint32_t | 64 |
| Duplicate | float | float | 64 |

**表13**  离散操作

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Gather | b8 | b8 | 64 |
| Gather | b16 | b16 | 32 |
| Gather | b32 | b32 | 16 |

**表14**  数据重排

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Interleave | int8_t | int8_t | 256 |
| Interleave | uint8_t | uint8_t | 256 |
| Interleave | int16_t | int16_t | 128 |
| Interleave | uint16_t | uint16_t | 128 |
| Interleave | half | half | 128 |
| Interleave | bfloat16_t | bfloat16_t | 128 |
| Interleave | int32_t | int32_t | 64 |
| Interleave | uint32_t | uint32_t | 64 |
| Interleave | float | float | 64 |
| DeInterleave | int8_t | int8_t | 256 |
| DeInterleave | uint8_t | uint8_t | 256 |
| DeInterleave | int16_t | int16_t | 128 |
| DeInterleave | uint16_t | uint16_t | 128 |
| DeInterleave | half | half | 128 |
| DeInterleave | bfloat16_t | bfloat16_t | 128 |
| DeInterleave | int32_t | int32_t | 64 |
| DeInterleave | uint32_t | uint32_t | 64 |
| DeInterleave | float | float | 64 |

**表15**  数据压缩

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Unsqueeze | MaskReg | int8_t | 64 |
| Unsqueeze | MaskReg | uint8_t | 64 |
| Unsqueeze | MaskReg | int16_t | 32 |
| Unsqueeze | MaskReg | uint16_t | 32 |
| Unsqueeze | MaskReg | int32_t | 16 |
| Unsqueeze | MaskReg | uint32_t | 16 |
| Pack | int16_t | uint8_t | 128 |
| Pack | uint16_t | uint8_t | 128 |
| Pack | int32_t | uint16_t | 64 |
| Pack | uint32_t | uint16_t | 64 |
| UnPack | int8_t | int16_t | 128 |
| UnPack | uint8_t | uint16_t | 128 |
| UnPack | int16_t | int32_t | 64 |
| UnPack | uint16_t | uint32_t | 64 |

**表16**  直方图计算

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Histograms | uint8_t | uint16_t | 32 |

**表17**  索引操作

| 接口 | 源操作数数据类型 | 目的操作数数据类型 | 理论并行度（elements/cycle） |
| -------- | ---------------- | ---------------- | ---------------- |
| Arange | int8_t | int8_t | <!-- npu="950" id91 -->针对Ascend 950PR/Ascend 950DT：<br>128<br><!-- end id91 --><!-- npu="960" id92 -->针对Ascend 960PR/Ascend 960DT：<br>256<br><!-- end id92 --> |
| Arange | int16_t | int16_t | <!-- npu="950" id93 -->针对Ascend 950PR/Ascend 950DT：<br>64<br><!-- end id93 --><!-- npu="960" id94 -->针对Ascend 960PR/Ascend 960DT：<br>128<br><!-- end id94 --> |
| Arange | half | half | 128 |
| Arange | int32_t | int32_t | <!-- npu="950" id95 -->针对Ascend 950PR/Ascend 950DT：<br>32<br><!-- end id95 --><!-- npu="960" id96 -->针对Ascend 960PR/Ascend 960DT：<br>64<br><!-- end id96 --> |
| Arange | float | float | 64 |
