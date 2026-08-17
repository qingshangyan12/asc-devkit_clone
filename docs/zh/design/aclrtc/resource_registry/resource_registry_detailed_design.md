# ResourceRegistry 编译资源注册中心详细设计

> 文档类型：迭代详细设计<br>
> 设计对象：`tools/aclrtc/specialization/resource_registry.cpp`<br>
> 关联文件：`resource_registry.h`、`specialization_compile_types.h`、`ascendc_manifest_abi.h`<br>
> 源码基线：`f9db91e10`（2026-08-15）<br>
> 刷新日期：2026-08-17<br>
> 适用读者：aclrtc/Ascend C 编译链路开发、测试、维护及安全评审人员

## 一、需求描述

### 1.1 背景

aclrtc specialization 编译需要按 `resource_id` 获取与算子源码配套的编译资源，包括结构化 JSON 元数据、资源文件集合以及源码所在目录。资源由构建侧打包进命名为 `lib*_compile_database.so` 的动态库，并通过稳定的 C ABI 导出。

`ResourceRegistry` 位于消费链路中，负责发现和加载资源动态库、校验 ABI 与资源内容、处理多来源资源优先级，并在查询命中时将内存资源物化到进程级临时目录。当前实现每次成功查询都会创建新的隔离目录并更新 `resourceDir`。

### 1.2 处理目标

1. 支持显式路径加载和基于环境变量的自动加载。
2. 安全解析资源动态库，避免符号污染、软链接逃逸、路径穿越和资源量失控。
3. 将 External、Custom、BuiltIn 三类资源隔离存储，并按确定性优先级查询。
4. 加载时先暂存再提交，避免解析失败造成半成品资源进入注册表。
5. 查询时延迟物化资源文件，降低未使用资源的磁盘 I/O 与临时空间占用。
6. 串行化加载流程并保护注册表状态；以条目级互斥锁串行化同一资源的物化过程。
7. 提供明确的状态码和分层日志，便于快速定位发现、加载、校验、冲突和 I/O 问题。

### 1.3 非目标

- 不负责生成资源动态库；资源导出由 `asc_compile_exporter` 等构建侧组件完成。
- 不提供资源卸载、覆盖、热更新或版本仲裁能力。
- 不负责解释 JSON 中除 `resource_id`、`source_file` 外的业务字段。
- 不保证临时物化目录跨进程保留；注册中心析构时会清理该目录。
- 当前实现不消费 `AcCompileResourceExtension` 扩展表。

## 二、需求及场景分析

### 2.1 使用角色与核心场景

| 角色/调用方 | 场景 | 输入 | 预期结果 |
|---|---|---|---|
| aclrtc specialization 初始化流程 | 自动加载已安装资源 | `Load(nullptr)` 或 `Load("")` | 扫描环境配置的 Custom/BuiltIn 根目录并缓存本次状态 |
| 外部资源注入流程 | 加载指定 SO | `Load("/path/libxxx.so")` | 将该 SO 作为 External 资源加载 |
| 外部资源注入流程 | 加载指定目录 | `Load("/path/to/dir")` | 仅扫描目录第一层的 `lib*_compile_database.so` |
| 编译请求处理流程 | 查询编译资源 | `Lookup(resourceId, resource)` | 按优先级返回 JSON、源码目录和已物化资源目录 |
| 并发编译线程 | 同时查询同一资源 | 相同 `resource_id` | 物化过程按条目串行执行；每次调用生成新目录并更新同一 `ResourceData` |
| 运维/测试人员 | 定位加载失败 | 状态码与日志 | 能定位到来源类型、SO、manifest、资源 ID 或路径 |

### 2.2 资源来源与优先级

![资源来源优先级](figures/01_source_priority.png)

*图 1 资源来源优先级：External > Custom > BuiltIn。*

优先级固定为 **External > Custom > BuiltIn**。不同来源允许存在相同 `resource_id`，查询时由优先级决定最终可见资源；同一来源内不允许重复 ID。

### 2.3 加载模式分析

