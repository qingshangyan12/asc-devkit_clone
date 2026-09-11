# DataCopy Pre-Allocation 最佳实践样例


## 概述

本样例在 double buffer 的 DataCopy 搬运场景下，演示 MTE2 Pre-Allocation 特性。两块 UB buffer 轮换复用，每块 buffer 同时承担 CopyIn（GM→UB，MTE2 搬入）与 CopyOut（UB→GM，MTE3 搬出）。double buffer 让相邻两轮迭代使用不同 buffer，从而相邻迭代之间不存在 MTE3→MTE2 屏障；但同一块 buffer 被复用时（每跨一轮），下一轮 MTE2 仍须等待前一轮 MTE3 搬出完成（MTE3→MTE2 屏障）才能向 GM 发出读请求。不使能 Pre-Allocation 时，该屏障期间 MTE2 流水空闲，形成残余 bubble；使能 Pre-Allocation 后，MTE2 搬运指令可提前发出 GM 读请求，数据先暂存在总线上，待同步屏障解除后再写入 UB，从而隐藏 GM 读取延迟，进一步提升 MTE2 流水利用率。

样例不包含计算逻辑，仅做 GM→UB→GM 的往返搬运，便于聚焦观察 MTE2 流水行为。通过编译选项 `ENABLE_PRE_ALLOCATION` 控制是否使能 Pre-Allocation 特性，分别编译基线（OFF）与使能（ON）两个版本进行对比。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 960PR/Ascend 960DT | >= CANN 9.1.0 |

> **说明：** MTE2 Pre-Allocation 特性仅支持 Ascend 960PR/Ascend 960DT（dav-9201），且仅对 MTE2 搬运有效，因此本样例固定使用 dav-9201 架构。

## 目录结构介绍

```
├── data_copy_pre_allocation
│   ├── scripts
│   │   ├── gen_data.py                       // 输入数据和真值数据生成脚本
│   │   └── verify_result.py                  // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                        // 编译工程文件
│   ├── data_utils.h                          // 数据读入写出函数
│   ├── data_copy_pre_allocation.asc          // Ascend C 样例入口与 Kernel 调用
│   └── README.md                             // 样例说明文档
```

## 算子描述

- 算子功能：
  在 double buffer 的 DataCopy 搬运场景下，演示 MTE2 Pre-Allocation 特性对同一块 buffer 复用时 MTE3→MTE2 屏障残余 bubble 的消除效果。
- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="3" align="center">AIC算子</td></tr>
  </tr>
  <tr><td rowspan="2" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">8192</td><td align="center">half</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">y</td><td align="center">8192</td><td align="center">half</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">data_copy_pre_allocation</td></tr>
  </table>

## 样例实现

### 场景构造：double buffer 循环复用

样例将总长度 8192 的输入数据沿元素方向切分为 8 个 tile，每个 tile 1024 个元素。分配两块 UB buffer（ping/pong，`TPosition::VECCALC`），8 个 tile 交替写入两块 buffer：偶数 tile 用 `bufPing`，奇数 tile 用 `bufPong`。每个 tile 执行两步：

1. CopyIn：GM→UB，MTE2 搬运，写入当前轮选中的 buffer。
2. CopyOut：UB→GM，MTE3 搬运，把同一块 buffer 的数据搬回 GM。

double buffer 让相邻两轮（如 tile0 用 ping、tile1 用 pong）使用不同 buffer，故相邻迭代之间不存在 MTE3→MTE2 屏障；但同一块 buffer 被复用时（如 tile2 再次用 ping），其 MTE2 必须等待该 buffer 上一次的 MTE3（tile0 的搬出）完成，形成 MTE3→MTE2 屏障。

