RegTensor是Vector计算单元内部的寄存器数据抽象，用于在Reg寄存器中暂存数据，支持反复计算。

# 什么是RegTensor？

**RegTensor定义**：Vector计算单元内部的矢量数据寄存器，用于暂存计算数据。

**直观理解**：
```
RegTensor = Vector计算单元内部的"工作区"

类比：
UB（LocalTensor） = 仓库（存储区，容量大，访问慢）
Reg（RegTensor） = 工作台（计算区，容量小，访问快）
数据需要从仓库搬运到工作台，计算后搬回仓库
```

**与LocalTensor的区别**：

| 特性 | LocalTensor | RegTensor |
|------|------------|-----------|
| **存储位置** | UB/L1/L0存储区 | Vector单元寄存器 |
| **容量** | 大（UB 256KB） | 小（单次迭代128元素） |
| **访问速度** | 中（需要从UB读取） | 快（寄存器直接访问） |
| **数据持久性** | 持久存储 | 暂存，计算后需搬回UB |
| **适用场景** | 数据存储、搬运 | 反复计算、性能优化 |

# RegTensor结构定义

**头文件引用**：
```cpp
// Reg计算主入口
#include "basic_api/reg_compute/kernel_reg_compute_intf.h"

// 或单独引用
#include "basic_api/reg_compute/kernel_reg_compute_struct_intf.h"  // RegTensor定义
#include "basic_api/reg_compute/kernel_reg_compute_datacopy_intf.h" // 数据搬运
#include "basic_api/reg_compute/kernel_reg_compute_vec_binary_intf.h" // 二元运算
#include "basic_api/reg_compute/kernel_reg_compute_vec_unary_intf.h"  // 单目运算
#include "basic_api/reg_compute/kernel_reg_compute_vec_reduce_intf.h" // 归约运算
#include "basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"    // Mask操作
```

**RegTensor模板定义**：
```cpp
namespace AscendC {
namespace Reg {

// RegTrait：寄存器特征，控制寄存器数量
struct RegTrait {
    int REG_NUM = 1;  // 寄存器数量
};

constexpr RegTrait RegTraitNumOne = {1};   // 单寄存器
constexpr RegTrait RegTraitNumTwo = {2};   // 双寄存器

// RegTensor：寄存器Tensor模板
template <typename T, const RegTrait& regTrait = RegTraitNumOne>
struct RegTensor {
    __simd_callee__ inline RegTensor() {};  // 构造函数
    
    using ActualT = T;                      // 实际数据类型
    static constexpr RegTrait trait = regTrait;  // 寄存器特征
    static constexpr int REG_NUM = trait.REG_NUM;  // 寄存器数量
    using RegType = typename TypeGet<T>::T;  // 寄存器类型
    RegType reg[trait.REG_NUM];             // 寄存器数组
    
    __simd_callee__ inline operator RegType& () {
        return reg[0];  // 类型转换，获取第一个寄存器
    }
    
    __simd_callee__ void Print() const;     // 打印函数
};

} // namespace Reg
} // namespace AscendC
```

**常用类型别名**：
```cpp
namespace AscendC {
namespace Reg {

using MaskReg = vector_bool;        // Mask寄存器（掩码）
using AddrReg = vector_address;     // 地址寄存器
using UnalignReg = vector_align;    // 非对齐寄存器
using UnalignRegForLoad = vector_align;   // 非对齐加载寄存器
using UnalignRegForStore = vector_align;  // 非对齐存储寄存器

} // namespace Reg
} // namespace AscendC
```

# RegTensor数据类型

**RegTensor支持的类型映射**：

| 用户类型 | 寄存器类型 | 说明 |
|---------|-----------|------|
| `half` | `vector_half` | 16bit浮点，128元素 |
| `float` | `vector_float` | 32bit浮点，64元素 |
| `int8_t` | `vector_int8_t` | 8bit整数，256元素 |
| `int16_t` | `vector_int16_t` | 16bit整数，128元素 |
| `int32_t` | `vector_int32_t` | 32bit整数，64元素 |
| `bfloat16_t` | `vector_bfloat16_t` | 16bit脑浮点，128元素 |

**使用示例**：
```cpp
using namespace AscendC::Reg;

// 定义RegTensor变量
RegTensor<half> regData;       // half类型RegTensor（单寄存器）
RegTensor<float, RegTraitNumTwo> regData2;  // float类型，双寄存器

// 定义Mask寄存器
MaskReg mask;                  // Mask寄存器（掩码）

// 定义地址寄存器
AddrReg addrReg;               // 地址寄存器

// 访问寄存器
regData.reg[0]  // 访问第一个寄存器（vector_half类型）
regData2.reg[0]  // 访问第一个寄存器
regData2.reg[1]  // 访问第二个寄存器
```

# 数据搬运接口

**LoadAlign（UB → Reg，对齐搬运）**：
```cpp
template <typename T, LoadDist dist, typename U>
__simd_callee__ inline void LoadAlign(U& dstReg, __ubuf__ T* srcAddr);
```

