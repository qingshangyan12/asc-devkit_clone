# Ascend 950 asc_dcci_entire_ub Support Design

## 需求

为 Ascend 950（`__NPU_ARCH__ == 3510`）补充 C API `asc_dcci_entire_ub()` 的 Vector Core 实现，并在 Ascend 950 NPU 上验证该接口能够处理 UB 与 Scalar DCache 的缓存一致性。

本次交付包括：

- 3510 架构下的接口实现和公开转发；
- 3510 实现级单元测试；
- `examples/02_simd_c_api/03_c_api/04_cache_ctrl` 下可独立编译、运行和复验的 NPU 样例；
- 样例的 `test.sh`、中英文 README 和目录索引；
- API 产品支持范围文档更新；
- 将指定 CANN 包的 C API include/impl 子树切换为当前仓库软链接。

## 背景简述

目标开发仓为 `/home/g00954335/asc-devkit_GRJ`，目标 CANN 包为 `/usr/local/Ascend/cann-9.2.0-beta.1`，目标架构为 `dav-3510`。

当前仓库的 `include/c_api/cache_ctrl/cache_ctrl.h` 已声明 `asc_dcci_entire_ub()`，并已在 3510 分支包含 `impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h`，因此本次无需修改公共 include 源码。缺口位于 3510 的聚合实现和底层 DCCI 实现。

Ascend 950 的 AIC 没有 UB，因此该接口只允许在 AIV/Vector Core 调用。本次实现不添加 `ASC_IS_AIV` 静默保护；样例使用 `__vector__ __global__` Kernel，文档明确产品和 Core 约束，AIC 不属于验证范围。

仅验证“接口能够编译”或“调用后读到新值”不足以证明接口有效。只有在 `CTRL[49] = 1` 时，Scalar 读取 UB 才经过 DCache；样例必须先构造并观察到旧 DCache 副本，再证明调用接口后读取到 UB 新值。

## 涉及领域

- AscendC SIMD C API cache control；
- DAV 3510 架构实现分派；
- Scalar DCache 与 UB 缓存一致性；
- Vector 到 Scalar 的核内流水同步；
- DCCI 后的内存屏障；
- CANN 安装包与本地 devkit 源码联调；
- NPU 样例、单元测试和 API 文档。

## 方案详述

### 1. 3510 接口实现

修改 `impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_dcci_impl.h`，新增无参数内部实现：

```cpp
__aicore__ inline void asc_dcci_entire_ub_impl()
{
    dcci((__ubuf__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_UB);
}
```

修改 `impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h`，新增公开转发：

```cpp
__aicore__ inline void asc_dcci_entire_ub() { asc_dcci_entire_ub_impl(); }
```

实现与 2201 现有接口保持一致，不增加新的抽象、参数或 AIC 兼容分支。`include/c_api/cache_ctrl/cache_ctrl.h` 当前声明和架构分派已经满足要求，不产生源码修改。

### 2. 实现级单元测试

在 `tests/api/c_api/npu_arch_3510/cache_ctrl/test_dcci.cpp` 增加 `asc_dcci_entire_ub()` 用例。Mock 必须验证：

- 地址为 UB 地址空间的空地址；
- 范围为 `cache_line_t::ENTIRE_DATA_CACHE`；
- 目标为 `dcci_dst_t::CACHELINE_UB`；
- 底层 `dcci` 只调用一次。

该测试验证 API 到 intrinsic 参数的映射，不替代 NPU 功能测试。

### 3. CANN C API 子树软链接

目标目录为：

- `/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/include/c_api`；
- `/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/impl/c_api`。

这两个路径当前是普通目录，并包含与仓库不同的人工修改。执行时先分别移动为以下固定备份：

- `include/c_api.cann-9.2.0-beta.1.backup-before-asc-dcci-entire-ub`；
- `impl/c_api.cann-9.2.0-beta.1.backup-before-asc-dcci-entire-ub`。

若任一备份路径已存在，停止操作并报告冲突，不覆盖已有备份。备份成功后，在原路径建立绝对软链接：

- CANN `asc/include/c_api` -> `/home/g00954335/asc-devkit_GRJ/include/c_api`；
- CANN `asc/impl/c_api` -> `/home/g00954335/asc-devkit_GRJ/impl/c_api`。

只替换 C API 子树，不替换整个 `asc/include` 或 `asc/impl`。使用 `readlink -f` 验证最终目标；软链接保持到任务结束，便于用户手工复验。

恢复方式是删除两个软链接，再把两个固定备份目录移回原路径。恢复操作不属于本次默认执行范围。

### 4. NPU 功能样例

从空目录新建以下结构：

```text
examples/02_simd_c_api/03_c_api/04_cache_ctrl/
|-- README.md
`-- dcci_entire_ub/
    |-- CMakeLists.txt
    |-- README.md
    |-- README_en.md
    |-- dcci_entire_ub.asc
    `-- test.sh
```

样例不需要输入数据文件或数据生成脚本。Kernel 和 Host 启动、校验逻辑放在 `dcci_entire_ub.asc` 中，CMake 固定默认架构为 `dav-3510`、默认运行模式为 `npu`。

