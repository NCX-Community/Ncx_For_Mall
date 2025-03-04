cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(ncxs VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

# 从环境变量 MY_INSTALL_DIR 中读取路径
# 设置 CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH "/home/cjy/.local")

if(GRPC_AS_SUBMODULE)
  add_subdirectory(../../.. ${CMAKE_CURRENT_BINARY_DIR}/grpc EXCLUDE_FROM_ALL)
  message(STATUS "Using gRPC via add_subdirectory.")

  # After using add_subdirectory, we can now use the grpc targets directly from
  # this build.
  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_REFLECTION grpc++_reflection)
  set(_ORCA_SERVICE grpcpp_orca_service)
  if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
  else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
  endif()
  set(_GRPC_GRPCPP grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
  endif()
elseif(GRPC_FETCHCONTENT)
  # Another way is to use CMake's FetchContent module to clone gRPC at
  # configure time. This makes gRPC's source code available to your project,
  # similar to a git submodule.
  message(STATUS "Using gRPC via add_subdirectory (FetchContent).")
  include(FetchContent)
  FetchContent_Declare(
    grpc
    GIT_REPOSITORY https://github.com/grpc/grpc.git
    # when using gRPC, you will actually set this to an existing tag, such as
    # v1.25.0, v1.26.0 etc..
    # For the purpose of testing, we override the tag used to the commit
    # that's currently under test.
    GIT_TAG        vGRPC_TAG_VERSION_OF_YOUR_CHOICE)
  FetchContent_MakeAvailable(grpc)

  # Since FetchContent uses add_subdirectory under the hood, we can use
  # the grpc targets directly from this build.
  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_REFLECTION grpc++_reflection)
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
  set(_GRPC_GRPCPP grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
  endif()
else()
  # This branch assumes that gRPC and all its dependencies are already installed
  # on this system, so they can be located by find_package().

  # Find Protobuf installation
  # Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
  option(protobuf_MODULE_COMPATIBLE TRUE)
  find_package(Protobuf CONFIG REQUIRED)
  message(STATUS "Using protobuf ${Protobuf_VERSION}")

  set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
  set(_REFLECTION gRPC::grpc++_reflection)
  if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
  else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
  endif()

  # Find gRPC installation
  # Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
  find_package(gRPC CONFIG REQUIRED)
  message(STATUS "Using gRPC ${gRPC_VERSION}")

  set(_GRPC_GRPCPP gRPC::grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
  endif()
endif()


# module 
include_directories(
    cli
    util
    protocol
    network
    ncx_client
    ncx_server
)

# Boost
find_package(Boost REQUIRED COMPONENTS program_options)
if(BOOST_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Using Boost ${Boost_VERSION}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found")
endif()

# TomlPlusPlus
include(FetchContent)
FetchContent_Declare(
    TomlPlusPlus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(TomlPlusPlus)

# source code
aux_source_directory(network NET)
aux_source_directory(util UTIL)
aux_source_directory(ncx_client NCX_CLIENT)
aux_source_directory(ncx_server NCX_SERVER)
aux_source_directory(protocol PROTOCOL)
aux_source_directory(cli CLI)

# src 
file(GLOB NCXS "ncx_server/ncxs.cpp")
file(GLOB NCXC "ncx_client/ncxc.cpp")
add_executable(ncxs ${NCXS} ${NET} ${UTIL} ${NCX_SERVER} ${PROTOCOL} ${CLI})
add_executable(ncxc ${NCXC} ${NET} ${UTIL} ${NCX_CLIENT} ${PROTOCOL} ${CLI})

# link protobuf
target_link_libraries(ncxc
  absl::check
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

target_link_libraries(ncxs
absl::check
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF})

# link boost
target_link_libraries(ncxc ${Boost_LIBRARIES})
target_link_libraries(ncxs ${Boost_LIBRARIES})

# link boost program_options
target_link_libraries(ncxc Boost::program_options)
target_link_libraries(ncxs Boost::program_options)

# link tomlplusplus
target_link_libraries(ncxc tomlplusplus::tomlplusplus)
target_link_libraries(ncxs tomlplusplus::tomlplusplus)