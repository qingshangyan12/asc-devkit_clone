# asc_atomic_sub (half / bfloat16_t 单类型) 测试报告

## 1. 被测对象

新增的 `asc_atomic_sub` 接口，支持 `half` 和 `bfloat16_t` 单类型，通过 `asc_atomic_add(address, -val)` 实现。

### 1.1 实现代码

**half（impl/simt_api/asc_fp16_impl.h）— 3 个变体：**
```cpp
// UB
__SIMT_DEVICE_FUNCTIONS_DECL__ inline half asc_atomic_sub(__ubuf__ half* address, half val)
{
    return asc_atomic_add(address, -val);
}
// GM
__SIMT_DEVICE_FUNCTIONS_DECL__ inline half asc_atomic_sub(__gm__ half* address, half val)
{
    return asc_atomic_add(address, -val);
}
// pure SIMT
__SIMT_DEVICE_FUNCTIONS_DECL__ inline half asc_atomic_sub(half* address, half val)
{
    return asc_atomic_add(address, -val);
}
```

**bfloat16_t（impl/simt_api/asc_bf16_impl.h）— 3 个变体：**
```cpp
// UB
__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16_t asc_atomic_sub(__ubuf__ bfloat16_t* address, bfloat16_t val)
{
    return asc_atomic_add(address, -val);
}
// GM
__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16_t asc_atomic_sub(__gm__ bfloat16_t* address, bfloat16_t val)
{
    return asc_atomic_add(address, -val);
}
// pure SIMT
__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16_t asc_atomic_sub(bfloat16_t* address, bfloat16_t val)
{
    return asc_atomic_add(address, -val);
}
```

### 1.2 被调用的 asc_atomic_add 底层实现

**half asc_atomic_add（已有的、未被修改的代码）：**
```cpp
// UB (非 pure, #ifndef ASCENDC_CPU_DEBUG 内)
__SIMT_DEVICE_FUNCTIONS_DECL__ inline half asc_atomic_add(__ubuf__ half* address, half val)
{
    atomicAdd(address, val);   // 编译器内置 __builtin_cce_atom_add_S_half
    return *address;
}
// GM (非 pure, 始终定义)
__SIMT_DEVICE_FUNCTIONS_DECL__ inline half asc_atomic_add(__gm__ half* address, half val)
{
    atomicAdd(address, val);   // 编译器内置 __builtin_cce_atom_add_G_half
    return *address;
}
```

**bfloat16_t asc_atomic_add（已有的、未被修改的代码）：**
```cpp
// UB (非 pure, #ifndef ASCENDC_CPU_DEBUG 内)
__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16_t asc_atomic_add(__ubuf__ bfloat16_t* address, bfloat16_t val)
{
    atomicAdd(address, val);   // 编译器内置 __builtin_cce_atom_add_S_bf16
    return *address;
}
// GM (非 pure, 始终定义)
__SIMT_DEVICE_FUNCTIONS_DECL__ inline bfloat16_t asc_atomic_add(__gm__ bfloat16_t* address, bfloat16_t val)
{
    atomicAdd(address, val);   // 编译器内置 __builtin_cce_atom_add_G_bf16
    return *address;
}
```

### 1.3 编译器内置原语

| 地址空间 | half | bfloat16_t |
|----------|------|------------|
| GM (`__gm__`) | `__builtin_cce_atom_add_G_half` | `__builtin_cce_atom_add_G_bf16` |
| UB (`__ubuf__`) | `__builtin_cce_atom_add_S_half` | `__builtin_cce_atom_add_S_bf16` |

内置声明来源：`__clang_cce_simt_bf16.h` / `__clang_cce_simt_fp16.h`

---

## 2. 测试环境

| 项目 | 值 |
|------|-----|
| 硬件架构 | dav-3510 (Ascend 950PR / 950DT) |
| CANN 版本 | cann-9.2.0 |
| 编译器 | bisheng (clang 15.0.5, clang-5c68a1cb1231) |
| 编译选项 | `--npu-arch=dav-3510`（混编模式，不带 `--enable-simt`） |
| 编程模式 | SIMD+SIMT 混编（`__simt_vf__` + `asc_vf_call` + 3-slot `<<<>>>` launch） |
| 头文件来源 | repo `/home/qxk/asc-devkit/.worktree/master`（软链到 CANN 安装目录） |

---

## 3. 测试设计

### 3.1 bf16 精度步长计算

