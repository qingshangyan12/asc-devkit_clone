## 1. VF函数优化手段详解

### 1.1 优化一：VF循环优化（optimize_vf_loop）

#### 1.1.1 优化背景

VF函数内部的核心结构是一个循环，每次迭代处理一个VL长度的数据。VF函数中可以包含最多四层嵌套循环，每层循环中还可以包含多个串行循环，同时支持非循环的向量操作和标量操作。VF循环对控制结构的支持有限，仅支持for循环和条件判断，不支持switch、do-while和while-do等其他控制结构。

VF循环通过尽可能优化为硬件级向量循环（Hardware Loop），从而实现性能优化。当VF函数中的循环满足Hardware Loop编码规范时，会被编译器优化为Hardware Loop，提升整体的编码性能；否则它的循环逻辑会由迭代变量和条件判断语句构成Software Loop，无法使能VF循环优化。

#### 1.1.2 Hardware Loop编码规范

为了能让编译器识别并生成Hardware Loop，对应的Loop代码必须符合硬件设计的要求。具体规范如下：

**1) 迭代变量类型**

VF内所有Loop的迭代变量必须是`uint16_t`类型。

**2) 起始值与步长**

- 循环起始值从0开始
- 每次迭代的步长必须是递增1

**3) 循环内不允许跳转指令**

VF内if/else在Loop内会阻碍Hardware Loop的生成，编译器虽然会尽可能做if/else消除优化，但是不做完全性保证。

- `if constexpr`在编译时已经完成，无运行时开销，但需要传入的参数是编译时常量，不能依赖运行时变量
- `for(1)`可以触发编译器的循环优化，在当前硬件条件下，Vector侧执行Loop的性能远远优于条件分支跳转

```cpp
//【反例】使用if语句
uint16_t tailK = srcK % floatRepSize;
uint16_t hasTail = 0;
hasTail = !!tailK;
if(tailK > 0){
  // 尾块处理内容
}

//【正例】使用for(1)来替代if判断语句
uint16_t tailK = srcK % floatRepSize;
uint16_t hasTail = 0;
hasTail = !!tailK;
for(uint16_t i = 0 ; i < hasTail ; i++){
  // 尾块处理内容
}
```

**4) 循环计数/边界不允许被更改**

一旦执行，循环计数/边界不允许被更改。若要利用外层循环的计数作为循环边界，将外层循环计数器移动到另一个寄存器中，然后将其设置为循环边界。

```cpp
//【正例】被编译器优化为Hardware Loop
for (uint16_t i = 0; i < LoopBound; i++) {
    for (uint16_t j = 0; j < LoopBound; j++) {
        for (uint16_t k = 0; k < LoopBound; k++) {
            for (uint16_t m = 0; m < LoopBound; m++) {
              // ...
            }
        }
    }
}

//【反例】无法被编译器优化，构成Software Loop
for (uint16_t i = 0; i < LoopBound; i++) {     // Software Loop，循环内包含了if判断
    if(){ }
}

for (uint16_t i = 2; i < LoopBound*3; i+=2){   // Software Loop，起始值不为0，步长不为1
    // ...
}
```

#### 1.1.3 样例数据计算详解

以 `optimize_vf_loop.asc` 为例，详细分析数据是如何被计算处理的。

**1) 样例目标**

从 `gen_data.py` 可以确认样例的计算目标：

```python
src = np.ones([1024]).astype(np.float32)
dst = np.ones([1024]).astype(np.float32)
dst = dst + 1  # golden = src + 1
```

即：**对1024个float元素执行加1操作**，每个输入元素值为1.0，输出应为2.0。

**2) VL与MaskPattern::VL8的含义**

VL（Vector Length）是向量寄存器的宽度，对于当前架构 `VECTOR_REG_WIDTH = 256` 字节。对于float类型（4字节），一个VL可承载64个float元素。

`MaskPattern` 枚举定义如下（来源于 `include/c_api/reg_compute/reg_vector.h`）：

```
PAT_ALL,   // 所有元素置True
PAT_VL1,   // 最低1个元素
PAT_VL2,   // 最低2个元素
PAT_VL4,   // 最低4个元素
PAT_VL8,   // 最低8个元素
PAT_VL16,  // 最低16个元素
PAT_VL32,  // 最低32个元素
PAT_VL64,  // 最低64个元素
PAT_H,     // 最低半数元素
PAT_Q,     // 最低四分之一元素
```

官方文档（`docs/api/context/MaskReg.md`）说明：**VL8 = 最低8个元素**，元素粒度由模板参数T决定。对于 `CreateMask<float, MaskPattern::VL8>()`，VL8选中最低8个float元素。

MaskReg的宽度为VL/8（`docs/guide/编程指南/硬件实现/架构规格/NPU架构版本351x.md`），对于b32类型，每4个bit位对应1个float元素（`docs/api/context/MoveMask.md`），因此256bit的MaskReg可表示64个float的掩码。

各MaskPattern对应的float元素数：

| MaskPattern | 含义 | float元素数 | 占VL比例 |
|-------------|------|------------|---------|
| VL8 | 最低8个元素 | 8 | 1/8 |
| VL16 | 最低16个元素 | 16 | 1/4 |
| VL32 | 最低32个元素 | 32 | 1/2 |
| VL64 | 最低64个元素 | 64（=完整VL） | 1 |
| H | 最低半数元素 | 32 | 1/2 |
| ALL | 全部元素 | 64 | 1 |

**3) StoreAlign与Mask的关系**

官方文档（`docs/guide/编程指南/编程模型/AI-Core-SIMD编程/SIMD编程/Reg矢量计算编程.md`）对StoreAlign的说明：

> StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask)表示将dstReg搬出到LocalTensor，目标地址是dstAddr + i * oneRepeatSize，**mask表示有多少元素参与搬出**。

因此，当mask=VL8时，`StoreAlign` 实际只写出8个float（32字节）到UB，而不是整个256字节的RegTensor。同理，`Adds` 在mask控制下也只对8个float元素执行计算。

**4) AddVF中的偏移计算与数据流**

AddVF使用四层嵌套循环，每层迭代4次，stride参数如下：

```cpp
constexpr uint16_t loop = 4;
constexpr uint16_t stride1 = 512;          // 最外层i的步长
constexpr uint16_t stride2 = stride1 / loop; // = 128，第2层j的步长
constexpr uint16_t stride3 = stride2 / loop; // = 32，第3层k的步长
constexpr uint16_t stride4 = stride3 / loop; // = 8，最内层m的步长
```

