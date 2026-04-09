# reducemax_min_mean_sum样例

## 概述

本样例在归约场景下，基于ReduceMax、ReduceMin、ReduceMean、ReduceSum高阶API实现对一个多维向量数据按照指定维度求最大值、最小值、平均值和求和。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── reducemax_min_mean_sum
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── reducemax_min_mean_sum.asc  // Ascend C样例实现 & 调用样例
```

## 样例规格

<table border="2" align="center">
<caption>reducemax_min_mean_sum</caption>
<tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> reducemax_min_mean_sum </td></tr>
<tr><td rowspan="3" align="center">样例输入</td></tr>
<tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">[136, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="2" align="center">样例输出</td></tr>
<tr><td align="center">y</td><td align="center">[1, 136]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">reduce_custom</td></tr>
</table>

## 样例实现

本样例通过编译参数`SCENARIO_NUM`选择不同的规约场景，固定shape输入[136，32]，按照矩阵最后一个维度（列）求最大值、最小值、平均值和求和。

- Kernel实现

  计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用ReduceMax/ReduceMin/ReduceMean/ReduceSum高阶API接口完成计算，得到结果搬出到外部存储上。

- Tiling实现

  该样例的tiling实现流程如下：使用GetReduceMeanMaxMinTmpSize接口计算所需最大/最小临时空间大小，使用最小临时空间，然后根据输入长度确定所需tiling参数。

- 调用实现

  使用内核调用符<<<>>>调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 编译选项说明

| 参数 | 说明 | 可选值 | 默认值 |
|------|------|---------|--------|
| RUN_MODE | 运行模式 | npu, cpu, sim | npu |
| NPU_ARCH | NPU硬件架构 | dav-2201, dav-3510 | dav-2201 |

### 配置环境变量

请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quickstart.md#prepare&install)，选择对应配置环境变量的命令。

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

### 样例执行

**NPU模式（默认）：**
```bash
SCENARIO_NUM=1
mkdir -p build && cd build;      # 创建并进入build目录
cmake .. -DSCENARIO_NUM=$SCENARIO_NUM -DNPU_ARCH=dav-2201;make -j;    # 编译工程
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
./demo                           # 执行编译生成的可执行程序，执行样例
```

**CPU调试模式：**
```bash
SCENARIO_NUM=1
mkdir -p build && cd build;      # 创建并进入build目录
cmake .. -DSCENARIO_NUM=$SCENARIO_NUM -DRUN_MODE=cpu -DNPU_ARCH=dav-2201;make -j;    # 编译工程
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
./demo                           # 执行编译生成的可执行程序，执行样例
```

**NPU仿真模式：**
```bash
SCENARIO_NUM=1
mkdir -p build && cd build;      # 创建并进入build目录
cmake .. -DSCENARIO_NUM=$SCENARIO_NUM -DRUN_MODE=sim -DNPU_ARCH=dav-2201;make -j;    # 编译工程
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
./demo                           # 执行编译生成的可执行程序，执行样例
```

> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

执行结果如下，说明精度对比成功。
```bash
test pass!
```

