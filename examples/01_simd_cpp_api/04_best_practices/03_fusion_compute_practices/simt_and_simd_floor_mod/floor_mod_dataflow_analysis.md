# SIMT 与 SIMD 混合编程实现 floor_mod 算子：数据流向与内存排布全解

> 本文档聚焦于**SIMT 与 SIMD 的基础概念、数据如何被切分与搬运、两种编程范式下的计算路径差异**，结合代码详细解释。

---

## 一、基础概念：SIMD 与 SIMT

### 1.1 SIMD（Single Instruction Multiple Data）

SIMD 是一种**数据并行**的编程范式：

- **单指令多数据**：一条指令同时作用于多个数据元素
- **寄存器级操作**：数据需要先加载到寄存器（RegTensor），再通过向量指令批量处理
- **固定粒度**：单次处理的数据量由寄存器长度决定（本平台为 256 Bytes），超出部分需循环处理
- **显式循环管理**：程序员需要手动切分数据、管理循环次数、处理尾部

```
SIMD 计算模型（以 float 为例）:
  寄存器长度 = 256 Bytes = 64 个 float
  1024 个 float 需 1024/64 = 16 次循环

  ┌───────────────────────────────────────────────────┐
  │  循环 j=0:  Reg ← UB[0..63]     → 计算 → UB[0..63]    │
  │  循环 j=1:  Reg ← UB[64..127]   → 计算 → UB[64..127]  │
  │  ...                                               │
  │  循环 j=15: Reg ← UB[960..1023] → 计算 → UB[960..1023] │
  └───────────────────────────────────────────────────┘
  每次循环: 1条向量指令 → 同时处理64个float
```

### 1.2 SIMT（Single Instruction Multiple Threads）

SIMT 是一种**线程并行**的编程范式：

- **单指令多线程**：每个线程独立执行相同的代码，但处理不同的数据
- **标量视角**：每个线程像写串行代码一样处理单个元素，无需手动切分
- **线程索引**：通过 `threadIdx.x` 区分不同线程的数据位置
- **硬件调度**：硬件自动将线程映射到执行单元，程序员无需管理循环

```
SIMT 计算模型（以 1024 个线程为例）:
  每个线程处理1个元素，共1024个线程

  Thread 0:   index=0   → self[0] % other[0]   → out[0]
  Thread 1:   index=1   → self[1] % other[1]   → out[1]
  ...
  Thread 1023: index=1023 → self[1023] % other[1023] → out[1023]
  所有线程并行执行同一段代码
```

### 1.3 SIMD 与 SIMT 的对比

```
┌──────────────┬──────────────────────────┬──────────────────────────┐
│ 维度          │ SIMD                     │ SIMT                     │
├──────────────┼──────────────────────────┼──────────────────────────┤
│ 编程视角      │ 向量/寄存器              │ 标量/线程                │
│ 数据切分      │ 程序员手动循环切分        │ 硬件自动调度线程          │
│ 单次处理量    │ 寄存器长度 (256B)        │ 1个元素/线程             │
│ 循环管理      │ 显式 for 循环            │ 无循环, 线程索引替代      │
│ 尾部处理      │ 需要 Mask 机制           │ 无需, 每线程独立          │
│ 适用场景      │ float 等有向量指令支持    │ int32 等标量操作高效      │
│ 函数标记      │ __simd_vf__              │ __simt_vf__              │
│ 地址空间      │ __ubuf__ (UB 地址)       │ __ubuf__ (UB 地址)       │
│ 数据搬运      │ 由外层 SIMD 代码完成      │ 由外层 SIMD 代码完成      │
│ 内存共享      │ ─────────── 共享同一 UB 空间 ──────────────          │
└──────────────┴──────────────────────────┴──────────────────────────┘
```

**核心要点**：SIMT 和 SIMD 在 Vector Core 上共享片上存储（UB），因此可以将数据搬入/搬出交给 SIMD 完成的 DataCopy，而计算逻辑根据数据类型选择 SIMT 或 SIMD。

### 1.4 `asc_vf_call`：VF 函数调用接口

```cpp
template <auto funcPtr, typename... Args>
__aicore__ inline void asc_vf_call(dim3 threadNums, Args &&...args);
```

- `funcPtr`：SIMT 或 SIMD 函数指针
- `threadNums`：启动的线程数（`dim3` 类型，本例为 `dim3(1024)`）
- `args`：传递给函数的参数（UB 地址指针等）

**调用方式**：
```cpp
// SIMT 调用（int32_t 场景）
asc_vf_call<floor_mod_simt<T>>(dim3(THREAD_COUNT), selfAddr, otherAddr, dstAddr, count_per_block);

// SIMD 调用（float 场景）
asc_vf_call<floor_mod_float_simd<T>>(fmodResAddr, otherAddr, dstAddr, count_per_block);
```

---

## 二、全局参数与数据 Shape

