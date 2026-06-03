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

---

## 拆分中未保留的内容（因所属通路不在本次拆分范围内）

> 以下内容存在于原始文档中，但在拆分后的 1 个文档（`GMToUB随路转换ND2NZ搬运(DataCopy).md`）中**已被删除或简化**。请人工确认是否需要补回。

### 一、函数原型（已删除）

| 原文档内容 | 涉及通路 | 删除原因 |
|:---|:---|:---|
| LM->LM 通路函数原型：`DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const Nd2NzParams&)` | UB -> L1 Buffer（VECIN/VECCALC/VECOUT -> TSCM） | 不在范围 |

### 二、参数说明 — 表 2 接口参数（已简化）

| 参数 | 原文档中的额外内容 | 涉及物理位置 | 拆分后是否保留 |
|:---|:---|:---|:---:|
| **dst** | 原文档未区分，统一为 LocalTensor | — | ✅ 一致 |
| **src** | 原文档为 "LocalTensor **或 GlobalTensor**" | — | ❌ 简化为仅 GlobalTensor（GM->UB 通路） |

### 三、参数说明 — 表 1 模板参数（已简化）

| 参数 | 原文档中的额外内容 | 涉及产品 | 拆分后是否保留 |
|:---|:---|:---|:---:|
| **enableSmallC0** | SmallC0 模式开关，仅 Ascend 950PR/Ascend 950DT 支持 | 950 | ✅ 保留（用 cann-filter 标签包裹） |

### 四、产品支持情况 — 产品表格（已简化）

| 原文档 | 拆分后的处理 |
|:---|:---|
| 原文档产品表格有 **3 列**（产品 / GM->LM 是否支持 / LM->LM 是否支持） | 拆分后简化为 **单列**（"是否支持"），LM->LM 列被移除 |
| 原文档 Kirin X90、Kirin 9030 仅 GM->LM（实际为 GM->L1 Buffer）为 √ | 拆分 GM->UB 文档中 Kirin X90/9030 标记为 x（因其不涉及 UB 通路） |

### 五、数据类型（已过滤）

| 原文档内容 | 涉及通路/产品 | 拆分后是否保留 |
|:---|:---|:---:|
| 表 4 中 **GM -> A1、B1（GM -> L1 Buffer）** 通路的所有产品数据类型（950/AI Core/A2/A3/Kirin X90/Kirin 9030） | GM -> L1 Buffer | ❌ 已删除 |
| 表 5 中 **VECIN/VECCALC/VECOUT -> TSCM（UB -> L1 Buffer）** 通路的所有产品数据类型（950/A2/A3） | UB -> L1 Buffer | ❌ 已删除 |
| 950 DT 的 GM->L1 通路支持 fp4x2_e2m1_t、fp4x2_e1m2_t（GM->UB 通路不支持） | GM -> L1 Buffer / 950 | ❌ 已删除 |
| AI Core 的 GM->L1 通路类型（int16_t、uint16_t、int32_t、uint32_t、half、float）与 GM->UB 通路类型不同 | GM -> L1 Buffer / 310p | ❌ 已删除（注意 AI Core 的 GM->UB 和 GM->L1 支持类型不完全相同） |
| Kirin X90 / Kirin 9030 仅支持 GM->L1 通路 | GM -> L1 Buffer | ❌ 已删除（这两个产品在 GM->UB 拆分文档中标记为 x） |

### 六、约束说明（已保留）

| 原文档内容 | 涉及产品/通路 | 拆分后是否保留 |
|:---|:---|:---:|
| "针对 Atlas 推理系列产品AI Core，使用 Global Memory -> Local Memory 通路的 ND2NZ 搬运接口时，需要预留 8K 的 UB 空间" | AI Core / GM->UB | ✅ 保留 |

### 七、调用示例（已简化）

| 原文档内容 | 涉及通路 | 拆分后是否保留 |
|:---|:---|:---|
| 原文档调用示例包含 GM->LM 和 LM->LM 两组调用 | — | ❌ 拆分后只保留 GM->LM 示例 |
| `DataCopy(srcLocal, workLocal, intriParams)` — LM->LM 示例 | UB -> L1 Buffer | ❌ 已删除 |

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
