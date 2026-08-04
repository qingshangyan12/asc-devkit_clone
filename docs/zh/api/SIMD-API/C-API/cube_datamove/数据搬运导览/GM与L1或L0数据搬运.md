# GM与L1或L0数据搬运

## 总体说明<a name="ZH-CN_TOPIC_0000002543422572"></a>

Global Memory与L1 Buffer/L0 Buffer之间的数据搬运提供了灵活、分层的接口支持，能够充分适配多样化的计算场景（见[表1](#zh-cn_topic_0000002565857683_table12546123753110)）。其中，Global Memory与L1 Buffer、Global Memory-\>L0A Buffer、Global Memory-\>L0B Buffer的数据搬运用于矩阵计算（参考[矩阵计算的搬入](../../cube_compute/矩阵计算概述.md)），L0C Buffer-\>Global Memory用于将矩阵计算结果搬出，完成矩阵计算的输出流程（参考[矩阵计算的搬出](../../cube_compute/矩阵计算概述.md)）。开发者能够通过灵活配置C API的显式标量参数，精确控制数据块数量、数据块长度及地址间隔等关键属性，实现高维切分数据搬运，充分释放硬件的数据搬运与计算协同能力，显著提升算子的执行效率与整体性能。

**表 1**  Global Memory与L1 Buffer/L0 Buffer数据搬运接口概述<a name="zh-cn_topic_0000002565857683_table12546123753110"></a>

| 数据通路 | 功能 | 描述 |
|----------|------|------|
| Global Memory与L1 Buffer | 连续数据搬运(asc_copy_gm2l1) | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续数据搬运。 |
| Global Memory与L1 Buffer | 高维切分数据搬运(asc_copy_gm2l1) | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的数据搬运。 |
| Global Memory->L1 Buffer | 随路转换-[ND2NZ](../../cube_compute/矩阵计算概述.md)搬运(asc_copy_gm2l1) | 支持在数据搬运时进行[ND](../../cube_compute/矩阵计算概述.md)到[NZ](../../cube_compute/矩阵计算概述.md)格式的转换。 |
| Global Memory->L1 Buffer | 随路转换-[DN2NZ](../../cube_compute/矩阵计算概述.md)搬运(asc_copy_gm2l1) | 支持在数据搬运时进行[DN](../../cube_compute/矩阵计算概述.md)到[NZ](../../cube_compute/矩阵计算概述.md)格式的转换。 |
| Global Memory->L1 Buffer | 非对齐数据搬运(asc_copy_gm2l1_align) | 支持数据非对齐搬运，可以根据开发者的需要自行填充数据。 |
| Global Memory->L1 Buffer | [NZ](../../cube_compute/矩阵计算概述.md)数据搬运(asc_copy_gm2l1) | 负责完成[NZ](../../cube_compute/矩阵计算概述.md)矩阵的数据搬运。 |
| Global Memory->L1 Buffer | [NZ](../../cube_compute/矩阵计算概述.md)数据搬运(asc_copy_gm2l1) | 负责完成[NZ](../../cube_compute/矩阵计算概述.md)矩阵的数据搬运。 |
| Global Memory->L0A Buffer | 2D格式分形矩阵搬运(`asc_copy_gm2l0a`) | 负责完成普通矩阵计算所需的2D格式数据的搬运。 |
| Global Memory->L0B Buffer | 2D格式分形矩阵搬运(`asc_copy_gm2l0b`) | 负责完成普通矩阵计算所需的2D格式数据的搬运。 |
| L0C Buffer->Global Memory | 随路量化激活搬运(asc_copy_l0c2gm) | 支持多种随路能力的组合，需要设置不同的寄存器，配合asc_copy_l0c2gm接口使能不同的数据搬运能力。 |
| L0C Buffer->Global Memory | 随路量化激活搬运(asc_copy_l0c2gm) | 支持多种随路能力的组合，asc_copy_l0c2gm接口内包含了设置寄存器与数据搬运能力。 |

## asc_copy_gm2l1 / asc_copy_l12gm（GM与L1连续数据搬运）<a name="ZH-CN_TOPIC_0000002574022813"></a>

asc_copy_gm2l1 / asc_copy_l12gm能够实现Global Memory和L1 Buffer之间的连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1.md)和[asc_copy_l12gm](../asc_copy_l12gm.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L1 Buffer
- L1 Buffer -> Global Memory

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1.md)和[asc_copy_l12gm](../asc_copy_l12gm.md)。

src和dst分别为源操作数和目的操作数；size为Global Memory到L1 Buffer连续搬运的字节数。L1 Buffer到Global Memory通过n_burst、len_burst、src_gap和dst_gap配置搬运。

