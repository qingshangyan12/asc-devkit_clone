---
name: migrate-hccl-kfc-algorithms
description: 从最新HCCL源仓持续迁移或同步集合通信算法到asc-devkit CC架构，完成AICPU单template/多template算法、CCU/KFC Server、selector/executor/template/kernel、资源注册和算法改名适配，并生成从MC2算子tiling起步的AICPU E2E ST、CCU host-only ST及精确到文件和代码行的LCOV覆盖率报告。用户要求迁移或更新HCCL算法、同步性能优化、删除legacy host op入口、适配多template或KFC机制、补充MC2 ST、分析本地算法未命中或生成相关覆盖率时使用。
---

# HCCL集合通信算法迁移与KFC适配

## 输入与边界

开始前解析并记录：

- `DEVKIT_PATH`：目标asc-devkit仓根目录。
- `HCCL_PATH`：源HCCL仓根目录，优先从workspace发现，常见位置为`../hccl`。
- `CANN_PATH`：CANN安装根目录。
- `SOC_ARCH`：目标SoC和测试架构。
- `SOURCE_REV`：asc-devkit现有代码上一次同步的HCCL revision。
- `OPS`：待迁移或更新的通信操作和算法名。
- `ENGINES`：AICPU、CCU/KFC或两者。
- `TOPO_SCOPE`：单/双die、层数、Mesh/Clos、Jetty范围。
- `TEST_SCOPE`：AICPU E2E、CCU host-only、device、DPU或硬件E2E。
- `EXCLUSIONS`：明确不处理的算法、拓扑和引擎。

从仓库、需求文档和环境变量发现路径，不硬编码个人目录。严格执行`EXCLUSIONS`，不要因为源文件相邻而顺带接入HalfAllToAllV、双die、DPU或其他范围外实现。

若迁移引入新的AscendC公共kernel API、重载、dtype、TPosition或参数组合，先使用仓内`asc-ar-dev`核对`docs/api`、声明、实现和样例。

## 强制更新HCCL源仓

每次使用本Skill执行分析、迁移或增量同步前，必须先更新HCCL仓。先记录更新前revision，再执行非合并式快进更新，最后记录更新后revision：

```bash
git -C "$HCCL_PATH" status --short --branch
git -C "$HCCL_PATH" remote -v
HCCL_REV_BEFORE=$(git -C "$HCCL_PATH" rev-parse HEAD)
git -C "$HCCL_PATH" pull --ff-only
HCCL_REV_AFTER=$(git -C "$HCCL_PATH" rev-parse HEAD)
git -C "$HCCL_PATH" log -1 --oneline
```

网络受限时请求执行`git pull --ff-only`所需权限。HCCL工作树存在本地修改时，不自动stash、reset或覆盖；先保留现场并解决更新阻塞。更新失败时不得声称基于最新HCCL，也不得继续提交源同步结果。

同时记录asc-devkit的分支、HEAD和dirty状态。后续差异分析使用已记录的`SOURCE_REV..HCCL_REV_AFTER`，不要把“更新前HEAD”自动当作历史同步基线。

## 建立迁移基线

1. 读取需求、迁移记录和当前ST设计，确认引擎、拓扑、dtype、阈值、性能优化及排除项。
2. 选择asc-devkit中已经迁移成功且结构最接近的算法作为样板。
3. 为每个算法建立source-to-target清单：selector、executor、template、kernel、公共资源、注册、CMake、ST和覆盖率文件。
4. 对比`SOURCE_REV..HCCL_REV_AFTER`，逐提交识别新功能、性能优化、重命名和公共框架变化，不整体覆盖目录。
5. 检查`cann_host_bridge`、动态符号和安装版`libhccl.so`，证明调用不会绕开asc-devkit本地实现。

```bash
git -C "$HCCL_PATH" diff --name-status "$SOURCE_REV..$HCCL_REV_AFTER" -- src/ops/<op>
git -C "$HCCL_PATH" log --oneline "$SOURCE_REV..$HCCL_REV_AFTER" -- src/ops/<op>
rg -n '<OldAlgName>|<NewAlgName>|REGISTER_EXEC|REGISTER_SELECTOR' "$HCCL_PATH/src/ops/<op>"
```

把差异分为必须同步的目标算法变化、需要适配的公共框架变化、范围外变化和纯格式变化。HCCL中的cache、分段、channel请求、loop/tail及性能比例优化属于算法行为，不能因asc-devkit已有旧实现而跳过。

