# L1与Fixpipe Buffer数据搬运

## asc_copy_l12fb（L1ToFixpipe Buffer连续数据搬运）<a name="ZH-CN_TOPIC_0000002543262934"></a>

asc_copy_l12fb能够实现L1 Buffer至Fixpipe Buffer的连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_l12fb](../asc_copy_l12fb/asc_copy_l12fb_arch_3510.md)。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L1 Buffer -> Fixpipe Buffer

搬运的数据为[随路量化](../../cube_compute/矩阵计算概述.md)和[随路ReLU](../../cube_compute/矩阵计算概述.md)参数，接口具体介绍请参考：[asc_copy_l12fb](../asc_copy_l12fb/asc_copy_l12fb_arch_3510.md)。

src和dst分别为源操作数和目的操作数；size为连续搬运的字节数。

```c++
__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
```

## asc_copy_l12fb（L1ToFixpipe Buffer高维切分数据搬运）<a name="ZH-CN_TOPIC_0000002573902863"></a>

asc_copy_l12fb能够实现L1 Buffer至Fixpipe Buffer的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

接口详情请参见[asc_copy_l12fb](../asc_copy_l12fb/asc_copy_l12fb_arch_3510.md)。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的数据通路为（以C API地址空间限定符表示）：

- L1 Buffer -> Fixpipe Buffer

搬运的数据为[随路量化](../../cube_compute/矩阵计算概述.md)和[随路ReLU](../../cube_compute/矩阵计算概述.md)参数，接口具体介绍请参考：[asc_copy_l12fb](../asc_copy_l12fb/asc_copy_l12fb_arch_3510.md)。

src和dst分别为源操作数和目的操作数；n_burst、len_burst、src_gap_size和dst_gap_size直接配置高维切分搬运。

```c++
__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size, uint16_t dst_gap_size)
```
