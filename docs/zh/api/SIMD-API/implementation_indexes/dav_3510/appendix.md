# 附录：软实现、疑问与不匹配

## A. 低优先级目录不应直接推导 C API

`impl/basic_api/dav_3510` 包含 CPU 调试分支、`TPipe`/队列/LocalTensor 管理、KFC、跨核控制与兼容逻辑。即使在其中看到 CCE 指令，也可能仅处于非 CPU 分支、带条件编译，或被多个高层参数重组。因此这些条目在 JSON 中标为 `candidate`；必须回到公共 API 文档和实际调用点确认。

## B. 软/复合实现

以下基础 API 操作不应仅凭共享 CCE 指令认定为 C API 一对一替换：

| 基础 API 操作族 | 原因 | 处理 |
| --- | --- | --- |
| `Div`、`Sqrt`、`Ln`、`Log`、`Log2`、`Log10` | 基础 Reg 实现含 IEEE 特判、次正规数处理、倒数/牛顿迭代或多次转换；C API 包装可能调用同一主指令但组合不同。 | 以 `composite` 检索，按 dtype 与特殊值要求重新验证。 |
| B64/complex 的 `Add`、`Sub`、`Mul`、`Div`、比较 | 会拆为两个寄存器并加入 `vintlv`/`vdintlv`、比较、选择和进位指令。 | 不可由单个 `asc_add`/`asc_mul` 推导。 |
| `Cast` | 取决于 round、saturate、part mode 及数据类型，可能含多条 `vcvt` 与重排。 | 先匹配具体 `asc_<src>2<dst>_<round>[_sat]`。 |
| 对齐/非对齐 Load/Store、Gather/Scatter | 地址寄存器、后更新、stride、mask 和 B64 分裂会改变实际指令序列。 | 按 C API 精确变体审计。 |

## C. 文档与实现的可用性矛盾

`asc_leakyrelu`：`impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_leakyrelu_impl.h` 存在实现，但当前 C API 文档标示 Ascend 950PR/Ascend 950DT 不支持。以文档的产品支持结论为准，不得因内部实现文件存在而将其列为可用的 `exact` 映射。

## D. Cube 与同步的参数风险

| 操作 | 静态结论 | 风险 |
| --- | --- | --- |
| `Mmad` ↔ `asc_mmad` | 两侧在3510路径直接调用 `mad`。 | Bias、`cmatrixSource`、`cmatrixInitVal`、`disableGemv`、MX和数据类型决定语义，必须逐参数比对。 |
| `LoadData` ↔ `asc_copy_l12l0a`/`asc_copy_l12l0b*` | 两侧调用 `load_cbuf_to_ca/cb`。 | 转置、布局、步长和L0位置不是同一个高层参数模型。 |
| `SetFlag`/`WaitFlag`/`PipeBarrier` ↔ `asc_sync_notify`/`asc_sync_wait`/`asc_sync_pipe` | 两侧分别落到 `set_flag`/`wait_flag`/`pipe_barrier`。 | pipe 对、事件ID与生命周期必须保留；同步 C API 搬运接口还会隐式等待。 |

## E. 尚未建立精确映射的区域

- 基础 API 的对象生命周期、`TPipe`、`TQue`、`TBufPool`、`LocalMemAllocator`、`ListTensorDesc`、KFC 与跨核任务编排；它们不是单一 CCE 指令包装。
- `DataCopyPad`、N维 DMA、图像/卷积数据搬运和部分 Fixpipe 配置；虽然可共享底层搬运/配置指令，但参数编码尚未逐组合归档。
- 原子、缓存、调试、Dump、Print、CPU 调试替代路径以及所有 2201 兼容软实现。

这些区域在后续需要时应新增独立条目，并附上公共文档、声明、目标 dtype/布局与源码行级证据；不要把“C API 实现文件存在”视为产品支持承诺。
