---
name: asc-api-doc-section-sync
description: 将需求稿、设计稿、Basic API文档或临时API文档中的指定二级章节安全同步到asc-devkit正式Markdown API文档。用户要求替换、同步或合并“功能说明”“函数原型”“参数说明”“约束说明”章节，依据Basic API与代码优化C API约束，批量更新API说明，或在文档目录迁移后核对分类索引和Markdown链接时使用。
---

# AscendC API文档章节同步

将源文档的指定二级章节同步到正式API文档，默认处理“功能说明”“函数原型”“参数说明”。“约束说明”必须由用户显式指定，并按语义映射流程处理。只修改明确指定的章节或目录范围，并验证接口事实、目录归属和链接完整性。

## 输入

执行前解析以下信息：

- asc-devkit仓库根目录。
- 源Markdown文档路径。
- 正式API文档路径。若用户给出的旧路径不存在，按文件名在`docs/zh/api`中搜索当前位置；仅允许唯一匹配。
- 待替换章节。未指定时使用“功能说明”“函数原型”“参数说明”。
- 同步“约束说明”时，确认作为基准的API族、目标C API目录和产品/架构范围。用户重复给出同一路径时按一个目录处理，不擅自扩展到名称相近但未指定的API族。

先读取源文件、目标文件和`git status --short`。保留工作区已有修改，不覆盖用户在目标章节之外的变更。

## 同步流程

1. 对照当前仓库的公开声明、实现和相关样例，确认源文档中的函数名、重载、参数类型和参数含义仍然有效。发现冲突时停止替换并报告证据，不以需求稿覆盖当前接口事实。
2. 预览章节替换：

   ```bash
   python3 .agent/skills/asc-api-doc-section-sync/scripts/sync_sections.py \
     --source <源文档> \
     --target <正式文档>
   ```

3. 检查预览差异仅覆盖指定章节。默认不得修改“产品支持情况”“数据类型”“返回值说明”“流水类型”“约束说明”和“调用示例”；用户显式指定“约束说明”时除外。
4. 执行替换：

   ```bash
   python3 .agent/skills/asc-api-doc-section-sync/scripts/sync_sections.py \
     --source <源文档> \
     --target <正式文档> \
     --write
   ```

5. 只替换部分章节时，显式传入章节名：

   ```bash
   python3 .agent/skills/asc-api-doc-section-sync/scripts/sync_sections.py \
     --source <源文档> \
     --target <正式文档> \
     --sections 功能说明 参数说明 \
     --write
   ```

脚本按Markdown二级标题边界解析章节，并忽略代码块中的伪标题。源文件或目标文件缺少章节、章节重复、三个默认章节顺序错误、目标为软链接时必须失败，不得降级为字符串替换。

## 约束说明同步

“约束说明”通常不是一对一复制。一个Basic API可能对应多个C API入口，同一个C API也可能承载连续、2D、3D或带转换的多个模式。先完成语义映射，再编辑目标文档。

1. 按功能、数据通路、源/目的存储位置、模式、参数结构和产品支持建立`Basic API -> C API文档 -> C API声明/实现`映射。不得仅凭文件名相似建立对应关系。
2. 以指定Basic API文档的公开行为为描述基准，以当前仓库的C API声明和目标架构实现确认C API入口、参数名、位域和接口拆分。Basic API与代码冲突时停止并报告，不自行选择一方；Basic API没有直接C API入口时不猜测对应关系。
3. 逐项核对并同步以下约束：
   - 源、目的地址对齐和存储空间要求。
   - 0值语义、NOP（空操作）行为及参数范围。
   - 地址重叠、写入顺序和流水同步要求。
   - 数据通路、转置、稀疏、padding、repeat及步长约束。
   - 配套setter的调用顺序、作用范围和默认值。
   - 产品差异、支持的数据类型和模式限制。
4. Basic API使用结构体参数而C API使用独立参数或寄存器字段时，按语义转换为C API参数名，不直接复制Basic API参数名、类型或范围。C API特有的字段截断等约束仅在声明或实现能够确认时保留。
5. C API将普通搬运、转置搬运或不同Feature Map配置拆为独立入口时，约束必须写入对应入口。例如普通2D入口不能描述为执行转置；3D到L0B Buffer的自动转置要求应只写入对应3D模式。
6. Basic API明确0值为NOP时，C API文档同步描述为“不执行搬运，本接口被视为NOP（空操作）”，不得改写为报错或异常。Basic API的约束为“无”且代码没有额外公开约束时，目标章节写“无”，删除推导出的位域后果。

多源文档需要合并到多个目标文档，或约束需要按C API入口重写时，不使用`sync_sections.py`强行复制；使用`apply_patch`进行受控编辑。只有源、目标“约束说明”能够一对一替换时，才执行：

```bash
python3 .agent/skills/asc-api-doc-section-sync/scripts/sync_sections.py \
  --source <源文档> \
  --target <正式文档> \
  --sections 约束说明 \
  --write
```

### 约束措辞

