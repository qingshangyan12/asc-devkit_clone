# GMToUB非对齐数据搬运 与 UBToGM非对齐数据搬运 内容重复性分析报告

## 对比文档

| 项目 | GMToUB | UBToGM |
|:---|:---|:---|
| 文件 | `GMToUB非对齐数据搬运(DataCopyPad).md` | `UBToGM非对齐数据搬运(DataCopyPad).md` |
| 数据通路方向 | GM → UB | UB → GM |

## 逐章节对比

### 1. 产品支持情况

| 产品 | GMToUB | UBToGM | 是否一致 |
|:---|:---:|:---:|:---:|
| Ascend 950PR/Ascend 950DT | √ | √ | ✅ |
| Atlas A3 训练/推理 | √ | √ | ✅ |
| Atlas A2 训练/推理 | √ | √ | ✅ |
| Atlas 200I/500 A2 推理产品 | √ | √ | ✅ |
| Atlas 推理系列产品AI Core | x | x | ✅ |
| Atlas 推理系列产品Vector Core | x | x | ✅ |
| Atlas 训练系列产品 | x | x | ✅ |

**结论：完全相同。**

---

### 2. 功能说明

| 文档 | 内容 |
|:---|:---|
| GMToUB | 该接口提供数据非对齐搬运的功能，**从Global Memory搬运数据至Local Memory时，可以根据开发者的需要自行填充数据**。 |
| UBToGM | 该接口提供数据非对齐搬运的功能，**从Local Memory搬运数据至Global Memory时，支持非对齐搬运**。 |

**结论：📌 不同**。GMToUB 强调"可以自行填充数据"，UBToGM 强调"支持非对齐搬运"。

---

### 3. 函数原型

| 文档 | 原型数量 | padParams 参数 | mode 参数 |
|:---|:---:|:---:|:---:|
| GMToUB | **8 个**（DataCopyExtParams 4 个 + DataCopyParams 4 个） | ✅ 有 | ✅ 有 |
| UBToGM | **4 个**（仅 DataCopyExtParams 和 DataCopyParams 无 padParams 版本） | ❌ 无 | ✅ 有 |

**结论：📌 显著不同**。GM->UB 方向有 padParams（填充参数），UB->GM 方向没有。函数数量也不同（8 vs 4）。

---

### 4. 参数说明

#### 4.1 模板参数（表 2）

| 参数 | GMToUB | UBToGM | 差异 |
|:---|:---|:---|:---|
| **T** | 操作数以及**paddingValue（待填充数据值）**的数据类型 | 操作数的数据类型 | 📌 GMToUB 多了 paddingValue 说明 |
| **mode** | PaddingMode 定义 + 仅950支持 | 相同 | ✅ |

#### 4.2 接口参数（表 3）

| 参数 | GMToUB | UBToGM | 差异 |
|:---|:---|:---|:---|
| dst | LocalTensor，32B对齐 | GlobalTensor，无对齐约束 | 📌 方向导致 |
| src | GlobalTensor，无对齐约束 | LocalTensor，32B对齐 | 📌 方向导致 |
| dataCopyParams | DataCopyExtParams/DataCopyParams | 相同 | ✅ |
| **padParams** | ✅ 有（DataCopyPadExtParams/DataCopyPadParams） | ❌ **无** | 📌 **关键差异** |
| nd2nzParams | ❌ 无 | ❌ 无 | ✅ |

#### 4.3 DataCopyExtParams（表 4）

| 参数 | GMToUB | UBToGM | 差异 |
|:---|:---|:---|:---|
| blockCount | 相同 | 相同 | ✅ |
| blockLen | 相同 | 相同 | ✅ |
| srcStride | GM→UB：源为GM，**单位字节**；950取值范围[-blockLen, 2^40-1] | UB→GM：源为VECIN/VECOUT，**单位dataBlock**；950取值范围[0, 65535] | 📌 **方向导致单位不同** |
| dstStride | GM→UB：目的为VECIN/VECOUT，**单位dataBlock**；950取值范围[0, 65535] | UB→GM：目的为GM，**单位字节**；950取值范围[0, 2^40-1] | 📌 **方向导致单位不同** |
| rsv | 相同 | 相同 | ✅ |

#### 4.4 DataCopyParams（表 5）

| 参数 | GMToUB | UBToGM | 差异 |
|:---|:---|:---|:---|
| blockCount | 相同 | 相同 | ✅ |
| blockLen | 相同 | 相同 | ✅ |
| srcStride | 源为GM，单位字节 | 源为VECIN/VECOUT，单位dataBlock | 📌 **方向导致单位不同** |
| dstStride | 目的为VECIN/VECOUT，单位dataBlock | 目的为GM，单位字节 | 📌 **方向导致单位不同** |

#### 4.5 填充参数（表 6、表 7）

