LocalTensor是AscendC Tensor编程体系的核心数据抽象，支持两种模板形态。

# 前置知识：模板特化机制

**什么是模板特化？**

C++模板特化是为特定类型提供特殊实现的技术。类比理解：

```
普通模板：一个通用的"容器模板"
template <typename T>
class Container { /* 存储T类型数据 */ };

特化版本：为特定类型提供"定制容器"
template <>
class Container<int> { /* 专门为int优化的容器 */ };
```

**LocalTensor的模板特化扩展**：
```cpp
// 基础模板：通用的LocalTensor
template <typename T>
class LocalTensor { /* 指针 + 大小 */ };

// 特化版本：为TensorAttribute类型提供扩展实现
template <typename Engine, typename Layout>
class LocalTensor<TensorAttribute<Engine, Layout>> { /* Engine + Layout */ };
```

**为什么用TensorAttribute作为特化标签？**

TensorAttribute是一个"空壳结构体"，只用于携带类型信息：
```cpp
template <typename Engine, typename Layout>
struct TensorAttribute {};  // 没有数据成员，只传递类型
```

就像快递单上的"标签"，标明了包裹里有什么（Engine和Layout类型），但本身不是包裹内容。

**为什么这样设计？三个关键原因**：

1. **保持兼容性**：原始形态`LocalTensor<T>`仍然可用，不影响现有代码
2. **渐进式扩展**：通过特化添加新能力（Layout），而非破坏原有设计
3. **编译期类型检查**：模板特化让编译器知道两种形态是不同类型，避免误用

```
设计对比：

方案A（破坏性修改）：修改LocalTensor类，添加Layout成员
  问题：现有代码全部需要修改，不兼容

方案B（继承扩展）：class ExtendedLocalTensor : public LocalTensor
  问题：运行时多态，性能开销

方案C（模板特化）✓：特化LocalTensor<TensorAttribute<...>>
  优点：编译期静态，无性能开销，保持兼容
```

# 前置知识：Layout布局抽象

**什么是Layout？**

Layout描述数据的"内存排列方式"，包含Shape（形状）和Stride（步长）。

**直观理解：Shape和Stride**

假设有一个2x3的矩阵：
```
逻辑形状 Shape = (2, 3)      内存布局（行优先）：
   ↓                           ┌───┬───┬───┬───┬───┬───┐
[0,0] [0,1] [0,2]              │ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │ ← 内存地址
[1,0] [1,1] [1,2]              └───┴───┴───┴───┴───┴───┘
                               ↑   ↑   ↑   ↑   ↑   ↑
                            [0,0][0,1][0,2][1,0][1,1][1,2]

Stride = (3, 1)      步长含义：
   ↓                 第一维步长=3：从[0,0]到[1,0]跳3个位置
跳3个 → [1,0]        第二维步长=1：从[0,0]到[0,1]跳1个位置
跳1个 → [0,1]
```

**索引计算公式**：
```
坐标(i, j) → 内存偏移 = i * stride[0] + j * stride[1]
                        = i * 3 + j * 1

例如：坐标(1, 2) → 偏移 = 1*3 + 2*1 = 5 → 内存位置[5]
```

**更多Stride计算示例（帮你熟练掌握）**：

| 场景 | Shape | Stride | 坐标(2,3)的偏移量 |
|------|-------|--------|------------------|
| **4x5矩阵（行优先）** | (4, 5) | (5, 1) | 2*5+3*1=13 |
| **4x5矩阵（列优先）** | (4, 5) | (1, 4) | 2*1+3*4=14 |
| **3x4x5三维张量** | (3, 4, 5) | (20, 5, 1) | 2*20+3*5+1*1=56 |
| **非连续存储** | (2, 3) | (6, 2) | 2*6+3*2=18（间隔存储） |

**Stride计算规则总结**：
```
行优先存储：stride[i] = shape[i+1] * stride[i+1]（最后一维stride=1）
列优先存储：stride[i] = shape[i-1] * stride[i-1]（第一维stride=1）

行优先示例：Shape=(4,5)
  stride[1] = 1（最后一维）
  stride[0] = shape[1] * stride[1] = 5 * 1 = 5
  所以Stride=(5, 1)
```

**Layout的作用**：
- 原始形态：用户手动计算偏移量（容易出错，需要记住公式）
- 扩展形态：Layout自动计算（坐标(1,2) → 自动得到偏移5）

# LocalTensor原始形态

