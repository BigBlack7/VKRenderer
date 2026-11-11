# 03 — Drawing a Triangle

> Phase 1 · M1 First Triangle · [返回学习地图](../VulkanLearningRoadmap.md)

## 本章定位

在已验收的 M0 开发环境基础上，循序建立 `HelloTriangle` Sandbox。

从 GLFW 应用骨架出发，逐步创建 Vulkan Instance、设备与队列、呈现系统、图形管线、命令缓冲区及第一帧绘制。

各对象与同步逻辑必须在对应小节完成原理学习、代码实现、验证和审查，不提前整体实现。

- **当前状态：** 03.0.0 Base Code、03.0.1 Instance 已通过验收。
- **当前小节：** 03.0.2 Validation Layers，尚未开始。
- **里程碑：** M1 First Triangle，进行中，尚未完成。
- **工程入口：** `Sandbox/HelloTriangle/Main.cpp`。
- **当前约束：** 保留 `EnvironmentCheck`，不提前向 Core 引入 Window、Renderer、VulkanContext 等抽象。

## 本章目录

| 教学单元 | 内容 | 当前进度 |
|---|---|---|
| [03.0 Setup](03-0-Setup.md) | Base Code、Instance、Validation Layers、Physical/Logical Device | Base Code、Instance 已完成；Validation Layers 下一步 |
| [03.1 Presentation](03-1-Presentation.md) | Surface、Swapchain、Image Views | 待开始 |
| [03.2 Graphics Pipeline Basics](03-2-Graphics-Pipeline-Basics.md) | Shader、固定功能、Dynamic Rendering、Pipeline | 待开始 |
| [03.3 Drawing](03-3-Drawing.md) | Command Buffer、提交与呈现、Frames in Flight | 待开始 |
| [03.4 Swap Chain Recreation](03-4-Swap-Chain-Recreation.md) | Resize、资源重建及生命周期 | 待开始 |

## 当前课程位置

**已通过验收：**

- [03.0.0 Base Code](03-0-Setup.md#lesson-03-0-0)
- [03.0.1 Instance](03-0-Setup.md#lesson-03-0-1)

**下一小节：**

- [03.0.2 Validation Layers](03-0-Setup.md#lesson-03-0-2)

**最近代码审查：**

- Instance 首次实现 Commit：
  `832810c00ba809a9c4ebf39423c44f3a9cfdd5da`
- Instance 修复与最终代码审查 Commit：
  `80e4a23f09b36535ae51bb32b1cd93dc84d4f7b7`

**当前工程状态：**

- GLFW 窗口与事件循环已完成。
- Vulkan-Hpp RAII Context 与 Instance 已建立。
- GLFW required Instance Extensions 已实现检查。
- Instance 生命周期符合当前学习阶段要求。
- Debug / Release 编译运行通过。
- C4819 编码警告已在本地解决，待统一提交复核。
- 无未解决的 P0 / P1 问题。
- 尚未创建 Vulkan Debug Messenger、Physical Device、Logical Device 或 Swapchain。

## 本章工程约定

- 继续以现有 `HelloTriangle` 为实践基础。
- 保持 Vulkan 对象所有权与生命周期明确。
- 优先使用 Vulkan-Hpp RAII。
- 每个小节只实现当前必要功能。
- 不提前添加复杂 Renderer、Manager、RHI 或其他通用框架。
- 只有出现实际复用需求时才讨论 Core 抽象。
- 不将正常运行等同于 Vulkan Validation 或规范正确性。
- 每个实践小节按约定完成本地构建验证和 GitHub Code Review。

详细任务与验收记录以对应教学单元文件为准。

全局里程碑状态以[学习地图](../VulkanLearningRoadmap.md#milestones)为准。

---

[上一章：02 Development Environment](02-Development-Environment.md) · [下一章：04 Vertex Buffers](04-Vertex-Buffers.md) · [返回学习地图](../VulkanLearningRoadmap.md)