bfloat16_t 格式：**1 位符号 + 8 位指数 + 7 位尾数**（隐含 1，有效尾数 8 位）。

对 `INIT_VAL = 1000.0`：
- `2^9 = 512 ≤ 1000 < 1024 = 2^10` → 指数 e = 9
- **ULP (单位最后一位) = 2^(e-7) = 2^(9-7) = 2^2 = 4**
- 1000.0 的 bf16 位模式 = `0_10001000_1111010` = `0x447A`（host 侧实测确认）

因此：
- half 步长用 **1**（half 在 1000 附近 ULP 远小于 1，精度足够）
- bf16 步长用 **4**（= ULP，保证每次减法能实际改变 bf16 值，不会被舍入吞掉）

### 3.2 测试参数

```
THREAD_COUNT = 256       // 每个原子操作启动 256 个线程并发执行
INIT_VAL     = 1000.0    // 初始值（host 侧写入 GM，device 侧写入 UB）
HALF_SUB_VAL = 1.0       // half 每线程减 1
BF16_SUB_VAL = 4.0       // bf16 每线程减 4（= ULP）
```

期望值：
- half:  1000 - 256 × 1 = **744**
- bf16:  1000 - 256 × 4 = **-24**

### 3.3 测试结构

4 个**独立的 `__global__` kernel**，各自只测一种类型+地址空间组合，排除互相影响：

| kernel | 测试内容 | init 方式 | readback 方式 |
|--------|----------|-----------|---------------|
| `test_gm_half_kernel` | GM half sub | host `aclrtMemcpy` | VF 函数直接读 `gm_h[0]` |
| `test_gm_bf16_kernel` | GM bf16 sub | host `aclrtMemcpy`（4字节，高16位 padding=0） | VF 函数直接读 `gm_b[0]` |
| `test_ub_half_kernel` | UB half sub | VF 函数 `simt_ub_init_half` | VF 函数直接读 `ub_h[0]` |
| `test_ub_bf16_kernel` | UB bf16 sub | VF 函数 `simt_ub_init_bf16` | VF 函数直接读 `ub_b[0]` |

每个 kernel 内部用 `asc_vf_call` 启动 VF 函数，不使用接口返回值，直接从 address 读取结果。

### 3.4 关键设计决策

1. **host 侧初始化 GM**：用 `aclrtMemcpy` 从 host 写入初值，避免 device 侧 init 的潜在问题。GM bf16 写入 4 字节（bf16 低 16 位 + 0x0000 高 16 位 padding）。
2. **不使用接口返回值**：readback VF 函数直接从 address 读取，不依赖 `asc_atomic_sub` 的返回值。
3. **GM/UB 独立 kernel**：4 个 kernel 各自独立，排除 GM 和 UB 原子操作混跑互相影响。
4. **bf16 步长 = ULP**：用 4（= 2^2）而非 1，避免 bf16 精度不足导致减法被舍入吞掉。

---

## 4. 测试结果

### 4.1 最终结果（修正 host 侧初始化后）

```
[ PASS ] GM half: got=744.00 expected=744.00
[ PASS ] GM bfloat16_t: got=-24.00 expected=-24.00
[ PASS ] UB half: got=744.00 expected=744.00
[ PASS ] UB bfloat16_t: got=-24.00 expected=-24.00
=== ALL PASS ===
```

| 类型 | GM | UB |
|------|-----|------|
| half (步长1) | ✅ PASS (744) | ✅ PASS (744) |
| bfloat16_t (步长4=ULP) | ✅ PASS (-24) | ✅ PASS (-24) |

### 4.2 修正前的现象及其定位过程

初版测试中 GM bf16 得 `-1024`（期望 `-24`）。该失败是**测试代码缺陷**，与被测接口无关，定位过程如下。

初版 host 侧初始化写法为：

```cpp
bfloat16_t b_init = (bfloat16_t)INIT_VAL;              // 期望得到 bf16(1000.0)
std::memcpy(&b_init32, &b_init, sizeof(bfloat16_t));
```

增加一路诊断，在原子操作**之前**回读 GM 首地址的原始 16 位，并同时对照 device 侧初始化：

```
[host] (bfloat16_t)1000.0f raw bits = 0x0000
[host] correct bf16(1000.0)  bits   = 0x447A
[host] host cast is *** WRONG (no real conversion) ***

[Case A] host 侧初始化 GM（初版测试写法）
  device 端看到的原始位 = 0x0000（bf16(1000.0) 应为 0x447A）
  原子操作前 = 0.00   256 线程 sub(4) 后 = -1024.00   （delta = -1024.00）

[Case B] device 侧初始化 GM
  原子操作前 = 1000.00   256 线程 sub(4) 后 = -24.00   （delta = -1024.00）
```