每次迭代的地址偏移（以float元素为单位）：

```
offset = i × stride1 + j × stride2 + k × stride3 + m × stride4
       = i × 512  + j × 128  + k × 32   + m × 8
```

stride4=8个float = 32字节 = 1个32B对齐块，而VL8恰好选中8个float = 32字节。**stride4的步长 = Mask选中的元素数**，确保每次迭代恰好处理1个32B对齐块，各迭代之间无重叠。

**5) 一次迭代的完整计算流程**

以 `(i=0, j=0, k=0, m=0)` 为例：

```
步骤1: LoadAlign(srcReg, srcAddr + 0)
       → 从srcAddr[0]加载64个float到srcReg（LoadAlign加载完整VL=256字节）

步骤2: Adds(dstReg, srcReg, 1.0f, mask)
       → mask=VL8，仅对srcReg[0..7]执行加1.0
       → dstReg[0..7] = srcReg[0..7] + 1.0f = 2.0f
       → dstReg[8..63] 不参与计算（ZEROING模式下置0）

步骤3: StoreAlign(dstAddr + 0, dstReg, mask)
       → mask控制搬出，仅将dstReg[0..7]写回dstAddr[0..7]（32字节）
```

以 `(i=0, j=0, k=0, m=1)` 为例：

```
offset = 0 + 0 + 0 + 1×8 = 8
→ LoadAlign从srcAddr[8]加载64个float
→ Adds对srcReg[0..7]加1.0（对应UB中srcAddr[8..15]的数据）
→ StoreAlign仅写回dstAddr[8..15]（32字节）
```

注意：LoadAlign总是加载完整VL（256字节），此时srcReg[0..7]对应的是UB中从 `srcAddr+offset` 开始的8个float，即srcAddr[8..15]。

**6) 四层循环如何覆盖1024个元素**

各层循环的覆盖范围逐步扩大：

| 循环层 | 变量 | 范围 | 覆盖float数 | 说明 |
|--------|------|------|-----------|------|
| 最内层 | m: 0→3 | offset += 0,8,16,24 | 4×8 = 32 | 连续4个32B块 |
| 第3层 | k: 0→3 | offset += 0,32,64,96 | 4×32 = 128 | |
| 第2层 | j: 0→3 | offset += 0,128,256,384 | 4×128 = 512 | |
| 最外层 | i: 0→3 | offset += 0,512,1024,1536 | 4×512 = 2048 | |

四层循环共 4×4×4×4 = **256次迭代**，每次处理8个float，理论上覆盖 **2048个float**。

但实际数据只有1024个float。分析可知：
- **i=0**：覆盖 srcAddr[0..511]，共512个float — 有效
- **i=1**：覆盖 srcAddr[512..1023]，共512个float — 有效
- **i=2**：覆盖 srcAddr[1024..1535] — 超出UB分配范围
- **i=3**：覆盖 srcAddr[1536..2047] — 超出UB分配范围

因此，只有i=0和i=1的迭代（共128次）真正处理了有效的1024个float数据，完成了 `dst[i] = src[i] + 1.0` 的计算。i=2和i=3的迭代访问了超出UB分配的地址空间。

> **说明**：本样例的重点是展示VF函数的四维地址管理模式（stride层级与AddrReg优化），而非精确匹配数据量。在实际开发中，应确保loop参数与数据量匹配，避免越界访问。

**7) 四维偏移的连续性**

当stride层级满足 `stride4 = stride3/loop`、`stride3 = stride2/loop`、`stride2 = stride1/loop` 时，四层循环按字典序遍历等效于连续地址访问：

```
(i,j,k,m)按字典序的偏移序列：
0, 8, 16, 24, 32, 40, 48, 56, 64, ..., 504, 512, 520, ..., 1023×8, ...
→ 形成步长为8个float（32字节）的连续访问模式
```

这种连续的访问模式正是编译器能识别并优化为AddrReg地址寄存器的关键。

**8) 为什么用固定Mask而非UpdateMask**

本样例使用 `CreateMask<T, MaskPattern::VL8>()` 固定Mask，而非 `UpdateMask<T>(count)` 动态Mask，原因是：
- 固定Mask在循环外创建一次，循环内不再更新，减少指令数
- 动态Mask（`UpdateMask`）在每次循环迭代中都要调用，用于处理末尾不足一个VL的数据
- 当每个迭代处理的元素数固定（本样例固定8个float=1个32B块）时，固定Mask更高效

#### 1.1.4 优化手段

本样例通过以下手段优化VF循环：

**1) 循环内地址管理优化**

在VF循环中，当使用搬运指令时，需要计算搬入和搬出的地址偏移量，由此会引入较多的标量计算开销。通过引入地址寄存器（AddrReg），可有效优化地址偏移量的计算。地址寄存器最多支持4层循环寻址，当满足地址寄存器生成模式时，编译器有机会生成地址寄存器，从而消除相关的Scalar计算消耗，提升整体性能。

使用多维嵌套循环+stride参数替代单层循环+偏移计算：

```cpp
// optimize_vf_loop 样例
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* srcAddr, __ubuf__ T* dstAddr,
                               uint16_t stride1, uint16_t stride2,
                               uint16_t stride3, uint16_t stride4, uint16_t loop)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::VL8>();
    for (uint16_t i = 0; i < loop; i++) {
        for (uint16_t j = 0; j < loop; j++) {
            for (uint16_t k = 0; k < loop; k++) {
                for (uint16_t m = 0; m < loop; m++) {
                    // 使用stride层级管理地址，替代手动计算偏移
                    AscendC::Reg::LoadAlign(srcReg,
                        srcAddr + i * stride1 + j * stride2 + k * stride3 + m * stride4);
                    AscendC::Reg::Adds(dstReg, srcReg, 1.0f, mask);
                    AscendC::Reg::StoreAlign(
                        dstAddr + i * stride1 + j * stride2 + k * stride3 + m * stride4,
                        dstReg, mask);
                }
            }
        }
    }
}
```

调用端配置stride层级关系（stride1 > stride2 > stride3 > stride4）：

```cpp
constexpr uint16_t loop = 4;
constexpr uint16_t stride1 = 512;        // 最外层stride
constexpr uint16_t stride2 = stride1 / loop;  // 128
constexpr uint16_t stride3 = stride2 / loop;  // 32
constexpr uint16_t stride4 = stride3 / loop;  // 8
AscendC::VF_CALL<AddVF<T>>(srcAddr, dstAddr, stride1, stride2, stride3, stride4, loop);
```

