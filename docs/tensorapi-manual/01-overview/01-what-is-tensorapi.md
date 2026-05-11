# 1.1 TensorAPI 简介

TensorAPI 是 Ascend C 中面向高阶算子开发的核心编程抽象层。它提供了 **基于 Layout + Tensor 的统一编程范式**，使得开发者能够以直观、类型安全的方式操作 NPU 上的多维数据。

## 1.1.1 在 Ascend C 中的定位

Ascend C 提供了多层级的编程 API，从底层到高层依次为：

```
┌──────────────────────────────────────────────────┐
│                  高阶 API (adv_api)                │
│   预置算子：Matmul / LayerNorm / Softmax / ...     │
├──────────────────────────────────────────────────┤
│                TensorAPI (tensor_api)              │  ← 你在这里
│   Layout / Tensor / Copy / MMAD 统一抽象           │
├──────────────────────────────────────────────────┤
│              基础 API (basic_api)                  │
│   单指令抽象：Vec / Cube / 数据搬运 / 同步         │
├──────────────────────────────────────────────────┤
│          语言扩展层 C API (c_api)                   │
│   纯 C 接口，直接硬件指令                           │
└──────────────────────────────────────────────────┘
```

TensorAPI 位于**基础 API 之上，高阶 API 之下**，是编写高性能自定义算子的推荐入口。它与基础 API 的关键区别在于：

| 特性 | 基础 API | TensorAPI |
|------|---------|-----------|
| 编程范式 | 逐指令编程 | Layout + Tensor 数据驱动 |
| 数据描述 | 手动计算偏移 | Layout 自动管理坐标→地址映射 |
| 类型安全 | 弱（依赖 DataCopy 等通用接口） | 强（Layout Pattern 嵌入类型） |
| 学习曲线 | 陡峭 | 平缓 |

## 1.1.2 核心能力一览

TensorAPI 提供以下核心能力：

1. **构造抽象** — `MakeShape` / `MakeLayout` / `MakeTensor` 等工厂函数
2. **数据描述** — 通过 Layout 描述数据在内存中的排布方式（形状 + 步长）
3. **分形格式支持** — 原生支持 NPU 特有的 NZ / ND / DN / ZZ 等分形数据布局
4. **坐标访问** — 支持通过坐标 `[i,j]` 访问元素，自动处理地址映射
5. **子区域操作** — `operator()` / `Slice()` / `Compose()` / `Tile()` 等方法
6. **数据搬运** — Copy 算法族，覆盖 Cube 和 Vector 两条搬运通路
7. **矩阵计算** — MMAD 算法族，支持带 Bias 的矩阵乘加
8. **多级存储** — 统一管理 GM / L1 / UB / L0A / L0B / L0C 等存储层次

## 1.1.3 与其他 API 的关系

TensorAPI 不替代基础 API——在需要极致性能或进行底层控制时，仍可使用基础 API。但 TensorAPI 是更高层的抽象，它内部生成的基础 API 调用已经过架构适配和性能调优。

对于绝大多数算子开发场景，TensorAPI 是推荐选择。

## 1.1.4 代码示例：第一印象

以下是一个完整的 TensorAPI 程序片段，直观感受其编程风格：

```cpp
#include "include/tensor_api/tensor.h"
using namespace AscendC::Te;

// 1. 获取 NPU 内存指针
__gm__ float*   gm_in  = /* ... */;
__cbuf__ half*  l1_buf = /* ... */;

// 2. 创建内存指针（绑定 Location）
auto gmPtr = MakeMemPtr<Location::GM>(gm_in);
auto l1Ptr = MakeMemPtr<Location::L1>(l1_buf);

// 3. 创建分形 Layout（NZ 格式, half 类型: C0=16）
auto gmLayout = MakeFrameLayout<NDLayoutPtn>(rows, cols);
auto l1Layout = MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(rows, cols);

// 4. 创建 Tensor
auto gmTensor = MakeTensor(gmPtr, gmLayout);
auto l1Tensor = MakeTensor(l1Ptr, l1Layout);

// 5. 设定缓存策略
gmTensor.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);

// 6. 数据搬运：GM → L1
auto atom = MakeCopy(CopyGM2L1{});
Copy(atom, l1Tensor, gmTensor);

// 7. 按坐标访问
auto val = gmTensor[MakeCoord(3, 5)];
```
