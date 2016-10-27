@echo off 

set CONFIGURATION=%~1
set SOLUTION_DIR=%~2
set PROJECT_NAME=%~3

echo. 
echo ***************************************************************************
echo *   Copying files and folders defined in build.txt (%CONFIGURATION%)    
echo ***************************************************************************
echo. 

setlocal enabledelayedexpansion
for /f "tokens=1,2,3 usebackq delims=; eol=#" %%a in ("%cd%\build.txt") do (
		for /f "tokens=* delims= " %%t in ('echo %%a') do set src=%%t
		for /f "tokens=* delims= " %%t in ('echo %%b') do set trg=%%t
		for /f "tokens=* delims= " %%t in ('echo %%c') do set config=%%t

		set doCopy=0
		if "allconfigs"=="!config!" set doCopy=1
		if "%CONFIGURATION%"=="!config!" set doCopy=1
				
		if !doCopy!==1 (
			set src=!src:_KEYWORD_CONFIG_NAME_=%CONFIGURATION%!
			set trg=!trg:_KEYWORD_CONFIG_NAME_=%CONFIGURATION%!
			
			set src=!src:_KEYWORD_PROJECT_NAME_=%PROJECT_NAME%!
			set trg=!trg:_KEYWORD_PROJECT_NAME_=%PROJECT_NAME%!
			
			xcopy /Y /D /F /S "%SOLUTION_DIR%\..\!src!" "%SOLUTION_DIR%\..\!trg!\"		
		)
)
setlocal disabledelayedexpansion