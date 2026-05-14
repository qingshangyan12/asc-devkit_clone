# MXFP4 Matmul多场景样例

## 概述

在MXFP4数据格式下的带有量化系数的矩阵乘法样例。计算公式为：C = (scaleA ⊗ A) * (scaleB ⊗ B)。"⊗"表示广播乘法。本样例可通过编译参数 `SCENARIO_NUM` 选择不同场景，采用动态Tiling策略。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── adv_api_practice
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── matmul_mxfp4.asc        // Ascend C样例实现 & 调用样例
│   └── README.md               // 样例说明文档
```

## 样例描述

- 样例功能：  
  matmul_mxfp4_custom样例调用Matmul API计算时，左量化系数矩阵与左矩阵乘积，右量化系数矩阵与右矩阵乘积，对两个乘积的结果做矩阵乘法。本样例使用动态Tiling策略。

- 样例规格：  
  本样例通过编译参数 `SCENARIO_NUM` 选择场景，场景的shape、转置配置和执行核数如下表所示。其中 `scaleK = ceil(K / 64) * 2`。
  <table>
  <tr><td align="center">SCENARIO_NUM</td><td align="center">M</td><td align="center">K</td><td align="center">N</td><td align="center">scaleK</td><td align="center">isTransA</td><td align="center">isTransB</td><td align="center">执行核数</td></tr>
  <tr><td align="center">1</td><td align="center">256</td><td align="center">256</td><td align="center">256</td><td align="center">8</td><td align="center">false</td><td align="center">true</td><td align="center">1</td></tr>
  <tr><td align="center">2</td><td align="center">40</td><td align="center">70</td><td align="center">80</td><td align="center">4</td><td align="center">false</td><td align="center">true</td><td align="center">1</td></tr>
  <tr><td align="center">3</td><td align="center">1024</td><td align="center">1024</td><td align="center">1024</td><td align="center">32</td><td align="center">false</td><td align="center">true</td><td align="center">8</td></tr>
  </table>

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="5" align="center">matmul_mxfp4_custom</td></tr>
  </tr>
  <tr><td rowspan="5" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
  <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">fp4x2_e1m2_t</td><td align="center">ND</td><td align="center">false</td></tr>
  <tr><td align="center">scaleA</td><td align="center">[M, scaleK]</td><td align="center">fp8_e8m0_t</td><td align="center">ND</td><td align="center">false</td></tr>
  <tr><td align="center">b</td><td align="center">[N, K]</td><td align="center">fp4x2_e1m2_t</td><td align="center">ND</td><td align="center">true</td></tr>
  <tr><td align="center">scaleB</td><td align="center">[N, scaleK]</td><td align="center">fp8_e8m0_t</td><td align="center">ND</td><td align="center">true</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">bfloat16_t</td><td align="center">ND</td><td align="center">-</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">matmul_mxfp4_custom</td></tr>
  </table>

- 样例实现： 
  - Kernel关键步骤
    - 创建Matmul对象：使用MatmulTypeWithScale使能scaleA、scaleB。
      ```cpp
      using aType = AscendC::MatmulTypeWithScale<AscendC::TPosition::GM, AscendC::TPosition::GM, CubeFormat::ND, fp4x2_e1m2_t, false>;
      using bType = AscendC::MatmulTypeWithScale<AscendC::TPosition::GM, AscendC::TPosition::GM, CubeFormat::ND, fp4x2_e1m2_t, false>;
      using cType = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, bfloat16_t>;
      // 定义 matmul 对象时，传入 MatmulWithScalePolicy 表示开启 MxMatmul 模板策略
      AscendC::Matmul<aType, bType, cType, cType, CFG_MDL, 
          AscendC::MatmulCallBackFunc<nullptr, nullptr, nullptr>,
          AscendC::Impl::Detail::MatmulWithScalePolicy> matmulObj;
      ```
    - 设置左矩阵A与左量化系数矩阵scaleA、右矩阵B与右量化系数矩阵scaleB。
      ```cpp
      matmulObj.SetTensorA(aGlobal, IS_TRANS_A);
      matmulObj.SetTensorB(bGlobal, IS_TRANS_B);
      matmulObj.SetTensorScaleA(asGlobal, IS_TRANS_A);
      matmulObj.SetTensorScaleB(bsGlobal, IS_TRANS_B);
      ```

  - Tiling关键步骤
      - 创建一个Tiling对象：使用SetMadType设置Matmul模式，使能MxMatmul场景，使用SetScaleAType设置ScaleA的信息、使用SetScaleBType设置scaleB的信息。
        ```cpp
        tilingApi.SetMadType(matmul_tiling::MatrixMadType::MXMODE);
        tilingApi.SetScaleAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, IS_TRANS_A);
        tilingApi.SetScaleBType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, IS_TRANS_B);
        ```

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

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
  SCENARIO_NUM=1
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式或 `SCENARIO_NUM` 前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|---------|--------|
  | CMAKE_ASC_RUN_MODE | 运行模式 | npu, cpu, sim | npu |
  | CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-3510 | dav-3510 |
  | SCENARIO_NUM | 样例场景编号 | 1, 2, 3 | 1 |

- 执行结果

  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```
