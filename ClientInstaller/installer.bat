@echo off
>nul 2>&1 net session
if %errorlevel% neq 0 (
    echo Please run the script as an administrator.
    pause
    exit /b 1
)

set "destinationFolder=C:\ProgramData\NetMaster"

mkdir "%destinationFolder%" 2>nul
if errorlevel 1 (
    echo Error: Failed to create directory %destinationFolder%.
    pause
    exit /b 1
)

set "sourceFile=%~dp0config.ini"
set "ServicesourceFile=%~dp0NetMaster_Client.exe"

if not exist "%sourceFile%" (
    echo Error: config.ini not found. Place the file in the same directory as the batch file.
    pause
    exit /b 1
)

if not exist "%ServicesourceFile%" (
    echo Error: NetMaster_Client.exe not found. Place the file in the same directory as the batch file.
    pause
    exit /b 1
)

copy /y "%sourceFile%" "%destinationFolder%\config.ini" > nul
if errorlevel 1 (
    echo Error: Failed to copy config.ini to %destinationFolder%.
    pause
    exit /b 1
)

copy /y "%ServicesourceFile%" "%destinationFolder%\NetMaster_Client.exe" > nul
if errorlevel 1 (
    echo Error: Failed to copy NetMaster_Client.exe to %destinationFolder%.
    pause
    exit /b 1
)

echo Files copied successfully.

echo Creating and starting service
sc create netmaster binpath= "\"%destinationFolder%\NetMaster_Client.exe\"" start= auto
if errorlevel 1 (
    echo Error: Failed to create the service.
    pause
    exit /b 1
)

echo Service created successfully.

echo Checking service status
sc start netmaster
sc query netmaster | find "STATE" | find "RUNNING" > nul
if errorlevel 1 (
    echo Error: Service is not running.
    pause
    exit /b 1
)

echo Service is running.
pause
exit /b 0
