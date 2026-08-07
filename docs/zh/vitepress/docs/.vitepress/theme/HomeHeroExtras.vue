<!--
  Copyright (c) 2026 Huawei Technologies Co., Ltd.
  Licensed under the CANN Open Software License Agreement Version 2.0.
-->

<script setup>
import UiIcon from './UiIcon.vue'

function openSearch() {
  const trigger = document.querySelector('.blog-search .nav-search-btn-wait, .VPNavBarSearch button')
  trigger?.click()
}
</script>

<template>
  <div class="home-hero-extra">
    <button class="home-search-trigger" type="button" @click="openSearch">
      <UiIcon name="search" :size="20" />
      <span>搜索文档、API 与示例</span>
      <kbd>Ctrl K</kbd>
    </button>
    <div class="home-capabilities" aria-label="主要编程能力">
      <span>SIMD</span>
      <span>SIMT</span>
      <span>AI CPU</span>
      <span>Utils API</span>
    </div>
  </div>
</template>

<style scoped>
.home-hero-extra {
  width: min(100%, 560px);
  margin-top: 24px;
}

.home-search-trigger {
  display: grid;
  grid-template-columns: 20px minmax(0, 1fr) auto;
  gap: 12px;
  align-items: center;
  width: 100%;
  height: 50px;
  padding: 0 14px;
  border: 1px solid rgba(255, 255, 255, 0.86);
  border-radius: 8px;
  background: rgba(255, 255, 255, 0.78);
  box-shadow: var(--asc-shadow-soft);
  color: var(--vp-c-text-2);
  cursor: pointer;
  text-align: left;
}

.home-search-trigger:hover {
  background: #fff;
  color: var(--vp-c-brand-2);
}

.home-search-trigger kbd {
  padding: 3px 7px;
  border: 1px solid var(--vp-c-divider);
  border-radius: 5px;
  background: var(--vp-c-bg-alt);
  color: var(--vp-c-text-3);
  font-family: var(--vp-font-family-mono);
  font-size: 11px;
}

.home-capabilities {
  display: flex;
  flex-wrap: wrap;
  gap: 8px 18px;
  margin-top: 14px;
  color: var(--vp-c-text-2);
  font-family: var(--vp-font-family-mono);
  font-size: 12px;
  font-weight: 650;
}

.home-capabilities span:not(:last-child)::after {
  content: '';
  display: inline-block;
  width: 4px;
  height: 4px;
  margin-left: 18px;
  border-radius: 50%;
  background: var(--asc-teal-core);
  vertical-align: middle;
}

:global(.dark) .home-search-trigger {
  border-color: rgba(255, 255, 255, 0.12);
  background: rgba(39, 51, 71, 0.84);
}

:global(.dark) .home-search-trigger:hover {
  background: #334157;
}

@media (max-width: 640px) {
  .home-search-trigger {
    height: 48px;
  }

  .home-search-trigger kbd {
    display: none;
  }

  .home-capabilities {
    gap: 8px 12px;
  }

  .home-capabilities span:not(:last-child)::after {
    margin-left: 12px;
  }
}
</style>