| 模式 | 触发条件 | 搜索范围 | 来源类型 | 重复调用语义 |
|---|---|---|---|---|
| 显式 SO | `directory` 指向普通文件 | 单个文件；拒绝软链接；不强制文件名后缀 | External | 可重复调用；同来源同 ID 冲突 |
| 显式目录 | `directory` 指向目录 | 仅第一层；匹配 `lib*_compile_database.so` | External | 可重复调用 |
| 自动加载 | `directory == nullptr` 或空字符串 | Custom/BuiltIn 根目录递归扫描 | Custom、BuiltIn | 进程内只执行一次，成功或失败状态均缓存 |

自动根目录来源：

- `ASCEND_CUSTOM_OPP_PATH`：按 `:` 切分，每个路径追加 `op_impl/ai_core/tbe/kernel/jit`。
- `ASCEND_OPP_PATH/vendors/config.ini`：读取首个 `load_priority`，按 `,` 切分 vendor 名称，并追加 vendor 对应 JIT 根目录。
- `ASCEND_OPP_PATH/built-in/op_impl/ai_core/tbe/kernel/jit`：作为 BuiltIn 根目录。

### 2.4 成功判据

- 动态库发现顺序、资源优先级和冲突结果可重复、可预测。
- 任一动态库或 manifest 在加载阶段失败时，不提交该次暂存结果。
- 查询成功时返回非空、生命周期稳定的 `ResourceData` 指针。
- 每次查询成功后，`resourceDir` 指向本次新建且已规范化的物化目录。
- 非法 ABI、路径、资源大小、文件数量和重复资源均被拒绝并返回明确状态码。

## 三、方案概述

### 3.1 总体架构

![ResourceRegistry 总体架构](figures/02_architecture.png)

*图 2 ResourceRegistry 总体架构：资源输入、加载校验、进程注册表与查询消费四层。*

核心设计由四段组成：

1. **发现**：将显式输入或环境配置转换为规范化、去重的 `LibrarySpec` 列表。
2. **加载**：局部加载 SO，校验 ABI，深拷贝 JSON 和文件字节，随后立即 `dlclose`。
3. **提交**：先进入 `StageState`，再在注册表锁保护下校验同来源冲突和全局容量后提交。
4. **查询**：按来源优先级查找；每次命中均将内存文件写入新的随机隔离目录并更新返回路径。

### 3.2 类与数据模型

![ResourceRegistry 类与数据模型](figures/03_class_model.png)

*图 3 类与数据模型：展示单例注册中心、事务暂存结构、资源实体及 C ABI 边界。*

关键数据对象：

| 类型 | 职责 | 生命周期/所有权 |
|---|---|---|
| `ResourceRegistry` | 统一加载、存储、优先级查询和临时目录管理 | 函数内静态单例，进程生命周期 |
| `ResourceEntry` | 保存单个资源的元数据、内存文件、来源和物化锁 | 由对应 `ResourceStore` 的 `unique_ptr` 独占 |
| `ResourceData` | 对调用方暴露 JSON、物化目录和源码目录 | 内嵌于 `ResourceEntry`，返回只读指针 |
| `StageState` | 一次 `Load` 的事务暂存区 | 单次调用栈内；失败自动销毁 |
| `StagedResources` | 某一来源的暂存资源及容量/冲突状态 | 隶属于 `StageState` |
| `LibraryHandle` | 管理 `dlopen` 句柄 | RAII；完成深拷贝后自动 `dlclose` |

> `sourceFilePath` 的当前实现值是唯一匹配源码文件的**父目录**，不是包含文件名的完整路径。调用方必须按此现有语义使用。

### 3.3 类对外接口能力

#### 3.3.1 接口总览

| 接口 | 能力 | 输入契约 | 输出契约 | 并发语义 |
|---|---|---|---|---|
| `static ResourceRegistry& Instance()` | 获取进程级注册中心 | 无 | 返回唯一实例引用，不得复制/赋值 | C++11 函数内静态初始化保证首次构造安全 |
| `ResourceStatus Load(const char* directory)` | 发现、加载并提交资源 | `nullptr`/空串表示自动加载；否则必须是可用普通文件或目录 | 返回加载结果；不通过异常传递业务失败 | `loadMutex_` 串行化全部加载；提交时使用 `registryMutex_` |
| `ResourceStatus Lookup(const char* resourceId, const ResourceData*& resource)` | 按 ID 查询并执行一次资源物化 | ID 必须为非空 C 字符串；输出引用有效 | 入口先置 `resource=nullptr`；成功返回稳定地址的只读指针，但其中 `resourceDir` 会被后续查询更新 | 注册表查找受 `registryMutex_` 保护；同一资源的每次物化受独立锁串行化 |

