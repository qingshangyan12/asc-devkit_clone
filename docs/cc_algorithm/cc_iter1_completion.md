# 集合通信算法迭代 1 完成说明

## 1. 基本信息

| 项目 | 值 |
|---|---|
| 完成日期 | 2026-08-06 |
| asc-devkit 仓 | `/mnt/c/data/gitcodes/asc-devkit` |
| 工作分支 | `cc0806_2` |
| 分支基线提交 | `2f582c106dc7b15e965bde4bfff1892051ee00b1` |
| HCCL 仓 | `/mnt/c/data/gitcodes/hccl` |
| HCCL 对比提交 | `530d77c111ede79f390e1e14cc27a50c93ae2e01` |
| CANN 安装目录 | `/home/houyu/Ascend/cann-9.2.0` |
| 需求文档 | `/mnt/c/data/design_docs/cc_algorithm/cc_iter1_plan.md` |
| xiaoting 对比分支 | `pr_4721`，提交 `0dbb7ac870a2f6fb74260e347c5c348a4d236ca9` |

当前成果位于未提交工作区。不要使用 `git reset --hard`、`git checkout --` 等命令清理；迁移目录中包含大量新增文件。

## 2. 本轮范围

本轮完成单层、单 die、FullMesh/Mesh1D 范围：

| 集合通信 | AICPU | CCU/KFC |
|---|---|---|
| AllGather | `InsAllGatherMesh1D` | `CcuAllGatherMesh1DMem2Mem` |
| ReduceScatter | `InsReduceScatterMesh1D` | `CcuKfcReduceScatterMesh1DMem2Mem` |
| AllReduce | `InsAllReduceMesh1DOneShot`、`InsAllReduceMesh1DTwoShot` | `CcuAllReduceMesh1DMem2Mem` |
| AllToAll | 计划中无独立 AICPU 算法 | `CcuAlltoAllMesh1D` |
| AllToAllV | `InsAlltoAllVMesh1D` | `CcuAlltoAllVMesh1D` |

本轮明确不包含：

- `HalfAllToAllV`。
- 双 die 算法。
- DPU ST。
- CCU device 仿真；当前没有 CCU 仿真器，只运行 CCU host ST。

源码树中可能保留 HCCL 同步过来的双 die、DPU 或旧 selector 文件，但它们未进入本轮 CMake、注册或有效 selector 路径。

## 3. 成果路径

### 3.1 算法实现

| 内容 | 路径 |
|---|---|
| HCCL CC 总入口 | `impl/adv_api/detail/hccl/cc/src/` |
| AllGather | `impl/adv_api/detail/hccl/cc/src/ops/all_gather/` |
| ReduceScatter | `impl/adv_api/detail/hccl/cc/src/ops/reduce_scatter/` |
| AllReduce | `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/` |
| AllToAll/AllToAllV | `impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/` |
| KFC Server | `impl/adv_api/detail/hccl/cc/src/ops/kfc_server/` |
| KFC 多算子配置 | `impl/adv_api/detail/hccl/cc/src/common/kfc_server_config.h` |
| CCU 资源与算法编码 | `impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.{h,cc}` |
| 本地 HCCL bridge 控制 | `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc` |

### 3.2 ST

| 内容 | 路径 |
|---|---|
| ST 总目录 | `tests/api/adv_api/hccl/cc/st/algorithm/` |
| AICPU 用例 | `tests/api/adv_api/hccl/cc/st/algorithm/testcase/` |
| CCU host 用例 | `tests/api/adv_api/hccl/cc/st/algorithm/testcase/mc2_ccu/host/` |
| CCU adapter/stub | `tests/api/adv_api/hccl/cc/st/algorithm/utils/src/ccu/` |
| CCU KFC/MC2 stub | `tests/api/adv_api/hccl/cc/st/algorithm/utils/src/mc2_ccu/` |
| CCU checker | `tests/api/adv_api/hccl/cc/st/algorithm/utils/src/hccl_verifier/` |
| CCU sim world | `tests/api/adv_api/hccl/cc/st/algorithm/utils/src/sim_world/` |

### 3.3 构建与包

| 内容 | 路径 |
|---|---|
| 构建目录 | `/mnt/c/data/gitcodes/asc-devkit/build` |
| 打包目录 | `/mnt/c/data/gitcodes/asc-devkit/build_out` |
| 最终 run 包 | `/mnt/c/data/gitcodes/asc-devkit/build_out/cann-asc-devkit_9.1.0_linux-x86_64.run` |
| device 包 | `build/device-asc-devkit.tar.gz` |
| 仓内 makeself 缓存 | `build/makeself/` |
| CANN makeself 备选 1 | `/home/houyu/Ascend/cann-9.2.0/tools/op_project_templates/ascendc/customize/cmake/util/makeself` |
| CANN makeself 备选 2 | `/home/houyu/Ascend/cann-9.2.0/x86_64-linux/tikcpp/ascendc_kernel_cmake/fwk_modules/util/makeself` |

最终 run 包 SHA256：

```text
3c23e014aea7ebc8a666a8989ba20f7c8e103195e8314b73f9de95c287fc1f78
```

## 4. 用例构建与执行方法

### 4.1 环境准备

