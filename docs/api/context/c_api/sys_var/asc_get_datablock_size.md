# asc_get_datablock_size

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :----------------------- | :------: |
| <term>Ascend 910C</term> |    √     |
| <term>Ascend 910B</term> |    √     |

## 功能说明

获取当前芯片版本一个DataBlock的大小，单位为byte。

## 函数原型

```cpp
__aicore__ inline uint16_t asc_get_datablock_size()
```

## 参数说明

无

## 返回值说明

当前芯片版本一个DataBlock的大小，单位为byte。

## 流水类型

PIPE_TYPE_S

## 约束说明

无

## 调用示例

无