- 将“按位宽截断处理”统一改为“截断处理”。不要删除后续实际生效值、数据缺失或地址错位等有效结果说明。
- 删除“硬件不报错”及“不报告告警和错误”“不返回警告或错误”“不会产生警告或错误”“不触发异常码”等同义描述。保留“不保证写入顺序”“数据可能不完整”等有效行为，并修复删除后的标点和语句衔接。
- 存储位置的地址对齐描述不使用“端口”。例如将“L1 Buffer端口对齐要求”“Global Memory端口对齐要求”分别写为“L1 Buffer对齐要求”“Global Memory对齐要求”；L0A Buffer、L0B Buffer、L0C Buffer、Unified Buffer（或UB）、GM、BiasTable Buffer、Fixpipe Buffer、L0B MX等同类描述按此处理。
- 将“否则搬运异常”统一改为“否则会导致搬运异常”，使条件和结果之间的因果关系完整。
- 连续的已定义模式使用闭区间描述。例如写为“pad_func_mode取值范围为[0, 8]，未定义值会导致结果异常”，不再枚举未定义值或描述具体硬件异常类型。
- n_burst、len_burst、src_stride、dst_stride、m_step、k_step及其派生stride参数的超范围截断约束写为“参数名取值范围为[下限, 上限]，超出取值范围的值会被截断，导致搬运结果不符合预期”。取值范围必须依据用户指定值、Basic API公开范围及C API声明/实现确定，不得直接用位域宽度推导。删除截断示例、实际生效值、部分数据未搬运和截断后NOP等展开描述；参数为0时的NOP或异常行为作为独立约束保留。
- l2_cache_ctl等离散模式参数只保留合法枚举集合，写为“参数名支持值列表，未定义值会导致结果异常”。若参数表已完整列出合法模式，约束说明无需重复。删除“l2_cache_ctl取值范围为[0, 15]，超出取值范围的值会被截断”等字段范围描述；产品支持差异紧随合法枚举说明保留。
- 若“数据类型”章节已完整说明dtype支持范围，约束说明不再按1字节、2字节、4字节重复罗列类型，也不补充“不支持8字节类型，dtype取值非法触发指令未定义异常”等推导描述。
- 处理保留字段时直接描述“传入非0值不生效”或“行为未定义”，不补充无依据的报错行为。
- 术语替换可在用户指定的整个目标子树内统一执行；Basic API语义合并只修改已建立对应关系的C API文档，不扩展到无对应关系的store方向或相邻API族。

## 目录迁移

章节同步本身不改变文档路径。若工作区已将目标文档迁移到其他分类目录：

1. 修改当前位置的文档，不恢复旧路径，也不自行创建兼容软链接。
2. 从旧分类索引移除条目，并加入新分类索引。例如检查：
   - `cube_data_move/cube_data_move.md`
   - `vector_data_move/vector_data_move.md`
   - `sys_var/system_variables.md`
3. 同步`docs/zh/api/SIMD-API/c_api/c_api.md`和`docs/zh/api/README.md`。
4. 用`rg`确认接口在分类索引中只出现一次，且不存在旧路径残留。

## 文档规范

- 同时使用仓库内`doc-checker`技能检查正式文档；不要在本技能中复制其完整规则。
- 保持二级章节顺序、代码块语言、表格语法、NPU标签、术语和中英文标点符合仓库规范。
- 替换“函数原型”时保留每个有效重载；替换“参数说明”时确保所有原型参数都有说明，且没有无对应原型的遗留参数。
- 修改“约束说明”时检查同一文档内不同模式之间是否矛盾或重复，并确保参数名、配套接口链接与当前C API声明一致。
- 检查被替换章节中的相对链接和锚点。源文档相对路径不能未经换算直接带入目标目录。

## 验证

一对一章节同步时执行：

```bash
python3 .agent/skills/asc-api-doc-section-sync/scripts/sync_sections.py \
  --source <源文档> \
  --target <正式文档> \
  --check
```

多源语义合并时不执行无意义的`--check`，改为逐个审查目标文档差异和映射覆盖。所有场景至少执行：

```bash
git diff -- <正式文档及受影响索引>
MARKDOWN_LINK_CHECK_ONLINE=0 bash scripts/markdown_link_check.sh <正式文档及受影响索引>
git diff --check
find docs/zh/api -xtype l -print
rg -n '按位宽截断处理|硬件不报错|硬件不报告告警和错误|硬件将不返回任何警告或错误|硬件不会产生任何警告或错误|不触发异常码|否则搬运异常|(^|[^[:alnum:]_])(n_burst|len_burst|src_stride|dst_stride|m_step|k_step|l2_cache_ctl|[[:alnum:]_]+_(src|dst)_stride)`?传入>|(pad_func_mode|l2_cache_ctl)取值需为已定义|pad_func_mode传入>|l2_cache_ctl取值范围为\[0, 15\]|dtype仅支持1字节类型|不支持8字节类型.*dtype取值非法触发指令未定义异常|(L1 Buffer|Global Memory|GM|UB|Unified Buffer|L0A Buffer|L0B Buffer|L0C Buffer|BiasTable Buffer|Fixpipe Buffer|L0B MX)端口' \
  <修改目录> --glob '*.md'
```

`--check`在目标已与源章节一致时返回成功；仍有差异时返回失败。链接检查必须无本地断链。`find`必须无输出。

`rg`必须无输出。若仓库原有文本明确不在本次范围内，需在最终报告中列出。

最后报告替换的章节、Basic API到C API的映射依据、正式文档实际路径、是否发生分类迁移，以及各项验证结果。若全仓链接检查受外部URL或工具异常影响，明确区分本地目标文档检查结果与全仓检查结果。
