# tensor_api 数据类型校验与底层 c_api 支持范围差异表

仅核对 `__NPU_ARCH__ == 3510` 段；类型对按 `dst → src` 顺序记录。

| Tensor API 接口 | Check 函数 | 对应 c_api | 前端校验的数据类型组合 | 后端支持的数据类型组合 | 差异 |
| --- | --- | --- | --- | --- | --- |
| CopyGM2L1（AlignV2 ND 路径） | CheckGm2L1AlignV2NDDataType | asc_copy_gm2l1_align | `__cbuf__ × __gm__` 同型：`half / bfloat16_t / float / int8 / uint8 / int16 / uint16 / int32 / uint32 / int64 / uint64 / fp4x2_e1m2_t / fp4x2_e2m1_t / fp8_e5m2_t / fp8_e4m3fn_t / hifloat8_t` | `__cbuf__ × __gm__` 同型：`bfloat16_t / half / float / hifloat8_t / int8 / uint8 / int16 / uint16 / int32 / uint32` | 前端过宽：多 `int64 / uint64 / fp4x2_e1m2 / fp4x2_e2m1 / fp8_e5m2 / fp8_e4m3fn`，后端无对应重载 |
| CopyGM2L1（Multi ND2Nz / DN2Nz 路径） | CheckGm2L1DataType | asc_copy_gm2l1_nd2nz / asc_copy_gm2l1_dn2nz | 同型：`half / bfloat16_t / float / int8 / uint8 / int16 / uint16 / int32 / uint32 / fp8_e5m2_t / fp8_e4m3fn_t / hifloat8_t` | 同型：`half / bfloat16_t / float / int8 / uint8 / int16 / uint16 / int32 / uint32 / hifloat8_t / float8_e5m2_t / float8_e4m3_t` | 类型名不一致：前端 `fp8_e4m3fn_t` vs 后端 `float8_e4m3_t`、前端 `fp8_e5m2_t` vs 后端 `float8_e5m2_t`（需确认是否别名） |
| CopyGM2L1（Fp4 路径） | CheckGm2L1Fp4DataType | asc_copy_gm2l1_nd2nz / asc_copy_gm2l1_dn2nz | 在 `CheckGm2L1DataType` 基础上加 `fp4x2_e1m2_t / fp4x2_e2m1_t` 同型 | 无 `fp4x2_*` 重载 | 前端过宽：允许 `fp4x2_e1m2 / fp4x2_e2m1` 同型，后端无 |
| CopyGM2L1（Scale 路径） | CheckGm2L1ScaleDataType | asc_copy_gm2l1_nd2nz | `(__cbuf__ fp8_e8m0_t, __gm__ fp8_e8m0_t)` | 通用，但无 `fp8_e8m0_t` 专门重载 | 后端缺 `fp8_e8m0_t` 重载 |
| CopyL12L0A | CheckL12L0ADataType | asc_copy_l12l0a | 同型：`half / int8 / uint8 / int16 / uint16 / int32 / uint32 / bfloat16_t / float / fp8_e4m3fn_t / fp8_e5m2_t / fp4x2_e1m2_t / fp4x2_e2m1_t / hifloat8_t` | 同型：`bfloat16_t / float / float8_e4m3_t / float8_e5m2_t / fp4x2_e1m2_t / fp4x2_e2m1_t / half / hifloat8_t / int4b_t / int8 / int16 / int32 / uint8 / uint16 / uint32` | 前端缺 `int4b_t`；类型名 `fp8_e4m3fn_t` vs `float8_e4m3_t`、`fp8_e5m2_t` vs `float8_e5m2_t` 待确认 |
| CopyL12L0B | CheckL12L0BDataType | asc_copy_l12l0b / asc_copy_l12l0b_trans | 同型：`half / int8 / uint8 / int16 / uint16 / int32 / uint32 / bfloat16_t / float / fp8_e4m3fn_t / fp8_e5m2_t / fp4x2_e1m2_t / fp4x2_e2m1_t / hifloat8_t` | 普通：同 L0A（含 `int4b_t`）；trans：另含 `float4_e1m2x2_t / float4_e2m1x2_t` | 前端缺 `int4b_t`；trans 路径若 `float4_e*x2_t` ≠ 前端 `fp4x2_e*_t` 则缺转置 b4 |
| CopyL12L0ScaleA | CheckL12L0ScaleADataType | asc_copy_l12l0a_mx（scale 通路） | `(fp8_e8m0_t, __cbuf__ fp8_e8m0_t)` | mx 路径下未见 `fp8_e8m0_t` 同名重载 | 后端是否真有 `fp8_e8m0_t` 路径需确认 |
| CopyL12L0ScaleB | CheckL12L0ScaleBDataType | asc_copy_l12l0b_mx（scale 通路） | `(fp8_e8m0_t, __cbuf__ fp8_e8m0_t)` | 同 ScaleA，待确认 | 同 ScaleA |
| CopyL12BT | CheckL12BtDataType | asc_copy_l12bt | `(__biasbuf__ float, __cbuf__ bfloat16_t/half/float) / (__biasbuf__ int32, __cbuf__ int32_t)` | src ∈ `{bfloat16_t / half / float / int32_t / void}`；dst 形式为 `uint64_t`（biasbuf 经 wrapper 转 uint64_t） | 基本一致；后端额外接受 `void*` src（前端约束略紧但合理） |
| CopyL12FB | CheckL12FbDataType | asc_copy_l12fb | `(__fbuf__ uint64_t, __cbuf__ uint64_t)` | `(__fbuf__ void*, __cbuf__ void*)` 通用类型 | 前端过紧：仅允许 uint64_t |
| CopyL0C2GM | CheckL0C2GmDataType | asc_copy_l0c2gm | `(gm float, cc float) / (gm int32, cc int32) / (gm half, cc float) / (gm bfloat16, cc float)` | dst ∈ `{half / bfloat16_t / float / float8_e4m3_t / float8_e5m2_t / hifloat8_t / int8 / uint8 / int32 / void}`，src ∈ `{float, int32_t}` | 前端缺 `int8 / uint8 / hifloat8 / fp8_e4m3 / fp8_e5m2` dst（即量化输出路径） |
| CopyL0C2UB | CheckL0C2UbDataType | asc_copy_l0c2ub | `(ubuf float, cc float) / (ubuf int32, cc int32) / (ubuf half, cc float) / (ubuf bfloat16, cc float)` | dst ∈ `{half / bfloat16_t / float / float8_e4m3_t / float8_e5m2_t / hifloat8_t / int8 / uint8 / int32 / void}`，src ∈ `{float, int32_t}` | 前端缺 `int8 / uint8 / hifloat8 / fp8_e4m3 / fp8_e5m2` dst |
| CopyL12UB | （无 Check） | asc_copy_l12ub | — | `(ubuf void*, cbuf void*)` | 前端无 Check（如需可补；后端通用） |
| Mmad（普通） | CheckMmadDataType | asc_mmad | `(cc int32, ca int8, cb int8) / (cc float, ca half, cb half) / (cc float, ca float, cb float) / (cc float, ca bfloat16, cb bfloat16) / (cc float, fp8_e4m3fn × fp8_{e4m3fn,e5m2}) / (cc float, fp8_e5m2 × fp8_{e4m3fn,e5m2}) / (cc float, hifloat8 × hifloat8)` | `(cc int32, int8 × int8) / (cc int32, int4b_t × int4b_t) / (cc float, half × half) / (cc float, float × float) / (cc float, bfloat16 × bfloat16) / (cc float, float8_e4m3 × float8_{e4m3,e5m2}) / (cc float, float8_e5m2 × float8_{e4m3,e5m2})` | 前端缺 `(int32, int4b_t × int4b_t)`；后端无 `hifloat8 × hifloat8`（前端列了）；类型名 `fp8_e4m3fn_t` vs `float8_e4m3_t` 待确认 |
| Mmad with bias | CheckMmadBiasDataType | asc_mmad（配合 L0C / BIAS 累积） | 在 `CheckMmadDataType` 基础上加 bias ∈ `{__biasbuf__ float / __biasbuf__ int32 / __cc__ float / __cc__ int32}` | 同上 | 与 `CheckMmadDataType` 同步 |
| Mmad MX | CheckMxMmadDataType | asc_mmad_mx | `(cc float, fp4x2_e2m1 × fp4x2_{e2m1,e1m2}) / (cc float, fp4x2_e1m2 × fp4x2_{e2m1,e1m2}) / (cc float, fp8_e4m3fn × fp8_{e4m3fn,e5m2}) / (cc float, fp8_e5m2 × fp8_{e4m3fn,e5m2})` 共 8 组 | impl 8 组同上；公开头仅导出 4 组同型（fp4 同型 2 + fp8 同型 2） | 公开头是否含异型 mix 声明需复查 |
| Mmad MX with bias | CheckMxMmadBiasDataType | asc_mmad_mx | 与 `CheckMxMmadDataType` 同步 | 同上 | 同 `CheckMxMmadDataType` |

