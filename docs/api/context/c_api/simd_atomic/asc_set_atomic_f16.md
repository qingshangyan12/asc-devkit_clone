# asc_set_atomic_f16

## 产品支持情况

|产品   | 是否支持 |
| ------------|:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

设置原子操作数据类型为half。

## 函数原型

```c++
__aicore__ inline void asc_set_atomic_f16()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需配合[asc_set_atomic_add](./asc_set_atomic_add.md)/[asc_set_atomic_max](./asc_set_atomic_max.md)/[asc_set_atomic_min](./asc_set_atomic_min.md)接口使用。
- 使用完成后，建议使用[asc_set_atomic_none](./asc_set_atomic_none.md)清空原子操作的状态，以免影响后续相关指令功能。

## 调用示例

```c++
//total_length指参与搬运的数据总长度, dst是外部输入的half类型的GM内存。
constexpr uint64_t total_length = 128;
__ubuf__ half src[total_length];
asc_set_atomic_min();
asc_set_atomic_f16();    // 设置原子操作类型为half
asc_copy_ub2gm(dst, src, total_length * sizeof(half));    // 将数据搬运到Global Memory
asc_set_atomic_none();    // 清空原子操作的状态
```