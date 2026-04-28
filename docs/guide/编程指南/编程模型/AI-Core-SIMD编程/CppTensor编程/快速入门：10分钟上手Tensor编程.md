# 快速入门：10分钟上手Tensor编程

本节提供三个最简单的示例，帮助你快速理解Tensor编程的基本概念。

## 最简示例1：LocalTensor原始形态

```cpp
#include "basic_api/kernel_cube_intf.h"

extern __ubuf__ half ubBuffer[256];  // UB存储区，256个half元素

__aicore__ void SimpleExample() {
    // 步骤1：创建LocalTensor（原始形态）
    // LocalTensor就像一个"窗口"，指向UB中的一段数据
    auto tensor = LocalTensor<half>((uint32_t)ubBuffer);
    tensor.SetSize(256);  // 设置大小为256个元素
    
    // 步骤2：访问元素（偏移量方式）
    // tensor(5) 表示访问第5个元素（偏移量访问）
    half value = tensor.GetValue(5);   // 读取第5个元素的值
    tensor(10) = 3.14f;                // 写入第10个元素
    
    // 步骤3：获取子Tensor
    // tensor[16] 表示从偏移16开始创建一个新的子Tensor
    auto subTensor = tensor[16];       // subTensor指向从第16个元素开始的数据
}
```

**核心概念直观理解**：

| 概念 | 直观类比 | 说明 |
|------|---------|------|
| **LocalTensor** | 数据窗口 | 一段连续数据的"视图"，不实际存储数据 |
| **偏移量访问** | 数组索引 | `tensor(5)` 就像 `array[5]`，访问第5个元素 |
| **子Tensor** | 子数组 | `tensor[16]` 创建一个从第16个元素开始的新视图 |

---

## 最简示例2：LocalTensor扩展形态（TensorAPI）

```cpp
#include "experimental/tensor_api/tensor.h"

using namespace AscendC::Te;  // TensorAPI命名空间

extern __ubuf__ half ubBuffer[256];  // UB存储区

__aicore__ void TensorAPIExample() {
    // 步骤1：创建扩展形态的LocalTensor
    // MakeTensor组合Pointer和Layout，创建带布局的Tensor
    
    // 创建UB指针
    auto ubPtr = MakeUBmemPtr<half>(ubBuffer);
    
    // 创建ND Layout（行优先，32x8形状）
    auto layout = MakeNDLayout<half>(32, 8);
    
    // 组合创建Tensor
    auto tensor = MakeTensor(ubPtr, layout);
    // tensor现在知道：数据在UB，形状32x8，行优先存储
    
    // 步骤2：访问元素（坐标方式）
    // tensor[coord] 坐标访问，Layout自动计算偏移量
    auto coord = MakeCoord(5, 3);       // 创建坐标(5, 3)
    half value = tensor[coord];          // Layout自动计算偏移，访问元素
    // 等价于：tensor内部计算偏移 = 5*8 + 3 = 43
    
    // 步骤3：简写形式（多参数）
    half value2 = tensor(5, 3);         // 等价于tensor[MakeCoord(5, 3)]
    
    // 步骤4：创建分块子Tensor
    auto tile = MakeTile(MakeShape(16, 4));  // 创建16x4分块
    auto subTensor = tensor(MakeCoord(0, 0), tile);
    // subTensor指向tensor从(0,0)开始的16x4子矩阵
}
```

**核心概念直观理解**：

| 概念 | 直观类比 | 说明 |
|------|---------|------|
| **Layout** | 数据地图 | 描述数据的形状和排列方式，自动计算索引 |
| **MakeTensor** | 组合工厂 | 组合Pointer和Layout，创建带布局的Tensor |
| **坐标访问** | 地图导航 | `tensor(5,3)` 直接用坐标，Layout自动计算偏移 |
| **Tile分块** | 地图分区 | 创建指定大小的子区域 |

---

