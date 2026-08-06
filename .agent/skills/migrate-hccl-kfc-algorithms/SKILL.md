---
name: migrate-hccl-kfc-algorithms
description: 将 HCCL 集合通信算法迁移到 asc-devkit 的 CC 新架构，并完成 AICPU、CCU/KFC 适配、selector/executor/template/kernel/资源注册、调用链审计和 ST 验证。用户要求迁移或同步 HCCL 算法、对比 HCCL 新提交、适配 KFC Server、多算子 CCU dispatch、补充集合通信 ST、分析算法为何未命中本地实现时使用。
---

# HCCL 集合通信算法迁移与 KFC 适配

## 1. 输入与边界

开始前解析并记录：

- `DEVKIT_PATH`：目标 asc-devkit 仓根。
- `HCCL_PATH`：源 HCCL 仓根。
- `CANN_PATH`：CANN 安装根目录。
- `SOC_ARCH`：目标 SoC/测试架构。
- `SOURCE_REV`、`TARGET_REV`：HCCL 对比区间。
- `OPS`：待迁移集合通信操作和算法名。
- `ENGINES`：AICPU、CCU/KFC 或两者。
- `TOPO_SCOPE`：单/双 die、层数、Mesh/Clos 等。
- `TEST_SCOPE`：e2e、host-only、device、DPU。
- `EXCLUSIONS`：明确暂不迁移的算法或拓扑。

从 workspace 和环境变量发现路径，不硬编码个人目录。缺少会改变实现方案的输入时再询问用户。严格执行 `EXCLUSIONS`，不要因为源文件相邻而顺带接入 HalfAllToAllV、双 die、DPU 或其他拓扑。

若迁移引入新的 AscendC 公共 kernel API、重载、dtype、TPosition 或参数组合，先使用仓内 `asc-ar-dev` 的 API 文档门禁，核对 `docs/zh/api`、声明、实现和样例。

## 2. 建立事实基线

1. 确认两个仓的分支、HEAD、dirty 状态和远端关系。
2. 读取需求文档，列出算法、引擎、拓扑、数据类型、性能阈值和 ST 要求。
3. 选择 asc-devkit 中已经迁移成功且架构最接近的算法作为样板。
4. 对每个源算法建立文件清单：op、selector、executor、template、kernel、公共资源、CMake、注册和测试。
5. 用 Git 对比 HCCL 迁移基线与最新提交，不使用目录整体覆盖。

推荐命令：

```bash
git -C "$HCCL_PATH" diff --name-status "$SOURCE_REV..$TARGET_REV" -- src/ops/<op>
git -C "$HCCL_PATH" log --oneline "$SOURCE_REV..$TARGET_REV" -- src/ops/<op>
rg -n '<AlgorithmName>|REGISTER_EXEC|REGISTER_SELECTOR' "$HCCL_PATH/src/ops/<op>"
```

把差异分为：

- 与当前引擎和拓扑直接相关，必须迁移。
- 公共框架变化，需要按 asc-devkit 接口适配。
- AIV、UBX、多层、双 die、DPU 等范围外变化，只记录结论。
- 纯重命名、版权或格式变化，不扩大修改面。

## 3. 迁移分层

### 3.1 Op 与调用链

确认 op 的入口、参数构造、selector registry 和 executor registry 都来自 asc-devkit 本地目标。检索 `cann_host_bridge`、动态符号转发和安装版 `libhccl.so` 调用；如果 bridge 抢先返回，本地迁移代码不会执行。

禁止用“源码已编译”推断“算法已生效”。用 selector 命中日志、注册表或 ST stub 证明本地执行链。

### 3.2 Selector

将范围限制写在 selector 入口，至少校验：

- `topoLevelNums`。
- `level0Topo`。
- `level0MeshType`，明确 `SINGLE_DIE` 或目标双 die 类型。
- rank 数、dtype、reduce op、数据量阈值。
- input/output overlap 和引擎能力。

不要保留可达但未构建的算法名。相同 opType 下多个 selector 不得以同优先级竞争同一路径，除非注册框架定义了确定性顺序并有测试。

### 3.3 Executor 与 Template

保持 selector 名称、`REGISTER_EXEC_V2` 名称、`AlgorithmType` 和 `algorithmMap` 完全一致。迁移 `CalcRes`、`Run/KernelRun`、fast launch、资源继承和数据切片逻辑时，优先复用 asc-devkit 已有 helper，而不是复制 HCCL 私有依赖。

AICPU 算法保留任务图语义，确保 ST 可以检查 LocalCopy、Read/Write、Reduce、Post/Wait 和线程依赖。

CCU 算法将 host 侧资源/IR 生成和 kernel 算法本体分开。组合算法可复用已有子算法 kernel，但必须核对参数顺序、token、channel、rank 和 buffer offset。

### 3.4 CMake 与注册

逐层检查：

```text
ops/CMakeLists.txt
<op>/CMakeLists.txt
<op>/selector/CMakeLists.txt
<op>/executor/CMakeLists.txt
<op>/template/{aicpu,ccu}/CMakeLists.txt
```

源码位于安装目录或打包目录不代表它进入 `mc2_client`。从生成的 `build.make` 或 `DependInfo.cmake` 反查真实编译单元。

## 4. KFC Server 适配

### 4.1 生命周期模型

把 KFC Server 视为算法末端 CCU kernel 的常驻替代物，不把它注册为独立 HCCL op：

