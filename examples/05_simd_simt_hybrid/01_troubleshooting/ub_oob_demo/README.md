# SIMD与SIMT混合编程UB访问越界定位样例

## 概述

本样例基于SIMD和SIMT混合编程模式，演示UB（Unified Buffer）访问越界的定位方法。与卡死类问题不同，UB越界最棘手的地方在于**症状不稳定**：同一个越界bug，越界距离不同，表现从"结果错但无任何报错"一直到"硬件直接报异常"。样例把这三级症状做成三个可独立编译运行的CASE，并给出一套在SIMT场景下可用的定位手段。

由于msSanitizer和msDebug当前均**不支持SIMT编程场景**（详见[NPU域上板调试](../../../../docs/zh/guide/编程指南/调试调优/功能调试/NPU域上板调试.md)），本样例给出的是不依赖这两个工具的定位方法：UB预算自查、guard band（哨兵）检测和printf边界打点。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.0.0-beta.2 |

## 目录结构介绍

```text
├── ub_oob_demo
│   ├── CMakeLists.txt          // cmake编译文件
│   ├── ub_oob_demo.asc         // Ascend C样例实现 & 调用样例
│   └── README.md
```

## 背景：UB划分决定了越界的后果

UB总大小为256KB，从低地址到高地址依次划分为**静态内存**、**动态内存**、**预留空间（固定8KB）**、**Data Cache（32KB~128KB）**，详见[内存层级](../../../../docs/zh/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md)。

用户可合法访问的范围只有静态内存 + 动态内存。越过这条边界后会依次落入预留空间和Data Cache，而这两个区域**没有硬件边界检查来保护用户数据**，于是越界距离直接决定了症状：

```text
UB base                                                                    256KB
├──────────────┬────────────────┬──────────────┬───────────────────────────────┤
│  静态内存     │   动态内存      │ 预留空间 8KB  │        Data Cache             │
└──────────────┴────────────────┴──────────────┴───────────────────────────────┘
     ↑ 合法访问范围 ↑            ↑              ↑
                    │            │              └─ CASE 3：触发硬件异常 507035
                    │            └─ CASE 2：被预留空间吸收，跑得"很干净"
                    └─ CASE 1：踩坏自己的相邻缓冲区，结果错但无任何报错
```

## 样例描述

- 样例功能：
  1024个线程（32个warp）对输入做warp内归约，每个warp把归约结果写入UB上的暂存区`partial[]`，再由tid=0汇总输出。动态内存上的布局为`[partial | guard | keep]`，其中`keep[]`代表算子里其它必须保持完好的数据。

- 注入的bug：
  `partial[]`按"每block 256线程=8个warp"的旧假设写死为8个slot，而线程数提到1024后实际有32个warp。`warp_id`取值范围变成[0, 32)，高编号warp写`partial[warp_id]`时越过`partial[]`边界。这是实际开发中很常见的一类越界：**线程数改了，但派生自threadIdx的索引范围对应的缓冲区容量没跟着改**。

- 三级症状（对应`CASE_NUM`）：

  | CASE | 越界落点 | 实测症状 |
  |------|---------|---------|
  | 1 | 仍在已申请的动态内存内 | `ret=0`，无任何报错，但归约结果错误（`partial[]`与`keep[]`互相踩踏） |
  | 2 | 超出边界但≤8KB，落入预留空间 | `ret=0`，且所有数据校验**全部通过**——bug被完全掩盖 |
  | 3 | 超出边界>8KB，落入Data Cache | `ACL_ERROR_RT_VECTOR_CORE_EXCEPTION`（507035） |

  CASE 2/3中`partial[]`按正确尺寸申请，唯一的越界来自`probe_vf`的定点写，因此症状不会和CASE 1混在一起。

- 样例实现：
  SIMT逻辑实现在`__simt_vf__`函数`reduce_vf`（归约+越界写）和`probe_vf`（定点越界写）中，由外层`__global__ __vector__`核函数`kernel_ub_oob`通过`asc_vf_call`调用，符合SIMD与SIMT混合编程的函数分层约定。