不要只对比`src/ops/<op>`。最新HCCL可能同时修改`src/common/alg_parse.*`、`src/ops/op_common/selector/`、executor/template基类和注册宏。算法新增`CalcCostCoeff`、`GetAlgNetMeta`、名称映射或cost table依赖时，将这些文件加入依赖闭包；先判断asc-devkit是否已经具备对应公共框架，再决定完整迁移还是显式适配，禁止复制一半接口形成不可达或失真的选择链。

## 同步算法名称

以更新后的HCCL名称为canonical名称。HCCL若重命名AICPU或CCU算法，asc-devkit迁移结果必须同步改名，不保留旧迁移名伪装成兼容层，除非存在明确的外部ABI合同。

同步检查以下位置：

- selector返回字符串和forced `algConfig`。
- `REGISTER_EXEC_V2`、`REGISTER_EXECUTOR_BY_TWO_TEMPS`等注册宏。
- `AlgorithmType`、`algorithmMap`、KFC允许列表和dispatch映射。
- executor/template/kernel类名、文件名、CMake源文件列表和include。
- ST期望算法名、过滤器、日志、文档和覆盖率文件清单。

完成后对旧名执行全仓`rg`。旧名仅可出现在明确解释历史映射的迁移文档中；生产注册、测试断言或CMake中残留旧名均视为迁移未完成。

## 清理legacy host op入口

HCCL仓中的`<op>_op.h`和`<op>_op.cc`属于源仓host侧独立op入口，不是asc-devkit MC2算法核心。目标算法已经由MC2 tiling入口承载时，不要迁移这些文件；asc-devkit若留有该算法的旧入口，则删除目标仓中的文件及其CMake、include、声明和ST引用。例如AllGather迁移不得重新引入：

```text
src/ops/all_gather/all_gather_op.h
src/ops/all_gather/all_gather_op.cc
```

执行以下审计：

```bash
rg -n '<op>_op\.h|<op>_op\.cc|Hccl<OpName>' "$DEVKIT_PATH/impl" "$DEVKIT_PATH/tests"
rg -n '<op>_op\.cc' "$DEVKIT_PATH/impl/adv_api/detail/hccl/cc" -g 'CMakeLists.txt'
```

只删除asc-devkit目标算法已被MC2流程取代的legacy入口，不删除HCCL源仓文件，也不要机械删除仍被最新asc-devkit构建依赖的其他操作入口。删除前沿CMake和符号引用闭环确认影响；删除后必须刷新ST，使其从MC2 tiling起步。

禁止继续以已删除的`<op>_op.h`、`HcclAllGather`等host接口作为新ST入口。存在这种调用时，即使算法用例通过，也不能认定MC2迁移完成。

## 分层迁移

### Selector

保持selector与最新HCCL的决策顺序和算法名一致，并按目标范围校验：

- `topoLevelNums`、`level0Topo`和`level0MeshType`。
- rank数、dtype、reduce op、数据量阈值和输入输出overlap。
- AICPU/CCU引擎能力、单/双die及Jetty约束。
- forced algorithm、fallback和未注册算法错误路径。

不要留下可达但未构建的算法名。用ST中的实际launch算法名或注册表断言证明本地selector命中，不能仅用源码存在或编译成功作为证据。

最新HCCL若启用`SelectorEngine`、`AlgoNameMapper`或cost model，额外检查：

- op是否位于新selector白名单，实际入口走静态selector还是cost model。
- 算法注册是否进入`AllAlgos`，名称是否能拆解为engine、executor和template维度。
- executor的`CalcCostCoeff`与template顺序、数据比例和网络类型是否一致。
- `GetAlgNetMeta`中的`netTypes`、`groupSizes`和`CostAggMode`是否表达并行或串行语义。
- auto选择ST和forced algorithm ST是否分开，避免forced算法掩盖cost model未接入。

asc-devkit尚未具备上游新selector公共框架时，记录明确差异和风险，保留本地可验证的selector入口，并迁移不依赖该框架的数据面增强；不要伪造cost model已生效。若需求要求选择逻辑完全一致，则先迁移公共selector依赖闭包，再迁移具体算法。

### Executor与template

保持selector名称、注册名称、`AlgorithmType`和映射完全一致。同步`CalcRes`、`Orchestrate`、`KernelRun`、fast launch、channel请求、资源序列化、cache、数据切分、loop和tail逻辑。

AICPU template必须保留LocalCopy、Read、Write、Reduce、Post、Wait及thread间依赖语义，使ST能够生成全局任务图。CCU实现必须分开审计host资源/IR生成和device kernel本体。