```text
double buffer 轮换复用（ping=bufPing, pong=bufPong）：

  tile0(ping): CopyIn(GM->ping, MTE2) -> CopyOut(ping->GM, MTE3)
  tile1(pong): CopyIn(GM->pong, MTE2) -> CopyOut(pong->GM, MTE3)   // 与tile0用不同buffer，无屏障
  tile2(ping): Wait(MTE3_MTE2@ping)   -> CopyIn(GM->ping, MTE2) -> CopyOut(ping->GM, MTE3)  // 等tile0的MTE3
  tile3(pong): Wait(MTE3_MTE2@pong)   -> CopyIn(GM->pong, MTE2) -> CopyOut(pong->GM, MTE3)  // 等tile1的MTE3
  ...
  tile7(pong): Wait(MTE3_MTE2@pong)   -> CopyIn(GM->pong, MTE2) -> CopyOut(pong->GM, MTE3)

不使能 Pre-Allocation：同一块buffer复用时的下一轮MTE2须等前一轮MTE3完成才发GM读请求，GM读取延迟期间MTE2空闲，形成残余bubble。
使能   Pre-Allocation：MTE2提前发GM读请求，数据暂存总线，待MTE3_MTE2屏障解除后立即写入UB，消除残余bubble。
```

### 同步事件管理

样例使用两组硬件同步事件 `EVENT_ID0` 与 `EVENT_ID1`，分别绑定 ping/pong 两块 buffer，通过 `SetFlag/WaitFlag` 显式管理两道屏障：

- `HardEvent::MTE3_MTE2`：前一轮同 buffer 的 CopyOut（MTE3）完成后释放该 buffer，允许本轮 CopyIn（MTE2）覆写。循环开始前预置两个该事件（`EVENT_ID0` 与 `EVENT_ID1`），使两块 buffer 的首次 WaitFlag 都不会因无前序 MTE3 而永久阻塞。
- `HardEvent::MTE2_MTE3`：本轮 CopyIn（MTE2）完成后再执行 CopyOut（MTE3），保证同一块 buffer 上的读后写顺序。

偶数 tile 使用 `bufPing + EVENT_ID0`，奇数 tile 使用 `bufPong + EVENT_ID1`，因此同一块 buffer 的跨轮复用与对应事件 ID 精确对应。

### 关键代码

```cpp
// double buffer：两块UB buffer（ping/pong）轮换复用。
AscendC::LocalTensor<T> bufPing(AscendC::TPosition::VECCALC, bufPingAddr, TILE_LENGTH);
AscendC::LocalTensor<T> bufPong(AscendC::TPosition::VECCALC, bufPongAddr, TILE_LENGTH);

// 预置两个MTE3_MTE2事件，使两块buffer首次MTE2都不会因无前序MTE3而永久阻塞。
AscendC::SetFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID0);
AscendC::SetFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID1);

for (uint32_t i = 0; i < TILE_NUM; i++) {
    // 偶数轮用bufPing+EVENT_ID0，奇数轮用bufPong+EVENT_ID1。
    int32_t eventID = (i % 2 == 0) ? EVENT_ID0 : EVENT_ID1;
    AscendC::LocalTensor<T>& buf = (i % 2 == 0) ? bufPing : bufPong;

    // 等待前一轮同buffer的MTE3搬出完成，释放该块UB供本轮MTE2覆写。
    AscendC::WaitFlag<AscendC::HardEvent::MTE3_MTE2>(eventID);
    // CopyIn：GM->UB，MTE2搬运。enablePreAlloc为true时使能Pre-Allocation，
    // MTE2提前发出GM读请求，跳过MTE3_MTE2屏障等待，数据暂存总线待屏障解除后写入UB。
    AscendC::DataCopy(buf, xGm[i * TILE_LENGTH], TILE_LENGTH, enablePreAlloc);

    // 等待本轮MTE2搬入完成，再执行MTE3搬出，保证同一块buffer上的读后写顺序。
    AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(eventID);
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(eventID);
    // CopyOut：UB->GM，MTE3搬运。
    AscendC::DataCopy(yGm[i * TILE_LENGTH], buf, TILE_LENGTH);

    // 通知下一轮同buffer的MTE2：本轮MTE3搬出已完成，UB可被覆写。
    AscendC::SetFlag<AscendC::HardEvent::MTE3_MTE2>(eventID);
}
// 排空两块buffer最后一次MTE3_MTE2事件。
AscendC::WaitFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID0);
AscendC::WaitFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID1);
```

