<!--
  Copyright (c) 2026 Huawei Technologies Co., Ltd.
  This program is free software, you can redistribute it and/or modify it under the terms and conditions of
  CANN Open Software License Agreement Version 2.0 (the "License").
  Please refer to the License for details. You may not use this file except in compliance with the License.
  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE in the root of the software repository for the full text of the License.
-->

<script setup>
import { withBase } from 'vitepress'
import UiIcon from './UiIcon.vue'

const tasks = [
  { icon: 'play', title: '首次开发算子', detail: '从概念到HelloWorld', link: '/guide/入门教程/Ascend-C概述与学习路径' },
  { icon: 'search', title: '查询API', detail: '按产品与接口定位', link: '/api/README' },
  { icon: 'gauge', title: '调试与调优', detail: '分析功能与性能问题', link: '/guide/算子实践参考/本文档组织结构' },
  { icon: 'refresh', title: '跨代迁移', detail: '检查API兼容策略', link: '/guide/跨代迁移兼容性指南/概述' },
]

const sections = [
  { icon: 'play', tone: 'green', title: '入门教程', detail: '环境准备、基础概念和首个Ascend C算子的完整上手路径。', link: '/guide/入门教程/Ascend-C概述与学习路径' },
  { icon: 'book', tone: 'purple', title: '编程指南', detail: 'SIMD、SIMT编程模型、语言扩展、编译运行与开发能力。', link: '/guide/编程指南/本文档组织结构' },
  { icon: 'gauge', tone: 'gold', title: '算子实践参考', detail: '功能调试、性能分析、优化方法和可复用的优秀实践。', link: '/guide/算子实践参考/本文档组织结构' },
  { icon: 'refresh', tone: 'coral', title: '跨代迁移兼容性指南', detail: '面向不同架构版本的API兼容策略和迁移注意事项。', link: '/guide/跨代迁移兼容性指南/概述' },
  { icon: 'library', tone: 'neutral', title: '技术附录', detail: 'C++标准支持、概念原理、术语表及补充参考信息。', link: '/guide/技术附录/概念原理和术语/术语表' },
  { icon: 'braces', tone: 'teal', title: 'Ascend C API', detail: 'SIMD、SIMT、AI CPU 与 Utils API 的完整接口参考。', link: '/api/README' },
]
</script>

<template>
  <main class="home-sections">
    <section class="task-section" aria-labelledby="task-title">
      <div class="section-heading">
        <p class="section-kicker">QUICK PATHS</p>
        <h2 id="task-title">按任务开始</h2>
      </div>
      <div class="task-grid">
        <a v-for="task in tasks" :key="task.title" class="task-link" :href="withBase(task.link)">
          <span class="task-icon"><UiIcon :name="task.icon" :size="18" /></span>
          <span class="task-copy">
            <strong>{{ task.title }}</strong>
            <small>{{ task.detail }}</small>
          </span>
          <UiIcon name="arrow" :size="17" />
        </a>
      </div>
    </section>

    <section class="explore-section" aria-labelledby="explore-title">
      <div class="section-heading">
        <p class="section-kicker">DOCUMENTATION</p>
        <h2 id="explore-title">探索文档</h2>
      </div>
      <div class="feature-grid">
        <a
          v-for="section in sections"
          :key="section.title"
          class="feature-card"
          :class="`tone-${section.tone}`"
          :href="withBase(section.link)"
        >
          <span class="feature-icon"><UiIcon :name="section.icon" :size="22" /></span>
          <span class="feature-copy">
            <strong>{{ section.title }}</strong>
            <small>{{ section.detail }}</small>
          </span>
          <span class="feature-arrow"><UiIcon name="arrow" :size="18" /></span>
        </a>
      </div>
    </section>
  </main>
</template>

<style scoped>
.home-sections {
  width: min(1200px, calc(100% - 48px));
  margin: 0 auto;
  padding: 42px 0 0;
}

.task-section,
.explore-section {
  padding: 0;
}

.explore-section {
  margin-top: 56px;
  padding-top: 34px;
  border-top: 1px solid var(--vp-c-divider);
}

