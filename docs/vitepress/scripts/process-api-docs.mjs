import { readFileSync, writeFileSync, readdirSync, mkdirSync, statSync, existsSync, renameSync } from 'node:fs'
import { join, relative, dirname } from 'node:path'

const ROOT = join(import.meta.dirname, '..')
const SOURCE_DIR = join(ROOT, 'docs', 'api', '_build', 'html', '_sources')
const TARGET_DIR = join(ROOT, 'docs', 'api')

function walkMdTxt(dir) {
  const results = []
  for (const entry of readdirSync(dir, { withFileTypes: true })) {
    const full = join(dir, entry.name)
    if (entry.isDirectory()) {
      results.push(...walkMdTxt(full))
    } else if (entry.name.endsWith('.md.txt')) {
      results.push(full)
    }
  }
  return results
}

function stripInlineTags(text) {
  return text
    .replace(/<a\s+name\s*=\s*["'][^"']*["'][^>]*>\s*<\/a>/gi, '')
    .replace(/<a\b[^>]*>([\s\S]*?)<\/a>/gi, '$1')
    .replace(/<(term|ph|__gm__|__ubuf__)\b[^>]*>([\s\S]*?)<\/\1>/gis, '$2')
    .replace(/<(span|strong|em|b|i|u|code|kbd|samp|var|mark|sub|sup|small|abbr|cite|dfn|time|q|font|center)\b[^>]*>/gi, '')
    .replace(/<\/(span|strong|em|b|i|u|code|kbd|samp|var|mark|sub|sup|small|abbr|cite|dfn|time|q|font|center)>/gi, '')
    .replace(/<\/?(p|div)\b[^>]*>/gi, ' ')
    .replace(/<br\s*\/?>/gi, ' ')
    .replace(/\s+/g, ' ')
    .trim()
}

