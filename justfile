clean:
    rm -fRd ./build
    rm -fRd ./cpp_joinstr/target
    rm -fRd ./lib

clear:
    just clean
    rm -fRd ./cpp_joinstr
    rm -fRd ./.cache

make:
    just build

build:
    ./build.sh
