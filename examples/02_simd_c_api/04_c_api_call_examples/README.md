# C API完整调用样例

本目录按`docs/zh/api/SIMD-API/C-API`中的API分类组织最小完整调用样例。每个`.asc`文件包含Device
Kernel、Host侧Runtime调用、输入与golden生成、完整结果比较和部分数据打印。

每个样例首行给出了仓库根目录下可直接执行的编译运行命令，第二行记录目标NPU架构。构建产物默认
写入`/tmp/asc-devkit-c-api-call-examples`，不会污染源码目录。