即 GM 中的初值实际是 **0 而非 1000**。两种初始化方式下 delta 均为 `-1024`（= 256 × 4），恰好证明原子累加本身完全正确。

### 4.3 根因：host 侧 `bfloat16_t` 转换是空操作

`__clang_cce_types.h` 中，host 编译（`#ifndef __CCE_AICORE__` 分支，L178）下：

```cpp
typedef bisheng::BISHENG_BUILTIN_TYPE_PROXY(BFloat16) bfloat16_t;
```

该 proxy 由 `DEFINE_BISHENG_BUILTIN_TYPE_PROXY(BFloat16, 2)` 展开为一个只含 `char Storage[2]` 的不透明结构体，其唯一可用构造函数是**空实现**的 `PROXY(int x) {}`。因此 host 侧 `(bfloat16_t)1000.0f` 能正常编译，但**不做任何数值转换**，Storage 保持为 `0x0000`。

device 侧 `bfloat16_t` 是真正的 `__bf16`，同样的转换是有效的，所以 device 侧初始化（Case B）正常。

`half` 不受影响：host 侧 `half` 是 `__cce_half`，为真实内置类型，转换有效。这解释了为何同一测试中 half 全部 PASS，唯独 GM bf16 失败。

### 4.4 初版报告中各观测值的正确解释

初版报告将上述现象归因于编译器内置 `__builtin_cce_atom_add_G_bf16` 行为异常（"写入 val 而非 old+val，类似 atomicExch"）。该结论不成立——所有观测值在"初值为 0"的前提下都是**正确**的：

| 初版观测 | 实际解释 |
|----------|----------|
| 256 线程 → `-1024` | `0 - 256×4 = -1024`，正确 |
| 单线程 `sub(4)` → `-4` | `0 - 4 = -4`，正确，非"写入 -val" |
| 单线程 `add(+8)` → `8` | `0 + 8 = 8`，正确，非 atomicExch 行为 |
| 直接调内置 `atomicAdd` 仍异常 | 初值同样为 0，故现象一致 |

初版排除项表中"host init 问题"一项被误排除：其依据是"UB bf16 PASS 且 GM init 一致"的推理，而非实测 GM 中的原始位。UB 走的是 device 侧初始化，与 GM 的 host 侧初始化并不同路，该推理不成立。

### 4.5 修正方案

host 侧改为从 float 位模式构造 bf16（取 IEEE-754 float 高 16 位，对截断部分做 round-to-nearest-even），并加一条自检确保位模式能正确回解：

```cpp
uint16_t float_to_bf16_bits(float val)
{
    uint32_t bits = 0;
    std::memcpy(&bits, &val, sizeof(uint32_t));
    uint32_t lsb = (bits >> 16) & 1u;
    uint32_t round_bias = 0x7FFFu + lsb;
    return (uint16_t)((bits + round_bias) >> 16);
}
```

---

## 5. 结论

| 结论 | 说明 |
|------|------|
| `asc_atomic_sub` half 实现 | ✅ 正确（GM + UB 全 PASS） |
| `asc_atomic_sub` bf16 实现 | ✅ 正确（GM + UB 全 PASS） |
| 编译器内置 GM bf16 atomicAdd | ✅ 行为正常，初版报告的"异常"结论已推翻 |
| 初版 GM bf16 FAIL | 测试代码缺陷：host 侧 `(bfloat16_t)float` 转换为空操作，初值实为 0 |

**通用注意事项**：编写涉及 bf16 的 host/device 联合测试时，不要用 host 侧强制转换构造 bf16 初值。定位此类失败时应直接回读目标地址在操作前的原始位，而非依据"同类用例通过"推断初始化正确；另外可观察 delta——delta 正确而绝对值错误，通常指向初始化问题。

---

## 6. 复现步骤

```bash
# 1. source 环境
source /home/qxk/0722pkg/cann/set_env.sh

# 2. 确认 simt_api 软链指向 repo（含新增 asc_atomic_sub）
ls -l /home/qxk/0722pkg/cann/asc/include/simt_api  # 应指向 repo

# 3. 编译
cd /home/qxk/asc-devkit/.worktree/master/test_atomic_sub
rm -rf build && mkdir build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j

# 4. 运行
./test_atomic_sub
```

