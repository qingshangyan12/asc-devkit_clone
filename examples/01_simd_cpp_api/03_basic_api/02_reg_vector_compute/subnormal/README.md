# Subnormal样例

## 概述

本样例演示了在Ascend C Reg矢量计算中，使用DivAlgo::DIFF_COMPENSATION差值补偿算法进行除法运算，验证其0ulp精度特性。通过选取在float32中无法精确表示的normal值除法，展示差值补偿算法输出与IEEE 754正确舍入结果完全一致。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
subnormal
│   ├── scripts/             // 测试脚本目录
│   │   └── gen_data.py      // 生成测试输入和golden数据
│   ├── CMakeLists.txt       // 编译工程文件
│   ├── data_utils.h         // 数据读写工具函数
│   ├── subnormal.asc        // Ascend C算子实现 & 调用样例
│   └── README.md            // 样例说明文档
```

## 样例描述

- 样例功能：

  样例计算float类型数据，使用DivAlgo::DIFF_COMPENSATION差值补偿算法配置DivSpecificMode作为Div操作的精度算法参数，对normal值执行除法运算。golden数据由float64高精度除法结果舍入到float32生成（模拟IEEE 754正确舍入），用于验证0ulp精度。计算公式如下：
  $$z_i = \frac{x_i}{y_i}$$

- 样例规格
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">AIV样例</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center" colspan="2">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[1024]</td><td align="center" colspan="2">float</td></tr>
  <tr><td align="center">y</td><td align="center">[1024]</td><td align="center" colspan="2">float</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[1024]</td><td align="center" colspan="2">float</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">subnormal</td></tr>
  </table>

- 样例实现
    - 实现流程：
        1. 在Host侧分配内存并初始化输入数据
        2. 将数据从Host内存搬运至Device全局内存
        3. 调用核函数，在Vector核上执行计算
        4. 核函数内部：通过DataCopy将输入数据从Global Memory搬入Unified Buffer
        5. 在Unified Buffer上通过asc_vf_call调用VF（Vector Function）函数
        6. VF函数内部使用DivSpecificMode模板参数，执行LoadAlign、Div、StoreAlign操作
        7. 将计算结果从Unified Buffer搬回Global Memory
        8. 在Host端进行精度验证和golden data对比

  - 0ulp精度说明：
      - 差值补偿算法（DIFF_COMPENSATION）：通过计算初步商后，利用余数对结果进行差值补偿修正，消除单指令除法的舍入误差，使输出与IEEE 754正确舍入结果完全一致，最大精度误差为0ulp。
      - 默认算法（INTRINSIC）：使用单指令计算，最大精度误差为1ulp，对于无法精确表示的除法结果可能产生1ulp的舍入偏差。

  - 场景说明：
      - 实现函数：SubnormalDivVF<T>
      - 实现：
        ```cpp
        static constexpr AscendC::Reg::DivSpecificMode mode = {ZEROING, false, DIFF_COMPENSATION};
        AscendC::Reg::Div<T, &mode>(zReg, xReg, yReg, mask);
        ```
      - 说明：使用差值补偿算法对normal值执行除法，最大精度误差为0ulp，输出与golden（IEEE 754正确舍入）完全一致。
      - 应用场景：对除法精度有严格要求的科学计算、数值模拟、高精度计算等场景。

    - 计算数据示例：

      | 序号 | 被除数 | 除数 | float32精确舍入结果（golden） | 说明 |
      |:---:|:---:|:---:|:---:|:---|
      | 1 | 1.0 | 3.0 | 0.33333334（0x3EAAAAAB） | 1/3在float32中无法精确表示，需舍入 |
      | 2 | 1.0 | 7.0 | 0.14285715（0x3E124925） | 1/7在float32中无法精确表示，需舍入 |
      | 3 | 2.0 | 3.0 | 0.6666667（0x3F2AAAAB） | 2/3在float32中无法精确表示，需舍入 |
      | 4 | 1.0 | 10.0 | 0.100000001（0x3DCCCCCD） | 1/10在float32中无法精确表示，需舍入 |
      | 5 | 3.0 | 7.0 | 0.42857143（0x3EDB6DB7） | 3/7在float32中无法精确表示，需舍入 |

      - DIFF_COMPENSATION输出与golden完全一致（0ulp误差）。

  - 约束条件：
      - 输入数据长度必须为GetVecLen()的整数倍
      - 除数不能为0，实际应用中需要增加除数为0的检查
      - 样例目前仅支持float数据类型

## 编译运行

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake ..; make -j;                                                        # 编译工程
  python3 ../scripts/gen_data.py;                                           # 生成测试输入数据
  ./demo                                                                    # 执行样例
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。
- 编译选项说明

  | 选项 | 可选值                   | 说明                                           |
  |:--------------------------|:----------------------|:---------------------------------------------|
  | `CMAKE_ASC_RUN_MODE`      | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真                      |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`            | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```