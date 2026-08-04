# L1与BiasTable Buffer数据搬运

## asc_copy_l12bt（L1ToBiasTable Buffer连续数据搬运）<a name="ZH-CN_TOPIC_0000002574022837"></a>

asc_copy_l12bt能够实现L1 Buffer至BiasTable Buffer的连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_l12bt](../asc_copy_l12bt/asc_copy_l12bt_arch_3510.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L1 Buffer -> BiasTable Buffer
    - C1 -> C2

搬运的数据用于矩阵计算，接口具体介绍请参考：[asc_copy_l12bt](../asc_copy_l12bt/asc_copy_l12bt_arch_3510.md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```c++
__aicore__ inline void asc_copy_l12bt(__bt__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
```

## asc_copy_l12bt（L1ToBiasTable Buffer高维切分数据搬运）<a name="ZH-CN_TOPIC_0000002543262936"></a>

asc_copy_l12bt能够实现L1 Buffer至BiasTable Buffer的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_l12bt](../asc_copy_l12bt/asc_copy_l12bt_arch_3510.md)。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L1 Buffer -> BiasTable Buffer
    - C1 -> C2

搬运的数据用于矩阵计算，接口具体介绍请参考：[asc_copy_l12bt](../asc_copy_l12bt/asc_copy_l12bt_arch_3510.md)。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

- 连续或非连续搬运场景（高维切分）

    ```c++
    __aicore__ inline void asc_copy_l12bt(__bt__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```

- 连续或非连续搬运场景，支持源操作数和目的操作数数据类型不一致

    ```c++
    __aicore__ inline void asc_copy_l12bt(__bt__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```
