# 2.4 内存指针与引擎

TensorAPI 通过 Engine 和 Pointer 两层抽象来管理 NPU 多级存储上的内存指针。

## 2.4.1 整体架构

```
┌──────────────────────────────────────────────────┐
│                    LocalTensor                    │
│          ┌─────────────┬──────────────────┐      │
│          │   Layout    │     Engine       │      │
│          │(Shape+Stride)│ (ViewEngine<T>)  │      │
│          └─────────────┴──────┬───────────┘      │
│                               │                   │
│                         Begin() → Iterator        │
│                               │                   │
│                  ┌────────────┴───────────┐       │
│                  │   HardwareMemPtr       │       │
│                  │  (PtrPattern, Pointer) │       │
│                  └────────────┬───────────┘       │
│                               │                   │
│                   原始 NPU 指针 (__gm__ T* 等)     │
└──────────────────────────────────────────────────┘
```

## 2.4.2 ViewEngine

ViewEngine 是 TensorAPI 中的内存引擎，封装了原始指针并提供偏移和缓存控制能力。

```cpp
template <typename Iterator>
struct ViewEngine {
    using iterator     = Iterator;
    using reference    = typename IterRef<iterator>::type;  // T&
    using elementType  = typename IterEle<iterator>::type;  // 去引用
    using valueType    = typename IterVal<iterator>::type;  // 去cv限定

    constexpr iterator const& Begin() const;    // 获取原始指针
    constexpr iterator&       Begin();

    constexpr uint8_t GetCacheMode() const;
    constexpr void    SetCacheMode(CacheMode mode);

    template <typename Index>
    constexpr ViewEngine operator+(const Index& i) const;  // 偏移操作
};
```

关键设计：
- `operator+` 返回一个新的 ViewEngine，其指针偏移了 `i` 个元素位置——这个特性是 Tensor 切片的核心实现机制。

### ConstViewEngine

```cpp
template <typename Iterator>
struct ConstViewEngine {
    // 类似 ViewEngine，但无 SetCacheMode
    constexpr iterator const& Begin() const;
};
```

## 2.4.3 内存层次位置编码

NPU 的每种内存类型都有唯一的编译器地址空间限定符（address-space qualifier）。TensorAPI 通过编译期映射将指针类型与 `Location` 标记关联：

```cpp
namespace Location {
    struct INVALID {};
    struct GM {};      // Global Memory
    struct L1 {};      // L1 Buffer
    struct UB {};      // Unified Buffer
    struct L0A {};     // L0A (Matrix A) Buffer
    struct L0B {};     // L0B (Matrix B) Buffer
    struct L0C {};     // L0C (Accumulator) Buffer
    struct BIAS {};    // Bias Buffer
    struct FIXBUF {};  // Fix Buffer
    struct SSBUF {};   // Shared Scratch Buffer
}
```

指针类型与 Location 的映射关系（定义在 `location_impl.h`）：

| Location | 指针类型 | 说明 |
|----------|----------|------|
| `GM` | `__gm__ T*` | Global Memory |
| `L1` | `__cbuf__ T*` | L1 Buffer (compute buffer) |
| `L0A` | `__ca__ T*` | Cube A Buffer |
| `L0B` | `__cb__ T*` | Cube B Buffer |
| `L0C` | `__cc__ T*` | Cube C Buffer |
| `UB` | `__ubuf__ T*` | Unified Buffer |
| `BIAS` | `__biasbuf__ T*` | Bias Buffer |
| `FIXBUF` | `__fbuf__ T*` | Fix Buffer |
| `SSBUF` | `__ssbuf__ T*` | Shared Scratch Buffer |

## 2.4.4 MakeMemPtr

```cpp
// (1) 创建硬件指针：指定 Location 和数据类型，传入字节偏移
template <typename Hardware, typename TraitOrType, typename... Args>
constexpr auto MakeMemPtr(Args... args);  // args = 字节偏移量

// (2) 从已有指针创建硬件指针
template <typename Hardware, typename... Args>
constexpr auto MakeMemPtr(Args... args);  // args = 已有指针

// (3) 自动检测 Location
template <typename Iterator>
constexpr auto MakeMemPtr(const Iterator& iter);
```

### 使用示例

```cpp
// 方式1：从原始 NPU 指针构造
__gm__ float*  gmData  = /* ... */;
__cbuf__ half* l1Data  = /* ... */;

auto gmPtr = MakeMemPtr<Location::GM>(gmData);
auto l1Ptr = MakeMemPtr<Location::L1>(l1Data);

// 方式2：自动检测（编译期推断 Location）
auto autoPtr = MakeMemPtr(gmData);  // 自动推断为 Location::GM
```

## 2.4.5 IterAdaptor 与 HardwareMemPtr

```cpp
template <typename PtrPattern, typename Pointer>
struct HardwareMemPtr : IterAdaptor<Pointer, HardwareMemPtr<PtrPattern, Pointer>> {
    using ptrPattern = PtrPattern;
};
```

`HardwareMemPtr` 是底层指针的包装器，通过 `PtrPattern`（Location 标记）记录该指针所指向的内存层次。它继承自 `IterAdaptor`，后者提供了迭代器接口（`operator*` / `operator+` 等），使其能被 `ViewEngine` 使用。

## 2.4.6 内存类型 Traits

| Trait | 说明 |
|-------|------|
| `GetAttributeLocation<T>` | 从指针类型提取 Location 标记 |
| `GetAttributeElementType<T>` | 从指针类型提取元素类型 |
| `GetMemLocation<T>` | 从 Tensor 的迭代器提取 Location |
| `IsHardwareMem<PtrPattern, T>` | 判断是否为指定层次的硬件内存指针 |

## 2.4.7 完整构造流程

```cpp
// Step 1: 分配 NPU 内存（外部完成）
__gm__ half* gmTensorIn  = /* 分配在 GM */;
__gm__ half* gmTensorOut = /* 分配在 GM */;
__cbuf__ half* l1Buf     = /* 分配在 L1 */;

// Step 2: 创建内存指针
auto gmInPtr  = MakeMemPtr<Location::GM>(gmTensorIn);
auto gmOutPtr = MakeMemPtr<Location::GM>(gmTensorOut);
auto l1Ptr    = MakeMemPtr<Location::L1>(l1Buf);

// Step 3: 创建 Layout
auto shape    = MakeShape(rows, cols);                 // 或 MakeFrameLayout
auto layout   = MakeLayout(shape);

// Step 4: 创建 Tensor
auto gmInTensor  = MakeTensor(gmInPtr,  layout);
auto gmOutTensor = MakeTensor(gmOutPtr, layout);
auto l1Tensor    = MakeTensor(l1Ptr,    layout);
```
