# Fill样例

## 概述

本样例介绍了调用Fill实现将Global Memory上的数据初始化为0。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── fill
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── fill.asc                // Ascend C样例实现 & 调用样例
```

## 样例描述
- 样例功能：  
  用于将输出Global Memory上的数据初始化为0。Add的结果放入初始化为0后的Global Memory上。

- 样例规格：  
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">fill</td></tr>

  <tr><td rowspan="4" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input_x</td><td align="center">[1,256]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">input_y</td><td align="center">[1,256]</td><td align="center">float</td><td align="center">ND</td></tr>


  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">output_z</td><td align="center">[1,256]</td><td align="center">float</td><td align="center">ND</td></tr>


  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">fill_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape为输入input_x[1,256]，input_y[1,256]，输出output_z[1,256]的fill样例，所用api详细介绍请参考[Fill](../../../../../docs/api/context/Fill.md)。

  - Kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用Fill高阶API接口完成初始化为0的计算，使用Add得到最终结果，再搬出到外部存储上。

  - Tiling实现

    本样例中，无需tiling实现。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行样例。
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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```