```
input_total_length = 6400     (输入元素总数)
DATA_LEN_PER_CORE = 1024     (每个核处理的数据量)
THREAD_COUNT = 1024           (SIMT 每个核启动的线程数)
VECTOR_LENGTH = 256           (SIMD 寄存器长度, Bytes)
numBlocks = ceil(6400, 1024) = 7  (核数)

数据类型:
  float:   sizeof = 4 Bytes,  vecLen = 256/4 = 64 元素/寄存器
  int32_t: sizeof = 4 Bytes,  vecLen = 256/4 = 64 元素/寄存器

UB 空间 (dynUBufSize):
  float:   1024 × 4 × 3 = 12,288 Bytes  (3 = self + other + output)
  int32_t: 1024 × 4 × 3 = 12,288 Bytes
  (int32_t 场景额外需要 div 临时空间, 实际由 LocalMemAllocator 分配)
```

---

## 三、多核任务分配

### 3.1 核函数启动

```cpp
floor_mod<T><<<numBlocks, dynUBufSize, stream>>>(input_device, index_device, output_device, input_total_length);
```

- `numBlocks = 7`：启动 7 个核
- `dynUBufSize`：每个核的 UB 大小
- `__global__ __vector__`：纯 Vector 核函数（非 Cube-Vector 混合）

### 3.2 数据分配策略

```
6400 个元素, 7 个核, 每核处理 1024 个

┌────────┬──────────────┬────────────────────┐
│ Core # │ GM 偏移       │ 处理元素范围         │
├────────┼──────────────┼────────────────────┤
│ 0      │ 0 × 1024     │ [0, 1024)          │
│ 1      │ 1 × 1024     │ [1024, 2048)       │
│ 2      │ 2 × 1024     │ [2048, 3072)       │
│ 3      │ 3 × 1024     │ [3072, 4096)       │
│ 4      │ 4 × 1024     │ [4096, 5120)       │
│ 5      │ 5 × 1024     │ [5120, 6144)       │
│ 6      │ 6 × 1024     │ [6144, 7168)       │
│        │              │ (实际只有 6400-6144 │
│        │              │  = 256 个有效元素)  │
└────────┴──────────────┴────────────────────┘

GM 偏移计算（Kernel::Init）:
  selfGm  = (__gm__ T*)self  + GetBlockIdx() * 1024
  otherGm = (__gm__ T*)other + GetBlockIdx() * 1024
  outGm   = (__gm__ T*)out   + GetBlockIdx() * 1024
```

**尾部核处理**：Core 6 只有 256 个有效元素（6400 - 6×1024 = 256），但 Kernel 仍按 `count_per_block=1024` 分配 UB 和执行 DataCopy。实际有效数据由后续计算逻辑通过 `count` 参数控制。

---

## 四、Kernel 类执行流程

### 4.1 Init：GM 绑定与 UB 分配

```cpp
__aicore__ inline void Init(GM_ADDR self, GM_ADDR other, GM_ADDR out, TPipe* pipeIn, uint32_t total_count)
{
    // ① GM 地址绑定（每个核偏移自己的数据段）
    selfGm.SetGlobalBuffer((__gm__ T*)self + GetBlockIdx() * DATA_LEN_PER_CORE);
    otherGm.SetGlobalBuffer((__gm__ T*)other + GetBlockIdx() * DATA_LEN_PER_CORE);
    outGm.SetGlobalBuffer((__gm__ T*)out + GetBlockIdx() * DATA_LEN_PER_CORE);

    // ② UB 队列分配
    pipe->InitBuffer(inQueueSelf,  1, sizeof(T) * count_per_block);  // 1 × 4096 B
    pipe->InitBuffer(inQueueOther, 1, sizeof(T) * count_per_block);  // 1 × 4096 B
    pipe->InitBuffer(outQueue,     1, sizeof(T) * count_per_block);  // 1 × 4096 B
}
```

**UB 内存排布**：

```
UB 总大小 = 3 × 1024 × 4 = 12,288 Bytes (float/int32_t)

┌──────────────────────────────────────────────────────┐
│  UB                                                   │
│                                                       │
│  inQueueSelf:  [0, 4096)       ← self 数据            │
│  inQueueOther: [4096, 8192)    ← other 数据           │
│  outQueue:     [8192, 12288)   → output 数据          │
│                                                       │
│  注意: BUFFER_NUM = 1 (单缓冲, 非双缓冲流水)           │
│                                                       │
│  float 场景额外分配 (由 LocalMemAllocator 管理):      │
│  fmodRes:      额外 4096 Bytes ← Fmod 中间结果        │
│                                                       │
│  int32_t 场景额外分配:                                │
│  divAddr:      额外 4096 Bytes ← 整除结果临时空间      │
└──────────────────────────────────────────────────────┘
```

### 4.2 CopyIn：GM → UB

