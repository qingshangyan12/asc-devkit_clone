# Vec 侧搬运 Tensor API 样例

本样例面向 `dav-3510`，演示以下四个 Vec 侧 Tensor API 搬运入口：

- `copy_gm_to_ub`：GM 搬入 UB，对应 `asc_copy_gm2ub_align`。
- `copy_ub_to_ub`：UB 内搬运，对应 `asc_copy_ub2ub`。
- `copy_ub_to_gm`：UB 搬出 GM，对应 `asc_copy_ub2gm`。
- `copy_ub_to_l1`：UB 搬入 L1，对应 `asc_copy_ub2l1`。

输入 A 的逻辑布局为 `16 x 32`，每行只搬运前 16 个 `half` 数据。
单行有效数据为 32 字节，行跨度为 64 字节，因此：

- GM 与 UB 之间的高维搬运使用 64 字节的首地址间 `stride`。
- UB 内搬运的源、目的 `gap` 均为 1 个 DataBlock（32 字节）。
- UB 到 L1 搬运的 `src_gap` 为 1 个 DataBlock，紧凑 L1 布局的 `dst_gap` 为 0。

样例内核声明为 `__global__ __vector__`，只在 Vector Core 上执行。
核内异步流水之间使用配对的 `asc_lock`、`asc_unlock` 串联 MTE2、V、MTE3 流水。

纯 Vector 核不能将 L1 内容直接回读到主机，因此 `copy_ub_to_l1` 在样例中用于演示接口调用和流水同步；
`GM -> UB -> UB -> GM` 路径通过 `output.bin` 验证数据结果。

## 编译运行

```bash
python3 scripts/gen_data.py
cmake -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
./build/demo
python3 scripts/verify_result.py
```

运行前需配置 CANN 环境，并保证 `ASCEND_HOME_PATH` 指向对应安装目录。

## 参考资料

- [asc_copy_gm2ub_align（dav-3510）](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md)
- [asc_copy_ub2ub](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_ub2ub.md)
- [asc_copy_ub2gm（dav-3510）](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md)
- [asc_copy_ub2l1](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_ub2l1.md)