#### 3.3.2 `Load` 详细契约

- 自动模式只尝试一次；首次结果包括失败状态都会缓存在 `automaticLoadStatus_` 中。
- 显式模式每次均重新发现和加载。
- `LoadLibraries` 任一环节失败时不会调用 `Commit`，本次暂存资源整体丢弃。
- `Commit` 以来源类别为提交单元：某类别与已提交资源冲突时跳过该类别；同一次自动加载中的其他无冲突类别仍可提交，最终状态返回 `Conflict`。
- 动态库内容在句柄关闭前被深拷贝到进程内存，注册表不持有 SO 内部地址。

#### 3.3.3 `Lookup` 详细契约

- 查询顺序固定为 External、Custom、BuiltIn。
- 未命中返回 `NotFound`；输入为空返回 `InvalidResource`。
- 每次命中都会创建 `${TMPDIR:-/tmp}/aclrtc-resource-XXXXXX/<source-type>/materialize-<random>/<resource_id>` 并重新写入全部资源文件。
- 物化成功后用本次 canonical 目录覆盖 `resourceDir`。旧的成功物化目录不会立即删除，而是在注册中心析构时随进程级临时根统一清理。
- 返回指针地址在注册中心存活期间保持有效。当前实现仅增量插入、从不删除资源，`unordered_map` 扩容不会移动 `unique_ptr` 指向的 `ResourceEntry`。
- `ResourceData` 内容并非不可变：后续同 ID 查询会更新 `resourceDir`。同 ID 的物化过程虽然串行，但调用返回后的字段读取不再受 `materializeMutex` 保护；当前实现不保证“一个线程读取旧 `resourceDir`、另一线程再次查询并更新它”这一场景的数据竞争安全。

#### 3.3.4 调用示例

```cpp
auto& registry = ResourceRegistry::Instance();
ResourceStatus status = registry.Load(nullptr);  // 自动加载，进程内仅尝试一次

const ResourceData* resource = nullptr;
status = registry.Lookup("MyKernel", resource);
if (status == ResourceStatus::Success) {
    // resource->json: 完整 manifest JSON
    // resource->resourceDir: 按需物化后的资源根目录
    // resource->sourceFilePath: 唯一匹配源码文件的父目录
}
```

### 3.4 动态库发现设计

1. 显式普通文件：拒绝软链接并解析 canonical path；文件名不受 `lib*_compile_database.so` 规则限制。
2. 显式目录：拒绝软链接目录，仅扫描第一层；候选必须是普通文件、非软链接并匹配命名规则。
3. 自动目录：对每个存在的根目录递归扫描；缺失根目录仅记录 DEBUG 日志并跳过。
4. 所有候选使用 canonical path 放入 `std::set`，实现路径去重和字典序确定性。
5. 自动模式先生成 Custom 列表，再生成 BuiltIn 列表。

### 3.5 `Load` 主流程

![Load 主流程](figures/04_load_flow.png)

*图 4 Load 主流程：加载串行化、自动状态缓存、事务暂存及提交。*

### 3.6 ABI 与 Manifest 校验

#### 3.6.1 动态库 ABI

资源 SO 使用 `RTLD_NOW | RTLD_LOCAL` 打开，必须导出：

```c
const AcCompileResourceBundleHeader* AscendcGetCompileResourceBundle(void);
```

加载器执行以下严格校验：

| 字段 | 要求 | 失败状态 |
|---|---|---|
| 导出符号 | 存在且可解析 | `LoadError` |
| getter 返回值 | 非空 | `InvalidResource` |
| `magic` | `AC_COMPILE_RESOURCE_MAGIC` | `InvalidResource` |
| `abiVersion` | `AC_COMPILE_RESOURCE_ABI_VERSION`，当前为 1 | `InvalidResource` |
| `structSize` | 必须等于 `sizeof(AcCompileResourceBundle)` | `InvalidResource` |
| `header.flags` | 必须为 0 | `InvalidResource` |
| manifest 表 | 指针非空，数量为 1..4096 | `InvalidResource` |

