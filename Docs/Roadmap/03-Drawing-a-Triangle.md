# 03 — Drawing a Triangle

> Phase 1 · M1 First Triangle · [返回学习地图](../VulkanLearningRoadmap.md)

## 本章定位

在已验收的 M0 开发环境基础上，循序建立 `HelloTriangle` Sandbox。

从 GLFW 应用骨架出发，逐步创建 Vulkan Instance、设备与队列、呈现系统、图形管线、命令缓冲区及第一帧绘制。

各对象与同步逻辑必须在对应小节完成原理学习、代码实现、验证和审查，不提前整体实现。

- **当前状态：** 03.0 Setup（Base Code、Instance、Validation Layers、Physical Devices、Logical Device）全部通过验收。
- **当前小节：** 03.1.0 Window Surface，尚未开始。
- **里程碑：** M1 First Triangle，进行中，尚未完成。
- **工程入口：** `Sandbox/HelloTriangle/Main.cpp`。
- **当前约束：** 继续最小必要架构设计，不提前建立通用 RHI 或 Renderer Manager。

## 本章目录

| 教学单元 | 内容 | 当前进度 |
|---|---|---|
| [03.0 Setup](03-0-Setup.md) | Base Code、Instance、Validation Layers、Physical/Logical Device | 全部完成 |
| [03.1 Presentation](03-1-Presentation.md) | Surface、Swapchain、Image Views | Window Surface 下一步 |
| [03.2 Graphics Pipeline Basics](03-2-Graphics-Pipeline-Basics.md) | Shader、固定功能、Dynamic Rendering、Pipeline | 待开始 |
| [03.3 Drawing](03-3-Drawing.md) | Command Buffer、提交与呈现、Frames in Flight | 待开始 |
| [03.4 Swap Chain Recreation](03-4-Swap-Chain-Recreation.md) | Resize、资源重建及生命周期 | 待开始 |

## 当前课程位置

**已通过验收：**

- [03.0.0 Base Code](03-0-Setup.md#lesson-03-0-0)
- [03.0.1 Instance](03-0-Setup.md#lesson-03-0-1)
- [03.0.2 Validation Layers](03-0-Setup.md#lesson-03-0-2)
- [03.0.3 Physical Devices and Queue Families](03-0-Setup.md#lesson-03-0-3)
- [03.0.4 Logical Device and Queues](03-0-Setup.md#lesson-03-0-4)

**下一小节：**

- [03.1.0 Window Surface](03-1-Presentation.md#lesson-03-1-0)

**最近代码审查：**

- Logical Device and Queues 实现及最终 Code Review Commit：
  `394a3af52b93949e5ac28d1f789525a3d66c6cdc`
- 03.0 Setup 全部小节已经通过验收。
- 当前无未解决的 P0 / P1 问题。

**当前工程状态：**

- GLFW 窗口与事件循环已完成。
- Vulkan-Hpp RAII Context 与 Instance 已建立。
- GLFW required Instance Extensions 已完成检查。
- Debug 构建已配置 `VK_LAYER_KHRONOS_validation`。
- Debug 构建已配置 `VK_EXT_debug_utils`。
- Debug Messenger 与 Debug Callback 已实现。
- Debug Messenger 消息通道自测通过。
- 真实 Vulkan Validation VUID 实验通过。
- Debug / Release 策略符合当前项目约定。
- Debug Messenger 的生命周期由 RAII 管理。
- Physical Device 枚举、Properties、Features、Extensions 查询已完成。
- Queue Family 查询与 Graphics 支持检查已完成。
- Device Suitability 与 GPU Selection 已完成。
- 已选中 NVIDIA GeForce RTX 4080 Laptop GPU。
- Physical Device 由 Vulkan-Hpp RAII 对象保存。
- 已创建 Logical Device。
- 已启用所需 Device Features 和 `VK_KHR_swapchain`。
- 已获取 Graphics Queue。
- Vulkan-Hpp RAII Device 与 Queue 生命周期符合当前阶段要求。
- Debug / Release 编译运行正常。
- 尚未创建 Window Surface、Swapchain 或 Image Views。
- 尚未向 GPU 提交绘制命令。

## 本章工程约定

- 继续以现有 `HelloTriangle` 为实践基础。
- 保持 Vulkan 对象所有权与生命周期明确。
- 优先使用 Vulkan-Hpp RAII。
- 每个小节只实现当前必要功能。
- 不提前添加复杂 Renderer、Manager、RHI 或其他通用框架。
- 只有出现实际复用需求时才讨论 Core 抽象。
- 不将正常运行等同于 Vulkan Validation 或规范正确性。
- 测试中故意违反 Vulkan 规范的调用不得保留在正式代码中。
- 每个实践小节按约定完成本地构建验证和 GitHub Code Review。

详细任务与验收记录以对应教学单元文件为准。

全局里程碑状态以[学习地图](../VulkanLearningRoadmap.md#milestones)为准。

---

[上一章：02 Development Environment](02-Development-Environment.md) · [下一章：04 Vertex Buffers](04-Vertex-Buffers.md) · [返回学习地图](../VulkanLearningRoadmap.md)