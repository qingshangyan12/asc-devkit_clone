# SIMD&SIMT混合编程UB使用上限样例

## 概述

本样例展示 Ascend C SIMD&SIMT 混合编程场景下 Unified Buffer（UB）的常见使用方式、容量计算口径和越界问题定位思路。样例采用 `__global__ __vector__` kernel 作为入口，通过 `asc_vf_call` 调用 `__simt_vf__` 和 `__simd_vf__`，默认运行 4 个正向 case；UB 越界场景作为 README 中的反例和调试 case，不在默认运行路径中触发。

除 4 个默认正向 case 外，`ub_usage_limit.asc` 还内置了 4 组**实测探针**（probe1 / case3_sweep / case5_probe / platform_query），用于在真实 NPU 上逐字节/逐字（word）实测验证下面四个问题的真实边界，而不是仅停留在理论推导：

1. 没有 SIMT VF 时，SIMD 能使用的 UB 上限能不能到 256KB？
2. SIMT VF 和 SIMD VF 都有静态 UB 时，动态 UB 的申请上限是否等于 `总UB - 静态1 - 静态2`？
3. 动态 UB 越界到不同程度时，是静默通过（数据被覆盖但不报错）还是触发 aicore 运行时错误？
4. Host 侧 `platform_ascendc::PlatformAscendC::GetCoreMemSize(CoreMemType::UB, ...)` 接口获取到的 UB 容量，在无 VF / 有 SIMD VF / 有 SIMT VF 的 kernel 场景下分别是多少，是否会随 kernel 里 VF 使用形态变化？

这 4 组探针的实测方法、命令和输出记录在下面的《实测结论与定位方法》章节中，均可在样例目录下按文中给出的命令直接复现。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- > CANN 9.0.0

## 目录结构介绍

```text
├── simd_simt_ub_usage_limit
│   ├── CMakeLists.txt       // 编译工程文件
│   ├── ub_usage_limit.asc   // Ascend C样例实现
│   └── README.md
```

## UB容量口径

在混合编程场景中，SIMD/MainScalar、SIMD VF 和 SIMT VF 访问的是同一块 UB，需要按同一资源池统一计算容量。样例使用以下口径说明容量边界：

| 项目 | 数值 | 说明 |
| --- | ---: | --- |
| UB总空间 | 256KB | 单个 Vector Core 的 UB 总容量 |
| 系统预留空间 | 8KB | 不建议作为用户可用空间使用 |
| SIMT Data Cache最小保留空间 | 32KB | 存在 SIMT VF 时需要至少保留给 Data Cache 的空间 |
| 无SIMT VF时用户可用空间 | 248KB | `256KB - 8KB` |
| 有SIMT VF时用户可用空间 | 216KB | `256KB - 8KB - 32KB` |

当 kernel 中存在 SIMT VF 时，静态 UB、动态 UB、SIMD VF 使用的 UB、SIMT VF 使用的 UB，以及 MainScalar 使用的 UB 都应统一计入用户可用空间；不能只按某一个 VF 函数内部的局部声明单独判断。

上表是理论估算口径，其中"8KB 系统预留空间"何时真正被扣除、"总UB-静态1-静态2"是否严格成立、越界后的真实表现，均在下面的实测章节中给出了在真实 NPU 上逐字节验证的结果，部分结论比理论口径更细致。

## 样例实现

### Case 1：无SIMT VF场景

`Case1NoSimtVfKernel` 不调用 SIMT VF，仅在 `__global__ __vector__` 入口中声明并访问静态 UB。该 case 用于说明未启用 SIMT VF 时，用户可按 `UB总空间 - 系统预留空间` 估算 SIMD/MainScalar 可使用的 UB 上限。

> 实测发现：8KB 预留空间是否被扣除，实际取决于 kernel 内是否发生了 `asc_vf_call`，而不是是否存在 *SIMT* VF。详见下方实测 1。

### Case 2：有SIMT VF场景