**编译器地址寄存器优化原理**：当四层循环按照上述模式寻址时，编译器会将地址计算优化为使用AddrReg进行地址管理：

```cpp
// 编译器优化后的等效代码
AscendC::Reg::AddrReg aReg;
for(uint16_t i = 0; i < extent1; i++){
    for(uint16_t j = 0; j < extent2; j++){
        for(uint16_t k = 0; k < extent3; k++){
            for(uint16_t m = 0; m < extent4; m++){
                aReg = AscendC::Reg::CreateAddrReg(i, const1, j, const2, k, const3, m, const4);
                AscendC::Reg::LoadAlign(srcReg, srcAddr, aReg);
            }
        }
    }
}
```

编译器通过识别`LoadAlign`、`StoreAlign`等指令进行模式匹配，当代码结构满足特定优化模式时，将有机会进行高效优化，从而获得最佳的性能收益。相反，若直接使用`AddrReg`存储偏移量或其它底层接口，可能会限制编译器的全局优化能力。

**特别地**，当四层循环构成连续访问场景时，地址管理可被简化为一维模式，从而实现更高效的搬运优化：

```cpp
// 使能向量地址生成指令优化——连续访问简化为一维模式
__simd_vf__ inline void ComputeModeVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr,
                                       uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T>();
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        AscendC::Reg::LoadAlign(dstReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::StoreAlign(dstAddr, dstReg, i * oneRepeatSize, mask);
    }
}
```

**2) 循环内指令分布优化**

通过使用固定Mask（`CreateMask<T, MaskPattern::VL8>()`）替代动态Mask（`UpdateMask`），减少了循环内的Mask更新开销。当数据量恰好是VL的整数倍时，可以使用固定Mask。

此外，减少Loop循环内非索引相关语句。for循环中存在与索引无关的语句可以提出for循环外来减少指令数：

```cpp
//【反例】Duplicate语句放在for循环中，每次循环都会执行一次
template<typename T>
__simd_vf__ inline void DuplicateVF(__ubuf__ T* dstAddr, T scalarValue,
                                     uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T>();
    for (uint16_t i = 0; i < repeatTimes; i++) {
        AscendC::Reg::Duplicate(dstReg, scalarValue);      // 每次循环都执行
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}

//【正例】Duplicate放在for循环外，仅执行一次，有效减少指令数
template<typename T>
__simd_vf__ inline void DuplicateVF(__ubuf__ T* dstAddr, T scalarValue,
                                     uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T>();
    AscendC::Reg::Duplicate(dstReg, scalarValue);          // 循环外仅执行一次
    for (uint16_t i = 0; i < repeatTimes; i++) {
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}
```

**3) 循环内成员变量访问优化**

在一个VF内，不推荐直接访问类的成员变量。直接访问类的成员变量相当于从栈上把内容搬运到Tensor寄存器中，之后通过地址访问Tensor上的内容，该操作将导致VF融合失效，建议通过局部变量临时传参解决：

```cpp
//【反例】直接读取成员变量
__aicore__ inline void SoftMaxGenericNDImpVF(__ubuf__ float* dstAddr, __ubuf__ float* sumAddr,
    __ubuf__ float* maxAddr, __ubuf__ float* srcAddr, __ubuf__ float* workAddr,
    const LastAxisShapeND originalSrcShape, const SoftMaxTiling tiling)
{
    for (uint16_t i = 0; i < (uint16_t)tiling.srcM; i++) {
        AscendC::ReduceMax(maxAddr + i * FLOAT_NUM_PER_BLK,
                           srcAddr + i * tiling.srcK, workAddr,
                           (uint16_t)originalSrcShape.k);
    }
}

//【正例】通过局部变量传递VF内访问的成员变量
__aicore__ inline void SoftMaxGenericNDImpVF(__ubuf__ float* dstAddr, __ubuf__ float* sumAddr,
    __ubuf__ float* maxAddr, __ubuf__ float* srcAddr, __ubuf__ float* workAddr,
    const LastAxisShapeND originalSrcShape, const SoftMaxTiling tiling)
{
    uint16_t srcK = tiling.srcK;
    uint16_t srcM = tiling.srcM;
    uint16_t reduceK = FLOAT_NUM_PER_BLK;
    uint16_t originK = (uint16_t)originalSrcShape.k;
    for (uint16_t i = 0; i < (uint16_t)srcM; i++) {
        AscendC::ReduceMax(maxAddr + i * reduceK, srcAddr + i * srcK, workAddr, originK);
    }
}
```

#### 1.1.5 为什么能优化

- **Hardware Loop vs Software Loop**：Hardware Loop由硬件直接管理循环计数和跳转，无额外标量计算开销；Software Loop需要软件维护迭代变量和条件判断，开销较大
- 多层嵌套循环让编译器更容易识别数据访问模式，生成地址寄存器，消除标量计算消耗
- 层级化的stride计算减少运行时地址计算复杂度
- 固定Mask避免了每次循环的Mask更新指令
- 与索引无关的语句提到循环外，减少循环内指令数
- 局部变量替代成员变量访问，避免VF融合失效

---

### 1.2 优化二：VF指令双发优化（optimize_vf_dual_instr）

#### 1.2.1 优化背景

昇腾AI处理器的Vector计算单元支持**指令双发**（Dual Issue），即同时发射两条指令到不同的执行端口。指令双发需要满足以下两个条件：

1. **两条指令之间没有数据上的依赖关系**（依赖关系指后一条指令需要使用前一条指令产生的结果）
2. **硬件中拥有足够的执行资源**

这种机制可以在不改变程序逻辑的前提下，提升处理器在单位时间内的指令处理效率，是实现指令级并行的重要基础之一。

#### 1.2.2 问题场景

计算 `z = x + y + 60`，其中 `x` 和 `y` 是长度为2048的向量：

```cpp
// 单个VF循环中：x + y 的结果立即被后续 Adds 使用
// dstReg0 依赖前一条 Add 的输出 → 无法双发
AscendC::Reg::Add(dstReg0, srcReg0, srcReg1, mask);    // Add
AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);        // 依赖dstReg0 → 串行
AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);        // 依赖dstReg0 → 串行
AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);        // 依赖dstReg0 → 串行
```

