# SIMD与SIMT混合编程asc_syncthreads()卡死定位样例

## 概述

本样例基于SIMD和SIMT混合编程模式，演示`asc_syncthreads()`的一个已知卡死风险：在分支中调用block barrier时，若有线程无法到达同步点，其他已到达同步点的线程会永久阻塞。样例提供一套可复用的卡死定位流程：如何在不被卡死拖死进程的前提下，及时发现并诊断这类问题。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.0.0-beta.2 |

## 目录结构介绍

```text
├── simt_hang_demo
│   ├── CMakeLists.txt          // cmake编译文件
│   ├── sync_hang_demo.asc      // Ascend C样例实现 & 调用样例
│   └── README.md
```

## 样例描述

- 样例功能：
  1024个线程对输入数据做warp内归约求和，再由tid=0的线程汇总各warp归约结果输出。其中tid≥512的线程跳过`asc_syncthreads()`，改为在一个永不被host置位的GM flag上自旋而不退出——这些线程始终存活但永远不会到达同步点，导致调用了`asc_syncthreads()`的线程（tid<512）永久阻塞，构成真实卡死。

- 样例实现：
  SIMT逻辑（warp归约 + 分支同步）实现在`__simt_vf__`函数`sync_hang_demo_vf`中，由外层`__global__ __vector__`核函数`kernel_sync_hang_demo`通过`asc_vf_call`调用，符合SIMD与SIMT混合编程的函数分层约定。

- 调用实现：
  使用内核调用符`<<<block_num, dyn_ub_size, stream>>>`调用核函数，SIMT线程数通过`asc_vf_call`的第一个`dim3(...)`参数配置。

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
  | `BISECT_PRINTF` | `OFF`（默认）、`ON` | 是否启用printf二分定位打点，见下文"卡死定位流程"第2步 |

- 执行结果

  约5秒后自动检测到超时并退出（`exit code 1`），不会无限期挂起：
  ```
  [HANG DETECTED] aclrtSynchronizeStreamWithTimeout timed out after 5000ms, ret=507046 (ACL_ERROR_RT_STREAM_SYNC_TIMEOUT).
  Diagnosis: ...
  Localization steps:
    1. ...
    2. ...
    3. ...
  ```

## 卡死定位流程

真实卡死场景下，设备侧kernel永不返回，host侧任何等它完成的调用都会一起被拖死。以下流程基于实测结论：

1. **先用带超时的流同步止损**
   样例中`main()`使用`aclrtSynchronizeStreamWithTimeout(stream, timeoutMs)`替代无超时的`aclrtSynchronizeStream`，把"host侧永久死等"转化为一个有限时间、有明确错误码的可诊断事件——超时返回`ACL_ERROR_RT_STREAM_SYNC_TIMEOUT`（507046）。这是判断"确实发生卡死"而非"只是执行慢"的第一手信号。
   收到该错误码后**不要**再对同一stream/device发起其他操作（如`aclrtMemcpy`/`aclrtFree`/`aclrtDestroyStream`/`aclrtResetDevice`），设备可能仍处于卡死状态，这些调用同样可能被拖死；应直接退出进程，后续通过设备复位再重试。

2. **用printf打点二分定位，直接在卡死的那次运行上生效**

   SIMT的`printf()`（`#include "utils/debug/asc_printf.h"`）写入GM缓冲区并实时回传到host，**即使kernel整体永不返回，之前已经执行到的打点也会正常打印出来**——这一点已经用本样例实测确认。这种方式能直接在**真实卡死的那次运行上**定位，不需要另外找一个能结束的变体，适合作为拿到`ACL_ERROR_RT_STREAM_SYNC_TIMEOUT`之后的第一个尝试；代价是需要预先在可疑位置插好打点、重新编译。

   打开方式：
   ```bash
   cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DBISECT_PRINTF=ON ..
   make -j
   ./demo
   ```

   **二分思路（实测过程，非纸面推演）：** 面对一个不熟悉的卡死kernel，不需要一开始就知道该打在哪——从函数头尾开始，每一轮根据上一轮的结果继续往可疑区域收窄，逐步重新编译、运行：

   - **第1轮：函数入口 vs. 出口。** 只在`sync_hang_demo_vf`最开头（`tid==0`打"ENTER"）和`tid==0`写`output`之前（打"EXIT"）各打一个点。运行后只看到`ENTER`，说明卡死发生在函数体内部，而不是没launch成功或卡在调用方——这一步排除了"kernel根本没跑起来"这类误判，但还没定位到具体代码。
     ```
     [bisect][round1] tid=0 ENTER sync_hang_demo_vf
     [HANG DETECTED] ...
     ```
   - **第2轮：warp归约之后、分支同步之前。** 把打点往函数体内部移，放在`asc_reduce_add`归约结束、写`smem`之后，同时从分支两侧各选一个代表线程（`tid==0`和`tid==SYNC_THRESHOLD`）打点。运行后两侧都打出来了，说明归约阶段本身没问题，两侧线程都正常走到了分支入口——把可疑范围进一步收窄到分支内部。
     ```
     [bisect][round2] tid=0 reached reduce/smem stage, entering barrier branch
     [bisect][round2] tid=512 reached reduce/smem stage, entering barrier branch
     [HANG DETECTED] ...
     ```
   - **第3轮：分支内部，`asc_syncthreads()`前后 + 自旋入口。** 在`tid < SYNC_THRESHOLD`分支里`asc_syncthreads()`调用的前后各打一点，在`else`分支自旋循环入口打一点（即本样例`BISECT_PRINTF`默认内置的打点）。运行后：
     ```
     [bisect] tid=0 reached reduce/smem stage
     [bisect] tid=512 reached reduce/smem stage
     [bisect] tid=0 BEFORE asc_syncthreads()
     [bisect] tid=512 ENTERING spin loop, will never reach barrier
     [HANG DETECTED] ...
     ```
     `tid=0 BEFORE asc_syncthreads()`打出后再无`AFTER`，说明tid=0所在线程永久阻塞在这一行的`asc_syncthreads()`调用上；`tid=512`打出"进入自旋循环"后同样没有后续打点，说明它确实存活但从未到达barrier——两条线索互相印证，卡死位置被锁定在`asc_syncthreads()`这一行，不用再猜。

   这个"整体收窄、每轮只加打点不删打点"的过程就是二分的核心：每一轮都用上一轮"最后出现的打点在哪"来决定下一轮打点往哪个方向收，而不是凭经验一次性猜中位置。

   **注意事项：** 线程数或打印调用次数较多时，GM缓冲区（默认2MB，可通过acl.json的`simt_printf_fifo_size`调整到1MB~64MB）可能被打满导致丢弃或阻塞，因此建议按官方文档的建议做法：只在少数代表线程（如每种分支路径选1个）上打印，既能定位问题又不明显放大打印量，具体用量建议见[printf() API文档](../../../../docs/zh/api/Utils-API/tuning_interface/printf.md)。

3. **代码审查：排查所有`asc_syncthreads()`调用点**
   针对每一处`asc_syncthreads()`调用，检查其所在分支条件是否可能依赖`tid`/`threadIdx`，并让部分线程在存活状态下（而非退出/retire）跳过该调用——常见于自旋等待、依赖未被正确置位的flag、或提前`return`但线程未真正退出等场景。

## 参考

- [asc_syncthreads() API文档](../../../../docs/zh/api/SIMT-API/sync_and_memory_fence/sync_interface/asc_syncthreads.md)
- [printf() API文档](../../../../docs/zh/api/Utils-API/tuning_interface/printf.md)
