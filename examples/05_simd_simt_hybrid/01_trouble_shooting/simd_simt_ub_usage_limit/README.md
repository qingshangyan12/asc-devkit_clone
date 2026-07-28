# SIMD与SIMT混合编程UB越界定位样例

## 概述

本样例从一个简化的逐元素计算算子UB访问越界问题出发，介绍动态UB访问范围的定位方法，并通过四个正向Case说明不同VF配置下的可用静态UB上限、动态UB以及混合编程场景下UB资源的正确使用方式。

样例通过编译选项`SCENARIO_NUM`选择Case，一次构建只编译并运行一个Case，各Case之间不共享kernel、动态UB、输出buffer或运行状态。Case 0故意触发静态UB越界并导致精度失败；Case 1~4为相互独立的正向或边界研究场景。

四个Case的讲述关系如下：

| Case | 场景 | 主要验证内容 | 运行方式 |
| --- | --- | --- | --- |
| Case 0 | 静态UB越界踩入动态UB | 越界写破坏算子输入并导致精度失败 | `SCENARIO_NUM=0` |
| Case 1 | 无VF，MainScalar使用静态UB | 不调用VF时静态UB的基本使用方式 | `SCENARIO_NUM=1` |
| Case 2 | 可用静态UB上限研究 | 有无SIMD VF及编译选项对可用静态UB上限的影响 | `SCENARIO_NUM=2` |
| Case 3 | SIMT VF使用动态UB | 动态UB的配置大小必须覆盖实际访问范围 | `SCENARIO_NUM=3` |
| Case 4 | MainScalar、SIMD VF和SIMT VF混合使用UB | 静态UB累加以及动态UB全局共用的内存布局 | `SCENARIO_NUM=4` |

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── simd_simt_ub_usage_limit
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── ub_usage_limit.asc     // Ascend C样例实现
│   ├── README.md              // 中文说明
│   └── README_en.md           // 英文说明
```

## Case 0：静态UB越界导致精度失败

Case 0模拟实际算子中静态数组访问下标计算错误的问题。kernel申请4KB静态UB，并正确配置8KB动态UB；动态UB依次保存`input`和`output`两个4KB缓冲区：

```text
低地址
├── static_ub 4KB，合法下标为[0, 1023]
├── input     4KB，动态UB起始位置
└── output    4KB
高地址

动态UB配置：8KB，配置大小正确
```

SIMT VF先初始化`input`。随后MainScalar错误访问`static_ub[1024]`，该地址超过静态数组边界4B并落到紧随静态区的动态UB起始位置，覆盖`input[0]`：

```cpp
__ubuf__ uint32_t static_ub[1024];
extern __ubuf__ uint32_t case0_dynamic_ub[];
__ubuf__ uint32_t* input = case0_dynamic_ub;

