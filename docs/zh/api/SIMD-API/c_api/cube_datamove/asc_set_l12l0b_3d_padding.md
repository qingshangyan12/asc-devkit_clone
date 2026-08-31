# asc_set_l12l0_padding_val

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

设置3D格式分形矩阵搬运（im2col展开）双源模式下第二个Feature Map在边界区域常数填充使用的填充值。配置写入后，作用于后续调用的3D格式分形矩阵搬运接口在双源模式（第二源）下从L1 Buffer搬入L0时，对L1 Buffer侧边界区域进行常数填充时使用的填充字节。本接口以`uint64_t`立即数方式传入，调用方需按硬件填充字段位宽约定自行拼装64 bit `config`的各字节区间。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

双源模式（第二源）下进行3D格式分形矩阵搬运时，第一/第二Feature Map属性需通过[asc_set_l13d_fmatrix](asc_set_l13d_fmatrix.md)、[asc_set_l13d_fmatrix_b](asc_set_l13d_fmatrix_b.md)预先配置，repeat参数需通过[asc_set_l13d_rpt](asc_set_l13d_rpt.md)、[asc_set_l3d_rpt_b](asc_set_l3d_rpt_b.md)预先配置，L1 Buffer边界值需通过[asc_set_l13d_size](asc_set_l13d_size.md)预先配置。上述配置完成后，再调用本接口设置第二源边界区域常数填充使用的填充值。

## 函数原型

```cpp
__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config)
```

## 参数说明

**表1**参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | 第二个Feature Map的填充值配置，64 bit立即数。调用方按后续3D格式分形矩阵搬运接口在双源模式下取用填充字段的位区间约定拼装各字节：<br>&nbsp;&nbsp;&bull; 按数据类型b32取用`config`的bit 0~31（低32 bit）作为填充值；<br>&nbsp;&nbsp;&bull; 按b16取用`config`的bit 0~15（低16 bit）作为填充值；<br>&nbsp;&nbsp;&bull; 按b8取用`config`的bit 0~7与bit 8~15两个填充字节，要求两字节相同，否则后续搬运触发异常；<br>&nbsp;&nbsp;&bull; 按b4取用`config`的bit 0~3、bit 4~7、bit 8~11、bit 12~15四个4 bit字段，要求四字段相同，否则后续搬运触发异常；<br>&nbsp;&nbsp;&bull; `config`的bit 32~33为填充模式控制位，bit 34~63在b32/b16/b8/b4取用时被忽略。<br>取值范围：[0, 2^64−1]，参数类型与64 bit字段位宽一致，无截断风险。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅设置填充值，实际填充由后续调用的3D格式分形矩阵搬运接口在双源模式下执行边界区域填充时取用本接口写入的配置完成，须先调用本接口写入配置，再调用对应搬运接口。
- 配置写入后对后续所有3D格式分形矩阵搬运接口在双源模式下的第二源填充行为生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同填充值，应在调用搬运接口前重新调用本接口设置。
- 本接口须与其他配套setter接口配合使用：先调用配置第一个与第二个Feature Map属性、repeat参数、L1 Buffer边界值的配套setter接口，再调用本接口配置第二源填充值，最后调用3D格式分形搬运接口执行搬运（配套setter名见功能说明章节）。

### 填充值取用约束

- 后续搬运按b8取用填充字节时要求`config`的bit 0~7与bit 8~15两个填充字节相同，否则后续搬运触发异常。
- 后续搬运按b4取用填充字节时要求`config`的bit 0~3、bit 4~7、bit 8~11、bit 12~15四个4bit字段相同，否则后续搬运触发异常。
- `config`的bit 32~33为填充模式控制位，本接口不限制其取值，由后续搬运接口按填充模式约定取用；bit 34~63在b32/b16/b8/b4取用时被忽略，调用方传入0即可。
- 本接口不自动做字节复制，调用方需自行保证传入的`config`在低16bit内按数据类型满足上述填充字节/字段相同约束，否则填充结果与预期不符（b8/b4取用场景下硬件会触发异常）。

## 调用示例
```cpp
uint64_t config = 0;
asc_set_l12l0_padding_val(config);
```