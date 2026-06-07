<script setup>
import { ref, computed, watch, onMounted, onBeforeUnmount } from 'vue'
import { useRoute } from 'vitepress'
import CustomSidebarItem from './CustomSidebarItem.vue'

const props = defineProps({
  open: Boolean,
})

const route = useRoute()
const versionSidebars = ref(null)
const currentVersion = ref('all')

const prefix = computed(() => {
  if (route.path.startsWith('/api/')) return '/api/'
  if (route.path.startsWith('/guide/')) return '/guide/'
  return null
})

const currentSidebar = computed(() => {
  if (!versionSidebars.value || !prefix.value) return []
  const versionData = versionSidebars.value[currentVersion.value]
  if (!versionData) return []
  return versionData[prefix.value] || []
})

const hasSidebar = computed(() => {
  if (!versionSidebars.value) return true
  const items = currentSidebar.value
  return items && items.length > 0
})

onMounted(async () => {
  try {
    const resp = await fetch('/version-sidebars.json')
    versionSidebars.value = await resp.json()
  } catch (e) {
    console.error('Failed to load version-sidebars.json:', e)
    versionSidebars.value = null
  }

  const saved = localStorage.getItem('cann-filter-selection')
  if (saved) currentVersion.value = saved

  window.addEventListener('version-changed', onVersionChanged)
})

onBeforeUnmount(() => {
  window.removeEventListener('version-changed', onVersionChanged)
})

function onVersionChanged(e) {
  if (e.detail === currentVersion.value) return
  currentVersion.value = e.detail
}
</script>

<template>
  <aside
    v-if="hasSidebar"
    class="VPSidebar custom-sidebar"
    :class="{ open: props.open }"
    @click.stop
  >
    <div class="curtain" />
    <nav class="nav" id="VPSidebarNav" role="navigation" aria-labelledby="sidebar-aria-label" tabindex="-1">
      <span class="visually-hidden" id="sidebar-aria-label">Sidebar Navigation</span>
      <div v-for="item in currentSidebar" :key="item.text" class="group">
        <CustomSidebarItem :item="item" :depth="0" />
      </div>
    </nav>
  </aside>
</template>
