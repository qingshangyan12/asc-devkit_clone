/** Copyright (c) 2026 Huawei Technologies Co., Ltd. */
#ifndef IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_CAST_ENUMS_H
#define IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_CAST_ENUMS_H
namespace asc {
namespace te {
namespace cast_round_mode {
using RD = rd;
using RN = rn;
using RNA = rna;
using RH = rh;
using RO = ro;
using RU = ru;
using RZ = rz;
} // namespace cast_round_mode
namespace CastRoundMode = cast_round_mode;

namespace cast_sat_mode {
using Sat = sat;
using NoSat = no_sat;
} // namespace cast_sat_mode
namespace CastSatMode = cast_sat_mode;

namespace cast_index_pos {
using Even = even;
using Odd = odd;
using PartP0 = part_p0;
using PartP1 = part_p1;
using PartP2 = part_p2;
using PartP3 = part_p3;
} // namespace cast_index_pos
namespace CastIndexPos = cast_index_pos;

using CastTraitDefault = cast_trait_default;
} // namespace te
} // namespace asc
#endif // IMPL_TENSOR_API_LEGACY_ARCH_VECTOR_CAST_ENUMS_H
