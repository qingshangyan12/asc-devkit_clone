# GatherMask 样例

## 概述

本样例介绍了[GatherMask接口](../../../../../../docs/api/context/GatherMask.md)在多场景下的使用方式，包括内置固定模式和用户自定义模式来生成gather mask（数据收集的掩码），并从源操作数中选取元素写入目的操作数。样例支持通过编译参数切换不同场景，便于开发者理解GatherMask接口的使用方法和实现差异。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── gather_mask
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── gather_mask.asc         // Ascend C样例实现 & 调用样例
```

## 场景详细说明
本样例通过编译参数`SCENARIO_NUM` 来切换不同的掩码生成场景：

**场景1：内置固定模式**
- 说明：通过`src1Pattern`选择对应的二进制作为掩码，来获取数据
- 输入：[1, 128]
- 输出：[1, 128]
- 数据类型：uint16
- 实现：
    ```cpp
    AscendC::GatherMask(dstLocal, src0Local, src1Pattern, reduceMode, mask, gatherMaskParams, rsvdCnt);
    ```
- 参数：使用内置固定模式src1Pattern=2进行元素选取，reduceMode=false（Normal模式），mask=0，gatherMaskParams={1, 1, 0, 0}

**场景2：用户自定义模式**
- 说明：通过用户输入的`src1Local`对应的二进制作为掩码，来获取数据
- 输入：[1, 256], [1, 32]
- 输出：[1, 256]
- 数据类型：uint32
- 实现：
    ```cpp
    AscendC::GatherMask (dstLocal, src0Local, src1Local, reduceMode, mask, gatherMaskParams, rsvdCnt);
    ```
- 参数：使用用户提供的Tensor进行元素选取，reduceMode=true（Counter模式），mask=70，gatherMaskParams={1, 2, 4, 0}

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
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DRUN_MODE=npu -DNPU_ARCH=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```
  其中编译参数使用说明如下表所示：
  <table border="2">
  <caption>表1：编译参数说明</caption>
  <tr>
    <th>参数</th>
    <th>说明</th>
    <th>默认值</th>
  </tr>
  <tr>
    <td>SCENARIO_NUM</td>
    <td>场景编号：1（内置固定模式）、2（用户自定义模式）</td>
    <td>1</td>
  </tr>
  </table>

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```