```cpp
__aicore__ inline void CopyIn()
{
    // ① self: GM → UB
    LocalTensor<T> selfLocal = inQueueSelf.AllocTensor<T>();
    DataCopy(selfLocal, selfGm, count_per_block);
    //   源: selfGm[coreIdx * 1024], 1024 个 T
    //   大小: 1024 × 4 = 4096 Bytes
    inQueueSelf.EnQue<T>(selfLocal);

    // ② other: GM → UB
    LocalTensor<T> otherLocal = inQueueOther.AllocTensor<T>();
    DataCopy(otherLocal, otherGm, count_per_block);
    inQueueOther.EnQue<T>(otherLocal);

    // ③ 同步等待: MTE2 → V (数据搬运完成才能计算)
    SetFlag<HardEvent::MTE2_V>(0);
    WaitFlag<HardEvent::MTE2_V>(0);
}
```

**数据流**：
```
GM                          UB
self[coreIdx*1024 .. +1024]  ──DataCopy──►  inQueueSelf [4096 Bytes]
other[coreIdx*1024 .. +1024] ──DataCopy──►  inQueueOther [4096 Bytes]
```

### 4.3 Compute：分数据类型执行

Compute 是本样例的核心，根据数据类型选择不同的计算路径：

```cpp
__aicore__ inline void Compute()
{
    LocalTensor<T> selfLocal = inQueueSelf.DeQue<T>();
    LocalTensor<T> otherLocal = inQueueOther.DeQue<T>();
    LocalTensor<T> outLocal = outQueue.AllocTensor<T>();

    if constexpr (std::is_same_v<T, int32_t>) {
        // ★ SIMT 路径: 整型场景
        __ubuf__ T* selfAddr = (__ubuf__ T*)selfLocal.GetPhyAddr();
        __ubuf__ T* otherAddr = (__ubuf__ T*)otherLocal.GetPhyAddr();
        __ubuf__ T* dstAddr = (__ubuf__ T*)outLocal.GetPhyAddr();
        asc_vf_call<floor_mod_simt<T>>(dim3(THREAD_COUNT), selfAddr, otherAddr, dstAddr, count_per_block);
    } else if constexpr (std::is_same_v<T, float>) {
        // ★ SIMD 路径: float 场景
        // Step 1: 先用高级 API Fmod 计算 fmod 结果
        LocalMemAllocator<Hardware::UB> ub_allocator;
        LocalTensor<float> fmodRes = ub_allocator.Alloc<float>(count_per_block);
        constexpr FmodConfig config = {FmodAlgo::ITERATION_COMPENSATION, 11};
        Fmod<T, false, config>(fmodRes, selfLocal, otherLocal);

        // Step 2: 再用 SIMD 寄存器操作完成 floor_mod 修正
        __ubuf__ T* otherAddr = (__ubuf__ T*)otherLocal.GetPhyAddr();
        __ubuf__ T* dstAddr = (__ubuf__ T*)outLocal.GetPhyAddr();
        __ubuf__ T* fmodResAddr = (__ubuf__ T*)fmodRes.GetPhyAddr();
        asc_vf_call<floor_mod_float_simd<T>>(fmodResAddr, otherAddr, dstAddr, count_per_block);
    }

    outQueue.EnQue<T>(outLocal);
}
```

**关键**: 两种路径都通过 `GetPhyAddr()` 获取 UB 物理地址，转为 `__ubuf__` 指针后传入 `asc_vf_call`。SIMT 和 SIMD 函数直接在 UB 上操作，不需要额外的数据搬运。

### 4.4 CopyOut：UB → GM

```cpp
__aicore__ inline void CopyOut()
{
    // ① 同步等待: V → MTE3 (计算完成才能搬出)
    SetFlag<HardEvent::V_MTE3>(0);
    WaitFlag<HardEvent::V_MTE3>(0);

    // ② UB → GM
    LocalTensor<T> outLocal = outQueue.DeQue<T>();
    DataCopy(outGm, outLocal, count_per_block);
    //   目标: outGm[coreIdx * 1024], 1024 个 T
    //   大小: 1024 × 4 = 4096 Bytes
}
```

---

## 五、SIMT 路径详细数据流（int32_t 场景）

### 5.1 floor_mod_simt 函数

```cpp
template <typename T>
__simt_vf__ inline void floor_mod_simt(
    __ubuf__ T* self,
    __ubuf__ T* other,
    __ubuf__ T* out,
    uint32_t input_total_length)
{
    uint32_t index = threadIdx.x;          // ★ 线程索引
    auto rem = self[index] % other[index];  // C语言 % 运算 (向零取整)
    bool signs_differ = ((rem < 0) != (other[index] < 0));  // 余数与除数符号不同?
    if (signs_differ && (rem != 0)) {
        out[index] = rem + other[index];    // 修正: 向负无穷取整
    } else {
        out[index] = rem;
    }
}
```

### 5.2 SIMT 数据流详解

