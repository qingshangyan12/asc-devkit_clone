# HF32Mode样例

## 概述

本样例基于 `SetHF32Mode` 和 `SetHF32TransMode` 接口验证 CTRL 寄存器的 HF32 模式设置功能，通过 `GetCtrlSpr` 接口读取配置值进行验证。

CTRL寄存器bit位置：
- **HF32_MODE_BIT**: CTRL[46] - HF32模式使能位
- **HF32_TRANS_MODE_BIT**: CTRL[47] - HF32舍入模式位

HF32Mode 枚举：
- `HF32Mode::ENABLE`: CTRL[46]=1，启用HF32舍入模式
- `HF32Mode::DISABLE`: CTRL[46]=0，禁用HF32舍入模式

HF32TransMode 枚举：
- `HF32TransMode::NEAREST_ZERO`: CTRL[47]=1，向零舍入
- `HF32TransMode::NEAREST_EVEN`: CTRL[47]=0，向偶数舍入

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
├── hf32_mode
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── hf32_mode.asc           // Ascend C样例实现 & 调用样例
│   └── README.md               // 样例说明文档
```

## 样例描述

- 样例功能：  
  验证 CTRL 寄存器的 HF32 模式设置功能：
  1. SetHF32Mode：设置HF32模式使能位（CTRL[46]）
  2. SetHF32TransMode：设置HF32舍入模式位（CTRL[47]）
  3. GetCtrlSpr：读取CTRL寄存器bit值
  4. Duplicate：将结果存入 ctrlLocal（每次填充4个元素，保证32字节对齐）
  5. DataCopy：输出到 dstCtrlGlobal

- 样例规格：
  <table>
  <caption>表1：样例规格说明</caption>
  <tr><td rowspan="1" align="center">核数</td><td colspan="4" align="center">1</td></tr>
  <tr><td rowspan="1" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[1, 256]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output_ctrl</td><td align="center">[1, 16]</td><td align="center">int64_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">kernel_hf32_mode</td></tr>  
  </table>

- 输出说明：
  - output_ctrl.bin：寄存器值验证输出，16个 int64_t 值
    - [0-3]: DISABLE + NEAREST_EVEN -> 输出0 (CTRL[46]=0, CTRL[47]=0)
    - [4-7]: ENABLE + NEAREST_EVEN -> 输出1 (CTRL[46]=1, CTRL[47]=0)
    - [8-11]: ENABLE + NEAREST_ZERO -> 输出2 (CTRL[46]=1, CTRL[47]=1)
    - [12-15]: DISABLE + NEAREST_ZERO -> 输出3 (CTRL[46]=0, CTRL[47]=1)

- 验证逻辑：
  - SetHF32Mode和SetHF32TransMode设置CTRL寄存器bit位
  - GetCtrlSpr读取对应bit值
  - 对比读取值与预期值是否一致（4种组合）

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                 # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py                                       # 生成测试输入数据和golden数据
  ./demo                                                               # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py                                  # 验证输出结果是否正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

  > **注意：** 本样例仅在 Ascend 950PR/Ascend 950DT 上支持。

- 执行结果

  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```