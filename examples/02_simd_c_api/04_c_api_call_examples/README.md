# C API完整调用样例

本目录按`docs/zh/api/SIMD-API/C-API`中的API分类组织最小完整调用样例。每个`.asc`文件包含Device
Kernel、Host侧Runtime调用、输入与golden生成、输出与golden比较和部分数据打印。

当前目录与文档中包含“函数原型”的454个API叶子页一一对应，并保留架构拆分页面的子目录结构。

每个样例首行给出了仓库根目录下可直接执行的编译运行命令，第二行记录目标NPU架构。构建产物默认
写入`/tmp/asc-devkit-c-api-call-examples`，不会污染源码目录。
