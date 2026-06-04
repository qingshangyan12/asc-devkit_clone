#define CCU_IF_NEW(expr) CCU_IF_IMPL_NEW(expr, __COUNTER__)
#define CCU_IF_IMPL_NEW(expr, ctr) for(int i=0;i<1;i++)

#include "include/adv_api/hccl/internal/hcomm/pkg_inc/hcomm/ccu_new/ccu_control_flow_macro.h"

// Check which CCU_IF is defined
