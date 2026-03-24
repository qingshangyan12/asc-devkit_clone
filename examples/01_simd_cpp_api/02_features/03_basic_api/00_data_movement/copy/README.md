# Copy接口多场景示例

## 概述

本样例介绍Copy接口在多种场景下的使用方法。Copy接口用于在Unified Buffer内部进行数据搬运（VECIN、VECCALC、VECOUT之间），支持mask连续模式、mask逐bit模式和counter模式。样例支持通过编译参数切换不同场景，便于开发者理解Copy接口的使用方法。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── copy
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── copy.asc         // Ascend C样例实现 & 调用样例
```

## 样例描述

Copy接口用于在Unified Buffer内部进行数据搬运，支持通过mask控制每次迭代内参与计算的元素，通过repeatTime控制迭代次数，通过stride参数控制地址步长。本样例展示了以下四种场景：

<caption>表1：场景配置对照表</caption>

|| scenarioNum | 输入Shape | 输出Shape | 搬运模式 | 说明 |
||-------------|----------|----------|---------|------|
|| 1 | [1, 512] | [1, 512] | mask连续模式 | 简单数据搬运，源和目的空间相同 |
|| 2 | [18, 64] | [18, 8] | mask连续模式 | 从大空间搬运部分数据，源和目的空间不同 |
|| 3 | [1, 1024] | [1, 1024] | counter模式 | 使用counter模式搬运数据 |
|| 4 | [18, 64] | [18, 8] | counter模式 | 使用counter模式从大空间搬运部分数据 |

### 场景详细说明

**场景1：mask连续模式，源和目的空间相同**
- 输入输出：[1, 512]个int32元素
- 参数配置：mask=64, repeatTime=8, stride={1, 1, 8, 8}
- 说明：每次迭代处理64个元素，迭代8次，共搬运512个元素

**场景2：mask连续模式，源和目的空间不同**
- 输入：[18, 64]个int32元素（共1152个）
- 输出：[18, 8]个int32元素（共144个）
- 参数配置：mask=8, repeatTime=18, stride={1, 1, 1, 8}
- 说明：从每行64个元素中搬运前8个，共18行；srcRepeatSize=1跳过64个元素（8个block），dstRepeatSize=8紧凑排列

**场景3：counter模式，源和目的空间相同**
- 输入输出：[1, 1024]个int32元素
- 参数配置：使用SetVectorMask设置counter模式，mask=256, repeatTime=4, stride={1, 1, 32, 32}
- 说明：counter模式下mask代表每次repeat处理的元素个数，迭代次数生效

**场景4：counter模式，源和目的空间不同**
- 输入：[18, 64]个int32元素（共1152个）
- 输出：[18, 8]个int32元素（共144个）
- 参数配置：使用SetVectorMask设置counter模式，mask=144, repeatTime=1, stride={1, 8, 8, 8}
- 说明：counter模式下每次迭代处理144个元素（全部行），迭代1次；srcStride=8（每次选择每一行的起始datablock）

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
