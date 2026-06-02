# 拆分日志：DataCopyPad（非对齐数据搬运）

源文档：`docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md`
接口：DataCopyPad

> **说明**：该文档没有"支持的通路和数据类型"章节，数据通路从**表1**的"支持的数据传输通路"列提取，数据类型从**数据类型**章节获取。

## 全部数据通路

| 数据通路（逻辑） | 数据通路（物理） | 是否在本次拆分范围内 | 拆分后文档 |
|:---|:---|:---:|:---|
| GM->VECIN/VECOUT | **GM -> UB** | ✅ 是 | `GMToUB非对齐数据搬运(DataCopyPad).md` |
| VECIN/VECOUT->GM | **UB -> GM** | ✅ 是 | `UBToGM非对齐数据搬运(DataCopyPad).md` |
| GM->A1/B1 | GM -> L1 Buffer | ❌ 否 | — |
| GM->TSCM | GM -> L1 Buffer | ❌ 否 | — |
| A1/B1->GM | L1 Buffer -> GM | ❌ 否 | — |
| VECIN/VECOUT->TSCM | **UB -> L1 Buffer** | ❌ 否 | — |
| GM->VECCALC | GM -> UB（纳入GM->UB） | ✅ 已合并 | — |

## 各通路支持的产品和数据类型

### 通路：GM -> UB（GM->VECIN/VECOUT / GM->VECCALC）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | A3 | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | 910b | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double |
| Atlas 200I/500 A2 推理产品 | 310b | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float |

### 通路：UB -> GM（VECIN/VECOUT->GM）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | A3 | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | 910b | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double |
| Atlas 200I/500 A2 推理产品 | 310b | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float |

### 通路：GM -> L1 Buffer（GM->A1/B1、GM->TSCM）（不在本次拆分范围）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64 |

### 通路：L1 Buffer -> GM（A1/B1->GM）（不在本次拆分范围）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64 |

### 通路：UB -> L1 Buffer（VECIN/VECOUT->TSCM）（不在本次拆分范围）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | A3 | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | 910b | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double |

> 注：GM->L1 Buffer、L1 Buffer->GM、UB->L1 Buffer 通路的数据类型参考了 GM->UB 通路的数据类型（原文档数据类型章节未按通路区分）。
