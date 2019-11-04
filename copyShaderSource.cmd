@echo off
echo %1
SET BUILDDIR="%1\out\build\x64-Debug\shaderSource"
IF NOT EXIST %BUILDDIR% MKDIR %BUILDDIR%
FOR /R "%1\shaderSource" %%i IN (*s) DO (
	COPY %%i %BUILDDIR%
) 