### AICPU多template算法

把`REGISTER_EXECUTOR_BY_TWO_TEMPS`或同类注册视为多template算法。多template通常仍是一次MC2 AICPU入口launch，由一个executor协调两套或多套template资源和任务，不要误实现成多个互不关联的AICPU入口kernel。

迁移多template算法时执行：

1. 按HCCL注册顺序保留template类型、拓扑匹配器和算法名。
2. 在`CalcRes`中分别调用每个template的资源计算，保存独立thread、notify和channel请求。
3. 按HCCL公式合并资源请求，保证总量、顺序和序列化布局确定；不要用硬编码下标猜测资源边界。
4. 在`Orchestrate`中按与`CalcRes`相同的布局拆回各template资源，分别恢复主thread、子thread、notify和channel。
5. 保留`PreSyncInterThreads`、template执行和`PostSyncInterThreads`的依赖闭环；HCCL并行执行时不得改成无依赖串行替代。
6. 原样同步数据比例、buffer offset、scratch切分、loop和尾块计算。例如`AicpuAllGatherConcurMeshNHR`的Mesh/NHR比例来自executor中的37:25常量和`GetParallelDataSplit`，不是ST自定义比例。
7. 上游存在cost model时，同步每个template的成本系数、网络类型、template分组和聚合模式。并行template通常按`MAX`聚合关键路径，串行template通常按`SUM`聚合；以HCCL源码为准，不根据函数名猜测。
8. 同步每个template的channel请求公式和多Jetty逻辑。若迭代只允许第一阶段Jetty约束，在selector或资源门禁中显式限制，同时保持算法主体尽量与HCCL一致。
9. ST除集合通信语义外，还要断言目标算法名、活跃thread/queue、template任务类型及数据切分边界；仅断言返回成功不足以证明多template生效。

名称必须以更新后的HCCL为准。本次迁移中的历史名`InsAllGatherConcurrentMesh1DNHR`已经更新为`AicpuAllGatherConcurMeshNHR`；迁移经验应复用其多template方法，不应继续复用历史算法名。

以当前AllGather实现为参考：

```text
impl/adv_api/detail/hccl/cc/src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc
impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc
impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc
```

### CMake与注册

逐层检查op、selector、executor、template、kernel和测试CMake。使用生成的`build.make`或`DependInfo.cmake`反查真实编译单元；文件位于源码或安装目录不表示已进入`mc2_client`或ST目标。

## CCU与KFC Server适配

把KFC Server视为算法末端CCU kernel的常驻替代物，不注册成独立HCCL op：

1. 算法template先计算原始CCU kernel资源。
2. KFC template继承原始kernel的channel、rank、参数和资源。
3. 用KFC Server kernel替换目标末端kernel。
4. client通过共享队列提交opId和动态参数。
5. Server根据opId进入预生成的CCU IR分支。

维护多算子配置时保证：

- opId编码`opType`、输入dtype和输出dtype；AllReduce、ReduceScatter额外编码`reduceType`。
- AllToAll系列从send/recv descriptor读取dtype。
- 重复opId去重，opId与`OpParam`保持一一对应。
- Server cache key覆盖算法tag和有序opId列表，配置变化不得复用旧IR。
- `DispatchKfcSubKernel`使用`CCU_IF`生成设备dispatch IR，而不是普通运行时C++ switch。
- client参数索引与kernel参数顺序、token、channel、rank和buffer offset完全一致。

新增或改名算法时同步KFC允许列表、原始kernel名称校验、dispatch include、adapter和host ST。本仓生产代码按当前CMake标准编译，避免引入目标标准不支持的语言特性。

## AICPU MC2 E2E ST

### 强制入口

新AICPU算法ST必须从MC2算子tiling开始，不允许从legacy host op或直接调用selector/executor代替E2E。参考当前实现：

```text
tests/api/adv_api/hccl/cc/st/algorithm/testcase/all_gather_aicpu_testcase.cc
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/hccl_proxy/aclrt_stub.cc
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/sim_world/
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/hccl_verifier/
impl/adv_api/detail/hccl/cc/src/common/hccl_mc2.cc
impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc
impl/adv_api/detail/hccl/cc/src/ops/op_common/template/aicpu/kernel_launch.cc
```

### E2E构造步骤

