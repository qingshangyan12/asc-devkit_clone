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
import test from 'node:test'
import { load as cheerioLoad } from 'cheerio'

import {
  addHeadingAnchors,
  extractHeaders,
  normalizeHeadingText,
} from '../heading-utils.mjs'
import { stripTags } from '../../docs/.vitepress/build-cjk-index.mjs'

test('decodes named and numeric entities in heading text', () => {
  const content = '<span>GM &gt; UB &amp; L1 &#62; L0 &#x2192; GM &quot;copy&quot;</span>'

  assert.equal(normalizeHeadingText(content), 'GM > UB & L1 > L0 → GM "copy"')
})

test('uses decoded heading text for outline titles and links', () => {
  const html = [
    '<h2>DataCopy（GM -&gt; UB随路转换ND2NZ搬运）<a name="topic"></a></h2>',
    '<h2>DataCopy（GM -&#62; UB随路转换ND2NZ搬运）</h2>',
  ].join('')

  assert.deepEqual(extractHeaders(html), [
    {
      level: 2,
      title: 'DataCopy（GM -> UB随路转换ND2NZ搬运）',
      slug: 'DataCopy（GM-->-UB随路转换ND2NZ搬运）',
      link: '#DataCopy（GM-->-UB随路转换ND2NZ搬运）',
    },
    {
      level: 2,
      title: 'DataCopy（GM -> UB随路转换ND2NZ搬运）',
      slug: 'DataCopy（GM-->-UB随路转换ND2NZ搬运）-1',
      link: '#DataCopy（GM-->-UB随路转换ND2NZ搬运）-1',
    },
  ])
})

test('generates anchors that resolve to the same decoded heading ids', () => {
  const html = addHeadingAnchors([
    '<h2>DataCopy（GM -&gt; UB）</h2>',
    '<h2 id="custom-&amp;-heading">Existing ID</h2>',
    '<h2>Quoted &quot;heading&quot;</h2>',
  ].join(''))
  const $ = cheerioLoad(html, {}, false)
  const headings = $('h2').toArray()

  assert.equal($(headings[0]).attr('id'), 'DataCopy（GM-->-UB）')
  assert.equal($(headings[0]).find('.header-anchor').attr('href'), '#DataCopy（GM-->-UB）')
  assert.equal($(headings[1]).attr('id'), 'custom-&-heading')
  assert.equal($(headings[1]).find('.header-anchor').attr('href'), '#custom-&-heading')
  assert.equal($(headings[2]).attr('id'), 'Quoted-"heading"')
  assert.equal($(headings[2]).find('.header-anchor').attr('href'), '#Quoted-"heading"')
})

test('preserves decoded entity characters in the CJK search text', () => {
  const html = '<h2>DataCopy（GM -&gt; UB）</h2><p>A &amp; B &#x2192; C</p>'

  assert.equal(stripTags(html), 'DataCopy（GM -> UB） A & B → C')
})
