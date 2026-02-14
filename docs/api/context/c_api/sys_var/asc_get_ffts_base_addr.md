# asc_get_ffts_base_addr

## 产品支持情况

| 产品 | 是否支持 |
| :-----------| :------: |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √     |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √     |

## 功能说明

获取核间同步寄存器的基地址。

## 函数原型

```cpp
__aicore__ inline int64_t asc_get_ffts_base_addr()
```

## 参数说明

无

## 返回值说明

核间同步寄存器的基地址。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
int64_t ffts_base = asc_get_ffts_base_addr();
```
