# UB与UB数据搬运<a name="ZH-CN_TOPIC_0000001499154365"></a>

## 总体说明<a name="ZH-CN_TOPIC_0000002574022829"></a>

Unified Buffer（UB）之间的数据搬运提供了灵活的接口支持，共有两组接口：asc_copy_ub2ub提供了基础的数据搬运能力，支持连续和非连续数据搬运；asc_copy除了支持连续和非连续数据搬运，还提供了mask操作和DataBlock间隔操作的能力。具体接口功能如下表所示。

**表 1**  UB与UB数据搬运接口概述

<a name="zh-cn_topic_0000002568294885_table12546123753110"></a>

| 数据通路 | 功能 | 描述 |
| :---: | :--- | :--- |
| UB->UB | 连续数据搬运(asc_copy_ub2ub) | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续数据搬运。 |
| UB->UB | 高维切分数据搬运(asc_copy_ub2ub) | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的数据搬运。 |
| UB->UB | 连续数据搬运(asc_copy) | 支持连续数据搬运，数据搬运时格式和内容保持不变。 |
| UB->UB | 掩码式高维数据搬运(asc_copy) | 支持mask操作和DataBlock间隔操作，支持在接口内部和接口外部设置mask。 |

## asc_copy_ub2ub（UB -> UB连续数据搬运）<a name="ZH-CN_TOPIC_0000002382907025"></a>

支持Unified Buffer与Unified Buffer之间的连续数据搬运，数据在传输过程中保持原始格式和内容不变。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Unified Buffer -> Unified Buffer

搬运的数据用于[矢量计算](../../vector_compute/Memory矢量计算.md)，具体的接口请参考：[asc_copy_ub2ub](../asc_copy_ub2ub.md)。

src和dst分别为源操作数和目的操作数；size为连续搬运的字节数。

```c++
__aicore__ inline void asc_copy_ub2ub(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
```

## asc_copy_ub2ub（UB -> UB高维切分数据搬运）<a name="ZH-CN_TOPIC_0000002382908321"></a>

支持Unified Buffer与Unified Buffer之间的高维切分数据搬运，数据在传输过程中保持原始格式和内容不变。

高维切分数据搬运可通过配置数据块个数、单个数据块长度、地址偏移等搬运参数，同时支持非连续和连续的数据搬运。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Unified Buffer -> Unified Buffer

搬运的数据用于[矢量计算](../../vector_compute/Memory矢量计算.md)，具体的接口请参考：[asc_copy_ub2ub](../asc_copy_ub2ub.md)。

src和dst分别为源操作数和目的操作数；n_burst、len_burst、src_gap和dst_gap分别配置数据块个数、每个数据块长度、源地址间隔和目的地址间隔。

```c++
__aicore__ inline void asc_copy_ub2ub(__ubuf__ void* dst, __ubuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
```

## asc_copy（UB -> UB连续数据搬运）<a name="ZH-CN_TOPIC_0000002575088175"></a>

支持Unified Buffer和Unified Buffer之间的连续数据搬运，数据搬运时格式和内容保持不变。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Unified Buffer -> Unified Buffer

搬运的数据用于[矢量计算](../../vector_compute/Memory矢量计算.md)，具体的接口请参考：[asc_copy](../../vector_compute/asc_copy.md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```c++
__aicore__ inline void asc_copy(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
```

## asc_copy（UB -> UB掩码式高维数据搬运）<a name="ZH-CN_TOPIC_0000002575088676"></a>

支持Unified Buffer和Unified Buffer之间的数据搬运，数据搬运时格式和内容保持不变，支持mask操作和DataBlock间隔操作。

具体支持的数据通路为（以C API地址空间限定符表示）：

- Unified Buffer -> Unified Buffer

搬运的数据用于[矢量计算](../../vector_compute/Memory矢量计算.md)，具体的接口请参考：[asc_copy](../../vector_compute/asc_copy.md)。

src和dst分别为源操作数和目的操作数；repeat配置重复次数，dst_block_stride、src_block_stride、dst_repeat_stride和src_repeat_stride分别配置数据块内与重复间的源、目的Stride。mask状态需在调用接口前配置。

- mask逐bit模式

    ```c++
    __aicore__ inline void asc_copy(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    ```

- mask连续模式

    ```c++
    __aicore__ inline void asc_copy(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    ```