.section-heading {
  margin-bottom: 20px;
}

.section-kicker {
  margin: 0 0 5px;
  color: var(--vp-c-brand-1);
  font-family: var(--vp-font-family-mono);
  font-size: 11px;
  font-weight: 700;
}

.section-heading h2 {
  margin: 0;
  color: var(--vp-c-text-1);
  font-size: 23px;
  line-height: 1.3;
  letter-spacing: 0;
}

.task-grid {
  display: grid;
  grid-template-columns: repeat(4, minmax(0, 1fr));
  gap: 20px;
}

.task-link {
  display: grid;
  grid-template-columns: 34px minmax(0, 1fr) 18px;
  gap: 11px;
  align-items: center;
  min-height: 64px;
  padding: 8px 0;
  border-bottom: 1px solid var(--vp-c-divider);
  color: var(--vp-c-text-2);
  text-decoration: none;
}

.task-link:hover {
  border-color: var(--vp-c-brand-1);
  color: var(--vp-c-brand-1);
}

.task-icon,
.feature-icon {
  display: inline-flex;
  align-items: center;
  justify-content: center;
}

.task-icon {
  width: 34px;
  height: 34px;
  border-radius: 8px;
  background: var(--vp-c-brand-soft);
  color: var(--vp-c-brand-1);
}

.task-copy,
.feature-copy {
  display: flex;
  min-width: 0;
  flex-direction: column;
}

.task-copy strong {
  color: var(--vp-c-text-1);
  font-size: 14px;
}

.task-copy small {
  margin-top: 2px;
  color: var(--vp-c-text-3);
  font-size: 12px;
}

.feature-grid {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 18px;
}

.feature-card {
  position: relative;
  display: grid;
  grid-template-columns: 46px minmax(0, 1fr) 20px;
  gap: 15px;
  align-items: start;
  min-height: 154px;
  padding: 24px;
  border-radius: 8px;
  background: var(--vp-c-bg-elv);
  box-shadow: var(--asc-shadow-soft);
  color: var(--vp-c-text-2);
  text-decoration: none;
  transition: transform 0.18s ease, box-shadow 0.18s ease;
}

.feature-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 12px 32px rgba(39, 51, 71, 0.1);
}

.feature-icon {
  width: 46px;
  height: 46px;
  border-radius: 8px;
}

.feature-copy strong {
  color: var(--vp-c-text-1);
  font-size: 16px;
  line-height: 1.35;
}

.feature-copy small {
  margin-top: 9px;
  color: var(--vp-c-text-2);
  font-size: 13px;
  line-height: 1.7;
}

.feature-arrow {
  margin-top: 2px;
  color: var(--vp-c-text-3);
}

.feature-card:hover .feature-arrow {
  color: currentColor;
}

.tone-green { color: var(--asc-green-dark); }
.tone-green .feature-icon { background: var(--asc-green-light); }
.tone-purple { color: var(--asc-purple-dark); }
.tone-purple .feature-icon { background: var(--asc-purple-light); }
.tone-gold { color: var(--asc-gold-dark); }
.tone-gold .feature-icon { background: var(--asc-gold-light); }
.tone-coral { color: var(--asc-coral-dark); }
.tone-coral .feature-icon { background: var(--asc-coral-light); }
.tone-teal { color: var(--asc-teal-dark); }
.tone-teal .feature-icon { background: var(--asc-teal-light); }
.tone-neutral { color: var(--vp-c-text-2); }
.tone-neutral .feature-icon { background: var(--vp-c-bg-soft); }

@media (max-width: 959px) {
  .task-grid,
  .feature-grid {
    grid-template-columns: repeat(2, minmax(0, 1fr));
  }
}

@media (max-width: 640px) {
  .home-sections {
    width: min(100% - 32px, 1200px);
    padding-top: 32px;
  }

  .task-grid,
  .feature-grid {
    grid-template-columns: 1fr;
    gap: 12px;
  }

  .explore-section {
    margin-top: 42px;
  }

  .feature-card {
    min-height: 132px;
    padding: 20px;
  }
}
</style>
