# Transpose样例

## 概述

本样例介绍矩阵转置Transpose接口在普通转置和增强转置两种场景下的使用方法，适用以下两种转置功能：（1）对16*16的二维矩阵数据块进行转置，（2）[N,C,H,W]与[N,H,W,C]四维矩阵互相转换。

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

  2、增强转置，通过transposeParams指定转置类型，支持16*16的二维矩阵数据块转置，支持[N,C,H,W]与[N,H,W,C]四维矩阵互相转换

  并且样例可通过编译参数SCENARIO来切换不同的场景，参数详见表1。


- 场景说明：
  <table border="2" align="center">
  <caption>表1：编译参数与样例规格对照表</caption>
  <tr>
    <th align="center">SCENARIO</th>
    <th align="center">场景名称</th>
    <th align="center">输入shape</th>
    <th align="center">输出shape</th>
    <th align="center">数据类型</th>
    <th align="center">输入格式</th>
    <th align="center">输出格式</th>
    <th align="center">转置类型</th>
  </tr>
  <tr>
    <td align="center">1</td>
    <td align="center">普通转置</td>
    <td align="center">[16,16]</td>
    <td align="center">[16,16]</td>
    <td align="center">half</td>
    <td align="center">ND</td>
    <td align="center">ND</td>
    <td align="center">/</td>
  </tr>
  <tr>
    <td align="center">2</td>
    <td align="center">增强转置</td>
    <td align="center">[3,3,2,8]</td>
    <td align="center">[3,2,8,3]</td>
    <td align="center">half</td>
    <td align="center">NCHW</td>
    <td align="center">NHWC</td>
    <td align="center">TRANSPOSE_NCHW2NHWC</td>
  </tr>
  </table>

- 数据格式说明：

  卷积神经网络的特征图（Feature Map）通常用四维数组保存，即4D，4D格式解释如下：
    - N：Batch数量，例如图像的数目。
    - H：Height，特征图高度，即垂直高度方向的像素个数。
    - W：Width，特征图宽度，即水平宽度方向的像素个数。
    - C：Channels，特征图通道，例如彩色RGB图像的Channels为3。
    
  由于数据只能线性存储，因此这四个维度有对应的顺序。不同深度学习框架会按照不同的顺序存储特征图数据，比如Caffe，排列顺序为[Batch, Channels, Height, Width]，即NCHW。TensorFlow中，排列顺序为[Batch, Height, Width, Channels]，即NHWC。

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
  cmake .. -DRUN_MODE=npu -DNPU_ARCH=dav-2201 -DSCENARIO_NUM=$SCENARIO;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenario_num=$SCENARIO   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

  **编译参数说明：**
  <table border="2" align="center">
  <caption>表2：编译参数说明</caption>
  <tr>
    <th align="center">参数</th>
    <th align="center">说明</th>
    <th align="center">默认值</th>
    <th align="center">可选值</th>
  </tr>
  <tr>
    <td align="center">RUN_MODE</td>
    <td align="center">运行模式</td>
    <td align="center">npu</td>
    <td align="center">npu, cpu, sim</td>
  </tr>
  <tr>
    <td align="center">NPU_ARCH</td>
    <td align="center">NPU 硬件架构</td>
    <td align="center">dav-2201</td>
    <td align="center">dav-2201, dav-3510</td>
  </tr>
  <tr>
    <td align="center">SCENARIO_NUM</td>
    <td align="center">场景编号</td>
    <td align="center">1</td>
    <td align="center">1, 2</td>
  </tr>
  </table>