两块 buffer 的地址显式管理：`bufPingAddr = 0x0`，`bufPongAddr = TILE_LENGTH * sizeof(half) = 2048` 字节，两块 buffer 在 UB 中相邻不重叠。

使能 Pre-Allocation 的入口是 `DataCopy` 的第 4 个参数 `enablePreReadIgnoreSync`：连续搬运重载 `DataCopy(dst, src, count, enablePreReadIgnoreSync)`，传入 `true` 即使能。样例通过模板参数 `enablePreAlloc` 和编译选项 `ENABLE_PRE_ALLOCATION` 在编译期切换基线与使能版本。

### Pre-Allocation 原理

Ascend 960PR/Ascend 960DT 的 HBM 带宽较大，单核算力不足，当访存请求发送量不足、outstanding不能充满时，会出现请求间隙，算子不能打满带宽。在当前 double buffer 循环复用场景下，同一块 UB buffer 既承担 MTE2 搬入又承担 MTE3 搬出，该 buffer 被复用时下一轮的 MTE2 搬运指令必须等待前一轮的 MTE3 完成（MTE3→MTE2 屏障）才能向 GM 发出读请求，GM 读取延迟期间 MTE2 流水空闲，形成残余 bubble。double buffer 已通过相邻迭代使用不同 buffer 减小了 bubble，但 buffer 复用时仍有该屏障残余。

使能 Pre-Allocation 后，芯片支持 MTE2 相关指令在与前序指令存在数据依赖的情况下仍然提前发出，硬件保证数据的返回维持依赖关系——数据先从 GM 读到总线上暂存，待同步屏障解除后再写入 LocalMem，从而隐藏总线 latency，消除残余 bubble。相关文档参考 [MTE2支持Pre-Allocation](../../../../../docs/zh/guide/programming_guide/advanced_programming/mte2_pre_allocation.md)。

### 约束说明

- 该特性仅支持Ascend 960PR/Ascend 960DT，且仅对MTE2搬运有效。
- 如果该搬运指令读取的数据是前一条指令的输出，则应设置同步依赖关系，并且**禁用**Pre-Allocation功能。
- 该特性适用于存在MTE2前置屏障（如MTE3→MTE2、V→MTE2等）且该屏障会阻塞MTE2发出GM读请求的场景；若MTE2无前置屏障约束，使能该特性不会带来额外收益。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。本样例使用 NPU 仿真模式运行，通过 `cannsim` 采集性能数据。

- 配置环境变量

  请根据当前环境上 CANN 开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。


  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  分别编译基线（`ENABLE_PRE_ALLOCATION=OFF`）与使能（`ENABLE_PRE_ALLOCATION=ON`）两个版本。

  ```bash
  # 使能 Pre-Allocation 版本（ON）
  mkdir -p build && cd build
  cmake -DENABLE_PRE_ALLOCATION=ON ..;make -j;
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  ```bash
  # 基线版本（OFF），需清理 cmake 缓存后重新 cmake
  rm CMakeCache.txt
  cmake -DENABLE_PRE_ALLOCATION=OFF ..;make -j;
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  > **注意：** 切换 `ENABLE_PRE_ALLOCATION` 前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

  生成测试数据后可以用 `cannsim` 仿真运行和采集报告。

  示例：

  ```bash
  cannsim record ./demo -s Ascend960 --gen-report
  ```

  `cannsim record ./demo -s Ascend960 --gen-report` 执行成功后，会在 build 目录下生成以 `cannsim_<时间戳>_demo` 命名的文件夹，性能报告位于其中的 `report/results/kernel_0_reports/` 目录。

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | `ENABLE_PRE_ALLOCATION` | 是否使能 MTE2 Pre-Allocation | `ON`、`OFF` | `ON` |
  | `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-9201` | `dav-9201` |

