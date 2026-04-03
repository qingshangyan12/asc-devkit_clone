# ElementWiseCompoundCompute样例

## 概述

本样例展示了复合计算类接口的使用方法。复合计算接口将多个计算操作融合在一条指令中完成，相比分开调用多个基础接口，可有效减少指令数量、降低中间存储开销、提升计算效率。接口资料参考[CastDequant](../../../../../../docs/api/context/CastDequant.md)/[AddRelu](../../../../../../docs/api/context/AddRelu.md)/[Axpy](../../../../../../docs/api/context/Axpy.md)。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── element_wise_compound_compute
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── element_wise_compound_compute.asc    // Ascend C样例实现 & 调用样例
```

## 样例描述

- 场景说明：
  <table border="2">
  <caption>表1：场景编号与接口对照表</caption>
  <tr>
    <th align="left">场景编号(SCENARIO)</th>
    <th align="left">接口名称</th>
    <th align="left">功能说明</th>
    <th align="left">计算公式</th>
  </tr>
  <tr>
    <td align="left">1</td>
    <td align="left">CastDequant</td>
    <td align="left">反量化与类型转换融合</td>
    <td align="left">dst = (src * scale) + offset</td>
  </tr>
  <tr>
    <td align="left">2</td>
    <td align="left">AddRelu</td>
    <td align="left">向量加法与ReLU激活融合</td>
    <td align="left">dst = max(src0 + src1, 0)</td>
  </tr>
  <tr>
    <td align="left">3</td>
    <td align="left">Axpy</td>
    <td align="left">标量乘法与向量加法融合</td>
    <td align="left">dst = dst + src * scalar</td>
  </tr>
  </table>

- 样例规格：
  <table border="2">
  <caption>表2：样例输入输出规格</caption>
  <tr>
    <th align="left">类型</th>
    <th align="left">name</th>
    <th align="left">shape</th>
    <th align="left">data type</th>
    <th align="left">format</th>
  </tr>
  <tr>
    <td rowspan="2" align="left">样例输入</td>
    <td align="left">input0</td>
    <td align="left">[1, 512]</td>
    <td align="left">half/int16</td>
    <td align="left">ND</td>
  </tr>
  <tr>
    <td align="left">input1</td>
    <td align="left">[1, 512]</td>
    <td align="left">half/int16</td>
    <td align="left">ND</td>
  </tr>
  <tr>
    <td align="left">样例输出</td>
    <td align="left">output</td>
    <td align="left">[1, 512]</td>
    <td align="left">half/uint8</td>
    <td align="left">ND</td>
  </tr>
  <tr>
    <td align="left">核函数名</td>
    <td colspan="4" align="left">element_wise_compound_compute_custom</td>
  </tr>
  </table>

- 样例实现：  
  输入数据从Global Memory搬运至LocalTensor，根据SCENARIO编译参数执行对应的复合计算，结果再搬出到Global Memory。实现流程包含CopyIn、Compute、CopyOut三个任务，使用内核调用符`<<<>>>`调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  SCENARIO=2
  mkdir -p build && cd build
  cmake .. -DSCENARIO_NUM=$SCENARIO
  make -j
  python3 ../scripts/gen_data.py -scenario=$SCENARIO
  ./demo
  python3 ../scripts/verify_result.py -scenario=$SCENARIO output/output.bin output/golden.bin
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```