# asc_set_atomic_s16

## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

设置原子操作数据类型为int16_t。

## 函数原型

```c++
__aicore__ inline void asc_set_atomic_s16()
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

```c++
asc_set_atomic_s16();
```