**使用示例**：
```cpp
using namespace AscendC::Reg;

// 步骤1：定义UB存储和RegTensor
__ubuf__ half ubData[128];     // UB存储，128个half元素
RegTensor<half> regData;       // RegTensor
MaskReg mask = CreateMask<half, MaskPattern::ALL>();  // 创建mask

// 步骤2：UB → Reg搬运（对齐）
// LoadAlign：对齐搬运，UB地址必须32Byte对齐
LoadAlign<half>(regData, ubData);  // UB → Reg

// 步骤3：在Reg中计算
Add<half>(regData, regData, regData, mask);  // Reg中加法

// 步骤4：Reg → UB搬运（对齐）
// StoreAlign：对齐搬运，UB地址必须32Byte对齐
StoreAlign<half>(ubData, regData, mask);  // Reg → UB
```

**LoadUnAlign（UB → Reg，非对齐搬运）**：
```cpp
// 非对齐搬运：处理UB地址不满足32Byte对齐的情况
template <typename T>
__simd_callee__ inline void LoadUnAlignPre(UnalignRegForLoad& ureg, __ubuf__ T* srcAddr);

template <typename T, typename U>
__simd_callee__ inline void LoadUnAlign(U& dstReg, UnalignRegForLoad& ureg, __ubuf__ T* srcAddr);
```

**使用示例（非对齐搬运）**：
```cpp
using namespace AscendC::Reg;

// 场景：UB起始地址不满足32Byte对齐
__ubuf__ half ubBuffer[256];
half* ubSrc = ubBuffer + 5;    // 从第5个元素开始，可能不对齐

RegTensor<half> regData;
MaskReg mask = CreateMask<half, MaskPattern::ALL>();
UnalignRegForLoad ureg;        // 非对齐加载寄存器

// 非对齐搬运流程：
// 步骤1：准备非对齐寄存器
LoadUnAlignPre<half>(ureg, ubSrc);

// 步骤2：非对齐加载
LoadUnAlign<half>(regData, ureg, ubSrc);  // UB → Reg（非对齐）

// 步骤3：计算
Add<half>(regData, regData, regData, mask);

// 步骤4：非对齐存储（需要UnalignRegForStore）
UnalignRegForStore uregStore;
StoreUnAlign<half>(ubSrc, regData, uregStore);  // Reg → UB（非对齐）
```

**StoreAlign（Reg → UB，对齐搬运）**：
```cpp
template <typename T, StoreDist dist, typename U>
__simd_callee__ inline void StoreAlign(__ubuf__ T* dstAddr, U& srcReg, MaskReg& mask);
```

# Vector计算接口

**二元运算接口**：

| 接口 | 功能 | 示例 |
|------|------|------|
| `Add` | 加法 | `Add<half>(dst, src0, src1, mask)` |
| `Sub` | 减法 | `Sub<half>(dst, src0, src1, mask)` |
| `Mul` | 乘法 | `Mul<half>(dst, src0, src1, mask)` |
| `Div` | 除法 | `Div<half>(dst, src0, src1, mask)` |
| `Max` | 最大值 | `Max<half>(dst, src0, src1, mask)` |
| `Min` | 最小值 | `Min<half>(dst, src0, src1, mask)` |
| `And` | 按位与 | `And<half>(dst, src0, src1, mask)` |
| `Or` | 按位或 | `Or<half>(dst, src0, src1, mask)` |
| `Xor` | 按位异或 | `Xor<half>(dst, src0, src1, mask)` |

**单目运算接口**：

| 接口 | 功能 | 示例 |
|------|------|------|
| `Abs` | 绝对值 | `Abs<half>(dst, src, mask)` |
| `Relu` | ReLU激活 | `Relu<half>(dst, src, mask)` |
| `Exp` | 指数 | `Exp<half>(dst, src, mask)` |
| `Sqrt` | 平方根 | `Sqrt<half>(dst, src, mask)` |
| `Ln` | 自然对数 | `Ln<half>(dst, src, mask)` |
| `Log` | 对数 | `Log<half>(dst, src, mask)` |
| `Neg` | 取负 | `Neg<half>(dst, src, mask)` |
| `Not` | 按位取反 | `Not<half>(dst, src, mask)` |

**归约运算接口**：

| 接口 | 功能 | 示例 |
|------|------|------|
| `Reduce` | 通用归约 | `Reduce<ReduceType::SUM, half>(dst, src, mask)` |
| `ReduceDataBlock` | DataBlock内归约 | `ReduceDataBlock<ReduceType::SUM, half>(dst, src, mask)` |

**归约类型**：
```cpp
enum class ReduceType {
    SUM,    // 求和
    MAX,    // 最大值
    MIN,    // 最小值
    // ...
};
```

# Mask操作接口

**CreateMask（创建Mask）**：
```cpp
template <typename T, MaskPattern mode, const RegTrait& regTrait>
__simd_callee__ inline MaskReg CreateMask();
```

