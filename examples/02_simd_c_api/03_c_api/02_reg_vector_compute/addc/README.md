# asc_addc 三类头示范

本样例主要展示 `asc_addc` 接口，复用 [asc_addc API 文档](../../../../../docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_addc.md) 的调用示例。对两个 `uint32_t` 矢量及输入进位逐元素求和，校验低32位结果和进位输出。

支持 Ascend 950PR/950DT（A5，`__NPU_ARCH__ == 3510`），使用 SIMD VF 编程模型。

三类头分别位于 API 文档的 slim front matter、`include/c_api/reg_compute/compute/reg_arith.h` 的两个 `asc_addc` 声明上方，以及 `addc.asc` 的成对 `override_id` marker。每个源文件使用一对 marker 管理重点 API 的共同范围。

在已配置 CANN 和 bisheng 的环境中执行：

```bash
bisheng addc.asc -o main --npu-arch=dav-3510
./main
```

运行成功时输出 `[Success] asc_addc passed.`。
