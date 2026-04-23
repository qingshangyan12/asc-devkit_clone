LocalTensor扩展形态通过TensorAttribute特化，提供Layout抽象和Atom层封装。

### 4.1 头文件引用

```cpp
// Tensor API主入口
#include "experimental/tensor_api/tensor.h"

// 或单独引用
#include "experimental/tensor_api/algorithm/copy.h"  // Copy搬运
#include "experimental/tensor_api/algorithm/mad.h"   // Mad矩阵计算
```

### 4.2 Layout创建

**理解Layout**：Layout封装Shape（形状）和Stride（步长），自动计算坐标到偏移量的映射。

**Layout结构**：
```cpp
template <typename T, typename U>
struct Layout : private Std::tuple<T, U> {
    static constexpr auto size;   // 元素总数
    static constexpr auto depth;  // 嵌套深度（层次化布局）
    static constexpr auto rank;   // 维度数（如2维矩阵）
    
    decltype(auto) Shape();       // 获取Shape，如(128, 128)
    decltype(auto) Stride();      // 获取Stride，如(128, 1)
    auto operator()(coord);       // 核心：坐标→偏移量转换
    auto Size();                  // 元素总数
};
```

**Layout创建API示例**：
```cpp
using namespace AscendC::Te;

// === ND Layout（行优先，最常用）===
// Shape=(128, 128)，Stride自动计算为(128, 1)
auto ndLayout = MakeNDLayout<half>(128, 128);
// 用途：GM存储、UB计算、普通矩阵

// === NZ Layout（分块格式，Cube计算优化）===
// Shape=(16, 16)，按NZ格式分块存储
auto nzLayout = MakeNzLayout<half>(16, 16);
// 用途：L1存储、L0A/B输入、矩阵乘加

// === ZN Layout（列优先分块）===
auto znLayout = MakeZnLayout<half>(16, 16);
// 用途：特殊场景，列优先计算

// === L0C Layout（Cube输出格式）===
auto l0cLayout = MakeL0CLayout<float>(16, 16);
// 用途：L0C存储，矩阵乘加结果

// === 自定义Layout（手动指定Shape和Stride）===
auto shape = MakeShape(32, 32);     // 形状：32x32
auto stride = MakeStride(32, 1);    // 步长：行优先（行步长32，列步长1）
auto layout = MakeLayout(shape, stride);
```

### 4.3 格式详解：ND/NZ/ZN的区别与选择

**三种常见格式对比**：

| 格式 | 全称 | 存储方式 | 适用场景 |
|------|------|---------|---------|
| **ND** | Normal Data | 行优先连续存储 | GM输入输出、UB向量计算 |
| **NZ** | Normal Z-first | 按Z(C0)分块存储 | L1缓存、L0A/B输入、Cube计算 |
| **ZN** | Z Normal | 按N分块存储 | 特殊场景，列优先计算 |

**ND格式详解（行优先）**：
```
4x4矩阵ND格式存储：

逻辑矩阵：         内存布局（连续行优先）：
[0,0][0,1][0,2][0,3]   [0,0] [0,1] [0,2] [0,3] [1,0] [1,1] [1,2] [1,3] ...
[1,0][1,1][1,2][1,3]   ↓___________________________________________↓
[2,0][2,1][2,2][2,3]   连续16个元素，一行接一行
[3,0][3,1][3,2][3,3]

特点：
- 简单直观，符合人类习惯
- 内存连续访问，适合向量计算
- GM和UB默认使用ND格式
```

**NZ格式详解（分块存储）**：
```
4x4矩阵NZ格式存储（假设C0=2，即2x2分块）：

逻辑矩阵：         NZ分块布局：
[0,0][0,1][0,2][0,3]   分块0: [0,0][0,1]    分块2: [0,2][0,3]
[1,0][1,1][1,2][1,3]          [1,0][1,1]           [1,2][1,3]
[2,0][2,1][2,2][2,3]   分块1: [2,0][2,1]    分块3: [2,2][2,3]
[3,0][3,1][3,2][3,3]          [3,0][3,1]           [3,2][3,3]

内存顺序：分块0 → 分块1 → 分块2 → 分块3
         每个分块内2x2连续存储

特点：
- 按小分块（C0xC0）组织，如16x16分块
- 适合Cube单元的矩阵乘加硬件设计
- L1和L0A/B必须使用NZ格式
```

**为什么矩阵计算要用NZ格式？**

