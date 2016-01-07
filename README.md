# ProtoLab

A DX11(DX12 will be supported later) renderer for prototype.

## How to setup the projects

Currently ProtoCore.vcxproj is the main project, it should be just a core library and referenced by other projects, but for the moment, let's leave it like this. 

Before you launch this project, here are few steps that you need to do.

1. Set working directory as $(OutDir)

## Target features

* Physical based rendering
* Dynamic global illumination
* Deferred shading

## Conventions

* Right hand coordinate system
* Column major matrix
* Matrix multiplication : post-multiply  (or right multiply), same rule applied on c++ and HLSL 

## Shader debugging

I had integrate renderdoc 0.2.6 for shader debugging. Launch with parameter ``-renderdoc`` or ``/renderdoc``, then renderdoc.dll will be loaded. Use key ``c`` to have a capture. Captured file will be saved in ``Your working directory/renderdoc``.

## Known issues

* Deferred context not work well with renderdoc, can't display mesh correctly in mesh view due to the missing information of index buffer.