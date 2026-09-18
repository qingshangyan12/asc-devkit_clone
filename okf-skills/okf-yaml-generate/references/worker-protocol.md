# Worker 与 Resolver 快速协议

## Worker

worker 收到 task 后按以下顺序执行：

1. 先运行 `controller.py worker` 生成 scaffold，不手工重写整份 YAML。
2. 只检查目标 Markdown 当前 front matter、功能说明、函数原型、参数/约束、依赖链接和样例链接。
3. 使用 `rg` 定位，再用短范围读取；没有命中证据时不要读取全文或遍历整个 examples 目录。
4. 只有目标文档明确关联样例或依赖时，才读取对应的一个 README/源码片段。
5. 一次性提交一个 worker-result；不要为不同字段拆分 agent，也不要在结果之外写解释性长文本。

worker-result 必须包含字段、证据和问题；无法确认的字段直接保留空值并记录问题。

## Resolver

主控先执行：

```bash
python3 scripts/controller.py resolve-issues --run-dir <run-dir>
```

然后只把 `resolution-input.json` 的 JSON 内容交给一个 resolver agent，不把 run directory、DevKit 根目录、
worker-result 或其他源文件路径传给它。resolver 不重新读取完整 Markdown、头文件或样例，只根据问题中的
证据摘要给出一次性 decisions。只有证据互相冲突时，主代理才回读冲突位置并把补充摘要传给 resolver。

没有问题时不启动 resolver。resolver 完成后立即运行带 `--decisions` 的 `resolve-issues`，不要让 agent
再次复述文档或 write plan。
