# SIMD与SIMT混合编程asc_syncthreads()卡死定位样例

## 概述

本样例基于SIMD和SIMT混合编程模式，演示`asc_syncthreads()`的一个已知卡死风险：在分支中调用block barrier时，若有线程无法到达同步点，其他已到达同步点的线程会永久阻塞。样例提供一套可复用的卡死定位流程：如何在不被卡死拖死进程的前提下，及时发现并诊断这类问题。

定位手段覆盖三条互补路径：带超时的流同步（止损并确认确实卡死）、msdebug中断运行（不预先插桩，卡住时手动`CTRL+C`中断并回显中断位置与各线程停留行）、printf二分打点（在真实卡死的那次运行上观察历史执行轨迹）。

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
  | `HANG_FOREVER` | `OFF`（默认）、`ON` | 改用无超时的`aclrtSynchronizeStream`，让进程真正保持卡死，以便用msdebug手动中断，见第3步 |
  | `MSDEBUG_BUILD` | `OFF`（默认）、`ON` | 以`-g -O0`编译，使二进制带调试信息，msdebug才能回显文件名和行号，见第3步 |

- 执行结果

  默认配置（`HANG_FOREVER=OFF`）下约5秒后自动检测到超时并退出（`exit code 1`），不会无限期挂起：
  ```text
  [HANG DETECTED] aclrtSynchronizeStreamWithTimeout timed out after 5000ms, ret=507046 (ACL_ERROR_RT_STREAM_SYNC_TIMEOUT).
  Diagnosis: ...
  Localization steps:
    1. ...
    2. ...
    3. ...
    4. ...
  ```

  `HANG_FOREVER=ON`时进程会一直卡住不退出，这是msdebug中断运行场景的预期行为——需要手动`CTRL+C`（在msdebug中）或结束进程。

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
     ```text
     [bisect][round1] tid=0 ENTER sync_hang_demo_vf
     [HANG DETECTED] ...
     ```
   - **第2轮：warp归约之后、分支同步之前。** 把打点往函数体内部移，放在`asc_reduce_add`归约结束、写`smem`之后，同时从分支两侧各选一个代表线程（`tid==0`和`tid==SYNC_THRESHOLD`）打点。运行后两侧都打出来了，说明归约阶段本身没问题，两侧线程都正常走到了分支入口——把可疑范围进一步收窄到分支内部。
     ```text
     [bisect][round2] tid=0 reached reduce/smem stage, entering barrier branch
     [bisect][round2] tid=512 reached reduce/smem stage, entering barrier branch
     [HANG DETECTED] ...
     ```
   - **第3轮：分支内部，`asc_syncthreads()`前后 + 自旋入口。** 在`tid < SYNC_THRESHOLD`分支里`asc_syncthreads()`调用的前后各打一点，在`else`分支自旋循环入口打一点（即本样例`BISECT_PRINTF`默认内置的打点）。运行后：
     ```text
     [bisect] tid=0 reached reduce/smem stage
     [bisect] tid=512 reached reduce/smem stage
     [bisect] tid=0 BEFORE asc_syncthreads()
     [bisect] tid=512 ENTERING spin loop, will never reach barrier
     [HANG DETECTED] ...
     ```
     `tid=0 BEFORE asc_syncthreads()`打出后再无`AFTER`，说明tid=0所在线程永久阻塞在这一行的`asc_syncthreads()`调用上；`tid=512`打出"进入自旋循环"后同样没有后续打点，说明它确实存活但从未到达barrier——两条线索互相印证，卡死位置被锁定在`asc_syncthreads()`这一行，不用再猜。

   这个"整体收窄、每轮只加打点不删打点"的过程就是二分的核心：每一轮都用上一轮"最后出现的打点在哪"来决定下一轮打点往哪个方向收，而不是凭经验一次性猜中位置。

   **注意事项：** 线程数或打印调用次数较多时，GM缓冲区（默认2MB，可通过acl.json的`simt_printf_fifo_size`调整到1MB~64MB）可能被打满导致丢弃或阻塞，因此建议按官方文档的建议做法：只在少数代表线程（如每种分支路径选1个）上打印，既能定位问题又不明显放大打印量，具体用量建议见[printf() API文档](../../../../docs/zh/api/Utils-API/tuning_interface/printf.md)。

