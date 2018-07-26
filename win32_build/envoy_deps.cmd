setlocal 
REM @echo off
if "%1" == "" (
    echo Usage: %~nx0 rootfolder-for-installing-dependencies envoy-repo-root
    exit /b 1
)

if "%2" == "" (
    echo Usage: %~nx0 rootfolder-for-installing-dependencies envoy-repo-root
    exit /b 1
)

set ENVOY_REPO_ROOT=%2
set ENVOY_DEPENDENCY_ROOT=%1
if not exist %ENVOY_DEPENDENCY_ROOT% mkdir %ENVOY_DEPENDENCY_ROOT%
pushd %ENVOY_DEPENDENCY_ROOT%

set PROTOC=%ENVOY_DEPENDENCY_ROOT%\vcpkg\installed\x64-windows-static\tools\protobuf\protoc.exe
set PROTOC_FLAGS= --proto_path=%ENVOY_DEPENDENCY_ROOT%\vcpkg\installed\x64-windows-static\include
set PROTOC_OUTPUT=%ENVOY_DEPENDENCY_ROOT%\gens
set INCLUDE=%INCLUDE%;%ENVOY_DEPENDENCY_ROOT%\vcpkg\installed\x64-windows-static\include
if not exist %PROTOC_OUTPUT% mkdir %PROTOC_OUTPUT%
if not exist %PROTOC_OUTPUT%\lib mkdir  %PROTOC_OUTPUT%\lib
if not exist %PROTOC_OUTPUT%\lib\debug mkdir  %PROTOC_OUTPUT%\lib\debug

:vcpkg
REM vcpkg
if not exist vcpkg (
    git clone https://github.com/Microsoft/vcpkg.git
)
cd vcpkg
call .\bootstrap-vcpkg.bat
vcpkg install --triplet x64-windows-static abseil c-ares grpc http-parser libevent spdlog xxhash tclap rapidjson yaml-cpp
vcpkg install --triplet x64-windows nghttp2
vcpkg install --triplet x64-windows fmt

xcopy %ENVOY_DEPENDENCY_ROOT%\vcpkg\buildtrees\abseil\src\9a8c0641ad-f6c437fee1\absl\time\internal\cctz\include\* %ENVOY_DEPENDENCY_ROOT%\vcpkg\installed\x64-windows-static\include\absl\time\internal\cctz\include /i /y /s

@echo on

:boringssl
REM boringssl
cd %ENVOY_DEPENDENCY_ROOT%
if not exist boringssl (
    git clone https://boringssl.googlesource.com/boringssl
)
cd boringssl
git fetch
git apply %~dp0boringssl.patch
if not exist build_debug mkdir build_debug
if not exist build_release mkdir build_release
cd build_debug
cmake .. -G Ninja
cmake --build .
cd ..\build_release
cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build .

cd ..
if not exist %PROTOC_OUTPUT%\openssl mkdir %PROTOC_OUTPUT%\openssl
xcopy include\openssl %PROTOC_OUTPUT%\openssl /s /y
xcopy build_debug\ssl\ssl.lib %PROTOC_OUTPUT%\lib\debug /y
xcopy build_debug\crypto\crypto.lib %PROTOC_OUTPUT%\lib\debug /y
xcopy build_debug\decrepit\decrepit.lib %PROTOC_OUTPUT%\lib\debug /y
xcopy build_release\ssl\ssl.lib %PROTOC_OUTPUT%\lib /y
xcopy build_release\crypto\crypto.lib %PROTOC_OUTPUT%\lib /y
xcopy build_release\decrepit\decrepit.lib %PROTOC_OUTPUT%\lib /y

