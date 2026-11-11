# 15 — Migrating to glTF and KTX2

> Phase 5 · Vulkan 工程化与兼容性 · [总目录](../VulkanLearningRoadmap.md)
> 上一章：[14 Android — Optional](14-Android-Optional.md) ｜ 下一章：[16 Rendering Multiple Objects](16-Rendering-Multiple-Objects.md)
> 维护约定：本文件保存章节任务、实践与验收记录；总体进度只在[学习地图](../VulkanLearningRoadmap.md#current-progress)维护。

- [ ] 15.1 Introduction
- [ ] 15.2 OBJ → glTF 的动机
- [ ] 15.3 tinygltf
- [ ] 15.4 glTF Buffer / BufferView / Accessor
- [ ] 15.5 Mesh / Primitive / Attribute
- [ ] 15.6 glTF Indices
- [ ] 15.7 glTF Materials / Textures 基础
- [ ] 15.8 PNG / JPEG → KTX2 的动机
- [ ] 15.9 KTX2 Loading
- [ ] 15.10 KTX2 GPU Format / Mipmap / Layer
- [ ] 15.11 Cubemap / Texture Array 基础
- [ ] 15.12 Converting OBJ to glTF
- [ ] 15.13 Creating KTX2
- [ ] 15.14 Compression Options
- [ ] 15.15 Mipmap Generation
- [ ] 15.16 KTX2 Metadata
- [ ] 15.17 KTX2 Toolchain

**架构任务：**
重新设计 Assets 读取路径，为现代 Asset Pipeline 做准备。

---

[返回学习地图](../VulkanLearningRoadmap.md)
