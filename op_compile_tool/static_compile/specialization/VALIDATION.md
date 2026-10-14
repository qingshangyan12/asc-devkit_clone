# op_compiler 接入 aclrtcKernelSpecialization：实现与验证报告

日期：2026-10-09。平台：CANN 9.3.0 / Ascend910B4。

## 交付与范围

已修改 `/usr/local/Ascend/cann-9.3.0/python/site-packages/op_compile_tool`；未修改 Ascend 下其他源码/目录。完整工具目录复制到 `/data/hxj/code/2/asc-devkit_0921/op_compile_tool` 供 PR；没有提交或推送 Git。

新增实现集中在 `static_compile/specialization`。原文件只修改 `static_compile/static_compilation.py`（调度与隔离发布接入）和 `op_compiler_parse_args.py`（明确拒绝 AOE/tune 及对应帮助文字）。保留原 CLI、shape 扩展、并发、binary_info、复制、run 打包及 debug_compile 路径。新增每 shape JIT worker，失败回退原 OPC；无新增缓存。

原始工具备份：`/data/hxj/op_specialization_impl_20261009/original_op_compile_tool`。

## 实现与真实接口的对齐

1. `shape_config_adapter` 获取当前 shape、已有动态 ELF/JSON、节点归属的 tiling 文件；测试 context 如存在必须绑定 shape、平台和动态 ELF SHA256。
2. `resource_id_reader` 支持文档的 `.ascend.resource_id`，并支持当前真实 producer 的 `.ascend.meta` type 6、64 字节 resourceId。不是 ELF 哈希。
3. `static_op_tiling.get_static_op_tiling` 严格读取 docs/2.md 的 kernel_entry、九字段和有效 tiling bytes，校验 bin/hex，保留原始扩展字段。
4. `argument_binding` 复用 CANN `_gen_kernel_func_declare_head` 计算完整实际 ABI 顺序，结合 ELF DFX（存在时）交叉校验；不读取 manifest、不要求 devkit 新增 ABI 摘要、不要求生成源码。
5. `specialization_options` 转换已验证默认 SK 选项和 blockDim 宏；未覆盖选项走 OPC。`specialization_request` 与真实 C 头文件匹配，LP64 struct 328 字节、reserved offset 72，持有所有 host buffers 到调用完成。
6. `static_kernel_specialization` 调用实际已安装 `libacl_rtc.so`，aclrtc 自行发现 compile database 并调用编译器；`static_kernel_metadata` 复用 OPC staticKey 实现，生成静态 JSON，验证加载符号、SK_BIND 及四个 SK 函数。
7. `artifact_publisher` 对文件/目录 fsync，同文件系统目录 `renameat2(RENAME_NOREPLACE)`，再 fsync 父目录，禁止覆盖。rename 后若失败，终止打包，不能再次 OPC 回退造成两份可见结果。
8. 最终仍由原 `gen_binary_info_config -> copy_files -> run` 路径交付。每次编译独立日志子目录，明确 JIT 成功/OPC 回退计数。

## 实际发现并修复的 ABI 问题

最初使用 `gen_placeholder` 动态资源时，JIT ELF 生成、runtime 加载及 SK_BIND 解析都成功，但真正 ACLGraph SK 执行出现无效 GM 地址。定位证据为 `/data/hxj/op_specialization_impl_20261009/graph/run_attempt3_placeholder_failure.log` 与 `sk_meta/970879/model_49_1/super_kernel.log`。

原因：静态 ACLNN 调用会省略缺失的可选输入；该动态资源仍保留槽位，导致输出地址后移读错。仅拷贝动态 JSON 的 optional 模式不能修复调用 ABI。

修复：后端拒绝这类 shape/resource 不匹配并回退；本次临时资源按真实缺失可选参数生成 `no_placeholder` 变体。源文件来自已安装 CANN，资源生成/exporter 仅为验收准备，没有加入 op_compiler 的生产执行流程。

| 算子 | 本次完整 argsCount | tiling 槽（从 0 起） | argsBytes 常量长度 | 静态 opParaSize |
|---|---:|---:|---:|---:|
| DynamicQuant | 6 | 5 | 240 | 248 |
| MoeGatingTopKSoftmaxV2 | 7 | 6 | 592 | 600 |

这是匹配所选资源的完整 request，**不是最小 args 数组策略**。JSON 的 `parameters` 是运行时初始化描述，两者本例均为 6 个 null，不能用它推导 request 槽位。

## JIT / OPC 真实产物比较

使用相同两算子静态 shape、tiling、SoC、默认 SK 和 debug 编译配置，分别实际编译。`.o` 和完整 `.json` **不是逐字节/逐值一致**，不把关键字段相同当成完整产物相同。

| 算子 | JIT .o 字节数 | OPC .o 字节数 | 完整 .o / JSON | 共同静态字段 |
|---|---:|---:|---|---|
| DynamicQuant | 23104 | 25368 | 不一致 | 一致 |
| MoeGatingTopKSoftmaxV2 | 210744 | 212472 | 不一致 | 一致 |

