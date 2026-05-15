# Add算子多场景样例

## 概述

本样例基于静态Tensor编程范式实现Add向量加法计算，展示了4种不同的同步和切分策略。本样例给出了4种不同的测试场景（scenario）。

<table>
  <tr>
    <td align="center">scenarioNum</td>
    <td align="center">Shape</td>
    <td align="center">同步方式</td>
    <td align="center">切分策略</td>
    <td align="center">缓冲策略</td>
  </tr>
  <tr>
    <td align="center">1</td>
    <td align="center">[8, 2048]</td>
    <td align="center">PipeBarrier</td>
    <td align="center">不切分</td>
    <td align="center">单缓冲</td>
  </tr>
  <tr>
    <td align="center">2</td>
    <td align="center">[8, 2048]</td>
    <td align="center">SetFlag/WaitFlag</td>
    <td align="center">不切分</td>
    <td align="center">单缓冲</td>
  </tr>
  <tr>
    <td align="center">3</td>
    <td align="center">[2048, 2048]</td>
    <td align="center">SetFlag/WaitFlag</td>
    <td align="center">Tile切分（64次循环）</td>
    <td align="center">单缓冲</td>
  </tr>
  <tr>
    <td align="center">4</td>
    <td align="center">[2048, 2048]</td>
    <td align="center">SetFlag/WaitFlag</td>
    <td align="center">Tile切分（64次循环）</td>
    <td align="center">双缓冲</td>
  </tr>
</table>

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── add
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   │   └── verify_result.py           // 真值对比文件
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   └── add.asc                        // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  Add计算公式：

  ```python
  z = x + y
  ```

- 样例规格：  
  本样例使用8个核完成计算，不同场景的输入规格如下表所示：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Add</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

  **场景参数说明**：
  - 场景1和场景2：M=8, N=2048，总数据量=16384
  - 场景3和场景4：M=2048, N=2048，总数据量=4194304

  **分核逻辑**

  - 场景1和场景2：
    - 输入数据按行方向分核，共分为8个分块
    - 每个核处理2048个数据（即1行数据）
    - 核数配置：8核

  - 场景3和场景4：
    - 输入数据按行方向分核，共分为8个分块
    - 每个核处理256×2048=524288个数据（即256行数据）
    - 每个核内部按Tile切分，每个Tile处理4×2048=8192个数据（即4行数据）
    - Tile切分次数：524288 / 8192 = 64次循环
    - 核数配置：8核

