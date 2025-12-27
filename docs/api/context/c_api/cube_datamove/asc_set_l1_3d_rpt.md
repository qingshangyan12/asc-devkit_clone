# asc_set_l1_3d_rpt

## 产品支持情况

|产品   | 是否支持 |
| ------------|:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

用于设置Load3D的repeat参数。

## 函数原型

```c++
__aicore__ inline void asc_set_l1_3d_rpt(uint64_t value)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| value | 输入 | 源操作数（标量）。 |

## 返回值说明

无

## 流水类型

PIPE_TYPE_S

## 约束说明

无

## 调用示例

```c++
uint64_t value = 1;
asc_set_l1_3d_rpt(value); // 设置Load3D的repeat值为1
```