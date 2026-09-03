# asc_set_l12l0b_3d_padding

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持

## 功能说明

设置L1到L0B的3D搬运填充值。

## 函数原型

```cpp
__aicore__ inline void asc_set_l12l0b_3d_padding(uint64_t config)
__aicore__ inline void asc_set_l12l0b_3d_padding(int8_t padding_value)
__aicore__ inline void asc_set_l12l0b_3d_padding(uint8_t padding_value)
__aicore__ inline void asc_set_l12l0b_3d_padding(int16_t padding_value)
__aicore__ inline void asc_set_l12l0b_3d_padding(uint16_t padding_value)
__aicore__ inline void asc_set_l12l0b_3d_padding(half padding_value)
__aicore__ inline void asc_set_l12l0b_3d_padding(bfloat16_t padding_value)
__aicore__ inline void asc_set_l12l0b_3d_padding(int32_t padding_value)
__aicore__ inline void asc_set_l12l0b_3d_padding(uint32_t padding_value)
__aicore__ inline void asc_set_l12l0b_3d_padding(float padding_value)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| config | 输入 | 填充值寄存器配置。 |
| padding_value | 输入 | 填充值，数据类型由所选重载确定。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 在调用依赖该填充值的L1到L0B 3D搬运接口前完成配置。

## 调用示例

```cpp
asc_set_l12l0b_3d_padding(static_cast<half>(0));
```
