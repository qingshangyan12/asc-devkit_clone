# 标量原子操作样例

## 概述

本样例基于 [`AtomicAdd`](../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md) 和 [`AtomicCas`](../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicCas.md) 接口，介绍在 Global Memory（GM）地址上进行标量原子加和标量原子比较交换的实现流程。原子操作可保证多个核并行访问同一内存地址时原子更新的一致性。

标量原子操作使用标量流水（PIPE_S），GM到UB的 [`DataCopy`](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMAndUB_continuous.md) 使用搬运流水（PIPE_MTE2）。当两者访问同一GM地址并存在数据依赖时，编译器不会自动插入同步，需要通过核内多流水同步手动建立先后关系。本样例的场景3用于演示该约束：仅启动一个核，在同一核内执行原子操作、[`SetFlag/WaitFlag`](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md)、`DataCopy`，最后使用[`PipeBarrier`](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md)完成流水同步。

> **接口提示：** 除本样例使用的 `AtomicAdd`、`AtomicCas` 接口外，Ascend C 还提供了 `AtomicExch`、`AtomicMax`、`AtomicMin` 接口，调用方式与 `AtomicAdd` 一致，只需替换函数名即可切换。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.1.0 |

## 目录结构介绍

```
├── scalar_atomic_operations
│   ├── scripts
│   │   ├── gen_data.py               // 输入数据和真值数据生成脚本
│   │   └── verify_result.py          // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                // 编译工程文件
│   ├── data_utils.h                  // 数据读入写出函数
│   ├── scalar_atomic_operations.asc  // Ascend C样例实现 & 调用样例
│   └── README.md                     // 样例说明文档
```

## 场景详细说明

本样例通过编译参数 `SCENARIO_NUM` 来切换不同的场景：

**场景1：AtomicAdd原子加操作**  
  三个核并行执行，每个核对GM的首个元素（dst[0]）原子加1。最终dst[0]的值为初始值加3

**场景2：AtomicCas原子比较交换操作**  
  三个核并行执行，每个核检查GM首个元素是否等于expected值1，若相等则替换为newValue值2，不相等则不修改。

**场景3：单核标量原子操作与搬运同步**
  仅启动一个核执行 `AtomicAdd`。原子操作完成后，使用 `SetFlag<HardEvent::S_MTE2>` 和 `WaitFlag<HardEvent::S_MTE2>` 建立标量流水到GM到UB搬运流水的核内同步，再调用 `DataCopy` 将GM数据搬运到UB，并通过 `PipeBarrier<PIPE_ALL>` 等待此前流水指令完成。本场景专门展示标量原子操作与搬运接口存在数据依赖时的核内同步约束，不引入多核访问竞争。

<table border="2">
<caption>表1：场景配置对照表</caption>
<tr><th>scenarioNum</th><th>原子操作接口</th><th>输出Shape</th><th>数据类型</th><th>说明</th></tr>
<tr><td>1</td><td><a href="../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md">AtomicAdd</a></td><td>[1, 256]</td><td>int32</td><td>三个核并行对GM首个元素进行原子加1操作</td></tr>
<tr><td>2</td><td><a href="../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicCas.md">AtomicCas</a></td><td>[1, 256]</td><td>uint32</td><td>三个核并行对GM首个元素进行原子比较交换（如果值为1则替换为2）</td></tr>
<tr><td>3</td><td><a href="../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md">AtomicAdd</a>、<a href="../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMAndUB_continuous.md">DataCopy</a>、<a href="../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md">SetFlag/WaitFlag</a>、<a href="../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md">PipeBarrier</a></td><td>[1, 256]</td><td>int32</td><td>单核执行原子加，并演示原子操作与GM到UB搬运之间的核内同步</td></tr>
</table>

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  SCENARIO_NUM=1  # 设置场景编号（取值为1、2、3）
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin -scenarioNum $SCENARIO_NUM  # 验证输出结果是否正确
  ```

  使用CPU调试或NPU仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：

  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # CPU调试模式
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU 架构：AtomicAdd和AtomicCas仅支持dav-3510（对应 Ascend 950PR&950DT系列产品） |
  | `SCENARIO_NUM` | `1`（默认）、`2`、`3` | 场景编号：1（多核AtomicAdd原子加）、2（多核AtomicCas原子比较交换）、3（单核AtomicAdd与搬运同步） |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
