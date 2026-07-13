# 基于Tensor API实现的UB到UB搬运样例

## 概述

本样例基于Tensor API演示**UB到UB搬运通路**（`CopyUB2UB`）。整个流程是纯Vector（AIV）侧的数据搬运，不涉及矩阵乘：

1. GM到UB：通过`CopyGM2UB`将输入数据从GM搬入UB（`srcUB`）。
2. **UB到UB（本样例重点）**：通过`CopyUB2UB`将`srcUB`的数据搬运到另一块UB（`dstUB`）。
3. UB到GM：通过`CopyUB2GM`将`dstUB`的数据搬回GM。

由于三段搬运都是逐元素恒等操作，GM上的输出`out`应与输入`x`完全一致，据此校验`CopyUB2UB`通路的正确性。

## 本样例支持的产品

| 产品 | NPU架构 |
|------|---------|
| Ascend 950PR/Ascend 950DT | dav-3510 |

> **说明：** 该样例依赖尚未正式发布的CANN特性，请使用最新的CANN master包。

## 目录结构介绍

```text
├── copy_ub2ub_tensor_api
│   ├── scripts
│   │   ├── gen_data.py                     // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py                // 真值对比文件
│   ├── CMakeLists.txt                      // 编译工程文件
│   ├── data_utils.h                        // 数据读取写出函数
│   ├── copy_ub2ub_tensor_api.asc           // Ascend C样例实现与调用样例
│   └── README.md                           // 样例说明文档
```

## 样例描述

- 样例功能：

  样例在Vector Kernel中通过Tensor API构造GM、UB上的Tensor对象，使用`Copy`接口完成`GM -> UB -> UB -> GM`三段搬运。三段搬运分别位于MTE2、V、MTE3三条流水上，通过`SetFlag`/`WaitFlag`完成流水间同步。

- 样例规格：

  | 参数 | 默认值 | 说明 |
  | :--- | :----- | :--- |
  | ROW | 128 | 数据行数 |
  | COL | 256 | 数据列数 |
  | T | half | 数据类型 |

  <a name="表1"></a>
  <table border="2" align="center">
  <caption style="font-weight: normal;">
      <span style="font-weight: bold; font-size: 1.2em;">📌 表1：样例规格表</span></caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Copy via UB2UB</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[ROW, COL]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">out</td><td align="center">[ROW, COL]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">copy_ub2ub_custom</td></tr>
  </table>

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。**当前仅支持使用[CANN master](../../../../../docs/quick_start.md#下载-cann-master)**。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确
  ```

  NPU仿真模式：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 执行结果
  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
