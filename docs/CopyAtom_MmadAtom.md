# CopyAtom 和 MmadAtom 详细设计文档

## 1. 需求及场景描述

### 1.1 CopyAtom 需求及场景

#### 1.1.1 需求背景
在 Ascend C 算子开发中，数据搬运（Data Movement）通常位于性能优化链路的前端与末端。硬件采用多级存储架构（例如 GM/L1/L0/UB/OUT），不同存储层之间的数据传输需要满足带宽、对齐、步长/重复次数等约束。为了避免在算子代码中重复编写底层搬运细节，同时保证在编译期完成类型与配置的约束校验，本设计引入 `CopyAtom` 作为“搬运原子操作”的统一抽象。

#### 1.1.2 应用场景
CopyAtom 面向“搬运到合适的计算位置”的数据流阶段，典型包括：

1. **GM 到 L1 搬运**  
   - 目的：从全局内存读取到一级缓存，以便后续更低层的搬运/重排/计算复用。  
   - 常见特性：批量搬运、非连续访问、padding、步长（stride）与偏移（offset）处理。

2. **L1 到 L0 搬运**  
   - 目的：为 Cube 计算准备数据进入零级缓存。  
   - 常见特性：重复搬运（repeat）、地址偏移、广播或子块搬运（按算子切分粒度）。

3. **L0C 到 OUT 搬运（结果回写）**  
   - 目的：将计算结果从计算相关缓冲区写回输出缓冲区。  
   - 常见特性：结果回写的对齐、块大小约束，以及与后处理/下一阶段的衔接。

4. **灵活配置与扩展**  
   - 通过 `traits` 机制表达不同搬运模式与约束条件。  
   - 允许同一用户接口在不同硬件策略/编译参数下生成对应的搬运实现。

#### 1.1.3 设计目标
- **统一接口**：用一致的调用方式覆盖多种搬运路径与模式。
- **类型安全**：利用模板元编程约束搬运参数的类型与数量。
- **高性能**：配置与选择尽量在编译期完成，避免运行时分支开销。
- **可扩展**：通过 Traits 扩展新搬运操作与新模式，而不改变用户调用习惯。

### 1.2 MmadAtom 需求及场景

#### 1.2.1 需求背景
矩阵乘加（Matrix Multiply-Add）是深度学习模型中最核心的计算模式之一（全连接、卷积的部分等价形式、梯度累加、注意力中的相关投影等）。Cube 计算单元对矩阵数据布局与维度具有特定约束，且为获得高吞吐通常需要按 block/tiling 对矩阵进行切分和迭代。为隐藏底层布局细节、同时支持不同数据类型/累加模式/稀疏模式（若适配），本设计引入 `MmadAtom` 作为 Cube 计算的原子化抽象。

#### 1.2.2 应用场景
MmadAtom 支持常见矩阵乘加语义，典型包括：

1. **标准矩阵乘加**：`C = C + A × B`  
   - 支持多种数据类型组合（例如 `s8/f16/f32` 等，具体由 traits/实现支持集合决定）。
   - 支持与 tiled 迭代搭配的累加语义。

2. **稀疏矩阵乘加**  
   - 以稀疏格式输入（如 CSR/CSC 等，具体由实现支持）。  
   - 目标是在稀疏数据场景降低无效计算，提高整体效率。

3. **带偏置的矩阵乘加**：`C = A × B + bias`  
   - 偏置向量可按广播或按 block 融合到计算路径中，减少中间落盘与额外访存。

4. **批量矩阵乘加（batch）**  
   - 面向多矩阵并行/连续批次，提升 Cube 单元利用率。

#### 1.2.3 设计目标
- **高性能**：充分适配 Cube 的并行与吞吐机制（通过 traits 与布局优化实现）。
- **灵活性**：支持不同计算模式与数据类型组合。
- **可配置**：通过 `traits` 表达计算参数并允许用户进行模式选择。
- **易用性**：将维度检查、类型检查、实现选择封装在接口/traits 内。

## 2. 接口描述

> 本节按“用户可见接口”与“内部 Traits 驱动机制”两条线说明，便于用户理解如何构建并调用原子操作。

### 2.1 CopyAtom 接口

#### 2.1.1 结构体与类型模型

`CopyAtom` 是一个模板结构体，用于承载拷贝操作所需的参数/配置（以 `std::tuple` 的形式聚合）。