## 性能数据

下表数据由 `cannsim record ./demo -s Ascend960 --gen-report` 仿真采集，从 `cannsim_<时间戳>_demo/report/results/kernel_0_reports/` 下的 `aicore_utilization.json` 与 `core_0_tracing_report_0.json` 提取。样例单核运行（`numBlocks=1`），数据为 half、总长度 8192（16KB），仅做 GM→UB→GM 往返搬运，double buffer（ping/pong）轮换复用。`Task Duration` 取 AIV0（Core 0）trace 总时长；`MTE2 跨度`/`MTE3 跨度` 为对应流水首条指令 issue 到末条指令完成的时间跨度，反映流水实际占用区间；`WAIT_FLAG 跨度` 为同步等待区间。

<table>
<tr><th>配置</th><th>Task Duration(μs)</th><th>MTE2 跨度(μs)</th><th>MTE3 跨度(μs)</th><th>WAIT_FLAG 跨度(μs)</th><th>AIV0 利用率</th><th>相对基线 Task Duration 性能提升</th></tr>
<tr><td>ENABLE_PRE_ALLOCATION=OFF<br>（基线，未使能）</td><td>6.020</td><td>4.377</td><td>4.755</td><td>4.727</td><td>99.93%</td><td>基线</td></tr>
<tr><td>ENABLE_PRE_ALLOCATION=ON<br>（使能 Pre-Allocation）</td><td>3.742</td><td>2.103</td><td>2.477</td><td>2.449</td><td>99.89%</td><td>+60.8%</td></tr>
</table>

> **说明：** 表中“性能提升”按 `基线 Task Duration / 使能 Task Duration - 1` 计算，正值表示性能提升。

### 优化效果分析

- 使能 Pre-Allocation 后，MTE2 跨度从 4.377μs 压缩到 2.103μs（-51.9%），MTE3 跨度从 4.755μs 压缩到 2.477μs（-47.9%），说明同一块 buffer 复用时的下一轮 MTE2 不再被 MTE3→MTE2 屏障阻塞，可提前发出 GM 读请求，搬入搬出流水进一步重叠。
- 同步等待区间（WAIT_FLAG 跨度）从 4.727μs 缩短到 2.449μs（-48.2%），印证 buffer 复用时的屏障等待被 Pre-Allocation 隐藏。
- 端到端 Task Duration 从 6.020μs 降到 3.742μs，性能提升约 60.8%。AIV0 利用率两者均接近满载，说明性能差异主要来自流水重叠程度而非占用率。
- double buffer 相比单 buffer 已通过相邻迭代使用不同 buffer 减小了 bubble，但同一块 buffer 复用时仍存在 MTE3→MTE2 屏障残余；Pre-Allocation 专门消除该残余 bubble。由于本样例数据量较小（16KB）、单核运行，主要用以定性验证 Pre-Allocation 对 double buffer 场景残余 bubble 的消除效果；实际算子中的收益取决于 MTE2 前置屏障的阻塞程度与 GM 读取延迟。

### 优化要点总结

| 优化手段 | 核心原理 | 使用建议 |
|:---|:---|:---|
| 使能 MTE2 Pre-Allocation | MTE2 搬运指令在前序 MTE3/MTE1 屏障未解除时即可提前发出 GM 读请求，数据暂存总线，待屏障解除后写入 LocalMem，隐藏 GM 读取延迟 | double buffer / 单 buffer 循环复用同一块 UB/L1 buffer 且存在 MTE3→MTE2 或 V→MTE2 屏障时使能；仅 Ascend 960PR/DT 支持，且仅对 MTE2 有效 |
