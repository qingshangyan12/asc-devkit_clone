# asc_set_atomic_bf16

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

设置原子操作数据类型为bfloat16_t。

## 函数原型

```c++
__aicore__ inline void asc_set_atomic_bf16()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需要和asc_set_atomic_add/[asc_set_atomic_max](./asc_set_atomic_max.md)/[asc_set_atomic_min](./asc_set_atomic_min.md)接口配合使用。
- 使用完成后，建议使用asc_set_atomic_none清空原子操作的状态，以免影响后续相关指令功能。

## 调用示例

```c++
// total_length指参与搬运的数据总长度, dst是外部输入的half类型的GM内存。
constexpr uint64_t total_length = 128;
__ubuf__ half src[total_length * sizeof(half)];
asc_copy_ub2gm(dst, src, total_length * sizeof(half));    // 将数据搬运到Global Memory
asc_sync_mte3(0);
asc_set_atomic_min();
asc_set_atomic_bf16();    // 设置原子操作类型为bfloat16_t
```
