# Quant样例

## 概述

本样例在模型量化推理场景下，基于Quant高阶API实现量化计算，用于将高精度数据转换为低精度数据以减少存储和计算开销。样例展示了将float类型输入数据通过scale缩放和offset偏移，量化为int8_t类型输出的过程，在950PR上推荐使用Quantize接口。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── quant
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── quant.asc        // Ascend C算子实现 & 调用样例
```

## 样例描述

- 样例功能：
  QuantCustom单样例，对输入tensor按元素做量化计算，将half/float数据类型量化为int8_t数据类型。更多API详细信息请参考[AscendQuant](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0818.html)。

- 样例规格：
  <table border="2" align="center">
  <caption>表1：样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> quant </td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">inputGm</td><td align="center">[1024]</td><td align="center">float</td><td align="center">ND</td></tr>


  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">outputGm</td><td align="center">[1024]</td><td align="center">int8_t</td><td align="center">ND</td></tr>


  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">quant_custom</td></tr>
  </table>

- 样例实现：
  本样例实现了QuantCustom样例，其中固定shape输入为inputGm[1024]，量化参数scale=2.0、offset=0.9。本样例为PER_TENSOR场景（按张量量化），将float数据类型量化为int8_t数据类型。

  - Kernel实现

    计算逻辑是：Ascend C提供的矢量。计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用AscendQuant高阶API接口完成量化计算，得到最终结果，再搬出到外部存储上。

  - Tiling实现

    QuantCustom样例的tiling实现流程如下：首先获取AscendQuant接口能完成计算所需最大/最小临时空间大小，根据该范围结合实际的内存使用情况设置合适的空间大小，然后根据输入长度dataLength确定所需tiling参数。

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
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```