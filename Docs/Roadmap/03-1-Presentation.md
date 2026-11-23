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

- [x] Surface Capabilities
  - 使用 `getSurfaceCapabilitiesKHR()` 查询能力
  - 理解 minImageCount、maxImageCount 的含义
  - 理解 maxImageCount 为 0 的特殊含义
  - 查询 Surface Extent 与其他支持限制
- [x] Surface Format / Color Space
  - 使用 `getSurfaceFormatsKHR()`
  - 区分 Image Format 与 Color Space
  - 优先选择 `B8G8R8A8Srgb + SrgbNonlinear`
  - 不支持偏好格式时使用可用格式
- [x] Present Mode
  - 使用 `getSurfacePresentModesKHR()`
  - 理解 Immediate、FIFO、FIFO Relaxed 和 Mailbox
  - 优先选择 Mailbox
  - 不支持 Mailbox 时回退 FIFO
- [x] Swap Extent
  - 理解 Window Size 与 Framebuffer Size 的区别
  - 理解高 DPI 对图像尺寸的影响
  - 优先使用 Surface 指定的 currentExtent
  - 在允许自行选择尺寸时使用 GLFW Framebuffer Size
  - 使用 std::clamp() 限制尺寸
  - 防止零尺寸 Swapchain 创建
- [x] Image Count
  - 根据 Surface 能力尽量请求至少三张图像
  - 正确处理最小和最大数量约束
  - 区分请求 Image Count 与实际 Image Count
- [x] Image Sharing Mode
  - 理解 Exclusive 与 Concurrent
  - 当前 Graphics/Present 使用同一个 Queue Family
  - 使用 Exclusive Sharing Mode
  - 暂不引入跨 Queue Family Ownership Transfer
- [x] Swapchain Images
  - 创建 `vk::raii::SwapchainKHR`
  - 使用 `getImages()` 取得 Swapchain Images
  - 使用 `std::vector<vk::Image>` 保存非拥有型句柄
  - 理解 Swapchain Image 不由应用单独销毁
- [x] Acquire → Render → Present 模型
  - 理解后续帧呈现流程
  - 区分图像句柄获取与图像 Acquire
  - 当前尚未执行 Acquire、Render 或 Present
- [x] Double / Triple Buffering 与 VSync
  - 理解多图像缓冲的调度意义
  - 理解 Mailbox 与 FIFO 的区别
  - 理解 Present Mode 与 Image Count 是独立配置

**实践：**

在 `HelloTriangle` 中创建 Swapchain，并取得 Swapchain Images。

**已完成实现：**

- 新增 `vk::raii::SwapchainKHR mSwapChain`。
- 新增 `std::vector<vk::Image> mSwapChainImages`。
- 保存 Swapchain Surface Format 与 Extent。
- 新增 `createSwapChain()`。
- 新增 Swapchain Format、Present Mode、Extent、Image Count 选择函数。
- 查询 Surface Capabilities、Formats 和 Present Modes。
- 选择受支持的 Composite Alpha。
- 正确设置 `vk::SwapchainCreateInfoKHR`。
- 创建 Swapchain。
- 获取实际 Swapchain Images。
- 在清理阶段先释放 Swapchain，再释放 Surface。
- 未提前创建 Image Views、Command Buffers 或绘制同步对象。

**本机运行结果：**

- GPU：NVIDIA GeForce RTX 4080 Laptop GPU
- Graphics/Present Queue Family Index：0
- Surface Min Image Count：2
- Surface Max Image Count：8
- Surface Current Extent：800 × 600
- Swapchain Format：B8G8R8A8Srgb
- Color Space：SrgbNonlinear
- Present Mode：Mailbox
- Swapchain Extent：800 × 600
- Composite Alpha：Opaque
- Requested Image Count：3
- Actual Swapchain Image Count：3

**架构与生命周期：**

- 继续使用 `HelloTriangleApplication`。
- 暂不新增 SwapchainManager 或 Renderer 类。
- Swapchain 依赖 Logical Device 和 Window Surface。
- Swapchain Image 句柄不拥有独立的 Vulkan Image 生命周期。
- 当前销毁顺序为清理 Image 句柄集合、销毁 Swapchain、销毁 Surface、销毁 GLFW Window。
- Device 在相关 Swapchain 资源有效期间保持存活。
- 当前尚未提交 GPU 工作，因此不需要 GPU Idle 等待。
- 未来真正提交 GPU 工作后，必须处理 GPU 完成同步。
- 后续创建 Image Views 后，必须保证其先于对应 Swapchain 销毁。
- Swapchain Recreation 留待对应小节实现。

