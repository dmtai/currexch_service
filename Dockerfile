FROM gcc:13

RUN apt update && \
    apt install -y \
      pip \
      cmake \
    && \
    pip install conan==1.60.2 --break-system-packages 

COPY conanfile.txt /currexch_service/
COPY ./conan /currexch_service/conan
RUN cd /currexch_service && \
    mkdir -p build_release && \
    cd build_release && \
      conan install .. --build=missing -s build_type=Release -e CC=/usr/local/bin/gcc -e CXX=/usr/local/bin/g++ \
        --profile=../conan/profile.txt

COPY ./cmake /currexch_service/cmake
COPY ./db /currexch_service/db
COPY ./engine /currexch_service/engine
COPY ./src /currexch_service/src
COPY ./configs /currexch_service/configs
COPY ./tests /currexch_service/tests
COPY Makefile .clang-format .clang-tidy CMakeLists.txt /currexch_service/

RUN cd /currexch_service/build_release && \
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_COMPILER=/usr/local/bin/g++ .. && \
  cmake --build . -j 4 --target currexch_service