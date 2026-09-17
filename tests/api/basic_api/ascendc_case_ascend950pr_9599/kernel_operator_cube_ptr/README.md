# kernel_operator_cube_ptr — CUBE Basic API 指针重载签名锁定用例

## 用途

CUBE 侧 Basic API 的**指针重载**（`LoadData` / `LoadDataWithStride` / `LoadDataWithTranspose` /
`LoadDataWithSparse` / `Mmad` / `MmadMx` / `MmadWithSparse` / `Fixpipe` / `SetFixPipeConfig`）
在 `impl/basic_api/kernel_operator_mm_ptr_impl.h`、`impl/basic_api/kernel_operator_fixpipe_ptr_impl.h` 中提供。

`test_operator_cube_ptr.cpp` 把
`examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/` 下 14 个样例用到的**全部**指针调用形态
（21 种）各实例化一次。任何重载被删除、改名、改参数顺序、或与 Tensor 重载产生二义，该 TU 都会编译失败。

它同时锁定 `GetUnderlyingPtr` 的语义：

```cpp
static_assert(Std::is_pointer_v<__gm__ half*>);          // 地址空间限定指针被识别为指针
static_assert(isRawHwPtrV<__cbuf__ half*>);              // 裸硬件指针走透传分支
static_assert(!isRawHwPtrV<LocalTensor<half>>);          // Tensor 走 GetPhyAddr() 分支
```

## 前置条件

| 项 | 要求 |
| --- | --- |
| CANN | **>= 9.2.0**（与 `examples/.../03_matrix_compute` 下 14 个样例一致）。仓库 master 的头文件/样例使用 `fp8_e4m3_t` / `float4_e2m1_t` / `float8_e4m3x2_t` / `fp8_e5m2fn_t` 与内建 `__nop`，CANN 9.1.0 的 `ccec` 不认识它们，会报 `unknown type name`（与本次改动无关） |
| 编译器 | `ccec`（bisheng）可用：`source ${install_path}/cann/set_env.sh`，该脚本同时提供 `ASCEND_HOME_PATH` |
| 仓库 | 已 checkout 本仓库；**不需要**预先 `build.sh install` asc-devkit |

## 一键校验（推荐）

```bash
source /usr/local/Ascend/cann/set_env.sh
bash compile_check.sh all        # 也可写 dav-3510 / dav-2201
# 期望输出：PASS dav-3510: 21 种指针调用形态全部解析 / PASS dav-2201: ...
```

`compile_check.sh` 做了一件关键的事：**搭一个 CANN 影子根**。原因是 ASC 驱动对 `.asc` 输入会把
「已安装的 asc-devkit」硬编码进头文件搜索路径，且优先级高于用户的 `-I` / `-isystem`：

- `-isystem <源码树>/include` → **不生效**，仍会编译到已安装的那棵树（实测 27 处 `no matching function`）；
- `-iquote <源码树>/include -iquote <源码树>/impl` → 第一层能指向源码树（`-H` 可见），但第二层仍落到已安装树，
  两棵树混用会报 `utils/std/tuple.h: non-constexpr declaration of 'get'`、`hccl_utils.h: use of undeclared
  identifier 'WriteGmBypassDCache'` 之类无关错误（实测 118 处）；
- **`ASCEND_HOME_PATH` 被 ASC 驱动尊重**，因此用符号链接搭一个影子 CANN 根、只把 `asc/include`、`asc/impl`
  换成被测源码树，就能让驱动去读本仓库的头文件，且**不修改共享的 CANN 安装**。

等价的展开形式（脚本内部就是这么做的）：

```bash
REPO=<本仓库根目录>
SH=/tmp/cube_ptr_shadow/cann
mkdir -p "$SH"
for d in "${ASCEND_HOME_PATH}"/*; do ln -sfn "$d" "$SH/$(basename "$d")"; done
rm -f "$SH/asc"; mkdir -p "$SH/asc"
for d in "${ASCEND_HOME_PATH}/asc"/*; do ln -sfn "$d" "$SH/asc/$(basename "$d")"; done
rm -f "$SH/asc/include" "$SH/asc/impl"          # 只删符号链接，真实安装不受影响
cp -r "$REPO/include" "$SH/asc/include"
cp -r "$REPO/impl"    "$SH/asc/impl"

cp test_operator_cube_ptr.cpp /tmp/t.asc         # ccec 按扩展名选择 CCE 方言
ASCEND_HOME_PATH=$SH ccec --asc-aicore-lang -fsyntax-only --npu-arch=dav-3510 -ferror-limit=0 /tmp/t.asc
ASCEND_HOME_PATH=$SH ccec --asc-aicore-lang -fsyntax-only --npu-arch=dav-2201 -ferror-limit=0 /tmp/t.asc
# 期望：无输出，退出码 0
```

