# AscendC Basic_API优化实现(VECTOR矢量接口扩展)

2026年9月社区任务，参与团队：ddplys。本任务已完成社区验收，现申请将 VECTOR Basic API 指针扩展合入 asc-devkit。

## 功能与范围

为清单中的 Basic API 增加裸指针调用和各数据操作数独立选择指针或 `LocalTensor` 的能力，保留已有 Tensor 调用方式。普通矢量数据使用 `__ubuf__` 指针，Fill 使用 Cube L1 的 `__cbuf__` 指针。地址空间、对齐、容量和工作区大小遵循原 API 约束，底层数值计算复用现有实现。

范围为85个接口名称、256条签名、20个清单头文件，其中236条为 dav-3510 活动设备签名、2条为 CPU_DEBUG 签名，另18条属于本次设备不活动的旧架构分支。类型支持继承原后端。

- 接口声明：`include/basic_api/`。
- 接口实现：`impl/basic_api/`，公共操作数适配工具为 `impl/basic_api/utils/kernel_vector_operand.h`。
- 调用样例：`examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic/vector_pointer_*.asc`。
- [测试命令、环境及范围限制](tests/api/basic_api/vector_pointer/README.md)。
- [逐签名范围表](tests/api/basic_api/vector_pointer/signature_matrix.csv)。
- [设计评审 Issue #1680](https://gitcode.com/cann/asc-devkit/issues/1680)。
- [已合入的设计文档 PR #1611](https://gitcode.com/cann/cann-ops-competitions/pull/1611)。

## 调用示例

```cpp
// TPipe/TBuf 分配及同步按原 API 要求完成。
__ubuf__ float* dstPtr = (__ubuf__ float*)dst.GetPhyAddr();
__ubuf__ float* src0Ptr = (__ubuf__ float*)src0.GetPhyAddr();
AscendC::Add(dstPtr, src0, src1, count);
AscendC::Add(dst, src0Ptr, src1, count);
AscendC::Add(dstPtr, src0Ptr, src1, count);
```

## 验证与合入基线

验收提交：[`a3313b6ead63f3e8ad4ad2b9e6187c452a2ca375`](https://gitcode.com/ddplys/asc-devkit/commit/a3313b6ead63f3e8ad4ad2b9e6187c452a2ca375)，原验收分支为 `community/vector-pointer-sep2026`。

验收环境为 Ascend950PR / dav-3510、Ubuntu 22.04、CANN 9.2.0-beta.2、bisheng 15.0.5。归档结果包含8组运行测试的783条 PASS 记录、7组补充签名测试的188条 PASS 记录、728条精度记录及945条泛化精度记录；47个清单内官方场景在基线 Tensor、修改后 Tensor、指针三种形式下校验通过且输出逐位一致。清单外 `region_proposal_sort` 的基线失败已单独记录，不计入通过数。无额外性能指标，不声称性能提升。

合入分支为 `community/vector-pointer-sep2026-merge`，基于主分支提交 `d378849769f9ee610af9d0ac38db2b3b6bed3209`。同步时保留主分支新增架构条件和新增 API，以及验收版本的指针重载。

合入分支重新执行范围兼容检查和主机适配测试。上述设备结果对应原验收提交；同步主分支后的版本尚未重新执行 NPU 测试。完整验收报告、原始日志和补充泛化脚本在社区验收交付件中保存。

```sh
python3 tests/api/basic_api/vector_pointer/check_scope.py
python3 tests/api/basic_api/vector_pointer/test_host_adapters.py --compiler g++
```

设备编译、运行和官方样例对照命令见[测试 README](tests/api/basic_api/vector_pointer/README.md)。