- Global Memory -> L1 Buffer

    ```c++
    __aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    ```

- L1 Buffer -> Global Memory

    ```c++
    __aicore__ inline void asc_copy_l12gm(__gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```

## asc_copy_gm2l1 / asc_copy_l12gm（GM与L1高维切分数据搬运）<a name="ZH-CN_TOPIC_0000002543262916"></a>

asc_copy_gm2l1 / asc_copy_l12gm能够实现Global Memory和L1 Buffer之间的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1.md)和[asc_copy_l12gm](../asc_copy_l12gm.md)。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L1 Buffer
- L1 Buffer -> Global Memory

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1.md)和[asc_copy_l12gm](../asc_copy_l12gm.md)。

src和dst分别为源操作数和目的操作数；n_burst、len_burst、src_gap和dst_gap用于配置数据块个数、每个数据块长度以及源、目的地址间隔。

- Global Memory -> L1 Buffer

    ```c++
    __aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```

- L1 Buffer -> Global Memory

    ```c++
    __aicore__ inline void asc_copy_l12gm(__gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```

## asc_copy_gm2l1_nd2nz（GMToL1随路转换-ND2NZ搬运）<a name="ZH-CN_TOPIC_0000002573902841"></a>

该接口主要实现将数据从Global Memory搬运至L1 Buffer，并支持在数据搬运时进行[ND](../../cube_compute/矩阵计算概述.md)到[NZ](../../cube_compute/矩阵计算概述.md)格式的转换。

接口详情请参见[asc_copy_gm2l1_nd2nz](../asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L1 Buffer

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l1_nd2nz](../asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz.md)。

src和dst分别为源操作数和目的操作数；nd_num、n_value和d_value配置矩阵数量与形状，其余Stride参数配置源、目的矩阵间隔。

- 不支持enableSmallC0模式

    ```c++
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride)
    ```

<!-- npu="950" id1 -->
- 支持enableSmallC0模式（仅Ascend 950PR/Ascend 950DT支持）

    ```c++
    __aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
    ```
<!-- end id1 -->

<!-- npu="950" id2 -->
## asc_copy_gm2l1_dn2nz（GMToL1随路转换-DN2NZ搬运）<a name="ZH-CN_TOPIC_0000002573902899"></a>

该接口主要实现将数据从Global Memory搬运至L1 Buffer，并支持在数据搬运时进行[DN](../../cube_compute/矩阵计算概述.md)到[NZ](../../cube_compute/矩阵计算概述.md)格式的转换。

接口详情请参见[asc_copy_gm2l1_dn2nz](../asc_copy_gm2l1_dn2nz.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L1 Buffer

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l1_dn2nz](../asc_copy_gm2l1_dn2nz.md)。

src和dst分别为源操作数和目的操作数；loop1_src_stride、l2_cache_ctl、n_value、d_value、loop4_src_stride和smallc0_en直接配置DN2NZ搬运。

- 支持enableSmallC0模式（仅Ascend 950PR/Ascend 950DT支持）

    ```c++
    __aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
    ```
<!-- end id2 -->

<!-- npu="950" id3 -->
## asc_copy_gm2l1_align（GMToL1非对齐数据搬运）<a name="ZH-CN_TOPIC_0000002543262891"></a>

该接口主要实现将数据从Global Memory搬运至L1 Buffer，并支持在数据搬运时提供非对齐数据搬运功能，可以根据开发者的需要自行填充数据。

接口详情请参见[asc_copy_gm2l1_align](../asc_copy_gm2l1_align.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L1 Buffer

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l1_align](../asc_copy_gm2l1_align.md)。

src和dst分别为源操作数和目的操作数；n_burst、len_burst、左右填充数、data_select_bit、l2_cache_ctl以及源和目的Stride参数直接配置非对齐搬运。

- 仅Ascend 950PR/Ascend 950DT支持

    ```c++
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    ```
<!-- end id3 -->

## GMToL1 NZ数据搬运(asc_copy_gm2l1（2D矩阵搬运）)<a name="ZH-CN_TOPIC_0000002543422574"></a>

该接口主要实现将数据从Global Memory搬运至L1 Buffer，负责完成普通矩阵计算所需的2D格式数据的搬运，以大小为512Byte的数据分形为单位进行搬运。

接口详情请参见[asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L1 Buffer

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md)。

src和dst分别为源操作数和目的操作数；m_start_position、k_start_position、dst_stride、m_step、k_step、decomp_mode和l2_cache_ctl直接配置搬运。

```c++
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
```

<!-- npu="950" id4 -->
## GMToL1 NZ数据搬运(asc_copy_gm2l1（2D矩阵搬运V2）)