## CMake 方式

```bash
mkdir -p build && cd build
cmake -DARCH=all ..        # 或 -DARCH=dav-3510 / -DARCH=dav-2201
make
```

需要先 `source ${install_path}/cann/set_env.sh`（提供 `ccec` 与 `ASCEND_HOME_PATH`）。

## 运行命令

本用例是**编译期**用例，编译通过即通过，无需运行。

## 端到端：14 个样例

```bash
source /usr/local/Ascend/cann/set_env.sh
bash run_all_examples.sh all          # 或 dav-3510 / dav-2201 过滤
# 默认 RUN_MODE_2201=cpu：dav-2201 样例在 950PR 机器上需要 cpu/sim 模式；dav-3510 样例用 npu 模式
```

逐样例 `gen_data.py` → `cmake+make` → `./demo` → `verify_result.py`，日志落在 `./cube_examples_logs/`。

| # | 用例 | 样例目录（本仓库） | 默认 arch | SCENARIO_NUM |
| --- | --- | --- | --- | --- |
| 1 | `batch_matmul` | `.../03_matrix_compute/batch_matmul` | dav-2201 | — |
| 2 | `fixpipe_l0c2gm` | `.../fixpipe_l0c2gm` | dav-2201 | 1 |
| 3 | `fixpipe_l0c2l1` | `.../fixpipe_l0c2l1` | dav-2201 | 1 |
| 4 | `fixpipe_l0c2ub` | `.../fixpipe_l0c2ub` | dav-3510 | 1 |
| 5 | `load_data_2dmx_l12l0` | `.../load_data_2dmx_l12l0` | dav-3510 | 1 |
| 6 | `load_data_2dv2_l12l0` | `.../load_data_2dv2_l12l0` | dav-3510 | 1 |
| 7 | `load_data_l12l0` | `.../load_data_l12l0` | dav-2201 | 1 |
| 8 | `load_data_with_stride` | `.../load_data_with_stride` | dav-3510 | 1 |
| 9 | `mmad` | `.../mmad` | dav-2201 | 1 |
| 10 | `mmad_gemv` | `.../mmad_gemv` | dav-2201 | — |
| 11 | `mmad_load3dv2` | `.../mmad_load3dv2` | dav-2201 | 4 |
| 12 | `mmad_mx` | `.../mmad_mx` | dav-3510 | 1 |
| 13 | `mmad_unitflag` | `.../mmad_unitflag` | dav-2201 | — |
| 14 | `mmad_with_sparse` | `.../mmad_with_sparse` | dav-2201 | — |

> `...` = `examples/01_simd_cpp_api/03_basic_api/03_matrix_compute`。

## 覆盖率

| API | 本用例中实例化的指针重载数 |
| --- | --- |
| `LoadData` | 24 |
| `LoadDataWithStride` | 3（3510） |
| `LoadDataWithTranspose` | 3 |
| `LoadDataWithSparse` | 1（2201） |
| `Mmad` | 5 |
| `MmadMx` | 5（3510） |
| `MmadWithSparse` | 1（2201，含花括号初始化列表形态） |
| `Fixpipe` | 12 |
| `SetFixPipeConfig` | 2 |

## 注意

- `Fixpipe` 与 `LoadData` 的指针重载按 `__NPU_ARCH__` 门控，与本文件内的 `#if` 一一对应；某个架构下不适用的重载不会实例化。
- `Mmad` 的 5 参形态有两种 bias 入参地址空间：`__biasbuf__`（BiasTable，C 矩阵初值来自 BT）与 `__cc__`
  （L0C）。`__biasbuf__` 关键字在 x86-64 主机目标上不被 CCE 接受（`CCE memory attribute __biasbuf__ is not allowed
  in x86-64`），因此本用例只实例化 `__cc__` 形态；`__biasbuf__` 形态在设备目标编译时覆盖。
- 实测已知项：`mmad_load3dv2`（3Dv2Pro 形态）在 CANN 9.1.0 上会多出 `LoadDataNDVNLNLNACal` /
  `LoadDataNDVNLNLNBCal` 两条 no-match，与同时出现的 unknown-dtype 错误疑似级联，待 CANN >= 9.2 环境确认。
- 本目录**未**接入 `tests/api/basic_api/CMakeLists.txt`，以免在设备端验证前影响既有 950PR AIC 测试构建；
  验证通过后可在 `tests/api/basic_api/CMakeLists.txt` 的 `ASCENDC_TEST_ascend950pr_9599_AIC_CASE_SRC_FILES`
  glob 中加入 `kernel_operator_cube_ptr/*.cpp`（该用例只做签名检查，接入时建议单独建 target，避免与运行时用例混编）。