```
UB 内存布局 (int32_t 场景):

┌──────────────────────────────────────────────────────────────┐
│  inQueueSelf:   self[0], self[1], ..., self[1023]            │
│  inQueueOther:  other[0], other[1], ..., other[1023]         │
│  outQueue:      out[0], out[1], ..., out[1023]               │
└──────────────────────────────────────────────────────────────┘

SIMT 线程执行模型:
  启动 dim3(1024) = 1024 个线程
  每个线程的 index = threadIdx.x (0, 1, 2, ..., 1023)

  Thread 0:    index=0     self[0] % other[0]     → out[0]
  Thread 1:    index=1     self[1] % other[1]     → out[1]
  Thread 2:    index=2     self[2] % other[2]     → out[2]
  ...
  Thread 1023: index=1023  self[1023] % other[1023] → out[1023]

  所有线程并行执行, 每线程读取 1 个 self + 1 个 other, 写出 1 个 out
```

### 5.3 SIMT 的 floor_mod 算法逻辑

C 语言的 `%` 运算是**向零取整**（truncation），而 floor_mod 需要**向负无穷取整**。两者仅在**余数与除数符号不同**时有差异：

```
C 语言 % (向零取整):
  7 % 3 = 1       ✓ 与 floor_mod 相同
  -7 % 3 = -1     ✗ floor_mod 应为 2
  7 % -3 = 1      ✗ floor_mod 应为 -2
  -7 % -3 = -1    ✓ 与 floor_mod 相同

修正规则:
  若 rem ≠ 0 且 rem 与 other 符号不同:
    floor_mod = rem + other
  否则:
    floor_mod = rem

示例:
  -7 % 3 = -1 → rem=-1, other=3, 符号不同且非零 → -1 + 3 = 2 ✓
  7 % -3 = 1  → rem=1,  other=-3, 符号不同且非零 → 1 + (-3) = -2 ✓
```

### 5.4 int32_t 完整场景的 SIMD 路径

int32_t 场景实际使用的是 `floor_mod_int_simd` 函数，通过 `asc_vf_call` 调用：

```
但查看代码, int32_t 场景的 Compute 逻辑:

asc_vf_call<floor_mod_simt<T>>(dim3(THREAD_COUNT), selfAddr, otherAddr, dstAddr, count_per_block);

→ 直接调用 SIMT 函数, 每个 thread 处理 1 个元素
→ 不需要循环切分, 硬件自动调度 1024 个线程
```

---

## 六、SIMD 路径详细数据流（float 场景）

### 6.1 float 场景的两阶段计算

float 场景分两步完成：

```
阶段 1: Fmod 高级 API — 计算 fmod(self, other)
  输入: selfLocal[1024] (float), otherLocal[1024] (float)
  输出: fmodRes[1024] (float)
  计算: fmodRes[i] = self[i] - trunc(self[i]/other[i]) * other[i]

阶段 2: floor_mod_float_simd — 将 fmod 修正为 floor_mod
  输入: fmodRes[1024], other[1024]
  输出: dst[1024]
  修正逻辑: 若 fmod 结果非零且与 other 符号不同, 则 dst = fmod + other
```

### 6.2 Fmod 高级 API 的数据流

```cpp
LocalMemAllocator<Hardware::UB> ub_allocator;
LocalTensor<float> fmodRes = ub_allocator.Alloc<float>(count_per_block);
// 从 UB 剩余空间分配 1024 × 4 = 4096 Bytes

constexpr FmodConfig config = {FmodAlgo::ITERATION_COMPENSATION, 11};
Fmod<T, false, config>(fmodRes, selfLocal, otherLocal);
//   输入: selfLocal (UB), otherLocal (UB)
//   输出: fmodRes (UB)
//   算法: ITERATION_COMPENSATION, 迭代 11 次
//   整个计算在 UB 内完成, 无 GM 读写
```

**UB 空间扩展**（float 场景）：
```
原 UB 分配:
  inQueueSelf:   4096 B
  inQueueOther:  4096 B
  outQueue:      4096 B
  合计: 12,288 B

Fmod 额外分配:
  fmodRes:       4096 B (LocalMemAllocator 从 UB 剩余空间分配)

float 场景 UB 总使用: 12,288 + 4,096 = 16,384 B
```

### 6.3 floor_mod_float_simd 寄存器级数据流