:circllhist
REM circllhist
cd %ENVOY_DEPENDENCY_ROOT%
set current_directory=circllhist
if not exist %current_directory% (
    git clone https://github.com/circonus-labs/libcircllhist.git %current_directory%
)
if not exist %ENVOY_DEPENDENCY_ROOT%\%current_directory%\debug mkdir %ENVOY_DEPENDENCY_ROOT%\%current_directory%\debug
if not exist %ENVOY_DEPENDENCY_ROOT%\%current_directory%\release mkdir %ENVOY_DEPENDENCY_ROOT%\%current_directory%\release
cd %current_directory%\src
git fetch
git checkout 050da53a44dede7bda136b93a9aeef47bd91fa12
git apply %~dp0circllhist.patch
cl /nologo /Z7 /c /DWIN32 /D_DEBUG circllhist.c /Fo:%ENVOY_DEPENDENCY_ROOT%\%current_directory%\debug\
lib /nologo /out:%ENVOY_DEPENDENCY_ROOT%\%current_directory%\debug\%current_directory%.lib %ENVOY_DEPENDENCY_ROOT%\%current_directory%\debug\circllhist.obj
cl /nologo /Z7 /c /DWIN32 /DNDEBUG /Osx circllhist.c /Fo:%ENVOY_DEPENDENCY_ROOT%\%current_directory%\release\
lib /nologo /out:%ENVOY_DEPENDENCY_ROOT%\%current_directory%\release\%current_directory%.lib %ENVOY_DEPENDENCY_ROOT%\%current_directory%\release\circllhist.obj
xcopy %ENVOY_DEPENDENCY_ROOT%\%current_directory%\debug\%current_directory%.lib %ENVOY_DEPENDENCY_ROOT%\vcpkg\installed\x64-windows-static\debug\lib /y
xcopy %ENVOY_DEPENDENCY_ROOT%\%current_directory%\release\%current_directory%.lib %ENVOY_DEPENDENCY_ROOT%\vcpkg\installed\x64-windows-static\lib /y
xcopy %ENVOY_DEPENDENCY_ROOT%\%current_directory%\src\circllhist.h %ENVOY_DEPENDENCY_ROOT%\vcpkg\installed\x64-windows-static\include /y

:prometheus
REM prometheus
cd %ENVOY_DEPENDENCY_ROOT%
set current_directory=prometheus
if not exist %PROTOC_OUTPUT%\%current_directory%\debug mkdir %PROTOC_OUTPUT%\%current_directory%\debug
if not exist %PROTOC_OUTPUT%\%current_directory%\release mkdir %PROTOC_OUTPUT%\%current_directory%\release
if not exist %current_directory% (
    git clone https://github.com/prometheus/client_model.git prometheus
)
cd %current_directory%
git fetch
git checkout 99fa1f4be8e564e8a6b613da7fa6f46c9edafc6c
%PROTOC% %PROTOC_FLAGS% --proto_path=. --cpp_out=%PROTOC_OUTPUT%\%current_directory% metrics.proto
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /D_DEBUG %PROTOC_OUTPUT%\%current_directory%\metrics.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\debug\
lib /nologo /out:%PROTOC_OUTPUT%\lib\debug\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\debug\metrics.pb.obj
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /DNDEBUG /Osx %PROTOC_OUTPUT%\%current_directory%\metrics.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\release\
lib /nologo /out:%PROTOC_OUTPUT%\lib\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\release\metrics.pb.obj
set PROMETHEUS_PROTO_PATH=%cd%

:opencensus
REM opencensus
cd %ENVOY_DEPENDENCY_ROOT%
set current_directory=opencensus-proto
set OPENCENSUS_GEN_PATH=%PROTOC_OUTPUT%\%current_directory%
if not exist %PROTOC_OUTPUT%\%current_directory%\debug mkdir %PROTOC_OUTPUT%\%current_directory%\debug
if not exist %PROTOC_OUTPUT%\%current_directory%\release mkdir %PROTOC_OUTPUT%\%current_directory%\release
if not exist %current_directory% (
    git clone https://github.com/census-instrumentation/opencensus-proto.git
)
cd %current_directory%
git fetch
git checkout ab82e5fdec8267dc2a726544b10af97675970847
cd opencensus\proto\trace
%PROTOC% %PROTOC_FLAGS% --proto_path=. --cpp_out=%PROTOC_OUTPUT%\%current_directory% trace.proto
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /D_DEBUG %PROTOC_OUTPUT%\%current_directory%\trace.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\debug\
lib /nologo /out:%PROTOC_OUTPUT%\lib\debug\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\debug\trace.pb.obj
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /DNDEBUG /Osx %PROTOC_OUTPUT%\%current_directory%\trace.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\release\
lib /nologo /out:%PROTOC_OUTPUT%\lib\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\release\trace.pb.obj
set OPENCENSUS_PROTO_PATH=%cd%

