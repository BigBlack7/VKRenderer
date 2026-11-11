# Building VKRenderer — Engine Roadmap

> Phase 7 · Building VKRenderer · [总目录](../VulkanLearningRoadmap.md)
> 上一章：[18 Ray Tracing — Advanced](18-Ray-Tracing.md)
> 维护约定：本文件保存章节任务、实践与验收记录；总体进度只在[学习地图](../VulkanLearningRoadmap.md#current-progress)维护。

官方高级路线：
`Building a Simple Engine`

> 这一阶段不再以“复制教程代码”为目标，而是把此前所有 Sandbox 中验证过的知识收束成真正的 VKRenderer Core。

- [ ] E00 — Engine Architecture
  - Renderer responsibilities
  - Platform / Window layer
  - GPU context
  - resource ownership
  - frame lifecycle
- [ ] E01 — Camera & Transformations
- [ ] E02 — Loading Models
- [ ] E03 — Lighting & Materials
- [ ] E04 — GUI
- [ ] E05 — Subsystems
- [ ] E06 — Tooling
- [ ] E07 — Advanced Topics
- [ ] E08 — Performance / Profiling
- [ ] E09 — Resource Lifetime & Deferred Destruction
- [ ] E10 — Modern Render Architecture Review

最终 Core 的目录由实际实现自然演化，不提前锁死。

可能出现但不预设的组件包括：

```text
Core/
├─ Platform/
├─ Renderer/
├─ Resource/
├─ Scene/
└─ Asset/
```

---

[返回学习地图](../VulkanLearningRoadmap.md)