期望输出：
```
[ PASS ] GM half: got=744.00 expected=744.00
[ PASS ] GM bfloat16_t: got=-24.00 expected=-24.00
[ PASS ] UB half: got=744.00 expected=744.00
[ PASS ] UB bfloat16_t: got=-24.00 expected=-24.00
=== ALL PASS ===
```

---

## 7. 测试文件位置

- 测试源码：`/home/qxk/asc-devkit/.worktree/master/test_atomic_sub/test_atomic_sub.asc`
- CMake：`/home/qxk/asc-devkit/.worktree/master/test_atomic_sub/CMakeLists.txt`
- 被测实现：
  - `impl/simt_api/asc_fp16_impl.h`（half asc_atomic_sub，L857/L911/L1012）
  - `impl/simt_api/asc_bf16_impl.h`（bf16 asc_atomic_sub，L931/L986/L1088）
- 被测声明：
  - `include/simt_api/asc_fp16.h`（L303/L305/L355）
  - `include/simt_api/asc_bf16.h`（L313/L315/L367）

---

# 边界值测试补充报告（half / bfloat16_t，混编 + pure SIMT）

本节依据 `BOUNDARY_TEST_DESIGN.md` 对 `asc_atomic_sub` 做边界值（零、次正规数、溢出、inf、NaN）测试，并在**混编**与 **pure SIMT** 两种编程形态下各跑一遍。第 1–7 节为既有并发测试报告，其记录的踩坑（host 侧 bf16 转换为空操作、需位级校验）已作为本节实现的强制约束。

## 8. 测试依据与范围

被测接口：`asc_atomic_sub(address, val)`，`T ∈ {half, bfloat16_t}`，实现为 `asc_atomic_add(address, -val)`，因此测试同时覆盖"取负 + 累加"路径。

| 形态 | 被测过载 | 地址空间 | 编译选项 |
|------|----------|----------|----------|
| 混编 | `asc_atomic_sub(__gm__ T*, T)` / `asc_atomic_sub(__ubuf__ T*, T)` | GM + UB | `--npu-arch=dav-3510`（不带 `--enable-simt`） |
| pure SIMT | `asc_atomic_sub(T*, T)`（非限定指针 → GM） | GM | `--npu-arch=dav-3510 --enable-simt` |

底层实现差异：混编走 `atomicAdd` 编译器内置（`__builtin_cce_atom_add_G/S_*`）；pure SIMT 走 `__atomic_add`。两种路径分别验证。

## 9. 测试设计（遵从设计文档三条约束）

1. **禁止 host 侧构造 bf16/half 值**：所有目标初值与操作数一律以 `uint16_t` 原始位模式在 host 给出，经 GM 传入 device，device 侧用位重解释 `(bfloat16_t&)bits` / `(half&)bits`。
2. **校验比对原始位**：回读一律读 `uint16_t` 原始位（`±0`、`NaN` 必须位比对）。
3. **host 侧位模式自检**：跑 kernel 前先把每个位常量解回 float 与预期数值比对，不一致直接 `FATAL` 退出。

- **数据布局**：单块 GM 缓冲，`uint16_t` 索引：`operand[]`@0、`target[]`@64（stride 4）、`result[]`@256（stride 4）。
- **单线程执行**：边界用例一律 `dim3(1)` 且 `if (threadIdx.x == 0)` 保护，要的是确定性结果；多线程竞争由第 1–7 节并发用例覆盖。
- **邻位哨兵**：每个 target 的 `+1` 位置预置 `0xA5A5`，操作后校验仍为 `0xA5A5`，验证 2 字节原子不越界写相邻半字。
- **匹配模式**：`EXACT`（位精确，含 ±0 区分）、`IS_NAN`（指数全 1 且尾数非 0）、`IS_INF`（指数全 1 且尾数 0，符号需匹配）。NaN payload 由硬件决定，不断言。
- **返回值**：仅记录、不判定（语义待定，见第 12 节）。

## 10. 用例表与期望

`SUB(t, v)` 表示 `asc_atomic_sub(&target, val)`，target 初值位 `t`、操作数位 `v`。

### 10.1 bfloat16_t（1-8-7）

