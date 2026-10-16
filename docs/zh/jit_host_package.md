# 算子包交付 JIT 编译资源

## 1. 用途、前提和支持边界

本功能把动态 kernel 对应的编译资料随算子包交付，让安装后的程序能根据 `resource_id` 和 kernel symbol 调用 `aclrtcKernelSpecialization`，对实际参数做常量化并生成静态 kernel。构建算子包时仍执行原动态编译，不提前为所有常量组合生成静态实例。

需要同一版本配套的 CMake 模块、asc_opc、compile_op、asc_compile_exporter 和 ACLRTC。只替换 CMake 文件不够。SDK 原有 exporter 已支持 `.so` 和 `.o` 输出；本改动使用该能力，不增加系统工具或 Python 包依赖。

升级生成器后，应重新编译动态 kernel 并生成 manifest、kernel 桩和资源库，不能只替换 ACLRTC 后继续沿用旧桩。当前生成器在 manifest 重放时为选定入口重新选择 `g_opSystemRunCfg` 的唯一定义方；SK 的等待、通知和禁用 DCCI 能力位由生成桩中的编译宏计算，debug 检查也由宏控制。旧资源中的跨 key 定义条件和固定能力位不会在加载时自动转换。此修复不需要算子添加 `static inline` 变量或修改算法代码。

自定义工程沿用 `npu_op_package` 系列接口。内置仓若使用自己的 OPC 任务生成和打包脚本，需要仓级公共构建适配；升级 SDK 不会自动改写这些脚本。单算子算法代码不因资源打包新增修改。

当前开启功能时只支持一个 package、一个 kernel library，要求启用原源码与 binary 交付，并使用默认 PACKAGE_PATH。关闭时沿用原构建行为。STATIC 表示 host 静态库交付，不表示在打包时把 device kernel 编译成静态实例。

## 2. 用户配置

```cmake
npu_op_package(${vendor_name}
    TYPE RUN
    CONFIG KERNEL_SPEC_MODE Normal
)
```

保留已有 code_gen、host library、kernel library、kernel_sources、package_add。TYPE 可选 RUN、SHARED、STATIC；不新增用户 CMake function。

| 值 | 行为 |
|---|---|
| None，默认 | 不增加 JIT 导出和聚合任务 |
| Normal | manifest 保留 basic 对象的编译信息 |
| SK | 同时保留 basic 和 sk 信息；SK 实际执行兼容性仍需验证 |

如需保留编译临时文件，在原 kernel target 上使用原选项：

```cmake
npu_op_kernel_options(ascendc_kernels ALL OPTIONS --save-temp-files)
```

多芯片继续使用原 `ASCEND_COMPUTE_UNIT` 列表，不需要用户写 foreach。自定义包汇总本次所有芯片为一份资源；内置仓按原流程分别生成芯片包。资源查找只依赖 resource_id，不增加芯片匹配条件；相同 ID 保留任意一份完整资源。

## 3. package 配置如何传到 compile_op

| 阶段 | 实际行为 |
|---|---|
| npu_op_package | 校验 mode，记录 `_ASC_PKG_<包名>_KERNEL_SPEC_MODE` |
| npu_op_package_add | 找到 kernel target，经已有 npu_op_kernel_options 添加 `--kernel-spec` 和自动生成的 `--kernel-spec-dir` |
| ascendc_gen_options.py | 分离 OPC 参数并写入 custom_opc_options.ini，不把它们误传 device 编译器 |
| ascendc_bin_param_build.py | 按原算子、芯片配置生成 asc_opc 命令并追加上述参数 |
| asc_opc | 参数检查后写入 context 的 kernel_spec_mode、kernel_spec_dir |
| compile_op / KernelSpecCompilation | 原动态编译记录命令和 resource_id，生成 manifest、kernel 桩和生成头文件 |
| 原 binary/config target 完成 | 聚合当前 manifest，按 ID 去重，改写安装路径副本并调用 exporter |
| package | RUN 安装 SO；SHARED/STATIC 把 exporter object 链接或归档到最终 package |

一个 manifest 对应一次动态编译资源，其中 `kernels` 可有多个 tiling key 的入口，因此 manifest 数和 kernel 数不必相同。运行时先按 resource_id 定位资源，再按 kernelEntry 选择入口。

当 OPC 开启 `--op_relocatable_kernel_binary=true` 时，忽略 `KERNEL_SPEC_MODE` 的 manifest 生成配置，输出 Info 日志，继续原 relocatable 编译；该任务不生成 manifest，也不要求 manifest 输出目录。包内其他普通动态编译任务仍按配置导出资源。

只记录动态编译资料。后续静态编译结果不回流到打包阶段。聚合工具不扫描动态 ELF，不执行 readelf，不接受 --binary-dir。

### 3.1 内置工程使用脚本副本时的同步要求

