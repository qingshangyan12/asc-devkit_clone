# 基于Tensor_API实现的Mmad随路量化样例

## 概述

本样例基于Tensor_API编程方式实现动态Shape矩阵乘法和随路量化计算。矩阵A不转置，矩阵B转置存储，A/B矩阵数据类型为int8，Mmad累加类型为int32，输出矩阵C数据类型为half。量化系数quant以uint64_t格式输入，在Fixpipe阶段对L0C中的int32累加结果进行缩放并写回GM。Shape参数（M/N/K及单核切分参数）通过MatmulTiling结构体在运行时动态传入，baseM、baseN、baseK和stepM、stepN、stepK作为编译期模板参数静态配置。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── mmad_tensor_api
│   ├── scripts
│   │   ├── gen_data.py                 // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py            // 真值对比文件
│   ├── CMakeLists.txt                  // 编译工程文件
│   ├── data_utils.h                    // 数据读取写出函数
│   ├── mmad_tensor_api.asc             // Ascend C样例实现与调用样例
│   └── README.md                       // 样例说明文档
```

## 样例描述

- 样例功能：

  本样例实现了多核动态Shape Matmul和随路量化功能。

  1. 动态Matmul功能

     样例在Kernel中通过Tensor_API构造GM、L1、L0A、L0B、L0C上的Tensor对象，使用Copy接口完成GM到L1、L1到L0的分块搬运，并通过Mmad接口完成矩阵乘法。矩阵A按ND布局读取，矩阵B按DN布局读取（输入文件中以转置后的[N, K]保存）。Shape参数（M、N、K、singleCoreM、singleCoreN、singleCoreK）通过MatmulTiling结构体在运行时从GM拷贝到Kernel局部变量中，baseM、baseN、baseK和stepM、stepN、stepK作为编译期模板参数决定L1/L0 Buffer分配大小和分块步进。

  2. 随路量化功能

     样例在L0C结果写回GM前，将量化系数quant从GM搬运到L1，并在CopyL0C2GM阶段传入量化Tensor，对Mmad得到的int32累加结果进行缩放，最终输出half类型矩阵C。数据生成脚本中，quant由float32缩放系数编码为uint64_t格式，并写入`input/quant.bin`。

  3. SCENARIO_NUM编译宏

     `CMakeLists.txt`中通过`SCENARIO_NUM`编译宏控制Mmad/Fixpipe参数设置，默认值为1，可在配置CMake时通过`-DSCENARIO_NUM=<num>`指定。
     unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，使能该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于在L0C Buffer累加的场景。
     本样例中，执行Mmad指令时，沿着K轴进行多次迭代循环。每次迭代中Mmad计算时，unitFlag的取值说明如表1所示：
     <table border="2" align="center">
     <caption>表1：unitFlag取值说明</caption>
       <tr>
         <td>unitFlag值</td>
         <td>取值说明</td>
         <td>样例实现</td>
       </tr>
       <tr>
         <td>0</td>
         <td>保留值</td>
         <td>SCENARIO_NUM为0时，使用默认Mmad参数，CopyL0C2GM阶段使用FixpipeParams默认配置</td>
       </tr>
       <tr>
         <td>2</td>
         <td>使能unitFlag，硬件执行完指令之后，不会关闭unitFlag功能</td>
         <td>SCENARIO_NUM为1时，非最后一次Mmad的unitFlag设为2，保证后续Mmad可以写入L0C</td>
       </tr>
       <tr>
         <td>3</td>
         <td>使能unitFlag，硬件执行完指令之后，会将unitFlag功能关闭</td>
         <td>SCENARIO_NUM为1时，最后一次Mmad和Fixpipe的unitFlag设为3，保证Fixpipe可以读取L0C</td>
       </tr>
     </table>

  4. MatmulTiling结构体定义

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
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul with Quant</td></tr>
  <tr><td rowspan="5" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x (矩阵A)</td><td align="center">[M, K]</td><td align="center">int8_t</td><td align="center">ND</td></tr>
  <tr><td align="center">y (矩阵B，转置存储)</td><td align="center">[N, K]</td><td align="center">int8_t</td><td align="center">DN</td></tr>
  <tr><td align="center">quant</td><td align="center">[N]</td><td align="center">uint64_t</td><td align="center">ND</td></tr>
  <tr><td align="center">tiling</td><td align="center">MatmulTiling结构体</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z (矩阵C)</td><td align="center">[M, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">dynamic_matmul_with_quant</td></tr>
  </table>

  编译期模板参数默认值：

  | 参数    | 默认值 | 说明                    |
  | ------- | ------ | ----------------------- |
  | BASE_M  | 256    | M维度基础分块大小       |
  | BASE_N  | 256    | N维度基础分块大小       |
  | BASE_K  | 64     | K维度基础分块大小       |
  | STEP_M  | 1      | M维度L1缓存步进         |
  | STEP_N  | 1      | N维度L1缓存步进         |
  | STEP_K  | 4      | K维度L1缓存步进         |

  运行时动态参数默认值：

  | 参数         | 默认值 | 说明                       |
  | ------------ | ------ | -------------------------- |
  | m            | 1024   | 矩阵A的行数                |
  | n            | 1024   | 矩阵B的列数                |
  | k            | 256    | 矩阵A的列数/矩阵B的行数    |
  | singleCoreM  | 256    | 单核M维度大小              |
  | singleCoreN  | 128    | 单核N维度大小              |
  | singleCoreK  | 256    | 单核K维度大小              |
  | NUM_BLOCKS   | 32     | Kernel启动的Block数量      |

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

- 软连接配置

  由于当前tensor_api样例只用于功能展示，相关源码未随标准CANN软件包发布。

  如需编译运行该样例，需要先将tensor_api相关源码通过软连接挂载到CANN安装路径下。

  ```bash
  cd ${install_path}/cann/x86-64-linux/asc/impl
  ln -s ${code_path}/impl/tensor_api  # ${code_path}表示本地代码下载路径
  cd ../include
  ln -s ${code_path}/include/tensor_api
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake ..
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