- 调用实现：
  使用内核调用符`<<<block_num, dyn_ub_size, stream>>>`调用核函数，动态内存大小通过`dyn_ub_size`配置，SIMT线程数通过`asc_vf_call`的第一个`dim3(...)`参数配置。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令：
  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  ./demo
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：Ascend 950PR/Ascend 950DT |
  | `CASE_NUM` | `1`（默认）、`2`、`3` | 选择越界症状等级，见上表 |
  | `GUARD_BAND` | `OFF`（默认）、`ON` | 是否插入哨兵区检测并量化越界，见下文定位流程第2步 |
  | `BOUND_PRINTF` | `OFF`（默认）、`ON` | 是否启用printf边界打点，见下文定位流程第3步 |
  | `FIX_OOB` | `OFF`（默认）、`ON` | 按实际warp数申请暂存区，即修复后的正确版本 |

- 执行结果

  CASE 1（默认）：kernel正常返回，但结果错误（`exit code 1`）：
  ```
  Kernel returned ACL_SUCCESS (no hardware error reported).
  reduce sum=25672 golden=5620  [WRONG]
  ```

  CASE 3：硬件报异常：
  ```
  [OOB DETECTED BY HW] aclrtSynchronizeStreamWithTimeout ret=507035 (ACL_ERROR_RT_VECTOR_CORE_EXCEPTION).
  ```

## UB越界定位流程

UB越界与卡死类问题的定位思路不同：卡死至少有"host侧超时"这个明确信号，而UB越界在CASE 1/2下**完全没有信号**。因此定位的第一步不是抓现场，而是主动构造检测手段。以下流程基于本样例实测结论。

1. **先做UB预算自查（不需要上板）**

   越界的根因往往在一行算术里就能看出来。样例`audit_ub_budget()`把[内存层级](../../../../docs/zh/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md)的公式落成代码，每次运行都打印：

   ```
   [UB budget] static=0B dyn=4128B reserved=8192B => DataCache=131072B (min 32768B)
   [UB budget] max legal dyn_ub_size = 221184B; kernel UB access must stay < 4128B
   ```

   要核对两件事：`dyn_ub_size`没超过合法上限（`256KB - 8KB - 32KB - 静态内存`）；以及**kernel里所有UB缓冲区尺寸之和，等于传给`<<<>>>`的`dyn_ub_size`**。第二条是最容易出错的地方——本样例的bug就是`partial[]`的容量和`warp_id`的实际取值范围对不上。

   同时要检查每个UB索引表达式的取值范围。重点看派生自`threadIdx`/`warp_id`/`blockDim`的索引：这些范围会随线程数配置变化，而缓冲区尺寸常常是写死的常量。

2. **用guard band（哨兵）把"无症状"变成"有症状"**

   CASE 1/2下硬件不报错，所以要自己造检测点。做法是在被怀疑的缓冲区之后紧跟一段哨兵区，填入魔术值，kernel跑完后搬回Host检查哪些被冲掉了：

   ```bash
   cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCASE_NUM=1 -DGUARD_BAND=ON ..
   make -j
   ./demo
   ```

   实测输出直接把越界长度量化出来了：
   ```
   [GUARD BAND TRIPPED] 24 of 64 sentinels overwritten; furthest hit at guard[23]
   => partial[] overflowed by at least 24 element(s) past its 8-slot capacity.
   ```

   `partial[]`容量8、越界24个元素，8+24=32正好等于实际warp数——**越界长度本身就指向了根因**，不用再猜。这是本样例中信息量最大的一个手段：它不仅告诉你"越界了"，还告诉你"越界了多少"，而这个数字通常直接对应写错的那个上限。

   哨兵区只在调试期插入，定位完成后移除即可。

