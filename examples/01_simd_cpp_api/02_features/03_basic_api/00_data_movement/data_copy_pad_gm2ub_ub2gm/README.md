# DataCopyPad样例

## 概述

本样例在数据搬运场景下，基于DataCopyPad API实现非32字节对齐数据的搬运及填充功能。DataCopyPad API支持从Global Memory到Local Memory的非对齐数据搬运，并可在数据左侧或右侧填充指定数值，解决硬件要求的32字节对齐约束。

数据搬运过程包括：Global Memory（GM）→VECIN队列（使用DataCopyPad进行非对齐搬运并填充）、VECIN→VECOUT（计算处理）、VECOUT队列→Global Memory（GM）。本样例支持通过编译参数切换不同场景，演示DataCopyPad的不同使用方式。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── data_copy_pad_gm2ub_ub2gm
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── data_copy_pad.asc       // Ascend C样例实现 & 调用样例
```

## 场景说明

本样例通过编译参数 `SCENARIO_NUM` 选择不同场景，所有场景数据格式为 ND，核函数名为 `data_copy_pad_custom`。

<table border="2">
<caption>表1：场景配置对照表</caption>
<tr><th>scenarioNum</th><th>输入Shape</th><th>输出Shape</th><th>填充方式</th><th>说明</th></tr>
<tr><td>1</td><td>[1, 20]</td><td>[1, 32]</td><td>SetPadValue</td><td>右侧填充12个元素，填充值为1</td></tr>
<tr><td>2</td><td>[32, 59]</td><td>[32, 64]</td><td>rightPadding</td><td>右侧填充5个元素，填充值为0</td></tr>
<tr><td>3</td><td>[3, 24]</td><td>[1, 80]</td><td>Compact</td><td>Compact模式，最后一个数据块右侧填充16字节</td></tr>
</table>

### 场景详细说明

**场景1：使用SetPadValue进行自定义填充**
- 输入：[1, 20]个half元素（共20个）
- 输出：[1, 32]个half元素（共32个）
- 参数配置：isPad=false, leftPadding=0, rightPadding=12
- 说明：使用SetPadValue设置填充值为1，右侧填充12个元素

**场景2：使用rightPadding进行默认填充**
- 输入：[32, 59]个half元素（共1888个）
- 输出：[32, 64]个half元素（共2048个）
- 参数配置：isPad=true, leftPadding=0, rightPadding=5
- 说明：不使用SetPadValue，右侧填充5个元素，填充值为0

**场景3：使用Compact模式进行数据搬运**
- 输入：[3, 24]个half元素（共72个）
- 输出：[1, 80]个half元素（共80个）
- 参数配置：blockLen=48, blockCount=3, leftPadding=0, rightPadding=16, isPad=false
- 说明：blockLen为48，每个连续传输数据块包含48字节；srcStride为0，因为源操作数的逻辑位置为GM，srcStride的单位为字节，即源操作数相邻数据块之间紧密排列；dstStride为0，因为目的操作数的逻辑位置为VECIN、VECOUT，dstStride的单位为DataBlock（32字节），目的操作数相邻数据块之间紧密排列，不会填充数据。blockLen * blockCount + leftPadding + rightPadding 满足32字节对齐，isPad为false，左右两侧填充的数据值会默认为随机值。此处示例中，leftPadding为0，rightPadding为16，在最后一个数据块右侧填充16字节。目的操作数的总长度为160字节
- 注意：Compact模式仅支持 Ascend 950PR/Ascend 950DT（dav-3510）

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 编译选项说明

| 参数 | 说明 | 可选值 | 默认值 |
|------|------|---------|--------|
| RUN_MODE | 运行模式 | npu, cpu, sim | npu |
| NPU_ARCH | NPU硬件架构 | dav-2201, dav-3510 | dav-2201 |
| SCENARIO_NUM | 场景编号 | 1, 2, 3 | 1 |

### 配置环境变量

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

### 样例执行

**NPU模式（默认）：**
```bash
SCENARIO_NUM=1                                          # 设置（1~3）
mkdir -p build && cd build;                             # 创建并进入build目录
cmake .. -DSCENARIO_NUM=$SCENARIO_NUM -DNPU_ARCH=dav-2201; make -j;         # 编译工程
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
./demo                                                   # 执行编译生成的可执行程序，执行样例
python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
```

**CPU调试模式：**
```bash
SCENARIO_NUM=1
mkdir -p build && cd build;
cmake .. -DSCENARIO_NUM=$SCENARIO_NUM -DRUN_MODE=cpu -DNPU_ARCH=dav-2201; make -j;
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

**NPU仿真模式：**
```bash
SCENARIO_NUM=1
mkdir -p build && cd build;
cmake .. -DSCENARIO_NUM=$SCENARIO_NUM -DRUN_MODE=sim -DNPU_ARCH=dav-2201; make -j;
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

执行结果如下，说明精度对比成功。
```bash
test pass!
```