```cpp
template <typename... Args>
struct CopyAtom {
    std::tuple<Args...> value;
};
```

此外，针对“拷贝操作类型 + 对应 traits 参数”的情形，`CopyAtom` 会进一步继承/组合 Traits 类型，使得用户能够通过 `Call` 与 `with` 完成配置与执行。

```cpp
template <typename CopyOperation>
struct CopyAtom<CopyOperation> : CopyAtom<CopyTraits<CopyOperation>> {};
```

#### 2.1.2 Call 方法
`Call` 用于发起实际的数据搬运执行。其核心特性为：

- 支持将 `traits` 作为模板参数（编译期配置）或使用默认 `defaultTrait`。
- 将用户传入的 `params...` 解析为具体拷贝指令所需的参数集合。
- 通过 Traits 的 `CopyUnpack<traits>` 将调用展开到对应的底层实现。

```cpp
template <const TraitType& traits = defaultTrait, typename... Params>
__aicore__ inline void Call(const Params& ...params) const {
    CopyTraitType::template CopyUnpack<traits>(params...);
}
```

#### 2.1.2.1 参数约定（概念级）
由于具体实现可能因不同 `CopyOperation`/`CopyTraits` 特化而变化，用户侧在概念上需满足以下契约：

- `dst`/`src`：指向目标/源缓冲区的指针或张量句柄；其所在内存空间与 traits（例如 GM/L1/L0C/OUT）语义必须匹配。
- `size`/`length`：描述搬运数据量（可用字节数或元素数表示，具体由实现约定）。
- `stride`/`offset`（如适用）：当访问呈现非连续布局或需要子块偏移时，由 traits 决定需要哪些字段、以及字段语义。
- `repeat`（如适用）：当一次调用需要重复搬运多个块/行/列时，由 traits 定义重复粒度。

#### 2.1.2.2 traits 参数契约
`CopyAtom` 的 traits 通过编译期常量引用（`const TraitType& traits`）传入 `CopyUnpack<traits>`。因此：

- `TraitType` 必须提供 `defaultTrait`（用于未显式指定 traits 时的默认路径）。
- `CopyTraitType::CopyUnpack<traits>(params...)` 要求 `params...` 与该 `traits` 对应的 `CopyOperation` 参数签名一致（由类型系统/特化实现保证）。

#### 2.1.3 with 方法（配置扩展）
`with` 用于在不改变用户调用入口的前提下，生成带特定 traits 配置的新的 `CopyAtom` 实例，实现“链式配置”。

```cpp
template <typename... TraitsArgs>
__aicore__ inline auto with(TraitsArgs&&... args) const {
    auto traits = CopyTraitType::with(static_cast<TraitsArgs&&>(args)...);
    return CopyAtom<decltype(traits)>{traits};
}
```

#### 2.1.4 典型使用方式
- 创建 `CopyAtom`：通常由工厂函数（例如 `AscendC::MakeCopy(...)`）根据源/目标等参数推导生成。
- 执行搬运：通常由 `CopyAtom.Call(...)` 或上层提供的 `AscendC::Copy(copyAtom, ...)` 形式触发。

示例（概念性）：
```cpp
auto copyAtom = AscendC::MakeCopy(arg1, arg2, arg3);
copyAtom.Call(dst, src);

auto customCopyAtom = copyAtom.with(param1, param2);
customCopyAtom.Call(dst, src);

constexpr auto traits = /* 配置参数 */;
copyAtom.Call<traits>(dst, src);
```

#### 2.1.5 支持的搬运类型（语义级）
根据现有设计与文档，该接口覆盖以下类型语义（具体由实现与 Traits 特化决定）：

- **GM2L1**：GM -> L1  
- **L1L0**：L1 -> L0  
- **L0C2OUT**：L0C -> OUT  

### 2.2 MmadAtom 接口

#### 2.2.1 结构体与类型模型
`MmadAtom` 与 `CopyAtom` 的组织方式类似，用 `std::tuple` 承载参数，并通过继承/组合 Traits 类型实现可配置的 `Call` 与 `with`。

```cpp
template <typename... Args>
struct MmadAtom {
    std::tuple<Args...> value;
};
```

