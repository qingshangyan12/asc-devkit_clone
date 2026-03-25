# CopyAtom 和 MmadAtom 详细设计文档

## 1. 需求及场景描述

### 1.1 CopyAtom 需求及场景

#### 1.1.1 需求背景
在 Ascend C 算子开发中，数据搬运是性能优化的关键环节。NPU 采用多级存储架构（GM、L1、L0、UB、OUT），不同存储单元之间的数据搬运需要高效的原子操作支持。CopyAtom 旨在提供统一的、类型安全的、可配置的数据搬运原子操作接口。

#### 1.1.2 应用场景
CopyAtom 主要应用于以下场景：

1. **GM 到 L1 搬运**：将全局内存数据搬运到一级缓存，减少访问延迟
   - 典型应用：从 GM 读取输入数据到 L1 缓存
   - 支持特性：批量搬运、非连续地址访问、padding 等

2. **L1 到 L0 搬运**：将一级缓存数据搬运到零级缓存，供计算单元使用
   - 典型应用：为 Cube 计算准备数据
   - 支持特性：重复搬运、地址偏移、广播等

3. **L0C 到 OUT 搬运**：将计算结果从 L0C 搬运到输出缓冲区
   - 典型应用：将矩阵计算结果输出
   - 支持特性：结果回写、地址对齐等

4. **灵活配置**：通过 traits 机制支持不同搬运模式的配置
   - 支持自定义搬运参数
   - 支持运行时和编译时配置

#### 1.1.3 设计目标
- **统一接口**：为所有搬运操作提供一致的调用方式
- **类型安全**：通过模板元编程保证类型安全
- **高性能**：编译期优化，零运行时开销
- **可扩展**：通过 traits 机制支持新的搬运模式

### 1.2 MmadAtom 需求及场景

#### 1.2.1 需求背景
矩阵乘加（Matrix Multiply-Add）是深度学习计算的核心操作，卷积、全连接层、注意力机制等都依赖于高效的矩阵乘加。MmadAtom 旨在为 NPU 的 Cube 计算单元提供高效的矩阵乘加原子操作接口。

#### 1.2.2 应用场景
MmadAtom 主要应用于以下场景：

1. **矩阵乘法**：C = A × B
   - 典型应用：全连接层、矩阵变换
   - 支持不同数据类型组合（s8/f16/f32 等）

2. **矩阵乘加**：C = C + A × B
   - 典型应用：梯度累加、批量矩阵乘
   - 支持累加模式，减少内存访问

3. **稀疏矩阵乘加**：支持稀疏矩阵的乘加运算
   - 典型应用：稀疏神经网络、推荐系统
   - 优化稀疏数据的计算效率

4. **带偏置的矩阵乘加**：C = A × B + bias
   - 典型应用：卷积层、注意力机制
   - 支持偏置向量的高效融合计算

5. **批量矩阵乘**：支持多个矩阵的并行乘加
   - 典型应用：批量推理、Transformer 模型
   - 提高 Cube 单元的利用率

#### 1.2.3 设计目标
- **高性能**：充分利用 NPU Cube 计算单元的并行能力
- **灵活性**：支持多种数据类型组合和计算模式
- **可配置**：通过 traits 机制支持不同的计算参数配置
- **易用性**：提供简洁的接口，隐藏底层硬件细节

## 2. 接口描述

### 2.1 CopyAtom 接口

#### 2.1.1 结构体定义
```cpp
template <typename... Args>
struct CopyAtom {
    std::tuple<Args...> value;
};
```

#### 2.1.2 特化版本
```cpp
template <typename CopyOperation>
struct CopyAtom<CopyOperation> : CopyAtom<CopyTraits<CopyOperation>> {};

template <typename... Args>
struct CopyAtom<CopyTraits<Args...>>: CopyTraits<Args...>
{
    using CopyTraitType = CopyTraits<Args...>;
    using TraitType = typename CopyTraitType::TraitType;
    static constexpr const TraitType defaultTrait = CopyTraitType::defaultTrait;

    template <const TraitType& traits = defaultTrait, typename... Params>
    __aicore__ inline void Call(const Params& ...params) const {
        CopyTraitType::template CopyUnpack<traits>(params...);
    }

    template <typename... TraitsArgs>
    __aicore__ inline auto with(TraitsArgs&&... args) const {
        auto traits = CopyTraitType::with(static_cast<TraitsArgs&&>(args)...);
        return CopyAtom<decltype(traits)>{traits};
    }
};
```