| 表 | GMToUB | UBToGM |
|:---|:---:|:---:|
| 表 6 DataCopyPadExtParams | ✅ 有（isPad/leftPadding/rightPadding/paddingValue） | ❌ 无 |
| 表 7 DataCopyPadParams | ✅ 有 | ❌ 无 |

**结论：📌 填充参数表仅 GMToUB 有。**

---

### 5. 数据类型

| 产品 | GMToUB | UBToGM | 是否一致 |
|:---|:---|:---|:---:|
| Ascend 950PR/Ascend 950DT | bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64 | 相同 | ✅ |
| Atlas A3 训练/推理 | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double | 相同 | ✅ |
| Atlas A2 训练/推理 | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double | 相同 | ✅ |
| Atlas 200I/500 A2 推理 | int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float | 相同 | ✅ |

**结论：完全相同。**

---

### 6. 配置示例

| 示例 | GMToUB | UBToGM | 差异 |
|:---|:---:|:---:|:---:|
| **GM->VECIN/VECOUT** | ✅（3个配置示例 + 参数解释） | ❌ | 📌 **仅 GMToUB 有** |
| **VECIN/VECOUT->GM** | ❌ | ✅（2个配置示例） | 📌 **仅 UBToGM 有** |
| **搬运模式（Normal/Compact）** | ✅（950专属） | ❌ | 📌 **仅 GMToUB 有** |
| 示例图片 | datacopypad1/2/pad.png + paddingMode.png | datacopypad3/4.png | 📌 完全不同 |

**结论：📌 配置示例完全不同，各文档只含自身方向的示例，且 GMToUB 多了 padding 模式和搬运模式示例。**

---

### 7. 返回值说明

| GMToUB | UBToGM | 是否一致 |
|:---|:---|:---:|
| 无 | 无 | ✅ |

---

### 8. 约束说明

| 约束项 | GMToUB | UBToGM |
|:---|:---|:---|
| leftPadding、rightPadding ≤ 32字节 | ✅ | ❌ 无 |

**结论：📌 仅 GMToUB 有 leftPadding/rightPadding 约束。**

---

### 9. 调用示例

| 项目 | GMToUB | UBToGM |
|:---|:---|:---|
| 代码 | `DataCopyPad(dstGlobal, src0Global, copyParams, padParams)` | `DataCopyPad(dstGlobal, srcLocal, copyParams)` |
| padParams | ✅ 有 | ❌ 无 |

**结论：📌 调用代码不同，GMToUB 有 padParams 参数，UBToGM 没有。**

---

## 综合结论

```
┌─────────────────────────────────────────────────────────────┐
│  ⚠️ 这两篇文档的差异远超"连续数据搬运"和"高维切分数据搬运"  │
│    的两个方向版本。DataCopyPad 的两个方向存在实质性差异：    │
│                                                             │
│  🔴 不可合并的差异：                                        │
│  1. GM->UB 有 padParams（填充参数）— 4张结构体表，4个函数   │
│     UB->GM 无 padParams                                      │
│  2. 函数原型数量不同（8 vs 4）                               │
│  3. 配置示例完全不同（GMToUB 5个示例 vs UBToGM 2个示例）    │
│  4. 功能说明不同（填充 vs 非对齐）                           │
│  5. 约束说明不同（leftPadding 约束仅 GMToUB 有）             │
│  6. 调用示例不同（padParams 有无）                           │
│  7. srcStride/dstStride 单位不同（字节 vs dataBlock）        │
│                                                             │
│  🟢 可合并的内容（完全一致）：                               │
│  1. 产品支持情况（7个产品完全一致）                          │
│  2. 数据类型（4个产品的数据类型完全一致）                    │
│  3. mode 模板参数（PaddingMode 定义一致）                    │
│  4. 返回值说明                                              │
│  5. INSTALL_DIR 路径参考                                     │
└─────────────────────────────────────────────────────────────┘
```

| 对比维度 | 重复性判定 |
|:---|:---:|
| 产品支持情况 | 🔴 **完全重复** |
| 功能说明 | 🟢 **不完全重复**（方向导致语义不同） |
| 函数原型 | 🔵 **不可合并**（padParams 有无） |
| 参数说明（除 dst/src 类型外） | 🟡 **部分重复**（srcStride/dstStride 单位不同） |
| 填充参数表（6/7） | 🔵 **不可合并**（仅 GMToUB 有） |
| 数据类型 | 🔴 **完全重复** |
| 配置示例 | 🔵 **不可合并**（完全不同） |
| 约束说明 | 🔵 **不可合并**（仅 GMToUB 有） |
| 调用示例 | 🔵 **不可合并**（padParams 有无） |
| 返回值说明 | 🔴 **完全重复** |

> **最终判定：❌ 不建议合并。** DataCopyPad 的两个方向存在实质性差异，GM->UB 方向有填充（padding）能力而 UB->GM 方向没有，导致函数原型、参数表、配置示例、约束、调用示例均有显著差别。强行合并会造成文档臃肿且逻辑混乱。建议保持独立。
