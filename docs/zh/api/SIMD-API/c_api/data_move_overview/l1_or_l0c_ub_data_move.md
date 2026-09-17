# L1/L0C与UB数据搬运

DAV_3510支持L1到UB、UB到L1以及L0C到UB三条片上通路，可用于在Cube侧与Vector侧之间交换中间结果。每个方向属于不同流水和执行范围。

## 接口选择

| 方向 | 推荐接口 | 流水 | 执行范围 |
| --- | --- | --- | --- |
| L1到UB | [asc_copy_l12ub](../cube_datamove/asc_copy_l12ub.md) | PIPE_MTE1 | AIC |
| UB到L1 | [asc_copy_ub2l1](../vector_datamove/asc_copy_ub2l1.md) | PIPE_MTE3 | AIV |
| L0C到UB | [asc_copy_l0c2ub](../cube_datamove/asc_copy_l0c2ub.md) | PIPE_FIX | AIC |

## 使用要点

- 三条通路均为单向接口。DAV_3510不提供UB到L0C的通用搬运。
- L1与UB的架构基线对齐均为32字节，L0C为64字节；具体起始地址、长度和间隔仍以接口文档为准。
- AIC写入UB后由AIV读取，或AIV写入L1后由AIC读取时，需要结合任务调度方式和同步接口保证跨执行单元的数据可见性与执行顺序。
- L0C到UB可能包含格式转换、量化或双目标搬出等能力。启用这些能力时，应按转换后的UB排布和数据量检查边界。
