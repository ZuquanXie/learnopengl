@echo off
:: 参数1为源文件夹，参数2为build后文件夹
SET SDIR=%1
SET BDIR=%2
:: 转换路径连接符
SET SDIR=%SDIR:/=\%
SET BDIR=%BDIR:/=\%
IF %SDIR%=="" (
	GOTO end
)
:: 创建放置资源的文件夹
IF NOT EXIST %BDIR% (
	MKDIR %BDIR%
	ECHO Create directory %BDIR%
)
:: 拷贝资源文件
FOR /R %SDIR% %%i IN (*) DO (
	COPY %%i %BDIR% >NUL
	ECHO Create source file %BDIR%\%%~NXi 
) 
:end