```cpp
template <typename T>
__simd_vf__ inline void floor_mod_float_simd(
    __ubuf__ T* fmodResAddr, __ubuf__ T* otherAddr, __ubuf__ T* dstAddr, const uint32_t count)
{
    uint32_t vecLen = VECTOR_LENGTH / sizeof(T);  // 256 / 4 = 64 (float)
    uint16_t loopTimes = (count + vecLen - 1) / vecLen;  // (1024 + 63) / 64 = 16

    // ★ 寄存器分配
    RegTensor<T> zeroValue, fmodResValue, inputX2Value, addValue, resValue;
    RegTensor<uint32_t> signValue, fmodSignValue, inputX2signValue;
    MaskReg preg, negValue, signNegValue, resMaskValue;
    uint32_t sregMask = count;  // 1024

    // ★ 常量初始化
    Reg::Duplicate(zeroValue, T(0));           // 零
    Reg::Duplicate(signValue, FMOD_B32_SIGN);  // 0x80000000 (符号位掩码)

    for (uint16_t j = 0; j < loopTimes; j++) {
        // ① 更新 Mask (处理尾部)
        preg = Reg::UpdateMask<T>(sregMask);
        //   j=0:  sregMask=1024, preg=全1 (64个有效)
        //   j=1:  sregMask=960,  preg=全1
        //   ...
        //   j=15: sregMask=64,   preg=全1
        //   j=16: 不执行 (loopTimes=16, j最大15)

        // ② 加载 fmod 结果到寄存器
        Reg::DataCopy<T, LoadDist::DIST_NORM>(fmodResValue, fmodResAddr + vecLen * j);
        //   UB → Reg: fmodRes[j*64 .. (j+1)*64-1]

        // ③ 判断 fmod 结果是否非零
        Reg::Compare<T, CMPMODE::NE>(negValue, fmodResValue, zeroValue, preg);
        //   negValue[i] = (fmodResValue[i] != 0)

        // ④ 提取 fmod 结果的符号位
        Reg::And(fmodSignValue, (RegTensor<uint32_t>&)fmodResValue, signValue, preg);
        //   fmodSignValue[i] = fmodRes[i] & 0x80000000 → 提取符号位

        // ⑤ 加载 other 到寄存器
        Reg::DataCopy<T, LoadDist::DIST_NORM>(inputX2Value, otherAddr + vecLen * j);
        //   UB → Reg: other[j*64 .. (j+1)*64-1]

        // ⑥ 计算 fmod + other (修正值)
        Reg::Add(addValue, fmodResValue, inputX2Value, preg);
        //   addValue[i] = fmodRes[i] + other[i]

        // ⑦ 提取 other 的符号位
        Reg::And(inputX2signValue, (RegTensor<uint32_t>&)inputX2Value, signValue, preg);
        //   inputX2signValue[i] = other[i] & 0x80000000

        // ⑧ 判断 fmod 与 other 符号是否不同
        Reg::Compare<uint32_t, CMPMODE::NE>(signNegValue, fmodSignValue, inputX2signValue, preg);
        //   signNegValue[i] = (fmod符号 != other符号)

        // ⑨ 综合条件: 非零 且 符号不同
        Reg::MaskAnd(resMaskValue, signNegValue, negValue, preg);
        //   resMaskValue[i] = signNegValue[i] AND negValue[i]

        // ⑩ 条件选择: 条件为真选 addValue (修正), 否则选 fmodResValue (原值)
        Reg::Select(resValue, addValue, fmodResValue, resMaskValue);
        //   resValue[i] = resMaskValue[i] ? addValue[i] : fmodResValue[i]

        // ⑪ 写回 UB
        Reg::DataCopy<T, StoreDist::DIST_NORM>(dstAddr + vecLen * j, resValue, preg);
        //   Reg → UB: dst[j*64 .. (j+1)*64-1]
    }
}
```

### 6.4 SIMD 单次循环的数据流图

```
以第 j 次循环为例 (处理第 j*64 到 (j+1)*64-1 号元素):

UB                                           寄存器 (Reg)
┌─────────────────────┐                      ┌──────────────────────┐
│ fmodRes[j*64..+64]  │ ──DataCopy(Load)──►  │ fmodResValue         │
│                     │                      │                      │
│ other[j*64..+64]    │ ──DataCopy(Load)──►  │ inputX2Value         │
│                     │                      │                      │
│ dst[j*64..+64]      │ ◄──DataCopy(Store)─  │ resValue             │
└─────────────────────┘                      └──────────────────────┘

寄存器内计算流水:
  fmodResValue ──┬── Compare(≠0) ──► negValue ──┐
                 │                               │
  fmodResValue ──┼── And(&sign) ──► fmodSign ──┬── Compare(≠) ──► signNeg ──┐
                 │                              │                             │
  inputX2Value ──┼── And(&sign) ──► x2sign ────┘                             │
                 │                                                           │
  inputX2Value ──┴── Add(fmodRes) ──► addValue ──┐                          │
                                                 │                          │
                            Select ◄─────────────┤── MaskAnd ◄──────────────┘
                              │
                              ▼
                           resValue ──► Store ──► UB
```

### 6.5 16 次循环的完整追踪

```
vecLen = 64, loopTimes = 16, count = 1024

┌──────┬───────────────┬──────────────┬───────────────────┬──────────────┐
│ j    │ fmodRes偏移    │ other偏移     │ dst偏移            │ sregMask     │
│      │ (float元素)    │ (float元素)  │ (float元素)       │ (剩余元素)   │
├──────┼───────────────┼──────────────┼───────────────────┼──────────────┤
│ 0    │ 0             │ 0            │ 0                 │ 1024 → 960   │
│ 1    │ 64            │ 64           │ 64                │ 960 → 896    │
│ 2    │ 128           │ 128          │ 128               │ 896 → 832    │
│ ...  │ ...           │ ...          │ ...               │ ...          │
│ 15   │ 960           │ 960          │ 960               │ 64 → 0       │
└──────┴───────────────┴──────────────┴───────────────────┴──────────────┘

每次循环:
  从 UB 读取: 2 × 64 = 128 个 float (fmodRes + other) = 512 Bytes
  向 UB 写出: 1 × 64 = 64 个 float (dst) = 256 Bytes
  寄存器计算: 9 步向量操作 (2×Load + 2×And + 1×Add + 2×Compare + 1×MaskAnd + 1×Select + 1×Store)
```