#### 2.1.3 接口方法

**Call 方法**
- **功能**：执行数据搬运操作
- **参数**：
  - `traits`：搬运配置参数（可选，默认为 defaultTrait）
  - `params`：搬运操作所需参数（源地址、目标地址、大小等）
- **返回值**：无
- **说明**：模板参数支持编译期优化，traits 参数支持运行时配置

**with 方法**
- **功能**：创建带自定义配置的 CopyAtom
- **参数**：
  - `args`：自定义配置参数
- **返回值**：新的 CopyAtom 实例
- **说明**：支持链式调用，可以多次配置不同的参数

#### 2.1.4 支持的搬运类型

1. **GM2L1**：全局内存到一级缓存
   - 参数：源地址、目标地址、大小、步长等
   - 应用场景：批量数据加载

2. **L12L0**：一级缓存到零级缓存
   - 参数：源地址、目标地址、重复次数、偏移等
   - 应用场景：为 Cube 计算准备数据

3. **L0C2OUT**：L0C 到输出缓冲区
   - 参数：源地址、目标地址、大小等
   - 应用场景：计算结果输出

#### 2.1.5 调用示例
```cpp
// 基本用法
auto copyAtom = AscendC::MakeCopy(arg1, arg2, arg3);
copyAtom.Call(dst, src);

// 使用自定义 traits
auto customCopyAtom = copyAtom.with(param1, param2);
customCopyAtom.Call(dst, src);

// 编译期配置
constexpr auto traits = /* 配置参数 */;
copyAtom.Call<traits>(dst, src);
```

### 2.2 MmadAtom 接口

#### 2.2.1 结构体定义
```cpp
template <typename... Args>
struct MmadAtom {
    std::tuple<Args...> value;
};
```

#### 2.2.2 特化版本
```cpp
template <typename MadOperation>
struct MmadAtom<MadOperation> : MmadAtom<MmadTraits<MadOperation>> {};

template <typename MadOperation, typename... Args>
struct MmadAtom<MmadTraits<MadOperation, Args...>> : MmadTraits<MadOperation, Args...>
{
    using MadTraitType = MmadTraits<MadOperation, Args...>;
    using TraitType = typename MadTraitType::TraitType;
    static constexpr const TraitType defaultTrait = MadTraitType::defaultTrait;

    template <const TraitType& traits = defaultTrait, typename... Params>
    __aicore__ inline void Call(const Params& ...params) const {
        MadTraitType::template MmadUnpack<traits>(params...);
    }

    template <typename... TraitsArgs>
    __aicore__ inline auto with(TraitsArgs&&... args) const {
        auto traits = MadTraitType::with(static_cast<TraitsArgs&&>(args)...);
        return MmadAtom<decltype(traits)>{traits};
    }
};
```

#### 2.2.3 接口方法

**Call 方法**
- **功能**：执行矩阵乘加操作
- **参数**：
  - `traits`：计算配置参数（可选，默认为 defaultTrait）
  - `params`：计算所需参数（矩阵 A、矩阵 B、矩阵 C、维度信息等）
- **返回值**：无
- **说明**：支持累加模式，C = C + A × B

**with 方法**
- **功能**：创建带自定义配置的 MmadAtom
- **参数**：
  - `args`：自定义配置参数
- **返回值**：新的 MmadAtom 实例
- **说明**：支持链式调用，配置不同的计算参数

#### 2.2.4 支持的计算模式

1. **标准矩阵乘加**：C = C + A × B
   - 支持数据类型：s8×s8→s32, f16×f16→f32, f16×f16→f16 等
   - 维度支持：任意符合 Cube 对齐要求的维度

2. **稀疏矩阵乘加**：支持稀疏矩阵的乘加
   - 稀疏格式：CSR、CSC 等
   - 优化稀疏数据的存储和计算

3. **带偏置的矩阵乘加**：C = A × B + bias
   - 支持偏置向量的广播
   - 融合计算，减少中间结果

