# build container 

    docker build -t biosim4 .

# run cmake

* enter container

    docker run --rm -ti -v `pwd`:/app --name biosim biosim4 bash 

    cmake -B build/ -S .

    cd build/

    make

# exes are installed into the bin directory
    * biosim4

    * bio_tests - single exe built by gtests with all tests
