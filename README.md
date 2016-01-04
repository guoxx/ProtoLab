# ProtoLab

A DX11/DX12 renderer for prototype.

## How to setup the projects

Currently ProtoCore.vcxproj is the main project, it should be just a core library and referenced by other projects, but for the moment, let's leave it like this. 

Before you launch this project, here are few steps that you need to do.

1. Set working directory as $(SolutionDir)Debug

## Target features

* Physical based rendering
* Dynamic global illumination
* Deferred shading

## Conventions

* Right hand coordinate system
* Column major matrix
* Matrix multiplication : post-multiply  (or right multiply), same rule applied on c++ and HLSL 

## Known issues

* RenderDoc plugin doesn't work with deferred context