```
AI Core Cube单元的硬件设计：

Cube单元设计为"分块矩阵乘加"：
- 一次计算：16x16的小矩阵块乘加
- 输入要求：矩阵按16x16分块组织（NZ格式）
- 硬件优化：分块数据直接加载到L0A/B，无需额外转换

NZ格式优势：
1. 硬件匹配：Cube单元一次处理一个分块，NZ格式正好匹配
2. 访存优化：分块内连续访问，减少内存跳跃
3. 并行计算：多个分块可以并行处理

对比：
ND格式 → 每次需要从连续数据中提取16x16块 → 需要额外转换
NZ格式 → 16x16块已经组织好 → 直接加载计算 ✓
```

**格式选择指南**：

| 存储位置 | 推荐格式 | 原因 |
|---------|---------|------|
| **GM（输入输出）** | ND | 用户数据通常是行优先，无需转换 |
| **UB（向量计算）** | ND | Vector单元处理连续数据，ND格式效率高 |
| **L1（矩阵缓存）** | NZ | 为L0A/B准备数据，NZ格式匹配Cube单元 |
| **L0A/B（矩阵输入）** | NZ | Cube单元要求NZ格式输入 |
| **L0C（矩阵输出）** | L0C特殊格式 | Cube单元输出的累加结果格式 |

**格式转换时机**：

```
完整矩阵计算的数据流和格式转换：

GM（ND格式）
    │
    │ DataCopy + Nd2NzParams
    │ 自动转换：ND → NZ
    ▼
L1（NZ格式）
    │
    │ LoadData
    │ 加载16x16分块到L0A/B
    ▼
L0A/B（NZ格式）
    │
    │ Mmad
    │ 矩阵乘加计算
    ▼
L0C（L0C格式）
    │
    │ Fixpipe
    │ 自动转换：L0C → ND
    ▼
GM（ND格式）
```

**一句话总结**：
- **ND格式**：适合GM和UB，简单直观
- **NZ格式**：适合L1和L0，匹配Cube硬件设计
- **格式转换**：DataCopy和Fixpipe自动处理，无需手动干预

### 4.4 Pointer创建

**理解Pointer**：Pointer封装存储空间的指针，标记数据所在的存储层次（GM/L1/UB/L0等）。

**Pointer创建示例**：
```cpp
using namespace AscendC::Te;

// === GM Pointer（全局内存）===
__gm__ half gmData[1024];           // GM存储区声明
auto gmPtr = MakeGMmemPtr(gmData);  // 封装GM指针
// 用途：指向输入/输出数据

// === L1 Pointer（L1缓存）===
__cbuf__ half l1Data[256];          // L1存储区声明
auto l1Ptr = MakeL1memPtr(l1Data);  // 封装L1指针
// 用途：矩阵数据缓存

// === UB Pointer（统一缓冲区）===
__ubuf__ half ubData[128];          // UB存储区声明
auto ubPtr = MakeUBmemPtr(ubData);  // 封装UB指针
// 用途：向量计算

// === L0A/B/C Pointer（Cube单元）===
auto l0APtr = MakeL0AmemPtr<half>(l0AAddr);  // L0A指针
auto l0BPtr = MakeL0BmemPtr<half>(l0BAddr);  // L0B指针
auto l0CPtr = MakeL0CmemPtr<float>(l0CAddr); // L0C指针
// 用途：矩阵乘加
```

### 4.4 LocalTensor创建

**理解MakeTensor**：MakeTensor将Pointer和Layout组合，创建扩展形态的LocalTensor。

**MakeTensor示例**：
```cpp
using namespace AscendC::Te;

// === 创建UB Tensor（向量计算场景）===
// MakeTensor(指针, 布局) → LocalTensor<TensorAttribute<Engine, Layout>>
auto ubTensor = MakeTensor(
    MakeUBmemPtr<half>(ubAddr),      // UB指针：数据在哪里
    MakeNDLayout<half>(128, 128)     // ND布局：数据怎么排列
);
// ubTensor现在知道：数据在UB，形状128x128，行优先存储

// === 创建L1 Tensor（矩阵计算场景）===
auto l1Tensor = MakeTensor(
    MakeL1memPtr<half>(l1Addr),      // L1指针
    MakeNzLayout<half>(32, 32)       // NZ布局（Cube优化格式）
);
// l1Tensor用于矩阵乘加的输入缓存

// === 创建L0A Tensor（矩阵A输入）===
auto l0ATensor = MakeTensor(
    MakeL0AmemPtr<half>(l0AAddr),    // L0A指针
    MakeNzLayout<half>(16, 16)       // NZ布局
);
// l0ATensor用于Mmad的矩阵A输入

// === 创建GM Tensor（全局内存）===
auto gmTensor = MakeTensor(
    MakeGMmemPtr<half>(gmAddr),      // GM指针
    MakeNDLayout<half>(128, 128)     // ND布局
);
// gmTensor指向输入数据或输出结果
```

