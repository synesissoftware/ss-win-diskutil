@ECHO OFF

SETLOCAL ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION
IF ERRORLEVEL 1 (

	ECHO %~nx0: Requires Command extensions and delayed-expansion

	EXIT /B 1
)

IF {%7} == {} (

	GOTO :usage
)

SET ScriptDir=%~dp0
SET SolutionDir=%ScriptDir%..\..\
SET BinDir=%ScriptDir%..\..\bin\
SET ProjectsSupportDir=%SolutionDir%projects\support\

REM ECHO SolutionDir="%SolutionDir%"
REM ECHO ProjectsSupportDir="%ProjectsSupportDir%"

SET ClcpExePath=%BinDir%clcp.exe

SET ClcpSourcePath=%~1
SET TargetPath=%~2
SET TargetLibDir=%~3
SET VersionFile=%~4
SET VersionMajorSymbol=%~5
SET Platform=%~6
SET Configuration=%~7
SET ProjectName=%~8
SET ProjectSubname=%~9

REM ECHO.
REM ECHO ClcpSourcePath="%ClcpSourcePath%"
REM 
REM ECHO.
REM ECHO TargetPath="%TargetPath%"
REM ECHO TargetLibDir="%TargetLibDir%"
REM ECHO VersionFile="%VersionFile%"
REM ECHO VersionMajorSymbol="%VersionMajorSymbol%"
REM ECHO Platform="%Platform%"
REM ECHO Configuration="%Configuration%"
REM ECHO ProjectName="%ProjectName%"
REM ECHO ProjectSubname="%ProjectSubname%"



IF EXIST "%ClcpExePath%" (

	REM ECHO Deleting ^"%ClcpExePath%^"
	DEL /F "%ClcpExePath%"
)

REM ECHO.
REM ECHO cl.exe -nologo -DNDEBUG -W4 "%ClcpSourcePath%" -Fe"%ClcpExePath%"

cl.exe -nologo -DNDEBUG -W4 "%ClcpSourcePath%" -Fe"%ClcpExePath%" >NUL
IF ERRORLEVEL 1 (

	ECHO %~nx0: Failed to compile create-library-copy-program

	EXIT /B 1
)

ECHO Executing "%ClcpExePath%" to generate canonical form library in "%TargetLibDir%"
"%ClcpExePath%" "%TargetPath%" "%TargetLibDir%" "%VersionFile%" "%VersionMajorSymbol%" "%Platform%" "%Configuration%" "%ProjectName%" "%ProjectSubname%"



GOTO :eof
:usage

ECHO USAGE: %~n0(.%~x0) ^<clcp-src-path^> ^<target-path^> ^<target-lib-dir^> ^<version-file^> ^<ver-major-symbol^> ^<platform^> ^<configuration^> ^<project-name^> [ ^<project-subname^> ]

:eof

