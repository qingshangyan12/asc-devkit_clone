# asc_set_l0c_copy_lrelu_alpha

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持

## 功能说明

设置L0C搬出随路Leaky ReLU预处理的alpha值。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c_copy_lrelu_alpha(float scalar_relu_pre_alpha)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| scalar_relu_pre_alpha | 输入 | Leaky ReLU预处理使用的alpha值。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 在调用启用Leaky ReLU随路预处理的L0C搬出接口前完成配置。

## 调用示例

```cpp
asc_set_l0c_copy_lrelu_alpha(0.1F);
```