uint32_t faulty_index = 1024;
__ubuf__ volatile uint32_t* static_data = static_ub;
static_data[faulty_index] = 0xDEADBEEF;
```

`faulty_index`通过kernel参数在运行时传入，`volatile`保证该故障写入不会被编译器消除。SIMT VF再计算`output[index] = input[index] + 1`。除首元素外，其余元素仍与Golden一致；由于`input[0]`已被覆盖，`output[0]`由期望值`0xC0000001`变为`0xDEADBEF0`，Host精度校验失败。该场景说明即使静态UB与动态UB申请总量没有超过容量上限，单次静态数组越界访问仍可能破坏相邻动态数据并造成静默精度错误。

编译并运行Case 0：

```bash
cmake -S . -B build_case0 -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0
cmake --build build_case0 -j
./build_case0/ub_usage_limit
```

预期输出：

```text
[Case0] static_bytes=4096, dynamic_bytes=8192, overflow_bytes=4
[Case0] golden_first=0xc0000001, actual_first=0xdeadbef0
[Case0] mismatch_count=1
[Case0] precision_check=FAILED (fault reproduced)
```

## 正向Case

### Case 1：无VF场景使用静态UB

`NoVfStaticUbKernel`不调用SIMD VF或SIMT VF，也不配置动态UB，仅由MainScalar声明并访问4KB静态UB：

```cpp
__ubuf__ uint32_t static_ub[1024];
NoVfStaticUbKernel<<<1, 0, stream>>>(output_device);
```

MainScalar写入全部元素，Host校验首尾元素。本Case说明不使用VF时，静态UB由MainScalar在编译期申请，运行时不需要配置动态UB。

#### 执行结果

4KB静态UB读写正确，动态UB配置为0：

```text
Case 1 (no VF static UB): PASSED
```

本Case实际使用4096B静态UB；结合Case 2中相同无VF结构的边界实测，在固定包含32B结果区时，`static_ub`最大可配置为262112B，两段静态UB合计256KB。

### Case 2：有无SIMD VF及编译选项对可用静态UB上限的影响

Case 2用于比较不调用SIMD VF、调用SIMD VF以及关闭VF栈预留空间三种情况下，当前kernel可使用的静态UB上限。样例逐步调整`static_ub`大小，以“当前值运行成功、增加4B启动失败”确定边界。本Case不配置动态UB，结论仅适用于静态UB使用场景。

探针数组长度、是否调用SIMD VF以及是否传递编译选项都在编译期确定，无法通过运行时分支完成三组有效对照，因此每种场景需要分别构建`SCENARIO_NUM=2`。

研究kernel声明一段大小可配置的`static_ub`和一段固定的32B静态结果区：

```cpp
__ubuf__ uint32_t static_ub[BOUNDARY_STATIC_WORDS];
__ubuf__ uint32_t result[8];
```

`UB_BOUNDARY_STATIC_BYTES`控制第一段静态数组的字节数，配置值必须是4B的整数倍。

#### 场景1：不调用SIMD VF

```bash
cmake -S . -B build_boundary_no_vf \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=2 \
  -DUB_BOUNDARY_NO_SIMD_VF=ON \
  -DUB_BOUNDARY_STATIC_BYTES=262112
cmake --build build_boundary_no_vf -j
./build_boundary_no_vf/ub_usage_limit
```

#### 场景2：调用SIMD VF，使用默认编译配置

```bash
cmake -S . -B build_boundary_simd \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=2 \
  -DUB_BOUNDARY_STATIC_BYTES=253920
cmake --build build_boundary_simd -j
./build_boundary_simd/ub_usage_limit
```

#### 场景3：调用SIMD VF并关闭VF栈预留空间

```bash
cmake -S . -B build_boundary_flag \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=2 \
  -DUB_BOUNDARY_DISABLE_RESERVED=ON \
  -DUB_BOUNDARY_STATIC_BYTES=260064
cmake --build build_boundary_flag -j
./build_boundary_flag/ub_usage_limit
```

#### 执行结果

在CANN 9.2.0、`dav-3510`真实NPU上的执行结果如下：

下表同时列出每种配置下能够成功运行的最大`static_ub`大小，以及在此基础上增加一个`uint32_t`（4B）后的失败结果：

| 场景 | `static_ub`大小 | 结果 |
| --- | ---: | --- |
| 不调用SIMD VF | 262112B | 通过；与32B结果区合计256KB |
| 不调用SIMD VF | 262116B | kernel启动失败 |
| 调用SIMD VF，默认配置 | 253920B | 通过 |
| 调用SIMD VF，默认配置 | 253924B | kernel启动失败 |
| 调用SIMD VF，关闭VF栈预留 | 260064B | 通过 |
| 调用SIMD VF，关闭VF栈预留 | 260068B | kernel启动失败 |

由上述结果可以确定当前kernel结构下`static_ub`数组的可用上限。`static_ub`不是kernel中唯一的静态UB对象：固定的32B结果区也属于静态UB，需要一并计入当前kernel的静态UB总量。各列关系为“静态UB总量 = `static_ub`可用上限 + 32B结果区”：

| 场景 | `static_ub`可用上限 | 包含32B结果区后的静态UB总量 |
| --- | --- | --- |
| 不调用SIMD VF | 262112B | 262144B（256KB） |
| 调用SIMD VF，默认配置 | 253920B | 253952B（248KB） |
| 调用SIMD VF，关闭VF栈预留 | 260064B | 260096B |

三种场景均按4B粒度验证到“当前值通过、增加4B失败”，表中给出的是当前kernel结构下的实测静态UB上限。由于本Case没有配置动态UB，不能用这些结果直接证明静态UB与动态UB组合使用时的边界。

三种通过场景的关键输出如下：

```text
# 不调用SIMD VF，静态数组为262112B
[Case2] mode=without SIMD VF, static_bytes=262112
[Case2] launch_ret=0, sync_ret=0, result=PASSED

