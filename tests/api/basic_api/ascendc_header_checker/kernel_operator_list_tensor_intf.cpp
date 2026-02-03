#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#if __NPU_ARCH__ != 3002 && __NPU_ARCH__ != 3102
#include "kernel_operator_list_tensor_intf.h"
#endif
#endif
