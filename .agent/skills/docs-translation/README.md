# 翻译skill使用方法

1. 将exclude_docs.json和translation_skill.md下载到开源生态空间任意路径。

2. 修改translation_skill.md文件中exclude_docs.json文件的路径，全文搜索并修改：

   exclude_docs.json文件中`excludeDocsPaths`配置信息是可以自己配置的，根据自己仓的情况，排除不需要翻译的非产品文档内容。

3. 在opencode中输入类似如下信息，启动skill翻译任务：

   根据translation_skill.md skill，处理xxx仓，其中xxx为用户Fork到本地的组件仓库。
