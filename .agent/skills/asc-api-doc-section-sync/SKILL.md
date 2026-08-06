---
name: asc-api-doc-section-sync
description: 将需求稿、设计稿或临时API文档中的指定二级章节安全同步到asc-devkit正式Markdown API文档。用户要求替换、同步或合并“功能说明”“函数原型”“参数说明”章节，批量更新API说明，或在文档目录迁移后核对分类索引和Markdown链接时使用。
---

# AscendC API文档章节同步

将源文档的指定二级章节替换到正式API文档，默认处理“功能说明”“函数原型”“参数说明”。只修改明确指定的章节，并验证接口事实、目录归属和链接完整性。

## 输入

执行前解析以下信息：

- asc-devkit仓库根目录。
- 源Markdown文档路径。
- 正式API文档路径。若用户给出的旧路径不存在，按文件名在`docs/zh/api`中搜索当前位置；仅允许唯一匹配。
- 待替换章节。未指定时使用“功能说明”“函数原型”“参数说明”。

先读取源文件、目标文件和`git status --short`。保留工作区已有修改，不覆盖用户在目标章节之外的变更。

## 同步流程

1. 对照当前仓库的公开声明、实现和相关样例，确认源文档中的函数名、重载、参数类型和参数含义仍然有效。发现冲突时停止替换并报告证据，不以需求稿覆盖当前接口事实。
2. 预览章节替换：

   ```bash
   python3 .agent/skills/asc-api-doc-section-sync/scripts/sync_sections.py \
     --source <源文档> \
     --target <正式文档>
   ```

3. 检查预览差异仅覆盖指定章节。默认不得修改“产品支持情况”“数据类型”“返回值说明”“流水类型”“约束说明”和“调用示例”。
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
- 检查被替换章节中的相对链接和锚点。源文档相对路径不能未经换算直接带入目标目录。

## 验证

至少执行：

```bash
python3 .agent/skills/asc-api-doc-section-sync/scripts/sync_sections.py \
  --source <源文档> \
  --target <正式文档> \
  --check
MARKDOWN_LINK_CHECK_ONLINE=0 bash scripts/markdown_link_check.sh <正式文档及受影响索引>
git diff --check
find docs/zh/api -xtype l -print
```

`--check`在目标已与源章节一致时返回成功；仍有差异时返回失败。链接检查必须无本地断链。`find`必须无输出，除非仓库本身明确维护且目标有效的软链接。

最后报告替换的章节、正式文档实际路径、是否发生分类迁移，以及各项验证结果。若全仓链接检查受外部URL或工具异常影响，明确区分本地目标文档检查结果与全仓检查结果。
