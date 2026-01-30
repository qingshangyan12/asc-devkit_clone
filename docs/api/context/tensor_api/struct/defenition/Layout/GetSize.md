# GetSize

## 产品支持情况

|产品|是否支持|
|--|:-:|
|Ascend 950PR/Ascend 950DT|√|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|x|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|√|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Atlas 200/300/500 推理产品|x|

## 功能说明

获取当前Layout 所占的内存大小。

## 函数原型

```
__aicore__ inline uint32_t GetSize() const
```

## 参数说明

无

## 返回值说明

当前Layout 所占的内存大小。单位为元素。

## 约束说明

无

## 调用示例

```
AscendC::Shape<int,int,int> shape = AscendC::MakeShape(10, 20, 30);
AscendC::Stride<int,int,int> stride = AscendC::MakeStride(1, 100, 200);
auto layout = AscendC::MakeLayout(shape, stride);
auto capicity = layout.GetSize(); // capicity = 12
```