`Case2WithSimtVfKernel` 使用 `extern __ubuf__` 声明动态 UB，并通过 `asc_vf_call<SimtFillDynamicUb>(dim3(1024), ...)` 启动 SIMT VF 访问该动态 UB。该 case 用于说明存在 SIMT VF 时，用户可用 UB 需要扣除系统预留空间和 SIMT Data Cache 最小保留空间。

### Case 3：SIMT VF和SIMD VF均有静态UB

`Case3StaticUbInVfKernel` 依次调用 `SimtStaticUbProbe` 和 `SimdStaticUbProbe`。两个 VF 函数各自声明静态 UB，用于说明不同执行空间中的静态 UB 不是相互独立的资源，最终会共同占用同一个 Vector Core 的 UB 容量。

> 实测发现：动态 UB 的实际可申请上限确实等于 `216KB - 静态1 - 静态2`，但这里的"静态1/静态2"不仅包括两个 VF 各自声明的静态 UB，还必须把 MainScalar（`__global__ __vector__` 入口函数本身）声明的静态 UB 一并计入，否则会高估上限。详见下方实测 2。

### Case 4：动态UB配置方式

`Case4DynamicUbKernel` 使用 `extern __ubuf__` 声明动态 UB，Host 侧通过 kernel 启动参数配置动态 UB 大小：

```cpp
constexpr uint32_t case4DynamicUbBytes = CASE4_DYNAMIC_WORDS * sizeof(uint32_t);
Case4DynamicUbKernel<<<blockNum, case4DynamicUbBytes, stream>>>(outputDevice + RESULT_WORDS_PER_CASE * 3);
```

动态 UB 的实际可访问范围由启动参数决定。访问 `dynamicUb[index]` 时，`index * sizeof(T)` 必须落在本次启动配置的动态 UB 大小范围内，同时还要满足有 SIMT VF 场景下的整体 UB 可用空间限制。

### Case 5：UB越界反例和定位分析

Case 5 不是一个默认运行的正向 case，而是内置的一个可控越界探针 `case5_probe`（见下方实测 3），Host 侧固定配置 4KB 动态 UB，通过命令行参数控制 SIMT VF 实际访问的越界字数，在真实 NPU 上逐步逼近失败边界。完整的定位步骤、命令和输出见下方实测 3。

## 实测结论与定位方法

以下 4 组实测均基于同一份 `ub_usage_limit.asc`，通过 CMake 编译期开关或运行时命令行参数控制探针行为，在 Ascend 950PR（`dav-3510`）真实硬件上执行，所有命令均可直接复现。

### 实测 1：无 SIMT VF 时，SIMD 能否用满 256KB 静态 UB

**结论：不能一概而论——取决于 kernel 内是否发生过 `asc_vf_call`。**

- 如果 kernel 完全不调用任何 VF 函数（纯 MainScalar 直接读写静态 UB），静态 UB 上限可以逼近 256KB（仅需扣除代码里 MainScalar 自身其它局部静态数组），8KB 预留空间**不会**被扣除。
- 只要 kernel 内发生过一次 `asc_vf_call`（哪怖是调用的 SIMD VF，不涉及任何 SIMT VF），编译器就会为 VF 调用预留出 8KB 空间（用作调用栈/spill 缓存），此时静态 UB 上限就会掉到 `256KB - 8KB`。

也就是说，README 顶部 UB 容量口径表中"无 SIMT VF 时用户可用空间 = 248KB”这一行，严格来说只在"kernel 内调用过 VF 函数”的前提下成立；如果 kernel 完全不使用 VF 编程模型，248KB 这个上限是保守估计，实测可以逼近 256KB。

#### 复现步骤

`ub_usage_limit.asc` 中的 `Case1Probe1Kernel`（需要 `ENABLE_UB_PROBE1=ON` 才会参与编译）支持两种模式，通过 CMake 选项 `UB_PROBE1_NO_VF` 切换：

- `UB_PROBE1_NO_VF=OFF`（默认）：kernel 内通过 `asc_vf_call<Probe1FillVF>(...)` 调用一次 SIMD VF 填充静态 UB。
- `UB_PROBE1_NO_VF=ON`：kernel 完全不调用任何 VF 函数，直接在 MainScalar 里用 for 循环填充静态 UB。

