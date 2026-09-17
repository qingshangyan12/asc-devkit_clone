# 修订记录
| 日期       | 修订版本 | 修改章节            | 修改描述 | 作者         |
| ---------- | -------- | ------------------- | -------- | ------------ |
| 2026-08-01 |    V1.0    |         ALL         |     初稿     | zengjuan 00945165 |

---hi-lake begin---


# 1. 算子需求与规格说明

## 1.1. 功能说明

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002270155325.png)

deconvolution是一个upsampling过程，像是convolution的反过程。但是deconvolution并不是一个好名字，他是存在一定的歧义的，正确的名字应该是transposed convolution，这个transpose的由来是他的计算方式：当deconv等效成矩阵乘法时，权重部分实际上就是对应conv的权重矩阵之转置矩阵。

根据反卷积的数学含义，通过反卷积可以将通过卷积的输出信号，完全还原输入信号。而事实是，转置卷积只能还原shape大小，而不能还原value。你可以理解成，至少在数值方面上，转置卷积不能实现卷积操作的逆过程。所以说转置卷积与真正的反卷积有点相似，因为两者产生了相同的空间分辨率。因此反卷积（deconvolutions）的这种叫法是不合适的，因为它不符合反卷积的概念，因此现在对于反卷积还有一种transposed convolution的叫法。

以一个实际的例子来说明transposed convolution过程：
先看convolution过程，连接方式 如下图所示，绿色表示输出，蓝色表示输入，每个绿色块具与9个蓝色块连接。

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002235076094.png)

令卷积核 W =
![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002270155317.png)

为了便于理解，将卷积写成矩阵乘法形式。将4X4的输入矩阵以行优先的方式拉成长度为16的向量。2X2的输出矩阵以同样的方式拉长为长度为4的向量，则W可以表示为一个4X16的稀疏矩阵C。

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002235235890.png)

所以，convolution的过程可以描述为Cx=y.

作为convoluton反向过程的transposed convolution，是希望将长度为4的y向量映射成长度为16的x向量，且需要保持连接关系相同。则我们将C转置即可。得到CTy = x’。此时，1个蓝色块与9个绿色块连接。且权重连接关系与convolution相同。

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002270275229.png)

从一个具体的case来直观的理解这个计算过程：

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002235076098.png)

根据上图可以看到，卷积中输入左上角的3*3的框中的数据通过加权和最终得到输出数据中左上角的数据，相对应的反卷积中右下角的输入数据最终作用在了输出数据左上角3*3框中的数据上，这就保持了卷积及其对应反卷积中输入输出数据映射关系的一致性，输出数据中左上角数据与权重的相互遍历关系，正好是权重的转置。
上面是一个定性的分析，实际上，deconv就是conv backward的过程：

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002235076090.png)

其中L是损失函数，X是conv的输入load3d之后的矩阵，Y是conv的输出，W是conv的权重展开之后的矩阵，这个公式表达的是backward的过程：即根据L对Y的梯度和权重W来计算L对X的梯度。图示如下：

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002235235886.png)

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002270275221.png)

相应的，deconv通过col2img（img2col的逆过程）从输出矩阵CoutKK*HinWin来恢复出Cout*Hout*Wout。
由于Davinci硬件不支持col2img操作，因此NPU实现不通过col2img完成，通过插0法和拆分法。

## 1.2. 算子IR定义

### 1.2.1. CANN IR

算子基于如下CANN IR实现，IR路径：vendor/hisi/npu/inc/framework/graph/op/internal_nn_defs.h(临时路径，待调整)