Traits 关联方式（概念性）：
```cpp
template <typename MadOperation>
struct MmadAtom<MadOperation> : MmadAtom<MmadTraits<MadOperation>> {};
```

#### 2.2.2 Call 方法
`Call` 用于触发矩阵乘加执行，其语义强调累加模式：`C = C + A × B`（偏置/稀疏等扩展模式取决于具体 traits/实现）。

```cpp
template <const TraitType& traits = defaultTrait, typename... Params>
__aicore__ inline void Call(const Params& ...params) const {
    MadTraitType::template MmadUnpack<traits>(params...);
}
```

在 Cube 侧，典型的 `params...` 包含（概念级）：A 矩阵、B 矩阵、C 矩阵、以及维度/步长/布局相关信息。

#### 2.2.2.1 参数约定（概念级）
为与 Cube 的计算契约一致，用户在概念上需要保证：

- `A`/`B`：输入矩阵的指针或张量句柄，其内存布局/数据类型与当前 `traits` 对应的合法组合匹配。
- `C`：输出矩阵/累加矩阵句柄；在累加模式下必须保证 `C` 已完成初始化（避免读未定义数据）。
- 维度信息：矩阵维度或等价的切分参数（例如 m/n/k 及其 tile 粒度）；其取值通常需满足对齐与 Cube block 约束。
- 步长/布局：当输入矩阵以非紧凑布局提供时，需要提供对应的 stride/布局字段；具体由实现/traits 决定字段集合。

#### 2.2.2.2 traits 参数契约
`MmadAtom` 通过 `MmadUnpack<traits>` 将编译期 traits 展开到具体 Cube 计算路径，因此：

- `TraitType` 需提供 `defaultTrait` 以承载默认计算策略。
- 对于不同计算模式（标准/稀疏/偏置/batch），traits 会影响 `MmadUnpack` 期望的参数签名与布局契约。

#### 2.2.3 with 方法（配置扩展）
与 CopyAtom 类似，`with` 允许生成带特定计算 traits 的新的 `MmadAtom`：

```cpp
template <typename... TraitsArgs>
__aicore__ inline auto with(TraitsArgs&&... args) const {
    auto traits = MadTraitType::with(static_cast<TraitsArgs&&>(args)...);
    return MmadAtom<decltype(traits)>{traits};
}
```

#### 2.2.4 典型使用方式
- 创建 `MmadAtom`：例如 `AscendC::MakeMad(...)`。
- 执行计算：例如 `mmadAtom.Call(...)` 或上层 `AscendC::Mad(mmadAtom, ...)`。

示例（概念性）：
```cpp
auto mmadAtom = AscendC::MakeMad(arg1, arg2, arg3);
mmadAtom.Call(dst, src0, src1);
```

#### 2.2.5 支持的计算模式（语义级）
- 标准矩阵乘加（累加）  
- 稀疏矩阵乘加  
- 带偏置矩阵乘加  
- 批量矩阵乘加（batch）  

## 3. 方案概述

### 3.1 整体架构（统一 Traits 原子化）
CopyAtom 与 MmadAtom 采用统一的原子操作 + Traits 驱动设计：

1. **用户代码层**：通过 `MakeCopy/MakeMad` 创建原子对象，并通过 `Call` 发起执行；也可以通过 `with` 形成带特定 traits 的原子对象。
2. **原子对象层**（CopyAtom/MmadAtom）：负责将 traits 与参数封装到类型系统，并提供 `Call/with` 的入口。
3. **Traits 层**（CopyTraits/MmadTraits）：定义默认 traits、以及把“traits + 参数列表”拆解/转发到具体实现的 `CopyUnpack/MmadUnpack`。
4. **具体操作实现层**：实现 GM/L1/L0 搬运或 Cube 的具体乘加指令路径，并满足地址对齐、维度约束、数据布局约束等硬件要求。

### 3.2 CopyAtom 方案概述

#### 3.2.1 核心设计思想
- **模板元编程**：借助 C++ 模板在编译期推导参数类型与配置项，减少运行时分支。
- **Traits 模式**：将搬运路径（GM->L1、L1->L0、L0C->OUT 等）与具体搬运约束封装在 traits/特化中。
- **零开销抽象**：`Call`/`with` 的展开发生在编译期，最终生成与具体搬运类型匹配的底层代码。

