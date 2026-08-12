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
import { resolve } from 'node:path'
import { test } from 'node:test'

import {
  findKernelLaunch,
  kernelLaunchMatches,
  normalizeKernelLaunchQuery,
} from '../../docs/.vitepress/search-syntax.mjs'
import {
  bestPagefindSection,
  collectCandidates,
  createIdentifierIndexLoader,
  createQueryPlan,
  deserializePagefindMeta,
  findIdentifierIndexMatches,
  highlightIdentifierExcerpt,
  highlightSearchText,
  initializePagefindSearch,
  MAX_CJK_SUBPHRASES,
  MAX_QUERY_LENGTH,
  mergeRouteResult,
} from '../../docs/.vitepress/theme/pagefind-search.mjs'
import { sourcePathsForRoute } from '../../docs/.vitepress/build-pagefind-title-index.mjs'

test('normalizes kernel launch syntax queries and prefixes', () => {
  for (const query of [
    '<<<',
    '<<<gridDim',
    '<<<gridDim, blockDim>>',
    '<<<>>>',
    ' <<<...>>> ',
    '<<<gridDim, blockDim>>>',
  ]) {
    assert.equal(normalizeKernelLaunchQuery(query), '<<<>>>')
  }
})

test('does not treat partial or mixed text as a kernel launch syntax query', () => {
  for (const query of ['', '<<', '>>>', 'prefix <<<>>> suffix', 'foo<<<1>>>']) {
    assert.equal(normalizeKernelLaunchQuery(query), '')
  }
})

test('finds complete launch syntax while preferring the canonical spelling', () => {
  assert.deepEqual(kernelLaunchMatches('a <<<1, 2>>> b <<<>>>'), [
    { index: 2, value: '<<<1, 2>>>' },
    { index: 15, value: '<<<>>>' },
  ])
  assert.deepEqual(findKernelLaunch('a <<<1>>> b <<<>>>'), {
    index: 12,
    value: '<<<>>>',
  })
})

test('centers identifier excerpts around the highlighted match', () => {
  const highlighted = highlightIdentifierExcerpt(
    `${'前'.repeat(40)}核函数通过<<<...>>>启动${'后'.repeat(120)}`,
    '<<<>>>',
    true,
  )
  const markStart = highlighted.indexOf('<mark>')
  const markEnd = highlighted.indexOf('</mark>')

  assert.ok(highlighted.startsWith('...'))
  assert.equal(Array.from(highlighted.slice(3, markStart)).length, 12)
  assert.equal(highlighted.slice(markStart, markEnd + 7), '<mark>&lt;&lt;&lt;...&gt;&gt;&gt;</mark>')
  assert.ok(highlighted.endsWith('...'))
})

test('escapes identifier excerpts before adding highlight markup', () => {
  assert.equal(
    highlightIdentifierExcerpt('<tag>NO_OVER suffix', 'no_over'),
    '&lt;tag&gt;<mark>NO_OVER</mark> suffix',
  )
  assert.equal(
    highlightIdentifierExcerpt('<tag>without a match', 'no_over'),
    '&lt;tag&gt;without a match',
  )
})

test('selects the Pagefind section containing the strongest match', () => {
  const section = bestPagefindSection({
    url: '/guide/page.html',
    weighted_locations: [{ location: 18, weight: 10 }],
    sub_results: [
      {
        url: '/guide/page.html#first',
        anchor: { text: 'First section' },
        locations: [4],
      },
      {
        url: '/guide/page.html#second',
        anchor: { text: 'Second：section' },
        locations: [18],
      },
    ],
  })

  assert.deepEqual(section, {
    route: '/guide/page.html#second',
    sectionTitle: 'Second：section',
  })
})

test('weights title alias candidates above content candidates', async () => {
  const pagefind = {
    async search() {
      return { results: [{ id: 'page', score: 1 }] }
    },
  }
  const variants = [{ query: 'page', weight: 1 }]
  const content = await collectCandidates(pagefind, variants)
  const title = await collectCandidates(pagefind, variants, new Map(), 'title-alias')

  assert.equal(content.get('content:page').retrievalScore, 1 / 61)
  assert.equal(title.get('title-alias:page').retrievalScore, 1.3 / 61)
})

test('continues with the primary index when the title index is unavailable', async () => {
  let mergeCalls = 0
  const pagefind = {
    async mergeIndex() {
      mergeCalls++
      throw new Error('404')
    },
  }
  const originalWarn = console.warn
  console.warn = () => {}
  try {
    assert.equal(await initializePagefindSearch(pagefind), pagefind)
    assert.equal(await initializePagefindSearch(pagefind), pagefind)
  }
  finally {
    console.warn = originalWarn
  }
  assert.equal(mergeCalls, 1)
})

