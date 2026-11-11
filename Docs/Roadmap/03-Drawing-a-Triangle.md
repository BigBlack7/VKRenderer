# 03 — Drawing a Triangle

> Phase 1 · M1 First Triangle · [返回学习地图](../VulkanLearningRoadmap.md)

## 本章定位

在已验收的 M0 开发环境基础上，循序建立 `HelloTriangle` Sandbox，从 GLFW 应用骨架过渡到 Vulkan Instance、设备与队列、呈现系统、图形管线、命令与第一帧绘制。各对象与同步逻辑必须在对应小节学习和验证，不提前整体实现。

- **当前状态：** 03.0.0 Base Code 已验收；03.0.1 Instance 尚未开始。
- **里程碑：** M1 First Triangle，尚未完成。
- **工程入口：** `Sandbox/HelloTriangle/Main.cpp`。
- **当前约束：** 保留 `EnvironmentCheck`，不提前向 Core 引入新的 Window/Renderer/Context 层。

## 本章目录

| 教学单元 | 内容 | 当前进度 |
|---|---|---|
| [03.0 Setup](03-0-Setup.md) | Base Code、Instance、Validation、Physical/Logical Device | Base Code 完成；Instance 下一步 |
| [03.1 Presentation](03-1-Presentation.md) | Surface、Swapchain、Image Views | 待开始 |
| [03.2 Graphics Pipeline Basics](03-2-Graphics-Pipeline-Basics.md) | Shader、固定功能、Dynamic Rendering、Pipeline | 待开始 |
| [03.3 Drawing](03-3-Drawing.md) | Command Buffer、提交与呈现、Frames in Flight | 待开始 |
| [03.4 Swap Chain Recreation](03-4-Swap-Chain-Recreation.md) | Resize、资源重建及生命周期 | 待开始 |

## 当前课程位置

- **已验收：** [03.0.0 Base Code](03-0-Setup.md#lesson-03-0-0)
- **下一小节：** [03.0.1 Instance](03-0-Setup.md#lesson-03-0-1)
- **审查基线：** `b99f2e8df049efb61ddbe5f00a20f10516f6aac5`（最后的代码修复 Commit）。

详细任务与验收记录以各教学单元文件为准；全局里程碑状态以[学习地图](../VulkanLearningRoadmap.md#milestones)为准。

---

[上一章：02 Development Environment](02-Development-Environment.md) · [下一章：04 Vertex Buffers](04-Vertex-Buffers.md) · [返回学习地图](../VulkanLearningRoadmap.md)