`extensions` 和 `extensionCount` 已在 ABI 中预留，但当前加载器不读取扩展内容；新增依赖扩展的能力需要同步升级加载器协议。

#### 3.6.2 Manifest 所有权字段

JSON 必须是对象。所有权字段规则如下：

- `resource_id`：非空纯文件名，不得包含路径分隔或不安全相对路径。
- `source_file`：可选；存在时必须是非空纯文件名。缺失时不执行源码定位，`sourceFilePath` 保持为空。

当 manifest 提供 `source_file` 时，SO 必须位于 `kernel/jit/<soc>/` 目录结构下。加载器以相邻的 `kernel/<soc>` 为搜索根目录递归查找该文件，要求命中且仅命中一个普通文件；最终记录其 canonical 父目录。搜索根目录拒绝软链接，候选软链接不计入匹配，且 canonical 路径必须位于搜索根内。未提供 `source_file` 时不校验上述 SO 相对布局。

#### 3.6.3 资源文件校验

- `fileName` 必须为安全纯文件名。
- `filePath` 必须为安全相对路径，且 basename 必须等于 `fileName`。
- 同一 manifest 内 `filePath` 不得重复。
- 不允许同时出现 `a` 与 `a/b` 这类“文件与子路径”冲突。
- 非空文件必须提供数据指针；数据在 SO 关闭前深拷贝。
- 文件按 `relativePath` 排序后保存，保证确定性物化顺序。

### 3.7 暂存、冲突与提交

`StageState` 将三个来源完全分离。加载期间不触碰已提交注册表；只有所有 SO 和 manifest 均成功解析后才进入 `Commit`。

| 冲突位置 | 行为 | 最终结果 |
|---|---|---|
| 同一次加载、同一来源、相同 `resource_id` | `LoadManifest` 立即失败，本次不提交 | `Conflict` |
| 新暂存资源与已提交的同来源资源重复 | 标记该来源 `conflict=true`，跳过该来源提交 | `Conflict` |
| 不同来源具有相同 `resource_id` | 允许共同存在 | `Success`，查询按 External > Custom > BuiltIn |
| 某来源冲突，其他来源无冲突 | 冲突来源不提交，其他来源可提交 | `Conflict`，存在分类级部分提交 |
| 提交后全局字节数或文件数超限 | 所有类别均不执行 merge | `InvalidResource` |

### 3.8 `Lookup` 与逐次物化时序

![Lookup 与按需物化流程](figures/05_lookup_sequence.png)

*图 5 Lookup 与逐次物化流程：注册表查找、条目级互斥、随机目录、失败清理与路径更新。*

每次查询使用独立的 `materialize-<random>` 目录。物化失败会清理本次随机目录并清空 `resourceDir`；内存中的 JSON 和文件字节仍保留，因此后续查询可以重试。物化成功则保留本次目录，后续查询创建新目录并覆盖 `resourceDir`，旧目录直到注册中心析构时统一清理。

### 3.9 并发与生命周期设计

| 锁/机制 | 保护对象 | 持锁范围 | 设计说明 |
|---|---|---|---|
| `loadMutex_` | 自动加载状态及完整加载流程 | 整个 `Load` | 串行化 SO 发现、加载与提交，避免重复自动加载 |
| `registryMutex_` | 三类 Store、总量计数、`temporaryRoot_` | 查找、提交、临时根创建/读取 | 不在磁盘文件写入期间长期持有 |
| `ResourceEntry::materializeMutex` | 单个资源的物化过程及 `resourceDir` 更新 | 命中后至本次物化完成 | 不同资源可并行物化；同一资源的多次物化串行执行 |
| `LibraryHandle` | SO 句柄 | 单个 `LoadLibrary` | RAII 调用 `dlclose`，关闭失败记录 WARN |
| 单例析构 | 临时根目录 | 进程正常退出 | 调用 `RemoveAll` 清理全部物化文件 |

锁顺序避免内部死锁：`Lookup` 在获取条目指针后先释放 `registryMutex_`，再获取条目物化锁；`Materialize` 持有条目锁时仅短暂获取注册表锁，不存在反向同时持锁路径。该结论仅覆盖注册中心内部临界区，不等价于返回后对可变 `ResourceData::resourceDir` 的无锁并发读取安全。

### 3.10 内部方法职责

