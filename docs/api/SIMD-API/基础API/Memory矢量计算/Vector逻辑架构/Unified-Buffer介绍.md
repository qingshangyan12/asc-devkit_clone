# Unified Buffer介绍<a name="ZH-CN_TOPIC_0000002574022839"></a>

## Unified Buffer地址对齐约束<a name="zh-cn_topic_0000002524627718_section796754519912"></a>

Unified Buffer存储单元要求32Byte对齐。

## Unified Buffer地址重叠约束<a name="zh-cn_topic_0000002524627718_section668772811100"></a>

使用基础API的Tensor高维切分计算接口时，为了节省地址空间，开发者可以定义一个Tensor供源操作数与目的操作数复用（即地址重叠）。使用时需要注意以下约束：

- 单次迭代内：源操作数与目的操作数必须100%完全重叠，不支持部分重叠。
- 多次迭代间：不支持前序迭代的目的操作数与后序迭代的源操作数重叠。例如，第N次迭代的目的操作数是第N+1次的源操作数。在这种情况下，第N次迭代可能会覆盖源操作数的数值，导致无法得到预期结果。特别地，对于部分双目计算类的API（[`Add`](../基础算术/Add.md)、[`Sub`](../基础算术/Sub.md)、[`Mul`](../基础算术/Mul.md)、[`Max`](../基础算术/Max.md)、[`Min`](../基础算术/Min.md)、[`AddRelu`](../复合计算/AddRelu.md)、[`SubRelu`](../复合计算/SubRelu.md)），当数据类型为`half`、`int32_t`、`float`时，支持前序迭代的目的操作数与后序迭代的源操作数重叠：仅针对目的操作数和第二个源操作数重叠的情况，且`src1RepStride`或者`dstRepStride`必须为0。

**图 1**  地址重叠示例（不支持）

![](../../../../figures/address_overlap_unsupported.png)

>[!NOTE]说明
>本节所述地址重叠通用约束适用于一般情况。如果API参考中有额外说明，以具体API说明为准；如果API中没有描述地址重叠约束，则视为不支持Tensor高维切分计算中的地址重叠，地址重叠时计算结果可能不满足预期。
