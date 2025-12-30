# asc_set_hf32_trans2even

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

设置HF32模式取整方式, FP32以最接近偶数的方式四舍五入为HF32，需要先使用asc_set_hf32_mode开启HF32取整模式。


## 函数原型

 ```cpp
   __aicore__ inline void asc_set_hf32_trans2even()
```

## 参数说明

无


## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
asc_set_hf32_trans2even();
```