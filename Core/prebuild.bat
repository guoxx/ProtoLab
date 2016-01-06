SET SolutionDir=%1
SET ProjectDir=%2
SET OutDir=%3
Set FrameworkSdkDir=%4

@ECHO ON
ECHO "prebuild start"

ECHO "Copy render doc dll to "%OutDir%
COPY %SolutionDir%"ThirdParty\RenderDoc\x64\*" %OutDir%
IF NOT EXIST %OutDir%"renderdoc" MKDIR %OutDir%"renderdoc"

:: ECHO "COPY shaders to "%OutDir%
:: COPY %ProjectDir%"Shaders\*" %OutDir%

ECHO "Copy resources to "%OutDir%
COPY %SolutionDir%"Resources\*" %OutDir%

ECHO "Copy HLSL compiler library to "%OutDir%
COPY %FrameworkSdkDir%bin\x64\d3dcompiler_47.dll %OutDir%

ECHO "prebuild end"