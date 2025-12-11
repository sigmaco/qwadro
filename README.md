![Logo](doc/qwadro-banner-2048x520-bg-t.png)

# The Qwadro Execution Ecosystem
Qwadro is a name for a family of standards specifying ***software engineering infrastructures*** designed by the **SIGMA Technology Group** that defines the interface between an operating system and application software. The primary goal of Qwadro is to ensure that applications written for one Qwadro-compliant system can be easily ported to another, regardless of the underlying hardware or specific operating system implementation. This portability is achieved by standardizing the interfaces, utilities, and libraries that applications use to interact with the operating system. Qwadro provides a common ground, fostering interoperability and reducing the complexities associated with platform-specific development. Adherence to Qwadro standards simplifies software development, maintenance, and deployment across diverse platform environments.

The main design force behind Qwadro is to fix the main problem with POSIX. While POSIX is intended to specify how operating system must be made, Qwadro aims to standardize middleware development and deployment by providing unified architectures for APIs, specifically bridging low-level C17 APIs and abstracting them across various platforms. This can make it easier to build and deploy software on different operating systems without worrying too much about platform-specific intricacies.

Qwadro is data-driven and designed to handle distributed, federated systems. Instead of focusing solely on individual devices or local systems, it facilitates an interconnected system where data and tasks can be managed across multiple nodes or services, making it a potential backbone for cloud-based, edge computing, or distributed systems. The term federated execution expresses a collaborative execution model, where components work together in a decentralized or distributed way, which could be beneficial for large-scale applications requiring high scalability and flexibility.

Middleware typically helps applications communicate and manage resources without needing to interact directly with the OS. Qwadro offers a middleware layer that abstracts system software, providing standardized services, APIs, drivers, and libraries to improve performance and accelerate development. This is ideal for industries and use cases that require both high performance and platform portability, such as simulation, interactive entertainment, and multimedia (e.g., gaming, AR/VR, video processing).

Qwadro is not a single framework like Qt; it's an ecosystem of modular middleware components, each designed to handle specific platform aspects (e.g., graphics, shell, file systems). These middleware layers can be easily swapped or replaced after compilation, allowing for dynamic updates or customizations. So, by design, Qwadro Execution Ecosystem is a amalgamation of software infrastructures intended to offer a set of acceleration building blocks to edify system softwares targeting firstly computer simulation, interactive entertainment and multimedia user experience. Qwadro constitutes a unified infrastructure of and for middlewares, libraries, drivers and engines, continuously integrated, providing resources, devices, services and abstracting operating systems and platforms in a straightforwared uniform way. It is released as open source under the Qwadro License.

The "Public Test Build" featured everywhere is why Qwadro is primary a private project intented to unify most of codebase for SIGMA past, current and future projects, mainly some being desired to run on the operating systems being engineered by SIGMA for internal support and use, and to attend other divisons and entities partnered, also potentially benefiting the historical and regional audience of SIGMA efforts.

Consequentially, an OpenGL-based Draw I/O System ICD is currently and will continue to be maintained, as well as an Windows 7-based User I/O System ICD.

## Acceleration Framework Experimental (AFX)
The **AFX**, alphabetism and initialism for **Acceleration Framework Experimental**, is the standard implementation for **Qwadro Execution Ecosystem**. Qwadro is a name for a familiy of standards specifying software engineering infrastructures designed by SIGMA Technology Group. 

These standards are written in ISO C17 and specifies low-level, data-oriented components and its intercommunication protocols in an federated execution ecosystem; the Qwadro Execution Ecosystem. 

This ecosystem offers a set of acceleration building blocks to edify high-performance, general purpose, and hardware accelerated embedded system softwares such as entertainment systems, gamewares, computer simulations and multimedia centers. 
It is essentially a set of software infrastructures whose middlewares, libraries, drivers and engines developed on the Qwadro relies on.

