# Matmul算子性能优化场景直调样例
## 概述
本样例介绍Matmul API实现三种性能优化特性（纯Cube模式、MDL模板、UnitFlag）的单算子。
## 支持的AI处理器
- Ascend 910C
- Ascend 910B
## 目录结构介绍
```
├── matmul_perf
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── matmul.asc              // Ascend C算子实现 & 调用样例
```
## 算子描述
- 算子功能： 
  - 纯Cube模式：在只有矩阵计算，没有矢量计算的场景下，通过使能纯Cube模式，可以优化Matmul计算中的消息通信性能开销，提升算子性能。
  - MDL模板：在MTE2循环搬运次数多的大Shape场景下，使能MDL模板后，可以实现MTE2从Global Memory一次性搬入多个基本块到A1/B1，提升带宽利用率，减少MTE2的搬运次数，提升算子性能。
  - UnitFlag：在算子的CUBE计算流水和FIXPIPE数据搬出流水串行且未被其他流水掩盖时，通过使能UnitFlag功能，可以实现CUBE计算流水和FIXPIPE数据搬出流水之间的流水并行，提升算子性能。

  以上三个特性的编码是相互独立的，本样例支持特性逐个叠加。具体执行方式可参考下述“样例执行”。

- 算子规格： 

  在核函数直调样例中，算子实现的shape为：M = 128, N = 30720, K = 64。
  <table>性能优化特性
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Matmul</td></tr>
  </tr>
  <tr><td rowspan="4" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">M * K</td><td align="center">float16</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">K * N</td><td align="center">float16</td><td align="center">ND</td></tr>
  <tr><td align="center">bias</td><td align="center">-</td><td align="center">-</td><td align="center">-</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">c</td><td align="center">M * N</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">matmul_custom</td></tr>
  </table>
- 算子实现： 
  - kernel实现  
    - 默认实现，使用默认的NORM模板CFG_NORM创建Matmul对象。
      ```
      #include "lib/matmul_intf.h"

      using A_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>;
      using B_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>;
      using C_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>;
      using BIAS_TYPE =  AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>;

      AscendC::Matmul<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CFG_NORM> matmulObj;
      ```
    - 使能纯Cube模式实现，在定义Matmul对象的代码中，设置ASCEND_CUBE_ONLY宏，且必须在#include "lib/matmul_intf.h"之前设置。
      ```
      #define ASCNEND_CUBE_ONLY // 设置ASCEND_CUBE_ONLY宏
      #include "lib/matmul_intf.h"

      using A_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>;
      using B_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>;
      using C_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>;
      using BIAS_TYPE =  AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>;

      AscendC::Matmul<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CFG_NORM> matmulObj;
      ```
    - 使能MDL模板实现，使用默认的MDL模板CFG_MDL创建Matmul对象。
      ```
      #define ASCNEND_CUBE_ONLY // 设置ASCEND_CUBE_ONLY宏
      #include "lib/matmul_intf.h"

      using A_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>;
      using B_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>;
      using C_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>;
      using BIAS_TYPE =  AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>;

      AscendC::Matmul<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CFG_MDL> matmulObj; // 使用CFG_MDL创建Matmul对象
      ```
    - 使能UnitFlag功能实现，自定义MatmulConfig参数，将其中的enUnitFlag参数设置为true，使能UnitFlag功能。
      ```
      #define ASCNEND_CUBE_ONLY // 设置ASCEND_CUBE_ONLY宏
      #include "lib/matmul_intf.h"

      __aicore__ inline constexpr MatmulConfig GetUnitFlagCfg()
      {
          auto mmCfg = CFG_MDL;
          mmCfg.enUnitFlag = true; // 设置enUnitFlag参数为true
          return mmCfg;
      }
      constexpr static MatmulConfig CFG_MDL_UNITFLAG = GetUnitFlagCfg();

      using A_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>;
      using B_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>;
      using C_TYPE = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>;
      using BIAS_TYPE =  AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>;

      // 使用自定义的MatmulConfig参数CFG_MDL_UNITFLAG创建Matmul对象
      AscendC::Matmul<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CFG_MDL_UNITFLAG> matmulObj;
      ```

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/ascend-toolkit/set_env.sh
    ```
  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/ascend-toolkit/set_env.sh
    ```
  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/ascend-toolkit/set_env.sh
    ```

- 样例执行  
  ```bash
  # -DPERF_MODE=0：默认实现。使能MIX模式 + NORM模板（未使能UnitFlag功能）；
  # -DPERF_MODE=1：使能纯Cube模式；
  # -DPERF_MODE=2：使能纯Cube模式 + MDL模板；
  # -DPERF_MODE=3：使能纯Cube模式 + MDL模板 + UnitFlag功能；
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DPERF_MODE=3;make -j;  # 编译工程，以使能纯Cube模式 + MDL模板 + UnitFlag功能为例
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass
  ```