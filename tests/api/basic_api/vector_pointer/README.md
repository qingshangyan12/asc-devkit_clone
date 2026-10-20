# VECTOR Basic API 指针扩展测试

[English](README_en.md)

本贡献对应 ddplys 团队的 2026 年 9 月社区任务 **AscendC Basic_API优化实现(VECTOR矢量接口扩展)**。任务已通过社区验收，本分支用于申请合入 asc-devkit。

范围为 **85 个 API 名称、256 条清单签名、20 个头文件**。`scope.json` 保存接口清单及原始行号；逐签名配置、历史结果和原始日志见下方验收附件。

## 接口行为

每个数据操作数可独立使用裸指针或 `LocalTensor`，保留原 Tensor 调用。VECTOR 操作数使用 `__ubuf__` 指针，Fill 使用 Cube L1 的 `__cbuf__` 指针。调用者须满足原接口的地址空间、对齐、容量、工作空间和同步要求。适配层复用原数值实现，不扩展底层数据类型支持范围。

接口声明位于 `include/basic_api/`，实现及公共适配器 `impl/basic_api/utils/kernel_vector_operand.h` 位于 `impl/basic_api/`。

```cpp
// 按原接口要求分配 TPipe/TBuf 缓冲区并执行同步。
__ubuf__ float* dstPtr = (__ubuf__ float*)dst.GetPhyAddr();
__ubuf__ float* src0Ptr = (__ubuf__ float*)src0.GetPhyAddr();
AscendC::Add(dstPtr, src0, src1, count);
AscendC::Add(dst, src0Ptr, src1, count);
AscendC::Add(dstPtr, src0Ptr, src1, count);
```

设计评审：[Issue #1680](https://gitcode.com/cann/asc-devkit/issues/1680) 和[已合入的设计文档 PR #1611](https://gitcode.com/cann/cann-ops-competitions/pull/1611)。

## 环境要求

设备测试面向 Ascend 950PR / dav-3510。验收提交 `a3313b6ead63f3e8ad4ad2b9e6187c452a2ca375` 使用 CANN 9.2.0-beta.2；更早的开发验证使用 CANN 9.1。请先加载所安装 CANN 的 `set_env.sh`，确保 `ASCEND_HOME_PATH`、`bisheng` 和运行库可用。官方样例还需要 CMake、Python 3 和 NumPy；主机适配测试需要支持 C++17 的 g++。以下命令均在仓库根目录执行。

本合入分支基于主线提交 `d378849769f9ee610af9d0ac38db2b3b6bed3209`。当前合入版本的验证状态、精确提交和新日志见 [PR #6853](https://gitcode.com/cann/asc-devkit/pull/6853)。下方历史验收结果始终对应原验收提交。

## 范围与主机检查

```sh
python3 tests/api/basic_api/vector_pointer/check_scope.py
python3 tests/api/basic_api/vector_pointer/test_host_adapters.py --compiler g++
```

范围检查从 Git 读取基线 `d378849769f9ee610af9d0ac38db2b3b6bed3209`；浅克隆需先获取该历史，也可用 `VECTOR_BASELINE_ROOT` 指定已导出的基线目录。检查比较原 Tensor 声明和实现的代码内容，忽略格式差异，不依赖历史日志。主机测试将适配器提取到临时 CPU 模拟后端，不修改生产头文件，也不替代 NPU 验证。

## 编译与设备测试

```sh
bash tests/api/basic_api/vector_pointer/run_target_headers.sh /tmp/vector-headers
bash tests/api/basic_api/vector_pointer/run_target_negative.sh /tmp/vector-negative
bash tests/api/basic_api/vector_pointer/run_target_regressions.sh /tmp/vector-runtime
bash tests/api/basic_api/vector_pointer/run_cpu_address.sh /tmp/vector-cpu
python3 tests/api/basic_api/vector_pointer/run_signature_extensions.py --output /tmp/vector-signatures
python3 tests/api/basic_api/vector_pointer/run_precision_regressions.py --output /tmp/vector-precision
```

脚本包含编译命令，并在指定目录生成编译及运行日志。八组设备程序位于 `examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic/`。补充签名和精度程序由对应脚本生成后编译。运行时需要设备 0 可用。

| 测试组 | 用途 |
| --- | --- |
| 头文件编译 | 编译目标接口，确认采用本次修改的头文件 |
| 非法操作数 | 一个合法对照，以及只读输出、错误地址空间、元素类型不一致等拒绝用例 |
| 设备回归 | 算术、转换、比较、索引、布局、排序、转置和特殊接口；Tensor/指针排列组合 |
| CPU_DEBUG 地址 | GetAbsAddr 成员和自由函数的 UB/L1 多偏移验证 |
| 补充签名 | 显式元素类型、移位计数、比较掩码和 Select level-0 形式 |
| 精度记录 | 逐用例参数、混合容差匹配率、最大误差及整数精确比较；判据见 precision_metrics.h |

历史验证覆盖代表性合法配置：236 条活动设备签名、2 条 CPU_DEBUG 签名和 18 条不活动旧架构签名，不表示穷尽所有数据类型和配置。对数、除法须使用合法数值域，排序、转置和交织须遵守块大小要求。本任务无额外性能指标，不宣称性能测试结果。

## 官方样例对照

合入验证需准备未修改的基线 `d378849769f9ee610af9d0ac38db2b3b6bed3209`（历史验收使用 `639b2b5c1031188107d23d785d52b935f1b4f13b`）：

```sh
python3 tests/api/basic_api/vector_pointer/run_official_samples.py --root . --baseline /path/to/baseline --output /tmp/vector-official
python3 tests/api/basic_api/vector_pointer/finalize_official_samples.py --output /tmp/vector-official
```

脚本使用官方 CMake、数据生成和校验脚本，对比基线 Tensor、修改后 Tensor 和指针三条路径。三条路径采用相同的编译器兼容调整：显式引用 Basic API 头文件、在需要时初始化 TPipe、移除可选的设备 printf。输入输出哈希用于核对数据一致性。reduce 样例额外检查内置校验的成功信息，因为仅看进程退出码不足以判定通过。请查看每个场景的结果和日志；清单外场景单独记录，不计入清单内通过数。

## 历史验收证据

[完整验收报告及日志](https://raw.gitcode.com/cann/asc-devkit/attachment/uploads/7e5d6aae-c899-4ee3-ae53-b0d6fc1d44cc/VECTOR_Basic_API_acceptance_reports.zip)包含七个必要报告/日志文件、逐签名证据和补充复现脚本。SHA256：`d00ec30d4403edfa6dd2e8ba3731bd1099d528142d2b3ab519176780a096826a`。

原验收提交为 [`a3313b6ead63f3e8ad4ad2b9e6187c452a2ca375`](https://gitcode.com/ddplys/asc-devkit/commit/a3313b6ead63f3e8ad4ad2b9e6187c452a2ca375)，分支为 `community/vector-pointer-sep2026`。环境为 Ascend950PR / dav-3510、Ubuntu 22.04、CANN 9.2.0-beta.2、bisheng 15.0.5。结果为 8 组设备回归的 783 条 PASS、7 组补充签名的 188 条 PASS、728 条精度记录和 945 条泛化精度记录。47 个清单内官方场景三路校验通过，输入一致、输出逐位一致；清单外 `region_proposal_sort` 的基线失败单独记录，不计入通过数。

当前合入分支为 `community/vector-pointer-sep2026-merge`。其复验结果与精确提交见 [PR #6853](https://gitcode.com/cann/asc-devkit/pull/6853)，不能用历史验收结果替代新的运行结果。