1. 在算法testcase中构造连续存储的`Mc2InitTilingInner`和一个或多个`Mc2CcTilingInner`，正确填写version、数量和offset。
2. 填写`commEngine=AICPU_TS`、opType、输入输出dtype、groupName和可选强制算法名。
3. 使用`TopoMeta`表达super pod、server和device布局；为每个rank创建独立host线程、device、stream和comm。
4. 使用输入输出标记分配buffer，让验证器识别内存角色。
5. 调用产品`HcclAllocComResourceByTiling`，真实执行tiling解析、selector、executor `CalcRes`和`OpResCtx`生成。
6. 从`OpResCtx.algInfo[index].opParam`读取静态`OpParam`，只补真实MC2算子kernel在launch期提供的comm、stream、buffer、count、dtype和size。
7. 实现`SimulateMc2<Op>Launch`类测试适配函数：取得AICPU主thread，导出thread，提交启动notify，构造ACL kernel参数，调用`aclrtLaunchKernelWithConfig`，再等待完成notify。
8. 让ACL Runtime桩把普通入口转发到产品`HcclLaunchAicpuKernel`，open模式转发到`LaunchOpenOpParamDataImpl`；禁止空桩直接返回成功。
9. 让Hcomm桩把算法通信和同步转换为`SimTaskQueue`任务。所有rank结束后生成全局任务图，执行单任务、内存冲突和对应集合通信语义检查。
10. 断言每个rank返回值、实际launch算法名和最终语义；多template算法增加thread/queue、template任务和切分边界断言。

`SimulateMc2<Op>Launch`只是ST侧对MC2算子kernel控制面职责的替身，不负责selector、资源计算或最终语义校验。普通入口必须形成“算子启动Post、AICPU启动Wait、算法任务、AICPU完成Post、算子完成Wait”的依赖闭环。open入口若因常驻Server生命周期无法捕获完整初始wait，必须把入口可达与完整任务图语义拆成互补用例，不得扩大结论。

覆盖阈值前后、2次幂和非2次幂rank、单loop/多loop、整块/尾块、dtype、overlap、forced algorithm及错误参数。确认bridge没有转发到安装版HCCL。

## CCU host ST

没有CCU仿真器时只验证host侧，不伪造device或硬件E2E通过。参考当前机制：

```text
tests/api/adv_api/hccl/cc/st/algorithm/testcase/mc2_ccu/CMakeLists.txt
tests/api/adv_api/hccl/cc/st/algorithm/testcase/mc2_ccu/host/
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/mc2_ccu/
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/hccl_proxy/hcomm_ccu_stub.cc
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/sim_world/ccu_kfc_sim_world.cc
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/sim_world/ccu_sim_world.cc
```

CCU host ST至少覆盖：

- MC2 tiling到`OpParam`和`OpResCtx`的转换。
- workspace、scratch、XN、CKE、queue、token和启动参数。
- opType、`AlgorithmType`、`isKfc`及算法名映射。
- dtype、输出dtype和reduceType参与opId，避免配置碰撞。
- 单配置、多配置、重复配置和顺序变化下的Server共享与cache隔离。
- `kfcServerArgs`、channel、rank和kernel注册参数。
- null、空tiling、混合engine、未注册算法和不支持op错误路径。
- 生产selector/executor与测试固定映射桩之间的真实性边界。

检查父级CMake是否真正`add_subdirectory(mc2_ccu)`并定义host target。目录存在但目标未接入不算测试可用。device目录存在也不能描述为已执行；设备IR和数据语义必须留待CCU硬件验证。

## 覆盖率报告

新增或刷新ST后必须生成本次相关用例的覆盖率报告。AICPU算法和CCU host分别清零、运行和采集，禁止复用混合计数。

生产静态库必须显式加入coverage插桩。仅给测试对象设置`ENABLE_GCOV`不能证明生产算法覆盖率：

```bash
cmake -S "$DEVKIT_PATH" -B "$DEVKIT_PATH/build" \
  -DENABLE_GCOV=ON \
  -DCMAKE_CXX_FLAGS=--coverage

lcov --zerocounters --directory "$DEVKIT_PATH/build" \
  --ignore-errors mismatch,source,inconsistent,negative

"$TEST_BIN" --gtest_filter="$GTEST_FILTER"

lcov --capture --directory "$DEVKIT_PATH/build" \
  --output-file "$COV_DIR/raw.info" \
  --ignore-errors mismatch,source,inconsistent,negative

lcov --extract "$COV_DIR/raw.info" \
  "$DEVKIT_PATH/impl/adv_api/detail/hccl/cc/src/ops/<op>/*" \
  "$DEVKIT_PATH/impl/adv_api/detail/hccl/cc/src/common/hccl_mc2.cc" \
  "$DEVKIT_PATH/impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc" \
  --output-file "$COV_DIR/coverage.info" \
  --ignore-errors unused

genhtml "$COV_DIR/coverage.info" --show-details --legend \
  --output-directory "$COV_DIR/html"
```

