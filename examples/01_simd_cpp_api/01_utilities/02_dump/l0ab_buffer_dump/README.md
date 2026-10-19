# L0A/L0B Buffer DumpTensor 接口功能说明

## 概述

本样例以 Cube 编程模式的 half 矩阵乘为背景，通过 `DumpTensor` 打印矩阵 A、B 在 **L0A Buffer、L0B Buffer**
中的数据，并对照打印搬入前的 L1 Buffer 数据，展示 dav-9201（A6）L0A/L0B Buffer 的数据打印方法。

L0A/L0B 不存在直达 GM 的 DataCopy 通路，dump 数据经芯片 **DebugBus** 通路读出（`__NPU_ARCH__ == 9201` 的
`mem_copy_abuf_to_gm_impl` / `mem_copy_bbuf_to_gm_impl` 实现）。该通路仅 AIC（cube）核生效。

## 本样例支持的产品及CANN软件版本

| 产品 | 架构 | CANN软件版本 | HDK版本 |
|------|------|-------------|---------|
| Ascend 960DT系列产品 | dav-9201（A6） | >= CANN 9.1.0 | >= 25.7.RC1.6 |

> L0A/L0B dump 依赖 runtime 正确初始化 `DebugBlockHeadInfo::debugBusAddr`；`debugBusAddr == 0` 时该次 dump
> 静默跳过，不产出 TLV。本样例当前无 A6 实机环境验证，先提供样例与实现，后续在 A6 环境上验证。

## 目录结构介绍

```text
├── l0ab_buffer_dump
│   ├── scripts
│   │   ├── gen_data.py          // 生成输入数据和golden数据
│   │   └── verify_result.py     // 校验计算输出和dump日志
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── data_utils.h             // 二进制文件读写函数
│   ├── l0ab_buffer_dump.asc     // 矩阵乘、DumpTensor调用和Host调用
│   ├── README.md                // 中文样例说明文档
│   └── README_en.md             // 英文样例说明文档
```

## 样例描述

- 样例功能：

  以 half 矩阵乘为背景，计算 `C[m, n] = sum(A[m, k] * B[k, n])`。A、B 先由 Global Memory 搬入 L1
  （随路 ND2NZ），再由 L1 经 `LoadData` 搬入 L0A/L0B，最后由 `Mmad` 完成乘累加、`Fixpipe` 将 L0C 的
  float 结果写回 Global Memory。

- 样例规格：

  固定参数 `M=16, K=32, N=16`，shape 信息如下表所示：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">MatmulDumpL0ab</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">L0abBufferDump</td></tr>
  </table>

  输入 A 取 `0~511` 的互异整数（half 可精确表示），B 取 `1~7` 的小整数，便于按物理布局逐元素核对 dump
  数据。计算结果为 `< 2^24` 的整数，float32 可精确表示。

- DumpTensor 打印位置：

  | desc | 数据位置 | 计算中的用途 |
  |------|----------|--------------|
  | 100 | L1 Buffer | 搬入 L0A 前的矩阵 A（NZ 布局，对照参考） |
  | 101 | L1 Buffer | 搬入 L0B 前的矩阵 B（NZ 布局，对照参考） |
  | 200 | L0A Buffer | `LoadData` 搬入 L0A 后的矩阵 A |
  | 201 | L0B Buffer | `LoadData` 搬入 L0B 后的矩阵 B（每分形已转置） |

  `scripts/verify_result.py` 校验计算输出和四组 `DumpTensor` 日志。

## L0A/L0B 物理布局与 golden 组织

L1 的 NZ 物理布局（`DataCopy` 随路 ND2NZ 产生）与 L0A/L0B 的物理布局（`LoadData` 搬入决定）不同，
dump 按**物理存储顺序**输出，`verify_result.py` 按以下模型组织 golden：

- **A1（L1）/A2（L0A）**：以 `[K 分形 j][M 行][16 个连续 K 元素]` 排布，
  `phys[j*256 + m*16 + kk] = A[m][j*16 + kk]`。本样例配置下两者布局一致。
- **B1（L1）**：`nValue=K, dValue=N`，每个 K 行恰好 32B（16 个 half），按 `[K, N]` 行主序，
  `phys[k*16 + n] = B[k][n]`。
- **B2（L0B）**：`LoadData` 对每个 `[16,16]` 分形转置后按 K 分形顺序摆放，
  `phys[f*256 + n*16 + kk] = B[f*16 + kk][n]`。

> 注：L0A/L0B 的 DebugBus model base（`0x410000`/`0x420000`）、字节编址、每轮 32B 步进当前沿用 dav-3510
> 拷贝实现，A6 芯片若有差异需在 `npu_arch_9201/asc_aicore_dump_utils.h` 内修正。若上板 dump 结果与上述布局
> 模型不一致，请按实际物理布局调整 `verify_result.py` 中的 golden 组织方式。

## 编译运行

在本样例根目录下执行：

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-9201 .. && make -j
python3 ../scripts/gen_data.py
./demo 2>&1 | tee run.log
python3 ../scripts/verify_result.py output/output.bin output/golden.bin run.log \
    input/a_gm.bin input/b_gm.bin
```

`${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

校验通过时输出：

```text
Output result check: PASS
Matrix A in L1 data check: PASS
Matrix B in L1 data check: PASS
Matrix A in L0A data check: PASS
Matrix B in L0B data check: PASS
test pass!
```
