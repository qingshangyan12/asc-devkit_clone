# Cube指令理论性能汇总<a name="ZH-CN_TOPIC_0000002607728945"></a>

**表 1**  矩阵计算搬入至L0的指令理论性能说明

<a name="table19306524182917"></a>
| 接口 | M/N | K |
| --- | --- | --- |
| Load2D(L1->L0A) | 16 | 16 |
| Load2D(L1->L0A) | 32 | 32 |
| Load2D(L1->L0A) | 64 | 64 |
| Load2D(L1->L0A) | 128 | 128 |
| Load2D(L1->L0B) | 16 | 16 |
| Load2D(L1->L0B) | 32 | 32 |
| Load2D(L1->L0B) | 64 | 64 |
| Load2D(L1->L0B) | 128 | 128 |
| Load3D(L1->L0A) | 32 | 32 |
| Load3D(L1->L0A) | 64 | 64 |
| Load3D(L1->L0A) | 128 | 128 |
| Load3D(L1->L0B) | 32 | 32 |
| Load3D(L1->L0B) | 64 | 64 |
| Load3D(L1->L0B) | 128 | 128 |
| LoadDataWithTranspose(L1->L0A) | 64 | 64 |
| LoadDataWithTranspose(L1->L0A) | 128 | 128 |
| LoadDataWithTranspose(L1->L0B) | 64 | 64 |
| LoadDataWithTranspose(L1->L0B) | 128 | 128 |
| LoadDataWithSparse(L1->L0B) | 64 | 64 |
| LoadDataWithSparse(L1->L0B) | 128 | 128 |

**表 2**  Mmad理论性能计算公式中并行度和k0的取值

<a name="table1877123815211"></a>
| 接口 | 左矩阵A | 右矩阵B | cube<sub>m</sub> | cube<sub>n</sub> | cube<sub>k</sub> | k<sub>0</sub> |
| --- | --- | --- | --- | --- | --- | --- |
| Mmad | int8_t | int8_t | 16 | 16 | 32 | 32 |
| Mmad | half | half | 16 | 16 | 16 | 16 |
| Mmad | float | float | 16 | 16 | 4 | 8 |
| Mmad | bfloat16_t | bfloat16_t | 16 | 16 | 16 | 16 |
| Mmad | int4b_t | int4b_t | 16 | 16 | 64 | 64 |
| MmadWithSparse | int8_t | int8_t | 16 | 16 | 32 | 32 |
