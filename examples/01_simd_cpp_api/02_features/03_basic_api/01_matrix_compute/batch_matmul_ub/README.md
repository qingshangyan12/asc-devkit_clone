# Batch Matmul UB样例

## 概述

本样例介绍基于Tensor API验证带batch的L0C -> UB搬出能力。样例中输入A/B在GM上仍按batch存放，但kernel内每轮通过GM指针偏移构造2D Tensor，GM -> L1、L1 -> L0A/L0B和Mmad均按单个矩阵执行；Mmad结果写入L0C后，通过一次`Copy(CopyL0C2UB)`将L0C上的多个batch一次性搬出到UB，随后在mixed kernel的AIV侧将UB物理内存raw copy到GM用于结果校验。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── batch_matmul_ub
│   ├── scripts
│   │   ├── gen_data.py                 // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py            // 真值对比文件
│   ├── CMakeLists.txt                  // 编译工程文件
│   ├── data_utils.h                    // 数据读取写出函数
│   ├── batch_matmul_ub.asc             // Ascend C样例实现与调用样例
│   ├── run.sh                          // 编译运行脚本
│   └── README.md                       // 样例说明文档
```

## 样例描述

- 样例功能：

  本样例用于验证L0C上携带batch信息时，Tensor API的L0C -> UB搬出路径是否能正确完成NZ -> ND/DN转换，并写入UB上的batch空间。

  1. 输入A/B在GM上分别为`[B, M, K]`和`[B, K, N]`。
  2. kernel内每轮用`a + batchIndex * M * K`和`b + batchIndex * K * N`构造2D GM Tensor，避免依赖batch GM2L1能力。
  3. 每轮按2D路径完成GM -> L1、L1 -> L0A/L0B和Mmad。
  4. 每轮Mmad结果写入L0C的第`batchIndex`个batch slice。
  5. 所有batch计算完成后，调用一次`Copy(CopyL0C2UB)`，将L0C上的多个batch一次性搬出到UB。
  6. AIC通过cross-core flag通知AIV；AIV通过raw `Copy(CopyUB2GM)`将UB物理内存搬到GM，便于host侧校验L0C -> UB结果。

- 样例规格：

  本样例包含六个case，其中case1、case2、case3、case5、case6使用`B=3`，case4使用`B=2`。

  | case | 计算类型 | x1 shape | x2 shape | output shape | output dtype | output format | L0C大小 |
  | :--- | :------- | :------- | :------- | :----------- | :----------- | :------------ | :------ |
  | case1 | `half * half -> float` | `[3, 56, 80]` | `[3, 80, 64]` | `[3, 56, 64]` | `float` | `ND` | `3 * 64 * 64 * sizeof(float) = 49152B` |
  | case2 | `half * half -> half` | `[3, 64, 64]` | `[3, 64, 64]` | `[3, 64, 64]` | `half` | `ND` | `3 * 64 * 64 * sizeof(float) = 49152B` |
  | case3 | `half * half -> half` | `[3, 64, 80]` | `[3, 80, 64]` | `[3, 64, 64]` | `half` | `DN` | `3 * 64 * 64 * sizeof(float) = 49152B` |
  | case4 | `half * half -> float` | `[2, 52, 72]` | `[2, 72, 40]` | `[2, 52, 40]` | `float` | `ND` | `2 * 64 * 48 * sizeof(float) = 24576B` |
  | case5 | `half * half -> int8`，scalar quant | `[3, 52, 72]` | `[3, 72, 64]` | `[3, 52, 64]` | `int8` | `ND` | `3 * 64 * 64 * sizeof(float) = 49152B` |
  | case6 | `half * half -> int8`，vector quant | `[3, 52, 72]` | `[3, 72, 64]` | `[3, 52, 64]` | `int8` | `ND` | `3 * 64 * 64 * sizeof(float) = 49152B` |

  case1中M、K、N互不相同，且M不是16对齐规格；case2用于验证`CopyL0C2UB`最终搬出到UB half输出的转换路径；case3用于验证UB输出为DN格式时的`half * half -> half`路径，并按底层要求保持M为16对齐；case4进一步覆盖M、K、N均非完全对齐的小规格；case5验证scalar quant的`QF322B8_PRE`入口，并按底层要求保持b8输出N为32对齐；case6验证vector quant的`VQF322B8_PRE`入口，并按底层要求保持b8输出N为32对齐。六个case的L0C大小均小于256KB。

  `gen_data.py`中将每个batch的B矩阵构造为`eye(K, N)`，因此输出真值等于对应batch的A矩阵前N列。case3的golden按DN物理顺序写盘。case5使用scale为1的scalar quant；case6使用按N维交替为1和2的vector quant tensor，并先从GM搬到L1后再传入`Copy(CopyL0C2UB)`。该数据构造便于检查batch维、M维、N维在UB上的搬出顺序，以及scalar/vector quant入口是否正确生效。

  > **说明：** UB侧DN half输出要求M为16元素对齐，b8输出要求N为32元素对齐。case3、case5和case6已按该限制设置shape，避免触发底层对齐告警。

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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/case1/output.bin output/case1/golden.bin float32   # 验证case1输出结果
  python3 ../scripts/verify_result.py output/case2/output.bin output/case2/golden.bin float16   # 验证case2输出结果
  python3 ../scripts/verify_result.py output/case3/output.bin output/case3/golden.bin float16   # 验证case3输出结果
  python3 ../scripts/verify_result.py output/case4/output.bin output/case4/golden.bin float32   # 验证case4输出结果
  python3 ../scripts/verify_result.py output/case5/output.bin output/case5/golden.bin int8      # 验证case5输出结果
  python3 ../scripts/verify_result.py output/case6/output.bin output/case6/golden.bin int8      # 验证case6输出结果
  ```

  如需NPU模式，示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | :--- | :----- | :--- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果
  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  test pass!
  test pass!
  test pass!
  test pass!
  test pass!
  ```