| 方法组 | 关键方法 | 职责 |
|---|---|---|
| 根目录解析 | `AutomaticSearchRoots`、`SplitList` | 读取环境变量与 vendor 优先级配置 |
| 动态库发现 | `IsLibraryName`、`AddLibrary`、`CollectLibraries`、`DiscoverLibraries` | 扫描、过滤、canonical 化与去重 |
| ABI 加载 | `LoadLibrary`、`GetBundle` | `dlopen`/`dlsym`、ABI Header 与 manifest 表校验 |
| 通用校验辅助 | `ValidateBundleHeader`、`ValidateManifestTable`、`ParseManifestOwnership`、`ValidateResourceFilePath`、`ValidateResourceFilePayload`、`ValidateManifestFileTable` | 将 ABI、JSON、路径、指针和容量失败拆分为明确根因 |
| Manifest 解析 | `LoadManifest`、`CopyManifestFiles`、`AppendFile` | 编排所有权解析、源码定位、文件校验与数据深拷贝 |
| 源码定位 | `ResolveSourceRoot`、`AddSourceMatch`、`LocateSourceFile` | 在 `kernel/<soc>` 内查找唯一源码并防止路径逃逸 |
| 事务提交 | `SelectStagedResources`、`HasCommitConflict`、`Commit` | 分类暂存、冲突检测、全局上限与合并 |
| 查询物化 | `FindResource`、`PrepareMaterializationRoot`、`Materialize`、`WriteMaterializedFiles` | 优先级查询、随机隔离目录创建、资源写盘和路径更新 |

### 3.11 状态码设计

| 状态码 | 含义 | 典型触发点 | 调用方建议 |
|---|---|---|---|
| `Success` | 操作完成 | 加载提交或查询物化成功 | 继续编译流程 |
| `NotFound` | 未发现动态库或资源 ID 未命中 | 空扫描结果、`Lookup` miss | 按业务决定降级或报错 |
| `Conflict` | 同来源资源 ID 重复 | stage 内重复、与已提交 Store 重复 | 检查重复安装/重复显式加载 |
| `InvalidResource` | 输入或资源结构不满足契约 | ABI、JSON、路径、数量/大小、源码匹配异常 | 视为资源包问题，不应盲目重试 |
| `IoError` | 文件系统操作失败 | 扫描、canonical、创建目录、写文件、清理前置操作 | 检查权限、磁盘、TMPDIR 和文件系统状态 |
| `LoadError` | 动态装载失败 | `dlopen`/`dlsym` | 检查 SO 依赖、格式和导出符号 |
| `InternalError` | 预留内部错误 | 当前实现未主动返回 | 出现时按实现缺陷处理 |

## 四、约束条件

### 4.1 容量与数量限制

| 限制项 | 上限 | 作用范围 |
|---|---:|---|
| Manifest JSON | 8 MiB | 单个 manifest |
| 路径/名称字段 | 4096 Byte | 单个 `AcString` 路径字段 |
| 单个资源文件 | 256 MiB | 单文件 |
| Manifest 文件总字节 | 512 MiB | 单个 manifest |
| 注册表文件总字节 | 1 GiB | 已提交资源与本次待提交有效类别合计 |
| 单个 SO 的 manifest 数 | 4096 | 单个 bundle |
| Manifest 文件数 | 65536 | 单个 manifest |
| 注册表文件数 | 131072 | 已提交资源与本次待提交有效类别合计 |

字节统计仅包含资源文件内容，不包含 JSON 文本、容器开销和临时目录磁盘元数据，因此实际进程内存占用会高于 `resourceBytes_`。

### 4.2 路径与安全约束

- 运行平台需提供 POSIX `dlopen/dlsym/dlclose`、`mkdtemp` 和可写临时文件系统。
- SO 扫描和源码定位均拒绝软链接；所有关键路径进行 canonical 化。
- 资源输出路径必须是安全相对路径，并通过 basename 一致性和父子路径冲突检查。
- `TMPDIR` 为空时使用 `/tmp`；配置值必须能 canonical 化且为目录。
- `resource_id` 同时用作目录名，因此必须是安全纯名称。
- 资源 SO 属于进程内可执行代码。路径校验只能降低误加载和路径逃逸风险，不能替代发布链路的签名、权限和可信来源控制。