1. 算法 template 先计算原始 CCU kernel 资源。
2. KFC template 继承原始 kernel 的 channel、rank、参数和资源。
3. 用 KFC Server kernel 替换 `ccuKernelInfos` 中对应的末端 kernel。
4. client 通过共享队列提交 opId 和动态参数。
5. Server 根据 opId 进入预生成的 CCU IR 分支。

如果 KFC Server 被实现成独立 selector/executor，停止并重新检查生命周期；这通常会造成重复资源申请和错误的用户 op 语义。

### 4.2 多算子配置与缓存

维护一次资源申请所需的 op 配置：

- opId 至少编码 `opType`、输入 dtype、输出 dtype。
- AllReduce 和 ReduceScatter 必须编码 `reduceType`。
- AllToAll 系列从其 send/recv descriptor 读取 dtype。
- 对重复 opId 去重，同时保持 opId 与 `OpParam` 一一对应。
- Server cache key 同时覆盖算法 tag 和有序 opId 列表。
- 不同配置不得错误复用同一份 Server IR。

本仓以 C++14 构建。不要使用 C++17 `inline thread_local` 变量；使用函数内 `static thread_local` 返回配置对象。

### 4.3 Dispatch IR

`DispatchKfcSubKernel` 运行在 host 的 CCU IR 生成阶段。对配置中的每个 op 生成：

```cpp
CCU_IF(param[OP_ID_INDEX] == opId) {
    TargetCollectiveKernel(/* dynamic params + static resource params */);
}
```

不要把普通运行时 C++ switch 当作设备 dispatch。逐算法核对：

- HBM 参数索引和 client 入队格式。
- kernel 参数顺序。
- token、channels、channelCount、rankSize、rankId。
- dtype、output dtype、reduceType。
- INVALID opId 的退出流程。
- 环形队列游标回绕和 start/end 信号复位。

新增算法时同步更新 KFC template 的允许列表、原始 kernel 名称校验、dispatch include 和 host ST。

## 5. ST 设计

### 5.1 AICPU e2e

按需求生成真实 selector→executor→template→task model 流程，不只调用内部 helper。覆盖：

- 2/3/4/6/8 rank 等代表性规模。
- 小数据、阈值边界和大数据分段。
- 支持的 dtype 和 reduce op。
- task graph、内存冲突和结果语义。
- 确认调用未转发到安装版 HCCL。

删除范围外的 DPU 用例，除非用户明确要求。

### 5.2 CCU host-only

没有 CCU 仿真器时，不伪造 device 通过结论。host ST 至少覆盖：

- selector 只命中目标拓扑。
- tiling 到 `OpParam` 的转换。
- `AlgorithmType` 和算法名映射。
- CCU resource、queue、token 和启动参数。
- opId 字段以及 reduceType 碰撞防护。
- 多 op 配置共享一个 Server。
- 配置变化使 cache 隔离。
- dispatch proxy 选择正确 kernel adapter。
- null、空配置和不支持算法的错误路径。

## 6. 构建与打包

先执行窄目标，再执行完整目标：

```bash
source "$CANN_PATH/set_env.sh"
cmake --build "$DEVKIT_PATH/build" --target mc2_client -- -j4
cmake --build "$DEVKIT_PATH/build" --target <aicpu-st-target> -- -j4
cmake --build "$DEVKIT_PATH/build" --target <ccu-host-st-target> -- -j4
```

执行生成的 GTest 二进制并记录 suite 数、test 数、耗时和退出码。测试输出很大时写入工作区日志，再提取最终 GTest 汇总。

打包前确认 `pkg-config` 可用。优先复用 CANN 安装目录或 `build/makeself` 中已有 makeself，不因网络失败重复下载。

```bash
pkg-config --version
bash "$DEVKIT_PATH/build.sh" --pkg -j4
cd "$DEVKIT_PATH/build" && cpack --verbose
sha256sum "$DEVKIT_PATH/build_out/"*.run
```

若 run 包校验因默认 `$HOME/log/makeself` 不可写失败，把 `HOME` 和 `TMPDIR` 指向可写临时目录；先区分日志权限问题和 archive 校验失败。

## 7. 常见失败模式

| 现象 | 优先检查 |
|---|---|
| selector 已写但从未命中 | `cann_host_bridge`、注册优先级、opType、拓扑门禁 |
| 链接不到新算法 | 各层 CMake、注册名、namespace、`AlgorithmType` |
| Server 运行错误算法 | opId 字段、reduceType、HBM 参数索引、cache hash |
| 多配置复用了旧 IR | cache key 是否覆盖完整有序 opId 列表 |
| C++ 编译报 inline variable | 移除 C++17 `inline thread_local` |
| 目录里有双 die 代码但目标未构建 | 以 CMake/build.make 为准，不以文件存在为准 |
| ST 实际走安装版 HCCL | bridge stub、调用日志和本地 selector 断言 |
| package 卡在 makeself | `pkg-config`、已有 makeself、CPack External 日志 |

## 8. 完成门禁与交付

完成前逐项确认：

- HCCL 新旧版本差异已分类，范围内性能修改已处理。
- selector、executor、template、kernel、CMake 和注册形成闭环。
- 本地 bridge 不会绕过迁移实现。
- KFC Server 不是独立 op，多算子 opId/cache/dispatch 一致。
- AICPU e2e 全部通过。
- CCU host ST 全部通过；明确说明未运行的 device 验证。
- `mc2_client`、device 工程和 run 包构建通过。
- `git diff --check` 通过。
- 交付记录包含源/目标 revision、范围、排除项、命令、结果、产物和 SHA256。
