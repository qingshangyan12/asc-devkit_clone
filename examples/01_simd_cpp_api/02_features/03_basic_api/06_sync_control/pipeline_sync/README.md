# PipeBarrier + SetFlag/WaitFlag 流水线同步样例

## 概述

本样例在核内多流水同步场景下，基于PipeBarrier和SetFlag/WaitFlag实现核内同步。样例展示数据搬入、计算、数据搬出阶段中的同步控制，支持Vector模式和Cube模式两种场景。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── pipeline_sync
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── pipeline_sync.asc       // Ascend C样例实现 & 调用样例
```

## 场景说明

本样例通过编译参数 `SCENARIO_NUM` 选择不同场景，所有场景数据格式为 ND。

<table border="2">
<caption>表1：场景配置对照表</caption>
<tr><th>scenarioNum</th><th>输入Shape</th><th>输出Shape</th><th>数据类型</th><th>说明</th></tr>
<tr><td>1</td><td>[1, 768]</td><td>[1, 768]</td><td>float</td><td>Vector模式，执行Add+Mul计算</td></tr>
<tr><td>2</td><td>A:[128, 512]，B:[512, 128]</td><td>[128, 128]</td><td>half→float</td><td>Cube模式，执行Mmad矩阵乘法</td></tr>
</table>

### 场景详细说明

**场景1：Vector模式**
- 输入shape：[1, 768]
- 输出shape：[1, 768]
- 数据类型：float
- 核函数名：`pipeline_sync_vector_custom`
- 说明：采用分块处理模式，分块数量为3，每轮处理256个元素。数据从GM搬入Vector计算流水，先执行Add，再通过PipeBarrier&lt;PIPE_V&gt;确保Add结果可用后执行Mul，最后将Mul结果搬出到GM。

**场景2：Cube模式**
- 输入shape：A矩阵[128, 512]，B矩阵[512, 128]
- 输出shape：[128, 128]
- 数据类型：A/B矩阵为half，C矩阵为float
- 核函数名：`pipeline_sync_cube_custom`
- 说明：基于Mmad实现矩阵乘法，数据搬运路径为GM→L1→L0A/L0B→L0C→GM。K轴迭代8轮，每轮处理K方向64个元素。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  SCENARIO_NUM=1  # 设置场景编号（取值为1、2）
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin -scenarioNum=$SCENARIO_NUM  # 验证输出结果是否正确
  ```

  使用CPU调试或NPU仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如下：
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # CPU调试模式
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # NPU仿真模式
  ```
  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品、dav-3510 对应 Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`（默认）、`2` | 场景编号：1（Vector模式Add+Mul）、2（Cube模式Mmad矩阵乘法） |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