### 4.3 行为约束

- 自动加载失败后不会在同一进程内自动重试；环境修复后仍返回首次缓存状态。
- 当前没有卸载接口，已提交资源及内存文件保留至进程退出。
- 每次成功 `Lookup` 都会新增一份物化目录；旧目录不会按资源粒度回收，重复查询会增加临时磁盘占用，直到进程退出统一清理。
- 同来源资源不可覆盖；需要更新时必须启动新进程或采用新的 ID/来源策略。
- 源码文件名在 `kernel/<soc>` 树中必须唯一，否则整个 manifest 无效。
- 自动扫描为递归遍历；资源规模过大会增加启动加载时延。

## 五、兼容性分析

### 5.1 接口兼容性

`ResourceRegistry` 为新增内部 C++ 能力，对外只新增 `Instance`、`Load`、`Lookup`，不修改已有接口。类禁止复制和赋值，调用方只能使用单例引用。

`ResourceData` 以只读指针返回。后续若调整字段含义或对象生命周期，必须同步评估现有调用方；尤其要保持当前 `sourceFilePath` 表示源码父目录的行为，或在版本升级中明确迁移。

### 5.2 ABI 兼容性

- 资源生产侧与消费侧必须使用相同的 `AC_COMPILE_RESOURCE_ABI_VERSION` 和结构体布局。
- 当前采取严格 `structSize == sizeof(AcCompileResourceBundle)` 策略，不支持仅通过追加字段实现前向兼容。
- `header.flags` 仅接受 0；任何需要非零 flag 的新协议都必须先升级加载器。
- 扩展表字段已预留但未消费。当前 producer 即使填充扩展，也不会改变核心加载结果；不能依赖扩展提供必需语义。
- C ABI 导出函数使用 `extern "C"`，避免 C++ 名字改编；SO 内其他符号通过 `RTLD_LOCAL` 隔离。

### 5.3 环境与部署兼容性

- 支持现有 `ASCEND_CUSTOM_OPP_PATH` 多路径语义和 `ASCEND_OPP_PATH/vendors/config.ini` 的 `load_priority` 配置。
- 动态库目录结构必须与 `kernel/jit/<soc>` 约定一致，否则无法反向定位 `kernel/<soc>` 源码树。
- 显式文件加载不要求标准后缀，便于测试或定制部署；目录扫描仍遵循标准命名规则。

## 六、维测分析

### 6.1 运行时拦截

运行时在以下边界进行前置拦截：

- Load 输入：不存在路径、非普通文件/目录、软链接目录或 SO。
- 动态装载：SO 依赖缺失、导出符号缺失、ABI Header 不匹配。
- Manifest：JSON 非法、必需的 `resource_id` 缺失/类型错误、可选 `source_file` 类型错误、ID 或文件名不安全。
- 源码定位：目录结构不符合约定、源码 0 个或多个匹配、软链接或根目录逃逸。
- 文件资源：路径穿越、重复路径、文件/目录前缀冲突、空指针、容量超限。
- 提交：同来源 ID 冲突、注册表总字节或文件数超限。
- 物化：临时目录不可用、目录创建失败、文件打开/写入/收尾失败。

### 6.2 编译时拦截

构建侧 `asc_compile_exporter` 应在生成 SO 前校验 manifest、资源目录和文件安全性，并生成符合 `ascendc_manifest_abi.h` 的静态数据及唯一导出函数。消费侧仍执行完整校验，不能假设所有 SO 均由可信 exporter 生成。

### 6.3 日志设计

| 级别 | 记录内容 | 用途 |
|---|---|---|
| INFO | Load、发现、SO 列表、单 SO、manifest、Commit、Materialize 的开始或成功节点 | 还原主流程、当前阶段与处理规模；失败路径不打印成功信息 |
| DEBUG | 自动根目录、目录扫描、源码命中、文件写入、Lookup 命中/未命中及物化子阶段 | 定位高频细节和非异常环境差异，避免污染 INFO |
| WARN | 可选 vendor 配置不可读、非法 vendor、候选 SO 无法 canonical 化、同来源提交冲突、`dlclose` 或清理失败 | 表示可跳过、分类不提交或回收异常，不伪装为成功 |
| ERROR | 扫描、动态装载、ABI、JSON、路径、容量、I/O 及事务内重复失败 | 输出失败对象、具体原因、期望/实际值、限制值和可读状态名 |