#### 3.2.2 数据流设计（语义级）
典型的算子数据流可表达为：
```text
GM(全局内存)
  -> CopyAtom(GM2L1)
L1(一级缓存)
  -> CopyAtom(L1L0)
L0(零级缓存)
  -> Cube 计算
L0C(计算相关缓冲区)
  -> CopyAtom(L0C2OUT)
OUT(输出缓冲区)
```

#### 3.2.3 关键特性（接口层到实现层）
- **类型推导**：原子对象携带的参数元组使得接口与 traits 的参数契约更明确。
- **配置复用**：`with` 生成新的 traits 组合，便于在同一算法中对不同分块/不同路径复用代码结构。
- **编译期优化**：traits 作为模板参数时可触发更激进的编译期展开与分支消除。

### 3.3 MmadAtom 方案概述

#### 3.3.1 核心设计思想
- **硬件抽象**：`MmadAtom` 抽象 Cube 的矩阵乘加执行入口。
- **数据类型适配**：traits 描述合法的数据类型组合及必要的转换/布局处理策略。
- **计算模式配置**：不同计算模式（标准/稀疏/带偏置/batch）通过 traits 扩展实现。

#### 3.3.2 计算流程设计（语义级）
```text
数据准备阶段：
  矩阵 A、B 搬运/重排 -> 进入 L1/L0
计算阶段：
  MmadAtom 执行 C = C + A×B（或扩展语义）
结果阶段：
  通过 CopyAtom 将 C 写回 OUT
```

#### 3.3.3 关键校验点（约束驱动）
- **维度对齐**：矩阵维度需满足 Cube block 的对齐要求（文档中示例为 16 的倍数）。
- **数据类型组合合法性**：输入输出类型与实现能力必须匹配。
- **累加前置条件**：累加模式下，C 作为输入必须已初始化（避免未定义的读-改-写语义）。

### 3.4 性能优化策略（概念级）

#### 3.4.1 CopyAtom
- **批量搬运**：以块/向量粒度搬运，减少单条指令开销。
- **流水线优化**：搬运与后续计算分阶段重叠，提高并行利用率。
- **地址对齐**：确保源/目标满足对齐要求以获得更高带宽效率。
- **缓存复用**：按 tile 规划尽可能复用 L1/L0 中的数据块。

#### 3.4.2 MmadAtom
- **数据重排与布局匹配**：将输入数据调整为适合 Cube 的布局形式（例如文档中提到的 Nz/Zz 等概念布局）。
- **分块计算（tiling）**：将大矩阵切分成 Cube 可高效处理的小块。
- **融合计算（如可用）**：在 traits/实现允许时将偏置、激活或其他步骤融合以减少中间访存。
- **并行计算**：利用 Cube 的并行执行能力提升吞吐。

### 3.5 约束与限制（总结）
#### 3.5.1 CopyAtom 约束
- 地址对齐要求需满足硬件/实现的具体规定。
- 单次搬运的数据大小存在硬件上限。
- 搬运发生在受限的内存区域组合之间（例如 GM/L1/L0/OUT 等路径）。
- 并发搬运在相应内存区域可能存在约束（由实现的硬件调度/资源模型决定）。

#### 3.5.2 MmadAtom 约束
- 维度对齐要求（例如示例中的 16 倍数约束）。
- 输入输出数据类型组合必须在实现支持范围内。
- 输入数据内存布局必须满足实现的布局契约。
- 累加模式下，目标矩阵 `C` 必须已初始化。

### 3.6 可扩展性设计
通过 traits 扩展能力实现“新搬运类型/新计算模式”的加入流程：

- 新增搬运类型：定义新的 `CopyOperation`、补齐对应的 `CopyTraits`、并实现底层搬运逻辑。
- 新增计算模式：定义新的 `MadOperation`、补齐对应的 `MmadTraits`、并实现底层计算逻辑。

## 4. 验证与落地建议（可选补充）
为保证设计在实现与算子中稳定落地，建议至少覆盖以下验证面：

- 编译期验证：为非法 traits/维度/类型组合验证能否尽早报错或拒绝实例化。
- 运行期一致性：对边界维度（对齐边界、最小块、非整除情况等）进行正确性对比。
- 性能回归：不同 tile/buffer 策略下测量吞吐与访存占比，观察 CopyAtom 与 MmadAtom 的配合效果。