**GitHub Code Review：**

首次实现 Commit：

`4afb6b023c04c9ededcb963bdcb65170b1f1095c`

修复与最终审查 Commit：

`5e52ac09100b1fb5361efe00185cc9357a062784`

第一轮审查发现：

1. P1：Composite Alpha 未验证 Surface 支持。
   - 原实现无条件使用 eOpaque。
   - 修复：优先使用受支持的 eOpaque，必要时回退到其他受支持模式。
   - 修复后满足 Vulkan Composite Alpha 有效使用要求。

2. P2：缺少 Swapchain Extent 零尺寸保护。
   - 修复：创建前检查 width 和 height 是否为零。
   - 无效尺寸时抛出明确异常。

3. P3：Framebuffer Size 的 width 未显式初始化。
   - 修复：width 和 height 均初始化为零。

4. P3：缺少 `<limits>` 显式头文件。
   - 修复：补充 `<limits>`。

第二轮审查结果：

- P1 已解决。
- P2 已解决。
- 两项 P3 已解决。
- Composite Alpha 选择符合 Surface 支持条件。
- Swapchain 参数配置符合当前 Vulkan 有效使用条件。
- Swapchain Image 所有权正确。
- RAII 资源生命周期正确。
- 无新增 P0 / P1 问题。
- GitHub 静态代码审查通过。

**编译与运行验证（2026-11-18）：**

- Debug Clean Build 成功。
- Release Clean Build 成功。
- 无新增编译警告。
- Debug 程序正常运行，退出码为 0。
- Release 程序正常运行，退出码为 0。
- 成功创建 Swapchain。
- 成功取得三张 Swapchain Images。
- 未出现新的 Validation Error。
- 本地运行结果由开发者提供。
- GitHub 审查为静态代码审查，不等同于导师独立运行。

**原理理解验收：**

四道原理题全部通过。

已理解：

1. Surface、Swapchain 与 Swapchain Images 的职责区别。
2. Swapchain Images 不由应用逐张销毁。
3. Image Count 的数量约束。
4. maxImageCount 为 0 的特殊含义。
5. Mailbox 与 FIFO 的呈现策略区别。
6. Present Mode 与 Image Count 相互独立。
7. Window Size 与 Framebuffer Size 的区别。
8. currentExtent 与自行选择 Extent 的条件。
9. Surface、Swapchain、Device 和 GLFW Window 的依赖关系。
10. CPU 端对象析构顺序与 GPU 完成同步是不同的问题。

**最终验收结论（2026-11-18）：**

- 理论理解通过。
- Surface Capabilities 查询通过。
- Surface Format 选择通过。
- Present Mode 选择通过。
- Swap Extent 选择通过。
- Image Count 选择通过。
- Composite Alpha 选择通过。
- Swapchain 创建通过。
- Swapchain Images 获取通过。
- RAII 生命周期审查通过。
- Debug / Release 本地验证通过。
- GitHub Code Review 通过。
- 无遗留 P0 / P1 问题。

**小节状态：已完成。**

**下一小节：**

03.1.2 — Image Views

---

<a id="lesson-03-1-2"></a>

## 03.1.2 — Image Views

- [x] Image 与 ImageView 的区别
  - 理解 Image 是图像资源，ImageView 是对图像的访问视图
  - ImageView 不复制底层 Image 数据
  - ImageView 不拥有底层 Image
  - 理解一张 Image 可以创建多个符合约束的 ImageViews
- [x] View Type
  - 理解 ImageView Type 与底层 Image 类型的关系
  - 当前 Swapchain ImageView 使用 `vk::ImageViewType::e2D`
- [x] Format
  - 使用 Swapchain Image 的实际 Format
  - 当前格式为 `vk::Format::eB8G8R8A8Srgb`
  - 不进行不必要的格式重新解释
- [x] Component Mapping
  - 理解 RGBA 通道映射的作用
  - 当前使用 Identity Mapping
- [x] Subresource Range
  - 理解 ImageSubresourceRange 的作用
  - 使用合法的 Image 子资源范围
