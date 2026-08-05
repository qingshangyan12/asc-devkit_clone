#ifndef CCU_ST_SELECTOR_SHIM_CCU_PRIMITIVES_DL_HPP
#define CCU_ST_SELECTOR_SHIM_CCU_PRIMITIVES_DL_HPP

#include "ccu_primitives_stub.h"

namespace AscendC {
namespace ccu {

class CcuBuffer {
public:
    ResourceHandle handle{0};
};

class Loop {
public:
    ResourceHandle handle{0};
};

class LoopGroup {
public:
    ResourceHandle handle{0};
};

} // namespace ccu
} // namespace AscendC

#endif // CCU_ST_SELECTOR_SHIM_CCU_PRIMITIVES_DL_HPP
