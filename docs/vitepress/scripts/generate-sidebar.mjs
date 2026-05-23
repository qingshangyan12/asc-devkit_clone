import { readdirSync, statSync, writeFileSync } from 'node:fs'
import { join, relative } from 'node:path'
import { fileURLToPath } from 'node:url'

const __dirname = fileURLToPath(new URL('.', import.meta.url))
const ROOT = join(__dirname, '..')
const GUIDE_DIR = join(ROOT, 'docs', 'guide')
const API_DIR = join(ROOT, 'docs', 'api')
const OUTPUT = join(ROOT, 'docs', '.vitepress', 'sidebar.mjs')
const DIR_ORDER = ['入门教程', '编程指南', '算子实践参考', '技术附录', '跨代迁移兼容性指南']
const API_ORDER = ['SIMD-API', 'SIMT-API', 'Utils-API', 'AI-CPU-API', 'tensor_api']
const MAX_DEPTH = 3
const API_MAX_DEPTH = 5

function scanDir(dir, baseDir, depth, maxDepth) {
  if (depth > maxDepth) return []

  const entries = readdirSync(dir, { withFileTypes: true })
  const dirs = []
  const files = []
  const items = []

  for (const e of entries) {
    if (e.name.startsWith('_') || e.name.startsWith('.')) continue
    if (e.name === 'figures') continue
    const full = join(dir, e.name)
    if (e.isDirectory()) {
      dirs.push({ name: e.name, path: full })
    } else if (e.name.endsWith('.md')) {
      files.push({ name: e.name, path: full })
    }
  }

  dirs.sort((a, b) => {
    const ai = (baseDir === API_DIR ? API_ORDER : DIR_ORDER).indexOf(a.name)
    const bi = (baseDir === API_DIR ? API_ORDER : DIR_ORDER).indexOf(b.name)
    if (ai >= 0 && bi >= 0) return ai - bi
    if (ai >= 0) return -1
    if (bi >= 0) return 1
    return a.name.localeCompare(b.name, 'zh-CN')
  })
  files.sort((a, b) => a.name.localeCompare(b.name, 'zh-CN'))

  for (const f of files) {
    const rel = relative(baseDir, f.path).replace(/\\/g, '/')
    const prefix = baseDir === API_DIR ? '/api/' : '/guide/'
    const text = f.name.replace(/\.md$/, '')
    items.push({ text, link: prefix + rel.replace(/\.md$/, '') })
  }

  for (const d of dirs) {
    const children = scanDir(d.path, baseDir, depth + 1, maxDepth)
    if (children.length === 0) continue
    const defaultCollapsed = baseDir === API_DIR ? true : depth >= 2
    const entry = { text: d.name, collapsed: defaultCollapsed, items: children }
    items.push(entry)
  }

  return items
}

const sidebars = {}

for (const section of DIR_ORDER) {
  const dir = join(GUIDE_DIR, section)
  try {
    if (statSync(dir).isDirectory()) {
      sidebars['/guide/' + section + '/'] = scanDir(dir, GUIDE_DIR, 0, MAX_DEPTH)
    }
  } catch {}
}

sidebars['/guide/'] = scanDir(GUIDE_DIR, GUIDE_DIR, 0, MAX_DEPTH)

if (statSync(API_DIR).isDirectory()) {
  for (const section of API_ORDER) {
    const dir = join(API_DIR, section)
    try {
      if (statSync(dir).isDirectory()) {
        sidebars['/api/' + section + '/'] = scanDir(dir, API_DIR, 0, API_MAX_DEPTH)
      }
    } catch {}
  }
  sidebars['/api/'] = scanDir(API_DIR, API_DIR, 0, 1)
}

let code = 'export default ' + JSON.stringify(sidebars, null, 2) + '\n'
writeFileSync(OUTPUT, code, 'utf-8')
console.log('Sidebar generated:', OUTPUT)