### 6.6 floor_mod_int_simd 寄存器级数据流

int32_t 场景的 SIMD 版本（`floor_mod_int_simd`）虽然在本样例中未直接调用，但其实现展示了更复杂的 SIMD 计算模式：

```
算法: floor_mod(self, other) = self - floor(self/other) * other

步骤:
  1. div = self / other      (整除, 向零取整)
  2. mul = other * div
  3. mod = self - mul        (即 C 语言的 % 结果)
  4. 若 mod == 0: 结果 = mod
     若 mod 与 other 符号不同且 mod ≠ 0: 结果 = mod + other
     否则: 结果 = mod

寄存器计算流水:
  input1(self) ──┐
                 ├── Div ──► divValue ──┐
  input2(other) ─┘                      ├── Mul ──► mulValue
                                         │
  input1(self) ──────────────────────────┴── Sub ──► modValue
                                                              │
  input2(other) ──┬── And(&sign) ──► input2Sign ──┐         │
                  │                                 │         │
  modValue ───────┼── And(&sign) ──► modSign ──────┤         │
                  │                                 │         │
                  │   Compare(≠) ◄──────────────────┘         │
                  │       │                                    │
                  │   signNegValue ──┐                         │
                  │                  ├── MaskAnd ◄── negValue │
  modValue ── Compare(≠0) ──► negValue              │         │
                  │                       resMaskValue       │
  modValue ───────┼─────┐                                │    │
                  │     ├── Select ◄── resMaskValue ────┘    │
  other ── Add ───┘     │                                    │
              addValue ─┘                                    │
                  │                                          │
              resValue ──► Store ──► UB                      │
```

---

## 七、完整端到端数据流

### 7.1 int32_t 场景（SIMT）

```
┌──────────────────────────────────────────────────────────────────────────┐
│  GM (Global Memory)                                                      │
│                                                                           │
│  self[6400] int32_t     other[6400] int32_t     output[6400] int32_t     │
│  25,600 B               25,600 B                25,600 B                 │
│       │                      │                          ↑                │
└───────┼──────────────────────┼──────────────────────────┼────────────────┘
        │                      │                          │
        ▼                      ▼                          │
┌─────────────────────────────────────────────────────────────────────────┐
│  Kernel::CopyIn — DataCopy (SIMD)                                       │
│                                                                          │
│  每个 Core 从 GM 读取 1024 个 int32_t = 4096 Bytes                      │
│  偏移: coreIdx * 1024                                                    │
│                                                                          │
│  GM → UB:                                                                │
│    selfGm[coreIdx*1024]  → inQueueSelf  [1024 int32_t]                  │
│    otherGm[coreIdx*1024] → inQueueOther [1024 int32_t]                  │
│                                                                          │
│  同步: SetFlag<HardEvent::MTE2_V> + WaitFlag                             │
└─────────────────────────────────────────────────────────────────────────┘
        │                      │                          │
        ▼                      ▼                          │
┌─────────────────────────────────────────────────────────────────────────┐
│  Kernel::Compute — SIMT 路径 (int32_t)                                  │
│                                                                          │
│  asc_vf_call<floor_mod_simt<int32_t>>(dim3(1024), ...)                  │
│                                                                          │
│  UB 布局:                                                                │
│  ┌─────────────────────────────────────────────────────────────┐        │
│  │ inQueueSelf:  self[0..1023]    int32_t                      │        │
│  │ inQueueOther: other[0..1023]   int32_t                      │        │
│  │ outQueue:     out[0..1023]     int32_t                      │        │
│  └─────────────────────────────────────────────────────────────┘        │
│                                                                          │
│  1024 个线程并行:                                                        │
│    Thread i:  index = i                                                  │
│      rem = self[i] % other[i]                                            │
│      signs_differ = (rem<0) != (other[i]<0)                             │
│      out[i] = signs_differ && rem!=0 ? rem + other[i] : rem             │
│                                                                          │
│  每线程: 1次 UB 读取 self[i] + 1次 UB 读取 other[i] + 1次 UB 写出 out[i] │
│  总计: 1024 次标量读 + 1024 次标量读 + 1024 次标量写                      │
└─────────────────────────────────────────────────────────────────────────┘
        │
        ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  Kernel::CopyOut — DataCopy (SIMD)                                      │
│                                                                          │
│  同步: SetFlag<HardEvent::V_MTE3> + WaitFlag                             │
│                                                                          │
│  UB → GM:                                                                │
│    outQueue → outGm[coreIdx*1024]   1024 int32_t = 4096 Bytes           │
└─────────────────────────────────────────────────────────────────────────┘
```

### 7.2 float 场景（SIMD + Fmod）