**原始形态**（`basic_api/kernel_tensor.h`）：
```cpp
template <typename T> 
class LocalTensor : public BaseLocalTensor<T>, public BaseTensorTraitTensor<T> {
public:
    using PrimType = PrimT<T>;
    
    // 构造函数：通过地址创建Tensor
    __aicore__ inline LocalTensor<T>(TPosition pos, uint32_t addr, uint32_t tileSize);
    __aicore__ inline LocalTensor<T>(uint32_t addr);
    
    // 核心方法
    __aicore__ inline uint64_t GetPhyAddr() const;               // 获取物理地址
    __aicore__ inline uint64_t GetPhyAddr(uint32_t offset) const; // 获取偏移地址
    __aicore__ inline uint32_t GetSize() const;                   // 获取大小
    __aicore__ inline void SetSize(uint32_t size);                // 设置大小
    __aicore__ inline int32_t GetPosition() const;                // 获取存储位置（UB/L1等）
    
    // 元素访问：偏移量方式
    __aicore__ inline PrimType GetValue(uint32_t index) const;    // 读取第index个元素
    __aicore__ inline PrimType& operator()(uint32_t offset);      // 访问第offset个元素
    __aicore__ inline LocalTensor operator[](uint32_t offset);    // 创建从offset开始的子Tensor
    
    // ShapeInfo（可选，仅记录信息）
    __aicore__ inline void SetShapeInfo(const ShapeInfo& shapeInfo);
    __aicore__ inline ShapeInfo GetShapeInfo() const;
};
```

**原始形态特点**：
- 数据指针 + 大小（size）
- 偏移量访问：`tensor(offset)`，用户需要手动计算偏移
- ShapeInfo可选，仅用于记录，不参与索引计算

# LocalTensor特例化扩展形态

**扩展形态通过模板特化注入Layout能力**：

```cpp
// TensorAttribute：特化标签，携带Engine和Layout类型
template <typename EngineType, typename LayoutType>
struct TensorAttribute {};  // 空壳，只传递类型信息

// 特化LocalTensor：为TensorAttribute类型提供扩展实现
template <typename EngineType, typename LayoutType>
struct LocalTensor<TensorAttribute<EngineType, LayoutType>> {
    using iterator = typename EngineType::iterator;
    using valueType = typename EngineType::valueType;
    using engineType = EngineType;   // Engine类型
    using layoutType = LayoutType;   // Layout类型
    
    Std::tuple<layoutType, engineType> rep;  // 内部存储：Layout + Engine
    
    static constexpr int rank = LayoutType::rank;  // 维度数
    
    // 核心方法
    __aicore__ inline decltype(auto) Engine();      // 获取Engine（数据指针封装）
    __aicore__ inline decltype(auto) Layout();      // 获取Layout
    __aicore__ inline decltype(auto) Data();        // 获取原始数据指针
    __aicore__ inline decltype(auto) Shape();       // 获取Shape
    __aicore__ inline decltype(auto) Stride();      // 获取Stride
    __aicore__ inline auto Size();                  // 获取元素总数
    
    // 元素访问：坐标方式（Layout自动计算索引）
    __aicore__ inline decltype(auto) operator[](const Coord& coord) {
        return Data()[Layout()(coord)];  // Layout将coord转为偏移量
    }
    
    // 多参数坐标访问
    __aicore__ inline decltype(auto) operator()(Coord0 c0, Coord1 c1, ...);
    
    // 分块访问：创建子Tensor
    __aicore__ inline decltype(auto) operator()(Coord coord, InfoType info);
};
```

**扩展形态特点**：
- Engine + Layout：Engine封装指针，Layout管理布局
- 坐标访问：`tensor[coord]`，Layout自动计算偏移量
- 内置Shape和Stride，自动索引计算

# 两种形态对比示例

**访问第(1,2)个元素的对比**：

```cpp
// ========== 原始形态：手动计算偏移量 ==========
LocalTensor<half> tensor;
tensor.SetSize(6);
// 假设Shape = (2, 3)，行优先存储
// 坐标(1, 2) → 偏移量 = 1 * 3 + 2 = 5（用户手动计算）
half value = tensor.GetValue(5);  // 需要知道偏移5对应坐标(1,2)


// ========== 扩展形态：坐标直接访问 ==========
auto tensor = MakeTensor(ptr, MakeNDLayout<half>(2, 3));  // 自动创建Layout
// Layout知道Shape=(2,3)和Stride=(3,1)
half value = tensor[MakeCoord(1, 2)];  // 直接用坐标，Layout自动计算偏移=5
// 或者
half value = tensor(1, 2);             // 简写形式
```

**核心差异**：
- 原始形态：用户负责"坐标→偏移"的转换
- 扩展形态：Layout自动完成"坐标→偏移"的转换

---
