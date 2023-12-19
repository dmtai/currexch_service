CMAKE_COMMON_FLAGS ?= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_COMPILER=g++-13
CMAKE_DEBUG_FLAGS ?=
CMAKE_RELEASE_FLAGS ?=
NPROCS ?= $(shell nproc)
CLANG_FORMAT ?= clang-format
CC ?= /usr/bin/gcc-13
CXX ?= /usr/bin/g++-13

.PHONY: all
all: build-debug build-release

build_debug/Makefile:
	@mkdir -p build_debug
	@cd build_debug && \
      cmake -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMMON_FLAGS) $(CMAKE_DEBUG_FLAGS) $(CMAKE_OPTIONS) ..

build_release/Makefile:
	@mkdir -p build_release
	@cd build_release && \
      cmake -DCMAKE_BUILD_TYPE=Release $(CMAKE_COMMON_FLAGS) $(CMAKE_RELEASE_FLAGS) $(CMAKE_OPTIONS) ..

.PHONY: cmake-debug cmake-release
cmake-debug cmake-release: cmake-%: build_%/Makefile

.PHONY: build-debug build-release
build-debug build-release: build-%: cmake-%
	@cmake --build build_$* -j $(NPROCS) --target currexch_service

.PHONY: conan-install-release
conan-install-release:
	@mkdir -p build_release
	@cd build_release &&  \
	  conan install .. --build=missing -s build_type=Release -e CC=$(CC) -e CXX=$(CXX) \
	  	--profile=../conan/profile.txt

.PHONY: conan-install-debug
conan-install-debug:
	@mkdir -p build_debug
	@cd build_debug &&  \
	  conan install .. --build=missing -s build_type=Debug -e CC=$(CC) -e CXX=$(CXX) \
	  	--profile=../conan/profile.txt

.PHONY: docker-run
docker-run:
	@docker-compose run -p 1234:1234 --rm currexch_service

.PHONY: format
format:
	@find src engine -name '*pp' -type f | xargs $(CLANG_FORMAT) -i

.PHONY: clean
clean:
	@rm -rf build
	@rm -rf build_*