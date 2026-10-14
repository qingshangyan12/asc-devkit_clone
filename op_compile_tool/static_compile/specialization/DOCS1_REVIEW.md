# docs/1.md 实现复核补充

本次直接依据 `/data/hxj/code/2/docs/1.md` 的 §3、§5、§7—§13、§15—§16 复核实现，不仅依据 v2 详设。

## 本次修正

- §5.2 / §8：args/options/skOptions 数量为 0 时，相应 C 指针显式为 NULL；此前空 ctypes 数组会留下非 NULL 指针。
- §5.2：增加全部 C 字段 offset 的运行前校验，并使用已安装头文件编译 C helper，验证 sizeof/offsetof 与 NULL 约定。
- §5.2 / §12：调用失败增加 resourceId 前 12 位、entry、输出路径；原后端报告仍提供 op type 和日志路径。不记录 tiling 内容或参数地址。
- 测试临时文件统一使用工作目录 TMPDIR，不在安装目录内创建测试构建文件。

先新增回归用例，观察到 Python NULL 检查失败及 C helper 返回 4，再修改实现。修正后 24 项测试全部通过。两目标算子重新调用真实 aclrtc 成功，生成 ELF 与此前 NPU 数值验收版本逐字节一致；共同静态 JSON 字段仍与 OPC 一致。

证据目录：`/data/hxj/op_specialization_impl_20261009/validation/`：

- `docs1_request_red.log`：修复前失败。
- `docs1_tests.log`：24 项测试通过。
- `docs1/DynamicQuant/comparison.json`、`docs1/MoeGatingTopKSoftmaxV2/comparison.json`：真实重编译结果和同前版 JIT ELF 的比较。

## 仍按用户后续明确指令执行的部分

历史 1.md 中“不回退、五算子、ELF 缓存、后端主动 tiling、资源新增摘要”等内容，不覆盖用户后来明确的 JIT 优先/OPC 回退、首批两算子、无新增缓存、上游 tiling 临时打桩、不把 devkit ABI 摘要作为前置条件。

当前 resourceId 位于真实 CANN 9.3 的 `.ascend.meta` type 6，已按源码和实际 ELF 验证，兼容文档描述的独立节。参数布局直接复用 CANN 参数生成函数，符合 §5.6 中已验证固定 ABI 可复用 helper 的路径。静态运行时缺失 optional 槽与资源布局不匹配时拒绝发布；不会修改 JSON 参数数伪装匹配。SK 函数和 SK_BIND 以真实 ELF/runtime 证据验收。

## 不能宣称已完成的 1.md 完整替代条款

- §5.4 / §16.1：非默认 SK 选项、显式 additional_compile_options 的完整解析、覆盖和 basic/SK 作用域矩阵尚未完成 JIT 实现，目前回退 OPC。
- §9.1 / §9.6 / §16.1：OOM/DFX 特殊布局、MC2、特殊初始化/调试 metadata 的完整 JIT 后处理尚未完成，目前保留 OPC；不能把普通两算子的 opParaSize 验证泛化到这些分支。
- §13：当前真实数值验收是两目标算子的默认 SK 场景；不是文档要求的所有配置组合验收。
- §16：尚未通用消费所有可选 specializationInfo 映射；目前使用已有 JSON/ELF 和限定算子的 compiler helper。新资源模式若超出该已验证范围，需要补实现和交叉校验，不能猜测。

上述边界表示当前两算子实现不能被描述为 1.md 全范围 JIT 替代。后续问题处理应引用具体条款、核查对应源码并补回归证据，再更新代码与设计。

注：1.md 顶部链接的 `op_compiler_static_specialization_upstream_tiling_design.md` 在该 docs 目录中不存在；本次没有把这一失效链接当作新的实施依据。