静态数组大小由 `UB_PROBE1_STATIC_BYTES`（字节）指定，每次改变该值都需要重新编译（数组大小是编译期常量）。

**子实验 A：kernel 内调用了 SIMD VF**

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build_probe1 -DENABLE_UB_PROBE1=ON -DUB_PROBE1_STATIC_BYTES=253920
cmake --build build_probe1
./build_probe1/ub_usage_limit probe1
```

输出（253920 字节 = 256KB - 8KB - 32B，成功）：

```text
[Probe1] static_bytes=253920
[Probe1] launch_ret=0, aclrtSynchronizeStream ret=0
[Probe1] marker=0xc0ffee01, first=0xc0ffee01, last=0xc100e5f8
[Probe1][Success] kernel executed without runtime error.
```

再把 `UB_PROBE1_STATIC_BYTES` 调大 4 个字节到 253924 并重新编译运行：

```text
[Probe1] static_bytes=253924
[ERROR] [Bisheng] Launch kernel failure! ret 107000
[Probe1] launch_ret=107000, aclrtSynchronizeStream ret=107000
[Probe1][Failed] kernel launch/execution reported a runtime error.
```

即：只要 kernel 内调用过 SIMD VF，静态 UB 的实际边界精确落在 `256KB - 8KB(预留) - 32B(MainScalar本地result数组)` = 253920 字节，多 4 个字节（一个 word）即报错，与理论口径完全一致。

**子实验 B：kernel 完全不调用任何 VF 函数**

```bash
cmake -S . -B build_probe1 -DENABLE_UB_PROBE1=ON -DUB_PROBE1_NO_VF=ON -DUB_PROBE1_STATIC_BYTES=262112
cmake --build build_probe1
./build_probe1/ub_usage_limit probe1
```

输出（262112 字节 = 256KB - 32B，成功）：

```text
[Probe1] static_bytes=262112
[Probe1] launch_ret=0, aclrtSynchronizeStream ret=0
[Probe1] marker=0xc0ffee01, first=0xc0ffee01, last=0xc100edf8
[Probe1][Success] kernel executed without runtime error.
```

再把大小调大 4 个字节到 262116 并重新编译运行：

```text
[Probe1] static_bytes=262116
[ERROR] [Bisheng] Launch kernel failure! ret 107000
[Probe1] launch_ret=107000, aclrtSynchronizeStream ret=107000
[Probe1][Failed] kernel launch/execution reported a runtime error.
```

即：完全不使用 VF 编程模型时，静态 UB 边界精确落在 `256KB - 32B(MainScalar本地result数组)` = 262112 字节，8KB 预留空间**没有**被扣除。

#### 关于 `--cce-disable-vf-stack-reserved-ubuf` 的说明（已知限制）

devkit 文档中提到可以通过 bisheng 编译选项 `--cce-disable-vf-stack-reserved-ubuf` 主动关闭 8KB VF 栈预留空间，把它当作普通 UB 使用（但用户需自行保证不发生寄存器 spill）。本样例尝试通过 CMake 选项 `UB_PROBE1_DISABLE_RESERVED` 传递该编译选项进行实测，但在当前 CANN 9.0.0 环境下编译报错：

```text
bisheng: error: unsupported option '--cce-disable-vf-stack-reserved-ubuf'
```

排查过 `bisheng --help`、`bisheng --help-hidden`，以及对 CANN 9.0.0/9.1.0 两个版本的 `bisheng`/`ccec` 二进制做过字符串搜索，均未找到该选项的实现，只找到一个不相关的 `-cce-vf-stack-size=` 选项。因此该选项**在当前实测环境下不可用**，`UB_PROBE1_DISABLE_RESERVED` 这个 CMake 开关目前只是占位，打开后编译会直接失败，请勿在当前工具链版本下依赖该选项。

### 实测 2：SIMT VF 和 SIMD VF 都有静态 UB 时，动态 UB 上限是否等于 `216KB - 静态1 - 静态2`

**结论：公式成立，但"静态1 + 静态2”必须把 MainScalar 自身的静态 UB 一起算进去，否则会高估。**

`Case3SweepKernel` 中：

- SIMT VF（`SimtStaticUbProbeSweep`）声明 16K word（64KB）静态 UB；
- SIMD VF（`SimdStaticUbProbeSweep`）声明 16K word（64KB）静态 UB；
- MainScalar 自身还声明了 `simtResult[8]`、`simdResult[8]`、`result[8]` 三个 8-word（32B）局部数组，共 96 字节；
- 另外通过 `extern __ubuf__ uint32_t dynamicUb[]` 声明动态 UB，实际访问字数由运行时参数 `dynamicWordCount` 控制。

预测公式：`216KB(USER_UB_BYTES_WITH_SIMT) - 128KB(两个VF静态UB) - 96B(MainScalar本地静态) = 90016 字节`。

#### 复现步骤

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build && cmake --build build
./build/ub_usage_limit case3_sweep 22504   # 22504 words = 90016 bytes
./build/ub_usage_limit case3_sweep 22505   # 22505 words = 90020 bytes
```

