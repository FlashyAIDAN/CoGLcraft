if not exist "build" mkdir build
pushd build
if not exist "debug" mkdir debug
pushd debug
if not exist "res" mkdir res
echo Xcopy "../../res" "./res" /v /q /e /k /o /s /r /h /x /y
robocopy "../../res" "./res" *.* /s /e