关键日志维度包含 `resource_id`、`source_type`、SO 路径、manifest 索引、候选路径、匹配数量、字节/文件数量和 `status=name(number)`。下层函数记录根因，上层仅补充停止阶段；日志可能包含本地绝对路径，采集与展示需遵守运行环境的信息安全要求。

### 6.4 故障定位路径

![故障定位路径](figures/06_troubleshooting.png)

*图 6 故障定位路径：根据 ResourceStatus 快速缩小排查范围。*

## 七、测试用例

### 7.1 当前自动化回归基线

测试目标为 `ascendc_ut_aclrtc`。源码基线包含 54 个既有 aclrtc 用例和 29 个 `ResourceRegistryTest` 用例，共 83 个。29 个资源注册中心用例已覆盖：

- 自动/显式发现、命名过滤、递归策略、软链接与 canonical 身份检查；
- 源码根解析、唯一源码匹配以及 `source_file` 缺失时的 metadata-only manifest；
- `dlopen/dlsym/dlclose`、ABI Header、manifest/file table、JSON 和路径/容量边界；
- External/Custom/BuiltIn 暂存、同来源冲突、跨来源查询优先级及注册表总量限制；
- 嵌套/空文件写入、I/O 失败、临时根失败、显式端到端加载和自动状态缓存；
- `Lookup` 参数、未命中、每次重新物化及 `Instance()` 单例稳定性。

现有 UT 使用真实临时文件系统验证路径与物化行为，仅对动态装载接口使用 mock。当前自动化用例未覆盖多线程并发、Sanitizer 和大规模性能场景，这些列入后续扩展矩阵，不能表述为当前已验证能力。

### 7.2 功能与异常扩展矩阵

| 编号 | 测试主题 | 场景/输入 | 预期结果 |
|---|---|---|---|
| RR-001 | 单例 | 多线程并发调用 `Instance()` | 返回同一实例地址，无重复构造 |
| RR-002 | 显式 SO | 合法任意文件名 SO | `Success`，按 External 提交 |
| RR-003 | 显式目录 | 第一层含合法/非法后缀及软链接 | 仅加载合法 `lib*_compile_database.so` 普通文件 |
| RR-004 | 显式目录非递归 | 合法 SO 仅位于子目录 | `NotFound` |
| RR-005 | 自动根目录 | Custom、vendor、BuiltIn 均存在 | 正确递归发现并区分来源 |
| RR-006 | 自动加载缓存 | 首次 `NotFound` 后补齐环境再次调用 | 第二次仍返回首次 `NotFound` |
| RR-007 | 自动路径缺失 | 部分自动根目录不存在 | 跳过缺失根，其余根正常加载 |
| RR-008 | SO 装载失败 | 非 ELF、依赖缺失 | `LoadError`，无暂存资源提交 |
| RR-009 | 导出符号缺失 | SO 无 `AscendcGetCompileResourceBundle` | `LoadError` |
| RR-010 | ABI Header | magic/version/size/flags 分别异常 | `InvalidResource` |
| RR-011 | Manifest 表 | 0 个、超过 4096、空指针 | `InvalidResource` |
| RR-012 | JSON | 非法 JSON、非对象、`resource_id` 缺失/类型错误 | `InvalidResource` |
| RR-012A | 可选源码字段 | 缺失 `source_file` | `Success`，`sourceFilePath` 为空且不校验 SO 的 JIT 相对布局 |
| RR-013 | 所有权字段安全 | ID/源码名为空、含 `/`、`..` | `InvalidResource` |
| RR-014 | SO 布局 | 提供 `source_file`，但 SO 不在 `kernel/jit/<soc>` | `InvalidResource` |
| RR-015 | 源码唯一性 | 0 个、1 个、多个同名源码 | 仅 1 个时成功 |
| RR-016 | 源码软链接/逃逸 | 候选或根为软链接 | 拒绝或跳过，不能越过 canonical 根 |
| RR-017 | 文件路径安全 | 绝对路径、`../`、basename 不一致 | `InvalidResource` |
| RR-018 | 文件路径冲突 | 重复路径或同时包含 `a` 与 `a/b` | `InvalidResource` |
| RR-019 | 空文件 | size=0 且 data=nullptr | 合法加载并物化为空文件 |
| RR-020 | 文件大小边界 | 上限值与上限+1 | 上限成功，超限 `InvalidResource` |
| RR-021 | 数量边界 | manifest/file/registry 数量边界 | 边界成功，超限失败且无溢出 |
| RR-022 | stage 原子性 | 第二个 SO/manifest 失败 | 本次 `Load` 的全部暂存资源不提交 |
| RR-023 | 同来源冲突 | stage 内或与已提交 Store 同 ID | `Conflict`，该来源不提交 |
| RR-024 | 跨来源同 ID | External/Custom/BuiltIn 均有相同 ID | 均可提交，Lookup 返回 External |
| RR-025 | 分类级提交 | Custom 冲突、BuiltIn 无冲突 | 返回 `Conflict`，仅 BuiltIn 提交 |
| RR-026 | Lookup 参数 | `nullptr`、空字符串 | `InvalidResource` 且输出指针为空 |
| RR-027 | Lookup 未命中 | 不存在 ID | `NotFound` 且输出指针为空 |
| RR-028 | 逐次物化 | 对同一 ID 连续查询两次，包含嵌套/二进制/空文件 | 两次均成功且目录不同；内容完整，`resourceDir` 指向后一次 canonical 路径 |
| RR-029 | 物化失败重试 | 首次写入失败，修复文件系统后重试 | 首次 `IoError` 并清理，后续可成功 |
| RR-030 | 同资源并发查询 | 多线程 Lookup 同一 ID | 内部物化串行且每次生成独立目录；专项检查返回后读取 `resourceDir` 与后续更新之间的数据竞争风险 |
| RR-031 | 不同资源并发查询 | 多线程查询不同 ID | 可并行物化，无目录串扰 |
| RR-032 | Load/Lookup 并发 | 查询同时提交新资源 | 无数据竞争；旧资源稳定，新资源提交后可见 |
| RR-033 | 生命周期清理 | 单例析构或独立可测封装销毁 | 临时根被递归删除；失败记录 WARN |
| RR-034 | 确定性 | 文件和 SO 枚举顺序随机 | canonical SO 集合及资源文件顺序稳定 |

