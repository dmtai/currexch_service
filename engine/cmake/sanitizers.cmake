option(ADDRESS_SANITIZER_ENABLED "Enable address sanitizer" OFF)
if(ADDRESS_SANITIZER_ENABLED)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-omit-frame-pointer -fsanitize=address")
endif()