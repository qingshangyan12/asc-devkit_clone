# asc_get_datablock_size

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √     |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √     |

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