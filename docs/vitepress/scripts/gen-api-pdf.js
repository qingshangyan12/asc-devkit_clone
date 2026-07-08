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
    const m = line.match(/^(\s*)-\s+\[([^\]]+)\]\(([^)]+\.md)\)/)
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
  let secCounter = 0
  let allCss = ''
  function walk(nodeList) {
    for (const node of nodeList) {
      const htmlRel = node.mdPath.replace(/\.md$/, '.html')
      const htmlPath = path.join(vpApiDir, htmlRel)
      if (fs.existsSync(htmlPath)) {
        try {
          const parts = extractContent(htmlPath)
          if (!allCss && parts.css) allCss = parts.css
          rawSections.push({
            text: node.text,
            depth: node.depth,
            rawContent: parts.content,
            relPath: htmlRel,
            htmlDir: path.dirname(htmlPath),
            secId: 'sec-' + secCounter,
          })
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
  return { rawSections, allCss }
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
    return '<li class="toc-entry" data-sec="' + s.secId + '" style="margin-left:' + indent + 'em;">' +
      '<a href="#' + s.secId + '">' + escapeHtml(s.text) + '</a>' +
      '<span class="pdf-toc-pagenum">' + pageStr + '</span>' +
      '</li>'
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

async function renderBatchedPdf(browser, allCss, sections, options) {
  const BATCH_SIZE = 300
  const { PDFDocument } = require('pdf-lib')
  const mergedDoc = await PDFDocument.create()
  const sectionPageMap = {}

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
    '.pdf-tree-heading{border-bottom:1px solid #ddd;padding-bottom:.3em;margin-top:1em;}'

  let globalPageOffset = 0

  for (let start = 0; start < sections.length; start += BATCH_SIZE) {
    const end = Math.min(start + BATCH_SIZE, sections.length)
    const batch = sections.slice(start, end)
    console.log('[gen-pdf]   Render batch ' + (Math.floor(start / BATCH_SIZE) + 1) + ': sections ' + start + '-' + (end - 1))

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
      '\n</body>\n</html>'

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

  return { mergedDoc, sectionPageMap, totalPages: globalPageOffset }
}

async function renderTocPdf(browser, allCss, sections, pageNumbers, pdfTitle) {
  const title = pdfTitle || 'Ascend C API 完整文档'
  const tocHtml = buildTocHtml(sections, pageNumbers)
  const html = '<!DOCTYPE html>\n<html lang="zh-CN">\n<head>\n<meta charset="utf-8">\n<style>\n' +
    allCss + '\n' +
    '@page { size: A4; margin: 15mm 15mm 20mm 15mm; }\n' +
    'body { margin: 0; padding: 0; }\n' +
    '.pdf-title { font-size: 28px; text-align: center; margin: 200px 0 50px; font-family: sans-serif; page-break-after: always; }\n' +
    '.pdf-toc{page-break-after:always;padding:0 20px;}' +
    '.pdf-toc-title{font-size:24px;text-align:center;margin:30px 0 20px;font-weight:bold;font-family:sans-serif;}' +
    '.pdf-toc-list{list-style:none;padding:0;margin:0;font-size:11px;line-height:1.6;font-family:sans-serif;}' +
    '.pdf-toc-list li{white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}' +
    '.pdf-toc-list a{text-decoration:none;color:#333;}' +
    '.pdf-toc-pagenum{float:right;color:#666;font-variant-numeric:tabular-nums;margin-left:8px;}' +
    '</style>\n</head>\n<body>\n' +
    '<div class="pdf-title">' + escapeHtml(title) + '</div>\n' +
    tocHtml + '\n</body>\n</html>'

  const page = await browser.newPage()
  try {
    await page.setContent(html, { waitUntil: 'load', timeout: 300000 })
    await new Promise(resolve => setTimeout(resolve, 1000))

    const tocPositions = await page.evaluate(() => {
      const positions = []
      const entries = document.querySelectorAll('.toc-entry')
      for (const entry of entries) {
        const rect = entry.getBoundingClientRect()
        positions.push({
          secId: entry.dataset.sec || '',
          top: rect.top + window.scrollY,
          bottom: rect.bottom + window.scrollY,
          height: rect.height,
        })
      }
      return positions
    })

    const pdfData = await page.pdf({
      format: 'A4',
      margin: { top: '15mm', bottom: '20mm', left: '15mm', right: '15mm' },
      printBackground: true,
      timeout: 300000,
    })
    const { PDFDocument } = require('pdf-lib')
    const tocDoc = await PDFDocument.load(pdfData)
    return { tocDoc, pageCount: tocDoc.getPageCount(), tocPositions }
  } finally {
    await page.close()
  }
}

async function addTocLinkAnnotations(mergedDoc, tocPositions, sectionPageMap, tocPageCount) {
  const { PDFName, PDFNumber } = require('pdf-lib')
  const context = mergedDoc.context
  const pages = mergedDoc.getPages()

  const pageHeightPt = 842
  const marginTopPt = 15 * 72 / 25.4
  const contentHeightPt = pageHeightPt - marginTopPt - (20 * 72 / 25.4)
  const pxPerPt = 96 / 72
  const contentHeightPx = contentHeightPt * pxPerPt
  const ptPerPx = 72 / 96

  for (const pos of tocPositions) {
    const secId = pos.secId
    const targetPageNum = sectionPageMap[secId]
    if (targetPageNum === undefined || targetPageNum === null) continue

    const targetPageIndex = targetPageNum - 1
    if (targetPageIndex < 0 || targetPageIndex >= pages.length) continue

    const tocPageIndex = 1 + Math.floor(pos.top / contentHeightPx)
    if (tocPageIndex < 0 || tocPageIndex >= pages.length) continue

    const localPx = pos.top - (tocPageIndex - 1) * contentHeightPx
    const yPtFromTop = localPx * ptPerPx
    const yPtFromBottom = 842 - marginTopPt - yPtFromTop - pos.height * ptPerPx

    const targetPage = pages[targetPageIndex]
    const dest = context.obj([
      targetPage.ref,
      PDFName.of('XYZ'),
      PDFNumber.of(0),
      PDFNumber.of(800),
      PDFNumber.of(0),
    ])

    const linkAnnot = context.obj({
      Type: 'Annot',
      Subtype: 'Link',
      Rect: [30, yPtFromBottom, 550, yPtFromBottom + pos.height * ptPerPx],
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

async function buildOutlineForMergedPdf(mergedDoc, sections, sectionPageMap, tocPageCount) {
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

  function createOutlineItem(title, pageIndex) {
    const pageRef = pages[pageIndex].ref
    const destArray = PDFArray.withContext(context)
    destArray.push(pageRef)
    destArray.push(PDFName.of('XYZ'))
    destArray.push(PDFNumber.of(0))
    destArray.push(PDFNumber.of(800))
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

    const item = createOutlineItem(sec.text, pageIndex)

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

async function generateAllApiPdf(browser, allCss, rawSections, chipFilter) {
  const chipOpt = CHIP_FILTER_OPTIONS.find(o => o.value === chipFilter)
  const chipDisplayName = chipOpt ? chipOpt.label : chipFilter
  const pdfFileName = chipFilter === 'all' ? 'ALL-API.pdf' : 'ALL-API-' + chipFilter + '.pdf'
  const pdfTitle = chipFilter === 'all' ? 'Ascend C API 完整文档' : 'Ascend C API (' + chipDisplayName + ')'

  console.log('[gen-pdf] Generating ' + pdfFileName + ' (filter=' + chipFilter + ')...')

  const sections = rawSections.map(rs => {
    const cleaned = cleanContent(rs.rawContent, chipFilter)
    const fixed = fixImagePaths(cleaned, rs.htmlDir)
    return {
      text: rs.text,
      depth: rs.depth,
      content: fixed,
      relPath: rs.relPath,
      secId: rs.secId,
    }
  }).filter(s => s.content.trim().length > 0)

  if (sections.length === 0) {
    console.warn('[gen-pdf]   No content after chip filtering for ' + chipFilter + ', skipping')
    return null
  }

  console.log('[gen-pdf]   ' + sections.length + '/' + rawSections.length + ' sections have content')

  console.log('[gen-pdf]   Step 1: Batch rendering content sections...')
  const { mergedDoc, sectionPageMap, totalPages } = await renderBatchedPdf(browser, allCss, sections)
  console.log('[gen-pdf]     Content rendered: ' + totalPages + ' pages')

  console.log('[gen-pdf]   Step 2: Rendering placeholder TOC...')
  const { tocDoc: placeholderTocDoc, pageCount: tocPageCount, tocPositions } =
    await renderTocPdf(browser, allCss, sections, sections.map(() => 1), pdfTitle)
  console.log('[gen-pdf]     TOC pages: ' + tocPageCount)

  const titleOffset = 1 + tocPageCount
  const adjustedPageMap = {}
  for (const [secId, pageNum] of Object.entries(sectionPageMap)) {
    adjustedPageMap[secId] = pageNum + titleOffset
  }
  const finalPageNumbers = sections.map(s => {
    const p = adjustedPageMap[s.secId]
    return p !== undefined ? p : null
  })

  console.log('[gen-pdf]   Step 3: Rendering final TOC...')
  const { tocDoc: finalTocDoc } = await renderTocPdf(browser, allCss, sections, finalPageNumbers, pdfTitle)

  console.log('[gen-pdf]   Step 4: Merging TOC + content...')
  const { PDFDocument } = require('pdf-lib')
  const tocPages = await mergedDoc.copyPages(finalTocDoc, Array.from({ length: finalTocDoc.getPageCount() }, (_, i) => i))
  for (let i = tocPages.length - 1; i >= 0; i--) {
    mergedDoc.insertPage(0, tocPages[i])
  }

  console.log('[gen-pdf]   Step 5: Adding TOC link annotations...')
  await addTocLinkAnnotations(mergedDoc, tocPositions, adjustedPageMap, tocPageCount)

  console.log('[gen-pdf]   Step 6: Building outline bookmarks...')
  let outlineFailed = false
  try {
    await buildOutlineForMergedPdf(mergedDoc, sections, adjustedPageMap, tocPageCount)
  } catch (outlineErr) {
    console.warn('[gen-pdf]     Outline build failed (non-fatal): ' + outlineErr.message)
    outlineFailed = true
  }

  console.log('[gen-pdf]   Step 7: Saving ' + pdfFileName + '...')
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
  console.log('[gen-pdf]   OK ' + pdfFileName + ' (' + sizeMB + ' MB, ' + (totalPages + tocPageCount + 1) + ' pages)')

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
  const { rawSections, allCss } = collectRawSections(tree, apiDir)
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
          const pdfFile = await generateAllApiPdf(browsers[bi], allCss, rawSections, chipFilter)
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
