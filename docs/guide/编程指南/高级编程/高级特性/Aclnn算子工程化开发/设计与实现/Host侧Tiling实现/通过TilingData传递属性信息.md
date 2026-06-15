# 通过TilingData传递属性信息

> 类型：扩展

本文档介绍如何通过TilingData机制将算子属性从Host侧传递到Kernel侧，供算子计算时使用。

## 核心概念

算子的属性（Attribute）是在原型定义阶段通过`this->Attr()`接口声明的配置参数，例如维度、阈值、标志位等。这些属性需要在Kernel侧参与实际计算，因此需要通过某种机制从Host侧传递到Device侧。

TilingData不仅可以传递切分相关的参数（如totalLength、tileNum），还可以承载属性信息。具体做法是：

1. **原型定义**：声明算子属性
2. **Host侧提取**：在Tiling函数中通过API获取属性值
3. **放入TilingData**：将属性值存入TilingData结构体
4. **序列化传递**：随TilingData一起传递到Kernel侧
5. **Kernel侧使用**：反序列化后读取属性值参与计算

## Host侧获取属性的方法

在Tiling函数中，通过`gert::TilingContext`对象可以访问算子的属性列表。

### 核心API

```cpp
// 获取RuntimeAttrs对象
const gert::RuntimeAttrs* attrs = context->GetAttrs();

// 方式一：根据索引获取属性指针（index从0开始，按原型定义顺序）
template <typename T>
const T* GetAttrPointer(int index);

// 方式二：根据属性名称获取属性指针
template <typename T>
const T* GetAttrPointer(const char* name);
```

**关键要点**：

- `attrs`指针不会为nullptr，但返回的属性列表可能为空
- `index`表示属性在原型定义中的声明顺序（从0开始计数）
- `name`表示属性在原型定义中通过`this->Attr()`声明的名称
- 返回值是指向属性值的指针，如果属性不存在则返回nullptr
- 模板参数`T`必须与属性实际类型匹配

### 使用模式

#### 方式一：按索引获取属性

```cpp
namespace optiling {
static ge::graphStatus TilingFunc(gert::TilingContext* context)
{
    // 1. 获取属性对象
    const gert::RuntimeAttrs* attrs = context->GetAttrs();
    
    // 2. 提取第0个属性（假设原型定义中第一个Attr是整型）
    const int64_t* attr0Ptr = attrs->GetAttrPointer<int64_t>(0);
    if (attr0Ptr == nullptr) {
        // 属性不存在时的处理
        return ge::GRAPH_FAILED;
    }
    int64_t attr0Value = *attr0Ptr;
    
    // 3. 提取第1个属性（布尔型）
    const bool* attr1Ptr = attrs->GetAttrPointer<bool>(1);
    bool attr1Value = (attr1Ptr != nullptr) ? *attr1Ptr : false;
    
    // 4. 获取TilingData指针并将属性值放入
    MyOpTilingData *tiling = context->GetTilingData<MyOpTilingData>();
    tiling->paramFromAttr0 = static_cast<uint32_t>(attr0Value);
    tiling->flagFromAttr1 = attr1Value;
    
    // 5. 正常完成Tiling其他逻辑...
    
    return ge::GRAPH_SUCCESS;
}
} // namespace optiling
```

#### 方式二：按名称获取属性（推荐）

```cpp
namespace optiling {
static ge::graphStatus TilingFunc(gert::TilingContext* context)
{
    // 1. 获取属性对象
    const gert::RuntimeAttrs* attrs = context->GetAttrs();
    
    // 2. 按名称提取属性（假设原型定义中有名为"reduce_dim"的整型属性）
    const int64_t* reduceDimPtr = attrs->GetAttrPointer<int64_t>("reduce_dim");
    if (reduceDimPtr == nullptr) {
        // 属性不存在时的处理
        return ge::GRAPH_FAILED;
    }
    int64_t reduceDim = *reduceDimPtr;
    
    // 3. 按名称提取布尔型属性（假设原型定义中有名为"keep_dims"的属性）
    const bool* keepDimsPtr = attrs->GetAttrPointer<bool>("keep_dims");
    bool keepDims = (keepDimsPtr != nullptr) ? *keepDimsPtr : false;
    
    // 4. 获取TilingData指针并将属性值放入
    MyOpTilingData *tiling = context->GetTilingData<MyOpTilingData>();
    tiling->reduceDim = static_cast<uint32_t>(reduceDim);
    tiling->keepDims = keepDims;
    
    // 5. 正常完成Tiling其他逻辑...
    
    return ge::GRAPH_SUCCESS;
}
} // namespace optiling
```

**按名称获取的优势**：
- 代码可读性更强，属性用途一目了然
- 不依赖属性声明顺序，原型定义调整时无需修改Tiling代码
- 更适合属性较多的复杂算子场景

## 属性类型映射表

原型定义中的属性类型与`GetAttrPointer<T>`的模板参数对应关系：