function htmlTableToMarkdown(tableHtml) {
  const rowspanTracker = {}

  const rows = []
  const trRegex = /<tr\b[\s\S]*?<\/tr>/gi
  let trMatch
  while ((trMatch = trRegex.exec(tableHtml)) !== null) {
    const tr = trMatch[0]
    const cells = []
    const cellRegex = /<(th|td)\b([^>]*?)>([\s\S]*?)<\/\1>/gi
    let cellMatch
    while ((cellMatch = cellRegex.exec(tr)) !== null) {
      const attrs = cellMatch[2]
      const content = stripInlineTags(cellMatch[3])
      const rowspan = parseInt((attrs.match(/rowspan\s*=\s*["']?(\d+)/i) || [])[1]) || 1
      cells.push({ content, rowspan })
    }

    if (cells.length === 0) continue

    const finalCells = []
    let col = 0
    for (const cell of cells) {
      while (rowspanTracker[col] && rowspanTracker[col].count > 0) {
        finalCells.push(rowspanTracker[col].text)
        rowspanTracker[col].count--
        if (rowspanTracker[col].count === 0) delete rowspanTracker[col]
        col++
      }
      finalCells.push(cell.content)
      if (cell.rowspan > 1) {
        rowspanTracker[col] = { text: cell.content, count: cell.rowspan - 1 }
      }
      col++
    }
    while (rowspanTracker[col] && rowspanTracker[col].count > 0) {
      finalCells.push(rowspanTracker[col].text)
      rowspanTracker[col].count--
      if (rowspanTracker[col].count === 0) delete rowspanTracker[col]
      col++
    }

    rows.push(finalCells)
  }

  if (rows.length === 0) return '\n'

  const maxCols = Math.max(...rows.map(r => r.length))
  for (const row of rows) {
    while (row.length < maxCols) row.push('')
  }

  const lines = []
  for (let i = 0; i < rows.length; i++) {
    const escaped = rows[i].map(c => c.replace(/\|/g, '\\|').replace(/\n/g, ' '))
    lines.push('| ' + escaped.join(' | ') + ' |')
    if (i === 0) {
      lines.push('|' + rows[i].map(() => '---').join('|') + '|')
    }
  }
  return '\n' + lines.join('\n') + '\n'
}

function processContent(content) {
  const PLACEHOLDER = '\x00BLOCK\x00'
  const blocks = []

  content = content.replace(/(```+|~~~+)[\s\S]*?\1/g, (match) => {
    blocks.push(match)
    return PLACEHOLDER + (blocks.length - 1) + PLACEHOLDER
  })

  content = content.replace(/^(\|[^\n]*\|[ \t]*(\n[|:][^\n]*\|[ \t]*)*)/gm, (match) => {
    blocks.push(match)
    return PLACEHOLDER + (blocks.length - 1) + PLACEHOLDER
  })

  const filterDivs = []
  content = content.replace(
    /<div\b[^>]*\s+data-cann-filter\s*=\s*["'](\d+)["'][^>]*>([\s\S]*?)<\/div>/gi,
    (match, filter, inner) => {
      const ph = PLACEHOLDER + 'FD' + filterDivs.length + PLACEHOLDER
      filterDivs.push({ filter, inner })
      return ph
    }
  )

  content = content.replace(/<a\s+name\s*=\s*["'][^"']*["'][^>]*>\s*<\/a>/gi, '')

  content = content.replace(/<table\b[\s\S]*?<\/table>/gi, (match) => htmlTableToMarkdown(match))

  content = content
    .replace(/<a\b[^>]*>([\s\S]*?)<\/a>/gi, '$1')

  content = content.replace(/<(term|ph|__gm__|__ubuf__)\b([^>]*)>([\s\S]*?)<\/\1>/gis, '$3')

  const inlineWrapperTags = 'span|strong|em|b|i|u|code|kbd|samp|var|mark|sub|sup|small|abbr|cite|dfn|time|q|font|center'
  content = content.replace(new RegExp(`<\\/?(${inlineWrapperTags})\\b[^>]*>`, 'gi'), '')

  content = content.replace(/<br\s*\/?>/gi, '\n')

  content = content
    .replace(/<\/(p|div)>/gi, '\n')
    .replace(/<(p|div)\b[^>]*>/gi, '')

  content = content
    .replace(/<(style|script)\b[^>]*>[\s\S]*?<\/(style|script)>/gi, '')
    .replace(/<(img|input|meta|link|base|embed|source|track|area|param|wbr|col|hr)\b[^>]*\/?>/gi, '')
    .replace(/<(section|article|nav|aside|header|footer|main|figure|figcaption|details|summary|blockquote|ul|ol|li|dl|dt|dd|dialog|data|ruby|rt|rp|bdi|bdo|hgroup)\b[^>]*>/gi, '')
    .replace(/<\/(section|article|nav|aside|header|footer|main|figure|figcaption|details|summary|blockquote|ul|ol|li|dl|dt|dd|dialog|data|ruby|rt|rp|bdi|bdo|hgroup)>/gi, '')

  for (let i = 0; i < filterDivs.length; i++) {
    let inner = filterDivs[i].inner
    inner = inner.replace(/<(term|ph|__gm__|__ubuf__)\b[^>]*>([\s\S]*?)<\/\1>/gis, '$2')
    inner = inner.replace(new RegExp(`<\\/?(${inlineWrapperTags})\\b[^>]*>`, 'gi'), '')
    inner = inner.replace(/<br\s*\/?>/gi, '\n')
    inner = inner.replace(/<(p|div)\b[^>]*>/gi, '')
    inner = inner.replace(/<\/(p|div)>/gi, '\n')
    inner = inner.replace(/\n{2,}/g, '\n\n').trim()
    const replacement = `CANNFILTER_DIV_${filterDivs[i].filter}_OPEN\n\n${inner}\n\nCANNFILTER_DIV_${filterDivs[i].filter}_CLOSE`
    const ph = PLACEHOLDER + 'FD' + i + PLACEHOLDER
    content = content.replace(ph, replacement)
  }

  content = content.replace(/\n{2,}/g, '\n\n')

  content = content.replace(
    new RegExp(PLACEHOLDER.replace(/[.*+?^${}()|[\]\\]/g, '\\$&') + '(\\d+)' + PLACEHOLDER.replace(/[.*+?^${}()|[\]\\]/g, '\\$&'), 'g'),
    (_, n) => blocks[parseInt(n)]
  )

  return content
}

let totalFixed = 0
const files = walkMdTxt(SOURCE_DIR)

for (const srcFile of files) {
  const relPath = relative(SOURCE_DIR, srcFile).replace(/\\/g, '/').replace(/\.txt$/, '')
  const targetFile = join(TARGET_DIR, relPath)
  const targetDir = dirname(targetFile)

  if (!existsSync(targetDir)) {
    mkdirSync(targetDir, { recursive: true })
  }

  let content = readFileSync(srcFile, 'utf-8')

  const figuresRel = relative(dirname(targetFile), TARGET_DIR).replace(/\\/g, '/') + '/figures/'
  content = content.replace(/\]\((?:\.\/)?(?:\.\.\/)+figures\//g, '](' + figuresRel)

  const processed = processContent(content)

  writeFileSync(targetFile, processed, 'utf-8')
  totalFixed++
}

console.log(`Processed ${totalFixed} files from _sources to api/`)