| # | 用例 | target | val | 期望内存位 | 匹配模式 | 考察点 |
|---|------|--------|-----|-----------|----------|--------|
| B1 | 基线 | 0x447A (1000) | 0x4100 (8) | 0x4478 (992) | EXACT | 正常路径对照 |
| B2 | 自减归零 | 0x447A | 0x447A | 0x0000 (+0) | EXACT | 结果为 +0 而非 -0 |
| B3 | 低于 ULP 被吸收 | 0x447A | 0x3080 (2⁻³⁰) | 0x447A | EXACT | 舍入吸收 |
| B4 | 负零减正零 | 0x8000 (-0) | 0x0000 (+0) | 0x8000 (-0) | EXACT | 符号零保持 |
| B5 | 溢出至 -inf | 0xFF7F (-max) | 0x7F7F (+max) | 0xFF80 | IS_INF(neg) | 有限值相减溢出 |
| B6 | 有限减 +inf | 0x447A | 0x7F80 (+inf) | 0xFF80 | IS_INF(neg) | inf 操作数传播 |
| B7 | +inf 减有限 | 0x7F80 | 0x4100 | 0x7F80 | IS_INF(pos) | inf 目标保持 |
| B8 | inf 减 inf | 0x7F80 | 0x7F80 | — | IS_NAN | 未定义式 |
| B9 | NaN 目标 | 0x7FC0 (qNaN) | 0x4100 | — | IS_NAN | NaN 目标传播 |
| B10 | NaN 操作数 | 0x447A | 0x7FC0 (qNaN) | — | IS_NAN | NaN 经取负后传播 |
| B11 | 最小正规数自减 | 0x0080 (2⁻¹²⁶) | 0x0080 | 0x0000 (+0) | EXACT | 正规数下边界 |

### 10.2 half（1-5-10）

| # | 用例 | target | val | 期望内存位 | 匹配模式 | 考察点 |
|---|------|--------|-----|-----------|----------|--------|
| H1 | 基线 | 0x63D0 (1000) | 0x4800 (8) | 0x63C0 (992) | EXACT | 正常路径 |
| H2 | 自减归零 | 0x63D0 | 0x63D0 | 0x0000 (+0) | EXACT | 结果为 +0 |
| H3 | 次正规操作数被吸收 | 0x63D0 | 0x0001 (2⁻²⁴) | 0x63D0 | EXACT | 次正规数不被当 0/异常 |
| H4 | 负零减正零 | 0x8000 (-0) | 0x0000 (+0) | 0x8000 (-0) | EXACT | 符号零保持 |
| H5 | 溢出至 -inf | 0xFBFF (-65504) | 0x7BFF (65504) | 0xFC00 | IS_INF(neg) | half 动态范围窄易溢出 |
| H6 | 有限减 +inf | 0x63D0 | 0x7C00 (+inf) | 0xFC00 | IS_INF(neg) | inf 传播 |
| H7 | +inf 减有限 | 0x7C00 | 0x4800 | 0x7C00 | IS_INF(pos) | inf 保持 |
| H8 | inf 减 inf | 0x7C00 | 0x7C00 | — | IS_NAN | 未定义式 |
| H9 | NaN 目标 | 0x7E00 (qNaN) | 0x4800 | — | IS_NAN | NaN 传播 |
| H10 | NaN 操作数 | 0x63D0 | 0x7E00 (qNaN) | — | IS_NAN | NaN 经取负后传播 |
| H11 | 最小正规数自减 | 0x0400 (2⁻¹⁴) | 0x0400 | 0x0000 (+0) | EXACT | 正规数下边界 |

## 11. 测试结果

### 11.1 混编形态（GM/UB × half/bf16，4 组合 × 11 = 44 例）