```
┌──────────────────────────────────────────────────────────────────────────┐
│  GM (Global Memory)                                                      │
│                                                                           │
│  self[6400] float       other[6400] float        output[6400] float     │
│  25,600 B               25,600 B                25,600 B                 │
│       │                      │                          ↑                │
└───────┼──────────────────────┼──────────────────────────┼────────────────┘
        │                      │                          │
        ▼                      ▼                          │
┌─────────────────────────────────────────────────────────────────────────┐
│  Kernel::CopyIn — DataCopy (SIMD)                                       │
│    selfGm  → inQueueSelf   [1024 float]                                │
│    otherGm → inQueueOther  [1024 float]                                │
│    同步: MTE2_V                                                          │
└─────────────────────────────────────────────────────────────────────────┘
        │                      │                          │
        ▼                      ▼                          │
┌─────────────────────────────────────────────────────────────────────────┐
│  Kernel::Compute — SIMD 路径 (float)                                    │
│                                                                          │
│  ┌─ 阶段1: Fmod 高级 API ──────────────────────────────────────────┐    │
│  │  Fmod(fmodRes, selfLocal, otherLocal)                            │    │
│  │  输入: selfLocal[1024] + otherLocal[1024] (UB → 内部自动搬运)    │    │
│  │  输出: fmodRes[1024] (额外 UB 空间)                              │    │
│  │  计算: fmodRes[i] = self[i] - trunc(self[i]/other[i]) * other[i] │    │
│  └──────────────────────────────────────────────────────────────────┘    │
│                          │                                               │
│                          ▼                                               │
│  ┌─ 阶段2: floor_mod_float_simd ───────────────────────────────────┐    │
│  │  asc_vf_call<floor_mod_float_simd<float>>(...)                   │    │
│  │                                                                  │    │
│  │  16 次寄存器循环:                                                │    │
│  │  ┌────────────────────────────────────────────────────────────┐  │    │
│  │  │ j=0:  fmodRes[0..63]  + other[0..63]  → dst[0..63]       │  │    │
│  │  │ j=1:  fmodRes[64..127] + other[64..127] → dst[64..127]   │  │    │
│  │  │ ...                                                        │  │    │
│  │  │ j=15: fmodRes[960..1023] + other[960..1023] → dst[960..1023] │
│  │  └────────────────────────────────────────────────────────────┘  │    │
│  │                                                                  │    │
│  │  每次循环:                                                       │    │
│  │    UB→Reg: 2×64 float = 512 B (fmodRes + other)                 │    │
│  │    Reg计算: Compare+And+Add+MaskAnd+Select                       │    │
│  │    Reg→UB: 1×64 float = 256 B (dst)                             │    │
│  └──────────────────────────────────────────────────────────────────┘    │
│                                                                          │
│  UB 布局 (float 场景):                                                  │
│  ┌─────────────────────────────────────────────────────────────┐        │
│  │ inQueueSelf:   self[0..1023]     4096 B                      │        │
│  │ inQueueOther:  other[0..1023]    4096 B                      │        │
│  │ outQueue:      out[0..1023]      4096 B                      │        │
│  │ fmodRes:       fmod[0..1023]     4096 B (额外分配)            │        │
│  └─────────────────────────────────────────────────────────────┘        │
└─────────────────────────────────────────────────────────────────────────┘
        │
        ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  Kernel::CopyOut — DataCopy (SIMD)                                      │
│    outQueue → outGm[coreIdx*1024]   1024 float = 4096 Bytes            │
│    同步: V_MTE3                                                          │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 八、SIMT 与 SIMD 混合编程的关键设计

### 8.1 数据搬入/搬出统一使用 SIMD

无论是 SIMT 还是 SIMD 计算路径，数据搬入（GM→UB）和搬出（UB→GM）都使用 SIMD 的 `DataCopy` 接口：

```
原因:
  1. DataCopy 是 DMA 引擎操作, 与计算单元并行, 效率高
  2. SIMT 线程不适合做批量数据搬运, 它是标量执行模型
  3. 统一使用 DataCopy 简化编程, 搬运逻辑与计算逻辑解耦
```

### 8.2 计算路径选择策略

```
┌──────────────┬──────────────────────────────────────────────────┐
│ 数据类型      │ 计算路径选择原因                                    │
├──────────────┼──────────────────────────────────────────────────┤
│ int32_t      │ SIMT: 整型取模(%)是标量操作, SIMT 每线程1个元素    │
│              │ 无需手动循环, 代码简洁, 且硬件对整型标量操作高效      │
│              │ SIMD 对整型没有直接的取模向量指令, 实现复杂           │
├──────────────┼──────────────────────────────────────────────────┤
│ float        │ SIMD: 浮点取模有 Fmod 高级 API 支持                │
│              │ 后续的符号修正通过向量寄存器操作批量完成              │
│              │ 64个float/循环 的吞吐优于 SIMT 的逐元素处理         │
└──────────────┴──────────────────────────────────────────────────┘
```

### 8.3 `__ubuf__` 地址转换

```cpp
__ubuf__ T* selfAddr = (__ubuf__ T*)selfLocal.GetPhyAddr();
```

这是 SIMT/SIMD 函数与 Ascend C 队列系统的桥梁：
- `LocalTensor<T>` 是 Ascend C 的 UB 张量抽象
- `GetPhyAddr()` 获取 UB 物理地址
- `(__ubuf__ T*)` 转为 SIMT/SIMD 可识别的 UB 指针类型
- `asc_vf_call` 接收 `__ubuf__` 指针, 使 SIMT/SIMD 函数直接在 UB 上操作

### 8.4 事件同步机制

```
CopyIn 与 Compute 之间:
  SetFlag<HardEvent::MTE2_V>(0)   — 标记数据搬运完成
  WaitFlag<HardEvent::MTE2_V>(0)  — 等待数据搬运完成才进入计算