3. **用msdebug的中断运行功能抓卡死现场**

   printf打点的前提是"提前猜到该打在哪并重新编译"。msdebug的[中断运行](https://www.hiascend.com/document/detail/zh/mindstudio/latest/msOT/Operatordevelopmenttools/docs/zh/user_guide/msdebug_user_guide.md)功能提供了另一条路径：**不预先插桩，等程序卡住时手动中断，让工具直接回显中断位置**。对于卡死类问题这正是最贴合的场景——官方文档明确把"算子用到了同步类指令"列为卡顿的两类常见原因之一，`asc_syncthreads()`就属于这一类。

   **前提：让进程真正保持卡死。** 默认配置下样例5秒后就自己超时退出了，没有"正在卡顿的程序"可中断。因此中断运行要配合`HANG_FOREVER=ON`（改用无超时的`aclrtSynchronizeStream`）和`MSDEBUG_BUILD=ON`（`-g -O0`，否则回显里没有文件名和行号）：

   ```bash
   mkdir -p build && cd build
   cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DHANG_FOREVER=ON -DMSDEBUG_BUILD=ON ..
   make -j
   msdebug ./demo
   ```

   在msdebug中输入`run`启动，程序会停在提示处不再前进：

   ```text
   (msdebug) run
   ### SIMD/SIMT hybrid asc_syncthreads() hang demo
   Launching kernel...
   Waiting on aclrtSynchronizeStream (NO timeout) -- process will hang here.
   In msdebug, press CTRL+C now to interrupt and echo the hang location.
   ```

   确认卡住后**按`CTRL+C`**，工具中断运行并回显中断位置（`stop reason = signal SIGSTOP`，并给出聚焦的核与源码行）：

   ```text
   Process <pid> stopped
   [Switching to focus on Kernel kernel_sync_hang_demo, CoreId <n>, Type aiv]
   * thread #1, name = 'demo', stop reason = signal SIGSTOP
       frame #0: 0x... device_debugdata0`sync_hang_demo_vf(...) at sync_hang_demo.asc:79
   ```

   再用两条命令把"卡在哪"落到线程粒度：

   ```text
   (msdebug) ascend info cores      # 各核停在哪：CoreId/Type/PC/stop reason/Filename/Line
   (msdebug) ascend info threads    # SIMT线程停在哪：ThreadIdx区间/ActiveCount/PC/ActiveMask/Filename/Line
   ```

   `ascend info threads`是这个场景里信息量最大的一条：它按`ThreadIdx To ThreadIdx`区间列出存活线程各自停在哪一行。本样例的两组线程会停在不同位置——`tid < 512`的线程停在`asc_syncthreads()`所在行（本文件第79行），`tid >= 512`的线程停在自旋循环所在行（第81行）。这两行同时出现，就把"一部分线程等在barrier上、另一部分线程活着但停在别处永不到达barrier"这个因果关系直接摆了出来，不需要像printf那样先猜位置再重编译。

   调试结束输入`q`，再输入`Y`确认退出。

   **中断后可用的功能是受限的。** 按官方文档，中断生效后只有**调试信息展示**（`ascend info *`）和**核切换**（`ascend aiv <id>` / `ascend aic <id>`）可用；**单步调试、读寄存器、内存与变量打印、`continue`均不可用**。所以中断运行的定位思路是"看清停在哪"，而不是"停下来继续调"。

   **其他限制（务必先确认再投入时间）：**

   - 只能中断**在msdebug内部启动**的算子程序。已经在工具外跑起来、正卡着的进程无法用这种方式中断，必须重新在`msdebug ./demo`下启动。
   - `docs`中的[NPU域上板调试](../../../../docs/zh/guide/编程指南/调试调优/功能调试/NPU域上板调试.md)记载msDebug"当前只支持SIMD编程场景的程序调试，暂不支持SIMT编程场景"，而msDebug用户指南又将`ascend info threads`/`ascend thread`列为`simt_vf`上板调试专有命令。两处说法不一致，SIMT场景下的实际支持程度以你所用工具版本的实测结果为准。
   - **必须先打开驱动的调试通道，否则msdebug根本起不来。** 这是使用本步骤前最容易踩的前置条件。若驱动安装时没有开启调试能力，`msdebug ./demo`会在设备侧初始化阶段直接失败退出：
     ```text
     msdebug failed to initialize. please install HDK.
     [ERROR] error code: 0x20102
     ```
     判断方法：检查`/proc/debug_switch`是否存在。不存在即说明调试通道未开启。按官方文档，二者选其一（均需root权限，且会重装驱动，请自行评估对共享环境的影响）：
     ```bash
     # 方法1（CANN 8.1.RC1及以上配driver 25.0.RC1及以上推荐）：--full安装后打开调试开关
     ./Ascend-hdk-<chip_type>-npu-driver_<version>_linux-<arch>.run --full
     echo 1 > /proc/debug_switch          # 需root
     # 方法2：安装驱动时直接带--debug
     ./Ascend-hdk-<chip_type>-npu-driver_<version>_linux-<arch>.run --debug
     ```
     容器场景还需确认msdebug依赖的驱动设备节点已映射进容器且容器有访问权限。

   - **本样例的实测边界（如实说明）：** 编写本节的环境（driver `7.0.t9.0.B849`）上`/proc/debug_switch`不存在，即调试通道未开启，msdebug以上述`0x20102`失败退出，因此`CTRL+C`回显与`ascend info threads`的**实际输出未能在本环境跑通**，这两段回显格式取自msDebug用户指南。已确认该失败与本样例无关：同一环境下调试任意程序都在`aclrtBinaryLoadFromDataImpl`阶段以同样错误中止，而样例不带调试器时可正常运行并稳定复现卡死。已实测确认的部分是：`-g -O0`编译后**断点能解析进`__simt_vf__`函数**，符号与行号信息可用，说明SIMT VF代码的调试信息本身是完整的：
     ```text
     (msdebug) b sync_hang_demo.asc:79
     Breakpoint 1: where = device_debugdata0`sync_hang_demo_vf(...) (.vector) + 192 at sync_hang_demo.asc:79:9
     ```
     开启调试通道后，本节流程可直接使用。

   **与printf打点如何选择：** 两者互补，建议按"先中断、后打点"的顺序。中断运行不需要预先插桩、不需要重新编译，适合面对一个完全不熟悉的卡死kernel时**第一次抓现场**，一次就能同时看到所有线程停在哪；printf则不依赖驱动调试通道是否开启（无法重装驱动的环境下它是唯一手段），且能观察卡死前的**历史执行轨迹**——哪些打点已经过去了，这是中断运行看不到的。定位结论最好由两者互相印证。

4. **代码审查：排查所有`asc_syncthreads()`调用点**
   针对每一处`asc_syncthreads()`调用，检查其所在分支条件是否可能依赖`tid`/`threadIdx`，并让部分线程在存活状态下（而非退出/retire）跳过该调用——常见于自旋等待、依赖未被正确置位的flag、或提前`return`但线程未真正退出等场景。

## 参考

- [asc_syncthreads() API文档](../../../../docs/zh/api/SIMT-API/sync_and_memory_fence/sync_interface/asc_syncthreads.md)
- [printf() API文档](../../../../docs/zh/api/Utils-API/tuning_interface/printf.md)
- [NPU域上板调试（msDebug章节）](../../../../docs/zh/guide/编程指南/调试调优/功能调试/NPU域上板调试.md)
- [《msDebug用户指南》——中断运行、核切换、调试信息展示](https://gitcode.com/Ascend/msdebug/blob/master/docs/zh/user_guide/msdebug_user_guide.md)
