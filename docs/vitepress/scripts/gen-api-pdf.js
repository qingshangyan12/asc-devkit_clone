/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

const fs = require('fs')
const path = require('path')

const root = path.resolve(__dirname, '..')
const apiDir = path.resolve(root, 'docs', 'api')
const outDir = path.resolve(root, 'docs', 'public', 'api-pdf')

function findBrowser() {
  const candidates = [
    process.env.PUPPETEER_EXECUTABLE_PATH,
    'C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe',
    'C:\\Program Files\\Microsoft\\Edge\\Application\\msedge.exe',
    'C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe',
    'C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe',
    '/usr/bin/microsoft-edge',
    '/usr/bin/microsoft-edge-stable',
    '/usr/bin/chromium-browser',
    '/usr/bin/chromium',
    '/usr/bin/google-chrome',
    '/usr/bin/google-chrome-stable',
  ].filter(Boolean)

  for (const p of candidates) {
    if (fs.existsSync(p)) return p
  }
  return null
}

function parseReadmeTree() {
  const repoRoot = path.resolve(__dirname, '../../..')
  const readmePath = path.resolve(repoRoot, 'docs', 'api', 'README.md')
  if (!fs.existsSync(readmePath)) {
    console.warn('[gen-pdf] README.md not found:', readmePath)
    return []
  }
  const content = fs.readFileSync(readmePath, 'utf-8')
  const lines = content.split('\n')

  const rawEntries = []
  for (const line of lines) {
    const m = line.match(/^(\s*)-\s+\[([^\]]+)\]\((.+?\.md)\)/)
    if (!m) continue
    const indent = m[1].length
    const text = m[2].replace(/\\([\\`*_{}[\]()#+\-.!|~&<>])/g, '$1')
    const mdPath = m[3].replace(/^\.\//, '')
    rawEntries.push({ indent, text, mdPath })
  }

  if (rawEntries.length === 0) return []

  const indentValues = [...new Set(rawEntries.map(e => e.indent))].sort((a, b) => a - b)
  const indentUnit = indentValues[0] || 4

  const root = { items: [] }
  const stack = [{ depth: -indentUnit, node: root }]

  for (const entry of rawEntries) {
    const depth = Math.round((entry.indent - indentValues[0]) / indentUnit)
    const current = { text: entry.text, mdPath: entry.mdPath, depth, items: [] }

    while (stack.length > 1 && stack[stack.length - 1].depth >= depth) {
      stack.pop()
    }

    const parent = stack[stack.length - 1].node
    parent.items.push(current)
    stack.push({ depth, node: current })
  }

  return root.items
}

function collectRawSections(nodes, vpApiDir) {
  const rawSections = []
  const htmlToSecIdMap = {}
  let secCounter = 0
  let allCss = ''
  function walk(nodeList) {
    for (const node of nodeList) {
      const htmlRel = node.mdPath.replace(/\.md$/, '.html')
      const normalizedRel = htmlRel.replace(/\\/g, '/')
      const htmlPath = path.join(vpApiDir, htmlRel)
      if (fs.existsSync(htmlPath)) {
        try {
          const parts = extractContent(htmlPath)
          if (!allCss && parts.css) allCss = parts.css
          const secId = 'sec-' + secCounter
          rawSections.push({
            text: node.text,
            depth: node.depth,
            rawContent: parts.content,
            relPath: normalizedRel,
            htmlDir: path.dirname(htmlPath),
            secId: secId,
          })
          htmlToSecIdMap[normalizedRel] = secId
          secCounter++
        } catch (err) {
          console.warn('[gen-pdf]   skipping tree node ' + htmlRel + ': ' + err.message)
        }
      } else {
        console.warn('[gen-pdf]   tree node HTML not found: ' + htmlRel)
      }
      if (node.items && node.items.length > 0) {
        walk(node.items)
      }
    }
  }
  walk(nodes)
  return { rawSections, allCss, htmlToSecIdMap }
}

function extractContent(htmlPath) {
  const html = fs.readFileSync(htmlPath, 'utf-8')

  const cssMatch = html.match(/<style[^>]*>([\s\S]*?)<\/style>/i)
  const css = cssMatch ? cssMatch[1] : ''

  let content = ''
  const articleMatch = html.match(/<article[^>]*class="[^"]*markdown-body[^"]*"[^>]*>([\s\S]*?)<\/article>/i)
  if (articleMatch) {
    content = articleMatch[1]
  } else {
    const bodyMatch = html.match(/<body[^>]*>([\s\S]*?)<\/body>/i)
    content = bodyMatch ? bodyMatch[1] : html
  }

  return { css, content }
}

function fixImagePaths(content, htmlDir) {
  const mimeMap = { '.png': 'image/png', '.jpg': 'image/jpeg', '.jpeg': 'image/jpeg', '.gif': 'image/gif', '.svg': 'image/svg+xml', '.webp': 'image/webp' }
  return content.replace(/src="(?!https?:|\/\/|data:|file:|#)([^"]+)"/g, (match, src) => {
    try {
      const decoded = decodeURIComponent(src)
      const absPath = path.resolve(htmlDir, decoded)
      if (!fs.existsSync(absPath)) return match
      const ext = path.extname(absPath).toLowerCase()
      const mime = mimeMap[ext] || 'application/octet-stream'
      const data = fs.readFileSync(absPath)
      const base64 = data.toString('base64')
      return 'src="data:' + mime + ';base64,' + base64 + '"'
    } catch {
      return match
    }
  })
}

function filterContentByChip(content, chipFilter) {
  if (chipFilter === 'all') {
    content = content.replace(/<!--\s*npu\s*=\s*"[^"]*"\s+\w+\s*-->/gi, '')
    content = content.replace(/<!--\s*end\s+\w+\s*-->/gi, '')
    content = content.replace(/<\/?cann-filter\b[^>]*>/gi, '')
    return content
  }

  content = content.replace(
    /<!--\s*npu\s*=\s*"([^"]*)"\s+(\w+)\s*-->([\s\S]*?)<!--\s*end\s+\2\s*-->/gi,
    (match, npuType, id, innerContent) => {
      const npuValues = npuType.split(',').map(s => s.trim())
      return npuValues.includes(chipFilter) ? innerContent : ''
    }
  )
  content = content.replace(/<!--\s*npu\s*=\s*"[^"]*"\s+\w+\s*-->/gi, '')
  content = content.replace(/<!--\s*end\s+\w+\s*-->/gi, '')

  content = content.replace(
    /<cann-filter\b[^>]*npu[_-]type\s*=\s*"([^"]*)"[^>]*>([\s\S]*?)<\/cann-filter>/gi,
    (match, npuType, innerContent) => {
      const npuValues = npuType.split(',').map(s => s.trim())
      return npuValues.includes(chipFilter) ? innerContent : ''
    }
  )

  return content
}

function cleanContent(content, chipFilter) {
  content = filterContentByChip(content, chipFilter || 'all')
  content = content.replace(/<p>\s*CANNFILTER_DIV_[A-Za-z0-9_,]+_(?:OPEN|CLOSE)\s*<\/p>/gi, '')
  content = content.replace(/CANNFILTER_DIV_[A-Za-z0-9_,]+_(?:OPEN|CLOSE)/gi, '')
  content = content.replace(/<\/?cann-filter\b[^>]*>/gi, '')
  content = content.replace(/<(term|ph|__gm__|__ubuf__)\b[^>]*>([\s\S]*?)<\/\1>/gi, '$2')
  content = content.replace(/<\/?(term|ph|__gm__|__ubuf__)\b[^>]*>/gi, '')
  content = content.replace(/<div class="code-header\b[^"]*">[\s\S]*?<\/div>\s*/gi, '')
  content = content.replace(/<pre\b([^>]*)>([\s\S]*?)<\/pre>/gi, (match, attrs, inner) => {
    return '<pre' + attrs + '>' + inner.replace(/<\/?span\b[^>]*>/gi, '') + '</pre>'
  })
  return content
}

function fixInternalLinks(content, sourceRelPath, htmlToSecIdMap, fragmentLinks, fragmentCounter, currentSecId) {
  return content.replace(/<a\s+([^>]*)>/gi, (fullMatch, attrs) => {
    const hrefMatch = attrs.match(/href="([^"]*)"/i)
    if (!hrefMatch) return fullMatch
    const href = hrefMatch[1]

    if (href.startsWith('http://') || href.startsWith('https://') || href.startsWith('mailto:')) {
      return fullMatch
    }

    if (href.startsWith('#')) {
      let samePageFragment
      try {
        samePageFragment = decodeURIComponent(href.substring(1))
      } catch {
        samePageFragment = href.substring(1)
      }
      if (samePageFragment && currentSecId) {
        const key = currentSecId + '_f' + (fragmentCounter[currentSecId] = (fragmentCounter[currentSecId] || 0) + 1)
        fragmentLinks[key] = { secId: currentSecId, fragment: samePageFragment }
        const newHref = '#' + key
        const newAttrs = attrs.replace(/href="[^"]*"/i, 'href="' + newHref + '"')
        return '<a ' + newAttrs + '>'
      }
      return fullMatch
    }

    let decoded
    try {
      decoded = decodeURIComponent(href)
    } catch {
      return fullMatch
    }

    let fragment = ''
    let pathPart = decoded
    const hashIdx = decoded.indexOf('#')
    if (hashIdx >= 0) {
      fragment = decoded.substring(hashIdx + 1)
      pathPart = decoded.substring(0, hashIdx)
    }

    if (!pathPart || pathPart === '.' || pathPart === './') {
      return fullMatch
    }

    if (decoded.startsWith('/')) {
      if (decoded.startsWith('/guide/')) {
        let externalUrl = 'https://asc.gitcode.com' + decoded
        if (fragment) externalUrl += '#' + fragment
        const newAttrs = attrs.replace(/href="[^"]*"/i, 'href="' + externalUrl + '"')
        return '<a ' + newAttrs + '>'
      }
      if (decoded.startsWith('/api/')) {
        const apiRelativePath = decoded.substring('/api/'.length)
        const targetSecId = htmlToSecIdMap[apiRelativePath]
        if (targetSecId) {
          if (fragment) {
            const key = targetSecId + '_f' + (fragmentCounter[targetSecId] = (fragmentCounter[targetSecId] || 0) + 1)
            fragmentLinks[key] = { secId: targetSecId, fragment: fragment }
            const newHref = '#' + key
            const newAttrs = attrs.replace(/href="[^"]*"/i, 'href="' + newHref + '"')
            return '<a ' + newAttrs + '>'
          }
          const newHref = '#' + targetSecId
          const newAttrs = attrs.replace(/href="[^"]*"/i, 'href="' + newHref + '"')
          return '<a ' + newAttrs + '>'
        }
      }
      const newAttrs = attrs.replace(/href="[^"]*"/i, '')
      return '<a ' + newAttrs + '>'
    }

    const srcDir = sourceRelPath.includes('/') ? sourceRelPath.substring(0, sourceRelPath.lastIndexOf('/')) : ''
    const parts = srcDir ? srcDir.split('/') : []
    const targetParts = pathPart.split('/')
    const resolved = []
    for (const part of parts) {
      resolved.push(part)
    }
    for (const part of targetParts) {
      if (part === '..') {
        resolved.pop()
      } else if (part && part !== '.') {
        resolved.push(part)
      }
    }
    const normalizedKey = resolved.join('/')

    const targetSecId = htmlToSecIdMap[normalizedKey]
    if (targetSecId) {
      if (fragment) {
        const key = targetSecId + '_f' + (fragmentCounter[targetSecId] = (fragmentCounter[targetSecId] || 0) + 1)
        fragmentLinks[key] = { secId: targetSecId, fragment: fragment }
        const newHref = '#' + key
        const newAttrs = attrs.replace(/href="[^"]*"/i, 'href="' + newHref + '"')
        return '<a ' + newAttrs + '>'
      }
      const newHref = '#' + targetSecId
      const newAttrs = attrs.replace(/href="[^"]*"/i, 'href="' + newHref + '"')
      return '<a ' + newAttrs + '>'
    }

    const newAttrs = attrs.replace(/href="[^"]*"/i, '')
    return '<a ' + newAttrs + '>'
  })
}

function escapeHtml(text) {
  return text
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
}

function buildTocHtml(sections, pageNumbers) {
  const items = sections.map((s, i) => {
    const indent = (s.depth || 0) * 1.2
    const pageStr = pageNumbers ? String(pageNumbers[i] || '—') : '···'
    const boldStyle = (s.depth || 0) === 0 ? ' font-weight:bold;' : ''
    return '<li class="toc-entry" data-sec="' + s.secId + '" style="margin-left:' + indent + 'em;">' +
      '<a href="#' + s.secId + '" class="toc-link" style="' + boldStyle + '">' +
      '<span class="toc-text">' + escapeHtml(s.text) + '</span>' +
      '<span class="toc-dots"></span>' +
      '<span class="toc-page">' + pageStr + '</span>' +
      '</a></li>'
  }).join('\n')

  return '<div class="pdf-toc">\n' +
    '<div class="pdf-toc-title">目录</div>\n' +
    '<ul class="pdf-toc-list">\n' + items + '\n</ul>\n' +
    '</div>'
}

function extractOutlinePageList(batchDoc) {
  const { PDFName, PDFRef } = require('pdf-lib')
  const context = batchDoc.context
  const pages = batchDoc.getPages()
  const result = []

  const outlinesRef = batchDoc.catalog.get(PDFName.of('Outlines'))
  if (!outlinesRef || !(outlinesRef instanceof PDFRef)) return result

  let outlines
  try { outlines = context.lookup(outlinesRef) } catch { return result }
  if (!outlines) return result

  function resolvePageIndex(pageRef) {
    if (!(pageRef instanceof PDFRef)) return -1
    for (let i = 0; i < pages.length; i++) {
      if (pages[i].ref.objectNumber === pageRef.objectNumber) return i
    }
    return -1
  }

  function walkOutline(itemRef) {
    if (!itemRef || !(itemRef instanceof PDFRef)) return
    let item
    try { item = context.lookup(itemRef) } catch { return }
    if (!item) return

    const dest = item.get(PDFName.of('Dest'))
    if (dest) {
      try {
        const pageRef = dest.get(0)
        const pageIndex = resolvePageIndex(pageRef)
        if (pageIndex >= 0) result.push(pageIndex)
      } catch {}
    }

    const first = item.get(PDFName.of('First'))
    if (first) walkOutline(first)
    const next = item.get(PDFName.of('Next'))
    if (next) walkOutline(next)
  }

  const firstRef = outlines.get(PDFName.of('First'))
  if (firstRef) walkOutline(firstRef)
  return result
}

function extractNamedDests(batchDoc) {
  const { PDFName, PDFRef, PDFDict, PDFArray, PDFString, PDFHexString } = require('pdf-lib')
  const context = batchDoc.context
  const pages = batchDoc.getPages()
  const result = {}

  function resolvePageIndex(pageRef) {
    if (!(pageRef instanceof PDFRef)) return -1
    for (let i = 0; i < pages.length; i++) {
      if (pages[i].ref.objectNumber === pageRef.objectNumber) return i
    }
    return -1
  }

  function parseDestArray(destArray) {
    try {
      if (typeof destArray.size !== 'function' || destArray.size() < 4) return null
      const pageRef = destArray.get(0)
      let pageIndex = -1
      try {
        const resolved = context.lookup(pageRef)
        if (resolved) {
          for (let i = 0; i < pages.length; i++) {
            if (pages[i].node === resolved || (pages[i].ref && pageRef.objectNumber === pages[i].ref.objectNumber)) {
              pageIndex = i
              break
            }
          }
        }
      } catch {}
      if (pageIndex < 0) pageIndex = resolvePageIndex(pageRef)
      if (pageIndex < 0) return null
      let yPt = 800
      try {
        const yObj = destArray.get(3)
        if (yObj && typeof yObj.asNumber === 'function') yPt = yObj.asNumber()
      } catch {}
      return { pageIndex, yPt }
    } catch { return null }
  }

  function extractNameValue(nameObj) {
    try {
      if (nameObj instanceof PDFString) return nameObj.asString()
      if (nameObj instanceof PDFHexString) return nameObj.decodeText()
      if (nameObj instanceof PDFName) {
        let s = nameObj.asString()
        return s.startsWith('/') ? s.substring(1) : s
      }
    } catch {}
    return null
  }

  function walkFlatDestsDict(destsDict) {
    if (!destsDict || !(destsDict instanceof PDFDict)) return
    for (const key of destsDict.keys()) {
      const name = extractNameValue(key)
      if (!name) continue
      const val = destsDict.get(key)
      let destObj
      try { destObj = context.lookup(val) || val } catch { destObj = val }
      let destArray = destObj
      if (destObj instanceof PDFDict) {
        const d = destObj.get(PDFName.of('D'))
        if (d) try { destArray = context.lookup(d) || d } catch { continue }
      }
      if (destArray && typeof destArray.size === 'function') {
        const parsed = parseDestArray(destArray)
        if (parsed && !result[name]) result[name] = parsed
      }
    }
  }

  function walkNameTree(dict) {
    if (!dict || !(dict instanceof PDFDict)) return

    const kids = dict.get(PDFName.of('Kids'))
    if (kids) {
      let kidsArray
      try { kidsArray = context.lookup(kids) || kids } catch { return }
      if (kidsArray && typeof kidsArray.size === 'function') {
        for (let i = 0; i < kidsArray.size(); i++) {
          let kid
          try { kid = context.lookup(kidsArray.get(i)) } catch { continue }
          if (kid) walkNameTree(kid)
        }
      }
    }

    const names = dict.get(PDFName.of('Names'))
    if (names) {
      let namesArray
      try { namesArray = context.lookup(names) || names } catch { return }
      if (namesArray && typeof namesArray.size === 'function') {
        for (let i = 0; i < namesArray.size() - 1; i += 2) {
          let nameObj
          try { nameObj = context.lookup(namesArray.get(i)) || namesArray.get(i) } catch { continue }
          let destObj
          try { destObj = context.lookup(namesArray.get(i + 1)) || namesArray.get(i + 1) } catch { continue }

          const name = extractNameValue(nameObj)
          if (!name) continue

          let destArray = destObj
          if (destObj instanceof PDFDict) {
            const d = destObj.get(PDFName.of('D'))
            if (d) try { destArray = context.lookup(d) || d } catch { continue }
          }

          if (destArray && typeof destArray.size === 'function') {
            const parsed = parseDestArray(destArray)
            if (parsed && !result[name]) result[name] = parsed
          }
        }
      }
    }
  }

  try {
    const destsRef = batchDoc.catalog.get(PDFName.of('Dests'))
    if (destsRef) {
      let destsObj
      try { destsObj = context.lookup(destsRef) || destsRef } catch { destsObj = destsRef }
      if (destsObj instanceof PDFDict) {
        const hasKids = destsObj.get(PDFName.of('Kids'))
        const hasNames = destsObj.get(PDFName.of('Names'))
        if (hasKids || hasNames) {
          walkNameTree(destsObj)
        } else {
          walkFlatDestsDict(destsObj)
        }
      }
    }
  } catch {}

  try {
    const namesRef = batchDoc.catalog.get(PDFName.of('Names'))
    if (namesRef) {
      let namesDict
      try { namesDict = context.lookup(namesRef) || namesRef } catch { return result }
      if (namesDict instanceof PDFDict) {
        const dRef = namesDict.get(PDFName.of('Dests'))
        if (dRef) {
          let dDict
          try { dDict = context.lookup(dRef) || dRef } catch { return result }
          if (dDict instanceof PDFDict) {
            const hasKids = dDict.get(PDFName.of('Kids'))
            const hasNames = dDict.get(PDFName.of('Names'))
            if (hasKids || hasNames) {
              walkNameTree(dDict)
            } else {
              walkFlatDestsDict(dDict)
            }
          }
        }
      }
    }
  } catch {}

  return result
}

async function injectMathJax(page) {
  const mathjaxPath = path.resolve(root, 'node_modules', 'mathjax-full', 'es5', 'tex-svg.js')
  if (!fs.existsSync(mathjaxPath)) {
    console.warn('[gen-pdf] MathJax not found at ' + mathjaxPath + ', skipping math rendering')
    return
  }
  await page.evaluate(() => {
    window.MathJax = {
      tex: {
        inlineMath: [['$', '$']],
        displayMath: [['$$', '$$']],
        processEscapes: true,
        skipHtmlTags: ['script', 'noscript', 'style', 'textarea', 'pre', 'code', 'option']
      },
      svg: { fontCache: 'none' },
      startup: { typeset: true }
    }
  })
  await page.addScriptTag({ path: mathjaxPath })
  let ready = false
  for (let i = 0; i < 60; i++) {
    ready = await page.evaluate(() =>
      !!(window.MathJax && window.MathJax.startup && window.MathJax.startup.promise)
    )
    if (ready) break
    await new Promise(resolve => setTimeout(resolve, 500))
  }
  if (!ready) {
    console.warn('[gen-pdf] MathJax startup not detected after 30s, skipping math rendering')
    return
  }
  await page.evaluate(() => window.MathJax.startup.promise)
  await new Promise(resolve => setTimeout(resolve, 500))
}

async function renderBatchedPdf(browser, allCss, sections, anchorToSecIdMap, options) {
  const BATCH_SIZE = 300
  const { PDFDocument } = require('pdf-lib')
  const mergedDoc = await PDFDocument.create()
  const sectionPageMap = {}
  const destCoordMap = {}

  const sharedCss = allCss + '\n' +
    '@page { size: A4; margin: 15mm 15mm 20mm 15mm; }\n' +
    'body { margin: 0; padding: 0; }\n' +
    '.pdf-section-header { font-size: 10px; color: #888; margin-bottom: 10px; border-bottom: 1px solid #ddd; padding-bottom: 5px; }\n' +
    '.markdown-body { font-size: 13px; line-height: 1.6; max-width: none; }\n' +
    '.markdown-body table { width: 100%; border-collapse: collapse; }\n' +
    '.markdown-body pre { overflow-x: auto; white-space: pre-wrap; word-wrap: break-word; }\n' +
    '.markdown-body code { white-space: pre-wrap; word-wrap: break-word; }\n' +
    '.pdf-h1{font-size:2em;margin:.67em 0;font-weight:bold}' +
    '.pdf-h2{font-size:1.5em;margin:.75em 0;font-weight:bold}' +
    '.pdf-h3{font-size:1.17em;margin:.83em 0;font-weight:bold}' +
    '.pdf-h4{font-size:1em;margin:1.12em 0;font-weight:bold}' +
    '.pdf-h5{font-size:.83em;margin:1.5em 0;font-weight:bold}' +
    '.pdf-h6{font-size:.75em;margin:1.67em 0;font-weight:bold}' +
    '.pdf-tree-heading{border-bottom:1px solid #ddd;padding-bottom:.3em;margin-top:1em;}' +
    '.markdown-body a[href^="https://"] { color: #1a73e8; text-decoration: underline; }'

  let globalPageOffset = 0

  for (let start = 0; start < sections.length; start += BATCH_SIZE) {
    const end = Math.min(start + BATCH_SIZE, sections.length)
    const batch = sections.slice(start, end)
    console.log('[gen-pdf]   Render batch ' + (Math.floor(start / BATCH_SIZE) + 1) + ': sections ' + start + '-' + (end - 1))

    const referencedIds = new Set()
    for (const s of batch) {
      const hrefRe = /href="#(sec-\d+(?:_f\d+)?)"/g
      let m
      while ((m = hrefRe.exec(s.content)) !== null) {
        referencedIds.add(m[1])
      }
    }
    const existingIds = new Set(batch.map(s => s.secId))
    for (const s of batch) {
      const idRe = /\bid="(sec-\d+(?:_f\d+)?)"/gi
      let idM
      while ((idM = idRe.exec(s.content)) !== null) {
        existingIds.add(idM[1])
      }
    }
    const missingIds = [...referencedIds].filter(id => !existingIds.has(id))
    if (missingIds.length > 0) {
      console.log('[gen-pdf]     Injecting ' + missingIds.length + ' cross-batch anchors: ' + missingIds.slice(0, 5).join(',') + (missingIds.length > 5 ? ' ...' : ''))
    }
    const anchorHtml = missingIds.length > 0
      ? '\n' + missingIds.map(id => '<a id="' + id + '" style="position:absolute;left:-9999px;top:0"></a>').join('\n')
      : ''

    const batchHtml = '<!DOCTYPE html>\n<html lang="zh-CN">\n<head>\n<meta charset="utf-8">\n<style>\n' +
      sharedCss + '\n</style>\n</head>\n<body>\n' +
      batch.map((s, i) => {
        const breakStyle = i === 0 ? '' : ' style="page-break-before: always;"'
        const body = downgradeHeadings(s.content)
        const hLevel = Math.min(s.depth + 1, 6)
        const heading = '<h' + hLevel + ' class="pdf-tree-heading" id="' + s.secId + '">' + escapeHtml(s.text) + '</h' + hLevel + '>'
        return '<div class="pdf-section"' + breakStyle + '>\n' + heading +
          '<div class="pdf-section-header">' + s.relPath + '</div>\n' +
          '<article class="markdown-body">' + body + '</article>\n</div>'
      }).join('\n') +
      anchorHtml + '\n</body>\n</html>'

    const page = await browser.newPage()
    try {
      await page.setContent(batchHtml, { waitUntil: 'load', timeout: 300000 })
      await injectMathJax(page)

      const pdfData = await page.pdf({
        format: 'A4',
        margin: { top: '15mm', bottom: '20mm', left: '15mm', right: '15mm' },
        printBackground: true,
        outline: true,
        timeout: 300000,
      })

      const batchDoc = await PDFDocument.load(pdfData)
      const batchPages = batchDoc.getPageCount()

      const batchDests = extractNamedDests(batchDoc)
      for (const [name, coord] of Object.entries(batchDests)) {
        destCoordMap[name] = {
          pageIndex: coord.pageIndex + globalPageOffset,
          yPt: coord.yPt,
        }
      }

      const outlinePages = extractOutlinePageList(batchDoc)

      for (let bi = 0; bi < batch.length; bi++) {
        if (bi < outlinePages.length) {
          sectionPageMap[batch[bi].secId] = outlinePages[bi] + 1 + globalPageOffset
        } else {
          sectionPageMap[batch[bi].secId] = batchPages + globalPageOffset
        }
      }

      console.log('[gen-pdf]     Outline entries: ' + outlinePages.length + '/' + batch.length + ' sections matched')

      const copiedPages = await mergedDoc.copyPages(batchDoc, Array.from({ length: batchPages }, (_, i) => i))
      for (const p of copiedPages) mergedDoc.addPage(p)
      globalPageOffset += batchPages
      console.log('[gen-pdf]     ' + batchPages + ' pages, cumulative: ' + globalPageOffset)
    } finally {
      await page.close()
    }
  }

  return { mergedDoc, sectionPageMap, totalPages: globalPageOffset, destCoordMap }
}

async function renderTitlePdf(browser, pdfTitle) {
  const title = pdfTitle || 'Ascend C API 完整文档'
  const html = '<!DOCTYPE html>\n<html lang="zh-CN">\n<head>\n<meta charset="utf-8">\n<style>\n' +
    '@page { size: A4; margin: 15mm 15mm 20mm 15mm; }\n' +
    'body { margin: 0; padding: 0; }\n' +
    '.pdf-title { font-size: 28px; text-align: center; margin: 200px 0 50px; font-family: sans-serif; }\n' +
    '</style>\n</head>\n<body>\n' +
    '<div class="pdf-title">' + escapeHtml(title) + '</div>\n' +
    '</body>\n</html>'

  const page = await browser.newPage()
  try {
    await page.setContent(html, { waitUntil: 'load', timeout: 300000 })
    const pdfData = await page.pdf({
      format: 'A4',
      margin: { top: '15mm', bottom: '20mm', left: '15mm', right: '15mm' },
      printBackground: true,
      timeout: 300000,
    })
    const { PDFDocument } = require('pdf-lib')
    const titleDoc = await PDFDocument.load(pdfData)
    return { titleDoc, pageCount: titleDoc.getPageCount() }
  } finally {
    await page.close()
  }
}

async function renderTocPdf(browser, allCss, sections, pageNumbers, pdfTitle) {
  const tocHtml = buildTocHtml(sections, pageNumbers)
  const html = '<!DOCTYPE html>\n<html lang="zh-CN">\n<head>\n<meta charset="utf-8">\n<style>\n' +
    allCss + '\n' +
    '@page { size: A4; margin: 15mm 15mm 20mm 15mm; }\n' +
    'body { margin: 0; padding: 0; }\n' +
    '.pdf-toc{padding:0 20px;}' +
    '.pdf-toc-title{font-size:24px;text-align:center;margin:30px 0 20px;font-weight:bold;font-family:sans-serif;}' +
    '.pdf-toc-list{list-style:none;padding:0;margin:0;font-size:12px;line-height:1.8;font-family:sans-serif;}' +
    '.pdf-toc-list li{overflow:hidden;}' +
    '.toc-link{display:flex;align-items:baseline;text-decoration:none;color:#1a73e8;}' +
    '.toc-text{white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}' +
    '.toc-dots{flex:1;border-bottom:1px dotted #999;margin:0 4px;min-width:20px;}' +
    '.toc-page{white-space:nowrap;color:#333;font-variant-numeric:tabular-nums;}' +
    '</style>\n</head>\n<body>\n' +
    tocHtml + '\n</body>\n</html>'

  const page = await browser.newPage()
  try {
    await page.setContent(html, { waitUntil: 'load', timeout: 300000 })
    const pdfData = await page.pdf({
      format: 'A4',
      margin: { top: '15mm', bottom: '20mm', left: '15mm', right: '15mm' },
      printBackground: true,
      timeout: 300000,
    })
    const { PDFDocument } = require('pdf-lib')
    const tocDoc = await PDFDocument.load(pdfData)
    return { tocDoc, pageCount: tocDoc.getPageCount() }
  } finally {
    await page.close()
  }
}

async function collectTocPositions(browser, allCss, sections, pageNumbers, tocPageCount) {
  const ptPerPx = 72 / 96
  const marginTopPt = 15 * 72 / 25.4
  const marginBottomPt = 20 * 72 / 25.4
  const contentHeightPt = 842 - marginTopPt - marginBottomPt
  const contentHeightPx = contentHeightPt * 96 / 72
  const entryHeightPx = 21.6
  const titleHeightPx = 30 + 24 * 96 / 72 + 20

  const firstPageEntries = Math.floor((contentHeightPx - titleHeightPx) / entryHeightPx)
  const otherPageEntries = Math.floor(contentHeightPx / entryHeightPx)

  const tocCss = '@page { size: A4; margin: 15mm 15mm 20mm 15mm; }' +
    'body { margin: 0; padding: 0; }' +
    '.pdf-toc{padding:0 20px;}' +
    '.pdf-toc-title{font-size:24px;text-align:center;margin:30px 0 20px;font-weight:bold;font-family:sans-serif;}' +
    '.pdf-toc-list{list-style:none;padding:0;margin:0;font-size:12px;line-height:1.8;font-family:sans-serif;}' +
    '.pdf-toc-list li{overflow:hidden;}' +
    '.toc-link{display:flex;align-items:baseline;text-decoration:none;color:#1a73e8;}' +
    '.toc-text{white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}' +
    '.toc-dots{flex:1;border-bottom:1px dotted #999;margin:0 4px;min-width:20px;}' +
    '.toc-page{white-space:nowrap;color:#333;font-variant-numeric:tabular-nums;}'

  const allEntries = []
  const pPage = await browser.newPage()
  try {
    let secIdx = 0
    for (let p = 0; p < tocPageCount; p++) {
      const maxEntries = p === 0 ? firstPageEntries : otherPageEntries
      let pageSections = sections.slice(secIdx, secIdx + maxEntries)
      let pageNums = pageNumbers.slice(secIdx, secIdx + maxEntries)

      let actualCount = pageSections.length
      for (let attempt = 0; attempt < 3 && pageSections.length > 0; attempt++) {
        let pageHtml = '<!DOCTYPE html><html lang="zh-CN"><head><meta charset="utf-8"><style>' +
          allCss + '\n' + tocCss + '</style></head><body>'

        if (p === 0) {
          pageHtml += '<div class="pdf-toc"><div class="pdf-toc-title">\u76EE\u5F55</div>'
        } else {
          pageHtml += '<div class="pdf-toc">'
        }

        pageHtml += '<ul class="pdf-toc-list">'
        for (let i = 0; i < pageSections.length; i++) {
          const s = pageSections[i]
          const indent = (s.depth || 0) * 1.2
          const pageStr = pageNums ? String(pageNums[i] || '\u2014') : '\u00B7\u00B7\u00B7'
          const boldStyle = (s.depth || 0) === 0 ? ' font-weight:bold;' : ''
          pageHtml += '<li class="toc-entry" data-sec="' + s.secId + '" style="margin-left:' + indent + 'em;">' +
            '<a href="#' + s.secId + '" class="toc-link" style="' + boldStyle + '">' +
            '<span class="toc-text">' + escapeHtml(s.text) + '</span>' +
            '<span class="toc-dots"></span>' +
            '<span class="toc-page">' + pageStr + '</span>' +
            '</a></li>'
        }
        pageHtml += '</ul></div></body></html>'

        await pPage.setContent(pageHtml, { waitUntil: 'load', timeout: 300000 })
        await pPage.emulateMediaType('print')
        await new Promise(resolve => setTimeout(resolve, 500))

        if (attempt === 0) {
          const pdfCheck = await pPage.pdf({
            format: 'A4',
            margin: { top: '15mm', bottom: '20mm', left: '15mm', right: '15mm' },
            printBackground: true,
            timeout: 300000,
          })
          const { PDFDocument: PDFTmp } = require('pdf-lib')
          const checkDoc = await PDFTmp.load(pdfCheck)
          if (checkDoc.getPageCount() <= 1) break
          pageSections = pageSections.slice(0, pageSections.length - 1)
          pageNums = pageNums.slice(0, pageNums.length - 1)
          actualCount = pageSections.length
          continue
        }
        break
      }

      const positions = await pPage.evaluate(() => {
        const results = []
        const entries = document.querySelectorAll('.toc-entry')
        for (const entry of entries) {
          const rect = entry.getBoundingClientRect()
          results.push({
            secId: entry.dataset.sec || '',
            top: rect.top,
            height: rect.height,
          })
        }
        return results
      })

      for (const pos of positions) {
        const yFromTopPt = marginTopPt + pos.top * ptPerPx
        allEntries.push({
          secId: pos.secId,
          tocPageIndex: p + 1,
          yFromTop: yFromTopPt,
          height: pos.height,
        })
      }

      secIdx += actualCount
    }
  } finally {
    await pPage.close()
  }

  return allEntries
}

async function addTocLinkAnnotations(mergedDoc, tocEntries, sectionPageMap, destCoordMap) {
  const { PDFName, PDFNumber } = require('pdf-lib')
  const context = mergedDoc.context
  const pages = mergedDoc.getPages()
  const entryHeightPt = 21.6 * 72 / 96

  for (const entry of tocEntries) {
    const secId = entry.secId
    const targetPageNum = sectionPageMap[secId]
    if (targetPageNum === undefined || targetPageNum === null) continue

    const targetPageIndex = targetPageNum - 1
    if (targetPageIndex < 0 || targetPageIndex >= pages.length) continue

    const tocPageIndex = entry.tocPageIndex
    if (tocPageIndex < 0 || tocPageIndex >= pages.length) continue

    const yPtFromTop = entry.yFromTop
    const yPtFromBottom = 842 - yPtFromTop - entryHeightPt

    const targetPage = pages[targetPageIndex]
    let yPt = 800
    const coord = destCoordMap[secId]
    if (coord && coord.yPt !== undefined) {
      yPt = coord.yPt
    }
    const dest = context.obj([
      targetPage.ref,
      PDFName.of('XYZ'),
      PDFNumber.of(0),
      PDFNumber.of(yPt),
      PDFNumber.of(0),
    ])

    const linkAnnot = context.obj({
      Type: 'Annot',
      Subtype: 'Link',
      Rect: [30, yPtFromBottom, 550, yPtFromBottom + entryHeightPt],
      Dest: dest,
      Border: [0, 0, 0],
    })

    const tocPage = pages[tocPageIndex]
    const annots = tocPage.node.get(PDFName.of('Annots'))
    if (annots) {
      annots.push(linkAnnot)
    } else {
      tocPage.node.set(PDFName.of('Annots'), context.obj([linkAnnot]))
    }
  }
}

function decodePdfDestName(name) {
  if (!name || !name.includes('#')) return name
  let decoded = ''
  let i = 0
  while (i < name.length) {
    if (name[i] === '#' && i + 2 < name.length) {
      const hex = name.substring(i + 1, i + 3)
      if (/^[0-9A-Fa-f]{2}$/.test(hex)) {
        const byte = parseInt(hex, 16)
        decoded += String.fromCharCode(byte)
        i += 3
        continue
      }
    }
    decoded += name[i]
    i++
  }
  try {
    return decodeURIComponent(decoded)
  } catch {
    return decoded
  }
}

function fixContentLinkAnnotations(mergedDoc, sectionPageMap, anchorToSecIdMap, destCoordMap) {
  const { PDFName, PDFNumber, PDFArray } = require('pdf-lib')
  const context = mergedDoc.context
  const pages = mergedDoc.getPages()

  const decodedMap = {}
  for (const [key, val] of Object.entries(anchorToSecIdMap)) {
    decodedMap[key] = val
    const decoded = decodePdfDestName(key)
    if (decoded !== key) decodedMap[decoded] = val
  }

  let fixedCount = 0
  let notFoundCount = 0
  let totalAnnots = 0
  let namedDestCount = 0

  for (let i = 0; i < pages.length; i++) {
    const pg = pages[i]
    const annotsRaw = pg.node.get(PDFName.of('Annots'))
    if (!annotsRaw) continue

    let annotsArray
    try {
      annotsArray = context.lookup(annotsRaw) || annotsRaw
    } catch {
      annotsArray = annotsRaw
    }

    if (!annotsArray || typeof annotsArray.size !== 'function') continue

    for (let j = 0; j < annotsArray.size(); j++) {
      let annot
      try {
        const annotRef = annotsArray.get(j)
        annot = context.lookup(annotRef) || annotRef
      } catch { continue }

      if (!annot || typeof annot.get !== 'function') continue

      totalAnnots++

      const subtype = annot.get(PDFName.of('Subtype'))
      if (!subtype) continue
      const subStr = subtype.asString()
      if (subStr !== 'Link' && subStr !== '/Link') continue

      const dest = annot.get(PDFName.of('Dest'))
      if (!dest) continue

      if (!(dest instanceof PDFName)) continue
      namedDestCount++

      let destName = dest.asString()
      if (destName.startsWith('/')) destName = destName.substring(1)

      const coord = destCoordMap[destName] || destCoordMap[decodePdfDestName(destName)] || null
      if (coord) {
        const targetPageIndex = coord.pageIndex
        if (targetPageIndex >= 0 && targetPageIndex < pages.length) {
          const targetPage = pages[targetPageIndex]
          const newDest = context.obj([
            targetPage.ref,
            PDFName.of('XYZ'),
            PDFNumber.of(0),
            PDFNumber.of(coord.yPt),
            PDFNumber.of(0),
          ])
          annot.delete(PDFName.of('Dest'))
          annot.set(PDFName.of('Dest'), newDest)
          fixedCount++
          continue
        }
      }

      let targetSecId = null

      const aliasMatch = destName.match(/^(sec-\d+)_f\d+$/)
      if (aliasMatch) {
        targetSecId = aliasMatch[1]
      } else if (destName.startsWith('sec-')) {
        targetSecId = destName
      } else {
        targetSecId = decodedMap[destName] || decodedMap[decodePdfDestName(destName)] || null
      }

      if (!targetSecId) {
        notFoundCount++
        continue
      }

      const targetPageNum = sectionPageMap[targetSecId]
      if (targetPageNum === undefined || targetPageNum === null) {
        notFoundCount++
        continue
      }

      const targetPageIndex = targetPageNum - 1
      if (targetPageIndex < 0 || targetPageIndex >= pages.length) {
        notFoundCount++
        continue
      }

      const targetPage = pages[targetPageIndex]
      const newDest = context.obj([
        targetPage.ref,
        PDFName.of('XYZ'),
        PDFNumber.of(0),
        PDFNumber.of(800),
        PDFNumber.of(0),
      ])

      annot.delete(PDFName.of('Dest'))
      annot.set(PDFName.of('Dest'), newDest)
      fixedCount++
    }
  }

  console.log('[gen-pdf]     Scanned ' + totalAnnots + ' annotations, ' + namedDestCount + ' named dests, fixed ' + fixedCount + ', ' + notFoundCount + ' unresolvable')
}

function addContentLinkAnnotations(mergedDoc, allContentLinks, sectionPageMap, titlePageCount, destCoordMap) {
  return 0
}

async function buildOutlineForMergedPdf(mergedDoc, sections, sectionPageMap, tocPageCount, titlePageCount, destCoordMap) {
  const { PDFName, PDFHexString, PDFNumber, PDFDict, PDFArray } = require('pdf-lib')
  const context = mergedDoc.context
  const pages = mergedDoc.getPages()

  function toUnicodeHex(str) {
    const full = '\uFEFF' + str
    let hex = ''
    for (let i = 0; i < full.length; i++) {
      const code = full.charCodeAt(i)
      hex += ((code >> 8) & 0xFF).toString(16).padStart(2, '0').toUpperCase()
      hex += (code & 0xFF).toString(16).padStart(2, '0').toUpperCase()
    }
    return hex
  }

  function createOutlineItem(title, pageIndex, secId) {
    const pageRef = pages[pageIndex].ref
    let yPt = 800
    const coord = destCoordMap[secId]
    if (coord && coord.yPt !== undefined) {
      yPt = coord.yPt
    }
    const destArray = PDFArray.withContext(context)
    destArray.push(pageRef)
    destArray.push(PDFName.of('XYZ'))
    destArray.push(PDFNumber.of(0))
    destArray.push(PDFNumber.of(yPt))
    destArray.push(PDFNumber.of(0))

    const itemDict = PDFDict.withContext(context)
    itemDict.set(PDFName.of('Title'), PDFHexString.of(toUnicodeHex(title)))
    itemDict.set(PDFName.of('Dest'), destArray)
    const itemRef = context.register(itemDict)
    return { dict: itemDict, ref: itemRef }
  }

  function linkSiblings(items) {
    for (let i = 0; i < items.length; i++) {
      if (i > 0) items[i].dict.set(PDFName.of('Prev'), items[i - 1].ref)
      if (i < items.length - 1) items[i].dict.set(PDFName.of('Next'), items[i + 1].ref)
    }
  }

  function finalizeItem(item, children) {
    if (children.length === 0) return
    linkSiblings(children)
    item.dict.set(PDFName.of('First'), children[0].ref)
    item.dict.set(PDFName.of('Last'), children[children.length - 1].ref)
    item.dict.set(PDFName.of('Count'), PDFNumber.of(children.length))
    for (const child of children) {
      child.dict.set(PDFName.of('Parent'), item.ref)
    }
  }

  const stack = []
  const rootItems = []

  for (let i = 0; i < sections.length; i++) {
    const sec = sections[i]
    const globalPageNum = sectionPageMap[sec.secId]
    if (globalPageNum === undefined) continue

    const pageIndex = globalPageNum - 1
    if (pageIndex < 0 || pageIndex >= pages.length) continue

    const item = createOutlineItem(sec.text, pageIndex, sec.secId)

    while (stack.length > 0 && stack[stack.length - 1].depth >= sec.depth) {
      const popped = stack.pop()
      finalizeItem(popped.outlineItem, popped.children)
      if (stack.length > 0) {
        stack[stack.length - 1].children.push(popped.outlineItem)
      } else {
        rootItems.push(popped.outlineItem)
      }
    }

    stack.push({ depth: sec.depth, outlineItem: item, children: [] })
  }

  while (stack.length > 0) {
    const popped = stack.pop()
    finalizeItem(popped.outlineItem, popped.children)
    if (stack.length > 0) {
      stack[stack.length - 1].children.push(popped.outlineItem)
    } else {
      rootItems.push(popped.outlineItem)
    }
  }

  if (rootItems.length === 0) return

  const tocItem = createOutlineItem('\u76EE\u5F55', titlePageCount, null)
  rootItems.unshift(tocItem)

  linkSiblings(rootItems)

  const outlinesDict = PDFDict.withContext(context)
  const outlinesRef = context.register(outlinesDict)

  outlinesDict.set(PDFName.of('Type'), PDFName.of('Outlines'))
  outlinesDict.set(PDFName.of('First'), rootItems[0].ref)
  outlinesDict.set(PDFName.of('Last'), rootItems[rootItems.length - 1].ref)
  outlinesDict.set(PDFName.of('Count'), PDFNumber.of(rootItems.length))

  for (const rootItem of rootItems) {
    rootItem.dict.set(PDFName.of('Parent'), outlinesRef)
  }

  mergedDoc.catalog.set(PDFName.of('Outlines'), outlinesRef)
}

function downgradeHeadings(content) {
  return content.replace(/<h([1-6])\b([^>]*)>([\s\S]*?)<\/h\1>/gi, (match, level, attrs, inner) => {
    return '<div class="pdf-h' + level + '"' + attrs + '>' + inner + '</div>'
  })
}

const CHIP_FILTER_OPTIONS = [
  { value: 'all', label: '全量' },
  { value: '950', label: 'Ascend 950' },
  { value: 'A3', label: 'Atlas A3' },
  { value: '910b', label: 'Atlas A2' },
  { value: '910', label: 'Atlas 训练系列' },
  { value: '310p', label: 'Atlas 推理系列' },
  { value: '310b', label: 'Atlas 200I/500 A2' },
  { value: 'x90', label: 'Kirin X90' },
  { value: '9030', label: 'Kirin 9030' },
]

function cleanOutputDir() {
  if (!fs.existsSync(outDir)) return
  const files = fs.readdirSync(outDir)
  for (const file of files) {
    if (file.endsWith('.pdf') || file.endsWith('.tmp')) {
      try {
        fs.unlinkSync(path.join(outDir, file))
        console.log('[gen-pdf]   Removed old file: ' + file)
      } catch {}
    }
  }
}

function isSectionNotSupported(rawContent, chipFilter) {
  if (chipFilter === 'all') return false

  const supportIdx = rawContent.indexOf('<h2>产品支持情况')
  if (supportIdx === -1) return false

  const supportSection = rawContent.substring(supportIdx, supportIdx + 5000)

  const nextH2Idx = supportSection.indexOf('<h2', 4)
  const sectionEnd = nextH2Idx > 0 ? supportIdx + nextH2Idx : supportIdx + 5000
  const supportBlock = rawContent.substring(supportIdx, sectionEnd)

  const blockRegex = /<!--\s*npu\s*=\s*"([^"]*)"\s+\w+\s*-->([\s\S]*?)<!--\s*end\s+\w+\s*-->/gi
  let match
  let foundForChip = false
  let hasSupport = false

  while ((match = blockRegex.exec(supportBlock)) !== null) {
    const npuType = match[1]
    const innerContent = match[2]
    const npuValues = npuType.split(',').map(s => s.trim())

    if (npuValues.includes(chipFilter)) {
      foundForChip = true
      if (innerContent.includes('支持') && !innerContent.includes('不支持')) {
        hasSupport = true
      }
    }
  }

  if (!foundForChip) return false

  return !hasSupport
}

async function generateAllApiPdf(browser, allCss, rawSections, chipFilter, htmlToSecIdMap) {
  const chipOpt = CHIP_FILTER_OPTIONS.find(o => o.value === chipFilter)
  const chipDisplayName = chipOpt ? chipOpt.label : chipFilter
  const pdfFileName = chipFilter === 'all' ? 'ALL-API.pdf' : 'ALL-API-' + chipFilter + '.pdf'
  const pdfTitle = chipFilter === 'all' ? 'Ascend C API 完整文档' : 'Ascend C API (' + chipDisplayName + ')'

  console.log('[gen-pdf] Generating ' + pdfFileName + ' (filter=' + chipFilter + ')...')

  let skippedCount = 0
  const fragmentLinks = {}
  const fragmentCounter = {}
  const sections = rawSections.map(rs => {
    if (isSectionNotSupported(rs.rawContent, chipFilter)) {
      skippedCount++
      return null
    }
    const cleaned = cleanContent(rs.rawContent, chipFilter)
    const fixed = fixImagePaths(cleaned, rs.htmlDir)
    const linked = fixInternalLinks(fixed, rs.relPath, htmlToSecIdMap, fragmentLinks, fragmentCounter, rs.secId)
    return {
      text: rs.text,
      depth: rs.depth,
      content: linked,
      relPath: rs.relPath,
      secId: rs.secId,
    }
  }).filter(s => s !== null && s.content.trim().length > 0)

  if (sections.length === 0) {
    console.warn('[gen-pdf]   No content after chip filtering for ' + chipFilter + ', skipping')
    return null
  }

  const anchorToSecIdMap = {}
  for (const s of sections) {
    const idRe = /\bid="([^"]*)"/gi
    let m
    while ((m = idRe.exec(s.content)) !== null) {
      anchorToSecIdMap[m[1]] = s.secId
    }
    const nameRe = /\bname="([^"]*)"/gi
    while ((m = nameRe.exec(s.content)) !== null) {
      if (!anchorToSecIdMap[m[1]]) {
        anchorToSecIdMap[m[1]] = s.secId
      }
    }
  }

  const secFragmentAliases = {}
  for (const [compoundKey, info] of Object.entries(fragmentLinks)) {
    if (!secFragmentAliases[info.secId]) secFragmentAliases[info.secId] = []
    secFragmentAliases[info.secId].push({ compoundKey, fragment: info.fragment })
  }

  let injectedById = 0
  let injectedByHeading = 0
  for (const s of sections) {
    const aliases = secFragmentAliases[s.secId]
    if (!aliases) continue

    const fragmentGroups = {}
    for (const alias of aliases) {
      if (!fragmentGroups[alias.fragment]) fragmentGroups[alias.fragment] = []
      fragmentGroups[alias.fragment].push(alias)
    }

    for (const [fragment, groupAliases] of Object.entries(fragmentGroups)) {
      const escapedFragment = fragment.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')
      const allIds = groupAliases.map(a => '<a id="' + a.compoundKey + '"></a>').join('')

      const idRe = new RegExp('(<[^>]+\\bid="' + escapedFragment + '"[^>]*>)', 'i')
      if (idRe.test(s.content)) {
        s.content = s.content.replace(idRe, allIds + '$1')
        for (const alias of groupAliases) {
          anchorToSecIdMap[alias.compoundKey] = s.secId
          injectedById++
        }
      } else {
        const headingRe = new RegExp('(<h[1-6][^>]*>)(' + escapedFragment + ')(?=<a\\s|</h[1-6]>)', 'i')
        if (headingRe.test(s.content)) {
          s.content = s.content.replace(headingRe, '$1' + allIds + '$2')
          for (const alias of groupAliases) {
            anchorToSecIdMap[alias.compoundKey] = s.secId
            injectedByHeading++
          }
        }
      }
    }
  }

  const aliasCount = Object.keys(fragmentLinks).length
  if (aliasCount > 0) {
    console.log('[gen-pdf]   Injected ' + aliasCount + ' fragment alias anchors across ' + Object.keys(secFragmentAliases).length + ' sections (' + injectedById + ' by id, ' + injectedByHeading + ' by heading)')
  }

  console.log('[gen-pdf]   ' + sections.length + '/' + rawSections.length + ' sections have content (' + skippedCount + ' skipped by chip support)')

  console.log('[gen-pdf]   Step 1: Batch rendering content sections...')
  const { mergedDoc, sectionPageMap, totalPages, destCoordMap } = await renderBatchedPdf(browser, allCss, sections, anchorToSecIdMap)
  console.log('[gen-pdf]     Content rendered: ' + totalPages + ' pages, ' + Object.keys(destCoordMap).length + ' named dests extracted')

  console.log('[gen-pdf]   Step 2: Rendering title page...')
  const { titleDoc, pageCount: titlePageCount } = await renderTitlePdf(browser, pdfTitle)
  console.log('[gen-pdf]     Title pages: ' + titlePageCount)

  console.log('[gen-pdf]   Step 3: Rendering placeholder TOC (for page count)...')
  const { pageCount: tocPageCount } =
    await renderTocPdf(browser, allCss, sections, sections.map(() => 1), pdfTitle)
  console.log('[gen-pdf]     TOC pages: ' + tocPageCount)

  const titleOffset = titlePageCount + tocPageCount
  const adjustedPageMap = {}
  for (const [secId, pageNum] of Object.entries(sectionPageMap)) {
    adjustedPageMap[secId] = pageNum + titleOffset
  }
  const adjustedDestCoordMap = {}
  for (const [name, coord] of Object.entries(destCoordMap)) {
    adjustedDestCoordMap[name] = {
      pageIndex: coord.pageIndex + titleOffset,
      yPt: coord.yPt,
    }
  }
  const finalPageNumbers = sections.map(s => {
    const p = adjustedPageMap[s.secId]
    return p !== undefined ? p : null
  })

  console.log('[gen-pdf]   Step 4: Rendering final TOC...')
  const { tocDoc: finalTocDoc } = await renderTocPdf(browser, allCss, sections, finalPageNumbers, pdfTitle)

  console.log('[gen-pdf]   Step 5: Collecting TOC entry positions (page-by-page)...')
  const tocEntries = await collectTocPositions(browser, allCss, sections, finalPageNumbers, tocPageCount)
  console.log('[gen-pdf]     Collected ' + tocEntries.length + ' entries')

  console.log('[gen-pdf]   Step 6: Merging title + TOC + content...')
  const { PDFDocument } = require('pdf-lib')
  const tocPages = await mergedDoc.copyPages(finalTocDoc, Array.from({ length: finalTocDoc.getPageCount() }, (_, i) => i))
  for (let i = tocPages.length - 1; i >= 0; i--) {
    mergedDoc.insertPage(0, tocPages[i])
  }
  const titlePages = await mergedDoc.copyPages(titleDoc, Array.from({ length: titleDoc.getPageCount() }, (_, i) => i))
  for (let i = titlePages.length - 1; i >= 0; i--) {
    mergedDoc.insertPage(0, titlePages[i])
  }

  console.log('[gen-pdf]   Step 7: Adding TOC link annotations...')
  await addTocLinkAnnotations(mergedDoc, tocEntries, adjustedPageMap, adjustedDestCoordMap)

  console.log('[gen-pdf]   Step 7b: Fixing content link annotations (Named Dest -> Direct Dest)...')
  fixContentLinkAnnotations(mergedDoc, adjustedPageMap, anchorToSecIdMap, adjustedDestCoordMap)

  console.log('[gen-pdf]   Step 7c: Checking cross-batch content link annotations...')
  const crossBatchAdded = addContentLinkAnnotations(mergedDoc, [], adjustedPageMap, titlePageCount + tocPageCount, adjustedDestCoordMap)
  console.log('[gen-pdf]     Cross-batch links handled by fixContentLinkAnnotations')

  console.log('[gen-pdf]   Step 8: Building outline bookmarks...')
  let outlineFailed = false
  try {
    await buildOutlineForMergedPdf(mergedDoc, sections, adjustedPageMap, tocPageCount, titlePageCount, adjustedDestCoordMap)
  } catch (outlineErr) {
    console.warn('[gen-pdf]     Outline build failed (non-fatal): ' + outlineErr.message)
    outlineFailed = true
  }

  console.log('[gen-pdf]   Step 9: Saving ' + pdfFileName + '...')
  let pdfBytes
  try {
    pdfBytes = await mergedDoc.save()
  } catch (saveErr) {
    if (!outlineFailed) {
      console.warn('[gen-pdf]     Save with outline failed: ' + saveErr.message)
      console.warn('[gen-pdf]     Retrying without outline...')
      try {
        const { PDFName: PDFName2 } = require('pdf-lib')
        mergedDoc.catalog.delete(PDFName2.of('Outlines'))
      } catch {}
      pdfBytes = await mergedDoc.save()
    } else {
      throw saveErr
    }
  }

  const pdfPath = path.join(outDir, pdfFileName)
  fs.writeFileSync(pdfPath, pdfBytes)
  const sizeMB = (fs.statSync(pdfPath).size / (1024 * 1024)).toFixed(1)
  console.log('[gen-pdf]   OK ' + pdfFileName + ' (' + sizeMB + ' MB, ' + (totalPages + titlePageCount + tocPageCount) + ' pages)')

  return pdfFileName
}

async function main() {
  if (!fs.existsSync(apiDir)) {
    console.error('[gen-pdf] API directory not found:', apiDir)
    console.error('[gen-pdf] Run "npm run docs:convert" first')
    process.exit(0)
  }

  const htmlCheck = fs.readdirSync(apiDir).some(name => {
    const full = path.join(apiDir, name)
    return fs.statSync(full).isDirectory() && name !== 'figures'
  })
  if (!htmlCheck) {
    console.warn('[gen-pdf] No subdirectories found in', apiDir)
    console.warn('[gen-pdf] Run "npm run docs:convert" first')
    process.exit(0)
  }

  const browserPath = findBrowser()
  if (!browserPath) {
    console.warn('[gen-pdf] No browser found (Edge/Chrome)')
    console.warn('[gen-pdf] Skipping PDF generation')
    console.warn('[gen-pdf] Install Microsoft Edge or Google Chrome, or set PUPPETEER_EXECUTABLE_PATH')
    process.exit(0)
  }

  console.log('[gen-pdf] Using browser:', browserPath)

  fs.mkdirSync(outDir, { recursive: true })
  cleanOutputDir()

  // Phase 1: Parse README tree and extract raw content
  console.log('[gen-pdf] Parsing README sidebar tree...')
  const tree = parseReadmeTree()
  if (tree.length === 0) {
    console.warn('[gen-pdf] README tree empty, exiting')
    process.exit(0)
  }

  console.log('[gen-pdf] Extracting raw content from tree (' + tree.length + ' root nodes)...')
  const { rawSections, allCss, htmlToSecIdMap } = collectRawSections(tree, apiDir)
  if (rawSections.length === 0) {
    console.warn('[gen-pdf] No sections extracted, exiting')
    process.exit(0)
  }
  console.log('[gen-pdf] Extracted ' + rawSections.length + ' sections')

  // Phase 2: Launch browsers and generate PDFs in parallel
  const chipFilters = CHIP_FILTER_OPTIONS.map(o => o.value)
  const PARALLEL = Math.min(parseInt(process.env.GEN_PDF_PARALLEL || '4', 10), chipFilters.length)

  console.log('[gen-pdf] Launching ' + PARALLEL + ' browser instances for ' + chipFilters.length + ' chip filters...')
  const puppeteer = require('puppeteer-core')
  const browsers = await Promise.all(
    Array.from({ length: PARALLEL }, () => puppeteer.launch({
      executablePath: browserPath,
      headless: 'new',
      args: ['--no-sandbox', '--disable-setuid-sandbox', '--disable-dev-shm-usage', '--disable-gpu', '--allow-file-access-from-files'],
    }))
  )

  const manifest = {}

  try {
    const browserQueues = Array.from({ length: PARALLEL }, () => [])
    chipFilters.forEach((chip, i) => browserQueues[i % PARALLEL].push(chip))

    await Promise.all(browserQueues.map(async (queue, bi) => {
      for (const chipFilter of queue) {
        try {
          const pdfFile = await generateAllApiPdf(browsers[bi], allCss, rawSections, chipFilter, htmlToSecIdMap)
          if (pdfFile) {
            const key = chipFilter === 'all' ? '__ALL__' : '__' + chipFilter + '__'
            manifest[key] = pdfFile
          }
        } catch (err) {
          console.error('[gen-pdf]   FAIL ' + chipFilter + ': ' + err.message)
        }
      }
    }))
  } finally {
    await Promise.all(browsers.map(b => b.close()))
  }

  const manifestPath = path.join(outDir, 'manifest.json')
  fs.writeFileSync(manifestPath, JSON.stringify(manifest, null, 2), 'utf-8')
  console.log('[gen-pdf] Manifest written: ' + manifestPath)
  console.log('[gen-pdf] Done! Generated ' + Object.keys(manifest).length + ' PDFs')
}

main().catch(err => {
  console.error('[gen-pdf] Fatal error:', err)
  process.exit(1)
})
