#define CANN_ASC_USE_EXPERIMENTAL
#include "c_api/experimental/reg_arith.h"
#include "c_api/experimental/reg_permute_sel.h"
#include "c_api/experimental/loadalign.h"
#include "c_api/asc_simd.h"

static_assert(sizeof(asc_uint32_t_bool_pair) > 0);
static_assert(sizeof(asc_float_float_pair) > 0);
