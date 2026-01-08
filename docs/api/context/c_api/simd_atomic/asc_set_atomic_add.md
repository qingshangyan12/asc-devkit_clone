# asc_set_atomic_add

## 昇腾产品支持情况

|昇腾产品   | 是否支持 |
| ------------|:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

调用该接口后，可对后续的从Unified Buffer/L0C Buffer/L1 Buffer到Global Memory的数据传输开启原子累加。累加的数据类型可通过[asc_set_atomic_f16](asc_set_atomic_f16.md)，[asc_set_atomic_bf16](asc_set_atomic_bf16.md)，[asc_set_atomic_f32](asc_set_atomic_f32.md)，[asc_set_atomic_s8](asc_set_atomic_s8.md)，[asc_set_atomic_s16](asc_set_atomic_s16.md)，[asc_set_atomic_s32](asc_set_atomic_s32.md)等API进行设置。

## 函数原型

```c++
__aicore__ inline void asc_set_atomic_add()
```

## 参数说明

无
## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 使用完成后，建议清空原子操作的状态（详见[asc_set_atomic_none](./asc_set_atomic_none.md)），以免影响后续相关指令功能。
- 该指令执行前不会对GM的数据做清零操作，开发者需在需要时手动添加清零操作。

## 调用示例

```c++
//total_length指参与搬运的数据总长度。
constexpr uint32_t total_length = 256;
__ubuf__ half src0[total_length];
__ubuf__ half src1[total_length];
__gm__ half dst[total_length];
asc_set_atomic_add();
asc_set_atomic_f16();
asc_copy_ub2gm_sync(dst, src0, total_length * sizeof(half));
asc_copy_ub2gm_sync(dst, src1, total_length * sizeof(half));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1 1 1 ... 1]
输入数据src1：[2 2 2 ... 2]
输出数据dst：[3 3 3 ... 3]
```
