# Quant Group Matmul 样例深度解析

## 一、什么是 Quant Group Matmul

### 1.1 基本概念

Quant Group Matmul（量化分组矩阵乘法）是一种**分组量化矩阵乘与激活函数融合计算**的算子，广泛应用于大语言模型（LLM）的推理场景。其核心思想是：

- **分组（Grouped）**：将左矩阵 `x` 按 M 轴（行方向）分成 `g` 组，每组使用不同的右矩阵 `weight[i]` 进行矩阵乘法计算
- **量化（Quantized）**：输入矩阵使用低精度 int8 数据类型进行矩阵乘法，以减少计算量和内存带宽消耗
- **反量化（Dequant）**：矩阵乘法完成后，使用 `scale`（per-channel）和 `perTokenScale`（per-token）两级量化参数将 int32 结果反量化为浮点数
- **融合（Fusion）**：将矩阵乘法、反量化、激活函数（Gelu）融合为一个算子，减少中间结果的访存开销

### 1.2 计算公式

```python
offset = 0
for i in range(g):
    mmOut = x[offset:offset + group[i]] * weight[i] + bias[i]          # Cube 矩阵乘（int8 → int32）
    y[offset:offset + group[i]] = Gelu(mmOut * scale[i] * pertokenScale[offset:offset + group[i]])  # Vector 反量化+激活
    offset += group[i]
```

其中：
- `x`：左矩阵，形状 `[m, k]`，int8 类型
- `weight`：右矩阵（分组），形状 `[g, k, n]`，int8 类型
- `bias`：偏置，形状 `[g, n]`，int32 类型
- `group`：每组 M 维度大小，int64 类型
- `scale`：右矩阵 per-channel 量化参数，形状 `[g, n]`，float 类型
- `pertokenScale`：左矩阵 per-token 量化参数，形状 `[m]`，float 类型
- `y`：输出，形状 `[m, n]`，float16 类型

### 1.3 量化模型说明

本样例采用**W8A8 量化方案**（Weight 8-bit, Activation 8-bit），即：

| 参数 | 含义 | 维度 |
|------|------|------|
| `scale[i]` (per-channel) | 右矩阵（权重）的量化参数，每列一个 scale | `[g, n]` |
| `pertokenScale` (per-token) | 左矩阵（激活）的量化参数，每行一个 scale | `[m]` |

反量化过程为：
```
float_result = int32_mmOut * scale[i] * pertokenScale[offset:offset + group[i]]
```

- **per-channel 反量化**：矩阵乘结果每列乘以对应的 scale 值，由 `AscendDequant` 接口完成
- **per-token 反量化**：反量化后再逐行乘以 perTokenScale，由 `Mul` + `BroadCast` 完成

### 1.4 应用场景

Quant Group Matmul 在大模型推理中的典型应用包括：

- **MoE（Mixture of Experts）模型**：不同 token 路由到不同 expert，每个 expert 对应一组独立的权重矩阵，分组轴为 token 数（M轴）
- **多批次推理**：不同批次使用不同权重进行推理
- **GQA/MQA 注意力机制**：不同 head group 使用不同的 KV 权重

---
