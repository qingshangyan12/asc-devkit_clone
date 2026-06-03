# GM2UB非对齐数据搬运(DataCopyPad)<a name="ZH-CN_TOPIC_0000002544407954"></a>

## 产品支持情况<a name="zh-cn_topic_0000002565888823_section796754519912"></a>

| 产品 | 是否支持 |
| :--- | :---: |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ |

## 功能说明<a name="zh-cn_topic_0000002565888823_section106841136114319"></a>

头文件为: #include "basic_api/kernel_operator_data_copy_intf.h"

该接口提供将数据从Global Memory非对齐搬运至Unified Buffer的功能，可以根据开发者的需要自行填充数据。

填充的数据有两种配置方式：

-   配置结构体[DataCopyPadExtParams](#zh-cn_topic_0000002565888823_table2036831862917)的参数paddingValue，此时DataCopyPadExtParams的参数isPad需要设置为true。
-   使用寄存器配置接口[SetPadValue](寄存器配置说明.md#ZH-CN_TOPIC_0000002544248308)在接口外部设置需要填充的数据，此时DataCopyPadExtParams的参数isPad需要设置为false。

## 函数原型<a name="zh-cn_topic_0000002565888823_section82039854412"></a>

```
template <typename T> 
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams) 
```

## 参数说明<a name="zh-cn_topic_0000002565888823_section16128134420472"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| T | 操作数以及paddingValue（待填充数据值）的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002565888823_section4219135304818)。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| :--- | :---: | :--- |
| dst | 输出 | 目的操作数，类型为LocalTensor，存储位置为Unified Buffer，目的地址需要32B对齐。 |
| src | 输入 | 源操作数，类型为GlobalTensor，存储位置为Global Memory，源地址需要1B对齐。 |
| dataCopyParams | 输入 | 搬运参数。DataCopyExtParams类型，具体参数说明请参考[表3](#zh-cn_topic_0000002565888823_table108821810182917)。 |
| padParams | 输入 | 从Global Memory搬运数据至Local Memory时，可以根据开发者需要，在搬运数据左边或右边填充数据。padParams是用于控制数据填充过程的参数。DataCopyPadExtParams类型，具体参数请参考[表4](#zh-cn_topic_0000002565888823_table2036831862917)。 |

**表 3**  DataCopyExtParams参数说明

| 参数名 | 描述 |
| :--- | :--- |
| blockCount | 搬运的数据块个数，uint16_t类型，取值范围：blockCount∈[0, 4095]，默认值为1。<br>**注：blockCount = 0表示不执行搬运，该接口将被视为 NOP（空操作）**。 |
| blockLen | 搬运的每个数据块长度，数据类型为uint32_t，取值范围：blockLen∈[0, 2097151]，单位：1B。<br>**注：blockLen= 0表示不执行搬运，该接口将被视为 NOP（空操作）**。**blockLen必须是 sizeof(T) 的整数倍，需要注意UB不要超限。** |
| srcStride | 源操作数相邻数据块之间的间隔（即前一个数据块**结束地址**与后一个数据块**起始地址**的差值），uint32_t类型，取值范围：srcStride∈[0, 2^32 - 1]，单位：1B。 |
| dstStride | 目的操作数相邻数据块之间的间隔（即前一个数据块**结束地址**与后一个数据块**起始地址**的差值），uint32_t类型，取值范围：dstStride∈[0, 2^32 - 1]，单位：dataBlock（32B）。<br>**注：需要注意UB不要超限**。 |
| rsv | 保留字段。uint32_t类型，默认值为0。 |

**表 4**  DataCopyPadExtParams参数说明

| 参数名 | 描述 |
| :--- | :--- |
| isPad | 是否需要填充用户自定义的数据。<br>&bull; true：填充padding value。<br>&bull; false：表示用户不需要指定填充值，会默认填充随机值。 |
| leftPadding | leftPadding元素个数。参数取值范围为0到255，leftPadding、rightPadding与blockLen组成的总元素个数，需满足不超过65535元素的限制。 |
| rightPadding | rightPadding元素个数。参数取值范围为0到255，leftPadding、rightPadding与blockLen组成的总元素个数，需满足不超过65535元素的限制。 |
| paddingValue | 左右两侧需要填充的数据值，需要保证在数据占用字节范围内。数据类型和源操作数保持一致，T数据类型。**当数据类型长度为64位时，该参数只能设置为0。** |

**图 1**  blockLen+leftPadding+rightPadding满足32B对齐<a name="zh-cn_topic_0000002565888823_fig1039515311820"></a>  
![](figures/blockLen+leftPadding+rightPadding满足32B对齐.png "blockLen+leftPadding+rightPadding满足32B对齐")

如[图1](#zh-cn_topic_0000002565888823_fig1039515311820)，将两个数据块从GM搬运至UB，每个blockLen为48B，源操作数相邻数据块之间的间隔为1B，目的操作数相邻数据块之间的间隔为32B。

在blockLen左侧和右侧分别填充2个和3个half类型元素，此时blockLen + leftPadding + rightPadding = 48B + 2 * 2B + 3 * 2B = 58B，不足64B，该指令会在rightPadding后补6B的dummy假数据，使得blockLen + leftPadding + rightPadding + dummy满足32B对齐。

为了实现上述搬运，DataCopyExtParams参数配置如下：

-   blockCount = 2，搬运2个数据块。
-   blockLen =48，每个连续传输数据块包含48B。
-   srcStride = 1，源操作数相邻数据块之间的间隔为1B。
-   dstStride = 1，目的操作数相邻数据块之间的间隔为1个DataBlock（32B）。

DataCopyPadExtParams参数配置如下：

-   isPad = true，使用DataCopyPadExtParams内参数paddingValue的值作为填充值；或者设置isPad = false，使用寄存器配置接口[SetPadValue](寄存器配置说明.md#ZH-CN_TOPIC_0000002544248308)在接口外部设置填充值。
-   leftPadding = 2，搬运结果左侧填充2个half数据。
-   rightPadding = 3，搬运结果右侧填充3个half数据。
-   paddingValue = 1，blockLen的左右两侧填充值为1。

## blockLen+leftPadding+rightPadding不满足32B对齐<a name="zh-cn_topic_0000002565888823_section73461020593"></a>

**图 2**  blockLen+leftPadding+rightPadding不满足32B对齐<a name="zh-cn_topic_0000002565888823_fig1443611515363"></a>  
![](figures/blockLen+leftPadding+rightPadding不满足32B对齐.png "blockLen+leftPadding+rightPadding不满足32B对齐")

如[图2](#zh-cn_topic_0000002565888823_fig1443611515363)，将两个数据块从GM搬运至UB，每个blockLen为47B，源操作数相邻数据块之间的间隔为1B，目的操作数相邻数据块之间的间隔为32B。

在blockLen左侧和右侧分别填充2个和3个half类型元素，此时blockLen + leftPadding + rightPadding = 48B + 2 * 2B + 3 * 2B = 58B，不满足32B对齐，该指令会在rightPadding后补dummy假数据，使得blockLen + leftPadding + rightPadding + dummy满足32B对齐。

DataCopyExtParams参数配置如下：

-   blockCount = 2，搬运2个数据块。
-   blockLen =48，每个连续传输数据块包含48B。
-   srcStride = 1，源操作数相邻数据块之间的间隔为1B。
-   dstStride = 1，目的操作数相邻数据块之间的间隔为1个DataBlock（32B）。

DataCopyPadExtParams参数配置如下：

-   isPad = true，使用DataCopyPadExtParams内参数paddingValue的值作为填充值；或者设置isPad = false，使用寄存器配置接口[SetPadValue](寄存器配置说明.md#ZH-CN_TOPIC_0000002544248308)在接口外部设置填充值。
-   leftPadding = 2，搬运结果左侧填充2个half数据。
-   rightPadding = 3，搬运结果右侧填充3个half数据。
-   paddingValue = 1，blockLen的左右两侧填充值为1。

## 数据类型<a name="zh-cn_topic_0000002565888823_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、 float、int64_t、 uint64_t、double

Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、 float、int64_t、 uint64_t、double

## API映射关系<a name="zh-cn_topic_0000002565888823_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（set_mov_pad_val、copy_gm_to_ubuf_align）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
void set_mov_pad_val(uint64_t config);

void copy_gm_to_ubuf_align_b8(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_gm_to_ubuf_align_b16(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_gm_to_ubuf_align_b32(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);
```

## 约束说明<a name="zh-cn_topic_0000002565888823_section2045914466492"></a>

-   位于Global Memory的源地址必须1B对齐，位于Unified Buffer的目的地址必须32B对齐。
-   leftPadding、rightPadding所占字节数均不能超过32B。
-   blockLen必须是 sizeof(T) 的整数倍。
-   结构体DataCopyPadExtParams的参数paddingValue数据类型和源操作数保持一致。当数据类型为b64时，paddingValue只能设置为0。
-   结构体DataCopyPadExtParams的参数取值范围需要考虑UB超限问题。
