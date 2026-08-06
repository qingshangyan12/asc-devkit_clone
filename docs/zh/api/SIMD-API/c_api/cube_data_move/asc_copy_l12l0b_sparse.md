# asc_copy_l12l0b_sparse

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：不支持
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

头文件路径：`"c_api/cube_datamove/cube_datamove.h"`。

用于搬运存放在L1 Buffer里的512B大小的稠密权重矩阵到L0B Buffer里，同时读取128B大小的索引矩阵用于稠密矩阵的稀疏化。

索引矩阵在一个int8_t的地址中的排布是逆序排布的，例如：索引矩阵1 2 0 1 0 2 1 0，在地址中的排布为1 0 2 1 0 1 2 0，其中1 0 2 1（对应索引矩阵前四位1 2 0 1）拼成一个int8_t数据，0 1 2 0（对应索引矩阵后四位0 2 1 0）拼成一个int8_t数据。

<!-- npu="950" id8 -->
- Ascend 950PR/Ascend 950DT：不提供本接口。L1 Buffer到L0B Buffer的数据搬运由[asc_copy_l12l0b](asc_copy_l12l0b/asc_copy_l12l0b.md)系列接口承担。
- 如需实现稠密权重矩阵稀疏化加载，应先在L1 Buffer中准备索引后的权重数据，再通过[asc_copy_l12l0b](asc_copy_l12l0b/asc_copy_l12l0b.md)系列接口搬运至L0B Buffer。
<!-- end id8 -->

## 函数原型

### 固定类型原型

```cpp
__aicore__ inline void asc_copy_l12l0b_sparse(__cb__ int8_t* dst, __cbuf__ int8_t* src,
                                              __cbuf__ int8_t* index, uint16_t start_index,
                                              uint8_t repeat)
__aicore__ inline void asc_copy_l12l0b_sparse_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src,
                                                   __cbuf__ int8_t* index, uint16_t start_index,
                                                   uint8_t repeat)
```

### dtype支持的数据类型

dst、src和index的dtype仅支持`int8_t`。

### 典型示例

```cpp
// 示例：int8_t类型的稀疏搬运。
__aicore__ inline void asc_copy_l12l0b_sparse(__cb__ int8_t* dst, __cbuf__ int8_t* src,
                                              __cbuf__ int8_t* index, uint16_t start_index,
                                              uint8_t repeat)
```

## 参数说明

**表1** 参数说明
| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的操作数在L0B Buffer的起始地址。 |
| src | 输入 | 源操作数在L1 Buffer的起始地址。 |
| index | 输入 | 索引矩阵在L1 Buffer的起始地址。 |
| start_index | 输入 | 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位：512B。 |
| repeat | 输入 | 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_description_and_constraints.md#通用地址重叠约束)。
- repeat取值范围为[0, 255]，为0时不执行搬运，本接口被视为NOP（空操作）。
- start_index不能小于0。
- 仅支持L1 Buffer->L0B Buffer通路，且L1 Buffer中的源数据为ZN排布。
- 不支持转置和跳stride，仅支持连续的数据分形搬运。

<!-- npu="950" id9 -->
### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- 本接口在本芯片变体下不提供，调用无实际效果。如需在本变体下将L1 Buffer中的权重数据加载到L0B Buffer，应按功能说明章节所述的替代方式使用。
<!-- end id9 -->

## 调用示例

```cpp
__cb__ int8_t dst[256];
__cbuf__ int8_t src[256];
__cbuf__ int8_t index[64];
uint16_t start_index = 1;
uint8_t repeat = 8;
asc_copy_l12l0b_sparse(dst, src, index, start_index, repeat);
```