| 原型定义类型 | C++类型 | GetAttrPointer模板参数 | 备注 |
|-------------|---------|----------------------|------|
| `.Int()` | int64_t | `int64_t` | 最常用 |
| `.Float()` | float | `float` | |
| `.Bool()` | bool | `bool` | |
| `.String()` | std::string | 不支持 | 字符串不适合通过Tiling传递 |
| `.ListInt()` | std::vector\<int64_t\> | 不建议 | 列表较大，建议其他方式 |

## 适用场景

通过TilingData传递属性适合以下情况：

✅ **推荐场景**：
- 属性值较小（整数、浮点数、布尔值）
- 属性直接影响Kernel计算逻辑
- 需要在Kernel内部动态分支判断

❌ **不推荐场景**：
- 大型数组或字符串属性
- 仅影响Host侧行为的配置参数
- 可通过算子入参传递的信息

## 端到端示例：AddCustom 的 alpha 属性传递

以 AddCustom 算子（`z = x + alpha * y`）为例，展示 alpha 属性从原型定义到 Kernel 侧计算的完整传递流程。

### 原型定义：声明属性

```cpp
this->Attr("alpha")
    .AttrType(OPTIONAL)
    .Float(1.0);  // 默认值 1.0，等价于普通加法
```

### TilingData：增加属性字段

```cpp
struct AddCustomTilingData {
    uint32_t totalLength;   // 总计算数据量
    uint32_t tileNum;       // 每个Block上总计算数据分块个数
    float alpha;            // y 的缩放因子，从属性传递
};
```

### TilingFunc：获取属性并写入 TilingData

```cpp
static ge::graphStatus TilingFunc(gert::TilingContext *context)
{
    AddCustomTilingData *tiling = context->GetTilingData<AddCustomTilingData>();
    uint32_t totalLength = context->GetInputShape(0)->GetOriginShape().GetShapeSize();

    tiling->totalLength = totalLength;
    tiling->tileNum = 8;

    // 从属性获取 alpha（按名称获取，推荐方式）
    const gert::RuntimeAttrs* attrs = context->GetAttrs();
    const float* alpha = attrs->GetAttrPointer<float>("alpha");
    tiling->alpha = (alpha != nullptr) ? *alpha : 1.0f;  // 可选属性需判空

    context->SetBlockDim(8);
    size_t *currentWorkspace = context->GetWorkspaceSizes(1);
    currentWorkspace[0] = 0;

    return ge::GRAPH_SUCCESS;
}
```

### Kernel 侧：使用属性参与计算

```cpp
extern "C" __global__ __aicore__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z,
                                                  GM_ADDR workspace, GM_ADDR tiling)
{
    GET_TILING_DATA(tilingData, tiling);
    KernelAdd op;
    op.Init(x, y, z, tilingData.totalLength, tilingData.tileNum, tilingData.alpha);
    op.Process();
}
```

以下 Compute 函数为 KernelAdd 类的成员函数，`alpha` 等变量通过 Init 函数从 tilingData 赋值到类成员后使用：

```cpp
__aicore__ inline void Compute()
{
    LocalTensor<DTYPE_X> xLocal = inQueueX.DeQue<DTYPE_X>();
    LocalTensor<DTYPE_Y> yLocal = inQueueY.DeQue<DTYPE_Y>();
    LocalTensor<DTYPE_Z> zLocal = outQueueZ.AllocTensor<DTYPE_Z>();

    // Muls：标量乘法，zLocal = yLocal * alpha（alpha 为类成员变量，在 Init 中从 tilingData.alpha 赋值）
    Muls(zLocal, yLocal, alpha, totalLength);
    // z = x + z = x + alpha * y
    Add(zLocal, xLocal, zLocal, totalLength);

    outQueueZ.EnQue<DTYPE_Z>(zLocal);
    inQueueX.FreeTensor(xLocal);
    inQueueY.FreeTensor(yLocal);
}
```

完整样例请参考 [AddCustom 算子样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/custom_op/add_custom)。

## 注意事项

1. **索引顺序**：`GetAttrPointer(index)`的索引严格按照原型定义中`this->Attr()`的声明顺序排列，从0开始递增
2. **类型安全**：模板参数`T`必须与属性实际类型完全匹配，类型不匹配会导致未定义行为
3. **空指针检查**：可选属性（`OPTIONAL`）在未提供时会返回nullptr，使用前必须判空
4. **性能考量**：属性提取发生在Host侧，开销很小，但应避免在循环中重复提取

## 相关文档

- [算子原型定义](../算子原型定义.md) - 如何声明算子属性
- [Host侧Tiling实现基本流程](./基本流程.md) - Tiling函数的完整实现流程
- [Kernel侧算子实现](../Kernel侧算子实现/基本流程.md) - Kernel侧如何使用TilingData
- [编译与部署基本流程](../../编译与部署/基本流程.md) - 编译部署的具体步骤