Despite its name, AFX is NOT an application framework, it is designed to be the bedrock for other Qwadro modules. 
AFX is designed to provide primitive host interoperability, such as I/O, I/O MMUs, file handling, storage management, memory management tools, threading and synchronization, IRP, classes, object management tools, device management, etc, to other Qwadro components. 
This is why every other infrastructure is an "extension" of AFX at first. 

An user application can be entirely built with the AFX to avoid interfacing the host, but, by design, AFX does not want to drive the user application. 
Inevitably the user would be required to set up its event loops with Qwadro methodology. The AFX will handle only events associated to its API objects. If the user had created a window out of MMUX infrastructure, the AFX will ignore it completely.

Qwadro is low-level, hardcore, and is highly experimental in this phase (this project is actually bootstrapping its own specification). 
Any and every user will be forced to be familiar with how computers work. 
It is by nature a middleware doing its way to provide a virtualized host platform. 
An user looking for ways of making games without a game engine (or game engines/frameworks written in C) should proceed to look for [Raylib](https://github.com/raysan5/raylib) or any similar project.

Qwadro started inside the RenderWare as part of a discontinued project of public, open game engine. 
At some point, the RenderWare platform has been hooked inside Qwadro. 
Later, it has been spun off to its own independent existence as it became more and more low-level, generalized and capable. 
This new, independent phase is called "Engineering Phase II". 
Because of this initial entanglement, certain names are kept familiar to RenderWare, and a new project of ammendment to the Qwadro, called ARX, is being developed to fulfill the total decouplement of RenderWare from Qwadro ecosystem.

## Advanced Video Graphics Extensions (AVX)
A modern, sanitized, redefined, orthogonal architectural Vulkan-flavored refactory of OpenGL Core.

Advanced Video Graphics Extensions (AVX) is an amendment to the **Acceleration Framework Experimental** (**AFX**) extending the Qwadro Execution Ecosystem with a complete, scalable, hardware-accelerated video graphics foundation.
AVX is designed by SIGMA Technology Group to redefine and stabilize the OpenGL Core API. Part of this has been previously known as SIGMA GL/2.

Instead of introducing an entirely new graphics API, AVX reorganizes, sanitizes, and future-proofs the existing GL model by providing:
- A unified low-level graphics foundation (Qwadro Video Graphics Infrastructure)
- A platform-agnostic runtime for device enumeration, presentation, and swap-chain management
- A consistent, deterministic context-handling layer via the AUX project
- A standardized, ICD-compatible driver interface model

AVX is not "another Vulkan" wrapper. Instead, it is what OpenGL Core 5 should have been; a modernized, strict, predictable environment that preserves GL's strengths while eliminating decades of historical design debt.

### Motivation

OpenGL Core has faced long-standing issues:
- Inconsistent or proprietary context creation
- Undefined behavior across vendors
- Extension fragmentation
- Lack of strict state management
- Weak separation between the driver runtime and low-level tasks

AVX addresses these problems by introducing **Qwadro Video Graphics Infrastructure**, a strict low-level interface that driver implementers target, while higher layers sit cleanly on top.

### Qwadro Video Graphics Infrastructure
Strongly based on **DXGI**, **Qwadro Video Graphics Infrastructure** is the foundation layer for AVX. 
This infrastructure provides:
- Unified device discovery & enumeration
- Vendor-agnostic surface, buffer, and memory interfaces
- Swap-chain creation and management
- Presentation scheduling
- Synchronization and fence primitives
- A standardized DDI (Device Driver Interface) for all low-level tasks

This layer is designed to be deterministic, cross-platform, and free from legacy GL ambiguities.

#### Qwadro Draw I/O System
The **Qwadro Draw I/O System** is a hybrid model of client-server and host-device cooperative design. 
It is a modernized, modular replacement for traditional GL "bootstrapping", responsible for:
- Device initialization
- Capability solicitation
- Rendering surface allocation
- State and pipeline preparation

Bridging the OpenGL Core-like front-end to the VGI DDI.

Instead of deeply mixing state handling with driver logic, AVX isolates the GL runtime from the low-level backend.

## Advanced Multimedia Synthesis Extensions (AMX)
A unified, distributed, hardware-accelerated multimedia processing effort.

The **AMX**, alphabetism and initialism for **Advanced Multimedia Synthesis Extensions**, is an amendment to the **Acceleration Framework Experimental** (**AFX**) extending the Qwadro Execution Ecosystem with a hardware-accelerated multimedia synthesis foundation.
AMX is designed by SIGMA Technology Group to drive the hardware media engines such as Intel Video Acceleration, DirectX Video Acceleration, Direct3D 12 Video and Vulkan Video, mainly motivated to find ways of feeding OpenGL with it.

The Multimedia Synthesis Infrastructure that has been developed to encompasses functionality for device discovery, solicitation and enumeration, as well as equipments and technologies that support the creation, processing, distribution, and playback of audio content generated by sound synthesis that are needed by device driver interfaces (DDI) and implementations to the Mix I/O System. 
It also defines a DDI that manages low-level shared tasks independently from the drawing mechanism runtime. 
The tasks implemented with MSI will be handled by the MSI DDI.

## Advanced Multimedia UX Extensions (AUX)
A portable, cross-platform windowing system intergration (WSI) inspired on Windows 7.

The **AUX**, alphabetism and initialism for **Advanced MMUX Extensions**, is an amendment to the **Acceleration Framework Experimental** (**AFX**) extending the Qwadro Execution Ecosystem with the **Qwadro User Experience Infrastructure** (**MMUX**).
**MMUX**, which is the foundation for the User I/O System, has been developed to enhance usability and the user experience in multimedia user interfaces (MMUIs) with shell environments. 
MMUX manages how windows are opened, closed, resized, and arranged on the screen, contributing to a smooth and organized workflow. 
It offers options for users to customize themes, layouts, and toolbars to suit their preferences, enhancing personal usability and facilitating user interactions, making complex tasks easier to perform through intuitive designs.

The historic nightmare of OpenGL Core context creation is resolved through AUX, a companion specification to AVX.

AUX provides:
- A uniform surface interface for all operating systems
- A shell into Qwadro (VGI's systems layer)
- Predictable and strict surface lifecycle rules
- Plug-and-play ICD compatibility

With AUX, developers no longer rely on WGL, GLX, CGL, EGL, or platform-specific hacks.

## Contribution

The **SIGMA FEDERATION** and all members shall not be held liable to any person or entity for any reason related to the adoption or implementation of, nor adherence to the recommendations in, nor any other use of this project nor any accompanying software.

The codebase was written for compilation in LLVM/Clang. No other toolset was tested except MSVC (and worked when so), which is not officially supported because it has never been a real C compiler. Qwadro also doesn't use CMake because the main developer had problems (and laziness) with Visual Studio 2017, the main IDE used to code the Qwadro since 2017/01/01.

Qwadro was firstly designed for POSIX/Unix/Linux systems. Due to complexities handling multiple OpenGL contexts in Windows, the working group was forced to move to Windows and stay there still now. Game-related works featured in Qwadro are strongely inspired in notorious engineering works on Bang! Engine by Ensemble Studios and MaxFX by Remedy Entertainment.

There are several strange names in AFX, AVX, AMX and AUX. It is because these projects are an experiment that implements several paradigms of the same concept with random names to test both in parallel, and its API changes frequently depending on usage optimization. And... it is governed by methodist laws of Go Horse.
There is some semantics inherited from RenderWare still present. For example, you will see avxRaster, afxQuat, afxV3d, afxV4d.

> E no Qwadro é assim: o que funciona está bugado, e o que não está bugado não funciona.

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated. For details, see the "[Contributing Guidelines][contribute-guide]".

You can also contribute or address to any doubt or difficult although by join the SIGMA's gathering point on Discord.

[![SIGMA Discord Server](https://discord.com/api/guilds/349379672351571969/widget.png?style=banner2)](https://sigmaco.org/discord)

The SIGMA FEDERATION thanks [all the contributors][contributors] by their individual and collective involvements in the development of this project.

[![all-contributors](https://contrib.rocks/image?repo=sigmaco/qwadro&columns=16)][contributors]

## License

This project is published under the [Qwadro License][license].

Qwadro is, and its portions are, (c) 2017 SIGMA FEDERATION. All rights reserved; to its elaborators and collaborators.

[license]: https://github.com/sigmaco/qwadro/blob/master/LICENSE.txt
[contributors]: https://github.com/sigmaco/afx/graphs/contributors
[contribute-guide]: https://github.com/sigmaco/afx/blob/master/docs/CONTRIBUTING.md

## Techdemo Media

![Image 1](doc/img/ScreenShot_20200401222851.png)
![Image 1](doc/img/ScreenShot_20200322030852.png)
![Image 1](doc/img/ScreenShot_20200314032422.png)
![Image 1](doc/img/ScreenShot_20200314032338.png)
![Image 1](doc/img/ScreenShot_20200314032134.png)
![Image 1](doc/img/ScreenShot_20200314032048.png)
![Image 1](doc/img/ScreenShot_20200228000223.png)
![Image 1](doc/img/ScreenShot_20191117124129.png)
![Image 1](doc/img/ScreenShot_20191014011409.png)
![Image 1](doc/img/ScreenShot_20191005214808.png)
![Image 1](doc/img/ScreenShot_20191003014004.png)
![Image 1](doc/img/ScreenShot_20191003013922.png)
![Image 1](doc/img/ScreenShot_20191003013841.png)
![Image 1](doc/img/ScreenShot_20191003013830.png)
![Image 1](doc/img/ScreenShot_20190912204603.png)
![Image 1](doc/img/ScreenShot_20190912204537.png)
![Image 1](doc/img/ScreenShot_20190912204452.png)
![Image 1](doc/img/ScreenShot_20190912204331.png)
![Image 1](doc/img/ScreenShot_20190912204311.png)
![Image 1](doc/img/unknown%20(32).png)
![Image 1](doc/img/unknown%20(31).png)
![Image 1](doc/img/unknown%20(29).png)
![Image 1](doc/img/unknown%20(28).png)
![Image 1](doc/img/unknown%20(27).png)
![Image 1](doc/img/unknown%20(26).png)
![Image 1](doc/img/unknown%20(25).png)
![Image 1](doc/img/unknown%20(24).png)
![Image 1](doc/img/unknown%20(23).png)
![Image 1](doc/img/unknown%20(22).png)
![Image 1](doc/img/unknown%20(21).png)
![Image 1](doc/img/unknown%20(20).png)
![Image 1](doc/img/unknown%20(19).png)
![Image 1](doc/img/unknown%20(18).png)
![Image 1](doc/img/unknown%20(17).png)
![Image 1](doc/img/unknown%20(16).png)
![Image 1](doc/img/unknown%20(15).png)
![Image 1](doc/img/unknown%20(14).png)
![Image 1](doc/img/unknown%20(13).png)
![Image 1](doc/img/unknown%20(12).png)
![Image 1](doc/img/unknown%20(11).png)
![Image 1](doc/img/unknown%20(10).png)
![Image 1](doc/img/unknown%20(9).png)
![Image 1](doc/img/unknown%20(8).png)
![Image 1](doc/img/unknown%20(7).png)
![Image 1](doc/img/unknown%20(6).png)
![Image 1](doc/img/unknown%20(5).png)
![Image 1](doc/img/unknown%20(4).png)
![Image 1](doc/img/unknown%20(3).png)
![Image 1](doc/img/unknown%20(2).png)