:protoc-gen-validate
REM protoc-gen-validate
cd %ENVOY_DEPENDENCY_ROOT%
set current_directory=protoc-gen-validate
if not exist %PROTOC_OUTPUT%\%current_directory%\debug mkdir %PROTOC_OUTPUT%\%current_directory%\debug
if not exist %PROTOC_OUTPUT%\%current_directory%\release mkdir %PROTOC_OUTPUT%\%current_directory%\release
if not exist %current_directory% (
    git clone https://github.com/lyft/protoc-gen-validate.git
)
cd %current_directory%
git fetch
git checkout f9d2b11e44149635b23a002693b76512b01ae515
%PROTOC% %PROTOC_FLAGS% --proto_path=. --cpp_out=%PROTOC_OUTPUT%\%current_directory% validate\validate.proto
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /D_DEBUG %PROTOC_OUTPUT%\%current_directory%\validate\validate.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\debug\
lib /nologo /out:%PROTOC_OUTPUT%\lib\debug\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\debug\validate.pb.obj
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /DNDEBUG /Osx %PROTOC_OUTPUT%\%current_directory%\validate\validate.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\release\
lib /nologo /out:%PROTOC_OUTPUT%\lib\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\release\validate.pb.obj
set GEN_VALIDATE_PROTO_PATH=%cd%

:gogoproto
REM gogoproto
cd %ENVOY_DEPENDENCY_ROOT%
set current_directory=gogoproto
if not exist %PROTOC_OUTPUT%\%current_directory%\debug mkdir %PROTOC_OUTPUT%\%current_directory%\debug
if not exist %PROTOC_OUTPUT%\%current_directory%\release mkdir %PROTOC_OUTPUT%\%current_directory%\release
if not exist %current_directory% (
    git clone https://github.com/gogo/protobuf.git %current_directory%
)
cd %current_directory%
git fetch
git checkout 1adfc126b41513cc696b209667c8656ea7aac67c
%PROTOC% %PROTOC_FLAGS% --proto_path=. --cpp_out=%PROTOC_OUTPUT%\%current_directory% gogoproto\gogo.proto
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /D_DEBUG %PROTOC_OUTPUT%\%current_directory%\gogoproto\gogo.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\debug\
lib /nologo /out:%PROTOC_OUTPUT%\lib\debug\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\debug\gogo.pb.obj
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /DNDEBUG /Osx %PROTOC_OUTPUT%\%current_directory%\gogoproto\gogo.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\release\
lib /nologo /out:%PROTOC_OUTPUT%\lib\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\release\gogo.pb.obj
set GOGO_PROTO_PATH=%cd%

:googleapis
REM googleapis
cd %ENVOY_DEPENDENCY_ROOT%
set current_directory=googleapis
if not exist %PROTOC_OUTPUT%\%current_directory%\debug mkdir %PROTOC_OUTPUT%\%current_directory%\debug
if not exist %PROTOC_OUTPUT%\%current_directory%\release mkdir %PROTOC_OUTPUT%\%current_directory%\release
if not exist %current_directory% (
    git clone https://github.com/googleapis/googleapis.git
)
cd %current_directory%
git checkout d642131a6e6582fc226caf9893cb7fe7885b3411
for /R %%i in (google\api\*.proto) do (
    %PROTOC% %PROTOC_FLAGS% --proto_path=%cd% --cpp_out=%PROTOC_OUTPUT%\%current_directory% "%%i"
)
for /R %%i in (google\rpc\*.proto) do (
    %PROTOC% %PROTOC_FLAGS% --proto_path=%cd% --cpp_out=%PROTOC_OUTPUT%\%current_directory% "%%i"
)
cl /nologo /Z7 /c /EHsc /Z7 /I %PROTOC_OUTPUT%\%current_directory% /D_DEBUG %PROTOC_OUTPUT%\%current_directory%\google\api\annotations.pb.cc %PROTOC_OUTPUT%\%current_directory%\google\api\http.pb.cc %PROTOC_OUTPUT%\%current_directory%\google\rpc\status.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\debug\
lib /nologo /out:%PROTOC_OUTPUT%\lib\debug\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\debug\http.pb.obj %PROTOC_OUTPUT%\%current_directory%\debug\annotations.pb.obj %PROTOC_OUTPUT%\%current_directory%\debug\status.pb.obj
cl /nologo /Z7 /c /EHsc /Z7 /I %PROTOC_OUTPUT%\%current_directory% /DNDEBUG /Osx %PROTOC_OUTPUT%\%current_directory%\google\api\annotations.pb.cc %PROTOC_OUTPUT%\%current_directory%\google\api\http.pb.cc %PROTOC_OUTPUT%\%current_directory%\google\rpc\status.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\release\
lib /nologo /out:%PROTOC_OUTPUT%\lib\%current_directory%.lib %PROTOC_OUTPUT%\%current_directory%\release\http.pb.obj %PROTOC_OUTPUT%\%current_directory%\release\annotations.pb.obj %PROTOC_OUTPUT%\%current_directory%\release\status.pb.obj
set GOOGLE_APIS_PROTO=%cd%