内置工程若复制过 SDK 的任务生成脚本，应同步其中的 JIT 选项处理，不再增加另一套执行接口。公共 CMake 沿用工程已有的 add_ops_compile_options，将 --kernel-spec 和 --kernel-spec-dir 写入原配置；最终用户仍只设置一次 KERNEL_SPEC_MODE。

需要同步的内容：

- ascendc_gen_options.py 调用 SDK kernel_spec_options.py 的 split_and_write_options：把两项 OPC 选项写入 custom_opc_options.ini，不传给 device 编译器。
- ascendc_bin_param_build.py 根据原配置文件、算子及芯片调用 shell_options，将结果追加到生成的 OPC 命令。
- 使用 shell 生成 OPC 任务的工程，在对应生成位置调用同一辅助模块；解析失败必须让任务生成失败。
- kernel_spec_options.py 随 SDK 脚本一起同步，保持内容一致；不要为某个仓单独修改模式或作用范围规则。升级时一并同步依赖。

不应整体替换存在仓特有逻辑的任务生成器，只同步上述调用点和依赖。共享辅助模块在脚本相邻目录加载，不新增 Python 包依赖。原执行命令、任务依赖和单算子代码保持原样。

配置要在原 autogen 清理之后写入；有 prepare 子工程的工程也需通过原参数传递方式传入同一模式。配置文件在每次配置时按原流程重建，None 不追加导出参数。生成的 OPC 命令是显式配置依据，不从进程环境继承 JIT 模式。芯片子目录由辅助模块按既有规则追加，复用原芯片编译流程。

源码安装映射、kernel 完成依赖、资源聚合和安装位置仍使用现有实现。save_temp_files 继续读取原 OPC 配置；这次同步不改变资源格式或运行时加载方式。

## 4. 构建依赖和产物位置

```text
原 tiling 前置库 → 原动态 kernel 编译 → manifest + binary/config
                                               ↓
                                      exporter 聚合
                                               ↓
                           RUN 安装 / 最终 SHARED、STATIC 链接
```

资源 object 不加入 tiling 前置库，否则会形成 kernel 编译依赖自己的循环。

| 场景 | 交付位置 | 源码位置 |
|---|---|---|
| RUN 自定义 | vendors/<vendor>/op_spec/libcust_compile_db.so | 原 vendor 源码目录 |
| RUN 内置，需仓适配 | opp/built-in/op_spec/ops_<仓>/libcust_compile_db.so | 原内置仓源码目录 |
| SHARED | 资源 object 链接到最终 package 动态库，无额外 JIT SO | prefix/share/<package>/src |
| STATIC | 资源 object 归档到最终 package 静态库，无额外 JIT SO | prefix/share/<package>/src |

内置仓按仓分目录，避免不同仓的同名 SO 互相覆盖。不同芯片包仍使用既定覆盖安装规则，不承诺同时安装多个芯片版本后合并。

使用显式安装清单的内置仓，还必须在其 XML/filelist 中声明新 SO 和目录；只增加 CMake install 可能仅将文件装进归档，而不会被原 RUN 安装器安装。此项属于算子仓适配，不应写死到 SDK。

## 5. source_file 和头文件

manifest 继续只使用 `source_file`：

- 仅文件名，例如 add.cpp：在所属包允许的源码目录检索，必须唯一；多个匹配报歧义并列出候选，提示使用包内相对路径。
- 带目录，例如 op_impl/ai_core/tbe/impl/ops_cv/ascendc/add/add.cpp：相对于包根精确定位；缺失时报错，不回退猜测。

RUN 自定义包根为 vendors/<vendor>，内置为 opp/built-in；库交付根为安装前缀。不允许绝对路径、父目录跳转或符号链接逃出包根。

RUN、SHARED 根据原源码安装规则传入内部 source-map，在打包副本中改写 source_file，以及有明确目录映射的 include 路径。STATIC 不传自动安装映射，保留原 source_file，不新增 STATIC 专用的路径限制校验；源码布局与检索由使用方组织。compile_op 原始 manifest 始终保留。原源码和头文件复用工程交付；资源中额外保存生成桩、生成头文件和 manifest。默认以选中源文件的父目录作为 include 基础，额外 include 路径仍需编译选项提供，不扫描头文件自动猜测。

`${source_file_path}` 是已有编译命令占位符，表示定位后的源文件目录；不是另一个 manifest 字段。

打包工具复用该占位符，将明确映射的 `-I`、`-isystem`、`-iquote`、`-idirafter` 路径写成相对于源码目录的形式，保持搜索顺序。例如安装后的源码在 `src/op/kernel.cpp`，公共头文件在 `src/common`，则对应 include 写为 `-I${source_file_path}/../common`。目录映射按最长路径前缀匹配，同一前缀对应不同安装位置时报歧义；未映射的外部路径、环境变量和原相对路径保持不变。生成器也不会因目录在生成时不存在而静默删除用户的 include。

