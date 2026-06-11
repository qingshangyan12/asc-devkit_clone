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
 * \file conv2d_config.h
 * \brief
 */

#ifndef CONV2D_CONFIG_H
#define CONV2D_CONFIG_H

#include "../../impl/conv/conv_common/conv_config.h"

namespace conv2d {
using namespace conv;

struct Conv2dParam : public ConvParam {
    __aicore__ inline Conv2dParam() {};
};

template <class ConvDataType>
struct Conv2dCfg : public ConvConfig<ConvDataType> {
public:
    __aicore__ inline Conv2dCfg() {}

    using ContextData = struct _ : public ConvConfig<ConvDataType>::ContextData {
        __aicore__ inline _() {}
    };
};

}  // namespace conv2d

#endif // CONV2D_CONFIG_H