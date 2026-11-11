# 17 — Multithreading

> Phase 5 · Vulkan 工程化与兼容性 · [总目录](../VulkanLearningRoadmap.md)
> 上一章：[16 Rendering Multiple Objects](16-Rendering-Multiple-Objects.md) ｜ 下一章：[18 Ray Tracing — Advanced](18-Ray-Tracing.md)
> 维护约定：本文件保存章节任务、实践与验收记录；总体进度只在[学习地图](../VulkanLearningRoadmap.md#current-progress)维护。

- [ ] 17.1 Introduction
- [ ] 17.2 Vulkan Multithreading Model
- [ ] 17.3 Externally Synchronized Objects
- [ ] 17.4 Thread-safe / Non-thread-safe Vulkan Operations
- [ ] 17.5 Worker Threads
- [ ] 17.6 Per-thread Command Pools
- [ ] 17.7 Parallel Command Buffer Recording
- [ ] 17.8 Thread Resource Management
- [ ] 17.9 Queue Submission Synchronization
- [ ] 17.10 CPU Synchronization
  - Mutex
  - Condition Variable
  - Atomics
- [ ] 17.11 Asynchronous Resource Loading
- [ ] 17.12 Performance Considerations
  - Thread creation overhead
  - Work granularity
  - False sharing
  - Contention
- [ ] 17.13 Profiling & 判断并行是否真的有收益

**架构任务：**
到此阶段才讨论 Render Thread / Worker / Job System 等现代 Renderer 并行架构。

---

[返回学习地图](../VulkanLearningRoadmap.md)