Kernel 声明为 `__vector__ __global__`，只生成和运行 AIV 路径。使用两个分属不同 Cache Line 的 UB 地址，旧值和新值分别采用不同的常量，避免单地址偶然命中造成误判。

Kernel 时序如下：

1. 保存原始 CTRL。
2. Vector 将两个 UB Cache Line 初始化为旧值。
3. 建立 `V -> S` 同步。
4. 通过 `asc_get_ctrl()` 和 `asc_set_ctrl()` 只设置 `CTRL[49]`，保留其余 bit。
5. Scalar 通过 volatile UB 访问读取两个旧值，使两条 Cache Line 进入 DCache。
6. Vector 将相同 UB 地址更新为新值。
7. 再次建立 `V -> S` 同步，保证后续 Scalar 读取发生在 Vector 写完成之后。
8. Scalar 在 DCCI 前读取两个地址，记录为 `before_dcci`。
9. 调用 `asc_dcci_entire_ub()`。
10. 调用 `asc_sync_data_barrier(mem_dsb_t::DSB_UB)`，等待 UB 相关 DCCI 操作完成。
11. Scalar 再次读取两个地址，记录为 `after_dcci`。
12. 恢复原始 CTRL。
13. 将 CTRL 检查值、预热值、DCCI 前值和 DCCI 后值输出至 GM，并在 Kernel 结束前完成必要同步。

Host 使用 ACL 初始化设备、分配输出、启动 Kernel、同步设备并回读结果。每个 ACL 调用均检查返回码；失败时输出具体阶段并完成已分配资源的释放。

### 5. 样例判定规则

样例只有同时满足以下条件才返回成功：

- 设置后的 CTRL bit 49 为 1；
- 两个预热读取值均等于各自旧值；
- DCCI 前两个读取值仍等于各自旧值；
- DCCI 后两个读取值均等于各自新值；
- ACL 启动、同步和数据回读均成功。

若 DCCI 前已经读到新值，样例判定为失败或证据不足，不能据此认为接口有效。失败输出需要逐项打印名称、实际值和预期值。成功时输出关键观测值和唯一标志 `test pass!`。

### 6. test.sh

`test.sh` 使用 `set -euo pipefail`，通过脚本路径推导仓库根目录，不硬编码开发仓路径。CANN 路径优先读取 `ASCEND_CANN_PACKAGE_PATH`，未设置时使用本次指定的 `/usr/local/Ascend/cann-9.2.0-beta.1`。

脚本依次执行：

1. 检查 CANN 根目录、编译器和两个 C API 路径存在；
2. 要求两个 C API 路径为软链接；
3. 使用 `readlink -f` 校验它们分别指向当前仓库的 `include/c_api` 和 `impl/c_api`；
4. 清理样例自身的固定 `build` 目录；
5. 使用 `cmake -S ... -B ... -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu` 配置；
6. 使用 `cmake --build` 编译；
7. 运行生成的可执行文件并保存输出；
8. 同时检查进程退出码和输出中的 `test pass!`。

脚本不创建、修改或恢复 CANN 软链接。链接不正确时，在编译前失败并打印期望值和实际值。

### 7. 文档更新

修改 `docs/zh/api/SIMD-API/c_api/cache_ctrl/asc_dcci.md`：

- 删除 Ascend 950 不支持 `asc_dcci_entire_ub()` 的旧结论；
- 明确该接口在 Ascend 950 上仅支持 Vector Core；
- 保留 `CTRL[49] = 1` 和 DSB 相关一致性约束。

新增样例中英文 README，说明产品版本、目录结构、测试时序、编译运行命令、手工复验方法、通过条件和实测结果。更新 `04_cache_ctrl/README.md` 及 `examples/02_simd_c_api/03_c_api/README.md` 的样例索引。

README 的合入结论由实际验证结果决定。只有实现级 UT、dav-3510 编译、Ascend 950 NPU 功能测试和相关 C API 回归全部通过，才写明“可以合入”；否则写明“暂不可合入”并列出失败项。

### 8. 验证矩阵

按以下顺序验证：

1. 在切换软链接和补实现前记录原始 CANN 的 dav-3510 编译失败表现；
2. 运行 3510 cache control 定向 UT；
3. 检查两个 CANN 软链接的最终解析路径；
4. 在全新 build 目录编译 `dcci_entire_ub` 样例；
5. 在 Ascend 950 NPU 上运行样例；
6. 连续重复运行 `test.sh`，确认调用前旧值和调用后新值稳定出现；
7. 编译或运行一个现有 dav-3510 C API 样例，验证 C API 子树软链接没有造成明显兼容性回归；
8. 检查 Markdown 规范、Git diff 和工作区状态。

### 9. 不在范围内

- 不修改 `include/c_api/cache_ctrl/cache_ctrl.h`；
- 不支持 AIC 调用 `asc_dcci_entire_ub()`；
- 不为 AIC 添加静默 no-op 或 `ASC_IS_AIV` 保护；
- 不修改整个 CANN `asc/include` 或 `asc/impl`；
- 不在 `test.sh` 中改写 CANN 安装包；
- 不恢复任务结束时保留的 C API 软链接。
