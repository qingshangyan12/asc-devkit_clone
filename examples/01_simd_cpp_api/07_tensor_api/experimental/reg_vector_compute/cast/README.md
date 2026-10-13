# Cast Tensor API样例

## 概述

本样例组合实验性 Tensor API 的寄存器加载、类型转换和存储能力，完成逐元素数据类型转换。所有场景输入和输出均为 `[512]` 个逻辑元素的一维 Tensor。

样例覆盖浮点类型和浮点类型、整数类型和浮点类型之间的7种转换场景，并演示不同位宽转换时的 `unpack`、`unpack4` 访存模式和 `deinterleave` 数据重排。计算关系如下：

```text
y[i] = convert<To>(x[i])
```

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── CMakeLists.txt          // 编译工程
├── README.md               // 样例说明
├── cast.asc                   // 算子Kernel、Host调用与结果校验
└── scripts
    └── gen_data.py         // 输入数据和真值数据生成脚本
```

## 样例描述

- 样例功能：

  `SCENARIO_NUM` 用于选择编译和执行的类型转换场景。每个场景的输入、输出、布局、舍入模式和饱和模式如下。

  | SCENARIO_NUM | 输入类型 | 输出类型 | Layout | Round | Sat | 转换说明 |
  | --- | --- | --- | --- | --- | --- | --- |
  | 1 | `half` | `int32_t` | `zero` | `floor` | `not_sat` | 向下取整后转换为32位有符号整数 |
  | 2 | `float` | `int16_t` | `zero` | `rint` | `sat` | 舍入到最近整数并饱和到 `int16_t` 范围 |
  | 3 | `int8_t` | `int32_t` | `zero` | `rint` | `not_sat` | 8位有符号整数扩展为32位有符号整数 |
  | 4 | `int32_t` | `uint8_t` | `zero` | `rint` | `sat` | 饱和到 `[0, 255]` 后转换为8位无符号整数 |
  | 5 | `bfloat16_t` | `float` | `zero` | `rint` | `not_sat` | `bfloat16_t` 扩展为 `float` |
  | 6 | `float` | `bfloat16_t` | `zero` | `rint` | `not_sat` | `float` 舍入为 `bfloat16_t` |
  | 7 | UB：`int4b_t`，寄存器：`int4x2_t` | `half` | `zero` | `rint` | `not_sat` | packed 有符号 INT4 转为 half，覆盖 `[-8, 7]` |

- 样例规格：

  <table>
    <tr>
      <td align="center">样例类型（OpType）</td>
      <td colspan="4" align="center">cast</td>
    </tr>
    <tr>
      <td rowspan="2" align="center">样例输入</td>
      <td align="center">name</td>
      <td align="center">shape</td>
      <td align="center">data type</td>
      <td align="center">layout</td>
    </tr>
    <tr>
      <td align="center">x</td>
      <td align="center">[512]</td>
      <td align="center">half/float/int8_t/int32_t/bfloat16_t/int4b_t</td>
      <td align="center">一维</td>
    </tr>
    <tr>
      <td rowspan="2" align="center">样例输出</td>
      <td align="center">name</td>
      <td align="center">shape</td>
      <td align="center">data type</td>
      <td align="center">layout</td>
    </tr>
    <tr>
      <td align="center">y</td>
      <td align="center">[512]</td>
      <td align="center">int32_t/int16_t/uint8_t/float/bfloat16_t/half</td>
      <td align="center">一维</td>
    </tr>
    <tr>
      <td align="center">样例Kernel名</td>
      <td colspan="4" align="center">cast</td>
    </tr>
  </table>

  | SCENARIO_NUM | `input/input_x.bin` | 输入大小 | `output/output.bin` / `output/golden.bin` | 输出大小 |
  | --- | --- | --- | --- | --- |
  | 1 | 512个 `half` | 1024 B | 512个 `int32_t` | 2048 B |
  | 2 | 512个 `float` | 2048 B | 512个 `int16_t` | 1024 B |
  | 3 | 512个 `int8_t` | 512 B | 512个 `int32_t` | 2048 B |
  | 4 | 512个 `int32_t` | 2048 B | 512个 `uint8_t` | 512 B |
  | 5 | 512个 `bfloat16_t` | 1024 B | 512个 `float` | 2048 B |
  | 6 | 512个 `float` | 2048 B | 512个 `bfloat16_t` | 1024 B |
  | 7 | 512个 packed INT4（256个字节） | 256 B | 512个 `half` | 1024 B |

  场景7在GM和UB上直接构造 `int4b_t` Tensor，layout 均表示512个逻辑元素。
  普通 `copy(x_ub, x_gm)` 由公共 B4 搬运逻辑处理256字节 packed 输入，无需 `uint8_t` 视图。
  低半字节对应偶数元素，高半字节对应奇数元素。
  `load<unpack4>` 返回 `reg_tensor<int4x2_t>`，每次处理128个逻辑元素，使用逻辑偏移
  `0/128/256/384`（实际字节偏移 `0/64/128/192`），然后 `cast<half>` 并普通 `store`。
  输入覆盖全部256种打包字节，结果按half的16位编码逐项精确比较，无需浮点容差。
  加载结果使用 `auto` 推导寄存器类型，并通过 `static_assert` 验证为 `reg_tensor<int4x2_t>`；
  逻辑偏移到字节地址的换算由公共 Tensor 指针实现完成，样例不再单独换算。
  编译使用 CANN 包内的头文件，不添加本地仓库头文件覆盖路径。

- 样例实现：

  - Kernel关键步骤

    1. 使用 `asc::te::make_tensor`、`asc::te::make_mem_ptr` 和一维 layout 构造GM、UB Tensor；layout 使用 `make_shape(512)` 和 `make_stride(1)`。
    2. 使用 `asc_lock/asc_unlock` 包围MTE2流水，通过 `asc::te::copy` 将输入从GM搬运到UB。
    3. 根据源类型和目标类型的较大位宽计算单次寄存器处理元素数，使用 `asc::te::experimental::update_mask` 设置有效元素掩码。
    4. 相同位宽使用普通 `load/store`；扩宽转换使用 `unpack` 或 `unpack4` 加载；窄化转换使用 `deinterleave` 压紧寄存器数据后调用普通 `store`。
    5. 调用 `experimental::cast` 完成寄存器类型转换。
    6. 使用同一个mutex依次锁定Vector和MTE3流水，再将结果从UB搬运回GM。

    核心转换代码如下：

    ```cpp
    auto src_reg = asc::te::experimental::load(src, coord);
    src_reg = src_reg.with_mask(asc::te::experimental::update_mask<WiderT>(remain));

    auto dst_reg = asc::te::experimental::cast<int32_t>(src_reg);
    asc::te::experimental::store(dst, coord, dst_reg);
    ```

    窄化转换使用 layout zero 生成寄存器结果：2倍窄化通过一级 `deinterleave` 提取偶数位置，4倍窄化通过两级 `deinterleave` 提取每四个位置中的第一个元素，再使用普通 `store` 写入连续的目标数据。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/quick_start.md)，配置环境变量。**当前仅支持使用[CANN master](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/quick_start.md)**。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  下面以场景1为例：

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  python3 ../scripts/gen_data.py --scenario_num=${SCENARIO_NUM}
  ./demo
  ```

  生成数据时传入的 `scenario_num` 必须与编译时的 `SCENARIO_NUM` 一致。运行其他场景时，将其修改为1至7之间的对应值。

  使用NPU仿真时，可分别增加以下编译选项：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DSCENARIO_NUM=${SCENARIO_NUM} -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  ```

  场景7的完整仿真命令（在样例根目录执行）：

  ```bash
  cmake -S . -B build_int4_sim -DSCENARIO_NUM=7 -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON -DCMAKE_ASC_RUN_MODE=sim
  cmake --build build_int4_sim -j2
  cd build_int4_sim
  python3 ../scripts/gen_data.py --scenario_num=7
  ./demo
  ```

  构建时优先使用当前仓库的Tensor API头文件，避免意外验证CANN包内的另一个版本。

  切换运行模式、芯片型号或场景时，建议清除 `build` 目录中的CMake缓存后重新配置。

- 编译选项说明

  | 选项 | 说明 |
  | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | 算子执行模式，可选 `npu`、`sim`，默认值为 `npu`。 |
  | `CMAKE_ASC_ARCHITECTURES` | NPU芯片型号，默认值为 `dav-3510`。 |
  | `CANN_ASC_USE_EXPERIMENTAL` | 实验性ASC接口开关，本样例必须设为 `ON`，默认值为 `OFF`。 |
  | `SCENARIO_NUM` | 类型转换场景编号，必选，取值范围为1至7。 |

- 执行结果

  执行成功时输出：

  ```text
  test pass!
  ```
