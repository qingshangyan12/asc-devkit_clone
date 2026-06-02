# 拆分日志：随路转换NZ2ND搬运

源文档：`docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/随路转换NZ2ND搬运（只支持UB2GM）.md`
接口：DataCopy

## 全部数据通路

| 数据通路（逻辑） | 数据通路（物理） | 是否在本次拆分范围内 | 拆分后文档 |
|:---|:---|:---:|:---|
| VECOUT -> GM | UB -> GM | ✅ 是 | `UBToGM随路转换NZ2ND搬运(DataCopy).md` |
| CO2 -> GM | UB -> GM | ✅ 是 | `UBToGM随路转换NZ2ND搬运(DataCopy).md` |

## 各通路支持的产品和数据类型

### 通路：UB -> GM（VECOUT -> GM / CO2 -> GM）

| 产品型号 | cann-filter | 支持的数据类型 |
|:---|:---:|:---|
| Ascend 950PR/Ascend 950DT | 950 | bool、int8_t、uint8_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、fp8_e8m0_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64 |
| Atlas 推理系列产品AI Core | 310p | int16_t、uint16_t、int32_t、uint32_t、half、float |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | 910b | int16_t、uint16_t、int32_t、uint32_t、half、bfloat16_t、float |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | A3 | int16_t、uint16_t、int32_t、uint32_t、half、bfloat16_t、float |