```
HIAI_REG_OP(ExtendConvTranspose)
.HIAI_INPUT(input_size, TensorType({ DT_INT32, DT_INT64 }))
.HIAI_INPUT(x, TensorType({ DT_FLOAT16, DT_FLOAT, DT_BF16, DT_INT8 }))
.HIAI_INPUT(filter, TensorType({ DT_FLOAT16, DT_FLOAT, DT_BF16, DT_INT8 }))
.HIAI_OPTIONAL_INPUT(bias, TensorType({ DT_FLOAT16, DT_FLOAT32, DT_INT32 }))
.HIAI_OPTIONAL_INPUT(scale, TensorType({ DT_UINT64 }))
.HIAI_OUTPUT(y, TensorType({ DT_FLOAT16, DT_FLOAT, DT_BF16, DT_INT8 }))
.HIAI_REQUIRED_ATTR(strides, AttrValue::LIST_INT)
.HIAI_REQUIRED_ATTR(pads, AttrValue::LIST_INT)
.HIAI_ATTR(dilations, AttrValue::LIST_INT ({ 1, 1, 1, 1, 1 }))
.HIAI_ATTR(groups, AttrValue::INT { 1 })
.HIAI_ATTR(data_format, AttrValue::STR { "NDHWC" })
.HIAI_ATTR(output_padding, AttrValue::LIST_INT ({ 0, 0, 0, 0, 0 }))
.HIAI_ATTR(offset_x, AttrValue::INT { 0 })
.HIAI_ATTR(fusion_mode, AttrValue::INT { 0 })
.HIAI_ATTR(y_quant_mode, AttrValue::INT { 0 })
.HIAI_ATTR(pad_mode, AttrValue::STR { "SPECIFIC" })
.HIAI_OP_END()
```

### 1.2.1. FE IR

算子基于如下FE IR实现，IR路径：vendor/hisi/npu/src/cls/npucl/nn_core/fusion_engine/inc/fe_ir/convolution/conv_transpose_inner.h

```
HIAI_REG_OP(ConvTransposeInner)
    .HIAI_INPUT(input_size, TensorType({ DT_INT32, DT_INT64 }))
    .HIAI_INPUT(x, TensorType({ DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN }))
    .HIAI_INPUT(filter, TensorType({ DT_FLOAT16, DT_INT8, DT_FLOAT8_E4M3FN, DT_INT4 }))
    .HIAI_OPTIONAL_INPUT(bias, TensorType({ DT_FLOAT16, DT_INT32 }))
    .HIAI_OPTIONAL_INPUT(pre_quant, TensorType({ DT_UINT64 }))
    .HIAI_OPTIONAL_INPUT(pre_relu, TensorType({ DT_UINT32 }))
    .HIAI_OPTIONAL_INPUT(pre_clip_relu, TensorType({ DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN }))
    .HIAI_OPTIONAL_INPUT(eltwise, TensorType({ DT_FLOAT16, DT_INT32, DT_INT16, DT_INT8 }))
    .HIAI_OPTIONAL_INPUT(anti_quant, TensorType({ DT_UINT32 }))
    .HIAI_OPTIONAL_INPUT(post_quant, TensorType({ DT_UINT64 }))
    .HIAI_OPTIONAL_INPUT(post_relu, TensorType({ DT_UINT32 }))
    .HIAI_OPTIONAL_INPUT(post_clip_relu, TensorType({ DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN }))
    .HIAI_OPTIONAL_INPUT(padding, TensorType({DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN}))
    .HIAI_OUTPUT(y, TensorType({ DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN }))
    .HIAI_REQUIRED_ATTR(strides, AttrValue::LIST_INT)
    .HIAI_REQUIRED_ATTR(pads, AttrValue::LIST_INT)
    .HIAI_ATTR(dilations, AttrValue::LIST_INT ({ 1, 1, 1, 1 }))
    .HIAI_ATTR(groups, AttrValue::INT { 1 })
    .HIAI_ATTR(data_format, AttrValue::STR { "NCHW" })
    .HIAI_ATTR(output_padding, AttrValue::LIST_INT ({ 0, 0, 0, 0 }))
    .HIAI_ATTR(offset_x, AttrValue::INT { 0 })
    .HIAI_ATTR(fusion_mode, AttrValue::INT { 0 })
    .HIAI_ATTR(y_quant_mode, AttrValue::INT { 0 })
    .HIAI_ATTR(pad_mode, AttrValue::STR { "SPECIFIC" })
    .HIAI_ATTR(pre_dtype, AttrValue::INT { -1 })
    .HIAI_ATTR(post_dtype, AttrValue::INT { -1 })
    .HIAI_ATTR(pre_relu_mode, AttrValue::STR { "no" })
    .HIAI_ATTR(post_relu_mode, AttrValue::STR { "no" })
    .HIAI_ATTR(eltwise_op, AttrValue::STR { "no" })
    .HIAI_ATTR(bit_mask, AttrValue::INT { 0 })
    .HIAI_OP_END()
```

 
## 1.3. 算子规格说明

