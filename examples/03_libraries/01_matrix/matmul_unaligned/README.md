# Matmul算子多核非对齐切分场景直调样例
## 概述
本样例介绍Matmul高阶API实现多核非对齐切分的单算子。多核非对齐的切分为M，N，K无法整除singleCoreM, singleCoreN, singleCoreK时，需要在不改变原有Tiling的情况下，在Kernel侧调用SetTail接口重新设置本次计算的singleCoreM/singleCoreN/singleCoreK。
## 支持的AI处理器
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
## 目录结构介绍
```
├── matmul_splitk
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── matmul.asc              // Ascend C算子实现 & 调用样例
```
## 算子描述
- 算子功能： 

  本样例中实现的是[M, K, N]为[1000, 500, 700]的Matmul算子。
  Matmul算子的数学表达式为：
  $$
  C = A * B + Bias
  $$
  其中A的形状为[1000, 500], B的形状为[500, 700], C的形状为[128, 128], Bias的形状为[1, 700]。

- 算子规格： 

  在核函数直调样例中，算子实现支持的shape为：M = 1000, N = 700, K = 500
  <table>
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

  - Kernel实现
    - 关键步骤：
      - 进行Matmul计算前，先计算tailM、tailN、tailK，当tailM < singleCoreM || tailN < singleCoreN || tailK < singleCoreK时处理尾块，调用SetTail接口进行设置尾块大小。
        ```
        auto temp0 = AscendC::Ceil(tiling.M, tiling.singleCoreM);
        auto temp1 = AscendC::Ceil(tiling.N, tiling.singleCoreN);
        auto temp2 = AscendC::Ceil(tiling.Ka, tiling.singleCoreK);

        auto divideKCoreNum = tiling.usedCoreNum / temp2;
        auto mCoreIndex = (blockIdx % divideKCoreNum) % temp0;
        auto nCoreIndex = (blockIdx % divideKCoreNum) / temp0;
        auto subKIndex = blockIdx / divideKCoreNum;

        uint32_t gmUseM = tiling.M - mCoreIndex * tiling.singleCoreM;
        uint32_t tailM = gmUseM < tiling.singleCoreM ? gmUseM : tiling.singleCoreM;
        uint32_t gmUseN = tiling.N - nCoreIndex * tiling.singleCoreN;
        uint32_t tailN = gmUseN < tiling.singleCoreN ? gmUseN : tiling.singleCoreN;
        uint32_t gmUseK = tiling.Ka - subKIndex * tiling.singleCoreK;
        uint32_t tailK = gmUseK < tiling.singleCoreK ? gmUseK : tiling.singleCoreK;

        if (tailM < tiling.singleCoreM || tailN < tiling.singleCoreN || tailK < tiling.singleCoreK) {
            matmulObj.setTail(tailM, tailN, tailK);
        }
        matmulObj.IterateAll(cGlobal);
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
  mkdir -p build && cd build;    # 创建并进入build目录
  cmake ..;make -j;    # 编译工程
  python3 ../scripts/gen_data.py    # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin    # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass
  ```