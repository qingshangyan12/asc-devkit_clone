# asc_enable_hif8

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|<cann-filter npu_type = "950"> Ascend 950PR/Ascend 950DT | √ </cann-filter>|

## 功能说明

用于设置Mmad计算开启HF32模式，开启该模式后L0A Buffer/L0B Buffer中的8bit数据将在参与矩阵乘法运算前被转化为hifloat8_t类型数据。


## 函数原型


 ```cpp
__aicore__ inline void asc_enable_hif8()
```


## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
asc_enable_hif8();
```
