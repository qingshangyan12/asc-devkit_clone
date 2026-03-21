# Matmul算子直调样例
## 概述
本样例介绍调用Matmul API实现matmul单算子。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
- Atlas 推理系列产品AI Core

## 目录结构介绍
```
├── matmul
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── matmul.asc              // Ascend C算子实现 & 调用样例
```
## 算子描述
- 算子功能： 

  Matmul算子调用Matmul API对输入的A B矩阵做矩阵乘和加bias偏置的计算。

- 算子规格： 

  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="5" align="center">MatmulCustom</td></tr>
  </tr>
  <tr><td rowspan="4" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
  <tr><td align="center">a</td><td align="center">(128, 1024)</td><td align="center">float16</td><td align="center">ND</td><td align="center">false</td></tr>
  <tr><td align="center">b</td><td align="center">(1024, 2048)</td><td align="center">float16</td><td align="center">ND</td><td align="center">false</td></tr>
  <tr><td align="center">bias</td><td align="center">(2048, )</td><td align="center">float</td><td align="center">ND</td><td align="center">-</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">c</td><td align="center">(128, 2048)</td><td align="center">float</td><td align="center">ND</td><td align="center">-</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">matmul_custom</td></tr>
  </table>

- 算子实现： 
  - Kernel实现
    - 计算逻辑是：Ascend C提供一组Matmul高阶API，方便用户快速实现Matmul矩阵乘法的运算操作。MatMul的计算公式为：C = A * B + Bias。
      - A、B为源操作数，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]。
      - C为目的操作数，存放矩阵乘结果的矩阵，形状为[M, N]。
      - Bias为矩阵乘偏置，形状为[1, N]。对A*B结果矩阵的每一行都采用该bias进行偏置。
    - 实现MatMul矩阵乘运算的具体步骤如下：
      - 创建Matmul对象。
      - 初始化操作。
      - 设置左矩阵A、右矩阵B、Bias。
      - 完成矩阵乘操作。
      - 结束矩阵乘操作。

  - tiling实现
      - Ascend C提供一组Matmul Tiling API，方便用户获取Matmul kernel计算时所需的Tiling参数。只需要传入A/B/C矩阵等信息，调用API接口，即可获取到TCubeTiling结构体中的相关参数。
      - 获取Tiling参数的流程如下：
        - 创建一个Tiling对象。
        - 设置A、B、C、Bias的参数类型信息；M、N、Ka、Kb形状信息等。
        - 调用GetTiling接口，获取Tiling信息。

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
  mkdir -p build && cd build;    # 创建并进入build目录
  cmake ..;make -j;    # 编译工程
  python3 ../scripts/gen_data.py    # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin    # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```、


  我现在需要你把当前目录下的：matmul_singleCore.asc、matmul_singleCore_tiling_unNorm.asc、matmul_singleCore_tiling_norm.asc、matmul_multiCore.asc、matmul_multiCore_doubleBuffer.asc、matmul_multiCore_norm.asc、matmul_multiCore_mdl.asc、matmul_multiCore_mdl_ConstantsTiling.asc、matmul_multiCore_mdl_ConstantsTiling_Uintflag.asc、matmul_multiCore_mdl_ConstantsTiling_Uintflag_L2Cache.asc这十个文件，类似于https://github.com/NVIDIA/cuda-samples/blob/master/Samples/6_Performance/transpose/transpose.cu样例的switch——case方式，将这十个文件代表的case依次写入同一个文件中，可以命名为matmul_allcase.asc文件，通过不同case名称选择不同的样例，接下来我会告诉你需要注意的这十个文件中的不同点：
  1、matmul_singleCore.asc：你需要注意该文件tilingApi.SetFixSplit(64, 64, 64);以及会    tilingData.set_baseK(64);
    tilingData.set_depthA1(1);
    tilingData.set_depthB1(1);
    tilingData.set_stepM(1);
    tilingData.set_stepN(1);
    tilingData.set_stepKa(1);
    tilingData.set_stepKb(1);
    tilingData.set_dbL0A(1);
    tilingData.set_dbL0B(1);
    tilingData.set_dbL0C(1);，还需要注意是单核，即uint32_t numBlocks = 1;
  2、matmul_singleCore_tiling_unNorm.asc这个文件相比于前一个文件的修改点为：tilingApi.SetFixSplit(128, 256, 64);以及删去了tilingData.set_baseK(64);其余地方我理解应该是几乎一样的；
  3、matmul_singleCore_tiling_norm.asc这个文件相比于前一个文件，修改的点就在于删去了多余的tiling设置，由norm模板自动推到，即删去了    
    tilingData.set_depthA1(1);
    tilingData.set_depthB1(1);
    tilingData.set_stepM(1);
    tilingData.set_stepN(1);
    tilingData.set_stepKa(1);
    tilingData.set_stepKb(1);
  4、matmul_multiCore.asc，这个文件可以对比matmul_singleCore_tiling_unNorm.asc这个文件，相比于这个文件，主要是将核数改成了24，即uint32_t numBlocks = 24;，同时引入了singleMNK的概念，在设置tiling的时候有：matmul_tiling::MultiCoreMatmulTiling tilingApi(*ascendcPlatform);以及tilingApi.SetSingleShape(singleM, singleN, K);
  5、matmul_multiCore_doubleBuffer.asc这个文件相比于matmul_multiCore.asc文件的修改点主要是在    tilingData.set_dbL0A(2);
    tilingData.set_dbL0B(2);
  6、matmul_multiCore_norm.asc，这个文件和matmul_multiCore_doubleBuffer.asc的不同点主要是在于在于删去了多余的tiling设置，由norm模板自动推到，即删去了    
    tilingData.set_depthA1(1);
    tilingData.set_depthB1(1);
    tilingData.set_stepM(1);
    tilingData.set_stepN(1);
    tilingData.set_stepKa(1);
    tilingData.set_stepKb(1);
  7、matmul_multiCore_mdl.asc，这个文件和matmul_multiCore_norm.asc基本是一样的，修改点事在于    AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                    AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                    AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                    AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_MDL>
        matmulObj;这里，由CFG_NORM改成了CFG_MDL；
  8、matmul_multiCore_mdl_ConstantsTiling.asc这个文件和matmul_multiCore_mdl.asc差异就比较大了，主要修改点是将动态tiling变成了常量化tiling，所以改动比较大，你可以主要对比下这两个文件的差异；
  9、matmul_multiCore_mdl_ConstantsTiling_Uintflag.asc这个文件和matmul_multiCore_mdl_ConstantsTiling.asc文件比较接近，区别只是是否使能unitflag，这里就改成了mmCFG.enUnitFlag = true;
  10、最后一个文件是matmul_multiCore_mdl_ConstantsTiling_Uintflag_L2Cache.asc，这个文件和前一个文件matmul_multiCore_mdl_ConstantsTiling_Uintflag.asc差异就在于会对M进行切分，同时进行两次matmul运算
  综上，你需要按照顺序进行整合，因为这十个文件基本上是按照顺序进行递进的