### 1.3.1. 基本约束

1）任意输入、输出的维度数量<=4
2）```1 <= N <= 65535，N*C*H*W < 2^31```

### 1.3.2. 规格说明


### 1.3.3. 使用建议
<!--
如高性能等建议。
-->

## 1.4. 版本规格兼容说明
|   版本    | CHS|NCH|GZH|
| ----- | ---- | --- | --- |
| **场景x**   |   √   |   √   |   √   |
 

---hi-lake end---
 
# 2. 软件实现说明
## 2.1. 算子模板

<!-- zj: 模板参数 -->

```
 extern "C" __global__ __aicore__ void conv_transpose_inner(GM_ADDR input_size,
                                                            GM_ADDR x,
                                                            GM_ADDR filter,
                                                            GM_ADDR bias,
                                                            GM_ADDR preQuant,
                                                            GM_ADDR preRelu,
                                                            GM_ADDR preClipRelu,
                                                            GM_ADDR eltwise,
                                                            GM_ADDR antiQuant,
                                                            GM_ADDR postQuant,
                                                            GM_ADDR postRelu,
                                                            GM_ADDR postClipRelu,
                                                            GM_ADDR y,
                                                            GM_ADDR workspace,
                                                            GM_ADDR tiling)
{
}

```
参数说明：
| 参数        | 描述            |
|-----------|-------------------|
| input_size       | input_size输入GM地址            |
| x                | x输入GM地址                     |
| filter           | filter输入GM地址                |
| bias             | bias输入GM地址                  |
| preQuant         | preQuant输入GM地址              |
| preRelu          | preRelu输入GM地址               |
| preClipRelu      | preClipRelu输入GM地址           |
| eltwise          | eltwise输入GM地址               |
| antiQuant        | antiQuant输入GM地址             |
| postQuant        | postQuant输入GM地址             |
| postRelu         | postRelu输入GM地址              |
| postClipRelu     | postClipRelu输入GM地址          |
| y                | y输出GM地址                     |
| worksapce        | workspace GM地址                |
| tiling           | TilingData GM地址               |

## 2.2. 算子实现
### 2.2.1. 通路适配
通路适配，如pass

### 2.2.2. OpBuilder实现
#### 2.2.2.1. GetFormatCfg


#### 2.2.2.2. CheckSupported


#### 2.2.2.3. GetOpOutputDesc


#### 2.2.2.4. TransWeights

### 2.2.3. 模板实现
#### 2.2.3.1. TilingData定义
<!--
支持不同的kernel使用不同的TilingData。
-->

#### 2.2.3.2. Kernel划分

| Kernel名称                   | TilingKey                | 优先级 | 算子场景                                            |   |   |   |
|----------------------------|--------------------------|-----|-------------------------------------------------|---|---|---|
|  |   |   |    |   |   |   |


#### 2.2.3.3. kernel实现

##### 2.2.3.3.1. 通用实现

不同kernel，tiling和流水基本相同。

###### 2.2.3.3.1.1. tiling

**1）多核tiling**

a）切分轴

b）切分实现

**2）核内tiling**


###### 2.2.3.3.1.2. kernel

##### 2.2.3.3.3. 模板xxx

###### 2.2.3.3.3.1. tiling

###### 2.2.3.3.3.2. kernel


### 2.2.4. 高阶API


### 2.2.5. 端云兼容

1）IR兼容性
2）TilingData兼容性
3）TilingFunc兼容性
4）Kernel兼容性


## 2.3. 版本开发状态

| 模板名|CHS支持|NCH支持|GZH支持|
| --- | --- | --- | --- | --- |
|   |   |   |   |

 
# 3. 算子测试说明
## 3.1. 算子编译约束

so编译时长与size约束：https://clouddrive.huawei.com/p/3f7301d0677a3d51d8acd11dc255df90

## 3.2. STC设计

### 3.2.1. 测试因子


### 3.2.2. 测试用例

测试用例excel归档链接:
1）excel 1：ntg可导入直接生成用例
2）excel 2：每个用例的测试记录，测试了几轮，每轮遇到什么问题