以下示例中，VLoop-1循环16次，由于每个循环内的4条指令有数据依赖，所以执行队列的深度是64。64条指令并发执行时，LoadAlign_0和LoadAlign_1没有依赖关系，可以并发执行。但后续指令由于数据依赖关系，无法充分使能双发：

```cpp
for (uint16_t i = 0; i < 16; ++i) { // VLoop-1
    // 循环16次，每次4条指令
    // 数据依赖性：Adds依赖LoadAlign，Mul依赖Adds ...
    mask = AscendC::Reg::UpdateMask<T>(count);
    int16_t scalar = 2;
    AscendC::Reg::LoadAlign(srcReg, src0Addr + i * oneRepeatSize);
    AscendC::Reg::Adds(dstReg1, srcReg, scalar, mask);
    AscendC::Reg::Mul(dstReg2, dstReg1, srcReg, mask);
    AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg2, mask);
}
```

在编写算子时，开发者通常习惯于按"加载数据 → 处理计算 → 存储结果"的顺序来组织代码流程。这种写法在寄存器资源充足时运行良好，但一旦资源紧张，问题就会被放大。当多个计算指令之间会产生依赖关系，这些等待会堆积在执行队列中，导致后续指令无法及时发射。开发者在编程时应尽可能保证队列里存在数目充足且无依赖的并发指令，从而高效地利用硬件双发特性。

#### 1.2.3 优化方案

**方案一：合理拆分VF循环**

VF并不是写的越长、把所有运算都放在一个for循环内就好，需要适当的搬出中间结果到UB，减少数据依赖。

从 `gen_data.py` 可知，本样例的计算目标是 `z = x + y + 60`（2048个float）。如果不拆分，将7次标量加法全部放在一个循环中，会形成7级串行依赖链：

```cpp
// 不拆分：7级串行依赖链，每条指令必须等前一条完成
for (uint16_t i = 0; i < repeatTimes; ++i) {
    LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
    LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
    Add(dstReg0, srcReg0, srcReg1, mask);    // dstReg0 = x + y
    Adds(dstReg0, dstReg0, 10, mask);        // dstReg0依赖上一条 → 串行
    Adds(dstReg0, dstReg0, 10, mask);        // dstReg0依赖上一条 → 串行
    Adds(dstReg0, dstReg0, 10, mask);        // dstReg0依赖上一条 → 串行
    Adds(dstReg0, dstReg0, 10, mask);        // dstReg0依赖上一条 → 串行
    Adds(dstReg0, dstReg0, 10, mask);        // dstReg0依赖上一条 → 串行
    Adds(dstReg0, dstReg0, 10, mask);        // dstReg0依赖上一条 → 串行
    StoreAlign(dstAddr + i * oneRepeatSize, dstReg0, mask);
}
```

优化方式是拆分成两个循环，通过UB中转中间结果：

```cpp
// optimize_vf_dual_instr 样例：拆分为两个循环
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* src0Addr, __ubuf__ T* src1Addr, __ubuf__ T* dstAddr,
                               uint16_t oneRepeatSize, uint16_t repeatTimes, uint32_t count)
{
    AscendC::Reg::RegTensor<T> srcReg0, srcReg1, dstReg0, dstReg1;
    AscendC::Reg::MaskReg mask;

    // 循环1: z = x + y + 30（Add + 3次Adds，4级依赖链）
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
        AscendC::Reg::Add(dstReg0, srcReg0, srcReg1, mask);   // x + y
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg0, mask);
    }
    // 循环2: z = z + 30（3次Adds，3级依赖链）
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(dstReg0, dstAddr + i * oneRepeatSize);
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg0, mask);
    }
}
```

**拆分为什么能降低数据依赖？** 这是理解本优化的核心问题：

1. **拆分的目标不是让循环1和循环2并行执行**。循环2的 `LoadAlign` 需要读循环1的 `StoreAlign` 写入UB的结果，两者之间有明确的UB数据依赖，不可能并行。

2. **拆分的真正收益是缩短每个循环内的串行依赖链长度**。不拆分时，7级串行依赖链意味着执行队列中第2条指令必须等第1条完成，第3条等第2条……7条指令串行排队，双发机会几乎为零。拆分后，循环1只有4级依赖链（Add→Adds→Adds→Adds），循环2只有3级依赖链（Adds→Adds→Adds），每个循环内部的等待气泡更少。

3. **短依赖链让不同迭代之间更容易交错执行**。硬件有指令执行队列，当循环1的 `i=0` 迭代执行到 `StoreAlign` 时，`i=1` 迭代的 `LoadAlign` 已经可以开始发射（因为 `i=1` 的Load访问不同的源地址，不依赖 `i=0` 的Store结果）。依赖链越短，这种迭代间交错越容易发生。下面用时序图详细对比。

4. **StoreAlign→LoadAlign通过UB中转，打破了寄存器间的直传依赖**。虽然两个循环都使用了 `dstReg0` 这个变量名，但循环2的 `LoadAlign(dstReg0, dstAddr + ...)` 是从UB重新加载数据到 `dstReg0`，建立的是新的数据来源（从UB到寄存器），而非寄存器到寄存器的直传。从硬件视角看，StoreAlign写UB和LoadAlign从UB读是两个独立操作，只要StoreAlign完成（数据已写入UB），后续的LoadAlign就可以执行，不再依赖寄存器层面的传递。

**迭代间交错执行的时序对比**

关键前提：不同迭代（`i=0` vs `i=1`）访问不同的地址，因此 `i=1` 的 `LoadAlign`（加载srcAddr[i*oneRepeatSize]）不依赖 `i=0` 的 `StoreAlign`（写dstAddr[i*oneRepeatSize]），两者可以并行。但同一迭代内的计算指令必须严格串行。

假设每条指令耗时1个周期（简化模型），箭头→表示数据依赖：

```
【不拆分】7级依赖链 — i=0 和 i=1 的迭代时序：
  每个迭代需要 9 条指令（2×Load + Add + 6×Adds + Store），其中 7 条串行依赖

  时间:  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
  i=0:  L0 L1 A0 S1 S2 S3 S4 S5 S6 St
            ↓_________________________________↓
            7级串行：i=0的Add到Store必须逐条等待

  i=1:                              L0 L1 A0 S1 S2 S3 S4 S5 S6 St
                                     ↑
                                     i=1必须等i=0完成Store后，队列才有空间
                                     发射新的Load（队列被i=0的串行链占满）

  → i=0 耗时 9 周期，i=1 从第10周期才开始，总耗时约 18 周期（2个迭代）
  → 队列中大部分时间被串行等待占据，双发端口空闲
```

