# 03.1 — Presentation

> Phase 1 · [第 03 章目录](03-Drawing-a-Triangle.md) · [学习地图](../VulkanLearningRoadmap.md)

> 上一单元：[03.0 Setup](03-0-Setup.md) ｜ 下一单元：[03.2 Graphics Pipeline Basics](03-2-Graphics-Pipeline-Basics.md)

<a id="lesson-03-1-0"></a>

## 03.1.0 — Window Surface

- [x] WSI（Window System Integration）
  - 理解 Vulkan 与窗口系统的交互机制
  - 区分 Surface 与 Swapchain
  - 理解 Vulkan 无窗口渲染的可能性
- [x] Vulkan 的平台无关性
  - 理解平台无关的 Surface 抽象
  - 理解不同窗口系统具有不同的 Surface 创建方式
- [x] SurfaceKHR
  - 使用 `glfwCreateWindowSurface()` 创建 Surface
  - 检查 `VkResult`
  - 使用 `vk::raii::SurfaceKHR` 接管原始 Handle
  - 理解 Surface 与 Instance 的依赖关系
- [x] Win32 Surface 与 GLFW Abstraction
  - 理解 GLFW 对平台差异的封装
  - 复用已启用的 Instance WSI Extensions
  - 不直接引入 Win32 平台专用窗口创建代码
- [x] Presentation Support
  - 使用 `getSurfaceSupportKHR()` 查询设备呈现能力
  - 理解 Presentation Support 与具体 Surface 关联
  - 将 Graphics + Present 条件加入设备适用性检查
- [x] Graphics Queue 与 Present Queue 的关系
  - 区分 Graphics 能力和 Present 能力
  - 当前选择同时满足两者的 Queue Family
  - 理解不同队列族方案的复杂性
  - 不提前实现多队列呈现

**实践：**

在 `HelloTriangle` 中创建 Vulkan Window Surface，
并检查选中设备的 Graphics / Present Queue 支持情况。

**已完成实现：**

- 新增 `vk::raii::SurfaceKHR mSurface{nullptr}`。
- 新增 `createSurface()`。
- 使用 GLFW 创建 Vulkan Surface。
- 使用 Vulkan-Hpp RAII 管理 Surface。
- 将 Surface 创建放在 Physical Device 选择之前。
- 在 `isDeviceSuitable()` 中检查 Graphics + Present Support。
- 在 `createLogicalDevice()` 中查找 Graphics/Present Queue Family。
- 使用符合条件的队列族创建 Logical Device 和 Queue。
- 在 `cleanup()` 中先清理 Surface，再销毁 GLFW Window。
- 未创建 Swapchain。
- 未执行实际 Present 操作。

**架构与生命周期：**

- 保持单窗口 Application 架构。
- 暂不新增 Window Manager 或通用 Window 类。
- Surface 依赖 Vulkan Instance。
- GLFW Window 由当前 Application 管理。
- Surface 必须在其所属 Instance 销毁之前销毁。
- 当前显式保证 Surface 在 GLFW Window 销毁之前释放。
- 未来创建 Swapchain 后，必须确保 Swapchain 先于 Surface 销毁。
- 当前尚无 GPU 工作提交，不需要引入额外 GPU 同步机制。
- 后续根据 Swapchain、Resize 等实际需求渐进调整资源清理方案。

**GitHub Code Review：**

实现 Commit：

`2302ae2a223882d0b3511f632a3d1b71da04f0bf`

审查结果：

- Surface 创建方式正确。
- GLFW API 与 Vulkan-Hpp RAII 衔接正确。
- Vulkan Surface 所有权清晰。
- Surface 创建错误处理正确。
- Presentation Support 查询正确。
- 设备适用性检查正确。
- Graphics/Present Queue Family 选择正确。
- Surface 与 GLFW Window 的清理顺序正确。
- 未发现 P0 / P1 问题。
- GitHub 静态审查通过。

非阻断改进：

- P3：建议在 Debug 模式输出 Graphics/Present Queue Family Index。
- P3：建议更新 Main.cpp 顶部对象关系图，加入 Surface。

**编译与运行验证（2026-11-16）：**

- Debug Clean Build 成功。
- Release Clean Build 成功。
- Debug 运行正常，退出码为 0。
- Release 运行正常，退出码为 0。
- 成功选中 NVIDIA GeForce RTX 4080 Laptop GPU。
- Surface 创建与 Graphics/Present Queue 检查路径成功执行。
- 未报告新的 Validation Error。
- 当前代码选择一个同时支持 Graphics 与 Present 的 Queue Family。
- 根据本机 Queue Family 信息及成功路径，实际选中的队列族为 Index 0。
- 本地运行证据由开发者提供；GitHub 审查为静态代码审查。

**原理理解验收：**

四道原理题全部通过。

已理解：

1. WSI 与 Vulkan Window Surface 的作用。
2. `VkSurfaceKHR` 与 `VK_KHR_surface` 的区别。
3. `VkSwapchainKHR` 与 `VK_KHR_swapchain` 的区别。
4. GLFW 与 Win32 Surface 创建方式的关系。
5. Graphics Queue 不一定支持 Presentation。
6. Presentation Support 需要结合特定 Surface 查询。
7. Physical Device 可在创建 Logical Device 前查询 Surface Support。
8. Graphics/Present 使用同一 Queue Family 的简化策略。
9. Vulkan RAII 成员顺序不能自动管理外部 GLFW Window 的生命周期。
10. Surface、Swapchain 和 GLFW Window 的清理顺序必须明确。

**最终验收结论（2026-11-16）：**

- 理论理解通过。
- Surface 创建通过。
- Surface RAII 所有权检查通过。
- Presentation Support 查询通过。
- Graphics/Present Queue 选择通过。
- 生命周期及异常路径审查通过。
- Debug / Release 本地验证通过。
- GitHub Code Review 通过。
- 无未解决 P0 / P1 问题。

**小节状态：已完成。**

**下一小节：**

03.1.1 — Swap Chain

---

<a id="lesson-03-1-1"></a>

## 03.1.1 — Swap Chain

- [ ] Surface Capabilities
- [ ] Surface Format / Color Space
- [ ] Present Mode
- [ ] Swap Extent
- [ ] Image Count
- [ ] Image Sharing Mode
- [ ] Swapchain Images
- [ ] Acquire → Render → Present 模型
- [ ] Double / Triple Buffering 与 VSync

**实践：**
创建 Swapchain 并获取 Swapchain Images。

---

<a id="lesson-03-1-2"></a>

## 03.1.2 — Image Views

- [ ] Image 与 ImageView 的区别
- [ ] View Type
- [ ] Format
- [ ] Component Mapping
- [ ] Subresource Range
- [ ] Aspect / Mip Level / Array Layer

**实践：**
为所有 Swapchain Image 建立 ImageView。

---

[返回第 03 章](03-Drawing-a-Triangle.md) · [返回学习地图](../VulkanLearningRoadmap.md)
