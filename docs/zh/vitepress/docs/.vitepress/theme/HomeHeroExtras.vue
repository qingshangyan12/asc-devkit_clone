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
      <span>搜索</span>
      <kbd>Ctrl K</kbd>
    </button>
  </div>
</template>

<style scoped>
.home-hero-extra {
  width: min(100%, 520px);
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

.home-search-trigger:focus-visible {
  outline: 3px solid var(--vp-c-brand-soft);
  outline-offset: 3px;
  border-color: var(--vp-c-brand-1);
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

}
</style>
