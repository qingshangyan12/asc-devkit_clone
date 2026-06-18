/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file ain_host.h
 * \brief Ain host-side control-plane interface
 */
#ifndef INCLUDE_ADV_API_AIN_AIN_HOST_H
#define INCLUDE_ADV_API_AIN_AIN_HOST_H

#include <cstdint>
#include <vector>

#include "hccl/hccl.h"
#include "hccl/hccl_types.h"
#include "hccl/hccl_res.h"
#include "hcomm/hcomm_res.h"
#include "hcomm/hcomm_res_entity_defs.h"

/*!
 * @brief Opaque handle to a registered symmetric communication window.
 */
typedef void *CommSymWindow;

/*!
 * @brief Opaque handle to the device communication table built on the host and copied to the device.
 */
typedef void *HcclDevComm;

/*!
 * @brief Register a device buffer as a symmetric communication window on the given communicator.
 * @param [in] comm: The HCCL communicator.
 * @param [in] addr: Device address of the buffer to register.
 * @param [in] size: Size of the buffer in bytes.
 * @param [out] winHandle: Receives the encoded symmetric-window handle.
 * @param [in] flag: Reserved registration flag (currently unused).
 * @param [out] memHandle: Receives the underlying registered memory handle.
 * @return HCCL_SUCCESS on success, otherwise an HCCL error code.
 */
HcclResult HcclCommSymWinRegisterExp(HcclComm comm, void *addr, uint64_t size, CommSymWindow *winHandle,
    uint32_t flag, HcclMemHandle *memHandle);

/*!
 * @brief Build the device communication table (channels, signal buffers, rank metadata) for a
 *        communicator and copy it to the device.
 * @param [in] comm: The HCCL communicator.
 * @param [out] outDevComm: Receives the device-side communication table handle.
 * @param [in,out] memHandles: Registered memory handles; the signal buffer handle is appended.
 * @return HCCL_SUCCESS on success, otherwise an HCCL error code.
 */
HcclResult HcclDevCommCreateExp(HcclComm comm, HcclDevComm *outDevComm,
    std::vector<HcclMemHandle> &memHandles);

#endif // INCLUDE_ADV_API_AIN_AIN_HOST_H
