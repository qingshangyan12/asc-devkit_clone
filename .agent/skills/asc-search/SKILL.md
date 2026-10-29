---
name: asc-search
description: Ascend C devkit 的确定性关键词检索入口。输入 API、符号、路径片段或关键词组合，输出检索到的路径。无语义分析、同义词扩展或问答；自然语言接口选型交给 knowledge 仓的 ascendc-devkit-query。
---

# Ascend C 关键词检索

先解析本 Skill 的真实路径定位脚本，不依赖调用方当前工作目录。查询只读已经准备好的索引。

## 准备（安装或显式刷新时）

```bash
python3 <skill>/scripts/keyword_search.py build --root <devkit根> --index <外部缓存>/keywords.sqlite3 --source-id <包SHA256或固定Git提交>
```

索引覆盖 docs、include、examples 和 .agent/skills 的文本；不执行语料中的代码。
索引保存来源标识、内容和检索器指纹；缓存必须在 corpus 之外。
新文件、删除文件或源内容更新后由准备流程重新构建；查询不自动构建或联网。

## 查询

```bash
python3 <skill>/scripts/keyword_search.py search --index <缓存>/keywords.sqlite3 --query "asc_copy_gm2ub" --kind doc -k 5
python3 <skill>/scripts/keyword_search.py search --index <缓存>/keywords.sqlite3 --query "GM UB" --json
```

空白分隔的关键词为 AND，忽略大小写，按原文字面子串匹配路径、标题、摘要和正文。
不纠错、不翻译、不扩展同义词、不推断意图、不澄清问题、不阅读候选形成结论。
支持 `--kind doc|include|example|skill`（可重复）和包内相对路径 `--prefix`。
分数由路径、标题、摘要与正文匹配决定，同分按路径排序，去重后截取前 k 项。
stdout 每行一个 corpus 相对路径；`--json` 为字符串数组，仍只包含路径。
零命中 stdout 为空（JSON 为 []），stderr 为 no_match，退出 0；筛选范围没有文档时 stderr 为 no_coverage，退出 3；错误退出 2。
不能将无命中或无覆盖解释为 API 不支持。

## 结构化兼容接口

既有 `build_index.py` 和 `query.py` 保留三类头派生、架构/版本、依赖、参数、枚举等确定性能力。
`query.py` 默认也只输出路径；其 `--json` 输出路径数组；完整元数据仅保留在内部函数供上层适配器使用。
参数和推导规则见 [索引参考](references/index.md) 与 [推导契约](references/index_derivation_contract.md)。
自然语言的意图提取、多轮查询、依赖条件解释和证据阅读属于上层 ascendc-devkit-query。
