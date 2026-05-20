# PR 2363 检视意见清单

来源：https://gitcode.com/cann/asc-devkit/pull/2363

| 状态 | 文件 | 检视意见 |
| --- | --- | --- |

| 未解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:187) | “瓶颈从 Case 1 的 Vec bound 转移到了 MTE2/UB 访问竞争”解释存疑；double buffer 后 SIMT 和 MTE2 理论上独立访问。 |
| 未解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:221) | Case 3 的 MTE2 耗时偏高、带宽效率偏低，可能存在切分不合理问题。 |


| 已解决 | [floor_mod.asc](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/floor_mod.asc:33) | 业务相关变量不建议都写成常量；shape、tile 大小等可通过模板参数传递，硬件/平台相关配置再保留常量。 |
| 已解决 | [floor_mod.asc](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/floor_mod.asc:236) | 各 Case 隔离不清，不必都放到一个 `Process` 函数里；建议在 main 下发 kernel 处用 `if constexpr` 区分不同 kernel，或拆分实现。 |
| 已解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:347) | 附录场景没太看懂，也可能没必要展示；最佳实践应直接使用最优切分、用满核数。 |
| c解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:166) | 各 Case 的关键代码展示比较奇怪，没有体现 SIMD 连续搬运；建议展示 `DataCopy()`、SIMT 计算等核心流程。 |
| 已解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:121) | Case 1 的关键代码应放完整 VF 函数，包括 `LoadAlign` / `StoreAlign` 等。 |
| 已解决 | [floor_mod.asc](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/floor_mod.asc:74) | VL 可以直接用函数获取，Reg 编程接口可参考 RegBase 最佳实践实现。 |
| 已解决 | [floor_mod.asc](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/floor_mod.asc:77) | RegBase 实现中寄存器使用较多，建议化简和复用；寄存器数量有限，过多可能影响双发和性能。 |
| 已解决 | [floor_mod.asc](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/floor_mod.asc:104) | `DataCopy` 是否应替换成 `LoadAlign` / `StoreAlign`。 |
| 已解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:22) | 目录结构顺序建议参考 PR 2287 的新顺序调整。 |
| 已解决 | [scripts/gen_data.py](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/scripts/gen_data.py:1) / [scripts/verify_result.py](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/scripts/verify_result.py:1) | 两个 Python 文件缺 License 头。 |
| 已解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:1) | 标题建议改为“SIMT和SIMD混合编程 高性能优化样例”。 |
| 已解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:37) | FloorMod 公式变量调整成 `x`、`y`、`z` 等符号，并补充一句业务背景说明。 |
| 已解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:62) | “理论执行时间”描述有问题，需要调整。 |
| 已解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:208) | Case 3 的代码片段看起来和 Case 0 没区别，需要解释两者差异。 |
| 已解决 | [README.md](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/README.md:267) | 编译说明中默认运行模式应填 `npu`，并统一排查类似描述。 |
| 已解决 | [CMakeLists.txt](/mnt/workspace/z00940346/example/asc-devkit_4916/examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/CMakeLists.txt:34) | `target_link_libraries(... m)` 是否有用；当前已移除无用 `libm` 链接项。 |
