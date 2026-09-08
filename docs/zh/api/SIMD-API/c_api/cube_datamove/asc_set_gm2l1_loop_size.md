# asc_set_gm2l1_loop_size

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

将数据从Global Memory（GM）搬运到L1 Buffer时，设置[asc_copy_gm2l1_align](asc_copy_gm2l1_align.md)循环填充模式的内层和外层循环次数。

需与[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)和[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)配合使用。

以源操作数搬运场景为例，如下图所示。

![源操作数搬运场景示例](../figures/source_operand_move_example.png)

本接口仅在AIC上生效。

## 函数原型

```cpp
__aicore__ inline void asc_set_gm2l1_loop_size(uint64_t loop1_size, uint64_t loop2_size)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| loop1_size | 输入 | 内层循环的循环次数，取值范围为[0, $2^{21}-1$]。 |
| loop2_size | 输入 | 外层循环的循环次数，取值范围为[0, $2^{23}-1$]。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口在非AIC上调用直接返回。
- 配置仅由[asc_copy_gm2l1_align](asc_copy_gm2l1_align.md)循环填充模式取用，循环填充模式不支持左右填充。

## 调用示例

将代码保存为`examples.asc`后，可通过`bisheng`命令编译，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[`__NPU_ARCH__`](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译命令如下：

```bash
bisheng examples.asc -o main --npu-arch=dav-3510
```
<!-- end id8 -->

```cpp
asc_set_gm2l1_loop_size(2, 2);
asc_set_gm2l1_loop1_stride(96, 128);
asc_set_gm2l1_loop2_stride(192, 288);
constexpr uint32_t burst_count = 2;
constexpr uint32_t burst_len = 2;
constexpr uint8_t left_padding_count = 0;
constexpr uint8_t right_padding_count = 0;
constexpr bool enable_data_select = false;
constexpr asc_load_l2_cache_mode l2_cache_mode = asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM;
constexpr uint64_t src_stride = 0;
constexpr uint32_t dst_stride = 0;
// src表示源操作数的起始地址
__cbuf__ half dst[256];
asc_copy_gm2l1_align(dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
    l2_cache_mode, src_stride, dst_stride);
asc_set_gm2l1_loop_size(1, 1);
```
