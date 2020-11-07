#!/bin/bash

# Copyright (C) 2017 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Linux Build Script run inside docker container.

set -e # Fail on any error.

. /bin/using.sh # Declare the bash `using` function for configuring toolchains.

set -x # Display commands being run.

SKIP_TESTS="False"
BUILD_TYPE="Debug"

using cmake-3.17.2
using ninja-1.10.0

if [ ! -z "$COMPILER" ]; then
    using "$COMPILER"
fi

# Possible configurations are:
# ASAN, COVERAGE, RELEASE, DEBUG, DEBUG_EXCEPTION, RELEASE_MINGW

if [ $CONFIG = "RELEASE" ] || [ $CONFIG = "RELEASE_MINGW" ]
then
  BUILD_TYPE="RelWithDebInfo"
fi

ADDITIONAL_CMAKE_FLAGS=""
if [ $CONFIG = "ASAN" ]
then
  ADDITIONAL_CMAKE_FLAGS="-DCMAKE_CXX_FLAGS=-fsanitize=address -DCMAKE_C_FLAGS=-fsanitize=address"
elif [ $CONFIG = "COVERAGE" ]
then
  ADDITIONAL_CMAKE_FLAGS="-DENABLE_CODE_COVERAGE=ON"
  SKIP_TESTS="True"
elif [ $CONFIG = "DEBUG_EXCEPTION" ]
then
  ADDITIONAL_CMAKE_FLAGS="-DDISABLE_EXCEPTIONS=ON -DDISABLE_RTTI=ON"
elif [ $CONFIG = "RELEASE_MINGW" ]
then
  ADDITIONAL_CMAKE_FLAGS="-Dgtest_disable_pthreads=ON -DCMAKE_TOOLCHAIN_FILE=$ROOT_DIR/cmake/linux-mingw-toolchain.cmake"
  SKIP_TESTS="True"
fi

cd $ROOT_DIR
./utils/git-sync-deps

mkdir build
cd $ROOT_DIR/build

# Invoke the build.
BUILD_SHA=${KOKORO_GITHUB_COMMIT:-$KOKORO_GITHUB_PULL_REQUEST_COMMIT}
echo $(date): Starting build...
cmake -GNinja -DCMAKE_INSTALL_PREFIX=$KOKORO_ARTIFACTS_DIR/install -DSHADERC_ENABLE_SPVC=ON -DRE2_BUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=$BUILD_TYPE $ADDITIONAL_CMAKE_FLAGS ..

echo $(date): Build glslang...
ninja glslangValidator

echo $(date): Build everything...
ninja
echo $(date): Build completed.

echo $(date): Check Shaderc for copyright notices...
ninja check-copyright

if [ $CONFIG = "COVERAGE" ]
then
  echo $(date): Check coverage...
  ninja report-coverage
  echo $(date): Check coverage completed.
fi

echo $(date): Starting ctest...
if [ $SKIP_TESTS = "False" ]
then
  ctest --output-on-failure -j4
fi
echo $(date): ctest completed.

# Package the build.
ninja install
cd $KOKORO_ARTIFACTS_DIR
tar czf install.tgz install
