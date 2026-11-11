# 01 — Overview

> Phase 0 · 世界观与开发环境 · [总目录](../VulkanLearningRoadmap.md)
> 上一章：[00 Introduction](00-Introduction.md) ｜ 下一章：[02 Development Environment](02-Development-Environment.md)
> 维护约定：本文件保存章节任务、实践与验收记录；总体进度只在[学习地图](../VulkanLearningRoadmap.md#current-progress)维护。

官方：
`01_Overview`

目标：在写代码前建立完整 Vulkan mental model。

- [x] 01.1 Origin of Vulkan
  - 传统图形 API 的 Driver 隐式工作
  - Vulkan 为什么选择显式模型
  - CPU overhead 与多线程
- [x] 01.2 Coding Conventions
  - Vulkan C API 与 Vulkan-Hpp
  - handle / enum / flags / struct 基本阅读方式
- [x] 01.3 What It Takes to Draw a Triangle
  - [x] Step 1 — Instance & Physical Device
  - [x] Step 2 — Logical Device & Queue Families
  - [x] Step 3 — Window Surface & Swapchain
  - [x] Step 4 — Swapchain Image Views
  - [x] Step 5 — Dynamic Rendering
  - [x] Step 6 — Graphics Pipeline
  - [x] Step 7 — Command Pool & Command Buffer
  - [x] Step 8 — Main Loop / Acquire / Submit / Present
- [x] 01.4 Summary / 第一帧的完整数据流
- [x] 01.5 API Concepts
  - CreateInfo
  - `sType` / `pNext`
  - `vk::Result`
  - Exception
  - Allocation callbacks
  - Vulkan-Hpp / RAII 所有权
- [x] 01.6 Validation Layers
  - `VK_LAYER_KHRONOS_validation`
  - `VK_EXT_debug_utils`
  - Debug Messenger
  - Severity / Message Type
  - VUID 与规范查询
  - Validation 能力边界

**课程产物：**
- 建立从 Instance 到第一帧呈现的完整对象关系模型。
- 理解 CPU 命令录制、GPU 执行和 Presentation 的区别。
- 理解 Vulkan 对象所有权、生命周期及同步的基本约束。
- 理解 Vulkan-Hpp、RAII、CreateInfo、pNext 和 Result。
- 建立使用 Validation Layer、Debug Messenger 和 VUID 排查问题的基本方法。

**验收记录（2026-11-10）：**
- 01.1～01.6 全部完成理论学习并通过理解检查。
- 01.3 的八个 Step 全部通过单独验收。
- 能够解释第一帧 Vulkan 渲染的主要对象、执行流程和同步关系。
- 已区分 Surface、Swapchain、ImageView 的职责及所有权。
- 已区分 Graphics Queue、Present Queue 和 Queue Family。
- 已区分 Command Buffer 录制、Queue Submission 和 GPU 执行。
- 已区分 Graphics Pipeline、Dynamic Rendering 和 Image Layout。
- 已理解 Validation Layer 的作用及其不能保证程序完全正确的局限。
- 本章属于理论章节，无正式 Vulkan 代码产物，不要求编译及代码 Review。

**后续实践仍需验证：**
- 真实 Vulkan 对象的创建与 RAII 生命周期。
- GPU 同步、图像布局转换及 Swapchain 重建。
- Vulkan-Hpp / C++20 / MSVC 的实际编译兼容性。
- Validation Layers 的实际运行及错误诊断。

**章节状态：已完成（理论验收通过）。**

---

[返回学习地图](../VulkanLearningRoadmap.md)