```
【拆分后】4级依赖链 — 循环1的 i=0 和 i=1 交错执行：

  循环1每个迭代只有 7 条指令（2×Load + Add + 3×Adds + Store），4级串行依赖

  时间:  0  1  2  3  4  5  6  7  8  9  10 11 12 13
  i=0:  L0 L1 A0 S1 S2 S3 St
            ↓___________↓
            4级串行，链更短

  i=1:           L0 L1 A0 S1 S2 S3 St
                   ↑
                   i=1的Load0/Load1与i=0的Adds无地址冲突
                   可以提前进入队列执行

  → i=0 在第7周期完成，i=1 从第3周期就开始发射，重叠了4个周期
  → 两个迭代总共约 11 周期（vs 不拆分的18周期）
  → 交错期间，不同迭代的Load和Adds可利用不同执行端口双发
```

```
【拆分后】3级依赖链 — 循环2的 i=0 和 i=1 交错执行：

  循环2每个迭代只有 5 条指令（Load + 3×Adds + Store），3级串行依赖

  时间:  0  1  2  3  4  5  6  7  8  9
  i=0:  L0 S1 S2 S3 St
            ↓_______↓
            3级串行，链更短

  i=1:        L0 S1 S2 S3 St
                ↑
                i=1的Load与i=0的Adds无冲突，更早交错

  → 两个迭代总共约 8 周期，交错更充分
```

**对比总结**：

| 指标 | 不拆分（7级链） | 拆分后（4级+3级链） |
|------|---------------|-------------------|
| 单迭代串行依赖深度 | 7级 | 4级 / 3级 |
| i=0与i=1可重叠的周期数 | ≈0（队列被长链占满） | ≈4 / 2（短链快速释放队列空间） |
| 2个迭代的总耗时（估算） | ≈18周期 | ≈11 + 8 = 19周期* |
| 队列中可双发的指令对 | 极少（大部分时间在等串行） | 更多（不同迭代的Load和计算可并行发射） |

> *注：拆分后总周期数看起来差不多，但关键区别是：拆分后每个循环内部的队列中存在大量可并行的指令对（不同迭代的Load与计算），硬件双发机制能真正发挥作用；而不拆分时，队列中几乎全是串行等待，双发端口大部分时间空闲。当repeatTimes更大时，短链的迭代交错优势会更显著。

**方案二：手动控制循环展开**

如果循环内存在依赖关系过多的指令，队列没有办法同时加载进`for(i = n)`和`for(i = n+1)`的所有指令，那么即使循环之间没有依赖关系，也无法使能双发特性，指令无法并发执行。可以通过手动展开循环，这样做有两个好处：贴近硬件乱序执行的特性，为下发的指令创造更多执行的机会；减少指令因为寄存器资源未到位而产生的等待。

```cpp
// 展开前：循环间无法并行
for (uint16_t i = 0; i < 32; ++i) {
    AscendC::Reg::LoadAlign(srcReg, srcAddr, offset);
    AscendC::Reg::Adds(dstReg0, srcReg, 10, mask);
    AscendC::Reg::Muls(dstReg1, dstReg0, 20, mask);
    ... // 超过64条有数据依赖的指令
    AscendC::Reg::StoreAlign(dstAddr, dstReg1, offset, mask);
}

// 展开后：为乱序执行创造更多机会
for (uint16_t i = 0; i < 8; ++i) { // 32做4展开
    AscendC::Reg::LoadAlign(srcReg0, srcAddr, offset);
    AscendC::Reg::LoadAlign(srcReg1, srcAddr, offset);
    AscendC::Reg::LoadAlign(srcReg2, srcAddr, offset);
    AscendC::Reg::LoadAlign(srcReg3, srcAddr, offset);
    AscendC::Reg::Adds(...)
    AscendC::Reg::Adds(...)
    AscendC::Reg::Adds(...)
    AscendC::Reg::Adds(...)
    AscendC::Reg::Muls(...)
    AscendC::Reg::Muls(...)
    AscendC::Reg::Muls(...)
    AscendC::Reg::Muls(...)
    ...
    AscendC::Reg::StoreAlign(...)
    AscendC::Reg::StoreAlign(...)
    AscendC::Reg::StoreAlign(...)
    AscendC::Reg::StoreAlign(...)
}
```

**方案三：避免寄存器数量超限**

在同一个VF中，硬件可以同时处理的最大RegTensor寄存器个数为32，如果超出，编译器会进行数据的换入换出并加入同步指令，严重拖慢算子的执行效率。同理，在同一个VF中，MaskReg不超过8个，读/写UnalignRegForLoad和UnalignRegForStore各不超过4个，否则会发生寄存器溢出，导致性能劣化。

优化方案：
- 使用布尔代数运算。比如`!(a && b)`可简化为`!a || !b`，`!(a || b)`可以化简为`!a && !b`
- 适当等价调整指令顺序，节省寄存器

```cpp
//【反例】判断指数全为1、尾数不同时为0（结果为NAN）的特殊情况
// 用到的公式为!(a&&b)，需要多申请一个MaskReg即noNaNMask来记录a&&b中间结果
Reg::MaskReg noNaNMask;
Reg::MaskAnd(noNaNMask, dstMask, tmpMask0, mask);
Reg::MaskNot(noNaNMask, noNaNMask, mask);
// ...后续使用noNaNMask

//【正例】!(a&&b)化简为 !a||!b，不需要多申请寄存器
Reg::MaskNot(dstMask, dstMask, mask);
Reg::MaskOr(tmpMask0, tmpMask0, dstMask, mask);
```

#### 1.2.4 为什么能优化

**核心思想**：将一个长依赖链拆成两个短依赖链，通过UB中转中间结果，缩短每个循环内的串行依赖深度，使硬件指令队列中有更多无依赖的指令对可被双发。

