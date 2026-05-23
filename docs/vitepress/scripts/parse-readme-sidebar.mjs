import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

const root = resolve(import.meta.dirname, '..')
const readmePath = resolve(root, 'docs', 'api', 'README.md')
const content = readFileSync(readmePath, 'utf-8')

function parseMarkdownList(md) {
  const lines = md.split(/\r?\n/)
  const roots = []
  const stack = []

  for (const line of lines) {
    const match = line.match(/^(\s*)-\s\s\s\[(.*?)\]\((.*)\)/)
    if (!match) continue

    const indent = match[1].length
    const text = match[2]
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
  if (node.items && node.items.length === 0) {
    delete node.items
  } else if (node.items) {
    for (const child of node.items) {
      cleanup(child)
    }
  }
}

function toSidebar(items, collapsed = true) {
  const result = []
  for (const item of items) {
    if (item.items && item.items.length > 0) {
      result.push({
        text: item.text,
        collapsed: collapsed,
        items: toSidebar(item.items, true)
      })
    } else {
      result.push({ text: item.text, link: item.link })
    }
  }
  return result
}

const parsed = parseMarkdownList(content)
for (const root of parsed) {
  cleanup(root)
}
const sidebarItems = toSidebar(parsed, true)

console.log(JSON.stringify(sidebarItems, null, 2))