```
[ PASS ] B1   GM bf16   target=0x447A val=0x4100 -> got=0x4478 (992)  expect=0x4478 EXACT       nbr=OK ret=0x4478 (992)
[ PASS ] B2   GM bf16   target=0x447A val=0x447A -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
[ PASS ] B3   GM bf16   target=0x447A val=0x3080 -> got=0x447A (1000) expect=0x447A EXACT       nbr=OK ret=0x447A (1000)
[ PASS ] B4   GM bf16   target=0x8000 val=0x0000 -> got=0x8000 (-0)   expect=0x8000 EXACT       nbr=OK ret=0x8000 (-0)
[ PASS ] B5   GM bf16   target=0xFF7F val=0x7F7F -> got=0xFF80 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFF80 (-inf)
[ PASS ] B6   GM bf16   target=0x447A val=0x7F80 -> got=0xFF80 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFF80 (-inf)
[ PASS ] B7   GM bf16   target=0x7F80 val=0x4100 -> got=0x7F80 (+inf) expect=0x0000 IS_INF(pos) nbr=OK ret=0x7F80 (+inf)
[ PASS ] B8   GM bf16   target=0x7F80 val=0x7F80 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B9   GM bf16   target=0x7FC0 val=0x4100 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B10  GM bf16   target=0x447A val=0x7FC0 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B11  GM bf16   target=0x0080 val=0x0080 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
--- GM bf16: 11 pass, 0 fail ---
[ PASS ] H1   GM half   target=0x63D0 val=0x4800 -> got=0x63C0 (992)  expect=0x63C0 EXACT       nbr=OK ret=0x63C0 (992)
[ PASS ] H2   GM half   target=0x63D0 val=0x63D0 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
[ PASS ] H3   GM half   target=0x63D0 val=0x0001 -> got=0x63D0 (1000) expect=0x63D0 EXACT       nbr=OK ret=0x63D0 (1000)
[ PASS ] H4   GM half   target=0x8000 val=0x0000 -> got=0x8000 (-0)   expect=0x8000 EXACT       nbr=OK ret=0x8000 (-0)
[ PASS ] H5   GM half   target=0xFBFF val=0x7BFF -> got=0xFC00 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFC00 (-inf)
[ PASS ] H6   GM half   target=0x63D0 val=0x7C00 -> got=0xFC00 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFC00 (-inf)
[ PASS ] H7   GM half   target=0x7C00 val=0x4800 -> got=0x7C00 (+inf) expect=0x0000 IS_INF(pos) nbr=OK ret=0x7C00 (+inf)
[ PASS ] H8   GM half   target=0x7C00 val=0x7C00 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H9   GM half   target=0x7E00 val=0x4800 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H10  GM half   target=0x63D0 val=0x7E00 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H11  GM half   target=0x0400 val=0x0400 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
--- GM half: 11 pass, 0 fail ---
[ PASS ] B1   UB bf16   target=0x447A val=0x4100 -> got=0x4478 (992)  expect=0x4478 EXACT       nbr=OK ret=0x4478 (992)
[ PASS ] B2   UB bf16   target=0x447A val=0x447A -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
[ PASS ] B3   UB bf16   target=0x447A val=0x3080 -> got=0x447A (1000) expect=0x447A EXACT       nbr=OK ret=0x447A (1000)
[ PASS ] B4   UB bf16   target=0x8000 val=0x0000 -> got=0x8000 (-0)   expect=0x8000 EXACT       nbr=OK ret=0x8000 (-0)
[ PASS ] B5   UB bf16   target=0xFF7F val=0x7F7F -> got=0xFF80 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFF80 (-inf)
[ PASS ] B6   UB bf16   target=0x447A val=0x7F80 -> got=0xFF80 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFF80 (-inf)
[ PASS ] B7   UB bf16   target=0x7F80 val=0x4100 -> got=0x7F80 (+inf) expect=0x0000 IS_INF(pos) nbr=OK ret=0x7F80 (+inf)
[ PASS ] B8   UB bf16   target=0x7F80 val=0x7F80 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B9   UB bf16   target=0x7FC0 val=0x4100 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B10  UB bf16   target=0x447A val=0x7FC0 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B11  UB bf16   target=0x0080 val=0x0080 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
--- UB bf16: 11 pass, 0 fail ---
[ PASS ] H1   UB half   target=0x63D0 val=0x4800 -> got=0x63C0 (992)  expect=0x63C0 EXACT       nbr=OK ret=0x63C0 (992)
[ PASS ] H2   UB half   target=0x63D0 val=0x63D0 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
[ PASS ] H3   UB half   target=0x63D0 val=0x0001 -> got=0x63D0 (1000) expect=0x63D0 EXACT       nbr=OK ret=0x63D0 (1000)
[ PASS ] H4   UB half   target=0x8000 val=0x0000 -> got=0x8000 (-0)   expect=0x8000 EXACT       nbr=OK ret=0x8000 (-0)
[ PASS ] H5   UB half   target=0xFBFF val=0x7BFF -> got=0xFC00 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFC00 (-inf)
[ PASS ] H6   UB half   target=0x63D0 val=0x7C00 -> got=0xFC00 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFC00 (-inf)
[ PASS ] H7   UB half   target=0x7C00 val=0x4800 -> got=0x7C00 (+inf) expect=0x0000 IS_INF(pos) nbr=OK ret=0x7C00 (+inf)
[ PASS ] H8   UB half   target=0x7C00 val=0x7C00 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H9   UB half   target=0x7E00 val=0x4800 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H10  UB half   target=0x63D0 val=0x7E00 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H11  UB half   target=0x0400 val=0x0400 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
--- UB half: 11 pass, 0 fail ---
=== MIXED FORM TOTAL: 44 pass, 0 fail => ALL PASS ===
```

