# asc_set_atomic_s32

## 昇腾产品支持情况

|昇腾产品   | 是否支持 |
| ------------|:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

设置原子操作的数据类型为int32_t类型。

## 函数原型

```c++
__aicore__ inline void asc_set_atomic_s32()
```

## 参数说明

无
## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

使用完成后，建议清空原子操作的状态（详见[asc_set_atomic_none](./asc_set_atomic_none.md)），以免影响后续相关指令功能。

## 调用示例

```c++
// 本演示示例使用asc_copy_ub2gm从ubuf搬出到外部gm时进行原子最小操作，并调用asc_set_atomic_none修改原子最小的数据类型。
//total_length指参与搬运的数据总长度。dst是外部输入的int32_t类型的gm内存。
constexpr uint64_t offset = 0;
constexpr uint64_t total_length = 256;
__ubuf__ int32_t src[total_length];
asc_set_atomic_min()
asc_set_atomic_s32();
asc_copy_ub2gm(dst, src, total_length * sizeof(half));
asc_set_atomic_none();
```

```
每个核的输入数据为：
src0: [1, 1, 1, 1, 1,...,1] // 256个1
src1: [2, 2, 2, 2, 2,...,2] // 256个2
最终输出数据: [1, 1, 1, 1, 1,...,1] // 256个1
```