4. **批量矩阵乘**：多个矩阵的并行乘加
   -   支持 batch 维度
   - 提高 Cube 单元利用率

#### 2.2.5 调用示例
```cpp
// 基本用法
auto mmadAtom = AscendC::MakeMad(arg1, arg2, arg3);
mmadAtom.Call(dst, src0, src1);

// 使用自定义 traits
auto customMmadAtom = mmadAtom.with(param1, param2);
customMmadAtom.Call(dst, src0, src1);

// 编译期配置
constexpr auto traits = /* 配置参数 */;
mmadAtom.Call<traits>(dst, src0, src1);
```

## 3. 方案概述

### 3.1 整体架构

CopyAtom 和 MmadAtom 采用统一的原子操作架构，基于 Traits 模式实现灵活的参数配置和类型安全的接口设计。

```
                    ┌─────────────────┐
                    │   用户代码层     │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │  CopyAtom/      │
                    │  MmadAtom       │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │  CopyTraits/    │
                    │  MmadTraits     │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │  具体操作实现   │
                    │  (GM2L1/L12L0/  │
                    │   Mmad等)       │
                    └─────────────────┘
```

### 3.2 CopyAtom 实现方案

#### 3.2.1 核心设计思想
1. **模板元编程**：利用 C++ 模板实现编译期类型检查和代码生成
2. **Traits 模式**：通过 CopyTraits 封装不同搬运操作的配置和实现
3. **零开销抽象**：所有配置都在编译期确定，运行时无额外开销

#### 3.2.2 实现细节

**CopyTraits 设计**
```cpp
template <typename CopyOperation, typename... CopyOpArgs>
struct CopyTraits{};

template <typename CopyOp, typename Traits, typename CopyOpWith, typename TraitsWith>
struct CopyTraits<CopyOp, Traits, CopyOpWith, TraitsWith>
{
    using TraitType = typename Traits::TraitType;
    static constexpr const TraitType defaultTrait = Traits::value;

    template <const TraitType& trait = defaultTrait, typename... Args>
    __aicore__ inline void CopyUnpack(const Args& ...args) const {
      CopyOp::template Copy<TraitType, trait, Args...>(args...);
    }

    template <typename... Args>
    __aicore__ inline constexpr CopyTraits<CopyOpWith, TraitsWith>
    with(const Args& ...args) const
    {
        return {args...};
    }
};
```

**关键特性**
1. **类型推导**：自动推导搬运操作的参数类型
2. **配置复用**：通过 with() 方法创建带不同配置的 CopyAtom
3. **编译期优化**：模板参数在编译期展开，生成最优代码

#### 3.2.1 数据流设计

```
GM (全局内存)
    │
    ▼ DataCopy
L1 (一级缓存)
    │
    ▼ CopyAtom(GM2L1)
L1 (一级缓存)
    │
    ▼ CopyAtom(L12L0)
L0 (零级缓存)
    │
    ▼ Cube 计算
L0C (计算结果)
    │
    ▼ CopyAtom(L0C2OUT)
OUT (输出缓冲区)
```

### 3.3 MmadAtom 实现方案

#### 3.3.1 核心设计思想
1. **硬件抽象**：封装 NPU Cube 计算单元的硬件特性
2. **数据类型适配**：支持多种数据类型的组合和转换
3. **计算模式配置**：通过 Traits 支持不同的计算模式

#### 3.3.2 实现细节

**MmadTraits 设计**
```cpp
template <typename MadOperation, typename... MadOpArgs>
struct MmadTraits{};

template <typename MadOp, typename MadTraits, 
          typename MadOpWith, typename MadTraitsWith>
struct MmadTraits<MadOp, MadTraits, MadOpWith, MadTraitsWith>
{
    using TraitType = typename MadTraits::TraitType;
    static constexpr const TraitType defaultTrait = MadTraits::value;

    template <const TraitType& trait = defaultTrait, typename... Args>
    __aicore__ inline void MmadUnpack(const Args& ...args) const {
        MadOp::template Mad<TraitType, trait, Args...>(args...);
    }

    template <typename... Args>
    __aicore__ inline constexpr MmadTraits<MadOpWith, MadTraitsWith>
    with(const Args& ...args) const
    {
        return {args...};
    }
};
```