**Mask模式**：
```cpp
enum class MaskPattern {
    ALL,     // 全1，所有元素参与
    VL1,     // 只有最低1个元素
    VL2,     // 只有最低2个元素
    VL4,     // 只有最低4个元素
    VL8,     // 只有最低8个元素
    VL16,    // 只有最低16个元素
    H,       // 最低一半元素
    Q,       // 最低四分之一元素
    // ...
};
```

**使用示例**：
```cpp
using namespace AscendC::Reg;

// 创建全1 Mask（所有元素参与）
MaskReg maskAll = CreateMask<half, MaskPattern::ALL>();

// 创建只有前8个元素参与的Mask
MaskReg mask8 = CreateMask<half, MaskPattern::VL8>();

// 使用Mask进行计算
RegTensor<half> regDst, regSrc0, regSrc1;
Add<half>(regDst, regSrc0, regSrc1, maskAll);  // 所有元素参与
Add<half>(regDst, regSrc0, regSrc1, mask8);    // 只有前8元素参与
```

**Mask合并模式**：
```cpp
enum class MaskMergeMode {
    ZEROING,  // 不参与元素置零
    KEEPING,  // 不参与元素保持原值
};
```

# RegTensor使用场景

**场景1：反复计算同一数据**

```cpp
using namespace AscendC::Reg;

// 场景：对同一数据执行多次不同计算
// 优势：数据在Reg中可反复使用，减少UB访问

__ubuf__ half ubSrc[128];
__ubuf__ half ubDstAdd[128], ubDstMul[128], ubDstRelu[128];

RegTensor<half> regSrc, regDstAdd, regDstMul, regDstRelu;
MaskReg mask = CreateMask<half, MaskPattern::ALL>();

// 只搬运一次到Reg
LoadAlign<half>(regSrc, ubSrc);  // UB → Reg（一次）

// 在Reg中反复计算（无需再次搬运）
Add<half>(regDstAdd, regSrc, regSrc, mask);     // 第一次计算
Mul<half>(regDstMul, regSrc, regSrc, mask);     // 第二次计算
Relu<half>(regDstRelu, regSrc, mask);           // 第三次计算

// 一次性搬运回UB
StoreAlign<half>(ubDstAdd, regDstAdd, mask);
StoreAlign<half>(ubDstMul, regDstMul, mask);
StoreAlign<half>(ubDstRelu, regDstRelu, mask);

// 优势：减少了UB访问次数，性能提升
```

**场景2：非对齐地址访问**

```cpp
using namespace AscendC::Reg;

// 场景：UB地址不满足32Byte对齐
// 优势：RegTensor支持非对齐搬运

__ubuf__ half ubBuffer[256];
half* ubSrc = ubBuffer + 5;  // 从第5个元素开始，可能不对齐

RegTensor<half> regData;
MaskReg mask = CreateMask<half, MaskPattern::ALL>();
UnalignRegForLoad ureg;
UnalignRegForStore uregStore;

// 非对齐搬运流程
LoadUnAlignPre<half>(ureg, ubSrc);
LoadUnAlign<half>(regData, ureg, ubSrc);  // 非对齐搬运

// 计算
Add<half>(regData, regData, regData, mask);

// 非对齐存储
StoreUnAlign<half>(ubSrc, regData, uregStore);
```

**场景3：归约计算**

```cpp
using namespace AscendC::Reg;

// 场景：向量求和、最大值、最小值等归约操作
// 优势：RegTensor支持归约接口

__ubuf__ half ubData[128];
RegTensor<half> regData, regResult;
MaskReg mask = CreateMask<half, MaskPattern::ALL>();

// UB → Reg
LoadAlign<half>(regData, ubData);

// 归约求和
Reduce<ReduceType::SUM, half>(regResult, regData, mask);

// 归约最大值
Reduce<ReduceType::MAX, half>(regResult, regData, mask);

// 归约最小值
Reduce<ReduceType::MIN, half>(regResult, regData, mask);
```

# RegTensor与LocalTensor的关系

**数据流对比**：

```
LocalTensor计算流程：
UB（LocalTensor） → Vector计算单元 → UB（LocalTensor）
    ↓ 直接访问UB         ↓ 计算          ↓ 直接写入UB
    
特点：每次计算都需要访问UB

RegTensor计算流程：
UB（LocalTensor） → Reg（RegTensor） → Vector计算单元 → Reg（RegTensor） → UB（LocalTensor）
    ↓ LoadAlign搬运      ↓ 搬运到Reg     ↓ 计算          ↓ 在Reg中暂存     ↓ StoreAlign搬运
    
特点：数据在Reg中可反复使用，减少UB访问
```

**使用建议**：

| 场景 | 推荐使用 | 原因 |
|------|---------|------|
| **单次简单计算** | LocalTensor | 无需搬运，直接计算，效率高 |
| **连续数据处理** | LocalTensor | UB直接访问，简单直接 |
| **反复计算同一数据** | RegTensor | 数据在Reg中复用，减少UB访问 |
| **多步计算同一数据** | RegTensor | 避免重复搬运，性能提升 |
| **非对齐地址访问** | RegTensor | 支持非对齐搬运 |
| **归约操作** | RegTensor | RegTensor支持归约接口 |

---
