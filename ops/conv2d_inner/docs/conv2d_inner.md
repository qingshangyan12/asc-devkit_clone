# 修订记录
| 日期       | 修订版本 | 修改章节            | 修改描述 | 作者         |
| ---------- | -------- | ------------------- | -------- | ------------ |
| 2026-08-01 |    V1.0    |         ALL         |     初稿     | zengjuan 00945165 |

---hi-lake begin---


# 1. 算子需求与规格说明

## 1.1. 功能说明

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002235218694.gif)

上图是单通道图像卷积的例子，而在实际网络推理中，我们一般处理是多个通道的图像，典型的例子像是网络输入是RGB三通道图像，神经网络推理的中间层结果也是一个多通道的状态（通常有几十上百个通道）。
这里涉及到神经网络计算中，不同深度图层的推理过程。即，如何由一个深度为n的图层转换为深度为m的图层。这里的图层数目在神经网络计算中我们一般用通道数来表达。在两个卷积级联的场景，上层卷积的输出通道数，也就是当前层卷积的输入通道数。

网络中filter的维度表征为NCHW，其中C是输入特征图的通道数目，N是filter中包含的独立的kernel数目。
更多的计算细节参考《Vincent Dumoulin, Francesco Visin - A guide to convolution arithmetic for deep learning》，网页https://github.com/vdumoulin/conv_arithmetic使用动画呈现计算过程，也可参考。
一种将convolution计算等效为矩阵乘法的方法叫img2col，全称image to column，它的核心思路是将featuremap对应filter的每次滑动的元素抽取出来并拉直：

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002235058866.png)

img2col之后，和filter相乘：

![alt text](https://resource.idp.huawei.com/idpresource/nasshare/editor/image/203511894006/1_zh-cn_image_0000002270138109.png)

这种计算方法是诸多深度学习计算框架的核心方法，Davinci对于卷积计算的处理也基于此方法将convolution转换成矩阵乘法，进一步进行分块，在Cube中完成计算。


## 1.2. 算子IR定义

### 1.2.1. CANN IR

算子基于如下CANN IR实现，IR路径：vendor/hisi/npu/api/framework/graph/cann/experiment_ops.h

```
REG_OP(Conv2DV2)
    .INPUT(x, TensorType({DT_FLOAT16, DT_FLOAT, DT_BF16, DT_HIFLOAT8}))
    .INPUT(filter, TensorType({DT_FLOAT16, DT_FLOAT, DT_BF16, DT_HIFLOAT8}))
    .OPTIONAL_INPUT(bias, TensorType({DT_FLOAT16, DT_FLOAT, DT_BF16}))
    .OPTIONAL_INPUT(offset_w, TensorType({DT_INT8}))
    .OUTPUT(y, TensorType({DT_FLOAT16, DT_FLOAT, DT_BF16, DT_HIFLOAT8}))
    .REQUIRED_ATTR(strides, ListInt)
    .ATTR(pads, ListInt, {0, 0, 0, 0})
    .ATTR(dilations, ListInt, {1, 1, 1, 1})
    .ATTR(groups, Int, 1)
    .ATTR(data_format, String, "NCHW")
    .ATTR(offset_x, Int, 0)
    .ATTR(pad_mode, String, "SPECIFIC")
    .ATTR(enable_hf32, Bool, false)
    .OP_END_FACTORY_REG(Conv2DV2)
```

### 1.2.1. FE IR

算子基于如下FE IR实现，IR路径：vendor/hisi/npu/src/cls/npucl/nn_core/fusion_engine/inc/fe_ir/convolution/conv_2d_inner.h

```
HIAI_REG_OP(Conv2DInner)
    .HIAI_INPUT(x, TensorType({ DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN }))
    .HIAI_INPUT(filter, TensorType({ DT_FLOAT16, DT_INT8, DT_FLOAT8_E4M3FN, DT_INT4 }))
    .HIAI_OPTIONAL_INPUT(bias, TensorType({ DT_FLOAT16, DT_INT32 }))
    .HIAI_OPTIONAL_INPUT(offset_w, TensorType({ DT_INT8 }))
    .HIAI_OPTIONAL_INPUT(pre_quant, TensorType({ DT_UINT64 }))
    .HIAI_OPTIONAL_INPUT(pre_relu, TensorType({ DT_UINT32 }))
    .HIAI_OPTIONAL_INPUT(pre_clip_relu, TensorType({ DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN }))
    .HIAI_OPTIONAL_INPUT(eltwise, TensorType({ DT_FLOAT16, DT_INT32, DT_INT16, DT_INT8 }))
    .HIAI_OPTIONAL_INPUT(anti_quant, TensorType({ DT_UINT64, DT_UINT32, DT_UINT16 }))
    .HIAI_OPTIONAL_INPUT(post_quant, TensorType({ DT_UINT64, DT_UINT16 }))
    .HIAI_OPTIONAL_INPUT(post_relu, TensorType({ DT_UINT32 }))
    .HIAI_OPTIONAL_INPUT(post_clip_relu, TensorType({ DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN }))
    .HIAI_OPTIONAL_INPUT(dynamic_aipp, TensorType({ DT_UNDEFINED }))
    .HIAI_OPTIONAL_INPUT(padding, TensorType({DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN}))
    .HIAI_OUTPUT(y, TensorType({ DT_FLOAT16, DT_INT16, DT_INT8, DT_FLOAT8_E4M3FN }))
    .HIAI_REQUIRED_ATTR(strides, AttrValue::LIST_INT)
    .HIAI_ATTR(pads, AttrValue::LIST_INT ({ 0, 0, 0, 0 }))
    .HIAI_ATTR(dilations, AttrValue::LIST_INT ({ 1, 1 }))
    .HIAI_ATTR(groups, AttrValue::INT { 1 })
    .HIAI_ATTR(data_format, AttrValue::STR { "NCHW" })
    .HIAI_ATTR(offset_x, AttrValue::INT { 0 })
    .HIAI_ATTR(round_mode, AttrValue::STR { "rint" })
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
```
template<int8_t FmapTiling=2, int8_t WeightTiling=2,
         int8_t L1PingPong=2, int8_t L0PingPong=2,
         int8_t OutputOrder=1, int8_t IterOrder=0,
         int Bl1ByPass=0, int8_t GroupType=0>
extern "C" __global__ __aicore__ void conv2_d_inner(GM_ADDR x,
                                                    GM_ADDR filter,
                                                    GM_ADDR bias,
                                                    GM_ADDR offsetW, // not support
                                                    GM_ADDR preQuant,
                                                    GM_ADDR preRelu,
                                                    GM_ADDR preClipRelu,
                                                    GM_ADDR eltwise,
                                                    GM_ADDR antiQuant,
                                                    GM_ADDR postQuant,
                                                    GM_ADDR postRelu,
                                                    GM_ADDR postClipRelu,
                                                    GM_ADDR padding,
                                                    GM_ADDR dynamicAipp, // not support now
                                                    GM_ADDR workspace,
                                                    GM_ADDR tiling,
                                                    GM_ADDR y)
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
| offsetW          | offsetW输入GM地址               |
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
