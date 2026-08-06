# asc_set_l13d_fmatrix_b

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

设置Feature map属性描述，用于在调用[asc_copy_l12l0a](./asc_copy_l12l0a/asc_copy_l12l0a.md)/[asc_copy_l12l0b](./asc_copy_l12l0b/asc_copy_l12l0b.md)的3D格式搬运接口时配置填充数值。
仅当asc_copy_l12l0a/asc_copy_l12l0b接口的f_matrix_ctrl参数指示从右矩阵获取FeatureMap的属性时使用本接口设置Feature map属性描述，否则使用asc_set_l13d_fmatrix接口。

## 函数原型

```cpp
__aicore__ inline void asc_set_l13d_fmatrix_b(asc_l13d_fmatrix_config& config)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| config | 输入 | 用于设置asc_copy_l12l0a/asc_copy_l12l0b的3D格式搬运接口的Feature map属性参数，详细说明请参考[asc_l13d_fmatrix_config](../struct/asc_l13d_fmatrix_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

<!-- npu="950" id8 -->
### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅设置第二个Feature Map属性描述，实际搬运由后续调用的3D格式分形矩阵搬运接口执行，须先调用本接口写入配置，再调用对应搬运接口。
- 配置写入后对后续所有从右矩阵获取第二个Feature Map属性的3D格式分形矩阵搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同Feature Map属性，应在调用搬运接口前重新调用本接口设置。
- 本接口须与其他配套setter接口配合使用：先调用配置第一个Feature Map属性与本接口配置第二个Feature Map属性的配套setter接口，再配置repeat参数、padding填充值与L1 Buffer边界值的配套setter接口，最后调用3D格式分形搬运接口执行搬运（配套setter名见功能说明章节）。
- 结构体字段位宽与C类型完全匹配（`uint16_t`字段16bit、`uint8_t`字段8bit），用户字段值在C类型范围内即可被硬件完整接收，无截断风险。

<!-- end id8 -->

## 调用示例

```cpp
asc_l13d_fmatrix_config config;
config.l1_height = 1;
config.l1_width = 2;
config.padding_left_size = 3;
config.padding_right_size = 4;
config.padding_top_size = 5;
config.padding_bottom_size = 6;
asc_set_l13d_fmatrix_b(config);
```
