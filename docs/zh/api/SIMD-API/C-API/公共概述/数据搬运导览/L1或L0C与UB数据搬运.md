# L1或L0C与UB数据搬运

## 总体说明<a name="ZH-CN_TOPIC_0000002574022823"></a>

L1 Buffer/L0C Buffer与Unified Buffer之间的数据搬运提供了灵活的接口支持，能够充分适配多样化的计算场景（见[表1](#zh-cn_topic_0000002534897870_table1417935217588)）。其中，Unified Buffer-\>L1 Buffer的数据搬运用于矩阵计算（参考[矩阵计算的搬入](../矩阵计算概述.md)），L0C Buffer-\>Unified Buffer、L1 Buffer-\>Unified Buffer用于将矩阵计算结果搬出，完成矩阵计算的输出流程（参考[矩阵计算的搬出](../矩阵计算概述.md)）。

**表 1**  L1 Buffer/L0C Buffer与Unified Buffer数据搬运接口概述<a name="zh-cn_topic_0000002534897870_table1417935217588"></a>

| 数据通路 | 功能 | 描述 |
|----------|------|------|
| Unified Buffer->L1 Buffer | 连续数据搬运(asc_copy_ub2l1) | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续数据搬运。 |
| Unified Buffer->L1 Buffer | 高维切分数据搬运(asc_copy_ub2l1) | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的数据搬运。 |
| L0C Buffer->Unified Buffer | 随路转换与量化(asc_copy_l0c2ub) | 支持多种随路能力的组合，asc_copy_l0c2ub接口内包含了设置寄存器与数据搬运能力。 |
| L0C Buffer->Unified Buffer | 随路量化激活搬运(asc_copy_l0c2ub) | 支持多种随路能力的组合，asc_copy_l0c2ub接口内包含了设置寄存器与数据搬运能力。 |
| L1 Buffer->Unified Buffer | 连续数据搬运（asc_copy_l12ub） | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续数据搬运。 |
| L1 Buffer->Unified Buffer | 高维切分数据搬运（asc_copy_l12ub） | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的数据搬运。 |

## asc_copy_ub2l1（UBToL1连续数据搬运）

asc_copy_ub2l1能够实现Unified Buffer到L1 Buffer的连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_ub2l1](../../vector_datamove/asc_copy_ub2l1.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Unified Buffer -> L1 Buffer
    - UB -> C1

搬运的数据用于[矩阵计算](../矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_ub2l1](../../vector_datamove/asc_copy_ub2l1.md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```c++
__aicore__ inline void asc_copy_ub2l1(__cbuf__ void* dst, __ubuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
```

## asc_copy_ub2l1（UBToL1高维切分数据搬运）

asc_copy_ub2l1能够实现Unified Buffer到L1 Buffer的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_ub2l1](../../vector_datamove/asc_copy_ub2l1.md)。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Unified Buffer -> L1 Buffer
    - UB -> C1

搬运的数据用于[矩阵计算](../矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_ub2l1](../../vector_datamove/asc_copy_ub2l1.md)。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

```c++
__aicore__ inline void asc_copy_ub2l1(__cbuf__ void* dst, __ubuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
```

<!-- npu="310p" id1 -->
## asc_copy_l0c2ub（L0CToUB随路量化激活搬运）

该接口主要实现将数据从L0C Buffer搬运至Unified Buffer，并支持多种随路能力的组合，接口内包含了设置寄存器与数据搬运能力。

接口详情请参见[asc_copy_l0c2ub](../../cube_datamove/asc_copy_l0c2ub.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L0C Buffer -> Unified Buffer
    - CO1 -> UB

搬运的数据为[矩阵计算](../矩阵计算概述.md)的结果，接口具体介绍请参考：[asc_copy_l0c2ub](../../cube_datamove/asc_copy_l0c2ub.md)。

该接口仅Atlas 推理系列产品AI Core支持。

src和dst分别为源操作数和目的操作数；intriParams和enhancedParams为搬运参数。

- 源操作数和目的操作数类型一致

    ```c++
    __aicore__ inline void asc_copy_l0c2ub(__ubuf__ void* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint16_t dst_stride, uint16_t src_stride)
    ```

- 源操作数和目的操作数类型不一致

    ```c++
    __aicore__ inline void asc_copy_l0c2ub(__ubuf__ void* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint16_t dst_stride, uint16_t src_stride)
    ```
<!-- end id1 -->

## asc_copy_l0c2ub（L0CToUB随路量化激活搬运）

该接口主要实现将数据从L0C Buffer搬运至Unified Buffer，并支持多种随路能力的组合，接口内包含了设置寄存器与数据搬运能力。

接口详情请参见[asc_copy_l0c2ub](../../cube_datamove/asc_copy_l0c2ub.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L0C Buffer -> Unified Buffer
    - CO1 -> UB

搬运的数据为[矩阵计算](../矩阵计算概述.md)的结果，以Ascend 950PR/Ascend 950DT为例，接口示例如下：

注意，不同产品型号的接口原型可能不同，具体介绍请参考：[asc_copy_l0c2ub](../../cube_datamove/asc_copy_l0c2ub.md)。

src和dst分别为源操作数和目的操作数；intriParams为搬运参数，cbufWorkspace为开启tensor量化时所需的量化参数。

- 不开启随路[tensor量化](../矩阵计算概述.md)功能：

    ```c++
    __aicore__ inline void asc_copy_l0c2ub(__ubuf__ void* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint16_t dst_stride, uint16_t src_stride)
    ```

- 开启随路[tensor量化](../矩阵计算概述.md)功能：

    ```c++
    __aicore__ inline void asc_copy_l0c2ub(__ubuf__ void* dst_addr, __cc__ int32_t* src_addr, uint16_t n_size, uint16_t m_size, uint16_t dst_stride, uint16_t src_stride)
    ```

<!-- npu="950" id2 -->
## asc_copy_l12ub（L1ToUB连续数据搬运）

asc_copy_l12ub能够实现L1 Buffer到Unified Buffer的连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_l12ub](../../cube_datamove/asc_copy_l12ub.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L1 Buffer -> Unified Buffer
    - C1 -> UB

搬运的数据用于[矩阵计算](../矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_l12ub](../../cube_datamove/asc_copy_l12ub.md)。

该接口仅Ascend 950PR/Ascend 950DT支持。

该接口仅适用于AIC:AIV为1:2的Mix场景。`sub_blockid`用于选择目的Vector Core，false表示AIV0，true表示AIV1。

src和dst分别为源操作数和目的操作数；`n_burst`、`len_burst`、`src_gap`和`dst_gap`用于配置连续传输数据块。

```c++
__aicore__ inline void asc_copy_l12ub(__ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
```
<!-- end id2 -->

<!-- npu="950" id3 -->
## asc_copy_l12ub（L1ToUB高维切分数据搬运）

asc_copy_l12ub能够实现L1 Buffer到Unified Buffer的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_l12ub](../../cube_datamove/asc_copy_l12ub.md)。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L1 Buffer -> Unified Buffer
    - C1 -> UB

搬运的数据用于[矩阵计算](../矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_l12ub](../../cube_datamove/asc_copy_l12ub.md)。

该接口仅Ascend 950PR/Ascend 950DT支持。

该接口仅适用于AIC:AIV为1:2的Mix场景。`sub_blockid`用于选择目的Vector Core，false表示AIV0，true表示AIV1。

src和dst分别为源操作数和目的操作数；`n_burst`、`len_burst`、`src_gap`和`dst_gap`用于配置高维切分搬运。

```c++
__aicore__ inline void asc_copy_l12ub(__ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
```
<!-- end id3 -->