该接口主要实现将数据从Global Memory搬运至L1 Buffer，负责完成普通矩阵计算所需的2D格式数据的搬运，以大小为512Byte的数据分形为单位进行搬运。

接口详情请参见[asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L1 Buffer

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md)。

src和dst分别为源操作数和目的操作数；m_start_position、k_start_position、dst_stride、m_step、k_step、decomp_mode和l2_cache_ctl直接配置搬运。

- 仅Ascend 950PR/Ascend 950DT支持

    ```c++
    __aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
    ```
<!-- end id4 -->

## GMToL0A 2D格式分形矩阵搬运(asc_copy_gm2l0a（2D矩阵搬运）)<a name="ZH-CN_TOPIC_0000002574022815"></a>

该接口主要实现将数据从Global Memory搬运至L0A Buffer，负责完成普通矩阵计算所需的2D格式数据的搬运，以大小为512Byte的数据分形为单位进行搬运。

接口详情请参见[asc_copy_gm2l0a](../asc_copy_gm2l0a.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L0A Buffer

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l0a](../asc_copy_gm2l0a.md)。

src和dst分别为源操作数和目的操作数；base_idx、repeat、src_stride和dst_gap直接配置搬运。

```c++
__aicore__ inline void asc_copy_gm2l0a(__ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
```

## GMToL0B 2D格式分形矩阵搬运(asc_copy_gm2l0b（2D矩阵搬运）)<a name="ZH-CN_TOPIC_0000002543262918"></a>

该接口主要实现将数据从Global Memory搬运至L0B Buffer，负责完成普通矩阵计算所需的2D格式数据的搬运，以大小为512Byte的数据分形为单位进行搬运。

接口详情请参见[asc_copy_gm2l0b](../asc_copy_gm2l0b.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Global Memory -> L0B Buffer

搬运的数据用于[矩阵计算](../../cube_compute/矩阵计算概述.md)，接口具体介绍请参考：[asc_copy_gm2l0b](../asc_copy_gm2l0b.md)。

src和dst分别为源操作数和目的操作数；base_idx、repeat、src_stride和dst_gap直接配置搬运。

```c++
__aicore__ inline void asc_copy_gm2l0b(__cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
```

## asc_copy_l0c2gm（L0CToGM随路量化激活搬运）<a name="ZH-CN_TOPIC_0000002573902843"></a>

该接口主要实现将数据从L0C Buffer搬运至Global Memory，并支持多种随路能力的组合，需要设置不同的寄存器。

接口详情请参见[asc_copy_l0c2gm](../asc_copy_l0c2gm/asc_copy_l0c2gm_arch_2201.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L0C Buffer -> Global Memory

搬运的数据为[矩阵计算](../../cube_compute/矩阵计算概述.md)的结果，接口具体介绍请参考：[asc_copy_l0c2gm](../asc_copy_l0c2gm/asc_copy_l0c2gm_arch_2201.md)。

src和dst分别为源操作数和目的操作数；n_size、m_size、dst_stride_dst_d、src_stride、unit_flag_mode、quant_pre、relu_pre、channel_split和nz2nd_en直接配置矩阵搬出。

```c++
__aicore__ inline void asc_copy_l0c2gm(__gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split, bool nz2nd_en)
```

<!-- npu="950" id5 -->
## asc_copy_l0c2gm（L0CToGM随路量化激活搬运）<a name="ZH-CN_TOPIC_0000002543422576"></a>

该接口主要实现将数据从L0C Buffer搬运至Global Memory，并支持多种随路能力的组合，接口内包含了设置寄存器与数据搬运能力。

接口详情请参见[asc_copy_l0c2gm](../asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L0C Buffer -> Global Memory

搬运的数据为[矩阵计算](../../cube_compute/矩阵计算概述.md)的结果，以Ascend 950PR/Ascend 950DT为例，接口示例如下：

注意，不同产品型号的接口原型可能不同，具体介绍请参考：[asc_copy_l0c2gm](../asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)。

src和dst分别为源操作数和目的操作数；n_size、m_size、Stride、Cache以及量化、激活和格式转换参数直接配置矩阵搬出；开启tensor量化时，量化参数由相应的标量参数提供。

- 不开启随路[tensor量化](../../cube_compute/矩阵计算概述.md)功能：

    ```c++
    __aicore__ inline void asc_copy_l0c2gm(__gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    ```

- 开启随路[tensor量化](../../cube_compute/矩阵计算概述.md)功能：

    ```c++
    __aicore__ inline void asc_copy_l0c2gm(__gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    ```
<!-- end id5 -->
