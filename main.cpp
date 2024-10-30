/*
g++ main.cpp -o Compressor.exe -O2 -static-libgcc -static-libstdc++
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <cstdio>

int main(int argc, char ** argv)
{
    using namespace std;

    if(argc == 1){return 1;}
    fstream FileIn, FileOut;

    string Path(argv[1]);

    FileIn.open(argv[1], fstream::in | fstream::out | fstream::app | fstream::binary);
    FileIn.unsetf(fstream::skipws);
    if(FileIn.fail()){return 1;}
    std::minstd_rand RNG (std::chrono::system_clock::now().time_since_epoch().count());

    if(Path.substr(Path.size() - 4, 4) == ".cmp")
    {

    }else
    {

    }
}
