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

---

## 拆分中未保留的内容（因所属通路不在本次拆分范围内）

> 以下内容存在于原始文档中，但在拆分后的 2 个文档中**已被删除或简化**。请人工确认是否需要补回。

### 一、函数原型（已删除）

| 原文档内容 | 涉及通路 | 删除原因 |
|:---|:---|:---|
| `DataCopyPad(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams&, const Nd2NzParams&)` — LM->LM 通路（实际 VECIN/VECOUT->GM->TSCM） | UB -> L1 Buffer | 不在范围 |
| `DataCopyPad(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams&, const Nd2NzParams&)` — LM->LM 通路 | UB -> L1 Buffer | 不在范围 |
| 以上原型共 2 个，均带 `nd2nzParams` 参数用于 ND->NZ 格式转换 | UB -> L1 Buffer | 不在范围 |

### 二、参数说明 — 表 3 接口参数（已简化）

| 参数 | 原文档中的额外内容 | 涉及物理位置 | 拆分后是否保留 |
|:---|:---|:---|:---:|
| **dst** | "类型为 LocalTensor 或 GlobalTensor" — 原文档 dst 描述含两种类型 | — | ❌ 简化为单通路对应类型 |
| **src** | "类型为 LocalTensor 或 GlobalTensor" — 原文档 src 描述含两种类型 | — | ❌ 简化为单通路对应类型 |
| **nd2nzParams** | "从 VECIN/VECOUT->TSCM 进行数据搬运时，可以进行 ND 到 NZ 的数据格式转换" | UB -> L1 Buffer | ❌ 已删除 |

### 三、配置示例（已删除）

| 原文档内容 | 涉及通路 | 拆分后是否保留 |
|:---|:---|:---:|
| VECIN/VECOUT->TSCM 配置示例（含图 1 搬运示意图、Nd2NzParams 参数解析） | UB -> L1 Buffer | ❌ 已删除 |

### 四、约束说明（已简化）

| 原文档内容 | 涉及产品/通路 | 拆分后是否保留 |
|:---|:---|:---:|
| "针对 Ascend 950PR/Ascend 950DT，VECIN/VECOUT->TSCM 通路，仅支持如下数据类型：half、bfloat16_t、int16_t、uint16_t、float、int32_t、uint32_t、int8_t、uint8_t、int64_t、uint64_t、double" | 950 DT / UB->L1 Buffer | ❌ 已删除 |

### 五、数据类型（已过滤）

| 原文档信息 | 涉及通路 | 拆分后是否保留 |
|:---|:---|:---:|
| 原文档数据类型章节未按通路区分，所有产品合并列出 | — | ✅ 已完整保留（GM->UB 和 UB->GM 通路的类型完全相同） |

### 六、调用示例（已简化）

| 原文档内容 | 涉及通路 | 拆分后是否保留 |
|:---|:---|:---:|
| 原文档示例覆盖 GM->VECIN 场景 | GM -> UB | ✅ 保留于 GMToUB 文档 |
| 原文档无独立的 UB->GM 调用示例 | UB -> GM | 📌 拆分时为 UBToGM 文档从原示例推导生成 |