### 11.2 pure SIMT 形态（GM × half/bf16，2 组合 × 11 = 22 例）

pure SIMT 形态下被测过载为非限定指针 `asc_atomic_sub(T*, T)`（对应 GM），故仅覆盖 GM。

```
[ PASS ] B1   GM bf16   target=0x447A val=0x4100 -> got=0x4478 (992)  expect=0x4478 EXACT       nbr=OK ret=0x4478 (992)
[ PASS ] B2   GM bf16   target=0x447A val=0x447A -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
[ PASS ] B3   GM bf16   target=0x447A val=0x3080 -> got=0x447A (1000) expect=0x447A EXACT       nbr=OK ret=0x447A (1000)
[ PASS ] B4   GM bf16   target=0x8000 val=0x0000 -> got=0x8000 (-0)   expect=0x8000 EXACT       nbr=OK ret=0x8000 (-0)
[ PASS ] B5   GM bf16   target=0xFF7F val=0x7F7F -> got=0xFF80 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFF80 (-inf)
[ PASS ] B6   GM bf16   target=0x447A val=0x7F80 -> got=0xFF80 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFF80 (-inf)
[ PASS ] B7   GM bf16   target=0x7F80 val=0x4100 -> got=0x7F80 (+inf) expect=0x0000 IS_INF(pos) nbr=OK ret=0x7F80 (+inf)
[ PASS ] B8   GM bf16   target=0x7F80 val=0x7F80 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B9   GM bf16   target=0x7FC0 val=0x4100 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B10  GM bf16   target=0x447A val=0x7FC0 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] B11  GM bf16   target=0x0080 val=0x0080 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
--- GM bf16: 11 pass, 0 fail ---
[ PASS ] H1   GM half   target=0x63D0 val=0x4800 -> got=0x63C0 (992)  expect=0x63C0 EXACT       nbr=OK ret=0x63C0 (992)
[ PASS ] H2   GM half   target=0x63D0 val=0x63D0 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
[ PASS ] H3   GM half   target=0x63D0 val=0x0001 -> got=0x63D0 (1000) expect=0x63D0 EXACT       nbr=OK ret=0x63D0 (1000)
[ PASS ] H4   GM half   target=0x8000 val=0x0000 -> got=0x8000 (-0)   expect=0x8000 EXACT       nbr=OK ret=0x8000 (-0)
[ PASS ] H5   GM half   target=0xFBFF val=0x7BFF -> got=0xFC00 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFC00 (-inf)
[ PASS ] H6   GM half   target=0x63D0 val=0x7C00 -> got=0xFC00 (-inf) expect=0x0000 IS_INF(neg) nbr=OK ret=0xFC00 (-inf)
[ PASS ] H7   GM half   target=0x7C00 val=0x4800 -> got=0x7C00 (+inf) expect=0x0000 IS_INF(pos) nbr=OK ret=0x7C00 (+inf)
[ PASS ] H8   GM half   target=0x7C00 val=0x7C00 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H9   GM half   target=0x7E00 val=0x4800 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H10  GM half   target=0x63D0 val=0x7E00 -> got=0x7FFF (nan)  expect=0x0000 IS_NAN      nbr=OK ret=0x7FFF (nan)
[ PASS ] H11  GM half   target=0x0400 val=0x0400 -> got=0x0000 (0)    expect=0x0000 EXACT       nbr=OK ret=0x0000 (0)
--- GM half: 11 pass, 0 fail ---
=== PURE SIMT FORM TOTAL: 22 pass, 0 fail => ALL PASS ===
```

### 11.3 汇总

| 形态 | GM half | GM bfloat16_t | UB half | UB bfloat16_t | 合计 |
|------|---------|---------------|---------|---------------|------|
| 混编 | 11/11 ✅ | 11/11 ✅ | 11/11 ✅ | 11/11 ✅ | **44/44 ✅** |
| pure SIMT | 11/11 ✅ | 11/11 ✅ | —（非限定过载仅 GM） | — | **22/22 ✅** |

B1–B11、H1–H11 共 22 个用例在两种形态下全部通过，边界行为与 IEEE-754 一致。