3. **用printf打点确认第一个越界的索引**

   知道越界长度后，用printf（`#include "utils/debug/asc_printf.h"`）把第一个越界的索引和容量打出来，确认根因位置：

   ```bash
   cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCASE_NUM=1 -DBOUND_PRINTF=ON ..
   make -j
   ./demo
   ```

   ```
   [bound] warp_id=8 writes partial[8] but capacity is 8 -> OOB
   ```

   **注意事项：** 打点条件要收紧到只有越界的第一个代表线程会打印（样例中为`lane == 0 && warp_id == scratch_slots`）。线程数或打印次数较多时，GM缓冲区（默认2MB，可通过acl.json的`simt_printf_fifo_size`调整到1MB~64MB）可能被打满导致丢弃或阻塞，具体用量建议见[printf() API文档](../../../../docs/zh/api/Utils-API/tuning_interface/printf.md)。

4. **拿到507035后的处理**

   若越界距离超过8KB，会直接得到`ACL_ERROR_RT_VECTOR_CORE_EXCEPTION`（507035）。这个错误码说明UB访问已经落入Data Cache区域。收到后**不要**再对同一stream/device发起其他操作，设备可能仍处于异常状态；应直接退出进程，后续通过设备复位再重试。

   定位方式回到第1步：错误码只告诉你"越界了"，不会告诉你越界发生在哪一行，仍然要靠预算自查加哨兵区收窄。

## 一个必须强调的实测结论：CASE 2的"通过"是假的

CASE 2是三级症状里最危险的一级。实测中它**所有数据校验全部通过**，因为越界写落进了8KB预留空间，既没碰到用户数据也没碰到Data Cache：

```
[probe] writing 64 floats at UB byte offset 8320 (4096B past the declared 4224B)
Kernel returned ACL_SUCCESS (no hardware error reported).
reduce sum=5620 golden=5620  [OK]
```

但这个"通过"完全依赖预留空间恰好吸收了越界写，条件一变就会翻脸。实测验证：把同一份CASE 2代码加上`--cce-disable-vf-stack-reserved-ubuf`（该选项让编译器不再预留这8KB）重新编译，立刻变成507035硬件异常：

```bash
bisheng ub_oob_demo.asc -o demo --npu-arch=dav-3510 -DCASE_NUM=2 \
    --cce-disable-vf-stack-reserved-ubuf
./demo
# [OOB DETECTED BY HW] ... ret=507035 (ACL_ERROR_RT_VECTOR_CORE_EXCEPTION)
```

实测边界规律：越界距离**≤8KB时被容忍，>8KB时必定触发507035**，且该边界随已申请内存大小平移（在declared=8KB/16KB/32KB下均验证为`declared + 8KB`）；关闭预留空间后边界相应前移。

因此有两条实践结论：

- **上板跑通不等于UB访问正确。** 越界是否暴露取决于越界距离、预留空间是否启用、以及UB布局，这些都可能随编译选项或后续代码改动而变化。UB访问的正确性必须靠第1步的预算自查和索引范围核对来保证，而不是靠"跑过了"。
- **不要把预留空间当作可用缓冲。** 它是编译器用于寄存器溢出等用途的空间，今天能吸收越界写不代表明天还能。

## 修复

按实际warp数申请暂存区即可：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCASE_NUM=1 -DGUARD_BAND=ON -DFIX_OOB=ON ..
make -j
./demo
```

```
reduce sum=5620 golden=5620  [OK]
neighbour buffer keep[]: corrupted=0/1024
[GUARD BAND CLEAN] no sentinel was overwritten.
All checks passed.
```

更一般的做法是让缓冲区尺寸从线程配置推导出来，而不是写死常量，这样线程数变化时尺寸会自动跟随：

```cpp
constexpr uint32_t WARP_COUNT = THREAD_COUNT / WARP_SIZE;
constexpr uint32_t SCRATCH_SLOTS = WARP_COUNT;  // 而不是写死的8
```

## 参考

- [内存层级（UB划分）](../../../../docs/zh/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md)
- [核函数与VF函数](../../../../docs/zh/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/核函数与VF函数.md)
- [NPU域上板调试](../../../../docs/zh/guide/编程指南/调试调优/功能调试/NPU域上板调试.md)
- [printf() API文档](../../../../docs/zh/api/Utils-API/tuning_interface/printf.md)
