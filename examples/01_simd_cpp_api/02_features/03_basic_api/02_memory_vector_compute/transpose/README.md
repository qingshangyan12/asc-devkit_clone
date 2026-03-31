# Transpose样例

## 概述

本样例介绍矩阵转置Transpose接口在普通转置和增强转置两种场景下的使用方法，适用于对16*16的二维矩阵数据块进行转置，以及[N,C,H,W]与[N,H,W,C]互相转换。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── transpose
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── transpose.asc           // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  Transpose样例实现矩阵转置功能，支持普通转置和增强转置两种场景:

  1、普通转置，支持16*16的二维矩阵数据块进行转置

  2、增强转置，通过transposeParams指定转置类型，支持16*16的二维矩阵数据块转置，以及[N,C,H,W]与[N,H,W,C]互相转换

  并且样例可通过编译参数SCENARIO来切换不同的场景，参数详见表1。


- 场景说明：
  <table border="2" align="center">
  <caption>表1：编译参数与样例规格对照表</caption>
  <tr>
    <th>SCENARIO</th>
    <th>场景名称</th>
    <th>输入shape</th>
    <th>输出shape</th>
    <th>数据类型</th>
    <th>数据格式</th>
    <th>转置类型</th>
  </tr>
  <tr>
    <td>1</td>
    <td>普通转置</td>
    <td>[16,16]</td>
    <td>[16,16]</td>
    <td>half</td>
    <td>ND</td>
    <td>/</td>
  </tr>
  <tr>
    <td>2</td>
    <td>增强转置</td>
    <td>[3,3,2,8]</td>
    <td>[3,2,8,3]</td>
    <td>half</td>
    <td>NCHW->NHWC</td>
    <td>TRANSPOSE_NCHW2NHWC</td>
  </tr>
  </table>

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
  SCENARIO=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenario_num=$SCENARIO   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
