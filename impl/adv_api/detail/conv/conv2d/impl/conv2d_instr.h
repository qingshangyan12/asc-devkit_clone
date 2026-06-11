/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*!
 * \file conv2d_intr.h
 * \brief
 */

#ifndef CONV2D_INSTR_H
#define CONV2D_INSTR_H

#if defined(__DAV_L510__)
    #include "dav_l510/conv2d_instr_impl.h"
    #include "dav_l510/conv2d_instr_hw_mode_impl.h"
    #include "dav_l510/conv2d_instr_m_mode_impl.h"
    #include "dav_l510/conv2d_instr_opt_group_impl.h"
#else
#endif

#endif // CONV2D_INSTR_H