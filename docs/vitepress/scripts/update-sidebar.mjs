import { readFileSync, writeFileSync } from 'node:fs'
import { resolve } from 'node:path'

const root = resolve(import.meta.dirname, '..')
const sidebarPath = resolve(root, 'docs', '.vitepress', 'sidebar.mjs')
const readmePath = resolve(root, 'docs', 'api', 'README.md')

function parseMarkdownList(md) {
  const lines = md.split(/\r?\n/)
  const roots = []
  const stack = []

  for (const line of lines) {
    const match = line.match(/^(\s*)-\s\s\s\[(.*?)\]\((.*)\)/)
    if (!match) continue
    const indent = match[1].length

    // Unescape markdown escape sequences in link text
    let text = match[2]
    text = text.replace(/\\([\(\)\[\]_*#+\-.!{}|\\])/g, '$1')

    const rawLink = match[3].replace(/\.md$/, '')
    const link = '/api/' + rawLink
    const node = { text, link, items: [] }

    if (indent === 0) {
      roots.push(node)
      stack.length = 0
      stack.push({ node, indent })
    } else {
      while (stack.length > 0 && stack[stack.length - 1].indent >= indent) {
        stack.pop()
      }
      if (stack.length > 0) {
        stack[stack.length - 1].node.items.push(node)
      } else {
        if (roots.length > 0) {
          roots[roots.length - 1].items.push(node)
        } else {
          roots.push(node)
        }
      }
      stack.push({ node, indent })
    }
  }

  return roots
}

function cleanup(node) {
  if (node.items && node.items.length === 0) delete node.items
  else if (node.items) node.items.forEach(cleanup)
}

function jsStr(s) {
  return JSON.stringify(s)
}

function toSidebar(items, collapsed, indent) {
  let result = ''
  for (let i = 0; i < items.length; i++) {
    const item = items[i]
    if (i > 0) result += ',\n'
    if (item.items && item.items.length > 0) {
      result += indent + '{\n'
      result += indent + '  "text": ' + jsStr(item.text) + ',\n'
      result += indent + '  "collapsed": ' + collapsed + ',\n'
      result += indent + '  "items": [\n'
      result += toSidebar(item.items, true, indent + '    ')
      result += '\n' + indent + '  ]\n'
      result += indent + '}'
    } else {
      result += indent + '{\n'
      result += indent + '  "text": ' + jsStr(item.text) + ',\n'
      result += indent + '  "link": ' + jsStr(item.link) + '\n'
      result += indent + '}'
    }
  }
  return result
}

const readme = readFileSync(readmePath, 'utf-8')
const parsed = parseMarkdownList(readme)
parsed.forEach(cleanup)

// Add README at the end (after 附录)
parsed.push({ text: 'README', link: '/api/README' })

const newApiSidebar = '  "/api/": [\n' + toSidebar(parsed, true, '    ') + '\n  ]\n'

// Read sidebar.mjs
let sidebar = readFileSync(sidebarPath, 'utf-8')

const startMarker = '  "/api/": ['
const startIdx = sidebar.indexOf(startMarker)
if (startIdx === -1) {
  console.error('Could not find /api/ section in sidebar.mjs')
  process.exit(1)
}

const after = sidebar.substring(startIdx).split('\n')
let closingIdx = -1
let depth = 0
let foundStart = false
for (let i = 0; i < after.length; i++) {
  if (after[i].includes('"/api/": [')) {
    foundStart = true
    depth = 1
    continue
  }
  if (!foundStart) continue
  for (const ch of after[i]) {
    if (ch === '[') depth++
    else if (ch === ']') {
      depth--
      if (depth === 0) { closingIdx = i; break }
    }
  }
  if (closingIdx >= 0) break
}

const before = sidebar.substring(0, startIdx)
const remaining = after.slice(closingIdx + 1).join('\n')
const newSidebar = before + newApiSidebar + remaining

writeFileSync(sidebarPath, newSidebar, 'utf-8')
console.log(`Updated sidebar.mjs (${newSidebar.split('\n').length} lines)`)
