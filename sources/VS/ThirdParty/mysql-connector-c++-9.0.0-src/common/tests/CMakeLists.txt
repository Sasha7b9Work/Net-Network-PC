# Copyright (c) 2018, 2024, Oracle and/or its affiliates.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License, version 2.0, as
# published by the Free Software Foundation.
#
# This program is designed to work with certain software (including
# but not limited to OpenSSL) that is licensed under separate terms, as
# designated in a particular file or component or in included license
# documentation. The authors of MySQL hereby grant you an additional
# permission to link the program and your derivative works with the
# separately licensed software that they have either included with
# the program or referenced in the documentation.
#
# Without limiting anything contained in the foregoing, this file,
# which is part of Connector/C++, is also subject to the
# Universal FOSS Exception, version 1.0, a copy of which can be found at
# https://oss.oracle.com/licenses/universal-foss-exception.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License, version 2.0, for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

#
# Note: We must clear compile flags - the ones used to build the connector
# are not good for building client code that uses the connector.
#

set_property(
  DIRECTORY .
  PROPERTY COMPILE_DEFINITIONS ""
)

if(WIN32)
  add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    -D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
  )

endif()

#Add cdk includes because we are using their source tests
add_test_includes(${PROJECT_SOURCE_DIR}/cdk/include)
add_test_includes(${PROJECT_SOURCE_DIR}/cdk/extra/rapidjson/include)
add_test_includes(${PROJECT_BINARY_DIR}/cdk/include)

ADD_TEST_LIBRARIES(cdk_parser)

ADD_NG_TEST(common-t
  ${PROJECT_SOURCE_DIR}/cdk/parser/tests/parser-t.cc
)


