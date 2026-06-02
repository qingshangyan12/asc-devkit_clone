# 拆分日志：随路转换ND2NZ搬运

源文档：`docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/随路转换ND2NZ搬运（除了GM2UB，其余通路归希如）.md`
接口：DataCopy

## 全部数据通路

| 数据通路（逻辑） | 数据通路（物理） | 是否在本次拆分范围内 | 拆分后文档 |
|:---|:---|:---:|:---|
| GM -> VECIN | **GM -> UB** | ✅ 是 | `GMToUB随路转换ND2NZ搬运(DataCopy).md` |
| GM -> A1、B1 | **GM -> L1 Buffer** | ❌ 否（不涉及UB） | — |
| VECIN、VECCALC、VECOUT -> TSCM | **UB -> L1 Buffer** | ❌ 否（不涉及UB<->GM或UB<->UB） | — |

## 各通路支持的产品和数据类型

### 通路：GM -> UB（GM -> VECIN）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、fp8_e8m0_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32 |
| Atlas 推理系列产品AI Core | 310p | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、half、float |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | 910b | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、half、bfloat16_t、float |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | A3 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、half、bfloat16_t、float |

### 通路：GM -> L1 Buffer（GM -> A1、B1）（不在本次拆分范围）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、fp4x2_e2m1_t、fp4x2_e1m2_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、fp8_e8m0_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32 |
| Atlas 推理系列产品AI Core | 310p | int16_t、uint16_t、int32_t、uint32_t、half、float |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | 910b | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、half、bfloat16_t、float |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | A3 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、half、bfloat16_t、float |
| Kirin X90 | x90 | half、int8_t、uint8_t、int16_t、uint16_t |
| Kirin 9030 | 9030 | half、int8_t、uint8_t、int16_t、uint16_t |

### 通路：UB -> L1 Buffer（VECIN/VECCALC/VECOUT -> TSCM）（不在本次拆分范围）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、fp8_e8m0_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | 910b | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、half、bfloat16_t、float |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | A3 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、half、bfloat16_t、float |