90016 字节（预测边界内）输出：

```text
[Case3Sweep] static_total_bytes=131072, predicted_max_dynamic_bytes=90112, requested_dynamic_bytes=90016
[Case3Sweep] launch_ret=0, aclrtSynchronizeStream ret=0
[Case3Sweep] marker=0xc0030000, dynamic_first=0xc0030000, dynamic_last=0xc00357e7, simt_static_probe=0xc0030000, simd_static_probe=0xc0031000
[Case3Sweep][Success] dynamic_bytes=90016 executed without runtime error.
```

90020 字节（预测边界外 4 字节，即多 1 个 word）输出：

```text
[Case3Sweep] static_total_bytes=131072, predicted_max_dynamic_bytes=90112, requested_dynamic_bytes=90020
[ERROR] [Bisheng] Launch kernel failure! ret 107000
[Case3Sweep] launch_ret=107000, aclrtSynchronizeStream ret=107000
[Case3Sweep][Failed] dynamic_bytes=90020 triggered a runtime error (Data Cache below 32KB minimum, or other UB overflow).
```

注意 `predicted_max_dynamic_bytes` 输出的 90112 是代码里只按"两个 VF 静态UB”算出来的粗略预测值（`216KB - 128KB`），并未扣掉 MainScalar 本地的 96 字节；实测边界恰好落在 `90112 - 96 = 90016`，与"总UB - 静态1 - 静态2 - MainScalar本地静态”精确吻合，比"总UB - 静态1 - 静态2”更准确。这证明动态 UB 上限确实是对同一块 UB 资源池做减法，但减去的静态部分必须覆盖**所有**执行空间（SIMT VF、SIMD VF、MainScalar），不能只统计两个 VF。

### 实测 3：动态 UB 越界到不同程度，是静默通过还是触发 aicore 运行时错误

**结论：两者都会发生，且边界不是"整 8KB”这种可预测的整数，不能凭直觉估算，只能用同样的方法逐步实测。**

`Case5OobProbeKernel` 固定 Host 侧配置 4096 字节（1024 word）动态 UB，通过运行时参数 `actualAccessWords` 控制 SIMT VF 实际写入的字数（可以远超过配置值）。

#### 复现步骤（二分定位过程）

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build && cmake --build build

for words in 1024 2048 4096 16384; do
  echo -n "words=$words bytes=$((words*4)) -> "
  ./build/ub_usage_limit case5_probe $words 2>&1 | grep -E "NoRuntimeError|RuntimeError"