```
不拆分（7级串行依赖链）：
  i=0: Load → Load → Add → Adds → Adds → Adds → Adds → Adds → Adds → Store
                 ↑_______↑_______↑_______↑_______↑_______↑_______↑_______↑
                 每条指令等待前一条dstReg0就绪，7级串行，双发几乎不可能
  i=1: （i=0的Store完成后才能开始）...

拆分后（4级 + 3级依赖链）：
  循环1:
    i=0: Load → Load → Add → Adds → Adds → Adds → Store
                     ↑_______↑_______↑_______↑
                     4级串行，链更短
    i=1: Load → Load → Add → Adds → Adds → Adds → Store
                     ↑ i=1的Load不依赖i=0的Store（不同地址），可提前发射

  循环2:
    i=0: Load → Adds → Adds → Adds → Store
                 ↑_______↑_______↑
                 3级串行，链更短
    i=1: Load → Adds → Adds → Adds → Store
                 ↑ 同理，可提前发射
```

| 优化手段 | 原理 | 收益 |
|---------|------|------|
| 合理拆分VF循环 | 缩短单循环内串行依赖链长度（7级→4级+3级），通过UB中转打破寄存器直传依赖 | 队列中可双发的指令对更多，迭代间交错机会更大 |
| 手动控制循环展开 | 为乱序执行创造更多机会 | 减少因寄存器资源未到位的等待 |
| 避免寄存器超限 | 减少编译器插入的换入换出和同步 | 避免性能劣化 |
| 布尔代数化简 | 减少中间MaskReg使用 | 节省寄存器，减少指令数 |

---

### 1.3 优化三：VF融合优化（optimize_vf_fusion）

#### 1.3.1 优化背景

当算子中需要依次调用多个VF函数时，每个VF函数独立执行，函数之间需要将中间结果写回UB再由下一个VF函数读取。这带来了额外的UB读写开销。

#### 1.3.2 问题场景

计算 `y = 1/x + 1`（先取倒数再加一），拆成两个VF函数：

```cpp
// 两个独立VF函数
AscendC::VF_CALL<DivVF<T>>(dstAddr, srcAddr, ...);   // 第1个VF: 取倒数
AscendC::VF_CALL<AddVF<T>>(dstAddr, srcAddr, ...);   // 第2个VF: 加一
```

每个VF函数内部的循环结构为 `Load → 计算 → Store`，两个VF函数之间必须通过UB传递数据。

#### 1.3.3 VF融合原理

VF融合是将代码中多个VF函数融合成一个VF函数，有效提升性能。VF融合特性是优化特性，VF自动融合会借助Loop Fuse算法，将VF转换成Loop形态，然后将控制流等价（Control-Flow-Equivalent）的VF进行融合，最后将VF进行还原。

编译器首先会做融合前的合法性检查，判断两个VF是否等价，Main侧中间代码是否能在VF内执行以及融合后是否可产生正收益（不会引起传参寄存器溢出、VF代码不会过大）等，如果满足VF融合条件，编译器会自动执行VF融合优化。为保证融合后的VF执行逻辑与语义与融合前一致，会在原来两个VF之间保守地插入同步指令。编译器还会尝试外提、合并融合后的VF中的指令，对VF代码进行优化。融合策略是能融尽融，用户按照符合融合的合法性检查的模式进行编码，可以增加VF融合的机会。

VF融合优化可分为三个阶段：

**1) VF浅度融合**

编译器首先会分析两个VF的控制流是否等价，构建Cost Model分析是否有正向收益，如果满足VF融合条件，将VF外部的控制流融入到VF内，将VF外的Software Loop硬化成VF内的Hardware Loop，然后使能VF自动融合的基础能力，将两个VF融合成一个VF，为后续的VF深度融合提供基础。

**2) VF深度融合**

VF深度融合会继续对VF内的Hardware Loop进行融合，从而减少Hardware Loop的启动开销，并且极大地减少冗余的Load/Store操作，充分复用寄存器。

**3) VF内自动同步**

编译器会精准地插入必要的同步指令，删除冗余的同步指令，极大地释放了硬件OOO（Out of Order）能力。用户无需手动插入同步指令，极大地降低了用户的编码难度。

#### 1.3.4 优化方案

**方案一：多个VF函数自动融合**

如果多个VF函数的控制流等价，且满足均为Hardware Loop循环，编译器会执行VF融合优化特性：

```cpp
// optimize_vf_fusion 样例
template <typename T>
__simd_vf__ inline void DivVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr,
                               uint16_t oneRepeatSize, uint32_t repeatTimes, uint32_t count)
{
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<T> srcReg0, srcReg1, dstReg;
    constexpr float num = 1.0f;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg0, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Duplicate(srcReg1, num, mask);
        AscendC::Reg::Div(dstReg, srcReg1, srcReg0, mask);  // 1/x
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr,
                               uint16_t oneRepeatSize, uint32_t repeatTimes, uint32_t count)
{
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<T> srcReg, dstReg;
    constexpr float num = 1.0f;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Adds(dstReg, srcReg, num, mask);      // + 1
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}

// 调用端：两个VF_CALL
AscendC::VF_CALL<DivVF<T>>(dstAddr, srcAddr, oneRepeatSize, repeatTimes, count);
AscendC::VF_CALL<AddVF<T>>(dstAddr, srcAddr, oneRepeatSize, repeatTimes, count);
```

编译器会将这两个VF函数融合，效果等价于：

```cpp
// 融合后的等效VF函数
for (uint16_t i = 0; i < repeatTimes; ++i) {
    LoadAlign(srcReg0, srcAddr + i * oneRepeatSize);    // Load 1次
    Duplicate(srcReg1, 1.0f, mask);
    Div(dstReg, srcReg1, srcReg0, mask);                // 1/x（寄存器计算）
    Adds(dstReg, dstReg, 1.0f, mask);                   // +1（寄存器计算）
    StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask); // Store 1次
}
```

**方案二：使用基础API连续计算模式**

基础API实现对硬件能力的抽象，开放芯片的能力，保证完备性和兼容性。基础API根据对数据操作方法的不同，可以分为两大类：

- **连续计算API**：支持Tensor前n个数据计算。针对源操作数的连续n个数据进行计算并连续写入目的操作数，解决一维tensor的连续计算问题
- **高维切分API**：支持Repeat和Stride。功能灵活的计算API，提供与Builtin API完全对等的编程能力，充分发挥硬件优势，支持对每个操作数的DataBlock Stride，Repeat Stride，Mask等参数的操作

在VF融合优化中，推荐使用基础API的连续计算模式编写算子，可以充分发挥出VF融合优化的能力。与高维切分API相比，连续计算API使得编译器能更好地分析VF融合优化，更加容易满足VF融合优化的条件：

