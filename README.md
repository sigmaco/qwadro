```

          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
          ###### ###  ###   ###   ###     ### #########  ###    ###  ########

                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M

                                   Public Test Build
                               (c) 2017 SIGMA FEDERATION
                             <https://sigmaco.org/qwadro/>

```

# The Qwadro Execution Ecosystem
The **Qwadro** is an ISO C17 open standard, low-level data-oriented, federated execution ecosystem that offers a set of acceleration building blocks to edify system softwares targeting firstly computer simulation, interactive entertainment and multimedia user experience. Qwadro constitutes a unified infrastructure of and for middlewares, libraries, drivers and engines, continuously integrated, providing resources, devices, services and abstracting operating systems and platforms in a straightforwared uniform way. It is released as open source under the Qwadro License.

## Introduction

This project represents the work of a group of software engineers and application developers meeting as the **Qwadro Working Group** (**4DWG**) of the **SIGMA Technology Group**, a arm of the **SIGMA FEDERATION**, the federation of independent cyber science, technology and engineering societies from Latin America. The goal of this group is to influence, foster, viabilize and improve the genuine development of multimedia user experience applications, hardware-accelerated high-performance optimal softwares and optimal entertainment softwares in the areas of 2D, 3D and 4D simulations and its renderization for the Latin American audience.
This project defines the expectations of the group with regards to current acceptable 4D simulation and rendering functionality, and is an extension to the work done by Veryzon known as **Quadridimensional World Approximation**.

The API concept and specification proposed in this project were primarily contributed by Veryzon. They were drafted in large part by Veryzon and owe much in their form and format to Veryzon's essays around experimental 4D technologies. The Working Group process provided advice, comments, guidance and suggestions for additions to these guidelines, which were contributed by software developers with battle-tested experiences working with RenderWare, OpenGL and Vulkan graphics hardware and software vendors and application developers and enthusiasts, indie game developers and enthusiasts, game modding communities and enthusiasts, come from backgrounds like Republica SIGMA, SIGMA Technology Group, SIGMA FEDERATION, Scitech, Remedy Entertainment, Engitech, NVIDIA, Crytek, id Software, TVE, Bellas Arts, Criterion Software, EA Technology, Ensemble Studios, Telekinesys and Intel. Just to quote some of them.

The **SIGMA FEDERATION** and all members shall not be held liable to any person or entity for any reason related to the adoption or implementation of, nor adherence to the recommendations in, nor any other use of this project nor any accompanying software.

The codebase prefix in early days was ***QW*** but actually, to reduce confusion, it is ***AFX***, which was inherited of Qwadro Acceleration Frameworks, the standard Qwadro implementation. There are other prefixes for other partitions (***AVX***, ***AEX***, ***AAX***, ***AMX***, ***AUX***, ***ATX***, etc). The SIGMA reserves for itself and its experiments QW and every possible prefix A\*X for future components and expansions of the Qwadro.

## Qwadro Draw I/O System
The **Draw I/O System** (**DIOS**) is a data-oriented computer-generated imagery API specification designed to make common things (relatively) easy, while allowing optional super powerful low-level optimizations. This new API was firstly idealized to be easy as OpenGL but still trying to be powerful close to Vulkan, Direct3D e Metal. Unlike other low-level APIs, Draw I/O System follows a rapid application development (RAD) philosophy, where it delivers utilities to speed up and automate operations, still allowing the users to do it their own way if they want.

Draw I/O System is composed of two parts. One of them is the Qwadro Video Graphics Infrastructure, which is part of the Qwadro Execution Ecosystem offering a unified and prompt way of the common API components, and another, which is part of the device driver interface offering the horsepowers and additional capabilities althrough extensions.

## Qwadro Sound I/O System
The **Qwadro Sound I/O System** (**SIOS**) is a data-oriented sound synthesis API specification designed to enables creation, processing, distribution, and playback of audio content generated by sound synthesis in Qwadro.

## Qwadro Multimedia UX System
The **Qwadro Multimedia UX System** (**MMUX**) is a data-driven multimedia UX API specification desigend to create an intuitive, efficient, and enjoyable user experience in GUI shell environments, enabling users to perform tasks easily and effectively.

## Qwadro Simulation System
The **Simulation System** is a data-oriented high-performance computing API specification designed to build and run experiments, approximations and simulations where time-space (see, time over space) is manipulated in unconventional ways, making it easier to model complex systems, test hypotheses, and make data-driven decisions. Here is where the action happens, hence also where the project name originated from; the ***tetra/quadridimensional world approximations***.

In Qwadro, a matrix is neither row-major nor column-major. Their matrices are hybrids. The upper 3x3 submatrix (where linear operations occur) is column-major, while the translation set is row-major. Yes, that madness there. Different from what you will expect in the war of row-major (Direct3D) vs. column-major (OpenGL). Qwadro is the definitive supremacy. This is a complicated approach because it mixes both OpenGL and Direct3D concepts, functions, and operations at the same time. Writing AMX was hell because of this, and not all functions have been tested yet.

Some parts of the asset management for a Qwadro simulation is inspired in Bang! Engine (Age Of Empires III engineering) and MaxFX 2.0 (Max Payne 2 engineering).

A Qwadro Simulation also works on the "bring your own driver" model. Driver? Yes. You can install a custom device driver to run your simulation. 
It allow you to change compute and physics engines and sample animations in GPU pipelines with OpenGL, Vulkan, OpenCL, CUDA, DSPs, SPUs or other coprocessors. This is why we don't have SIMD methods for math (at least yet); you can install a hardware-accelerated physics engine.

## Contribution

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated. For details, see the "[Contributing Guidelines][contribute-guide]".

You can also contribute or address to any doubt or difficult although by join the SIGMA FEDERATION's gathering point on Discord.

[![SIGMA Discord Server](https://discord.com/api/guilds/349379672351571969/widget.png?style=banner2)](https://sigmaco.org/discord)

The SIGMA FEDERATION thanks [all the contributors][contributors] by their individual and collective involvements in the development of this project.

[![all-contributors](https://contrib.rocks/image?repo=sigmaco/qwadro&columns=16)][contributors]

## License

This project is published under the [Qwadro License][license].

Qwadro is, and its portions are, (c) 2017 SIGMA FEDERATION. All rights reserved; to its elaborators and collaborators.

[license]: https://github.com/sigmaco/qwadro/blob/master/LICENSE.txt
[contributors]: https://github.com/sigmaco/afx/graphs/contributors
[contribute-guide]: https://github.com/sigmaco/afx/blob/master/docs/CONTRIBUTING.md
