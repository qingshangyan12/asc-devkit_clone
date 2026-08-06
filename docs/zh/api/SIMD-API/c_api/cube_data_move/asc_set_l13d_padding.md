# asc_set_l13d_padding

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

设置3D格式分形矩阵搬运（im2col展开）以及相关卷积/矩阵搬运指令使用的填充值。配置写入PADDING后，作用于后续调用的3D格式分形矩阵搬运接口在L1 Buffer侧对边界区域进行常数填充时使用的填充字节。本接口提供4个重载，按调用方传入的C类型与硬件填充值字段的对应关系分为两类：

- **`int16_t`、`uint16_t`、`half`重载**：用于直接以一个16 bit数据元素传入填充值。实现内部将其按原始比特位写入PADDING字段的低16 bit，作为b16填充值使用；当后续搬运按b32取用PADDING[31:0] 时，仅低16 bit生效，高16 bit保持为0。
- **`uint64_t`重载**：用于以立即数方式按8 bit填充值传入。传入值的低8 bit作为填充字节，实现内部将其复制到PADDING字段的两个字节位区间，满足硬件对b8填充值要求两个字节相同的约束。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

3D格式分形矩阵搬运（im2col展开）场景下，填充值需通过本接口配置；Feature Map属性需通过[asc_set_l13d_fmatrix](asc_set_l13d_fmatrix.md)（左矩阵）与[asc_set_l13d_fmatrix_b](asc_set_l13d_fmatrix_b.md)（双源模式右矩阵）配置，repeat控制参数需通过[asc_set_l13d_rpt](asc_set_l13d_rpt.md)配置，L1 Buffer边界值需通过[asc_set_l13d_size](asc_set_l13d_size.md)配置，各setter需配合使用后再调用3D格式分形矩阵搬运接口执行搬运。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_set_l13d_padding(<dtype> config)
```

### dtype支持的数据类型

dtype支持的数据类型为`int16_t`、`uint16_t`、`half`、`uint64_t`。

### 典型示例

```cpp
// 示例：配置half类型的padding值。
__aicore__ inline void asc_set_l13d_padding(half config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | 填充值，写入PADDING后供后续3D格式分形矩阵搬运接口在边界区域填充使用。按重载类型分两种语义：<br>&nbsp;&nbsp;&bull; `int16_t`、`uint16_t`、`half`重载：传入一个16 bit数据元素作为填充值，实现按原始比特位写入PADDING字段的bit 0~15；后续搬运按b16取用PADDING[15:0]，按b8取用时要求`[15:8]`与`[7:0]`相同，调用方需保证传入的16 bit值两个字节相同（如对b8填充0x05，应传入`uint16_t`值0x0505）；<br>&nbsp;&nbsp;&bull; `uint64_t`重载：传入值的低8 bit作为填充字节，实现将其复制到PADDING字段的bit 0~7与bit 8~15两个字节位区间，满足硬件对b8填充值要求`[15:8]`与`[7:0]`相同的约束；用户传入一个8 bit填充值即可，传入 > 2^8−1的值高位被忽略，仅低8 bit生效。<br>取值范围：`int16_t`、`uint16_t`、`half`重载为各类型的可表示范围（`int16_t`为 [−2^15, 2^15−1]，`uint16_t`为 [0, 2^16−1]，`half`为16 bit浮点可表示值集合）；`uint64_t`重载为 [0, 2^64−1]，仅低8 bit作为填充字节生效。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 填充值取用约束

- PADDING字段按后续搬运指令的数据类型取用不同位区间：按b32取用`[31:0]`，按b16取用`[15:0]`，按b8取用`[15:8]`与`[7:0]`（要求两个字节相同），按b4取用`[15:12]`、`[11:8]`、`[7:4]`、`[3:0]`（要求4个4bit字段相同）；`[31:16]`在b16、b8、b4取用时被忽略，`[33:32]`为卷积填充控制模式位。
- `uint64_t`重载仅写入PADDING字段的bit0~15，其中低8bit复制到两个字节，不写入bit16及以上高位。后续搬运按b32取用PADDING字段时，仅低16bit为本接口写入的填充值，高16bit保持为0。
- `int16_t`、`uint16_t`、`half`重载按原始比特位写入PADDING字段的bit0~15。后续搬运按b8取用时，调用方需保证`[15:8]`与`[7:0]`相同，否则填充结果与预期不符。
- 后续搬运按b4取用时，调用方需保证PADDING字段低16bit的4个4bit字段相同，本接口不会自动复制，否则填充结果与预期不符。

### 重载选型约束

- 传入16bit数据元素作为b16填充值时，使用`int16_t`、`uint16_t`或`half`重载，接口会按原始比特位写入低16bit。
- 传入8bit填充字节时建议使用`uint64_t`重载，接口会将填充值复制到`[15:8]`与`[7:0]`，满足b8取用要求。
- 同一填充值需同时支持b8与b16取用时，建议使用`uint16_t`重载并传入两个字节相同的值，例如0x0505。

## 调用示例

```cpp
constexpr uint64_t config = 0;
asc_set_l13d_padding(config);
```