```bash
cd /mnt/c/data/gitcodes/asc-devkit
source /home/houyu/Ascend/cann/set_env.sh
pkg-config --version
```

本次验证使用 `pkg-config 2.5.1`。

### 4.2 构建主库

```bash
cd /mnt/c/data/gitcodes/asc-devkit
source /home/houyu/Ascend/cann/set_env.sh
cmake --build build --target mc2_client -- -j4
```

预期结果：`[100%] Built target mc2_client`。

### 4.3 构建并执行 AICPU e2e ST

首次配置或测试源码变化较多时：

```bash
cd /mnt/c/data/gitcodes/asc-devkit
source /home/houyu/Ascend/cann/set_env.sh
bash build.sh --adv_test_two -j4
```

仅增量构建目标：

```bash
cmake --build build \
  --target ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC \
  -- -j4
```

直接执行：

```bash
build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/\
ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC
```

已验证结果：`84 tests from 4 test suites`，`84/84 PASSED`。

### 4.4 构建并执行 CCU host ST

```bash
cd /mnt/c/data/gitcodes/asc-devkit
source /home/houyu/Ascend/cann/set_env.sh
cmake --build build \
  --target ascendc_ut_adv_api_hccl_ccu_mc2_host \
  -- -j4

build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/mc2_ccu/\
ascendc_ut_adv_api_hccl_ccu_mc2_host
```

已验证结果：`12 tests from 1 test suite`，`12/12 PASSED`。

CCU host ST 覆盖：算法选择、tiling、资源申请、KFC opId、reduceType 区分、Server cache、多配置共享/隔离、队列和启动参数校验。

### 4.5 打包

完整入口：

```bash
cd /mnt/c/data/gitcodes/asc-devkit
source /home/houyu/Ascend/cann/set_env.sh
bash build.sh --pkg -j4
```

已有完整构建目录时可直接执行最终阶段：

```bash
cd /mnt/c/data/gitcodes/asc-devkit/build
source /home/houyu/Ascend/cann/set_env.sh
cpack --verbose
```

run 包自校验会写 `$HOME/log/makeself`。在受限环境下将日志和临时目录放到 `/tmp`：

```bash
cd /mnt/c/data/gitcodes/asc-devkit
env HOME=/tmp/asc-devkit-package-check \
  TMPDIR=/tmp/asc-devkit-package-check \
  ./build_out/cann-asc-devkit_9.1.0_linux-x86_64.run --check
```

## 5. 迁移结论

### 5.1 xiaoting MR 4721

三项设计方向合理，应吸收其架构思路，但不能原样复制：

1. 删除 `kfc_server_op.cc/.h` 是正确的。KFC Server 不是用户可选择的集合通信 op，而是在资源申请阶段继承并替换末端算法 kernel 的常驻服务。把它注册成独立 op 会产生错误的 selector/executor 生命周期。
2. 新增 `kfc_server_config.h` 是必要的。一个 KFC Server 需要承载多个 opId，并用配置 hash 区分可安全复用的 Server IR。
3. 重写 `DispatchKfcSubKernel` 是必要的。CCU IR 在 host 侧生成，必须把配置内每个 opId 编译成 `CCU_IF` 分支，不能依赖设备运行时 C++ switch。

吸收时完成的修正：

- C++14 不支持 `inline thread_local` 变量，改为函数内 `static thread_local`。
- AllReduce 和 ReduceScatter 的 opId 都编码 `reduceType`，防止 SUM/MAX/MIN 等错误复用。
- dispatch 增加 AllReduce、固定 AllToAll 和 AllToAllV。
- Server cache hash 同时覆盖算法 tag 和完整 opId 列表。
- 删除 KFC Server 独立 op 源文件及 CMake 入口。

### 5.2 HCCL 新版本高性能修改

HCCL 后续高性能改动主要集中在 UBX AIV、三级拓扑和其他非本轮路径。本轮只迁移与单层、单 die、非 AIV Mesh1D 相符的变化；不机械复制不适用的优化。AllGather、ReduceScatter、AllReduce、AllToAll(V) 的有效 selector 均应在入口严格校验本轮拓扑。

### 5.3 必须审计本地执行链

仅迁移 selector/executor/template 不足以证明算法生效。主线 `cann_host_bridge` 曾把 AllReduce、AllToAll 和 AllToAllV 转发给安装版 `libhccl.so`，导致本仓实现未被执行。本轮已关闭这些 bridge。后续迁移每个算子都必须确认调用最终进入 asc-devkit 本地注册表。

### 5.4 文件存在不等于算法接入

判断算法是否生效必须同时检查：

1. 父级和子级 CMake 是否编译目标源码。
2. selector 是否注册、优先级是否冲突、拓扑分支是否可达。
3. executor 的算法名是否与 selector 完全一致。
4. `AlgorithmType`、`algorithmMap`、KFC dispatch 是否一致。
5. ST 是否真正执行本仓实现，而不是安装版 HCCL。

## 6. 最终验证记录

| 检查项 | 结果 |
|---|---|
| AICPU e2e ST | 84/84 通过 |
| CCU host ST | 12/12 通过 |
| `mc2_client` | 构建通过 |
| device 外部工程 | 构建通过 |
| CPack/makeself | 通过，生成 run 包 |
| run 包 SHA256 校验 | 通过 |
| `git diff --check` | 通过 |
