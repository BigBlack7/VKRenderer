# 03 — Drawing a Triangle

> Phase 1 · M1 First Triangle · [返回学习地图](../VulkanLearningRoadmap.md)

## 本章定位

在已验收的 M0 开发环境基础上，循序建立 `HelloTriangle` Sandbox。

从 GLFW 应用骨架出发，逐步创建 Vulkan Instance、设备与队列、呈现系统、图形管线、命令缓冲区及第一帧绘制。

各对象与同步逻辑必须在对应小节完成原理学习、代码实现、验证和审查，不提前整体实现。

- **当前状态：** 03.0 Setup、03.1 Presentation、03.2.0 Introduction 与 03.2.1 Shader Modules 已通过验收。
- **当前小节：** 03.2.2 Fixed Functions，尚未开始。
- **里程碑：** M1 First Triangle，进行中，尚未完成。
- **工程入口：** `Sandbox/HelloTriangle/Main.cpp`。
- **当前约束：** 继续使用最小必要架构，不提前建立通用 RHI 或 Renderer Manager。

## 本章目录

| 教学单元 | 内容 | 当前进度 |
|---|---|---|
| [03.0 Setup](03-0-Setup.md) | Base Code、Instance、Validation Layers、Physical/Logical Device | 全部完成 |
| [03.1 Presentation](03-1-Presentation.md) | Surface、Swapchain、Image Views | 全部完成 |
| [03.2 Graphics Pipeline Basics](03-2-Graphics-Pipeline-Basics.md) | Shader、固定功能、Dynamic Rendering、Pipeline | Introduction、Shader Modules 已完成；Fixed Functions 下一步 |
| [03.3 Drawing](03-3-Drawing.md) | Command Buffer、提交与呈现、Frames in Flight | 待开始 |
| [03.4 Swap Chain Recreation](03-4-Swap-Chain-Recreation.md) | Resize、资源重建及生命周期 | 待开始 |

## 当前课程位置

**已通过验收：**

- [03.0.0 Base Code](03-0-Setup.md#lesson-03-0-0)
- [03.0.1 Instance](03-0-Setup.md#lesson-03-0-1)
- [03.0.2 Validation Layers](03-0-Setup.md#lesson-03-0-2)
- [03.0.3 Physical Devices and Queue Families](03-0-Setup.md#lesson-03-0-3)
- [03.0.4 Logical Device and Queues](03-0-Setup.md#lesson-03-0-4)
- [03.1.0 Window Surface](03-1-Presentation.md#lesson-03-1-0)
- [03.1.1 Swap Chain](03-1-Presentation.md#lesson-03-1-1)
- [03.1.2 Image Views](03-1-Presentation.md#lesson-03-1-2)
- [03.2.0 Introduction](03-2-Graphics-Pipeline-Basics.md#lesson-03-2-0)
- [03.2.1 Shader Modules](03-2-Graphics-Pipeline-Basics.md#lesson-03-2-1)

**下一小节：**

- [03.2.2 Fixed Functions](03-2-Graphics-Pipeline-Basics.md#lesson-03-2-2)

**最近代码审查：**

- Shader 与 CMake 初步实现 Commit：
  `d779dd6e600edd9d9a8b5b96d1d09bb5556eec66`
- Shader Module 实现 Commit：
  `2f3b71d3f00e903b340e8dc9d599e0e5b50e36a8`
- Shader Module 修复与最终 Code Review Commit：
  `777fb2de56d01e55097a74605e88ed479798e4a7`
- 03.2.1 Shader Modules 全部知识单元已通过验收。
- 文件读取安全性 P0 问题已解决。
- 当前无未解决的 P0 / P1 问题。

## 当前工程状态

- GLFW Window 与事件循环已完成。
- Vulkan-Hpp RAII Context 与 Instance 已建立。
- Vulkan Validation Layers 与 Debug Messenger 已建立。
- Physical Device 枚举、能力查询和设备选择已完成。
- 当前选中 NVIDIA GeForce RTX 4080 Laptop GPU。
- Logical Device 已创建。
- Graphics/Present Queue 已创建并取得。
- Window Surface 已创建。
- Surface Presentation Support 检查已完成。
- Swapchain 创建与参数选择已完成。
- 当前 Swapchain 包含三张 Image。
- Image 句柄由 `std::vector<vk::Image>` 保存。
- ImageViews 由 `std::vector<vk::raii::ImageView>` 管理。
- ImageView 与 Swapchain Image 的映射关系明确。
- ImageViews 在 Swapchain 之前销毁。
- Swapchain 在 Surface 之前销毁。
- Surface 在 GLFW Window 之前销毁。
- 已建立 Slang Shader 源文件。
- 已实现 Vertex Shader 与 Fragment Shader。
- 已建立 CMake Shader 自动编译与增量构建流程。
- 已完成 SPIR-V 离线验证及反汇编。
- 已实现基于 EXE 所在目录的 Shader 资源定位。
- 已实现安全的 SPIR-V 二进制读取。
- 已创建 Vulkan-Hpp RAII Shader Module。
- 已配置两个 Shader Stage。
- Shader Module 局部 RAII 生命周期正确。
- Shader 文件缺失及损坏文件测试通过。
- Debug / Release 构建运行正常。
- 尚未创建 Pipeline Layout 或完整 Graphics Pipeline。

## 03.1 Presentation 阶段总结

**验收日期：2026-11-23。**

完成三个小节：

1. Window Surface
2. Swap Chain
3. Image Views

已建立从 GLFW Window 到 Vulkan Surface、Swapchain、Swapchain Images、ImageViews 的完整资源关系。

已明确：

- Instance 与 Surface 的关系。
- Device 与 Swapchain 的关系。
- Swapchain 与 Swapchain Images 的关系。
- Swapchain Images 与 ImageViews 的关系。
- 应用拥有对象与非拥有型 Vulkan 句柄的区别。
- RAII 生命周期与 Vulkan 对象依赖。
- CPU 资源清理与未来 GPU 同步的区别。

本阶段不要求实际渲染输出。

**阶段状态：03.1 Presentation 已全部完成。**

## 本章工程约定

- 继续以现有 `HelloTriangle` 为实践基础。
- 保持 Vulkan 对象所有权与生命周期明确。
- 优先使用 Vulkan-Hpp RAII。
- 每个小节只实现当前必要功能。
- 不提前添加复杂 Renderer、Manager、RHI 或 Render Graph。
- 只有出现实际复用需求时才讨论 Core 抽象。
- 遵循 Vulkan 1.4、Dynamic Rendering、Slang 的现代技术路线。
- 不将程序正常运行等同于 Vulkan 规范正确性。
- 测试中故意违反 Vulkan 规范的调用不得保留在正式代码中。
- 每个实践小节按约定完成本地构建验证和 GitHub Code Review。

详细任务与验收记录以对应教学单元文件为准。

全局里程碑状态以[学习地图](../VulkanLearningRoadmap.md#milestones)为准。

---

[上一章：02 Development Environment](02-Development-Environment.md) · [下一章：04 Vertex Buffers](04-Vertex-Buffers.md) · [返回学习地图](../VulkanLearningRoadmap.md)