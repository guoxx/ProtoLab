SET SolutionDir=%1
SET ProjectDir=%2
SET OutDir=%3

@ECHO ON
ECHO "prebuild start"

ECHO "COPY render doc dll to "%OutDir%
COPY %SolutionDir%"ThirdParty\RenderDoc\x64\*" %OutDir%
IF NOT EXIST %OutDir%"renderdoc" MKDIR %OutDir%"renderdoc"

:: ECHO "COPY shaders to "%OutDir%
:: COPY %ProjectDir%"Shaders\*" %OutDir%

ECHO "COPY resources to "%OutDir%
COPY %SolutionDir%"Resources\*" %OutDir%

ECHO "prebuild end"