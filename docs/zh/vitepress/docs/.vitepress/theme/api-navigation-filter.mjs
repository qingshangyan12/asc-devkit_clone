/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

import {
  filterNavigationCandidates,
  getFilteredSidebarConfig,
} from '../../../scripts/api-support.mjs'
import {
  apiUnsupportedIndex,
  sidebarConfig,
} from 'virtual:api-navigation-data'
import { selectedFilter } from './filter-state.js'

export { apiUnsupportedIndex }

export function getProductFilteredSidebarConfig() {
  return getFilteredSidebarConfig(
    sidebarConfig,
    selectedFilter.value,
    apiUnsupportedIndex
  )
}

export function getProductFilteredNavigationCandidates(candidates, currentRelativePath) {
  const filteredSidebarConfig = getProductFilteredSidebarConfig()
  return filterNavigationCandidates(
    candidates,
    currentRelativePath,
    selectedFilter.value,
    apiUnsupportedIndex,
    filteredSidebarConfig?.['/api/']
  )
}
