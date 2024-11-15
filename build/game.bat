@echo off
setlocal enabledelayedexpansion

set "folder=coding"

set count=0

cls
echo Danh sach cac file:
for %%f in (%folder%\*.cpp %folder%\*.py) do (
    set /a count+=1
    echo - %%~nxf
)

if %count%==0 (
    echo Khong tim thay file .cpp hoac .py trong thu muc %folder%.
    exit /b
)

set /p choice="Nhap ten file: "

set "selected_file="
for %%f in (%folder%\*.cpp %folder%\*.py) do (
    if /i "%%~nxf"=="%choice%" (
        set "selected_file=%%~f"
    )
)

if not defined selected_file (
    echo Khong tim thay file nao phu hop.
    exit /b
)

cls
if /i "%selected_file:~-4%"==".cpp" (
    :menu
    echo Chon yeu cau:
    echo 1 - Compile
    echo 2 - Run .exe
    echo 3 - Compile va Run
    echo 0 - End
    set /p compile_choice=">> "

    if "%compile_choice%"=="1" (
        call :compile "%selected_file%"
        pause
        goto menu
    ) else if "%compile_choice%"=="2" (
        call :run
        pause
        goto menu
    ) else if "%compile_choice%"=="3" (
        call :compile "%selected_file%"
        call :run
        pause
        goto menu
    ) else if "%compile_choice%"=="0" (
        goto end
    ) else (
        echo Lua chon khong hop le.
        goto menu
    )
) else (
    call :run_python "%selected_file%"
)

goto end

:compile
    for %%f in ("%choice%") do set "file_name=%%~nf"
    echo The program is being compiled from: %file_name%
    mingw32-make -f build/compile.mk FILE_NAME=%file_name%
    if %ERRORLEVEL% neq 0 (
        echo Error occurred during compilation.
    ) else (
        echo Congratulations! You have successfully built the product!
    )
    goto :eof

:run
    for %%f in ("%choice%") do set "file_name=%%~nf"
    echo The program is running...
    set "exe_file=environment/%file_name%.exe"
    "%exe_file%"
    echo The program has ended...
    goto :eof

:run_python
    echo The Python program is running...
    python "%~1"
    echo The Python program has ended...
    pause
    goto :eof

:end
endlocal