- [x] Aspect / Mip Level / Array Layer
  - 当前 Swapchain ImageView 使用 Color Aspect
  - 使用第 0 个 Mip Level，数量为 1
  - 使用第 0 个 Array Layer，数量为 1
  - 区分 Mip Level 与 Array Layer
  - 理解 Mip Level 表示不同分辨率层级
  - 理解 Array Layer 表示同一 Image 中不同的数组切片
- [x] Vulkan-Hpp RAII 与资源所有权
  - 使用 `vk::raii::ImageView`
  - 使用 `std::vector<vk::raii::ImageView>` 管理全部视图
  - 正确处理部分 ImageView 创建失败时的异常清理
  - 保证 ImageViews 先于 Swapchain 销毁
- [x] Image Layout 与 Dynamic Rendering
  - 理解创建 ImageView 不会自动执行 Image Layout Transition
  - 理解 ImageView 和 Image Layout 的职责不同
  - 理解 `VkRenderingInfo` 与 `VkRenderingAttachmentInfo` 的附件指定关系
  - 理解 Dynamic Rendering 不需要传统的 `VkFramebuffer`
  - 暂不实现 Image Layout Transition 或绘制命令

**实践：**

为现有 Swapchain 的每张 Image 创建对应的 ImageView。

**已完成实现：**

- 新增 `std::vector<vk::raii::ImageView> mSwapChainImageViews`。
- 新增 `createImageViews()`。
- 在 `createSwapChain()` 后调用 `createImageViews()`。
- 使用 `mSwapChainImages` 遍历实际 Swapchain Images。
- 使用 `vk::ImageViewCreateInfo` 配置 ImageView。
- 设置 View Type 为 `e2D`。
- 使用 Swapchain Surface Format。
- 使用 Identity Component Mapping。
- 使用 Color Aspect。
- 设置 Mip Level 和 Array Layer 范围。
- 使用 `emplace_back()` 构造 RAII ImageViews。
- 使用 `reserve()` 预留 Vector 容量。
- 在 `cleanup()` 中先清理 ImageViews，再清理 Swapchain Images 和 Swapchain。
- 未提前创建 Graphics Pipeline 或 Command Buffer。

**对象所有权：**

- `mSwapChain` 是由应用管理的 RAII Swapchain 对象。
- `mSwapChainImages` 保存非拥有型 `vk::Image` 句柄。
- `mSwapChainImageViews` 保存应用拥有的 RAII ImageView 对象。
- 销毁 ImageView 不会销毁底层 Image。
- Swapchain Images 随对应 Swapchain 销毁而失效。
- 不对 Swapchain Images 单独调用 `vkDestroyImage()`。

**生命周期与异常安全：**

当前正常关闭时的清理顺序：

```text
mSwapChainImageViews.clear()
             |
             v
mSwapChainImages.clear()
             |
             v
mSwapChain.clear()
             |
             v
mSurface.clear()
             |
             v
glfwDestroyWindow()
             |
             v
glfwTerminate()
             |
             v
其余 Vulkan RAII 成员按声明逆序析构
```

- ImageViews 必须在对应 Swapchain 销毁之前销毁。
- `mSwapChainImageViews.clear()` 释放实际 Vulkan ImageView 对象。
- `mSwapChainImages.clear()` 只清空非拥有型 C++ 句柄集合。
- 若部分 ImageView 创建失败，已经成功构造的 RAII 对象仍由 Vector 持有。
- 异常路径调用 `cleanup()`，释放已创建的 ImageViews。
- 当前没有提交 GPU 绘制工作，不需要额外 GPU Idle 等待。
- 后续提交 GPU 工作后，必须先确认 GPU 不再使用相关对象，才能销毁 ImageViews 和 Swapchain。
- C++ RAII 析构顺序不能替代 GPU 执行同步。

**GitHub Code Review：**

实现及代码审查 Commit：

`ef3635c60186d64f009f36d483b54daef425fbc9`

审查结果：

- ImageView 创建顺序正确。
- `VkImageViewCreateInfo` 关键参数正确。
- View Type 与 Swapchain Images 匹配。
- Format 正确。
- Component Mapping 正确。
- Subresource Range 正确。
- ImageView 数量由实际 Swapchain Image 集合驱动。
- RAII 所有权明确。
- 部分创建失败时的资源清理正确。
- ImageViews 在 Swapchain 之前销毁。
- 未发现 P0 / P1 问题。
- GitHub 静态代码审查通过。