done
```

第一轮粗测输出：

```text
words=1024 bytes=4096 -> [Case5Probe][NoRuntimeError] ...   # 恰好等于配置值，正常
words=2048 bytes=8192 -> [Case5Probe][NoRuntimeError] ...   # 越界8KB（2倍），仍然静默通过，无报错
words=4096 bytes=16384 -> [Case5Probe][RuntimeError] ...    # 越界12KB（4倍），触发aicore运行时错误
words=16384 bytes=65536 -> [Case5Probe][RuntimeError] ...
```

即：越界到 2 倍（8KB）时数据被静默覆盖但不报错；越界到 4 倍（16KB）时触发运行时错误。说明失败边界在 2048~4096 word 之间，继续二分：

```bash
for words in 3072 2560 2304 2176 2112 2080 2064; do
  echo -n "words=$words bytes=$((words*4)) -> "
  ./build/ub_usage_limit case5_probe $words 2>&1 | grep -E "NoRuntimeError|RuntimeError"
done
# 全部 NoRuntimeError，边界继续上移
for words in 3584 3840 3968 4032 4064; do
  echo -n "words=$words bytes=$((words*4)) -> "
  ./build/ub_usage_limit case5_probe $words 2>&1 | grep -E "NoRuntimeError|RuntimeError"
