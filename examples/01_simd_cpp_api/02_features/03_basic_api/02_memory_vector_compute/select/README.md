# Select类样例

## 概述

本样例基于Select接口完成多种场景模式下的数据选择功能，根据`selMask`掩码在两个源操作数或源操作数与标量之间选取元素写入目的操作数。选择的规则为：当selMask的比特位是1时，从src0中选取，比特位是0时从src1选取。

样例支持通过编译参数切换不同场景，便于开发者理解Select接口的使用方法和实现差异。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── select
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── select.asc              // Ascend C样例实现 & 调用样例
```

## 场景详细说明

本样例通过编译参数`SCENARIO_NUM` 来切换不同的选择场景：

**场景1：Select**
- 说明：该场景有三种模式，本样例介绍模式1的用法。

  给定两个源操作数`src0Local`和`src1Local`，根据`selMask`（用于选择的Mask掩码）的比特位值选取元素。其中selMask中有效数据的个数存在限制：256/sizeof(T)，T为输入数据类型。每轮迭代中，根据selMask的有效位数据进行选择操作，每一轮迭代采用的selMask，均为selMask的有效数值。
- 输入：src0Local=[1, 256], src1Local=[1, 256], selMask=[1, 128]
- 输出：dstLocal=[1, 256]
- 数据类型：输入输出数据类型float，selMask数据类型uint8
- 实现：
    ```cpp
    AscendC::Select(dstLocal, selMask, src0Local, src1Local, cmpMode, count);
    ```
- 参数：cmpMode=AscendC::SELMODE::VSEL_CMPMASK_SPR，根据selMask在src0和src1之间选择元素，count=256表示处理的数据个数

**场景2：Select（灵活标量位置）**
- 说明：与场景1的模式1功能类似，但是标量配置更灵活。

  根据selMask在源操作数`src0Local`和标量`0`之间选取元素，支持灵活的标量参数位置。其中`selMask`无有效数据限制。多轮迭代时，每轮迭代连续使用selMask的不同部分。
- 输入：src0Local=[1, 256], selMask=[1, 32]
- 输出：dstLocal=[1, 256]
- 数据类型：输入输出数据类型float，selMask数据类型uint8
- 实现：
    ```cpp
    AscendC::Select(dstLocal, selMask, src0Local, static_cast<float>(0), cmpMode, count); // 标量在后
    AscendC::Select(dstLocal, selMask, static_cast<float>(0), src0Local, cmpMode, count); // 标量在前
    ```
- 参数：cmpMode=AscendC::SELMODE::VSEL_TENSOR_SCALAR_MODE，根据selMask在src0和标量0之间选择元素，count=256表示处理的数据个数

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
  SCENARIO_NUM=1  # 设置场景编号
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DNPU_ARCH=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```

  使用CPU调试或NPU仿真模式时，添加 `-DRUN_MODE=cpu` 或 `-DRUN_MODE=sim` 参数即可。
  示例如下：
 	```bash
 	cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # CPU调试模式
 	cmake -DRUN_MODE=sim -DNPU_ARCH=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # NPU仿真模式
 	```
  若需详细了解CPU调试相关内容，请参考[03_cpudebug样例](../../../01_utilities/03_cpudebug)。

- 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
| `NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2/A3 系列、dav-3510 对应 Ascend 950PR/Ascend 950DT |
| `SCENARIO_NUM` | `1`（默认）、`2` | 场景编号<br>1：Select<br>2：Select（灵活标量位置）<br> |

- 执行结果
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