只把本次相关asc-devkit生产文件纳入主指标，排除测试文件、桩、第三方库和无关算法。根据最终`coverage.info`的`SF`和`DA:<line>,<hits>`生成`line_coverage.tsv`，至少包含状态、仓内文件、准确行号和命中次数：

```bash
awk -F'[:,]' '
  /^SF:/ { file = substr($0, 4) }
  /^DA:/ { print ($3 > 0 ? "covered" : "uncovered") "\t" file "\t" $2 "\t" $3 }
' "$COV_DIR/coverage.info" > "$COV_DIR/line_coverage.tsv"
```

每个数据集交付：

- `coverage.info`：可复现的LCOV数据。
- `html/index.html`：逐文件逐行源码报告。
- `line_coverage.tsv`：精确到文件和代码行的命中清单。
- `test.log`：过滤后用例数量、通过数和退出码。
- `coverage_summary.txt`：逐文件及合计的行覆盖率、函数覆盖率、过滤器、revision和未覆盖说明。

覆盖率报告必须列出0%或低覆盖的目标文件和关键未覆盖分支。代码行执行过不等于行为正确，覆盖率不能替代算法名、任务图、配置字段和集合通信语义断言。

## 构建、打包与交付

先执行窄目标和过滤用例，再执行完整目标：

```bash
source "$CANN_PATH/set_env.sh"
cmake --build "$DEVKIT_PATH/build" --target mc2_client -- -j4
cmake --build "$DEVKIT_PATH/build" --target <aicpu-st-target> -- -j4
cmake --build "$DEVKIT_PATH/build" --target <ccu-host-st-target> -- -j4
```

执行实际GTest二进制并记录suite数、test数、耗时和退出码。打包前确认`pkg-config`和CANN目录中已有的makeself可用，再运行仓库打包入口并记录run包SHA256。

完成前确认：

- 已记录HCCL更新前后revision，且迁移基于更新后的HEAD。
- HCCL新旧差异已分类，目标范围内性能和cache优化已同步。
- HCCL算法改名已传播到生产、注册、CMake、ST和文档，旧名无非预期残留。
- 目标算法的legacy host op及引用已清理，ST不再从host接口起步。
- selector、executor、单/多template、kernel、CMake和注册形成闭环。
- AICPU MC2 E2E从tiling起步并全部通过，多template有额外结构断言。
- CCU host ST全部通过，并明确列出未执行的device和硬件验证。
- AICPU和CCU覆盖率分别生成，能够下钻到文件和准确代码行。
- `mc2_client`、相关ST、完整构建和要求范围内的run包通过。
- `git diff --check`通过，未覆盖用户已有修改。
- 交付记录包含source/target revision、范围、排除项、命令、结果、产物路径、覆盖率和SHA256。

## 常见失败模式

| 现象 | 优先检查 |
| --- | --- |
| 声称同步最新但差异遗漏 | 是否先成功执行HCCL `git pull --ff-only`并记录revision |
| selector已写但从未命中 | bridge、注册优先级、opType、拓扑门禁和forced `algConfig` |
| 算法仍显示旧名称 | selector字符串、注册宏、枚举、KFC映射、ST断言和CMake |
| ST通过但没有进入MC2 | 是否仍调用`*_op.h/cc`或直接调用selector/executor |
| 多template只有一侧任务 | `CalcRes`合并、资源拆分边界、前后同步和第二template参数 |
| 多template数据范围错误 | HCCL数据比例、offset、scratch、loop和tail是否原样同步 |
| 链接不到新算法 | 各层CMake、注册名、namespace和`AlgorithmType` |
| Server运行错误算法 | opId字段、reduceType、参数索引、kernel参数顺序和cache key |
| CCU host目标不存在 | 父级CMake target变量和`add_subdirectory(mc2_ccu)` |
| ST实际走安装版HCCL | bridge桩、launch算法名和本地registry断言 |
| 覆盖率只显示测试文件 | 生产静态库是否加入`--coverage`，是否提取正确生产路径 |
| 覆盖率被其他用例污染 | 每个过滤器运行前是否执行`lcov --zerocounters` |
| package卡在makeself | `pkg-config`、CANN已有makeself和CPack日志权限 |
