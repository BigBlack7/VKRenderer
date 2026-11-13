# VKRenderer · Vulkan 学习地图

> **使用方式：** 先从本地图定位小节，再进入 `Docs/Roadmap/` 对应教材文件。
> **技术路线：** Windows + VS Code + MSVC + CMake + C++20 · Vulkan 1.4 · Vulkan-Hpp RAII · Dynamic Rendering · Slang。
> **总纲来源：** `BigBlack7/VKRenderer`，基于 Khronos Vulkan Tutorial（latest）。

## 从这里开始

- **当前学习目标：** [03.0.3 Physical Devices and Queue Families](Roadmap/03-0-Setup.md#lesson-03-0-3)
- **刚完成：** [03.0.2 Validation Layers](Roadmap/03-0-Setup.md#lesson-03-0-2)
- **当前里程碑：** M1 First Triangle — 进行中，尚未完成
- **进度概览：** [第 6 节 · 当前进度](#current-progress)
- **GitHub 仓库：** [BigBlack7/VKRenderer](https://github.com/BigBlack7/VKRenderer)

## 课程阅读与维护说明

- **本地图（当前文件）**：提供阅读入口、全局进度、学习规范、里程碑、Review 检查项和 Core 准则，不复制逐小节的代码/验收记录。
- **章节文件（`Roadmap/`）**：保留原 Roadmap 章节中的全部任务、实践产物、环境信息、设计决策、审查和验收记录。
- **第 03 章特别拆分**：由于持续增长，按五个自然教学单元分别维护，并保留[本章目录页](Roadmap/03-Drawing-a-Triangle.md)。
- **命名稳定**：编号采用官方课程顺序，文件名保持英文与稳定序号，文件中的教学内容以中文为主。
- **只有验收后才勾选**：除进度同步和纠错外，不因拆分而改变未开始章节的任务状态。

## 教学目录

### Phase 0 — 世界观与开发环境

- **[00 Introduction](Roadmap/00-Introduction.md)** — 入门、现代 Vulkan 技术路线与教程结构（✅ 已完成）
- **[01 Overview](Roadmap/01-Overview.md)** — Vulkan 原理、编码规范、首帧全流程与 Validation 理论（✅ 已完成）
- **[02 Development Environment](Roadmap/02-Development-Environment.md)** — Windows/MSVC/CMake/Vulkan SDK/ThirdParty 工具链基线（✅ 已完成）

### Phase 1 — Drawing a Triangle

- **[03 Drawing a Triangle](Roadmap/03-Drawing-a-Triangle.md)** — 首个 Triangle：完整渲染实践主线（进行中：03.0.0 Base Code、03.0.1 Instance、03.0.2 Validation Layers 已完成）
  - [03.0 — Setup](Roadmap/03-0-Setup.md)
  - [03.1 — Presentation](Roadmap/03-1-Presentation.md)
  - [03.2 — Graphics Pipeline Basics](Roadmap/03-2-Graphics-Pipeline-Basics.md)
  - [03.3 — Drawing](Roadmap/03-3-Drawing.md)
  - [03.4 — Swap Chain Recreation](Roadmap/03-4-Swap-Chain-Recreation.md)

### Phase 2 — GPU Resources

- **[04 Vertex Buffers](Roadmap/04-Vertex-Buffers.md)** — Vertex Input、GPU Buffer、Staging 与 Index（待开始）
- **[05 Uniform Buffers](Roadmap/05-Uniform-Buffers.md)** — Uniform Buffer 与 Descriptor（待开始）
- **[06 Texture Mapping](Roadmap/06-Texture-Mapping.md)** — 图像、采样器与纹理映射（待开始）

### Phase 3 — 3D Rendering Foundations

- **[07 Depth Buffering](Roadmap/07-Depth-Buffering.md)** — 深度缓冲（待开始）
- **[08 Loading Models](Roadmap/08-Loading-Models.md)** — 模型读取与顶点去重（待开始）
- **[09 Generating Mipmaps](Roadmap/09-Generating-Mipmaps.md)** — Mipmap 与图像 LOD（待开始）
- **[10 Multisampling](Roadmap/10-Multisampling.md)** — MSAA 与 Resolve（待开始）

### Phase 4 — GPU Programming

- **[11 Compute Shader](Roadmap/11-Compute-Shader.md)** — Graphics + Compute（待开始）

### Phase 5 — Vulkan 工程化与兼容性

- **[12 Ecosystem Utilities and GPU Compatibility](Roadmap/12-Ecosystem-Utilities-and-GPU-Compatibility.md)** — Vulkan 生态工具与 GPU 兼容性（待开始）
- **[13 Vulkan Profiles](Roadmap/13-Vulkan-Profiles.md)** — Vulkan Profiles（待开始）
- **[14 Android — Optional](Roadmap/14-Android-Optional.md)** — Android（可选）（待开始）
- **[15 Migrating to glTF and KTX2](Roadmap/15-Migrating-to-glTF-and-KTX2.md)** — glTF 与 KTX2（待开始）
- **[16 Rendering Multiple Objects](Roadmap/16-Rendering-Multiple-Objects.md)** — 多对象渲染（待开始）
- **[17 Multithreading](Roadmap/17-Multithreading.md)** — 多线程 Vulkan（待开始）

### Phase 6 — Advanced

- **[18 Ray Tracing — Advanced](Roadmap/18-Ray-Tracing.md)** — Ray Tracing（Advanced）（待开始）

### Phase 7 — Building VKRenderer

- **[Building VKRenderer](Roadmap/Engine-Building-VKRenderer.md)** — 将 Sandbox 知识收束为 VKRenderer Core（待开始）


## 0. 使用说明

本文件是课程的总目录、学习规范、里程碑和当前进度地图。各章节任务与验收记录保存在 `Docs/Roadmap/` 中。

- `[ ]`：尚未完成
- `[x]`：已达到对应小节的验收要求
- `Optional`：可选章节，不阻塞主线
- `Advanced`：高级专题，完成必要前置学习后进入


每完成一个教学小节：

1. 完成本节理论学习，以及该节要求的代码实践。
2. 理论小节通过理解检查后，可直接标记完成并继续下一节。
3. 简单理论章节允许在同一会话中连续推进，无需逐节 Git 提交。
4. 理论大章节结束后，统一更新学习进度并提交 Git。
5. 实践小节需完成相应的编译、运行和必要的 Validation。
6. 实践代码按阶段或里程碑提交 GitHub，进行 Code Review。
7. 修正问题并通过验收后，将对应 `[ ]` 修改为 `[x]`。
8. 如果出现稳定且重复的抽象，再讨论是否提升到 Core。

> **原则：先验证，后抽象。Sandbox 是实验场，Core 是稳定复用层。**

---

## 1. 项目架构约定

基础目录：

```text
VKRenderer/
├─ Assets/
│  ├─ Model/
│  └─ Texture/
├─ Core/
├─ Docs/
│  ├─ VulkanLearningRoadmap.md
│  └─ Roadmap/
├─ Sandbox/
├─ Slang/
├─ ThirdParty/
├─ CMakeLists.txt
├─ .gitignore
└─ README.md
```

职责约定：

- `Sandbox/`
  - Vulkan 学习、实验和章节 checkpoint。
  - 每个有独立实验意义的用例都应能够单独编译和运行。
  - Sandbox 之间禁止互相依赖。
  - Sandbox 可以依赖 Core 和 ThirdParty。

- `Core/`
  - 只放经过多个 Sandbox 验证、责任边界已经明确的可复用代码。
  - 不因为“以后可能会用到”而提前设计抽象。
  - Core API 修改后，需要重新验证所有依赖它的 Sandbox。

- `ThirdParty/`
  - 工程内显式管理第三方依赖。
  - 当前基础依赖：GLFW、GLM。
  - 后续 tinyobjloader、stb、tinygltf、KTX 等按课程进度加入。
  - Vulkan SDK 属于开发环境依赖，不复制进 ThirdParty。

- `Slang/`
  - GPU Shader 源码。
  - 编译后的 SPIR-V 属于构建产物，应输出到 Build 目录，而不是源码树。

- `Assets/`
  - Runtime 资源。
  - `Model/`：模型资源。
  - `Texture/`：纹理资源。

- `Docs/`
  - `VulkanLearningRoadmap.md` 是导航、规则和全局进度唯一入口。
  - `Roadmap/` 为各章任务、实现与验收记录的唯一维护位置。

---

## 2. 固定教学流程

每个正式小节按照以下顺序推进：

1. **官方文档拆解**
   - 逐段解释官方章节。
   - 明确这一节在完整 Vulkan 系统中的位置。

2. **原理**
   - 为什么 Vulkan 需要这个对象 / 状态 / 流程。
   - CPU、Driver、GPU、Presentation Engine 之间发生什么。

3. **API**
   - Vulkan-Hpp / RAII 对象。
   - CreateInfo、Flags、Feature、Extension、生命周期和依赖关系。

4. **代码**
   - 在当前 Sandbox 中完成最直接、最容易理解的实现。

5. **验证**
   - Validation Layer。
   - RenderDoc / vkconfig / vulkaninfo 等工具按需要加入。

6. **架构判断**
   - 是否出现真正稳定的重复逻辑。
   - 是否值得提升到 Core。
   - 不强行为每个 Vulkan handle 创建 wrapper class。

7. **作业与 Review**
   - Push GitHub。
   - 检查正确性、RAII 生命周期、CMake、现代 C++、性能隐患和 Vulkan Validation。

---

<a id="milestones"></a>

## 3. 关键里程碑

| Milestone | 目标 | 状态 |
|---|---|---|
| M0 | 开发环境 / CMake / ThirdParty / SDK 基线 | [x] |
| M1 | First Triangle | [ ] |
| M2 | Vertex + Index Buffer | [ ] |
| M3 | Uniform / Descriptor / Transform | [ ] |
| M4 | Texture Mapping | [ ] |
| M5 | Depth + Model + Mipmap + MSAA | [ ] |
| M6 | Compute Shader | [ ] |
| M7 | GPU Compatibility / Profiles | [ ] |
| M8 | glTF + KTX2 | [ ] |
| M9 | Multiple Objects | [ ] |
| M10 | Multithreaded Vulkan | [ ] |
| M11 | Ray Tracing | [ ] |
| M12 | VKRenderer Core Architecture | [ ] |

---

## 4. 每次仓库 Review 的固定检查项

每完成一小节，Review 至少检查：

- [ ] 功能结果与官方章节目标一致
- [ ] Vulkan Validation 无未解释错误
- [ ] Vulkan 对象创建和销毁依赖正确
- [ ] RAII 生命周期正确
- [ ] 没有悬空引用 / handle
- [ ] Move / copy 语义合理
- [ ] Exception 不被错误吞掉
- [ ] CMake target dependency 正确
- [ ] Debug / Release 行为合理
- [ ] 不硬编码本机绝对路径
- [ ] Shader 构建依赖正确
- [ ] Assets path 不依赖脆弱的 `../../../`
- [ ] Sandbox 之间无依赖
- [ ] Core 没有过度抽象
- [ ] 没有不必要的 `device.waitIdle()`
- [ ] 同步逻辑能够明确解释 stage / access / semaphore / fence
- [ ] 代码命名遵守 VKRenderer 约定
- [ ] README / 本学习地图及对应章节在需要时同步更新

---

## 5. Core 提升准则

一段代码进入 Core 前至少满足以下条件：

1. 已在一个或多个 Sandbox 中实际使用。
2. Responsibility 可以用一句话解释。
3. 输入、输出和所有权关系稳定。
4. Vulkan 对象依赖关系明确。
5. 抽象不会隐藏当前正在学习的关键 Vulkan 概念。
6. 抽出来能减少真实重复，而不是为了“看起来像引擎”。
7. 旧 Sandbox 在重构后仍能正常编译运行。

如果不能满足，则继续保留在 Sandbox。

---

<a id="current-progress"></a>

## 6. 当前进度

**记录日期：2026-11-13。**

当前按照 Khronos Vulkan Tutorial（latest）推进。

```text
Phase 0 — World View & Environment
  00 Introduction              [COMPLETED]
  01 Overview                  [COMPLETED]
  02 Development Environment   [COMPLETED]  M0

Phase 1 — Drawing a Triangle   [IN PROGRESS]
  03.0.0 Base Code             [COMPLETED]
  03.0.1 Instance              [COMPLETED]
  03.0.2 Validation Layers     [COMPLETED]
  03.0.3 Physical Devices      ← NEXT / NOT STARTED

M1 First Triangle              [IN PROGRESS]
```

**已通过验收：**

- [00 Introduction](Roadmap/00-Introduction.md)：2026-10-09，理论验收通过。
- [01 Overview](Roadmap/01-Overview.md)：2026-11-10，理论验收通过。
- [02 Development Environment](Roadmap/02-Development-Environment.md)：2026-11-10，M0 最终验收通过。
- [03.0.0 Base Code](Roadmap/03-0-Setup.md#lesson-03-0-0)：2026-11-11，理论、实现、编译运行与 GitHub Review 通过。
- [03.0.1 Instance](Roadmap/03-0-Setup.md#lesson-03-0-1)：2026-11-11，Instance 创建、扩展检查、RAII 生命周期及 GitHub Review 通过。
- [03.0.2 Validation Layers](Roadmap/03-0-Setup.md#lesson-03-0-2)：2026-11-13，理论、Debug Messenger 自测、真实 Validation VUID 实验及阶段性 GitHub Review 通过；最终清理版本待统一提交复核。

**当前工程基线：**

- GitHub：`BigBlack7/VKRenderer`。
- 默认分支：`main`。
- 最近一次代码审查 Commit：`47c86e00abdbff4b651355c4174ca344683ef27c`。
- 当前技术栈：Windows x64、Visual Studio 2022、MSVC、CMake、C++20。
- Vulkan SDK / Loader：1.4.357。
- GLFW：3.4。
- Slang：2026.13.1。
- 主要 GPU：NVIDIA GeForce RTX 4080 Laptop GPU。
- `EnvironmentCheck` 继续作为 M0 环境验证 Sandbox 保留。
- `HelloTriangle` 为 M1 正式实践 Sandbox。
- GLFW 窗口、事件循环及异常边界已完成。
- Vulkan-Hpp RAII Context 与 Instance 已创建。
- GLFW 必需 Instance Extensions 已完成检查。
- Vulkan Validation Layer 已完成 Debug 构建配置。
- `VK_EXT_debug_utils` 已在 Debug 配置中启用。
- Debug Messenger 与 Debug Callback 已完成实现。
- Debug Messenger 的 RAII 生命周期已通过静态审查。
- 已通过受控实验验证真实 Vulkan VUID 诊断。
- 尚未创建 Physical Device、Logical Device、Surface 或 Swapchain。
- `Core` 暂无需要提前抽象的 Vulkan 模块。
- 当前无未解决的 P0 / P1 问题。

**最近验证：**

- Debug / Release 编译与运行成功。
- Debug 构建中的自定义 Debug Utils 消息能够触发 Callback。
- Release 构建按应用配置不显式启用 Validation Layer，也不创建 Debug Messenger。
- 使用无效 `messageSeverity = 0` 成功触发真实 Validation Error。
- 目标 VUID：`VUID-vkSubmitDebugUtilsMessageEXT-messageSeverity-parameter`。
- 实验进程退出码为 0。
- 临时测试代码已由开发者确认移除。
- 最终清理代码与本次文档更新将统一提交 GitHub 复核。
- 不把成功运行或无 Validation Error 等同于程序完全正确。

**下一步：**

[03.0.3 Physical Devices and Queue Families](Roadmap/03-0-Setup.md#lesson-03-0-3)

下一阶段重点：

- Physical Device 与 Logical Device 的职责区别。
- 枚举可用 Physical Devices。
- Device Properties。
- Device Features。
- Device Extensions。
- Queue Family Properties。
- Graphics / Compute / Transfer Queue Families。
- GPU Suitability 与设备选择策略。
- Vulkan-Hpp RAII 的 Physical Device 查询方式。
- 设备能力与 API Version 的关系。

继续在现有 `HelloTriangle` Sandbox 中渐进实现。

不提前创建 Logical Device、Window Surface 或 Swapchain。

**进度维护原则：**

- 每个小节的详细实现、问题修复及验收记录保存在对应章节文件。
- 本学习地图只维护当前任务、已验收小节、工程基线与里程碑状态。
- M1 First Triangle 尚未完成。
- 下一小节通过独立验收后，才能继续推进学习进度。

## 学习与 Git 约定

- 简单理论章节可在同一会话中连续推进。
- 理论小节通过验收后即可继续下一小节，不要求立即提交 Git。
- 理论大章节结束时，统一整理学习记录并提交 Git。
- 复杂实践章节根据自然的知识单元与工程里程碑拆分会话。
- 实践代码需要编译、运行、必要的 Validation 和 GitHub Review。
- 新会话优先核查当前 Roadmap、最新 Commit 和工程状态。
- 不将理论验收等同于编译或运行验收。
- 不因环境配置细节而过度设计架构。

---

## 官方参考

- Khronos Vulkan Tutorial: https://docs.vulkan.org/tutorial/latest/
- Vulkan Specification: https://docs.vulkan.org/spec/latest/
- Vulkan-Hpp: https://github.com/KhronosGroup/Vulkan-Hpp
- Slang: https://shader-slang.org/
- Vulkan SDK: https://vulkan.lunarg.com/

---

> 课程总纲会随着 Khronos 官方教程和 VKRenderer 架构演进更新。  
> 进度以本学习地图、对应章节文件中的 checkbox 和仓库中可运行的 Sandbox 为准。
