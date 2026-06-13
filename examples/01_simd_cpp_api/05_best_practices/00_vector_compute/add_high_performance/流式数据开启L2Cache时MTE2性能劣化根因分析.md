# 流式数据开启 L2 Cache 时 MTE2 性能劣化根因分析

## 背景

在 Ascend AI Core 中，MTE2（Memory Transfer Engine 2）负责将数据从 Global Memory（GM/HBM）搬运到 Unified Buffer（UB）。当执行 `DataCopy` 操作时，L2 Cache 的开启与否对搬运性能有显著影响。

以 Add 算子为例（数据只读一次，无复用），实测数据对比如下：

| 指标 | L2 Cache 开启（Case 4） | L2 Cache Bypass（Case 5） | 变化 |
|:---|:---:|:---:|:---:|
| 端到端耗时 | 263.91μs | **187.68μs** | **↓28.9%** |
| MTE2 耗时 | 250.53μs | **176.00μs** | **↓29.8%** |
| 向量计算耗时 | 12.85μs | 12.85μs | 不变 |

MTE2 耗时增加了约 **30%**，本文分析其根本原因。

---

## 1. L2 Cache 开启时的数据流

### 1.1 Cache Miss 时的典型流程

```
MTE2 发起 GM → UB 的 DataCopy 请求（读地址 A）
        │
        ▼
① L2 Cache Tag 查询
   ── 判断地址 A 是否在 L2 Cache 中
        │
        ▼（Cache Miss）
② 从 HBM 读取数据
   ── 消耗 HBM 读带宽，读取 N 字节
        │
        ▼
③ 数据到达 L2 Cache 控制器
        │
        ├──→ 写入 L2 Cache（填充 Cache Line，粒度 64B/128B）
        │      └─ 这是开启 L2 Cache 才有的额外操作
        │
        └──→ 直接转发到 UB（直通路径，无需等 Cache Line 填满）
```

**关键结论**：数据不是"先到 L2 Cache，再从 L2 Cache 读到 UB"的两跳串行路径。数据流经 L2 Cache 控制器时，**并行地**完成两件事：
- 写入 L2 Cache 填充缓存行
- 转发到目标 UB

### 1.2 Cache Hit 时的流程（仅当数据被重复访问时）

```
MTE2 发起读请求
        │
        ▼
① L2 Cache Tag 查询 → **命中**
        │
        ▼
② 直接从 L2 Cache 读取数据到 UB
   ── 无需访问 HBM，延迟更低、带宽更高
```

这就是 L2 Cache 的收益来源——**数据复用**。但对于 Add 算子的输入 x、y，每个元素只读一次，永远不会发生 Cache Hit。

---

## 2. L2 Cache Bypass 时的数据流

```
MTE2 发起 GM → UB 的 DataCopy 请求
        │
        ▼
① 直接进行地址转换，跳过 L2 Cache 检查
        │
        ▼
② 从 HBM 读取数据
   ── 消耗 HBM 读带宽
        │
        ▼
③ 数据直接送达 UB
   ── 不经过 L2 Cache 控制器
   ── 不写入 L2 Cache
```

---

## 3. 性能劣化根因分析

### 3.1 核心根因：额外的 L2 Cache 写操作

开启 L2 Cache 后，MTE2 在搬运数据到 UB 的**同时**，还需要将同样的数据写入 L2 Cache（Cache Line 填充）。这个额外的写操作消耗了 L2 Cache 控制器的内部数据总线带宽。

```
L2 Cache 开启时：
  HBM 读带宽  = N 字节（给 UB）           ← 必要开销
  Cache 写带宽 = N 字节（写 L2 Cache）     ← 额外开销，流式场景纯浪费
  总有效开销  ≈ HBM 读 N + Cache 写 N

L2 Cache Bypass 时：
  HBM 读带宽  = N 字节（给 UB）           ← 唯一开销
  Cache 写带宽 = 0                         ← 无额外开销
  总有效开销  ≈ HBM 读 N
```

### 3.2 Cache Line 填充的开销（次要因素）

L2 Cache 以 Cache Line（通常 64B）为粒度管理数据。即使上层只需要 2 字节，Cache Miss 时也必须将整个 64B 的 Cache Line 从 HBM 读入并填充到 L2 Cache。

不过在本样例中，`DataCopy` 每次搬运 **32768 字节**，远大于 Cache Line 粒度，所以这部分开销相对于写操作本身来说占比不大。

### 3.3 Cache 污染（系统级影响）

流式数据写入 L2 Cache 后，会：
- 占据宝贵的 L2 Cache 空间
- 可能逐出其他热点数据（如卷积权重、后续可复用的特征图）
- 逐出时还需将脏数据写回 HBM，进一步消耗带宽

这是从**全局视角**看的负面效应，特别是在多算子流水线场景中。

---

## 4. 一个常见的误解

> ❌ 开启 L2 Cache 后，数据路径是：HBM → L2 Cache（完整 Cache Line）→ 再从 L2 Cache 读到 UB，多了一跳所以变慢。

这个理解是错误的。硬件实际实现是**数据在 L2 Cache 控制器处并行转发**，并非串行两跳。如果真是串行两跳，劣化幅度会远大于 30%。

正确的理解：

| 对比项 | 理解是否正确 | 说明 |
|:---|:---:|:---|
| "数据先从 HBM 到 L2，再从 L2 到 UB" | ❌ 错误 | 实际是 L2 控制器并行转发到 UB 和 L2 Cache |
| "多了一次 HBM 读取" | ❌ 错误 | HBM 始终只读一次，不论 L2 Cache 是否开启 |
| "多了一次 L2 Cache 写操作" | ✅ **正确** | 这是开销的本质来源 |
| "Cache Line 填充粒度导致浪费" | ✅ 部分正确 | 对大数据块搬运影响有限，对小粒度访问影响更大 |

---

## 5. 结论

对于**只读一次的流式数据**，L2 Cache 开启导致 MTE2 性能劣化的根本原因是：

> **MTE2 在将数据搬运到 UB 的同时，还必须将数据写入 L2 Cache 进行 Cache Line 填充。这个额外的写操作消耗了 L2 Cache 控制器的内部带宽，导致搬运吞吐下降约 30%。由于数据没有复用，写入 L2 Cache 的数据永远不会被命中，这笔开销是纯浪费的。**

优化方法：对只读一次的数据调用 `SetL2CacheHint(CACHE_MODE_DISABLE)` 禁用 L2 Cache，让 MTE2 直通 UB，同时避免 L2 Cache 污染。

---

## 参考文档

- [设置合理的L2 CacheMode](../../../../../../docs/guide/算子实践参考/SIMD算子性能优化/内存访问/设置合理的L2-CacheMode.md)
- [系统缓存概述](../../../../../../docs/api/SIMD-API/基础API/缓存控制/系统缓存概述.md)
- [Add性能调优样例 README](./README.md)
