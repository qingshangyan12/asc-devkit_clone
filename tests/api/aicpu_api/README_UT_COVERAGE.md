# aicpu_dump.cpp UT 覆盖率说明

本文件记录 `impl/aicpu_api/aicpu_dump.cpp`（FIFO aicpu printf 新增实现）的单元测试覆盖率结论。

## 1. 结论摘要

| 指标 | 目标 | 实际 | 是否达标 |
|---|---|---|---|
| 行覆盖率 | ≥ 95% | **95.9%**（165/172） | 是 |
| 函数覆盖率 | — | 95.0%（19/20） | — |
| 分支覆盖率 | 100% | **80.0%**（80/100） | 否（见下文不可达分支分析） |

分支覆盖率未达 100% 的原因：剩余 20 个分支在**数学上不可达**（防御性检查 + 编译器异常展开路径 + gcov 时序限制），无法通过公共 API `AscendC::printf` 驱动覆盖。经确认，维持当前 80% 分支覆盖率，保留本说明文档。

## 2. 测试二进制构成

新旧两套流程通过 weak symbol 分派，因此拆为两个测试二进制：

| 目标 | 二进制 | 覆盖路径 | 用例数 |
|---|---|---|---|
| `ascendc_ut_aicpu_api` | `test_aicpu_dump.cpp` | Legacy（线性缓冲区） | 通过 |
| `ascendc_ut_aicpu_api_fifo` | `test_aicpu_dump_fifo.cpp` | FIFO（环形缓冲区） | 18/18 通过 |

- 旧测试 `test_aicpu_dump.cpp` 无需修改：其二进制中未提供 `AicpuGetDfxInfo` 强符号，weak symbol 解析为 nullptr，`printf()` 恒走 Legacy 路径，旧用例语义不变。
- 新测试 `test_aicpu_dump_fifo.cpp` 提供 `AicpuGetDfxInfo` 强符号，强制走 FIFO 路径；通过 `g_utAicpuGetDfxInfoHook` 控制 rts 返回内容，覆盖 `GetAscAicpuPrintfMemInfo` 全部 7 个出口分支。
- `CMakeLists.txt` 为 fifo 目标增加 `-Wl,--wrap=malloc`，测试文件通过 `__wrap_malloc` 注入临时缓冲区分配失败，规避 mockcpp hook malloc 的重入崩溃。

## 3. 未覆盖分支清单（20 个）

### 3.1 防御性检查分支（数学不可达，5 个）

| 行号 | 代码 | 不可达原因 |
|---|---|---|
| `BRDA:155` | `if (AicpuGetDfxInfo == nullptr)` 的 true 分支（`GetAscAicpuPrintfMemInfo` 内） | `printf()` 入口（L459）已对 nullptr 分派：为 null 走 `PrintfLegacy`，为非 null 才进入 `PrintfFifo`。进入 `PrintfFifo` 时 `AicpuGetDfxInfo` 必非空，此检查仅作防御 |
| `BRDA:183` | `if (memInfo.printfMemAddr == 0U)` 的 true 分支（`GetAicpuDebugBlockInfo` 内） | 上游 `GetAscAicpuPrintfMemInfo`（L173）仅在 `printfMemAddr != 0U` 时返回 0；`PrintfFifo` 仅在返回 0 时继续调用本函数，故入参必非 0 |
| `BRDA:273` | `if (buf_ == nullptr && size > 0)` 的 `size > 0` false 分支（`FifoTempBuf::Get`） | `Get()` 唯一调用点（L368）传参 `ctx.ringBufLen`；调用前 L420 已保证 `ringBufLen != 0U`（为 0 时提前 return），故 `size > 0` 恒真 |
| `BRDA:362` | `size_t count = (ctx.strLen < destMax) ? ... : ...` 的 false 分支（非拆分写路径） | `tlvLen = CalcTlvLen(strLen)` ≥ `strLen + 8`；`CheckAicpuRingBuf` 已保证 `tlvLen <= ringBufLen`；`destMax = ringBufLen - writingOff` 且 header 已写入后 `writingOff < ringBufLen`，故 `strLen < destMax` 恒真 |
| `BRDA:374` | 同上，拆分写路径（tempBuf 版本） | `destMax = ringBufLen`，同上论证恒真 |

### 3.2 析构函数分支（gcov 时序限制，2 个 + 5 行）

| 行号 | 代码 | 不可达原因 |
|---|---|---|
| `BRDA:265`（两分支） | `FifoTempBuf::~FifoTempBuf()` 中 `if (buf_ != nullptr)` | `g_fifoTempBuf` 是文件级 static，析构发生在 `main()` 返回之后、**gcov 向 .gcda 文件 flush 之后**，该函数数据永远无法写入 .gcda，属 gcov 工具时序限制，非测试缺失 |
| `DA:263/265/266/267/269` | 析构函数体 5 行 | 同上 |

### 3.3 编译器异常展开分支（e0，13 个）

`BRDA:160,238,303,315,344,407,408,428,430,437,444,461,463` 上的 `,e0,` 分支为 GCC 在函数调用点生成的异常展开路径（`.gcc_except_table`）。

本文件无 `try/catch/throw`，所有异常分支在正常运行中不可能触发，属于编译器产物，业界一般不纳入覆盖率考核口径。这些分支对应的实际调用（如 `PrintfLegacy` L461、`PrintfFifo` L463）已被正常覆盖。

## 4. 如需达成“分支 100%”的可选方案

若后续强制要求分支 100%，业界标准做法（CANN UT 常用）为在 impl 源码**不可达分支处添加 lcov 排除注释**，仅影响统计口径、不改变运行行为：

```cpp
if (AicpuGetDfxInfo == nullptr) { // LCOV_EXCL_BR_LINE
    return -1;
}
```

或对析构函数使用：

```cpp
// LCOV_EXCL_START
~FifoTempBuf() { ... }
// LCOV_EXCL_STOP
```

经实测 lcov 2.0 默认开启源码排除标记（`--no-markers` 才关闭），此方案可行。**本仓库按当前决策未修改 impl 源码**。

## 5. 覆盖率复现步骤

```bash
source /usr/local/Ascend/cann/set_env.sh
cd /data/lx/0824/asc-devkit_liuliu
mkdir -p build && cd build
cmake .. -DENABLE_TEST=ON -DENABLE_GCOV=ON \
    -DTEST_MOD=ascendc_ut_aicpu_api,ascendc_ut_aicpu_api_fifo \
    -DASCEND_CANN_PACKAGE_PATH=/usr/local/Ascend/cann
cmake --build . --target ascendc_ut_aicpu_api ascendc_ut_aicpu_api_fifo -j 32
# POST_BUILD 自动运行测试；随后手动运行旧二进制以生成其 .gcda
./tests/api/aicpu_api/ascendc_ut_aicpu_api >/dev/null 2>&1
# 采集合并
lcov -c -d . --rc lcov_branch_coverage=1 --ignore-errors mismatch --ignore-errors source -o cov_report/full.info
lcov -e cov_report/full.info '*impl/aicpu_api/aicpu_dump.cpp' -o cov_report/aicpu.info \
    --rc lcov_branch_coverage=1 --ignore-errors mismatch --ignore-errors source
lcov --summary cov_report/aicpu.info --rc lcov_branch_coverage=1
```

最终数据归档于 `build/cov_report/aicpu_final.info`。