```cpp
//【反例】使用基础API的高维切分模式，编译器在分析VF融合时受复杂的计算逻辑影响，
// 无法对Add和Mul接口进行VF融合优化
AscendC::Add(yLocal, xLocal, xLocal, mask, 4, { 1, 1, 1, 8, 8, 8 });
AscendC::Mul(yLocal, yLocal, xLocal, mask, 4, { 1, 1, 1, 8, 8, 8 });

//【正例】使用基础API的连续计算模式，编译器分析Add和Mul函数后符合VF融合要求，
// 将Add和Mul融合成一个VF函数
AscendC::Add(yLocal, xLocal, xLocal, count);
AscendC::Mul(yLocal, yLocal, xLocal, count);
```

#### 1.3.5 为什么能优化

```
融合前：两个独立VF函数
  DivVF: [Load → Div → Store] [Load → Div → Store] ...
  AddVF: [Load → Adds → Store] [Load → Adds → Store] ...
         ↑ 需要从UB重新Load DivVF的Store结果

融合后：一个VF函数
  [Load → Div → Adds → Store] [Load → Div → Adds → Store] ...
         ↑ Div结果直接在寄存器中传给Adds，无需Store再Load
```

| 融合阶段 | 优化内容 | 收益 |
|---------|---------|------|
| VF浅度融合 | 将VF外控制流融入VF内，Software Loop硬化为Hardware Loop | 减少循环管理开销 |
| VF深度融合 | 对VF内Hardware Loop进行融合 | 减少Loop启动开销，减少冗余Load/Store，充分复用寄存器 |
| VF内自动同步 | 精准插入/删除同步指令 | 释放硬件OOO能力，降低编码难度 |

- 消除了DivVF和AddVF之间的UB Store/Load开销
- 减少了总体的Load/Store次数（从4次减为2次/迭代）
- 寄存器中直接传递中间结果，延迟更低
- 连续计算API使编译器更容易分析并执行VF融合

---

### 1.4 优化四：连续非对齐搬运优化（optimize_vf_continious_align）

#### 1.4.1 优化背景

当数据地址不是32字节对齐时，需要使用非对齐搬运接口`LoadUnAlign/StoreUnAlign`。这些接口内部使用非对齐寄存器（UnalignRegForLoad/UnalignRegForStore）作为缓冲区来处理跨对齐边界的数据。

为提升对不规则内存地址的处理能力，Reg矢量计算支持在数据搬运过程中对非32字节对齐的地址进行访问，适用于从UB向RegTensor非对齐搬运，或从RegTensor向UB非对齐搬运的场景。为降低非对齐访问带来的性能开销，RegBase引入非对齐寄存器缓存机制，该机制利用非对齐寄存器`UnalignRegForLoad`和`UnalignRegForStore`作为临时缓存区，用于暂存跨对齐边界的数据，从而实现高效的连续非对齐数据传输。

#### 1.4.2 非对齐搬运接口介绍

在读非对齐地址前，应该先通过`LoadUnAlignPre`进行初始化，然后再使用`LoadUnAlign`。在写非对齐地址时，先使用`StoreUnAlign`，再使用`StoreUnAlignPost`进行后处理。

- 连续非对齐搬入，使用uint32_t存储搬运量：
  ```cpp
  __simd_callee__ inline void LoadUnAlignPre(UnalignRegForLoad& ureg, __ubuf__ T* srcAddr);
  __simd_callee__ inline void LoadUnAlign(U& dstReg, UnalignRegForLoad& ureg,
                                           __ubuf__ T*& srcAddr, uint32_t postUpdateStride);
  ```

- 连续非对齐搬出，使用uint32_t存储偏移量：
  ```cpp
  __simd_callee__ inline void StoreUnAlign(__ubuf__ T*& dstAddr, U& srcReg,
                                            UnalignRegForStore& ureg, uint32_t postUpdateStride);
  __simd_callee__ inline void StoreUnAlignPost(__ubuf__ T*& dstAddr,
                                                UnalignRegForStore& ureg, int32_t postUpdateStride);
  ```

- 连续非对齐搬出，将maskReg搬出至UB：
  ```cpp
  __simd_callee__ inline void StoreUnAlign(__ubuf__ T*& dstAddr, MaskReg& mask,
                                            UnalignRegForStore& ureg);
  __simd_callee__ inline void StoreUnAlignPost(__ubuf__ T*& dstAddr,
                                                UnalignRegForStore& ureg, int32_t postUpdateStride);
  ```

#### 1.4.3 非对齐寄存器原理

**1) 非对齐数据搬入**

以从UB地址srcAddr ~ 304读取数据，并将其搬运至目标寄存器dstReg（256B）为例，处理流程如下：

1. 调用`LoadUnAlignPre`进行非对齐搬入初始化。非对齐寄存器ureg缓存UB地址32 ~ 64的有效数据，作为后续非对齐访问的前置数据缓存
2. 调用`LoadUnAlign`，硬件指令将UB地址64 ~ 320的数据搬入临时寄存器tmpReg，并将ureg中srcAddr ~ 64对应的数据与tmpReg中地址64 ~ 304对应的数据拼接在一起，将结果写入dstReg
3. 本次搬运后，UB地址288 ~ 320的数据会被写入ureg

连续非对齐搬入时，由于`LoadUnAlign`会将后续未对齐的数据缓存至ureg，所以下一次搬入不需要再次调用`LoadUnAlignPre`，只需在第一次搬入前调用一次`LoadUnAlignPre`，从而实现非对齐搬入的性能优化。

**2) 非对齐数据搬出**

将源寄存器srcReg中的非对齐数据写入UB地址dstAddr，根据ureg当前状态，分为两种场景：

- **场景一：ureg为空**（连续非对齐搬出的起始状态）
  1. 调用`StoreUnAlign`，此时ureg内无有效数据，将srcReg中对应UB地址的有效数据写入dstAddr，同步将srcReg中对应UB地址尾部跨对齐边界的数据缓存至ureg
  2. 调用`StoreUnAlignPost`进行非对齐搬出后处理，将ureg中缓存的有效数据写入UB

- **场景二：ureg不为空**（连续非对齐搬出的中间状态）
  1. 调用`StoreUnAlign`，此时ureg内有有效数据，系统将ureg中缓存的数据与srcReg中对应的有效数据进行拼接，结果写入UB。同步将srcReg中尾部的跨对齐边界数据缓存至ureg
  2. 连续非对齐搬出时，由于当ureg不为空时，下一次`StoreUnAlign`会读本次`StoreUnAlign`缓存至ureg中数据，所以本次搬出不需调用`StoreUnAlignPost`，只需在最后一次搬出后调用一次`StoreUnAlignPost`

