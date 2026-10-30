# Overview<a name="ZH-CN_TOPIC_0000002573904727"></a>

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-26T09:21:43.031Z pushedAt=2026-10-28T03:14:39.472Z nl=2 -->

Composite computation generally refers to the processing of composite functions or operations in mathematics or programming.

**Table 1**  Description of composite instructions

| API | Mathematical Expression |
| :--- | :--- |
| [Axpy](Axpy.md) | $dst_j=scalar \times src_i + dst_i$ |
| [MulAddDst](MulAddDst.md) | $dst_j=src0_i \times src1_i + dst_i$ |
| [MulCast](MulCast.md) | $dst_i=Cast(src0_i \times src1_i)$ |
| [MulsCast(ISASI)](MulsCast_ISASI.md) | $dst_i=Cast(src_i \times scalar)$ |
| [AddRelu](AddRelu.md) | $dst_i=Relu(src0_i + src1_i)$ |
| [AddReluCast](AddReluCast.md) | $dst_i=Cast(Relu(src0_i + src1_i))$ |
| [SubRelu](SubRelu.md) | $dst_i=Relu(src0_i - src1_i)$ |
| [SubReluCast](SubReluCast.md) | $dst_i=Cast(Relu(src0_i - src1_i))$ |
| [AbsSub(ISASI)](AbsSub_ISASI.md) | $dst_i=\lvert src0_i - src1_i\rvert$ |
| [ExpSub(ISASI)](ExpSub_ISASI.md) | $dst_i=e^{src0_i - src1_i}$ |
| [FusedMulAdd](FusedMulAdd.md) | $dst_j=src0_i \times dst_i + src1_i$ |
| [MulAddRelu](MulAddRelu.md) | $dst_i=Relu(src0_i \times dst_i + src1_i)$ |
| [CastDequant](CastDequant.md) | • Input data type is int16_t, with vector quantization mode disabled<br/>$dst_i=Cast(src_i \times scale + offset)$<br>• Input data type is int16_t, with vector quantization mode enabled<br>$dst_i=Cast(src_i \times scale_j + offset_j),j \in [0,15]$<br>• Input type is int32_t, quantizing the int32_t input and performing precision conversion (using RINT rounding)<br>$dst_i=Cast(src_i \times scale)$ |
| [AddDeqRelu](AddDeqRelu.md) | $dst_i=Relu(Deq(src0_i + src1_i))$<br>$Deq(x)=Cast(x \times scale)$ |

## Recommended Use Scenarios<a name="zh-cn_topic_0000002558185995_section23771839304"></a>

- Scenarios with tight Unified Buffer (UB) resources.

  Using compound instructions reduces UB space usage without modifying the original data of the source operands.

- High-performance scenarios.

  Ignoring other factors such as bank conflicts, using compound instructions reduces the number of computation instructions, thereby improving performance.

- High-precision scenarios.

  When the source operand and destination operand have the same data type, compound instructions do not improve precision;

  when the destination operand has a higher-precision data type than the source operand, compound instructions improve precision (for example, when the source operand data type is half and the destination operand data type is float), such as Axpy and MulAddDst.

## Key Features

### Precision Conversion

The MulCast, AddReluCast, SubReluCast, and MulsCast instructions include type conversion operations. The conversion rules are as follows:

**Table 2**  Precision conversion rules

| data type of the source operand | data type of the destination operand | Introduction to Type Conversion Modes |
| :----------------- | :------------------- | :--------------- |
| float | half | Convert the source operand to the number representable by half in CAST_NONE mode, and store it into the destination operand in half format (overflow defaults to saturating). |
| half | int8_t | Convert the source operand to an integer in CAST_NONE mode, and store it into the destination operand in int8_t format (overflow defaults to saturating). |
| half | uint8_t | Convert the source operand to an integer in CAST_NONE mode, and store it into the destination operand in uint8_t format (overflow defaults to saturating). |
| int16_t | int8_t | Convert the source operand to the number representable by int8_t in CAST_NONE mode, and store it into the destination operand in int8_t format (overflow defaults to saturating). |
| int64_t | float | Convert the source operand to the number representable by float in CAST_NONE mode, and store it into the destination operand in float format (overflow defaults to saturating). |
| int64_t | int32_t | Store the source operand into the destination operand in int32_t format (overflow defaults to saturating) |

### Quantization Computation

The CastDequant and AddDeqRelu APIs involve the setting of quantization parameters and must be used together with the [SetDeqScale](../type_conversion_aux_config/SetDeqScale.md) API.

### Three-Operand Read

For the MulAddDst, FusedMulAdd, and MulAddRelu APIs, if the three operands do not overlap, a three-read-one-write scenario occurs (where the destination operand is both read and written). Because the hardware supports at most two reads and one write, the theoretical parallelism of the instruction is halved. In this case, you can overlap operands to reduce read operations and restore the original parallelism.
