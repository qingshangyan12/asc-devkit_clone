# AntiQuantize样例

## 概述

本样例在模型量化推理场景下，基于AntiQuantize高阶API实现反量化计算，用于将量化后的低精度数据恢复为高精度数据。样例展示了PER_CHANNEL场景（按通道量化）下，将int8_t类型输入数据与offset偏移量相加后，乘以scale缩放因子，转换为half类型输出结果的过程。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── antiquantize_example
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   └── antiquantize_example.asc // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  按元素做伪量化计算，比如将int8_t数据类型伪量化为half数据类型。

  计算公式如下：
  - PER_CHANNEL场景（按通道量化）
    dst[i][j] = scale[0][j] * (src[i][j] + offset[0][j])

- 样例规格：  
  <table border="2" align="center">
  <caption>表1：样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> antiquantize </td></tr>

  <tr><td rowspan="5" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[8, 128]</td><td align="center">int8_t</td><td align="center">ND</td></tr>
  <tr><td align="center">offset</td><td align="center">[1, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">scale</td><td align="center">[1, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[8, 128]</td><td align="center">half</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">antiquantize_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape为输入src[8, 128]、offset[8, 1]、scale[8, 1]，输出dst[8, 128]的antiquantize_custom样例。本样例为PER_TOKEN场景（按token量化），将int8_t数据类型伪量化为half数据类型。

  - Kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用AntiQuantize高阶API接口完成antiquantize计算，得到最终结果，再搬出到外部存储上。

    AntiQuantizeCustom样例的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor srcGm、offsetGm、scaleGm存储在srcLocal、offsetLocal、scaleGm中，Compute任务负责对srcLocal、offsetLocal、scaleGm执行antiquantize计算，计算结果存储在dstLocal中，CopyOut任务负责将输出数据从dstLocal搬运至Global Memory上的输出Tensor dstGm。

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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./antiquantize_demo               # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 与AscendAntiQuant接口的区别

AntiQuantize接口与AscendAntiQuant接口功能类似，但AntiQuantize接口在不同量化场景下的形式更统一，因此推荐使用本接口。

主要区别：
1. AntiQuantize只支持Ascend 950PR/Ascend 950DT产品
2. AntiQuantize的配置参数更加统一，使用AntiQuantizeConfig结构体统一配置
3. AntiQuantize支持PER_TENSOR、PER_CHANNEL、PER_TOKEN、PER_GROUP四种量化策略
4. AntiQuantize的参数结构更简洁，使用AntiQuantizeParams统一配置形状参数