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
 * \file conv2d_api.h
 * \brief
 */

#ifndef CONV2D_API_H
#define CONV2D_API_H

#include "../../impl/conv/conv2d/conv2d_api_impl.h"
#include "conv2d_config.h"
#include "lib/conv/conv2d_intf.h"

namespace conv2d {
using namespace conv;

template <class Config,
          template<typename, class> class Impl = Conv2dApiImpl,
          template<class, template<typename, class> class>class Intf = Conv2dIntf>
struct Conv2dIntfExt : public Intf<Config, Impl> {
    __aicore__ inline Conv2dIntfExt() {}
};

#define REGISTER_CONV_API(name, Config, Impl, Intf)                                                                   \
    template <class FMAP_TYPE, class WEIGHT_TYPE, class OUTPUT_TYPE, class BIAS_TYPE, class CONV_CFG=Conv2dParam>     \
    using name =                                                                                                      \
        Conv2dIntfExt<Config<ConvDataType<FMAP_TYPE, WEIGHT_TYPE, OUTPUT_TYPE, BIAS_TYPE, CONV_CFG>>, Impl, Intf>

REGISTER_CONV_API(Conv2d, Conv2dCfg, Conv2dApiImpl, Conv2dIntf);

}  // namespace conv2d

#endif // CONV2D_API_H