- 样例实现：
  - 整体计算流程：

    样例整体流程如下：

    **场景1：PipeBarrier同步（不切分）**
    ```
    GM(x:ND,float) ----> UB(VECCALC,float) ----> GM(z:ND,float)
                   │             │             │
                DataCopy        Add         DataCopy
               GM->UB搬运    向量加法计算    UB->GM搬运
                   │             │
              PipeBarrier   PipeBarrier
                (同步1)        (同步2)
    ```

    **场景2、3：SetFlag/WaitFlag同步（不切分）**
    ```
    GM(x:ND,float) ----> UB(VECCALC,float) ----> GM(z:ND,float)
                 │             │                 │
              DataCopy        Add             DataCopy
             GM->UB搬运    向量加法计算        UB->GM搬运
                 │             │                 |
          SetFlag<MTE2_V>   SetFlag<V_MTE3>   SetFlag<MTE3_MTE2>
         WaitFlag<MTE2_V>  WaitFlag<V_MTE3>  WaitFlag<MTE3_MTE2>
              (同步1)        (同步2)        （同步3，适用于场景3）
    ```

    **场景4：双缓冲Tile切分**
    ```
    GM(x:ND,float) ----> UB(Ping/Pong,float) ----> GM(z:ND,float)  (循环64次)
                │                │                 │
            DataCopy            Add             DataCopy
           GM->UB搬运       向量加法计算        UB->GM搬运
                │                │                 │
           SetFlag<MTE2_V>   SetFlag<V_MTE3>    SetFlag<MTE3_MTE2>
          WaitFlag<MTE2_V>  WaitFlag<V_MTE3>   WaitFlag<MTE3_MTE2>
             (同步1)          (同步2)           (双缓冲流水并行)
    ```

  - 同步机制说明：

    **PipeBarrier同步（场景1）**：
    - 使用`PipeBarrier<PIPE_ALL>`在数据搬运、计算、写出之间插入全流水同步
    - 同步点列表：
      1. GM→UB搬运完成后，执行PipeBarrier<PIPE_ALL>同步
      2. Add计算完成后，执行PipeBarrier<PIPE_ALL>同步
    - 特点：简单易用，但流水效率较低，各流程串行执行

    **SetFlag/WaitFlag同步（场景2、3）**：
    - 使用`SetFlag`和`WaitFlag`实现事件同步，更加灵活高效
    - 同步点列表：
      1. GM→UB搬运完成后，SetFlag<MTE2_V>+WaitFlag<MTE2_V>同步搬运完成
      2. Add计算完成后，SetFlag<V_MTE3>+WaitFlag<V_MTE3>同步计算完成
      3. （场景3）UB→GM写出完成后，SetFlag<MTE3_MTE2>+WaitFlag<MTE3_MTE2>同步搬运完成
    - 特点：可精确控制同步时机，减少同步开销，流水效率高于PipeBarrier，但各流程仍串行执行

    **SetFlag/WaitFlag同步（场景4）**：
    - 使用`SetFlag`和`WaitFlag`实现双缓冲Ping-Pong流水同步
    - 同步点列表（每次循环）：
      1. 循环开始前，SetFlag<MTE3_MTE2>初始化双缓冲同步事件
      2. WaitFlag<MTE3_MTE2>等待上一次写出完成（双缓冲流水并行）
      3. GM→UB搬运完成后，SetFlag<MTE2_V>+WaitFlag<MTE2_V>同步搬运完成
      4. Add计算完成后，SetFlag<V_MTE3>+WaitFlag<V_MTE3>同步计算完成
      5. UB→GM写出完成后，SetFlag<MTE3_MTE2>标记写出完成，供下一次循环等待
    - 特点：Ping-Pong交替插入同步，使搬运和计算流程并行执行，流水效率高于单缓冲串行执行

  - Tile切分说明（场景3、4）：

    **Tile切分参数**：
    - 单核总数据量：256×2048=524288个元素
    - Tile大小：4×2048=8192个元素（32KB）
    - Tile切分次数：524288 / 8192 = 64次循环
    - 每次循环处理4行数据（4×2048个元素）

    **单缓冲vs双缓冲对比**：
    - 场景3（单缓冲）：每次循环串行执行搬运和计算，流水效率较低
    - 场景4（双缓冲）：Ping-Pong交替，搬运和计算可并行执行，流水效率高

  - Kernel实现

    **同步方式差异**

    <table>
      <tr>
        <td align="center">scenarioNum</td>
        <td align="center">同步方式</td>
        <td align="center">实现描述</td>
      </tr>
      <tr>
        <td align="center">1</td>
        <td align="center">PipeBarrier</td>
        <td align="center">调用PipeBarrier<PIPE_ALL>接口在搬运、计算、写出之间插入全流水同步</td>
      </tr>
      <tr>
        <td align="center">2/3/4</td>
        <td align="center">SetFlag/WaitFlag</td>
        <td align="center">调用SetFlag和WaitFlag接口实现事件同步，精确控制搬运和计算的流水时序</td>
      </tr>
    </table>

    **切分策略差异**

    <table>
      <tr>
        <td align="center">scenarioNum</td>
        <td align="center">切分策略</td>
        <td align="center">缓冲策略</td>
        <td align="center">实现描述</td>
      </tr>
      <tr>
        <td align="center">1/2</td>
        <td align="center">不切分</td>
        <td align="center">单缓冲</td>
        <td align="center">单次搬运、计算、写出，每个核处理2048个数据</td>
      </tr>
      <tr>
        <td align="center">3</td>
        <td align="center">Tile切分</td>
        <td align="center">单缓冲</td>
        <td align="center">64次循环，每次搬运8192个数据（32KB），单缓冲串行执行</td>
      </tr>
      <tr>
        <td align="center">4</td>
        <td align="center">Tile切分</td>
        <td align="center">双缓冲</td>
        <td align="center">64次循环，每次搬运8192个数据（32KB），Ping-Pong交替，搬运和计算并行执行</td>
      </tr>
    </table>

    **通用计算流程**

    - 调用DataCopy基础API，将数据从GM（Global Memory）搬运到UB（Unified Buffer）。
    - 调用Add接口，对两个输入Tensor执行加法操作。
    - 调用DataCopy基础API，将计算结果从UB（Unified Buffer）搬运至GM（Global Memory）。

- 调用实现  
  使用内核调用符<<<>>>调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行
  ```bash
  SCENARIO=1
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO ..;make -j;                      # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO                     # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |
| `SCENARIO_NUM` | `1`、`2`、`3`、`4` | 场景编号：1=PipeBarrier同步，2=SetFlag/WaitFlag同步，3=Tile切分单缓冲，4=Tile切分双缓冲 |

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
