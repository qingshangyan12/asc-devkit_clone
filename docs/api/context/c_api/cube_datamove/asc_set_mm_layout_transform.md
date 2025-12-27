# asc_set_mm_layout_transform

## 产品支持情况

|产品   | 是否支持 |
| ------------|:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

设置mmad计算时优先通过M/N中的哪个方向，M为矩阵的行，N为矩阵的列。

## 函数原型

```c++
//先通过N方向，然后通过M方向产生结果
__aicore__ inline void asc_set_mm_layout_transform_m()
//先通过M方向，然后通过N方向产生结果
__aicore__ inline void asc_set_mm_layout_transform_n()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_TYPE_S

## 约束说明

无

## 调用示例

无
