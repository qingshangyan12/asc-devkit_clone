# asc_enable_fp8

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

本接口用于启用矩阵乘加（Mmad）计算的FP8模式。调用该接口会关闭HiF8模式，其作用与[asc_enable_hif8](./asc_enable_hif8.md)相反，两个接口不同时生效。启用FP8模式后，当矩阵乘加指令的左矩阵A和右矩阵B均以`fp8_e4m3fn_t`作为输入数据类型时，L0A Buffer和L0B Buffer中的数据在参与矩阵乘法运算前不会转换为`hifloat8_t`，而是直接以`fp8_e4m3fn_t`类型参与计算。

## 函数原型

```c
__aicore__ inline void asc_enable_fp8()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口需在矩阵乘加指令（[asc_mmad](./asc_mmad.md)、[asc_mmad_mx](./asc_mmad_mx.md)）执行前调用，确保模式配置在矩阵乘加结果生成前生效。
- FP8模式启用后会持续生效，不会自动关闭。后续矩阵乘加指令若不显式重新配置，将沿用当前模式。如需切换为HiF8模式，请重新调用[asc_enable_hif8](./asc_enable_hif8.md)接口。
- 本接口仅对矩阵乘加输入数据类型为`fp8_e4m3fn_t`×`fp8_e4m3fn_t`的场景生效，其他FP8数据类型组合（`fp8_e4m3fn_t`×`fp8_e5m2_t`、`fp8_e5m2_t`×`fp8_e4m3fn_t`、`fp8_e5m2_t`×`fp8_e5m2_t`）不支持FP8与HiF8模式选择，调用本接口不产生实际作用。

## 调用示例

```cpp
constexpr uint64_t total_length = 128;
__cc__ float c_matrix[total_length];
__ca__ fp8_e4m3fn_t a_matrix[total_length];
__cb__ fp8_e4m3fn_t b_matrix[total_length];

uint16_t left_height = 16;  // M
uint16_t n_dim = 16;        // K
uint16_t right_width = 16;  // N

// 在 asc_mmad 前调用：以 FP8 格式参与矩阵运算（非 HiF8 转换路径）
asc_enable_fp8();

uint8_t unit_flag = 0;
bool disable_gemv = false;
bool c_matrix_source = false;
bool c_matrix_init_val = true;
asc_mmad_sync(c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width,
              unit_flag, disable_gemv, c_matrix_source, c_matrix_init_val);
```
