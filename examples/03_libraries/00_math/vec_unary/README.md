# Exp样例

## 概述

本样例是Exp基础API的算子实现。Exp属于单目矢量类计算API，其他诸如Ln、Abs、Reciprocal、Sqrt、Rsqrt、Relu等API都属于该类别，对应算子实现方式可参考本样例。

样例中提供了3类不同函数原型接口调用形式：
- tensor前n个数据连续计算
- tensor高维切分计算-mask连续模式
- tensor高维切分计算-mask逐bit模式  

支持的数据类型：half/float；程序中以scenario_num来区分上述组合的6类场景，具体对应关系如下表所示：

<table>
    <tr>
        <td>scenario_num</td>
        <td>输入/输出数据类型</td>
        <td>计算方式</td>
        <td>描述</td>
    </tr>
    <tr>
        <td>1</td>
        <td rowspan="3">half</td>
        <td>tensor前n个数据连续计算</td>
        <td>Counter模式连续计算</td>
    </tr>
    <tr>
        <td>2</td>
        <td>tensor高维切分计算</td>
        <td>mask连续模式</td>
    </tr>
    <tr>
        <td>3</td>
        <td>tensor高维切分计算</td>
        <td>mask逐bit模式</td>
    </tr>
    <tr>
        <td>4</td>
        <td rowspan="3">float</td>
        <td>tensor前n个数据连续计算</td>
        <td>Counter模式连续计算</td>
    </tr>
    <tr>
        <td>5</td>
        <td>tensor高维切分计算</td>
        <td>mask连续模式</td>
    </tr>
    <tr>
        <td>6</td>
        <td>tensor高维切分计算</td>
        <td>mask逐bit模式</td>
    </tr>
</table>


## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── exp
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── exp.asc                 // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：  
  对输入Tensor按元素取自然指数。

  计算公式如下：
  $$
  dst_i = e^{src_i}
  $$

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center"> exp </td></tr>

  <tr><td rowspan="3" align="center">算子输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">1024</td><td align="center">-</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">算子输出</td></tr>
  <tr><td align="center">dst</td><td align="center">1024</td><td align="center">-</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">exp_custom</td></tr>
  </table>
  data type支持half/float。

- 算子实现：  
  本样例中实现的是固定shape为输入src[1024]，输出dst[1024]的exp_custom算子。

  - Kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用Exp基础API接口完成Exp计算，得到最终结果，再搬出到外部存储上。

    exp_custom算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor srcGM存储在srcLocal中，Compute任务负责对srcLocal执行Exp计算，计算结果存储在dstLocal中，CopyOut任务负责将输出数据从dstLocal搬运至Global Memory上的输出Tensor dstGM。

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
  SCENARIO_NUM=1
  mkdir -p build && cd build; # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # 编译工程
  python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM # 生成测试输入数据
  ./demo # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
