# asc_set_atomic_max_float

## 产品支持情况

|产品   | 是否支持 |
| :------------|:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

设置计算结果以原子比较的方式传输到GM。在拷贝前，将待传输的float数据与GM中已有数据进行逐元素比较，并将最大值写入GM。

## 函数原型

```cpp
__aicore__ inline void asc_set_atomic_max_float()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

使用结束后，建议通过[asc_set_atomic_none](./asc_set_atomic_none.md)关闭原子最大操作，以免影响后续相关指令功能。

## 调用示例

```cpp
// total_length指参与计算的数据长度，dst是外部输入的float类型的GM内存。
constexpr uint32_t total_length = 256;
__ubuf__ float src0[total_length];
__ubuf__ float src1[total_length];
asc_copy_ub2gm(dst, src0, total_length * sizeof(float));
asc_sync_pipe(PIPE_MTE3);
asc_set_atomic_max_float();
asc_copy_ub2gm(dst, src1, total_length * sizeof(float));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1.0f, 1.0f, 1.0f, ..., 1.0f]// float类型
输入数据src1：[2.0f, 2.0f, 2.0f, ..., 2.0f]// float类型
输出数据dst：[2.0f, 2.0f, 2.0f, ..., 2.0f]// float类型
```