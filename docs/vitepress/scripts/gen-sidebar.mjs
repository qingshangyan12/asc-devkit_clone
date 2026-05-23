import { readFileSync, writeFileSync } from 'node:fs'
import { resolve } from 'node:path'

const guideIndexPath = resolve(import.meta.dirname, '..', '..', 'guide', 'index.md')
const sidebarPath = resolve(import.meta.dirname, '..', 'docs', '.vitepress', 'sidebar.mjs')

const content = readFileSync(guideIndexPath, 'utf-8')

function parseLines(content) {
  const lines = content.split('\n')
  const result = []
  const stack = [{ level: -1, children: result }]

  for (const line of lines) {
    const m = line.match(/^(\s*)-\s*\[(.+?)\]\((.+)\)/)
    if (!m) continue

    const level = Math.floor(m[1].length / 4)
    const text = m[2].replace(/\\n/g, '').trim()
    const link = m[3].replace(/\.md$/, '').replace(/^\.\//, '').replace(/^\.\\/, '')

    while (stack.length > 0 && stack[stack.length - 1].level >= level) {
      stack.pop()
    }

    const parent = stack[stack.length - 1]
    const node = { text, link, children: [] }
    parent.children.push(node)
    stack.push({ level, children: node.children })
  }

  return result
}

function toSidebarItem(node) {
  const basePath = '/guide/' + node.link
  if (node.children.length === 0) {
    return { text: node.text, link: basePath }
  }

  const items = node.children.map(toSidebarItem)

  if (items.length === 1) {
    const child = items[0]
    if (child.items) {
      return child
    }
  }

  const hasSelf = items.some(i => i.link === basePath)

  return {
    text: node.text,
    collapsed: true,
    items: hasSelf ? items : [{ text: node.text, link: basePath }, ...items]
  }
}

const sections = parseLines(content)

const guideSidebar = sections.map(section => {
  return {
    text: section.text,
    collapsed: true,
    items: section.children.map(toSidebarItem)
  }
})

const oldSidebar = readFileSync(sidebarPath, 'utf-8')
const apiStart = oldSidebar.indexOf('"/api/"')
if (apiStart === -1) {
  console.error('Could not find /api/ section in sidebar.mjs')
  process.exit(1)
}
const apiSection = oldSidebar.slice(apiStart).trim().replace(/}\s*$/, '')

const output = `export default {
  "/guide/": ${JSON.stringify(guideSidebar, null, 2)},


  ${apiSection}
}
`

writeFileSync(sidebarPath, output, 'utf-8')
console.log('sidebar.mjs updated successfully')
