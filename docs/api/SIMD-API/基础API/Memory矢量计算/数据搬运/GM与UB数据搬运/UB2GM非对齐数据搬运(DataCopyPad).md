# UB2GM非对齐数据搬运(DataCopyPad)<a name="ZH-CN_TOPIC_0000002574928191"></a>

## 产品支持情况<a name="zh-cn_topic_0000002565968807_section796754519912"></a>

| 产品 | 是否支持 |
| :--- | :---: |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ |

## 功能说明<a name="zh-cn_topic_0000002565968807_section106841136114319"></a>

头文件为: #include "basic_api/kernel_operator_data_copy_intf.h"

该接口提供将数据从Unified Buffer非对齐搬运至Global Memory的功能。

-   对于32B对齐搬运搬运场景，从UB读取的所有数据都会搬运至GM。
-   对于非32B对齐的场景，在读取UB数据时会填入dummy假数据，对齐到32B，搬入GM时会将dummy空数据丢弃，从而实现UB到GM的非对齐搬运，如[图1](#zh-cn_topic_0000002565968807_fig15924125113617)。

## 函数原型<a name="zh-cn_topic_0000002565968807_section82039854412"></a>

```
template <typename T>
__aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams)
```

## 参数说明<a name="zh-cn_topic_0000002565968807_section16128134420472"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| T | 源操作数或者目的操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002565968807_section4219135304818)。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| :--- | :---: | :--- |
| dst | 输出 | 目的操作数，类型为为GlobalTensor，存储位置为Global Memory，目的地址需要1B对齐。 |
| src | 输入 | 源操作数，类型LocalTensor，存储位置为Unified Buffer，源地址需要32B对齐。 |
| dataCopyParams | 输入 | 搬运参数类型，通过该参数可以配置搬运的数据块个数、长度、地址间隔等信息，同时支持非连续和连续搬运。DataCopyExtParams类型，具体参数说明请参考[表3](#zh-cn_topic_0000002565968807_table144203616291)。 |

**表 3**  DataCopyExtParams结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| blockCount | 搬运的数据块个数，数据类型为uint16_t，取值范围：blockCount∈[0, 4095]。<br>**注：blockCount = 0表示不执行搬运，该接口将被视为 NOP（空操作）**。 |
| blockLen | 搬运的每个数据块长度，数据类型为uint32_t，取值范围：blockLen∈[0, 2097151]，单位：1B。<br>**注：blockLen= 0表示不执行搬运，该接口将被视为 NOP（空操作）**。 |
| srcStride | 源操作数相邻数据块之间的间隔（即前一个数据块**结束地址**与后一个数据块**起始地址**的差值），uint32_t类型，取值范围：srcStride∈[0, 2^32 - 1]，单位：dataBlock（32B）。 |
| dstStride | 目的操作数相邻数据块之间的间隔（即前一个数据块**结束地址**与后一个数据块**起始地址**的差值），uint32_t类型，取值范围：dstStride∈[0, 2^32 - 1]，单位：1B。 |
| rsv | 保留字段，默认值为0。 |

**图 2**  blockLen不满足32字节对齐<a name="zh-cn_topic_0000002565968807_fig179380535217"></a>  
![](figures/blockLen不满足32字节对齐.png "blockLen不满足32字节对齐")

如[图2](#zh-cn_topic_0000002565968807_fig179380535217)，将两个数据块搬运从UB搬运至GM，每个数据块的blockLen为47B，源操作数相邻数据块之间的间隔为32B，目的操作数相邻数据块之间的间隔为1B。

由于数据块的blockLen为 47B不满足32B对齐，在读取UB数据时会填入dummy空数据，对齐到32B，搬入GM时会将dummy空数据丢弃，从而实现UB到GM的非对齐搬运。

为了实现上述搬运，DataCopyExtParams结构体参数配置如下：

-   blockCount = 2，搬运2个数据块。
-   blockLen = 47，每个连续传输数据块包含47B。
-   srcStride = 1，源操作数相邻数据块之间的间隔为1，单位：32B。
-   dstStride = 1，目的操作数相邻数据块之间的间隔为1，单位：1B。

## 数据类型<a name="zh-cn_topic_0000002565968807_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、 uint64_t、 half、bfloat16_t、float、double

Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、 uint64_t、 half、bfloat16_t、float、double

## API映射关系<a name="zh-cn_topic_0000002565968807_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（copy_ubuf_to_gm_align_xx）的基础上进行抽象封装实现的，其对应的底层built-in接口为：

```
void copy_ubuf_to_gm_align_b8(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_ubuf_to_gm_align_b16(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_tsrcGap, uint32_t dstGap);

void copy_ubuf_to_gm_align_b32(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);
```

## 约束说明<a name="zh-cn_topic_0000002565968807_section2045914466492"></a>

-   位于Unified Buffer的源地址必须32B对齐，位于Global Memory的目的地址必须1B对齐。
-   DataCopyExtParams结构体参数的值需在取值范围内：

    **表 4**  DataCopyExtParams结构体参数取值范围

    | 参数名称 | 取值范围 |
    | :--- | :--- |
    | blockCount | [0, 4095] |
    | blockLen | [0, 2097151] |
    | srcStride | [0, 2^32 - 1] |
    | dstStride | [0, 2^32 - 1] |