### 4.5 元素访问

**坐标访问示例**：
```cpp
using namespace AscendC::Te;

// 步骤1：创建一个32x32的Tensor
auto tensor = MakeTensor(
    MakeUBmemPtr<half>(ubAddr), 
    MakeNzLayout<half>(32, 32)
);

// 步骤2：通过Coord坐标访问元素
auto coord = MakeCoord(5, 10);       // 创建坐标(5, 10)
half value = tensor[coord];          // Layout自动计算偏移，访问元素
// tensor[coord]内部执行：Data()[Layout()(coord)]
// Layout()(coord)将(5,10)转为偏移量，Data()获取指针，最后访问

// 步骤3：通过多参数访问（简写形式）
half value2 = tensor(5, 10);         // 等价于tensor[MakeCoord(5, 10)]

// 步骤4：修改元素
tensor[MakeCoord(5, 10)] = 2.0f;     // 写入坐标(5,10)位置
```

**分块访问示例**：
```cpp
// 场景：提取一个16x16的子矩阵（分块计算）

// 步骤1：创建Tile描述分块大小
auto tile = MakeTile(MakeShape(16, 16));  // 16x16的分块

// 步骤2：通过坐标+Tile创建子Tensor
auto tileTensor = tensor(MakeCoord(0, 0), tile);
// tileTensor指向tensor从(0,0)开始的16x16子矩阵
// tileTensor继承了原Tensor的Layout能力

// 步骤3：访问分块元素
auto value = tileTensor(MakeCoord(0, 0));  // 访问分块内(0,0)元素
```

### 4.6 Copy搬运详解

**理解Copy**：扩展形态的数据搬运接口，封装底层DataCopy接口，通过Layout自动处理格式转换。

**Copy封装机制**：
```
扩展形态调用                底层接口调用
    │                          │
Copy(CopyGM2L1{}, dst, src)    │
    │                          ▼
    │                   DataCopy(dst, src, Nd2NzParams)
    │                   ├─ 自动配置：根据dst/src的Layout确定参数
    │                   ├─ 格式转换：ND→NZ，由Layout类型决定
    │                   └─ 参数映射：Shape/Stride→Nd2NzParams
```

**Copy通路类型详解**：
| 操作类型 | 通路 | 说明 | 对应底层接口 | 自动处理 |
|---------|------|------|-------------|---------|
| `CopyGM2L1` | GM → L1 | 全局内存到L1 | DataCopy(Nd2NzParams) | ND转NZ |
| `CopyL12L0A` | L1 → L0A | L1到L0A | LoadData | 分块加载 |
| `CopyL12L0B` | L1 → L0B | L1到L0B | LoadData | 分块加载 |
| `CopyL0C2GM` | L0C → GM | L0C输出到GM | Fixpipe | L0C转ND |
| `CopyL0C2UB` | L0C → UB | L0C输出到UB | Fixpipe | L0C转ND |

**Copy自动参数配置示例**：
```cpp
// 原始形态：手动配置参数
DataCopyParams params;
params.blockCount = 1;
params.blockLen = 128;    // 手动设置搬运128个元素
params.srcGap = 0;
params.dstGap = 0;
DataCopy(dstTensor, srcTensor, params);

// 扩展形态：自动配置参数
auto dstTensor = MakeTensor(MakeUBmemPtr<half>(ubAddr), MakeNDLayout<half>(128, 1));
auto srcTensor = MakeTensor(MakeGMmemPtr<half>(gmAddr), MakeNDLayout<half>(128, 1));
Copy(CopyGM2UB{}, dstTensor, srcTensor);
// Copy内部自动：
// 1. 从Layout获取Shape=(128,1)
// 2. 计算元素总数=128
// 3. 配置blockLen=128，blockCount=1
// 4. 调用底层DataCopy
```

### 4.7 Mad矩阵计算详解