test('retries the identifier index after a temporary request failure', async () => {
  const expected = { p: [['/api/page', 'Page']], i: [['identifier', [0]]], s: [], h: [] }
  let calls = 0
  const load = createIdentifierIndexLoader(async () => {
    calls++
    if (calls === 1) throw new Error('temporary failure')
    return expected
  })

  assert.deepEqual(await load(), { p: [], i: [], s: [], h: [] })
  assert.equal(await load(), expected)
  assert.equal(await load(), expected)
  assert.equal(calls, 2)
})

test('restores Pagefind dates and serialized frontmatter metadata', () => {
  const frontmatter = { category: 'api', date: 1786410419730, publish: false }
  const base64 = Buffer.from(encodeURIComponent(JSON.stringify(frontmatter))).toString('base64')

  assert.deepEqual(deserializePagefindMeta({ base64, title: 'Page title' }), {
    category: 'api',
    date: 1786410419730,
    publish: false,
    title: 'Page title',
  })
  assert.deepEqual(deserializePagefindMeta({
    date: '1786410419730',
    title: 'Page title',
  }), {
    date: 1786410419730,
    title: 'Page title',
  })
})

test('keeps content metadata and result when a title alias ranks higher', () => {
  const contentData = { meta: { date: '1786410419730', title: 'Page title' } }
  const titleData = { meta: { ascSearchSource: 'title-route', title: 'Page title' } }
  const merged = mergeRouteResult({
    route: '/guide/page.html',
    description: 'Content excerpt',
    meta: { category: 'guide', date: 1786410419730, description: 'Content excerpt', title: 'Page title' },
    result: contentData,
    score: 10,
  }, {
    route: '/guide/page.html',
    description: '',
    meta: { description: '', title: 'Page title' },
    result: titleData,
    score: 20,
  })

  assert.equal(merged.description, 'Content excerpt')
  assert.equal(merged.meta.category, 'guide')
  assert.equal(merged.meta.date, 1786410419730)
  assert.equal(merged.result, contentData)
})

test('escapes and highlights section titles', () => {
  assert.equal(
    highlightSearchText('配置<<<...>>>调用', '<<<'),
    '配置<mark>&lt;&lt;&lt;...&gt;&gt;&gt;</mark>调用',
  )
  assert.equal(
    highlightSearchText('<unsafe> section', 'section'),
    '&lt;unsafe&gt; <mark>section</mark>',
  )
})

test('matches omitted identifier decorators and prefers canonical macro spelling', () => {
  const entries = [
    ['__npu_arch__'],
    ['acl_rtc_npu_arch'],
    ['npu_arch'],
    ['npu_arch_2201'],
  ]
  for (const query of ['__NPU_ARCH__', '_NPU_ARCH', 'NPU_ARCH']) {
    const plan = createQueryPlan(query)
    assert.equal(plan.preferDecoratedIdentifier, true)
    assert.deepEqual(
      findIdentifierIndexMatches(entries, plan.identifierQuery, plan.preferDecoratedIdentifier)
        .map(match => match.entry[0]),
      ['__npu_arch__', 'npu_arch', 'npu_arch_2201'],
    )
  }

  const lowercasePlan = createQueryPlan('npu_arch')
  assert.equal(lowercasePlan.preferDecoratedIdentifier, false)
  assert.equal(
    findIdentifierIndexMatches(entries, lowercasePlan.identifierQuery)[0].entry[0],
    'npu_arch',
  )
})

test('bounds long CJK queries and generated subphrases', () => {
  const plan = createQueryPlan('中'.repeat(2048))

  assert.equal(Array.from(plan.query).length, MAX_QUERY_LENGTH)
  assert.ok(plan.cjkSubPhrases[0].length <= MAX_CJK_SUBPHRASES)
  assert.ok(plan.cjkSubPhrases[0].includes('中中'))
})

test('maps directory routes to intermediate index pages', () => {
  const root = resolve('/tmp/intermediate-html')

  assert.deepEqual(sourcePathsForRoute(root, '/guide/'), [
    resolve(root, 'guide.html'),
    resolve(root, 'guide/index.html'),
  ])
  assert.deepEqual(sourcePathsForRoute(root, '/'), [resolve(root, 'index.html')])
  assert.deepEqual(sourcePathsForRoute(root, '/../outside'), [])
})