### 7.3 性能与资源测试

| 编号 | 场景 | 指标/检查点 |
|---|---|---|
| RR-P01 | 接近 4096 manifests 的单 SO 加载 | 加载耗时、峰值 RSS、无整数溢出 |
| RR-P02 | 接近 1 GiB 资源总量 | 上限内成功、超限快速失败、失败后注册表计数不变 |
| RR-P03 | 大型 `kernel/<soc>` 源码树 | 评估每 manifest 递归搜索的累计开销 |
| RR-P04 | 高频重复查询同一资源 | 量化逐次物化的重复 I/O、条目锁竞争和进程级临时目录增长 |
| RR-P05 | 大量不同资源首次并发查询 | 验证条目级锁允许并行，临时目录无相互覆盖 |

### 7.4 安全测试

- 使用软链接、路径穿越、绝对路径、超长字段和父子路径冲突构造恶意 bundle。
- 构造指针/数量不一致、非空 size 配空 data、错误结构大小和未知 flags。
- 验证异常 bundle 不在注册表留下部分状态，不在目标根外创建文件。
- 验证 SO 采用 `RTLD_LOCAL` 后不会将 bundle 内部符号暴露给后续动态装载。
- 验证临时根目录权限与清理行为符合部署安全基线。

### 7.5 验收标准

1. 当前 83 个 `ascendc_ut_aclrtc` 用例全部通过；新增行为必须同步补充对应回归用例。
2. 扩展矩阵中的并发能力在对外宣称前，必须通过 AddressSanitizer、UndefinedBehaviorSanitizer 和 ThreadSanitizer 场景。
3. 边界测试无整数溢出、越界读取、路径逃逸或分类外资源提交。
4. 并发测试无死锁和未声明的数据竞争；逐次物化行为、目录增长及 `resourceDir` 更新语义符合本设计。
5. 性能测试结果满足 aclrtc specialization 初始化与并发编译时延预算；若源码树扫描成为热点，应单独立项引入索引或缓存，不在本需求内扩展。
