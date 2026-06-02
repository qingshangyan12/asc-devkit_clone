# GMToUB连续数据搬运 与 UBToGM连续数据搬运 内容重复性分析报告

## 对比文档

| 项目 | GMToUB | UBToGM |
|:---|:---|:---|
| 文件 | `GMToUB连续数据搬运(DataCopy).md` | `UBToGM连续数据搬运(DataCopy).md` |
| 数据通路方向 | GM → UB | UB → GM |

## 逐章节对比

### 1. 产品支持情况

| 产品 | GMToUB | UBToGM | 是否一致 |
|:---|:---:|:---:|:---:|
| Ascend 950PR/Ascend 950DT | √ | √ | ✅ |
| Atlas A3 训练/推理 | √ | √ | ✅ |
| Atlas A2 训练/推理 | √ | √ | ✅ |
| Atlas 200I/500 A2 推理产品 | √ | √ | ✅ |
| Atlas 推理系列产品AI Core | √ | √ | ✅ |
| Atlas 推理系列产品Vector Core | √ | √ | ✅ |
| Atlas 训练系列产品 | √ | √ | ✅ |
| Kirin X90 | √ | √ | ✅ |
| Kirin 9030 | √ | √ | ✅ |

**结论：完全相同，所有 9 个产品均支持。**

---

### 2. 功能说明

| 文档 | 内容 |
|:---|:---|
| GMToUB | 提供连续数据搬运能力，数据在传输过程中保持原始格式和内容不变。 |
| UBToGM | 提供连续数据搬运能力，数据在传输过程中保持原始格式和内容不变。 |

**结论：完全相同。**

---

### 3. 函数原型

| 文档 | 原型 | 差异 |
|:---|:---|:---|
| GMToUB | `DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const uint32_t count)` | dst=LocalTensor, src=GlobalTensor |
| UBToGM | `DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)` | dst=GlobalTensor, src=LocalTensor |

**结论：仅 dst 和 src 的 Tensor 类型互换（方向相反），其余签名完全一致。**

---

### 4. 参数说明

#### 4.1 模板参数（表 1）

| 参数 | GMToUB | UBToGM | 是否一致 |
|:---|:---|:---|:---:|
| T | 操作数的数据类型。支持的数据类型请参考[数据类型]。 | 操作数的数据类型。支持的数据类型请参考[数据类型]。 | ✅ |

#### 4.2 接口参数（表 2）

| 参数 | GMToUB | UBToGM | 差异 |
|:---|:---|:---|:---|
| **dst** | 输出，**LocalTensor**，32字节对齐 | 输出，**GlobalTensor**，按数据类型对齐 | 📌 类型和对齐要求不同 |
| **src** | 输入，**GlobalTensor**，按数据类型对齐 | 输入，**LocalTensor**，32字节对齐 | 📌 类型和对齐要求不同（互换） |
| **count** | 参与搬运的元素个数。count * sizeof(T)需32字节对齐。 | 参与搬运的元素个数。count * sizeof(T)需32字节对齐。 | ✅ |

**结论：dst 和 src 的角色互换（符合数据通路方向），count 完全相同。**

---

### 5. 数据类型

| 产品 | GMToUB | UBToGM | 是否一致 |
|:---|:---|:---|:---:|
| Ascend 950PR/Ascend 950DT | b8、b16、b32、b64 | b8、b16、b32、b64 | ✅ |
| Atlas 训练系列产品 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | ✅ |
| Atlas 推理系列产品AI Core | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | ✅ |
| Atlas 推理系列产品Vector Core | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | ✅ |
| Atlas A2 训练/推理 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double | ✅ |
| Atlas A3 训练/推理 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double | ✅ |
| Atlas 200I/500 A2 推理 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double | ✅ |
| Kirin X90 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | ✅ |
| Kirin 9030 | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double | ✅ |

**结论：所有 9 个产品的支持数据类型完全相同。**

---

### 6. 返回值说明

| GMToUB | UBToGM | 是否一致 |
|:---|:---|:---:|
| 无 | 无 | ✅ |

---

### 7. 约束说明

| 约束项 | GMToUB | UBToGM | 差异 |
|:---|:---|:---|:---|
| PipeBarrier 同步指令 | ✅ PipeBarrier<PIPE_MTE3> + PipeBarrier<PIPE_MTE2> | ✅ PipeBarrier<PIPE_MTE3> | 📌 GMToUB 多了 MTE2（搬入流水同步） |
| A2/A3 跨卡通信 | ✅ | ✅ | ✅ |

**结论：核心约束（跨卡通信）相同。GMToUB 多了一条 MTE2 搬入流水同步的说明，因为 GM→UB 方向涉及搬入 UB。这个差异是由通路方向决定的，合理。**

---

### 8. 调用示例

| 项目 | GMToUB | UBToGM |
|:---|:---|:---|
| 示例代码 | `DataCopy(srcLocal, srcGlobal, 512)` | `DataCopy(dstGlobal, dstLocal, 512)` |
| 结果示例 | 相同 | 相同 |

**结论：仅 dst/src 变量角色互换。**

---

## 综合结论

```
┌─────────────────────────────────────────────────────────────┐
│  两篇文档除数据通路方向相反导致的以下必要差异外，           │
│  其余内容完全相同：                                         │
│                                                             │
│  1. 标题中方向标识（GM->UB vs UB->GM）                      │
│  2. 函数原型中 dst/src 类型互换                             │
│  3. 参数说明中 dst/src 类型及对齐要求互换                   │
│  4. 约束说明中多一条 MTE2 搬入同步（方向导致）              │
│  5. 调用示例中变量角色互换                                  │
│                                                             │
│  完全相同的内容：                                           │
│  1. 产品支持情况（9个完全一致）                             │
│  2. 功能说明（完全一致）                                    │
│  3. 模板参数 T 的描述（完全一致）                           │
│  4. count 参数说明（完全一致）                              │
│  5. 数据类型（9个产品的数据类型完全一致）                   │
│  6. 返回值说明（完全一致）                                  │
│  7. 跨卡通信约束（完全一致）                                │
│  8. 调用示例的结果输出（完全一致）                          │
└─────────────────────────────────────────────────────────────┘
```

| 对比维度 | 重复性判定 |
|:---|:---:|
| 产品支持情况 | 🔴 **完全重复** |
| 功能说明 | 🔴 **完全重复** |
| 参数说明（除 dst/src 类型外） | 🔴 **完全重复** |
| 数据类型 | 🔴 **完全重复** |
| 约束说明 | 🟡 **高度重复**（仅 MTE2 差异） |
| 返回值说明 | 🔴 **完全重复** |

> **建议**：如果未来需要维护这两个文档，当某个产品的数据类型或产品支持情况发生变化时，两个文档需同步更新，存在遗漏风险。可以考虑将公共部分（产品支持、数据类型）抽取为引用或合并发布时自动生成两个方向版本。