## 12. 观察与结论

1. **两种形态结果完全一致**：混编（`atomicAdd` 内置）与 pure SIMT（`__atomic_add`）两条代码路径在所有 22 个边界用例上产出的内存位完全相同，互相印证。

2. **符号零保持（B4/H4）**：`-0 − (+0) = -0`，结果位 `0x8000`。这一项只有位比对能判定——`-0.0f == 0.0f` 为真，float 比较会把正确结果误判为失败。位级校验生效。

3. **次正规数被当数值处理（B3/H3）**：`1000 − 2⁻³⁰/2⁻²⁴ = 1000`（被舍入吸收，值不变），次正规操作数未被当作 0 或触发异常，与 dav-3510 半精度累减路径一致。

4. **溢出与 inf 传播（B5–B7/H5–H7）**：有限值相减溢出格式范围得 `±inf`；含 inf 操作数按 IEEE 传播；inf 目标减有限值保持 inf。全部符合预期。

5. **NaN 规范化（B8–B10/H8–H10）**：所有产生 NaN 的用例结果位均为 `0x7FFF`（bf16: 符号0/指数全1/尾数全1；half: 同）——即硬件输出的是**规范化 qNaN**，输入的 NaN payload（`0x7FC0`/`0x7E00`）未被保留。这与实现走 `add(-val)`、取负翻转 NaN 符号位后再经硬件规整一致；`IS_NAN` 模式只判分类不断言 payload，故通过。结论：**NaN payload 不可控，不应在测试或上层逻辑中依赖具体 payload**。

6. **邻位不受损（贯穿）**：全部 66 例的邻位哨兵 `0xA5A5` 操作后保持不变，证明 half/bf16 的 2 字节原子操作不会越界写相邻 2 字节。这排除了设计文档担心的"GM bf16 按 4 字节写入掩盖 2 字节原子越界写"风险。

7. **返回值 = 新值（操作后内存值）**：每个用例的返回值位与操作后内存位完全相同（如 B1 `ret=0x4478` 即新值 992，B8 `ret=0x7FFF` 即新 NaN），与第 4.3 节探针结论一致——返回的是回读值而非旧值。**返回值未作为断言项**，仅记录；接口文档 `asc_atomic_sub.md`"返回初始数据（旧值）"的表述与实现不符，建议接口负责人确认是更正文档还是更正实现。

8. **host 侧位模式自检有效**：自检先于 kernel 执行，曾拦截到一处 host 解码器（half 次正规解码）实现错误并修正——属于测试装置缺陷而非被测接口问题，修正后自检通过。这印证了设计文档第 3.2 节"自检必须有"的必要性。

| 结论 | 说明 |
|------|------|
| `asc_atomic_sub` half 边界行为 | ✅ 与 IEEE-754 一致（混编 GM/UB + pure GM 全通过） |
| `asc_atomic_sub` bf16 边界行为 | ✅ 与 IEEE-754 一致（混编 GM/UB + pure GM 全通过） |
| 混编 vs pure SIMT | ✅ 两条实现路径边界结果完全一致 |
| 2 字节原子邻位安全 | ✅ 邻位哨兵全程不变 |
| 返回值语义 | ⚠ 返回新值（操作后内存值），与文档"旧值"表述不符，待接口负责人定论 |
| NaN payload | ⚠ 硬件规范化为全 1 尾数 qNaN，payload 不可控，勿依赖 |

## 13. 测试文件与复现

测试文件：
- `test_atomic_sub_boundary_mixed.asc` — 混编形态（4 kernel：GM/UB × half/bf16）
- `test_atomic_sub_boundary_pure.asc` — pure SIMT 形态（2 kernel：GM × half/bf16）
- `CMakeLists.txt` — 新增 `test_atomic_sub_boundary_mixed`（不带 `--enable-simt`）与 `test_atomic_sub_boundary_pure`（带 `--enable-simt`）两个目标

复现步骤：
```bash
source /home/qxk/0722pkg/cann/set_env.sh
cd /home/qxk/asc-devkit/.worktree/master/test_atomic_sub
rm -rf build && mkdir build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make test_atomic_sub_boundary_mixed test_atomic_sub_boundary_pure -j
./test_atomic_sub_boundary_mixed   # 期望: 44 pass, 0 fail => ALL PASS
./test_atomic_sub_boundary_pure    # 期望: 22 pass, 0 fail => ALL PASS
```
