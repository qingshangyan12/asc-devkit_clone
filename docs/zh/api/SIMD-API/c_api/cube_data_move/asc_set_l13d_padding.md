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

设置Pad属性描述，用于在调用[asc_copy_l12l0a](asc_copy_l12l0a/asc_copy_l12l0a.md)接口时配置填充数值。

## 函数原型

```cpp
__aicore__ inline void asc_set_l13d_padding(uint64_t config)
__aicore__ inline void asc_set_l13d_padding(half config)
__aicore__ inline void asc_set_l13d_padding(int16_t config)
__aicore__ inline void asc_set_l13d_padding(uint16_t config)
```

## 参数说明

**表1** 参数说明

|参数名|输入/输出|描述|
|------------|------------|-----------|
| config | 输入     | Pad填充值的数值。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

<!-- npu="950" id8 -->
### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅设置填充值，实际填充由后续调用的3D格式分形矩阵搬运接口在执行边界区域填充时取用PADDING字段完成，须先调用本接口写入配置，再调用对应搬运接口。
- 配置写入后对后续所有3D格式分形矩阵搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同填充值，应在调用搬运接口前重新调用本接口设置。
- 本接口须与其他配套setter接口配合使用：先调用配置Feature Map属性、repeat参数、L1 Buffer边界值的配套setter接口，再调用本接口配置填充值，最后调用3D格式分形搬运接口执行搬运（配套setter名见功能说明章节）。

### 填充值取用约束

- PADDING字段按后续搬运指令的数据类型取用不同位区间：按b32取用`[31:0]`，按b16取用`[15:0]`，按b8取用`[15:8]`与`[7:0]`（要求两字节相同），按b4取用`[15:12]`、`[11:8]`、`[7:4]`、`[3:0]`（要求4个4bit字段相同）；`[31:16]`在b16/b8/b4取用时被忽略，`[33:32]`为卷积填充控制模式位。
- `uint64_t`重载仅写入PADDING字段的bit 0~15（低8bit复制到两个字节），不会写入bit 16及以上的高位；若后续搬运按b32取用PADDING[31:0]，仅低16bit为本接口写入的填充值，高16bit保持为0。
- `half`/`int16_t`/`uint16_t`重载按原始比特位写入PADDING字段bit 0~15；后续搬运按b8取用时要求`[15:8]`与`[7:0]`相同，调用方需保证传入的16bit值两个字节相同，否则填充结果与预期不符（硬件不报错）。
- 后续搬运按b4取用时要求PADDING字段低16bit的4个4bit字段相同；本接口不自动做此复制，调用方需自行保证传入值满足该约束，否则填充结果与预期不符（硬件不报错）。

### 重载选型约束

- 传入一个8bit填充字节时建议使用`uint64_t`重载，实现自动复制到`[15:8]`与`[7:0]`，满足b8取用要求。
- 传入一个16bit数据元素（如`half`浮点数、`int16_t`/`uint16_t`整数）作为b16填充值时使用对应类型重载，按原始比特位写入低16bit。
- 同一填充值若需同时支持b8与b16取用，应优先使用`uint16_t`重载并传入两个字节相同的值（如0x0505），可同时满足b8（两字节相同）与b16（低16bit为填充值）取用约束。

<!-- end id8 -->

## 调用示例

```cpp
constexpr uint64_t config = 0;
asc_set_l13d_padding(config);
```