**理解Mad**：扩展形态的矩阵乘加接口，封装底层Mmad接口。

**Mad封装机制**：
```
扩展形态调用                底层接口调用
    │                          │
Mad(MmadOperation{}, C, A, B, params)
    │                          ▼
    │                   Mmad(C, A, B, MMadParams)
    │                   ├─ 自动配置：根据Tensor的Layout确定部分参数
    │                   ├─ 格式匹配：NZ格式自动适配Cube单元
    │                   └─ 参数传递：params中的m/k/n直接使用
```

**MmadParams参数详解**：

| 参数 | 含义 | 取值范围 | 推荐值 | 说明 |
|------|------|---------|--------|------|
| `m` | 矩阵A的行数 | 1-1024 | 16 | 单次计算的分块行数 |
| `k` | 矩阵A的列数=矩阵B的行数 | 1-1024 | 16 | 矩阵乘法的中间维度 |
| `n` | 矩阵B的列数 | 1-1024 | 16 | 单次计算的分块列数 |
| `unitFlag` | 计算单元标志 | 0/1 | 0 | 0表示连续计算 |
| `cmatrixInitVal` | C矩阵初始值 | 0/其他 | 0 | 0表示从零累加，非0表示累加到现有值 |

**参数选择指南**：

```
矩阵乘加场景：计算 C = A * B

假设大矩阵：
  A: 128x128 (M=128, K=128)
  B: 128x128 (K=128, N=128)
  C: 128x128 (M=128, N=128)

分块计算策略：
  分块大小：16x16
  循环次数：128/16 = 8次（每维度）

每次Mmad参数：
  m = 16  （当前分块的行数）
  k = 16  （当前分块的中间维度）
  n = 16  （当前分块的列数）
  unitFlag = 0
  cmatrixInitVal = 0（第一次）或非0（累加后续分块）
```

**MmadParams最佳实践**：

```cpp
// === 单次16x16矩阵乘加 ===
MmadParams params;
params.m = 16;              // 固定16：Cube单元一次处理16行
params.k = 16;              // 固定16：匹配L0A/B的16x16存储
params.n = 16;              // 固定16：Cube单元一次处理16列
params.unitFlag = 0;        // 固定0：标准计算模式
params.cmatrixInitVal = 0;  // 0：从零开始累加

// === 累加多次分块（大矩阵计算）===
// 第一次：初始化C矩阵
params.cmatrixInitVal = 0;  // 从零开始
Mmad(...);                  // C = A_block0 * B_block0

// 后续累加：
params.cmatrixInitVal = 1;  // 累加到现有C值
for (int k = 16; k < K; k += 16) {
    Mmad(...);              // C += A_block_i * B_block_i
}
```

**使用示例：矩阵乘加完整流程**：
```cpp
using namespace AscendC::Te;

// 步骤1：创建L0 Tensor（矩阵乘加需要三个Tensor）
auto l0ATensor = MakeTensor(
    MakeL0AmemPtr<half>(l0AAddr),    // L0A：矩阵A输入
    MakeNzLayout<half>(16, 16)
);
auto l0BTensor = MakeTensor(
    MakeL0BmemPtr<half>(l0BAddr),    // L0B：矩阵B输入
    MakeNzLayout<half>(16, 16)
);
auto l0CTensor = MakeTensor(
    MakeL0CmemPtr<float>(l0CAddr),   // L0C：结果输出（float累加）
    MakeL0CLayout<float>(16, 16)
);

// 步骤2：配置矩阵乘加参数
MmadParams params;
params.m = 16;              // M维度：16行
params.k = 16;              // K维度：16（A的列=B的行）
params.n = 16;              // N维度：16列

// 步骤3：执行矩阵乘加：Mad自动调用底层Mmad
// 计算：l0CTensor = l0ATensor * l0BTensor（16x16矩阵乘法）
Mad(MmadOperation{}, l0CTensor, l0ATensor, l0BTensor, params);

// 步骤4（可选）：带Bias的矩阵乘加
auto biasTensor = MakeTensor(
    MakeBiasmemPtr<half>(biasAddr),  // Bias存储
    MakeLayout(MakeShape(16))        // 一维Bias向量
);
// 计算：l0CTensor = l0ATensor * l0BTensor + biasTensor
Mad(MmadOperation{}, l0CTensor, l0ATensor, l0BTensor, biasTensor, params);
```

---
