# 基于Tensor_API实现的动态Matmul带Bias样例

## 概述

本样例基于Tensor_API编程方式实现带Bias的动态Shape矩阵乘法运算。其中矩阵A不转置，矩阵B转置存储，A/B矩阵数据类型为half，C矩阵和Bias数据类型为float。Shape参数（M/N/K及单核切分参数）通过MatmulTiling结构体在运行时动态传入，baseM、baseN、baseK和stepM、stepN、stepK作为编译期模板参数静态配置。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 02_dynamic_matmul_with_bias
│   └── scripts
│       ├── gen_data.py                          // 输入数据和真值数据生成脚本文件
│       └── verify_result.py                     // 真值对比文件
│   ├── CMakeLists.txt                           // 编译工程文件
│   ├── data_utils.h                             // 数据读入写出函数
│   ├── dynamic_matmul_with_bias.asc             // Ascend C样例实现 & 调用样例
│   └── README.md                                // 样例说明文档
```

## 样例描述

- 样例功能：
  本样例实现了带Bias的动态Shape多核Matmul功能。

  1、动态Matmul功能

  本样例实现了多核Matmul功能，矩阵A不转置（ND布局），矩阵B转置存储（DN布局）。Shape参数（M、N、K、singleCoreM、singleCoreN、singleCoreK）通过MatmulTiling结构体在运行时动态传入kernel。baseM、baseN、baseK和stepM、stepN、stepK作为编译期模板参数，决定了L0/L1级buffer的分配大小。

  2、Bias功能

  本样例在Matmul计算的首次迭代中通过Mmad接口将Bias向量加到矩阵乘结果上，Bias为float类型的一维向量，长度为N。

  3、MatmulTiling结构体定义

  ```cpp
  struct MatmulTiling {
      int32_t m;            // 矩阵A的行数
      int32_t n;            // 矩阵B的列数
      int32_t k;            // 矩阵A的列数/矩阵B的行数
      int32_t singleCoreM;  // 单核处理的M维度大小
      int32_t singleCoreN;  // 单核处理的N维度大小
      int32_t singleCoreK;  // 单核处理的K维度大小
  };
  ```

- 样例规格：
  在核函数直调样例中，样例默认支持的shape为：M = 1024, N = 1024, K = 256。
  <table border="2" align="center">
  <caption>表1：样例规格表</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul with Bias</td></tr>
  </tr>
  <tr><td rowspan="5" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a (矩阵A)</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">b (矩阵B，转置存储)</td><td align="center">[N, K]</td><td align="center">half</td><td align="center">DN</td></tr>
  <tr><td align="center">bias</td><td align="center">[N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">tiling</td><td align="center">MatmulTiling结构体</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">c (矩阵C)</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">dynamic_matmul_with_bias</td></tr>
  </table>

  编译期模板参数默认值：

  | 参数    | 默认值 | 说明                     |
  | ------- | ------ | ------------------------ |
  | BASE_M  | 256    | M维度基础分块大小        |
  | BASE_N  | 256    | N维度基础分块大小        |
  | BASE_K  | 64     | K维度基础分块大小        |
  | STEP_M  | 1      | M维度L1缓存步进          |
  | STEP_N  | 1      | N维度L1缓存步进          |
  | STEP_K  | 4      | K维度L1缓存步进          |

  运行时动态参数默认值：

  | 参数         | 默认值 | 说明                   |
  | ------------ | ------ | ---------------------- |
  | m            | 1024   | 矩阵A的行数            |
  | n            | 1024   | 矩阵B的列数            |
  | k            | 256    | 矩阵A的列数/矩阵B的行数 |
  | singleCoreM  | 256    | 单核M维度大小           |
  | singleCoreN  | 128    | 单核N维度大小           |
  | singleCoreK  | 256    | 单核K维度大小           |

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

- 软连接配置  
  由于当前tensor_api样例只用于功能展示，相关源码未随标准CANN软件包发布。
  
  如需编译运行该样例，需要先将tensor_api相关源码通过软连接挂在到CANN安装路径下。
  ```bash
  cd ${install_path}/cann/x86-64-linux/asc/impl
  ln -s ${code_path}/impl/tensor_api  # ${code_path}表示本地代码下载路径
  cd ../include
  ln -s ${code_path}/include/tensor_api
  ```

- 样例执行  
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
