#!/ bin / bash

mkdir - p build;
cd build if[["$OSTYPE" == "msys"]] then cmake - G "Visual Studio 15 2017 Win64"..- Wno
        - dev &&cmake-- build.--config Release else cmake
        - DCMAKE_BUILD_TYPE = $1..&& cmake-- build.---j 4 fi