## 最简示例3：RegTensor寄存器形态

```cpp
#include "basic_api/reg_compute/kernel_reg_compute_intf.h"

using namespace AscendC::Reg;  // RegTensor命名空间

extern __ubuf__ half ubBuffer[128];  // UB存储区，128个half元素

__aicore__ void RegTensorExample() {
    // 步骤1：创建RegTensor变量
    // RegTensor暂存数据在寄存器中，可反复使用
    RegTensor<half> regSrc;       // Reg寄存器，存储源数据
    RegTensor<half> regDst;       // Reg寄存器，存储结果
    MaskReg mask;                 // Mask寄存器（掩码）
    
    // 步骤2：创建Mask
    // Mask控制哪些元素参与计算
    mask = CreateMask<half, MaskPattern::ALL>();  // 所有元素参与
    
    // 步骤3：UB → Reg搬运
    // LoadAlign：对齐搬运，将UB数据搬运到Reg
    LoadAlign<half>(regSrc, ubBuffer);  // UB → Reg
    
    // 步骤4：在Reg中计算
    // Add：寄存器内加法，数据在Reg中可反复使用
    Add<half>(regDst, regSrc, regSrc, mask);  // regDst = regSrc + regSrc
    
    // 步骤5：Reg → UB搬运
    // StoreAlign：对齐搬运，将Reg数据搬运回UB
    StoreAlign<half>(ubBuffer, regDst, mask);  // Reg → UB
}
```

**核心概念直观理解**：

| 概念 | 直观类比 | 说明 |
|------|---------|------|
| **RegTensor** | 工作台 | 数据暂存区，可反复使用，速度最快 |
| **LoadAlign** | 上料 | 将UB数据搬运到Reg工作台 |
| **Add** | 加工 | 在Reg中计算，无需访问UB |
| **StoreAlign** | 下料 | 将Reg结果搬运回UB仓库 |
| **Mask** | 选择器 | 控制哪些元素参与计算 |

---

## 数据流动基本模式

```
GM（Global Memory，全局内存）
    │      ← 算子的输入数据来自这里
    │      DataCopy: 将数据从GM搬运到内部存储
    ▼
UB（Unified Buffer，统一缓冲区）
    │      ← Vector计算在这里进行
    │      Add/Mul/Div等向量操作
    │      或搬运到Reg进行反复计算
    ▼
输出结果 → 写回GM

完整数据流（三种形态）：

1. 原始形态：
   GM → UB（DataCopy）→ 计算（Add）→ GM（DataCopy）
   
2. 扩展形态：
   GM → UB（Copy）→ 计算（Add）→ GM（Copy）
   Layout自动管理索引和格式
   
3. Reg形态：
   GM → UB（DataCopy）→ Reg（LoadAlign）→ 反复计算（Add）→ UB（StoreAlign）→ GM
   数据在Reg中可反复使用，减少UB访问
```

**一句话总结**：Tensor编程就是"搬运数据到内部存储 → 计算 → 写回全局内存"，三种形态提供不同抽象层次。

---

## 三种形态对比总结

| 特性 | 原始形态 | 扩展形态 | Reg形态 |
|------|---------|---------|---------|
| **创建方式** | `LocalTensor<T>` | `MakeTensor(ptr, layout)` | `RegTensor<T>` |
| **访问方式** | `tensor(offset)` | `tensor[coord]` | 寄存器直接访问 |
| **索引计算** | 手动计算偏移量 | Layout自动计算 | 无需计算 |
| **特点** | 简单直接 | Layout自动管理 | 数据复用 |
| **适用场景** | 简单算子、快速开发 | 复杂算子、矩阵计算 | 反复计算、性能优化 |

**学习建议**：
- **入门**：先学原始形态，理解数据窗口和偏移量访问
- **进阶**：学扩展形态，理解Layout自动管理
- **高级**：学Reg形态，理解数据复用和性能优化
