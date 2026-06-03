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

---

## 拆分中未保留的内容（因所属通路不在本次拆分范围内）

> 该文档原始仅支持 UB->GM 一个数据通路，拆分后内容基本完整保留，无实质性删除。以下为简化差异记录。

### 一、函数原型

| 原文档 | 拆分后 |
|:---|:---|
| 原文档仅有 LM->GM 通路一个函数原型 | ✅ 已完整保留 |

### 二、产品支持情况 — 产品表格（已简化）

| 原文档 | 拆分后的处理 |
|:---|:---|
| 原文档产品表格为 HTML 表格，共 9 个产品（部分 √、部分 x） | 拆分为简易 Markdown 表格 + cann-filter 标签，支持/不支持情况与原文档一致 |

### 三、数据类型

| 原文档 | 拆分后 |
|:---|:---|
| 原文档表 4 中包含 UB->GM 通路各产品的数据类型 | ✅ 已完整保留（950、AI Core、A2、A3 四个产品） |

### 四、参数说明

| 原文档 | 拆分后 |
|:---|:---|
| 模板参数（表 1）、接口参数（表 2）、Nz2NdParamsFull 结构体（表 3） | ✅ 已完整保留 |
| NZ2ND 转换示意图（half + float 两种数据类型） | ✅ 已完整保留 |

### 五、约束说明

| 原文档 | 拆分后 |
|:---|:---|
| 原文档无约束说明 | ✅ 一致 |

### 六、调用示例

| 原文档 | 拆分后 |
|:---|:---|
| LM->GM 通路示例 | ✅ 已完整保留 |

> **结论**：该文档为单通路（UB->GM），拆分过程中无内容丢失，仅做了格式简化（HTML→Markdown）和 cann-filter 标签添加。