非阻断性建议：

- P3：可增加 Debug 输出 `mSwapChainImageViews.size()`。
- 该输出用于增强调试可观察性，不影响当前实现验收。
- 无需单独提交修改。

**编译与运行验证（2026-11-23）：**

- Debug Clean Build 成功。
- Release Clean Build 成功。
- 未出现新的 MSVC 编译警告。
- Debug 程序运行正常，退出码为 0。
- Release 程序运行正常，退出码为 0。
- 成功创建 Swapchain 并获得三张 Swapchain Images。
- `createImageViews()` 已接入初始化流程。
- 创建函数遍历全部 Swapchain Images。
- ImageView 创建失败会通过异常处理路径报告。
- 本次正常退出支持 ImageViews 创建成功的判断。
- 当前日志没有单独输出 ImageView Count。
- 未报告新的 Validation Error。
- 本地编译运行由开发者完成，导师进行 GitHub 静态代码审查。

**原理理解验收（2026-11-23）：**

四道原理题及两道补充题全部通过。

已理解：

1. Image 与 ImageView 的区别。
2. ImageView 不复制或拥有底层 Image。
3. ImageView 的 View Type、Format、Component Mapping。
4. Image Subresource Range。
5. Color、Depth 和 Stencil Aspect 的基本区别。
6. Mip Level 与 Array Layer 是独立的图像子资源维度。
7. Mip Level 和 Array Layer 均采用从 0 开始的索引。
8. Swapchain Images 与 ImageViews 的所有权区别。
9. Vulkan-Hpp RAII 对部分资源创建失败的异常安全作用。
10. ImageViews 必须先于 Swapchain 销毁。
11. 创建 ImageView 不会自动改变 Image Layout。
12. Dynamic Rendering 使用 RenderingInfo 和 RenderingAttachmentInfo 指定渲染附件。
13. Dynamic Rendering 不要求创建传统的 VkFramebuffer。
14. Image Layout 和 GPU 同步仍由应用显式管理。

**最终验收结论（2026-11-23）：**

- 理论理解通过。
- ImageView 创建通过。
- CreateInfo 参数审查通过。
- Image Subresource Range 审查通过。
- RAII 所有权审查通过。
- 异常安全审查通过。
- 生命周期审查通过。
- Debug / Release 本地验证通过。
- GitHub Code Review 通过。
- 无未解决 P0 / P1 问题。

**小节状态：已完成。**

---

## 03.1 Presentation — 阶段验收总结

**完成日期：2026-11-23。**

以下小节全部通过验收：

- 03.1.0 Window Surface
- 03.1.1 Swap Chain
- 03.1.2 Image Views

**当前工程成果：**

- 已通过 GLFW 创建 Vulkan Window Surface。
- 已实现 Graphics / Present Support 检查。
- 已使用同一个 Queue Family 满足 Graphics 与 Present 需求。
- 已创建 Vulkan Swapchain。
- 已根据 Surface Capabilities 选择交换链配置。
- 已获得三张 Swapchain Images。
- 已为全部 Swapchain Images 创建 RAII ImageViews。
- 已明确 Surface、Swapchain、Swapchain Images 和 ImageViews 的资源所有权。
- 已建立正确的资源清理顺序。
- Debug / Release 构建与运行正常。
- 当前没有未解决的 P0 / P1 问题。

**当前尚未实现：**

- Graphics Pipeline
- Shader Stages
- Dynamic Rendering 的实际命令录制
- Command Buffer
- Image Layout Transition
- GPU 工作提交与帧同步
- Swapchain Image Acquire / Present
- Swapchain Recreation

**架构结论：**

继续保留当前 `HelloTriangleApplication` 的渐进式结构。

现阶段不引入复杂 Window Manager、Swapchain Manager、RHI 或 Render Graph。

下一阶段根据真实代码职责继续评估模块边界，优先保持资源生命周期明确。

**阶段状态：03.1 Presentation 已全部完成。**

**下一教学单元：**

03.2 — Graphics Pipeline Basics

**下一小节：**

03.2.0 — Introduction

---

[返回第 03 章](03-Drawing-a-Triangle.md) · [返回学习地图](../VulkanLearningRoadmap.md)