:data-plane-api
REM data-plane-api
cd %ENVOY_DEPENDENCY_ROOT%
set current_directory=data-plane-api
if not exist %PROTOC_OUTPUT%\%current_directory%\debug mkdir %PROTOC_OUTPUT%\%current_directory%\debug
if not exist %PROTOC_OUTPUT%\%current_directory%\release mkdir %PROTOC_OUTPUT%\%current_directory%\release
if not exist %current_directory% (
    git clone https://github.com/envoyproxy/data-plane-api.git
)
cd %ENVOY_REPO_ROOT%\api
for /R %%i in (*.proto) do (
    %PROTOC% %PROTOC_FLAGS% ^
        --proto_path=%cd% ^
        --proto_path=%GEN_VALIDATE_PROTO_PATH% ^
        --proto_path=%GOGO_PROTO_PATH% ^
        --proto_path=%GOOGLE_APIS_PROTO% ^
        --proto_path=%PROMETHEUS_PROTO_PATH% ^
        --proto_path=%OPENCENSUS_PROTO_PATH% ^
        --cpp_out=%PROTOC_OUTPUT%\%current_directory% "%%i"
)
cd %PROTOC_OUTPUT%\%current_directory%\
del trace_service.pb.cc /s
for /R %%i in (*.cc) do (
    cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /D_DEBUG /Fo:%%~dpi /I %PROTOC_OUTPUT%\%current_directory% ^
        /I %PROTOC_OUTPUT%\prometheus ^
        /I %PROTOC_OUTPUT%\opencensus-proto ^
        /I %PROTOC_OUTPUT%\protoc-gen-validate ^
        /I %PROTOC_OUTPUT%\gogoproto ^
        /I %PROTOC_OUTPUT%\googleapis ^
        "%%i"
)
dir *.obj /s /b > lib_debug.rsp
lib /nologo /out:%PROTOC_OUTPUT%\lib\debug\%current_directory%.lib @lib_debug.rsp

del /s /q *.obj
for /R %%i in (*.cc) do (
    cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\%current_directory% /DNDEBUG /Fo:%%~dpi /I %PROTOC_OUTPUT%\%current_directory% ^
        /I %PROTOC_OUTPUT%\prometheus ^
        /I %PROTOC_OUTPUT%\opencensus-proto ^
        /I %PROTOC_OUTPUT%\protoc-gen-validate ^
        /I %PROTOC_OUTPUT%\gogoproto ^
        /I %PROTOC_OUTPUT%\googleapis ^
        "%%i"
)
dir *.obj /s /b > lib_release.rsp
lib /nologo /out:%PROTOC_OUTPUT%\lib\%current_directory%.lib @lib_release.rsp

:health
REM health
cd %ENVOY_DEPENDENCY_ROOT%
set current_directory=health\src\proto\grpc\health\v1
if not exist %PROTOC_OUTPUT%\%current_directory%\debug mkdir %PROTOC_OUTPUT%\%current_directory%\debug
if not exist %PROTOC_OUTPUT%\%current_directory%\release mkdir %PROTOC_OUTPUT%\%current_directory%\release
%PROTOC% %PROTOC_FLAGS% ^
    --proto_path=%ENVOY_DEPENDENCY_ROOT%\vcpkg\buildtrees\grpc\src\v1.12.0-73cc223062 ^
    %ENVOY_DEPENDENCY_ROOT%\vcpkg\buildtrees\grpc\src\v1.12.0-73cc223062\src\proto\grpc\health\v1\health.proto ^
    --cpp_out=%PROTOC_OUTPUT%\health
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\health /D_DEBUG %PROTOC_OUTPUT%\%current_directory%\health.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\debug\ /I %PROTOC_OUTPUT%\%current_directory%
lib /nologo /out:%PROTOC_OUTPUT%\lib\debug\health.lib %PROTOC_OUTPUT%\%current_directory%\debug\health.pb.obj
cl /nologo /Z7 /c /EHsc /I %PROTOC_OUTPUT%\health /DNDEBUG /Osx %PROTOC_OUTPUT%\%current_directory%\health.pb.cc /Fo:%PROTOC_OUTPUT%\%current_directory%\release\ /I %PROTOC_OUTPUT%\%current_directory%
lib /nologo /out:%PROTOC_OUTPUT%\lib\health.lib %PROTOC_OUTPUT%\%current_directory%\release\health.pb.obj

endlocal