## 汇总

- **前端过宽（后端无重载，编译期会失败）**
  - `CheckGm2L1AlignV2NDDataType`：`int64 / uint64 / fp4x2_e1m2 / fp4x2_e2m1 / fp8_e5m2 / fp8_e4m3fn`
  - `CheckGm2L1Fp4DataType`：`fp4x2_e1m2 / fp4x2_e2m1` 同型
- **前端过紧（功能缺失）**
  - `CheckL12FbDataType`：仅 `uint64_t`，后端通用 `void*`
  - `CheckL0C2GmDataType` / `CheckL0C2UbDataType`：缺 `int8 / uint8 / hifloat8 / fp8_e4m3 / fp8_e5m2` dst
  - `CheckL12L0ADataType` / `CheckL12L0BDataType`：缺 `int4b_t`
  - `CheckMmadDataType`：缺 `(int32, int4b_t × int4b_t)`
- **类型别名待确认**
  - `fp8_e4m3fn_t` vs `float8_e4m3_t`
  - `fp8_e5m2_t` vs `float8_e5m2_t`
  - `fp4x2_e1m2_t / fp4x2_e2m1_t` vs `float4_e1m2x2_t / float4_e2m1x2_t`
- **声明缺失**
  - `asc_mmad_mx` 公开头是否包含 fp4/fp8 异型 mix 声明需复查
  - `fp8_e8m0_t` 在 ScaleA/ScaleB 路径上是否真有对应 cbuf→ca/cb 重载需确认