# 调用SIMD VF，使用默认编译配置，静态数组为253920B
[Case2] mode=with SIMD VF, static_bytes=253920
[Case2] launch_ret=0, sync_ret=0, result=PASSED

# 调用SIMD VF并关闭VF栈预留空间，静态数组为260064B
[Case2] mode=with SIMD VF, static_bytes=260064
[Case2] launch_ret=0, sync_ret=0, result=PASSED
```

超过对应边界时，关键输出如下：

```text
# 不调用SIMD VF，静态数组为262116B
[Case2] mode=without SIMD VF, static_bytes=262116
[Case2] launch_ret=107000, sync_ret=107000, result=FAILED

# 调用SIMD VF，使用默认编译配置，静态数组为253924B
[Case2] mode=with SIMD VF, static_bytes=253924
[Case2] launch_ret=107000, sync_ret=107000, result=FAILED

# 调用SIMD VF并关闭VF栈预留空间，静态数组为260068B
[Case2] mode=with SIMD VF, static_bytes=260068
[Case2] launch_ret=107000, sync_ret=107000, result=FAILED
```

默认配置下调用SIMD VF会预留8KB UB。`--cce-disable-vf-stack-reserved-ubuf`会改变可用边界，但不能据此认为调用SIMD VF的kernel可以使用完整的256KB，还需要考虑编译器生成的其它UB占用和具体VF实现。关闭预留后，编译器不能再使用该区域缓存寄存器溢出数据，实际算子使用前需要确认VF没有寄存器溢出。

### Case 3：有SIMT VF场景使用动态UB

Case 3用于验证仅包含SIMT VF时的动态UB上限。kernel包含32B静态结果区，按有SIMT VF时用户UB不超过216KB计算：

```text
动态UB上限 = 216KB - 32B = 221152B
```

`CASE3_DYNAMIC_BYTES`控制动态UB大小。SIMT线程写满整个动态区，MainScalar读取首尾元素并由Host校验，确保测试实际访问到配置范围的最后一个word。

#### 执行结果

理论上限221152B运行通过：

```text
[Case3] max_dynamic_bytes=221152, configured_dynamic_bytes=221152
Case 3 (SIMT VF dynamic UB): PASSED
```

增加一个word至221156B后，kernel启动失败：

```text
[Case3] max_dynamic_bytes=221152, configured_dynamic_bytes=221156
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 3 (SIMT VF dynamic UB): FAILED
```

因此当前kernel结构下动态UB实测可用上限为221152B。

### Case 4：静态UB和动态UB共用资源池

Case 4分别在MainScalar、SIMD VF和SIMT VF中声明静态UB。MainScalar包含1KB的`main_static_ub`以及`simt_result`、`simd_result`、`result`三个32B结果数组，共1120B；SIMD VF声明2KB静态UB，SIMT VF声明4KB静态UB。不同执行位置声明的静态UB分别占用空间，并累加计入当前kernel的静态UB总量。三个执行位置都实际访问各自静态数组的末元素，避免声明被编译器优化掉：

```text
静态UB总量 = 1KB + 96B + 2KB + 4KB = 7264B
动态UB上限 = 216KB - 7264B = 213920B
```

动态UB的情况不同。空间大小只由kernel启动时的`CASE4_DYNAMIC_BYTES`配置一次。为了让MainScalar、SIMD VF和SIMT VF访问同一块动态UB，三个执行位置需要声明并引用同一个外部符号`shared_dynamic_ub`；`extern`声明本身不分配新的空间：

```cpp
// MainScalar、SIMD VF和SIMT VF中分别出现同一条声明
extern __ubuf__ uint32_t shared_dynamic_ub[];
```

MainScalar通过`shared_dynamic_ub`写入A，SIMD VF通过同一符号读取A后写入B，SIMT VF继续读取B后写入C，最后由MainScalar读取C；同时MainScalar写入动态UB最后一个word并在交接后校验。该过程验证同一个外部动态UB对象可以跨三个执行位置共享，也确保实际访问到动态区末尾。

因此，Case 4验证的两条规则是：不同执行位置的静态UB需要累加统计；三个执行位置引用同一个`extern __ubuf__`符号时，共享kernel启动时配置的同一块动态UB。不同名称的外部动态UB符号不属于本Case的共享用法，不能据此认为它们的首地址相同。

#### 执行结果

理论上限213920B运行通过：

```text
[Case4] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213920
Case 4 usage: static_total_bytes=7264, dynamic_bytes=213920, dynamic_handoff=PASSED
Case 4 (mixed UB): PASSED
```

增加一个word至213924B后，kernel启动失败：

```text
[Case4] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213924
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 4 (mixed UB): FAILED
```

因此当前kernel结构下，7264B静态UB会从同一用户UB资源池中扣除，动态UB实测可用上限为213920B。

## 容量计算

在包含SIMT VF的默认编译模式下，单个Vector Core的256KB UB从低地址到高地址依次包含静态内存、动态内存、8KB预留空间和Data Cache：

```text
Data Cache = min(256KB - 静态UB - 动态UB - 8KB, 128KB)
```

Data Cache不能小于32KB，因此用户UB容量需要满足：

```text
静态UB + 动态UB <= 216KB
实际动态UB访问范围 <= kernel启动参数配置的动态UB大小
```

静态UB需要统计MainScalar、SIMD VF和SIMT VF中的全部声明。总容量限制和具体动态地址访问边界需要同时满足。

## 编译运行

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=<0~4>
cmake --build build -j
./build/ub_usage_limit
```

