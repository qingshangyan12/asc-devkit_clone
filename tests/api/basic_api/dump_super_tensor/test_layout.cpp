#include <cstdint>
#include <cstddef>
#include "impl/utils/debug/asc_debug_types.h"
using namespace __asc_aicore;
static_assert(sizeof(DumpSuperTensorTlv) == 88);
static_assert(sizeof(DumpSuperTensorBodyTlv) == 36);
static_assert(offsetof(DumpSuperTensorTlv, tensorLength) == 68);
static_assert(offsetof(DumpSuperTensorTlv, dumpSize) == 84);
static_assert(offsetof(DumpSuperTensorBodyTlv, tensorOffset) == 24);
static_assert(static_cast<unsigned>(DumpType::DUMP_SUPER_TENSOR) == 11);
static_assert(static_cast<unsigned>(DumpType::DUMP_SUPER_TENSOR_BODY) == 12);
int main() {}
