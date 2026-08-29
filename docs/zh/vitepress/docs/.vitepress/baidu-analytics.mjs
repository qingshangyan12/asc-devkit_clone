/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

export const BAIDU_ANALYTICS_SITE_ID = '21ffa0c0dcb3d3e9f19b3eb55aeb97c7'

export const baiduAnalyticsHead = [
  [
    'script',
    {},
    `
      var _hmt = _hmt || [];
      (function () {
        var hm = document.createElement("script");
        hm.src = "https://hm.baidu.com/hm.js?${BAIDU_ANALYTICS_SITE_ID}";
        var s = document.getElementsByTagName("script")[0];
        s.parentNode.insertBefore(hm, s);
      })();
    `,
  ],
]

export function getBaiduPageviewPath(location) {
  if (!location) return '/'
  return `${location.pathname || '/'}${location.search || ''}`
}

export function installBaiduAnalytics(
  router,
  browser = typeof window === 'undefined' ? undefined : window,
) {
  if (!router || !browser) return false

  let currentPath = getBaiduPageviewPath(browser.location)
  const previousAfterRouteChange = router.onAfterRouteChange

  router.onAfterRouteChange = async (...args) => {
    if (previousAfterRouteChange) {
      await previousAfterRouteChange.apply(router, args)
    }

    const nextPath = getBaiduPageviewPath(browser.location)
    if (nextPath === currentPath) return

    currentPath = nextPath
    browser._hmt = browser._hmt || []
    browser._hmt.push(['_trackPageview', nextPath])
  }

  return true
}