`${install_path}`为CANN开发套件包安装目录。样例要求CANN版本不低于9.2.0，并仅支持`dav-3510`架构。

例如选择`SCENARIO_NUM=4`时，预期输出：

```text
[Case4] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213920
Case 4 usage: static_total_bytes=7264, dynamic_bytes=213920, dynamic_handoff=PASSED
Case 4 (mixed UB): PASSED
```

各Case专用的编译选项如下：

| 选项 | 默认值 | 说明 |
| --- | --- | --- |
| `UB_BOUNDARY_STATIC_BYTES` | `253920` | Case 2 kernel中的静态UB数组大小，单位为Byte |
| `UB_BOUNDARY_NO_SIMD_VF` | `OFF` | 不调用SIMD VF，由MainScalar直接访问静态UB |
| `UB_BOUNDARY_DISABLE_RESERVED` | `OFF` | 传递`--cce-disable-vf-stack-reserved-ubuf` |
| `CASE3_DYNAMIC_BYTES` | `221152` | Case 3配置并实际访问的动态UB大小，单位为Byte |
| `CASE4_DYNAMIC_BYTES` | `213920` | Case 4配置并实际访问的动态UB大小，单位为Byte |

通用编译选项如下：

| 选项 | 默认值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，支持Ascend 950PR/Ascend 950DT |
| `SCENARIO_NUM` | `1` | 编译并运行的Case编号，可选`0~4` |

## 定位方法

1. 根据每个指针的起始偏移和最大访问下标，计算动态UB的实际访问范围。
2. 对照kernel启动参数中的动态UB字节数，确认实际访问范围没有超出配置值。
3. 统计MainScalar、SIMD VF和SIMT VF中的全部静态UB。
4. 存在SIMT VF时，确认静态UB与动态UB之和不超过216KB。
5. 在kernel启动后检查`aclrtGetLastError`，并检查`aclrtSynchronizeStream`返回值。
6. 即使运行时没有报错，也必须修复所有超过已配置范围的访问。
