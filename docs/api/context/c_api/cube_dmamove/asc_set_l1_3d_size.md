# asc_set_l1_3d_size

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

设置Load3D时A1/B1边界值。

## 函数原型

```c++
__aicore__ inline void asc_set_l1_3d_size(uint64_t value)
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
asc_set_l1_3d_size(value);
```
