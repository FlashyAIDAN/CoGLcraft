if not exist "build" mkdir build
pushd build
if not exist "release" mkdir release
pushd release
if not exist "res" mkdir res
echo Xcopy "../../res" "./res" /v /q /e /k /o /s /r /h /x /y
robocopy "../../res" "./res" *.* /s /e
windres ../../res/icons/first.rc -O coff -o ../../res/icons/first.res