done
# 全部 RuntimeError，边界收窄到 3072~3584 之间
```

持续二分后，最终锁定精确边界：

```bash
./build/ub_usage_limit case5_probe 3096   # 12384 bytes
./build/ub_usage_limit case5_probe 3097   # 12388 bytes
```

```text
words=3096 bytes=12384 -> [Case5Probe][NoRuntimeError] kernel executed and returned normally; check whether downstream data (Data Cache / adjacent UB) got silently overwritten.
words=3097 bytes=12388 -> [Case5Probe][RuntimeError] aclrtSynchronizeStream reported a runtime error for this access range.
```

即：Host 侧配置 4096 字节动态 UB，实际访问 12384 字节（超出配置值 8288 字节）仍能静默运行；一旦超出 12388 字节（超出配置值 8292 字节），立即触发 `aclrtGetLastError`/`aclrtSynchronizeStream` 可捕获的 aicore 运行时错误。

#### 结论解读

- 越界量在大致 8KB（8192 字节）附近仍会被静默容忍，这与理论口径中"8KB 系统预留空间”紧邻在动态 UB 之后的布局是吻合的：越界写入先落入这段预留区域，暂时不会破坏 SIMT 自身在 Data Cache 中的运行状态，因此不报错，但数据已经被覆盖，属于"静默的算子精度问题”。
- 实测边界（8288~8292 字节）比 8192 字节整数略大一些，说明真实边界还受编译器对 `extern __ubuf__` 动态数组的对齐/布局细节影响，并非一个用户可以精确计算的公开常量。
- **最重要的结论是：不能假设"越界在 8KB 以内就是安全的”。** 该边界依赖具体 kernel 的静态 UB 布局、VF 调用方式等实现细节，换一个 kernel（比如实测1、实测2中的 kernel）这个具体数字会变化。唯一可靠的做法是让实际访问范围严格不超过启动时配置的动态 UB 大小，任何越界都应被视为未定义行为，即使当前某次运行没有报错，也不能保证在其它编译版本、其它输入规模下同样"安全”。

#### 通用定位步骤（可用于排查真实越界问题）

1. 先确认现象：如果算子输出数据异常但没有报错日志，先怀疑静默越界；如果 `aclrtSynchronizeStream`/`aclrtGetLastError` 返回非 0（如 `107000`），说明越界已经严重到触发了 aicore 运行时校验。**注意 `aclrtSynchronizeStream` 单独调用可能不够**：某些越界是在 kernel 启动（`<<<...>>>`）时就被校验拦截的，必须在启动语句之后立即调用 `aclrtGetLastError(ACL_RT_THREAD_LEVEL)` 才能捕获，否则会把失败误判为成功（本样例在开发过程中就实际踩过这个坑，`aclrtSynchronizeStream` 返回 0 但 stderr 已经打印了 `[ERROR] [Bisheng] Launch kernel failure! ret 107000`）。
2. 统计该 kernel 里 MainScalar、SIMD VF、SIMT VF 中所有静态 UB 声明的字节数，以及 `<<<...>>>` 启动参数里配置的动态 UB 字节数。
3. 存在 SIMT VF 时，按 `256KB - 8KB - 32KB - 静态UB总量` 计算动态 UB 理论上限；不存在 SIMT VF 但 kernel 内调用过 VF 函数时，按 `256KB - 8KB - 静态UB总量` 计算；完全不使用 VF 编程模型时，按 `256KB - 静态UB总量` 计算（参见实测1）。
4. 对照实际访问下标的最大值，确认 `最大下标 * sizeof(T)` 是否超过第 3 步算出的上限或启动参数配置的动态 UB 大小。
5. 如果怀疑越界但没有报错，参照本样例 `case5_probe` 的做法：固定配置一个较小的动态 UB，通过一个可控的运行时参数逐步增大实际访问范围（可用二分法），观察 `aclrtGetLastError` 何时由 0 变为非 0，从而定位真实的失败边界，而不是依赖理论公式直接下结论。
6. 修复方式是让实际访问范围严格落在已申请/已配置的 UB 范围内；存在 SIMT VF 时还需保证 Data Cache 剩余空间不低于 32KB，不应该通过压缩预留空间的方式换取用户 UB。

### 实测 4：`GetCoreMemSize(CoreMemType::UB)` 在无 VF / 有 SIMD VF / 有 SIMT VF 场景下获取到的分别是什么

**结论：`GetCoreMemSize(CoreMemType::UB, ...)` 是 Host 侧对当前 SoC 的静态硬件规格查询，不感知也不跟随某一次 kernel launch 里是否调用了 VF、调用的是 SIMT VF 还是 SIMD VF——无论运行哪个 kernel、运行前还是运行后查询，返回值都是同一个常量。**

`GetCoreMemSize` 的实现（`impl/utils/tiling/platform/platform_ascendc.cpp`）直接读取 `PlatformInfoManager` 缓存的当前 SoC 平台信息表里的 `LocalMemSize(UB)`，只在两种情况下会减去额外字节：SoC 是 `Ascend910B`（本样例目标架构 `dav-3510`/Ascend950PR 不属于这一分支）时减去 256 字节 KFC 保留区；以及用户显式调用过 `ReserveLocalMemory(ReservedSize)` 时减去对应字节（`reservedMemSize_` 默认为 0，本样例未调用该接口）。也就是说这个接口读到的是"平台侧登记的 UB 物理容量表项"，和某次 kernel 里 Device 侧代码到底有没有调用 `asc_vf_call`、调用的是 `__simt_vf__` 还是 `__simd_vf__` 完全无关——它不是一个运行期按 kernel 实际占用回填的值。

#### 复现步骤

`ub_usage_limit.asc` 新增的 `platform_query` 模式依次执行：查询一次 `GetCoreMemSize(CoreMemType::UB)` → 运行不调用任何 VF 的 `Case1NoSimtVfKernel` → 再查询一次 → 运行调用 SIMT VF 的 `Case2WithSimtVfKernel` → 再查询一次 → 运行同时调用 SIMT VF 和 SIMD VF 静态内存的 `Case3StaticUbInVfKernel` → 再查询一次，对比 4 次查询结果是否发生变化。该模式复用默认 build，不需要额外 CMake 选项，但依赖 `target_link_libraries(ub_usage_limit PRIVATE m platform tiling_api dl)` 把 `platform`/`tiling_api` 库链接进可执行文件（否则会报 `undefined symbol: platform_ascendc::PlatformAscendCManager::...`）。

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build && cmake --build build
./build/ub_usage_limit platform_query
```

实测输出：

```text
[PlatformQuery][before_any_kernel] GetCoreMemSize(CoreMemType::UB) = 253952 bytes
[PlatformQuery][after_no_vf_kernel(Case1NoSimtVfKernel)] GetCoreMemSize(CoreMemType::UB) = 253952 bytes
[PlatformQuery][after_simt_vf_kernel(Case2WithSimtVfKernel)] GetCoreMemSize(CoreMemType::UB) = 253952 bytes
[PlatformQuery][after_simt_and_simd_vf_kernel(Case3StaticUbInVfKernel)] GetCoreMemSize(CoreMemType::UB) = 253952 bytes
```

