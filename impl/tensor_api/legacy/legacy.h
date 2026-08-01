/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_LEGACY_H
#define IMPL_TENSOR_API_LEGACY_LEGACY_H

#include "impl/tensor_api/legacy/utils/constant.h"
#include "impl/tensor_api/legacy/utils/location.h"
#include "impl/tensor_api/legacy/utils/map.h"
#include "impl/tensor_api/legacy/utils/utils.h"

#include "impl/tensor_api/legacy/tensor/tuple.h"
#include "impl/tensor_api/legacy/tensor/layout_definition.h"
#include "impl/tensor_api/legacy/tensor/layout_frame.h"
#include "impl/tensor_api/legacy/tensor/layout_pattern.h"
#include "impl/tensor_api/legacy/tensor/layout_impl.h"
#include "impl/tensor_api/legacy/tensor/layout.h"
#include "impl/tensor_api/legacy/tensor/pointer_mem.h"
#include "impl/tensor_api/legacy/tensor/pointer_pattern.h"
#include "impl/tensor_api/legacy/tensor/pointer_adaptor.h"
#include "impl/tensor_api/legacy/tensor/engine.h"
#include "impl/tensor_api/legacy/tensor/tensor_types.h"
#include "impl/tensor_api/legacy/tensor/pointer.h"
#include "impl/tensor_api/legacy/tensor/tensor.h"

#include "impl/tensor_api/legacy/arch/cube/copy_op.h"
#include "impl/tensor_api/legacy/arch/cube/mmad_op.h"
#include "impl/tensor_api/legacy/arch/utils/is_format.h"
#include "impl/tensor_api/legacy/arch/vector/copy_op.h"
#include "impl/tensor_api/legacy/arch/vector/cast_enums.h"
#include "impl/tensor_api/legacy/arch/vector/mask.h"
#include "impl/tensor_api/legacy/arch/vector/axpy.h"
#include "impl/tensor_api/legacy/arch/vector/binary.h"
#include "impl/tensor_api/legacy/arch/vector/binary_scalar.h"
#include "impl/tensor_api/legacy/arch/vector/cast_instruction.h"
#include "impl/tensor_api/legacy/arch/vector/dual.h"
#include "impl/tensor_api/legacy/arch/vector/unary.h"
#include "impl/tensor_api/legacy/arch/arch.h"

#include "impl/tensor_api/legacy/atom/cube/copy_atom.h"
#include "impl/tensor_api/legacy/atom/cube/mmad_atom.h"

#include "impl/tensor_api/legacy/algorithm/copy.h"
#include "impl/tensor_api/legacy/algorithm/mmad.h"
#include "impl/tensor_api/legacy/algorithm/transform.h"

#endif // IMPL_TENSOR_API_LEGACY_LEGACY_H