内置仓如果把公共头文件复制到构建目录再编译，应在已有内部 source-map 中补充该副本与安装目录的对应关系。它描述公共构建目录，不需要逐算子配置。SDK 不扫描同名头文件猜位置，不改写源码内的相对 include，也不补打包工程缺失的依赖。原 compile_op 使用同一安装包也会失败的缺文件或目录布局问题，由工程原源码交付处理。


## 6. 加载与静态编译

RUN 使用原安装环境，例如 ASCEND_CUSTOM_OPP_PATH 指向 vendor，或 ASCEND_OPP_PATH 指向 opp。ACLRTC 自动发现 op_spec。SHARED/STATIC 的资源由进程已加载或已链接的资源入口提供。

STATIC 用户应使用安装后的 CMake 导出 target，它包含资源符号保留和导出选项。只手写 `-l` 不保证资源 object 被链接器保留。SHARED 库必须被实际加载，不能只把它放在磁盘上。

调用方提供动态二进制对应的 resource_id、实际 kernel symbol，以及参数地址和长度；资源打包不会替调用方获取执行参数。静态编译成功只说明产生 ELF，不代表已执行或数值正确。

## 7. 错误和临时文件

启用导出后，KernelSpecCompilation 在 compile_op 成功退出前检查 manifest 已发布且非空；未发布、写失败或文件为空时，当前任务报错，包含 kernel、mode、输出目录和原因。原编译已失败时保留原错误。None 和不参与导出的任务不要求 manifest。

聚合输入为空、引用源码找不到、同名歧义、exporter 失败都停止构建，不发布不完整 SO/object。manifest 格式由 exporter 检查。相同 ID 不拼接来自不同副本的文件。

原 save-temp-files 对应 dump_cce，聚合读取同一 OPC 配置并传递到 exporter。开启时保留 `.bundle-*` 中的 build.log、暂存 manifest 和 exporter 工作目录；关闭时清理，失败仍打印日志。正式 manifest 不因关闭临时文件保存而删除。

## 8. 回归测试和独立提交

从 SDK 根目录运行无需算子仓的 Python 回归：

```bash
python3 -m unittest discover -s tests/tools/utils/asc_kernel_spec -v
python3 tests/python/asc_op_compiler/test_static_compile_resource_generator.py
```

前者已注册为仓内 `ascendc_pyut_kernel_spec_package`，后者属于原 asc_op_compiler 测试目录。C++ 测试位于原 ACLRTC 和 asc_compile_exporter 测试文件中，使用仓原 runner。

本变更不新增 example。用户在原有算子工程中增加本文的 package 配置即可。SDK 修改不要求同时合并算子仓修改；但内置仓要实际启用功能，仍需完成其公共构建适配。

## 9. 已知限制，不应当作已解决

1. RoiAlignV2 的 key 1 单独静态链接缺少 g_opSystemRunCfg。动态桩只在第一个 key 定义它；动态合并所有 key 时可用，单入口导出未完整处理此依赖。关闭常量化仍失败，说明不是常量化本身的问题。需要生成器保证每个独立链接结果恰好有一份定义；混合核/SK 也需验证。
2. STATIC 打包不自动改写 source_file 为安装相对路径。使用方自行组织源码布局与检索；保留原值不代表运行时新增了任意目录搜索能力，也不新增 STATIC 专用的路径禁止规则。
3. 当前不满足同一进程使用多个含 JIT 资源的 SHARED/STATIC 库的要求，需要补齐后才能声明支持：
   - SHARED：当前只通过进程句柄查找一次 `AscendcGetCompileResourceBundle`。多个全局加载的库只找到其中一个，局部加载的库不在该查找范围内；首次自动加载结果还会被缓存，后续新加载的库不会重新发现。
   - STATIC：不同资源 object 定义同名全局入口。普通归档链接可能只提取第一份资源，强制保留全部资源会发生重复定义；不能通过增加 whole-archive 选项解决。
   - 改进方向：exporter 为每份资源生成唯一链接标识，并处理生成的内部函数重名；CMake 保证每个静态库的资源 object 被保留；运行时逐库发现所有资源，并支持首次查询后新加载的库。具体注册或发现方式仍需设计和验证，不要求算子工程自行枚举资源。
   - 验收至少覆盖两个 SHARED（全局及局部加载）、两个 STATIC、二者混用、查询后再加载新库，以及相同 resource_id 保留任意一份完整资源。
4. RUN 实际安装、覆盖升级、卸载及 SK 上卡数值验证未完成。验证中使用 CMake install 和归档数据提取，不能代替安装生命周期验收。

已验证 AddCustom 三种交付的离线静态编译。ops-cv Ascend910B 全量包有 9 份 manifest、13 个入口，26 次离线编译中 24 次成功，2 次为上述 key 1 错误。不能宣称所有生产算子已经可用。

构建会清理上次的 manifest 集合，避免混入旧任务，但保留已发布的资源 SO/object。重新生成的输入相同时可复用聚合缓存；输入改变时先生成临时产物，成功后替换输出。构建失败仍返回错误，保留旧文件不代表本次构建成功。