四次查询结果完全一致，均为 253952 字节（= 256KB - 8KB - 32B），与运行过哪个 kernel、kernel 内是否调用 VF、调用的是 SIMT VF 还是 SIMD VF 无关，证实了上面的结论。

#### 结论解读

- 253952 字节恰好等于实测 1 中"kernel 内调用过 VF 函数"分支下的静态 UB 边界（`256KB - 8KB(预留) - 32B(MainScalar本地result数组)`），这是因为本样例的 Host 侧代码始终在同一个进程里、针对同一张 `PlatformInfoManager` 平台信息表发起查询，而这张表在进程启动时基于当前 SoC 型号一次性加载，之后不会因为某次 kernel 调用而改变。**不要把 `GetCoreMemSize` 的返回值理解为"这次 kernel 实际可用的 UB"**：它给出的是当前 SoC 型号下 UB 这块资源的登记容量（已经减去了平台侧统一维护的固定预留，但没有减去 32KB Data Cache 保留、也没有减去任何 kernel 自身的静态/动态 UB 占用）。
- 是否存在 SIMT VF、是否调用了 `asc_vf_call`、静态 UB 声明了多大，这些都是 Device 侧编译期/kernel 级别的信息，`GetCoreMemSize` 这个 Host 侧接口在设计上就没有入参能表达"某个具体 kernel"，因此它不可能，也不应该被期望随 kernel 形态变化。真正需要按 kernel 形态区分预留（比如是否需要额外扣 32KB Data Cache）的场景，要按本 README《UB 容量口径》一节里的公式手动计算，`GetCoreMemSize` 只能提供公式里"总 UB"这一项的起点。
- 实际编码时，若要计算某个具体 kernel 的动态 UB 可用上限，正确用法是：以 `GetCoreMemSize(CoreMemType::UB)` 的返回值作为基准，再按该 kernel 是否有 SIMT VF（额外减 32KB Data Cache）、静态 UB 总量（MainScalar + 各 VF 声明）等 kernel 级别信息手动扣减，不能指望调用该接口就能拿到"这个 kernel 专属"的可用值。

## 编译运行

### 默认4个正向case

在样例目录下执行：

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build
cmake --build build
./build/ub_usage_limit
```

默认输出包含 4 个 case 的 `used_bytes`、`limit_bytes` 和校验状态。所有 case 均输出 `PASSED` 且最终出现如下信息时，表示样例运行通过：

```text
[Success] All UB usage limit cases are verification passed.
```

### 实测探针（非默认路径，用于复现上文4组实测）

```bash
# 实测1：需要单独打开CMake选项后重新编译，见"实测1"章节的具体命令
cmake -S . -B build_probe1 -DENABLE_UB_PROBE1=ON -DUB_PROBE1_STATIC_BYTES=<字节数> [-DUB_PROBE1_NO_VF=ON]
cmake --build build_probe1
./build_probe1/ub_usage_limit probe1

# 实测2：复用默认build，运行时参数指定动态UB字数（word）
./build/ub_usage_limit case3_sweep <动态UB字数>

# 实测3：复用默认build，运行时参数指定SIMT VF实际访问字数（word），配置值固定为1024 word（4KB）
./build/ub_usage_limit case5_probe <实际访问字数>

# 实测4：复用默认build，无运行时参数，依次在无VF/有SIMT VF/有SIMT+SIMD VF的kernel前后查询GetCoreMemSize(UB)
./build/ub_usage_limit platform_query
```

`probe1`/`case3_sweep`/`case5_probe`/`platform_query` 均不属于验收要求的"4个默认正向case"，仅用于本 README 中的实测定位过程；默认运行路径（不带任何参数执行 `./build/ub_usage_limit`）始终只覆盖 4 个正向 case，不会触发 UB 越界失败。
