# SIGMA GL/2 Draw I/O System
Originally it was a proof of concept for ***OpenGL 5*** emerged from some discussions, then it became somehow a proof of concept for ***Vulkan-On-OpenGL***. After it all, the ***SIGMA GL/2*** has been partitioned into other projects and this part became a reference implementation of a device driver for the ***Qwadro Draw I/O System***, core component of the ***Qwadro Video Graphics Infrastructure***, part of the ***Advanced Video Graphics Extensions & Experiments***, both developed by SIGMA Technology Group. It is based on Advanced OpenGL to deliver hardware acceleration for Qwadro in major PC platforms.

Despite not being a real ***Vulkan-On-OpenGL***, you can find how certain features, behaviors and components have been paired between these two APIs. In the middle of 2019, there was a real combined ICD named ***OpenGL/Vulkan Continuous Integration***, which placed both side to side in code, but the codebase became too complex (not only because the boilerplate of Vulkan, but OpenGL's hell of extensions too), and there was no a permanent public able or wanting to participate of required tests.

This implementation requires OpenGL 3.2 by making use of argumentation in context creation. All other features used are expected to be available as extensions up to OpenGL 4.6 as it was published yet in 2017. The Qwadro project started in 2017, and should be fully functional with integrated GPUs up to 4 years before. This perspective has been originated of a regional market share evaluation. Qwadro Draw I/O System is first designed to operate with unified memory addressing. This model reflects the vision of future by SIGMA.

This implementation CAN use hardware acceleration resources not officially enabled and/or allowed by the IHV.

## Contribution

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated. For details, see the "[Contributing Guidelines][contribute-guide]".

You can also contribute or address to any doubt or difficult although by join the SIGMA FEDERATION's gathering point on Discord.

[![SIGMA Discord Server](https://discord.com/api/guilds/349379672351571969/widget.png?style=banner2)](https://sigmaco.org/discord)

The SIGMA FEDERATION thanks [all the contributors][contributors] by their individual and collective involvements in the development of this project.

[![all-contributors](https://contrib.rocks/image?repo=sigmaco/tarzgl4&columns=16)][contributors]

## License

This project is published under [Qwadro License][license].

Qwadro is, and its portions are, (c) 2017 SIGMA FEDERATION. All rights reserved; to its elaborators and collaborators.

[license]: https://github.com/sigmaco/afx/blob/master/LICENSE.txt
[contributors]: https://github.com/sigmaco/tarzgl4/graphs/contributors
[contribute-guide]: https://github.com/sigmaco/qwadro/blob/master/docs/CONTRIBUTING.md
