# Mask样例

## 概述

本样例演示了基于Adds基础API实现mask的多个计算场景。样例覆盖了6个计算场景，涵盖接口传参与外部API配置mask场景，支持不同计算模式与数据切分策略。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── mask
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── mask.asc                // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：  
  矢量内每个元素与标量求和，计算公式如下：
  $$dst_i = src_i + scalar$$

  scenarioNum 为0-5，对应的场景：  
  <table>
    <tr>
        <td>scenarioNum</td>
        <td>输入数据类型</td>
        <td>输出数据类型</td>
        <td>描述</td>
        <td>/</td>
    </tr>
    <tr>
        <td>0</td>
        <td>half</td>
        <td>half</td>
        <td>接口传参 + Normal模式 + 高维切分 + 逐 bit 计算</td>
        <td>/</td>
    </tr>
    <tr>
        <td>1</td>
        <td>half</td>
        <td>half</td>
        <td>接口传参 + Normal模式 + 高维切分 + 连续计算 </td>
        <td>mask <= 128 （half 类型对应 128 个元素），mask > 128 按照一次 repeat 计算。
        repeatTime > 1 时，所有 repeat 中都是前 mask 参与计算</td>
    </tr>
    <tr>
        <td>2</td>
        <td>half</td>
        <td>half</td>
        <td>接口传参 + Counter模式 + 前 n 个元素计算</td>
        <td>/</td>
    </tr>
    <tr>
        <td>3</td>
        <td>half</td>
        <td>half</td>
        <td>外部API设置mask + Normal模式 + 高维切分 + 逐 bit 计算</td>
        <td>/</td>
    </tr>
    <tr>
        <td>4</td>
        <td>half</td>
        <td>half</td>
        <td>外部API设置mask + Normal模式 + 高维切分 + 连续计算</td>
        <td>mask <= 128 （half 类型对应 128 个元素），mask > 128 按照一次 repeat 计算。
        repeatTime > 1 时，所有 repeat 中都是前 mask 参与计算</td>
    </tr>
    <tr>
        <td>5</td>
        <td>half</td>
        <td>half</td>
        <td>外部API设置mask + Counter模式 + 前 n 个元素计算</td>
        <td>/</td>
    </tr>
</table>


- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center"> adds </td></tr>

  <tr><td rowspan="3" align="center">算子输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">512</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">算子输出</td></tr>
  <tr><td align="center">dst</td><td align="center">512</td><td align="center">half</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mask_custom</td></tr>
  </table>

- 算子实现：  
  本样例中实现的是固定shape为输入src[512]，输出dst[512]的mask_custom算子。

  - Kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用Adds基础API接口完成计算，得到最终结果，再搬出到外部存储上。

    mask_custom算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor srcGm存储在srcLocal中，输入参数mode用于判断幂运算的类型，Compute任务负责对srcLocal执行Adds计算，按照 scenarioNum 的值设置不同场景实现计算，计算结果存储在dstLocal中，CopyOut任务负责将输出数据从dstLocal搬运至Global Memory上的输出Tensor dstGm。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行算子。
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
  SCENARIO=0                                                    # 场景选择（可修改SCENARIO值切换场景，0<=SCENARIO<=5）
  mkdir -p build && cd build;                                   # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO;make -j;                    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO         # 生成测试输入数据
  ./demo                                                        # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```