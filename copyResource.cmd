@echo off
:: ����1ΪԴ�ļ��У�����2Ϊbuild���ļ���
SET SDIR=%1
SET BDIR=%2
:: ת��·�����ӷ�
SET SDIR=%SDIR:/=\%
SET BDIR=%BDIR:/=\%
IF %SDIR%=="" (
	GOTO end
)
:: ����������Դ���ļ���
IF NOT EXIST %BDIR% (
	MKDIR %BDIR%
	ECHO Create directory %BDIR%
)
:: ������Դ�ļ�
FOR /R %SDIR% %%i IN (*) DO (
	COPY %%i %BDIR% >NUL
	ECHO Create source file %BDIR%\%%~NXi 
) 
:end