**3) MaskReg连续非对齐数据搬出**

将源寄存器MaskReg（32B）搬出至UB：

- UB地址上数据类型为b16时，硬件指令从每个2位数据中提取最低有效位（LSB），将MaskReg中32B数据打包成16B，写入UB。搬运完成后UB地址按16B偏移量更新
- UB地址上数据类型为b32时，硬件指令从每个4位数据中提取最低有效位（LSB），将MaskReg中32B数据打包成8B，写入UB。搬运完成后UB地址按8B偏移量更新

#### 1.4.4 优化方案

关键优化点：**将初始化和后处理操作移到循环外部**。

```cpp
// optimize_vf_continious_align 样例
template <typename T>
__simd_vf__ inline void LoadVF(__ubuf__ T* srcAddr, __ubuf__ T* dstAddr,
                                uint16_t postUpdateStride, uint32_t repeatTimes, uint16_t count)
{
    AscendC::Reg::RegTensor<T> srcReg, dstReg;
    AscendC::Reg::UnalignRegForLoad ureg0;
    AscendC::Reg::UnalignRegForStore ureg1;

    // 优化1：初始化移到循环外（仅执行一次）
    AscendC::Reg::LoadUnAlignPre(ureg0, srcAddr);

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        AscendC::Reg::LoadUnAlign(srcReg, ureg0, srcAddr + i * postUpdateStride);
        AscendC::Reg::StoreUnAlign(dstAddr, srcReg, ureg1, postUpdateStride);
    }

    // 优化2：后处理移到循环外（仅执行一次）
    AscendC::Reg::StoreUnAlignPost(dstAddr, ureg1, 0);
}
```

#### 1.4.5 为什么能优化

```
LoadUnAlignPre: 缓存UB地址前32B的数据到ureg（仅首次需要）
LoadUnAlign:    从UB搬入数据 + 与ureg拼接 → 写入RegTensor
                同时缓存后续未对齐数据到ureg（供下次使用）
                → 因此连续搬入时不需要再次调用Pre

StoreUnAlign:   将RegTensor数据写入UB + 缓存尾部数据到ureg
StoreUnAlignPost: 将ureg中缓存的尾部数据写入UB（仅最后需要）
                  → 因此连续搬出时不需要每次调用Post
```

| 操作 | 循环内执行（反例） | 循环外执行（正例） |
|------|-----------------|-----------------|
| `LoadUnAlignPre` | 每次循环1次 → N次 | 仅1次 |
| `StoreUnAlignPost` | 每次循环1次 → N次 | 仅1次 |

当 `repeatTimes` 较大时，节省的指令数非常可观。

**额外注意事项**：
- 优先采用连续对齐搬运以避免额外开销，提升整体性能
- 由于硬件支持限制，连续非对齐搬运时不建议使用AddrReg存储偏移量，推荐使用`uint32_t`类型存储偏移量，使用post update模式搬运，该场景下每次调用接口自动更新源操作数在UB上的地址

---

## 2. 优化手段总览与对比

### 2.1 四种优化手段总览

| 样例 | 优化维度 | 核心手段 | 关键原理 |
|------|---------|---------|---------|
| optimize_vf_loop | 循环结构 | 多维嵌套循环+层级stride、固定Mask、循环外提 | Hardware Loop编码规范、地址寄存器生成、指令分布优化 |
| optimize_vf_dual_instr | 指令调度 | 拆分VF循环、循环展开、避免寄存器超限 | 指令双发条件、执行队列深度、布尔代数化简 |
| optimize_vf_fusion | 函数融合 | 控制流等价VF自动融合、连续计算API | 三阶段融合（浅度/深度/自动同步）、Loop Fuse算法 |
| optimize_vf_continious_align | 搬运效率 | LoadUnAlignPre/StoreUnAlignPost提到循环外 | 非对齐寄存器缓存机制、连续搬运复用ureg |

### 2.2 VF循环优化的子维度

VF循环优化可以进一步细分为以下子维度，每个子维度都有明确的编码规范和优化原理：

| 子维度 | 核心规则 | 违反后果 |
|-------|---------|---------|
| Hardware Loop编码规范 | 迭代变量uint16_t、起始0、步长1、不允许跳转 | 退化为Software Loop，性能劣化 |
| 成员变量访问 | 通过局部变量传参，不直接访问成员变量 | 导致VF融合失效 |
| 指令分布 | 与索引无关的语句提到循环外 | 循环内指令数增多 |
| 地址管理 | 四维嵌套循环寻址模式匹配AddrReg | 标量地址计算开销大 |

### 2.3 优化手段的层次关系

```
优化层次（从宏观到微观）：

1. 编程范式选择
   ┌──────────────────────────────────────────┐
   │  MemBase → RegBase                        │
   │  （从LocalTensor编程切换到寄存器编程）        │
   └──────────────────────────────────────────┘

2. VF函数级优化
   ┌──────────────────────────────────────────┐
   │  VF融合优化                                │
   │  （多个VF函数 → 一个VF函数）                │
   │  消除VF间UB读写，寄存器直传中间结果           │
   └──────────────────────────────────────────┘

3. VF函数内部优化
   ┌──────────────────────────────────────────┐
   │  循环结构 / 指令调度 / 搬运效率              │
   │  （在VF函数内部进一步压榨硬件性能）           │
   └──────────────────────────────────────────┘
```

**核心本质**：所有优化的共同基础都是利用寄存器直接计算来消除UB访存开销。VF融合优化是"函数级"的寄存器复用，VF循环/双发/非对齐优化是"指令级"的寄存器复用。

---

## 3. 参考文档

| 文档名称 | 链接 |
|---------|------|
| VF循环优化 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00023.html) |
| 指令双发优化 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00024.html) |
| VF融合优化 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00025.html) |
| 连续非对齐场景优化 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00026.html) |
| Reg矢量计算编程 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlasascendc_1e_0059.html) |
| SIMD与SIMT混合编程基础知识 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlasascendc_1e_0060.html) |
| 优化建议总览表 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0010.html) |
| CANN社区版9.0.0-beta.2 文档首页 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/index/index.html) |
