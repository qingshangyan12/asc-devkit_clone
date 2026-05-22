import { defineConfig } from 'vitepress'
import { existsSync, readFileSync } from 'node:fs'
import { resolve } from 'node:path'
import sidebars from './sidebar.mjs'

function placeholderPlugin() {
  const root = resolve(import.meta.dirname, '..')
  const placeholder = resolve(root, 'api', 'figures', 'placeholder.png')
  return {
    name: 'placeholder-images',
    resolveId(id, importer) {
      if (!importer || !id.match(/\.(png|jpg|jpeg|gif|svg|webp)$/i)) return
      const importerDir = resolve(importer, '..')
      const resolved = resolve(importerDir, id)
      if (!existsSync(resolved)) {
        return placeholder
      }
    },
  }
}

function fixCannFilterTags(src) {
  return src
    .replace(/<\/?cann-filter\b[^>]*>/gi, '')
    .replace(/<(term|ph|__gm__|__ubuf__)\b[^>]*>([\s\S]*?)<\/\1>/gis, '$2')
    .replace(/<(term|ph|__gm__|__ubuf__)\b[^>]*>/gi, '')
    .replace(/<\/(term|ph|__gm__|__ubuf__)>/gi, '')
}

function stripInnerTags(text) {
  return text
    .replace(/<a\s+name\s*=\s*"[^"]*"\s*><\/a>/gi, '')
    .replace(/<\/?span\b[^>]*>/gi, '')
    .replace(/<\/?p\b[^>]*>/gi, '')
    .replace(/<a\s+href\s*=\s*"([^"]*)"\s*>/gi, '<a href="$1">')
    .trim()
}

function convertHtmlTablesToMarkdown(src) {
  let result = src
  result = result.replace(
    /<table\b[\s\S]*?<\/table>/gi,
    (table) => {
      const theadMatch = table.match(/<thead\b[\s\S]*?<\/thead>/i)
      const tbodyMatches = []
      const tbodyRegex = /<tbody\b[\s\S]*?<\/tbody>/gi
      let m
      while ((m = tbodyRegex.exec(table)) !== null) {
        tbodyMatches.push(m[0])
      }

      const rows = []
      if (theadMatch) {
        const thsMatch = theadMatch[0].match(/<th\b[\s\S]*?<\/th>/gi)
        if (thsMatch) {
          rows.push(thsMatch.map(th => stripInnerTags(th.replace(/<th\b[^>]*>/i, '').replace(/<\/th>/i, ''))))
        }
      }

      for (const tbody of tbodyMatches) {
        const trsMatch = tbody.match(/<tr\b[\s\S]*?<\/tr>/gi)
        if (trsMatch) {
          for (const tr of trsMatch) {
            const tdsMatch = tr.match(/<td\b[\s\S]*?<\/td>/gi)
            if (tdsMatch) {
              const cells = tdsMatch.map(td => {
                let text = td.replace(/<td\b[^>]*>/i, '').replace(/<\/td>/i, '')
                text = stripInnerTags(text)
                const linkMatch = text.match(/<a href="([^"]*)">([^<]*)<\/a>/i)
                if (linkMatch) {
                  text = text.replace(linkMatch[0], '[' + linkMatch[2] + '](' + linkMatch[1] + ')')
                }
                return text
              })
              rows.push(cells)
            }
          }
        }
      }

      if (rows.length === 0) return table
      const colCount = Math.max(...rows.map(r => r.length))
      const aligned = rows.map(r => {
        while (r.length < colCount) r.push('')
        return r
      })

      const mdRows = aligned.map((r, i) => '| ' + r.join(' | ') + ' |')
      const sep = '| ' + Array(colCount).fill('---').join(' | ') + ' |'
      return '\n' + mdRows[0] + '\n' + sep + '\n' + mdRows.slice(1).join('\n') + '\n'
    }
  )

  result = result.replace(/<a\s+name\s*=\s*"[^"]*"\s*><\/a>/gi, '')
  result = result.replace(/<br\s*\/?>/gi, '\n')
  return result
}

function fixIndentedBlockquotes(src) {
  return src.replace(/^[ \t]{4,}(?=>)/gm, '')
}

function fixSpanCrossingTableCells(src) {
  let prev
  do {
    prev = src
    src = src.replace(
      /(<span[^>]*>)([\s\S]*?)\|([\s\S]*?)<\/span>/g,
      (match, open, beforePipe, afterPipe) =>
        open + beforePipe + '</span>|' + afterPipe
    )
  } while (src !== prev)
  return src
}

function escapeVueInterpolations(html) {
  return html.replace(
    /(<code\b[\s\S]*?<\/code>|<pre\b[\s\S]*?<\/pre>)|(\{\{[\s\S]*?\}\})/g,
    (match, codeBlock, templateExpr) => {
      if (codeBlock) return codeBlock
      return '<span v-pre>' + templateExpr + '</span>'
    }
  )
}

function loadHeaderCache(filePath) {
  const cachePath = filePath + '.header'
  if (!existsSync(cachePath)) return null
  try {
    return JSON.parse(readFileSync(cachePath, 'utf-8'))
  } catch {
    return null
  }
}

export default defineConfig({
  mpa: false,
  title: 'Ascend C',
  description: 'Ascend C 算子开发文档',
  lang: 'zh-CN',
  ignoreDeadLinks: true,

  markdown: {
    html: false,
    config(md) {
      const originalRender = md.render.bind(md)
      md.render = function (src, env) {
        src = fixCannFilterTags(src)
        src = fixIndentedBlockquotes(src)
        src = fixSpanCrossingTableCells(src)
        src = convertHtmlTablesToMarkdown(src)

        let html = originalRender(src, env)

        html = html.replace(/<p>\s*CANNFILTER_DIV_(\w+)_OPEN\s*<\/p>/g, '<div data-filter="$1">')
        html = html.replace(/<p>\s*CANNFILTER_DIV_(\w+)_CLOSE\s*<\/p>/g, '</div>')

        html = escapeVueInterpolations(html)

        return `<div v-pre>${html}</div>`
      }
    },
  },

  transformPageData(pageData) {
    const cache = loadHeaderCache(pageData.filePath)
    if (cache && cache.length > 0) {
      pageData.headers = cache
    }
  },

  vite: {
    plugins: [placeholderPlugin()],
  },

  vue: {
    template: {
      compilerOptions: {
        onError: (error) => {
          if (error.code === 2) return
          throw error
        },
      },
    },
  },

  themeConfig: {
    nav: [
      { text: '首页', link: '/' },
      { text: 'AscendC算子开发指南', link: '/guide/入门教程/环境准备' },
      { text: 'Ascend C API', link: '/api/Ascend-C-API列表' },
    ],

    sidebar: sidebars,

    search: {
      provider: 'local',
    },

    outline: {
      level: [2, 3],
      label: '本页内容',
    },

    docFooter: {
      prev: '上一页',
      next: '下一页',
    },

    footer: {
      copyright: 'Copyright Huawei Technologies Co., Ltd.',
    },
  },
})
