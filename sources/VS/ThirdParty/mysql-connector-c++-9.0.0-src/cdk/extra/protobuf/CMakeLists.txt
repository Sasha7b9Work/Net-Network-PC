# Copyright (c) 2015, 2024, Oracle and/or its affiliates.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License, version 2.0,
# as published by the Free Software Foundation.
#
# This program is designed to work with certain software (including
# but not limited to OpenSSL) that is licensed under separate terms, as
# designated in a particular file or component or in included license
# documentation. The authors of MySQL hereby grant you an additional
# permission to link the program and your derivative works with the
# separately licensed software that they have either included with
# the program or referenced in the documentation.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
# the GNU General Public License, version 2.0, for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

# The files in protobuf-3.19.6/ are almost unmodified versions of google
# source files taken from protobuf-cpp-3.11.4.tar.gz archive.
# Changes:
# - remove directories and files
#    protobuf-3.19.6/benchmarks/
#    protobuf-3.19.6/conformance/
#    protobuf-3.19.6/editors/
#    protobuf-3.19.6/examples/
#    protobuf-3.19.6/m4/
#    protobuf-3.19.6/objectivec/
#    protobuf-3.19.6/python/
#    protobuf-3.19.6/third_party/
#    protobuf-3.19.6/util/
#    protobuf-3.19.6/ar-lib
#    protobuf-3.19.6/compile
#    protobuf-3.19.6/config.guess
#    protobuf-3.19.6/config.h.in
#    protobuf-3.19.6/config.sub
#    protobuf-3.19.6/configure
#    protobuf-3.19.6/depcomp
#    protobuf-3.19.6/install-sh
#    protobuf-3.19.6/ltmain.sh
#    protobuf-3.19.6/missing
#    protobuf-3.19.6/test-driver
#    protobuf-3.19.6/update_file_lists.sh
#    protobuf-3.19.6/autogen.sh
#    protobuf-3.19.6/build_files_updated_unittest.sh
#    protobuf-3.19.6/generate_descriptor_proto.sh
#    protobuf-3.19.6/maven_install.json

# - disable configuration check of cmake's mimimum version
# - disable configuration zlib check
# - disable installation script
# - fix minor compilation issues on SunPro 5.14.0


cmake_minimum_required(VERSION 3.1)
PROJECT(Protobuf)

include(../setup.cmake)
include(platform)

SET(PROTO_SRC_DIR  "${PROJECT_SOURCE_DIR}/protobuf-3.19.6")

#
# Produce position independent code.
# Note: This is crucial on Solaris
#

enable_pic()
enable_cxx17()

# -O3 using GCC on SPARC leds to segfault on protoc
if(SPARC AND GCC)
  foreach(LANG C CXX)
  foreach(TYPE RELEASE)
    string(REPLACE "-O3" "-O2" CMAKE_${LANG}_FLAGS_${TYPE} "${CMAKE_${LANG}_FLAGS_${TYPE}}")
  endforeach(TYPE)
  endforeach(LANG)
endif()

#
# Do not export any symbols
#
# Note: The LIBPROTOBUF_EXPORTS macro should *not* be defined
#

set_visibility(hidden)

if(NOT TOOLSET_MSVC AND NOT APPLE)
  add_compile_options(-Wno-stringop-overflow -Wno-stringop-overread)
endif()

if(APPLE)
  add_compile_options(-Wno-deprecated-declarations)
endif()


#
# Configure static runtime library on Windows if requested
#

OPTION(STATIC_MSVCRT "Use static MSVC runtime library" OFF)
SET(protobuf_MSVC_STATIC_RUNTIME ${STATIC_MSVCRT} CACHE INTERNAL "")


SET(protobuf_BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
SET(protobuf_BUILD_TESTS OFF CACHE INTERNAL "")
SET(protobuf_BUILD_EXAMPLES OFF CACHE INTERNAL "")
SET(protobuf_WITH_ZLIB OFF CACHE INTERNAL "")

ADD_SUBDIRECTORY(${PROTO_SRC_DIR}/cmake)

# Export targets

file(WRITE "${PROJECT_BINARY_DIR}/exports.cmake"
  "set(PROTOBUF_INCLUDE_DIR \"${PROTO_SRC_DIR}/src\")\n\n"
)

macro(pb_export)
  export(TARGETS ${ARGV} NAMESPACE pb_
         APPEND FILE "${PROJECT_BINARY_DIR}/exports.cmake")
endmacro()

pb_export(libprotobuf-lite)
pb_export(libprotobuf)
pb_export(protoc)