Compute 与 CopyOut 之间:
  SetFlag<HardEvent::V_MTE3>(0)   — 标记计算完成
  WaitFlag<HardEvent::V_MTE3>(0)  — 等待计算完成才搬出数据

时序:
  MTE2(搬运入) → V(计算) → MTE3(搬演出)
       │              │            │
       └─ MTE2_V ─────┘── V_MTE3 ─┘
```

---

## 九、floor_mod 算法详解

### 9.1 数学定义

```
floor_mod(self, other) = self - floor(self / other) * other

其中 floor() 是向负无穷取整:
  floor(2.3) = 2
  floor(-2.3) = -3  (不是 -2!)
```

### 9.2 与 C 语言 `%` 的关系

```
C 语言 % (向零取整, truncation division):
  self % other = self - trunc(self / other) * other

差异仅在 self/other 为负数时:
  trunc(-2.3) = -2   vs   floor(-2.3) = -3

修正规则:
  rem = self % other  (C 语言 %)
  若 rem ≠ 0 且 sign(rem) ≠ sign(other):
    floor_mod = rem + other
  否则:
    floor_mod = rem
```

### 9.3 float 场景的符号位判断实现

```
FMOD_B32_SIGN = 0x80000000  (IEEE 754 float 符号位掩码)

提取符号位:
  fmodSign = fmodRes & 0x80000000   → 提取 fmod 结果的符号位
  x2Sign   = other   & 0x80000000   → 提取 other 的符号位

比较符号位:
  signNegValue = (fmodSign != x2Sign)  → 符号不同

综合条件:
  resMaskValue = signNegValue AND negValue
    其中 negValue = (fmodRes != 0)

条件选择:
  resValue = resMaskValue ? (fmodRes + other) : fmodRes
```

---

## 十、切分策略总结

### 10.1 多核切分

|| 维度 | 切分粒度 | 切分次数 | 说明 |
||------|---------|---------|------|
|| 核间 | DATA_LEN_PER_CORE=1024 | ceil(6400/1024)=7 核 | 每个 Core 处理连续 1024 个元素 |

### 10.2 核内切分

|| 数据类型 | 编程范式 | 切分粒度 | 循环次数 | 说明 |
||---------|---------|---------|---------|------|
|| int32_t | SIMT | 1 元素/线程 | 无显式循环 | 1024 线程, 每线程 1 元素 |
|| float | SIMD | 64 float/循环 | 16 次 | 寄存器 256B = 64 float |

### 10.3 数据搬运量（单核）

|| 传递路径 | 单次大小 | 传递次数 | 总大小 | 说明 |
||---------|---------|---------|--------|------|
|| GM→UB (self) | 4096 B | 1 | 4096 B | DataCopy |
|| GM→UB (other) | 4096 B | 1 | 4096 B | DataCopy |
|| UB→GM (output) | 4096 B | 1 | 4096 B | DataCopy |

**float 场景额外搬运**：

|| 传递路径 | 单次大小 | 传递次数 | 总大小 | 说明 |
||---------|---------|---------|--------|------|
|| UB→Reg (fmodRes) | 256 B | 16 | 4096 B | 寄存器 Load |
|| UB→Reg (other) | 256 B | 16 | 4096 B | 寄存器 Load |
|| Reg→UB (dst) | 256 B | 16 | 4096 B | 寄存器 Store |

---

## 十一、完整执行计数

```
总输入元素:                    6400
核数:                         7
每核元素数:                    1024 (最后一核仅 256 个有效)

int32_t 场景 (SIMT):
  每核线程数:                  1024
  每核计算量:                  1024 次标量 %
  总计算量:                    7 × 1024 = 7168 次标量操作 (含尾部核多余计算)

float 场景 (SIMD):
  每核寄存器循环:              16
  每核计算量:                  16 × 64 = 1024 元素
  每核寄存器操作:              16 × 9 = 144 步向量操作
  总计算量:                    7 × 16 = 112 次循环

数据搬运量 (全核):
  GM→UB:  7 × 2 × 4096 = 57,344 B (self + other)
  UB→GM:  7 × 4096 = 28,672 B (output)
```