**MmadOperation 设计**
```cpp
struct MmadOperation {
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Mad(const Args& ...args)
    {
        Mmad<traits, Args...>(args...);
    }
};
```

**关键特性**
1. **维度检查**：编译期检查矩阵维度是否满足 Cube 对齐要求
2. **数据类型检查**：确保输入输出数据类型组合合法
3. **性能优化**：自动选择最优的计算路径

#### 3.3.3 计算流程设计

```
数据准备阶段：
    ┌─────────┐    ┌─────────┐
    │  矩阵 A │    │  矩阵 B │
    └────┬────┘    └────┬────┘
         │              │
         ▼ DataCopy     ▼ DataCopy
    ┌─────────┐    ┌─────────┐
    │  L1(A)  │    │  L1(B)  │
    └────┬────┘    └────┬────┘
         │              │
         ▼ CopyAtom     ▼ CopyAtom
    ┌─────────┐    ┌─────────┐
    │  L0(A)  │    │  L0(B)  │
    └────┬────┘    └────┬────┘
         │              │
         └──────┬───────┘
                ▼
         ┌──────────────┐
         │  MmadAtom    │
         │  C = C + A×B │
         └──────┬───────┘
                │
                ▼
         ┌─────────┐
         │  L0C(C) │
         └────┬────┘
              │
              ▼ CopyAtom
         ┌─────────┐
         │  OUT(C) │
         └─────────┘
```

### 3.4 性能优化策略

#### 3.4.1 CopyAtom 优化
1. **批量搬运**：一次搬运多个数据块，减少指令开销
2. **流水线优化**：利用 NPU 的多级流水线，实现搬运与计算并行
3. **地址对齐**：确保数据地址满足硬件对齐要求，提高访问效率
4. **缓存复用**：合理规划数据布局，提高缓存命中率

#### 3.4.2 MmadAtom 优化
1. **数据重排**：将数据转换为适合 Cube 计算的格式（如 Nz、Zz 格式）
2. **分块计算**：将大矩阵划分为多个小块，提高缓存利用率
3. **融合计算**：将矩阵乘加与偏置、激活等操作融合，减少中间结果
4. **并行计算**：充分利用 Cube 单元的并行计算能力

### 3.5 约束与限制

#### 3.5.1 CopyAtom 约束
1. **地址对齐**：源地址和目标地址必须满足相应的对齐要求
2. **大小限制**：单次搬运的数据大小不能超过硬件限制
3. **内存区域**：只能在指定的内存区域之间进行搬运
4. **并发限制**：同一内存区域的并发搬运需要满足硬件约束

#### 3.5.2 MmadAtom 约束
1. **维度对齐**：矩阵维度必须是 16 的倍数（Cube block 大小）
2. **数据类型**：输入输出数据类型组合必须支持
3. **内存布局**：输入数据必须满足特定的内存布局要求
4. **累加模式**：累加模式下，目标矩阵 C 必须已初始化

### 3.6 扩展性设计

#### 3.6.1 新增搬运类型
要添加新的搬运类型，需要：
1. 定义新的 CopyOperation
2. 定义对应的 Traits
3. 在 CopyTraits 中添加特化
4. 实现具体的搬运逻辑

#### 3.6.2 新增计算模式
要添加新的计算模式，需要：
1. 定义新的 MadOperation
2. 定义对应的 Traits
3. 在 MmadTraits 中添加特化
4. 实现具体的计算逻辑

## 4. 总结

CopyAtom 和 MmadAtom 是 Ascend C 算子开发中的核心原子操作接口，通过统一的接口设计、灵活的配置机制和高效的实现，为开发者提供了强大的数据搬运和矩阵计算能力。

**主要优势**：
1. **统一接口**：简化了底层硬件操作的调用
2. **类型安全**：通过模板元编程保证类型安全
3. **高性能**：编译期优化，零运行时开销
4. **易扩展**：通过 Traits 机制支持功能扩展

**应用价值**：
- 提高算子开发效率
- 优化算子性能
- 降低开发门槛
- 保证代码质量

通过合理使用 CopyAtom 和 MmadAtom，开发者可以充分发挥 NPU 的硬件性能，实现高效的算子实现。
