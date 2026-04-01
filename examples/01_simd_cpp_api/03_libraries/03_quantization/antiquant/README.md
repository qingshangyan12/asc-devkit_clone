# AntiQuant样例

## 概述

本样例在模型量化推理场景下，基于AntiQuant高阶API实现反量化计算，用于将量化后的低精度数据恢复为高精度数据。样例展示了PER_CHANNEL场景（按通道量化）下，将int8_t类型输入数据与offset偏移量相加后，乘以scale缩放因子，转换为half类型输出结果的过程，在950PR上推荐使用AntiQuantize接口。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── antiquant
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── antiquant.asc           // Ascend C算子实现 & 调用样例
```

## 样例描述

- 样例功能：
  按元素做伪量化计算，比如将int8_t数据类型伪量化为half数据类型。

  计算公式如下：
  - PER_CHANNEL场景（按通道量化）
    - 不使能输入转置
      groupSize = src.shape[0] / offset.shape[0]
      dst[i][j] = scale[i / groupSize][j] * (src[i][j] + offset[i / groupSize][j])

    - 使能输入转置
      groupSize = src.shape[1] / offset.shape[1]
      dst[i][j] = scale[i][j / groupSize] * (src[i][j] + offset[i][j / groupSize])

  - PER_TENSOR场景 （按张量量化）
    dst[i][j] = scale * (src[i][j] + offset)

- 样例规格：
  <table border="2" align="center">
  <caption>表1：样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> antiquant </td></tr>

  <tr><td rowspan="5" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[8, 128]</td><td align="center">int8_t</td><td align="center">ND</td></tr>
  <tr><td align="center">offset</td><td align="center">[1, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">scale</td><td align="center">[1, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[8, 128]</td><td align="center">half</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">antiquant_custom</td></tr>
  </table>

- 样例实现：
  本样例中实现的是固定shape为输入src[8, 128]、offset[1, 128]、scale[1, 128]，输出dst[8, 128]的antiquant_custom样例。按元素做反量化计算，将int8_t类型数据转换为half类型数据，更多API详细信息请参考[AscendAntiQuant API文档](../../../../../docs/api/context/AscendAntiQuant.md)，[AntiQuantize API文档](../../../../../docs/api/context/AntiQuantize.md)。
  - Kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用AntiQuant高阶API接口完成伪量化计算，得到最终结果，再搬出到外部存储上。

  - 调用实现
    使用内核调用符<<<>>>调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行算子。
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
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```