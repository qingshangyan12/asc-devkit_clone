/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

import { decodeHTML, escapeAttribute } from 'entities'

const HEADING_REGEX = /<h([23456])((?:\s[^>]*)?)>([\s\S]*?)<\/h\1>/gi
const ID_ATTRIBUTE_REGEX = /\sid\s*=\s*(["'])(.*?)\1/i

export function decodeHtmlEntities(text) {
  return decodeHTML(text)
}

export function normalizeHeadingText(content) {
  return decodeHtmlEntities(content.replace(/<[^>]+>/g, ''))
    .replace(/\u200b/g, '')
    .replace(/\s+/g, ' ')
    .trim()
}

export function slugifyHeading(text) {
  return text.trim().replace(/\s+/g, '-')
}

function getHeadingSlug(attrs, title, seen) {
  const idMatch = attrs.match(ID_ATTRIBUTE_REGEX)
  if (idMatch) return decodeHtmlEntities(idMatch[2])

  const baseSlug = slugifyHeading(title)
  const count = seen.get(baseSlug) || 0
  const slug = count === 0 ? baseSlug : `${baseSlug}-${count}`
  seen.set(baseSlug, count + 1)
  return slug
}

export function extractHeaders(html) {
  const headers = []
  const seen = new Map()
  let match
  while ((match = HEADING_REGEX.exec(html)) !== null) {
    const level = parseInt(match[1])
    const title = normalizeHeadingText(match[3])
    if (!title) continue
    const slug = getHeadingSlug(match[2], title, seen)
    headers.push({ level, title, slug, link: `#${slug}` })
  }
  return headers
}

export function addHeadingAnchors(html) {
  const seen = new Map()
  return html.replace(HEADING_REGEX, (match, level, attrs, content) => {
    const title = normalizeHeadingText(content)
    if (!title) return match
    const slug = getHeadingSlug(attrs, title, seen)
    const escapedSlug = escapeAttribute(slug)
    const idAttr = ID_ATTRIBUTE_REGEX.test(attrs) ? '' : ` id="${escapedSlug}"`
    const anchor = `<a class="header-anchor" href="#${escapedSlug}">&#x200B;</a>`
    return `<h${level}${attrs}${idAttr}>${content}${anchor}</h${level}>`
  })
}
