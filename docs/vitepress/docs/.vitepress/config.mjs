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
  mpa: true,
  title: 'Ascend C',
  description: 'Ascend C 算子开发文档',
  lang: 'zh-CN',
  ignoreDeadLinks: true,

  markdown: {
    html: false,
    config(md) {
      const originalRender = md.render.bind(md)
      md.render = function (src, env) {
        src = fixSpanCrossingTableCells(src)

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