## 资料（docs/api/context/tensor_api）与 c_api 后端的差异

仅列出"文档列出但后端 intrinsic 没有对应重载"或"后端有但文档没列"的情况；类型别名以下视为同一类型对待：`fp8_e4m3fn_t ≡ float8_e4m3_t`、`fp8_e5m2_t ≡ float8_e5m2_t`、`fp4x2_e*_t ≡ float4_e*x2_t`。

| 文档 | 文档列出的数据类型 | 后端支持 | 资料相对后端的差异 |
| --- | --- | --- | --- |
| `data_movement/gm_to_l1.md` | ND→ND、NZ→NZ、ZN→ZN 三类组合中含 `int64_t / uint64_t`；ND→NZ/ZN、DN→NZ/ZN、NZ→NZ、ZN→ZN 含 `fp4x2_e1m2 / fp4x2_e2m1`；多路径含 `fp8_e5m2 / fp8_e4m3fn` | nd2nz/dn2nz 后端无 `fp4x2_*`、`int64_t / uint64_t`；align 后端无 `fp4x2_*`、`fp8_*`、`int64_t / uint64_t` | 资料过宽：列出但后端无重载——`fp4x2_e1m2 / fp4x2_e2m1`（除 ND→ND/AlignV2 不含外，其它分形路径全部列了，后端 nd2nz/dn2nz 无）、`int64_t / uint64_t`（ND→ND、NZ→NZ、ZN→ZN，仅 align 路径列了，后端 align 无） |
| `data_movement/l1_to_l0a.md` | `fp4x2_e2m1 / fp4x2_e1m2 / int8 / uint8 / hifloat8 / fp8_e5m2 / fp8_e4m3fn / half / bfloat16 / int16 / uint16 / int32 / uint32 / float` | 后端额外含 `int4b_t` | 资料缺 `int4b_t` |
| `data_movement/l1_to_l0b.md` | 同 L0A | 同 L0A，且 trans 路径含 `float4_e1m2x2 / float4_e2m1x2`（视作 `fp4x2_*` 别名） | 资料缺 `int4b_t` |
| `data_movement/l1_to_l0scalea.md` | 仅 `fp8_e8m0_t` | mx 路径下未见 `fp8_e8m0_t` 同名重载 | 后端是否真有 `fp8_e8m0_t` 通路需确认；若无，资料过宽 |
| `data_movement/l1_to_l0scaleb.md` | 同 ScaleA | 同 ScaleA，待确认 | 同 ScaleA |
| `data_movement/l1_to_bias_table.md` | dst=float ↔ src ∈ `bfloat16 / half / float`；`int32 ↔ int32` | 后端 src ∈ `bfloat16 / half / float / int32_t / void` | 一致；后端 `void*` src 通用，文档不需要罗列 |
| `data_movement/l1_to_fixpipe.md` | 仅 `uint64_t` | 后端 `void*` 通用，等价任意类型 | 资料过紧；与 `CheckL12FbDataType` 同步过紧 |
| `data_movement/l0c_to_gm.md` | dst ∈ `int8 / uint8 / fp8_e4m3fn / hifloat8 / half / bfloat16 / int32 / float`；src ∈ `int32 / float`（按表中组合） | 后端 dst ∈ `bfloat16 / half / float / float8_e4m3 / float8_e5m2 / hifloat8 / int8 / uint8 / int32 / void`，src ∈ `float / int32` | 资料缺 `fp8_e5m2`（等价 `float8_e5m2_t`）dst —— 后端有，文档未列 |
| `data_movement/l0c_to_ub.md` | 同 l0c_to_gm | 同 l0c_to_gm | 同上：缺 `fp8_e5m2` dst |
| `data_movement/l0c_to_ub_dual_dst.md` | 文档未列"数据类型"小节 | 与 l0c_to_ub 共享 | 文档缺失，建议补充与 `l0c_to_ub.md` 一致的类型表 |
| `mmad_computation/mmad_computation.md` | A×B→C：`int8×int8→int32 / half×half→float / float×float→float / bfloat16×bfloat16→float / fp8_e4m3fn × fp8_{e4m3fn, e5m2} → float / fp8_e5m2 × fp8_{e4m3fn, e5m2} → float / hifloat8 × hifloat8 → float`；bias：`int8×int8→int32` 用 `int32` bias，其余用 `float` bias | 后端 `asc_mmad` 公开头还有 `int4b_t × int4b_t → int32`；后端无 `hifloat8 × hifloat8` 直接重载 | 资料缺 `int4b_t × int4b_t → int32`；资料的 `hifloat8×hifloat8→float` 在公开头未声明（需确认是否走 mx 路径或别处声明） |

### 文档与 c_api 差异汇总

- **资料过宽（与后端不符）**
  - `gm_to_l1.md`：列了 `fp4x2_e1m2 / fp4x2_e2m1` 在 nd2nz / dn2nz 路径，后端无；列了 `int64 / uint64` 在 align 路径，后端无
  - `l1_to_fixpipe.md`：限定 `uint64_t`，与 Check 同步过紧但与"硬件支持任意位"的真实情况不符
- **资料过紧（功能缺失）**
  - `l1_to_l0a.md` / `l1_to_l0b.md`：缺 `int4b_t`
  - `l0c_to_gm.md` / `l0c_to_ub.md`：缺 `fp8_e5m2_t` dst
  - `mmad_computation.md`：缺 `int4b_t × int4b_t → int32`
- **资料未列**
  - `l0c_to_ub_dual_dst.md`：未提供"数据类型"章节
- **资料保留待确认**
  - `mmad_computation.md`：列 `hifloat8 × hifloat8 → float`，公开头 mmad 未见，需复查走哪条 intrinsic
  - `l1_to_l0scalea.md` / `l1_to_l0scaleb.md`：列 `fp8_e8m0_t`，后端 ScaleA/B 通路是否有对应重载未确认