共同字段检查包含 `supportInfo`（含 staticKey、当前 inputs/outputs/attrs）、blockDim、workspace、opParaSize、localMemorySize、coreType、magic、taskRation、parameters、platformInfo、supportSuperKernel 等。

两算子的 JSON 差异均为：`feature_list`、`kernelName`、`sha256`、`split_mode`、`sub_operator_kernel_name`、`sub_operator_params`。JIT 保留自身真实符号和实际参数布局，以包含四个 SK 函数及真实 SK_BIND 的单 ELF 交付；OPC 使用自身符号及 split 对象描述。没有伪造相同 SHA、假 split 文件或复制 OPC JSON 冒充 JIT。

逐字段差异、SHA256、完整文件路径：`/data/hxj/op_specialization_impl_20261009/validation/comparison.json`。JIT 产物在各算子 `jit/`，OPC 对照在 `opc/`。

## 验证结果

- 安装目录中的 **23 项单元测试通过**。覆盖真实 C 头文件 ABI/buffer 生命周期、tiling 严格校验、显式 entry/key、shape/资源匹配、编译器参数布局、resourceId、JSON/SK 能力检查、JIT/OPC 回退及原子发布异常。
- 两算子真实 aclrtc + OPC 编译均成功，比较结果如上。
- entry 故意改为缺失值时，两算子均 **JIT 拒绝 → 原 OPC 编译成功**，仅发布一对最终文件。`/data/hxj/op_specialization_impl_20261009/validation/fallback.log`。
- 带完整 CLI 必填项的 AOE/tune 命令均被明确拒绝；`/data/hxj/op_specialization_impl_20261009/validation/mode_rejection.json`。
- 安装后的真实 `op_compiler` 经 npugraph_ex 调用，完成 shape 扩展、并发、JIT、JSON、binary_info、run 包、进程隔离安装、静态 kernel 重选、SK 优化和 NPU 执行。两次后端结果为 **JIT=2、OPC=0**。
- 验收图使用 normal_case 同样的 `MoeGatingTopKSoftmaxV2(k=1024) -> DynamicQuant`，BF16 输入 `[864,2048]`、smooth scales `[1024]`。五个输出均与普通无 SK 执行 **逐值完全相等**。
- 不仅检查返回码：`sk_meta/980721` 的 runtime 日志确认实际选中了 `MoeGatingTopKSoftmaxV2_jit_compact_101031` 和 `DynamicQuant_jit_compact_10` 的四个 SK 函数。

最终执行日志：`/data/hxj/op_specialization_impl_20261009/graph/run.log`；数值结果：`/data/hxj/op_specialization_impl_20261009/graph/numeric_result.json`。
最终包及编译报告：`/data/hxj/op_specialization_impl_20261009/graph/static_kernel_compile_outputs/ts20261009202759618616_pid980721_outputs`。
单测：`/data/hxj/op_specialization_impl_20261009/validation/installed_tests.log`。

## 临时打桩与后续边界

新上游 dump 尚未落地；只在测试 harness 中将原采集 shape 关联到已保存的真实 tiling bytes，并显式填写与临时资源匹配的 entry。shape、平台和资源 SHA256 均校验。最终安装验证未提供生成源码或 ABI 摘要。

临时 JIT/SK 资源和 exporter 数据库在 `/data/hxj/op_specialization_impl_20261009/resources`；`generate_compact.sh`、`package_compact.sh` 提供重建命令。未修改 devkit 的资源生成实现。正式接入仍需上游交付同节点 tiling/entry 和可发现、ABI 匹配的资源；缺少时自动走 OPC，不能声称旧 dump 默认已获得 JIT 覆盖。

首版明确限定两目标算子 / Ascend910B4 / 已验证普通 tiling 与默认 SK 参数。未验证的其他 SoC、算子、OOM/descriptor、非默认 SK/编译选项、特殊初始化与 tiling 条件保留原 OPC 路径。

复验脚本和所有构建/日志/缓存均位于 `/data/hxj/op_specialization_impl_20261009`。工具内 `tests/compare_artifacts.py` 可复用作产物对比；测试资源与大型二进制没有放入 PR 源码。

## 2026-10-10 SK 选项增量

已复用原 ACLGraph parser 和 SK 编译参数生成函数，旧文中“仅默认配置”的限制已扩展为当前已验证配置。31 项单测、两算子各 3 组真实 JIT 编译通过；非默认 DCCI 配置的完整 ACLGraph 回归 JIT=2、OPC=0、5 项输出相等。GE/源码模板及 early-start 能力缺失的路径仍不宣称支持。详见 [增量报告](/data/hxj/ai_change_report/2026_10/op_compiler_SK选项复用_实现与验证_20261010.md)。
