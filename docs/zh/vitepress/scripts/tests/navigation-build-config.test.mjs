/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

import assert from 'node:assert/strict'
import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'
import test from 'node:test'

const root = resolve(import.meta.dirname, '..', '..')
const config = (await import('../../docs/.vitepress/config.mjs')).default

test('keeps large navigation data out of serialized theme config', () => {
  assert.equal('sidebar' in config.themeConfig, false)
  assert.equal('apiSidebarSource' in config.themeConfig, false)
  assert.equal('apiUnsupportedIndex' in config.themeConfig, false)

  const plugin = config.vite.plugins.find(item => item.name === 'vitepress-navigation-data')
  assert.ok(plugin)
  const id = plugin.resolveId('virtual:api-navigation-data')
  const source = plugin.load(id)

  assert.match(source, /export const sidebarConfig =/)
  assert.match(source, /export const apiUnsupportedIndex =/)
  assert.equal(config.vite.build.rollupOptions.output.manualChunks(id), 'navigation-data')
})

test('patches sidebar and prev-next navigation to use shared data', () => {
  const plugin = config.vite.plugins.find(item => item.name === 'patch-vitepress-api-navigation')
  const sidebarPath = resolve(root, 'node_modules/vitepress/dist/client/theme-default/composables/sidebar.js')
  const prevNextPath = resolve(root, 'node_modules/vitepress/dist/client/theme-default/composables/prev-next.js')

  const sidebar = plugin.transform(readFileSync(sidebarPath, 'utf-8'), sidebarPath)
  const prevNext = plugin.transform(readFileSync(prevNextPath, 'utf-8'), prevNextPath)

  assert.match(sidebar, /const sidebarConfig = getProductFilteredSidebarConfig\(\)/)
  assert.match(prevNext, /getSidebar\(getProductFilteredSidebarConfig\(\)/)
  assert.match(prevNext, /getProductFilteredNavigationCandidates/)
})

test('renders children only for active or expanded sidebar branches', () => {
  const plugin = config.vite.plugins.find(item => item.name === 'patch-vpsidebar-item-depth')
  const componentPath = resolve(root, 'node_modules/vitepress/dist/client/theme-default/components/VPSidebarItem.vue')
  const component = plugin.transform(readFileSync(componentPath, 'utf-8'), componentPath)

  assert.match(component, /v-if="!collapsed \|\| hasActiveLink"/)
  assert.doesNotMatch(component, /v-if="depth < 5"/)
})
