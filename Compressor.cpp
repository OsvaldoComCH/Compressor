/*
g++ compressor.cpp -o Compressor.exe -O2 -static-libgcc -static-libstdc++
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <cstdio>

using namespace std;

int main(int argc, char ** argv)
{
    if(argc == 1){return 1;}
    fstream File;

    string Path (argv[1]);

    File.open(argv[1], fstream::in | fstream::out | fstream::app | fstream::binary);
    File.unsetf(fstream::skipws);
    if(File.fail()){return 1;}
    std::minstd_rand RNG (std::chrono::system_clock::now().time_since_epoch().count());

    if(Path.substr(Path.size() - 4, 4) == ".cmp")
    {
        vector<unsigned char> Buffer ((istreambuf_iterator<char>(File)), (istreambuf_iterator<char>()));
        File.close();

        long long Key = 0;
        Key |= (((unsigned long long)Buffer.back()));
        Buffer.pop_back();
        Key |= (((unsigned long long)Buffer.back()) << 8);
        Buffer.pop_back();
        Key |= (((unsigned long long)Buffer.back()) << 16);
        Buffer.pop_back();
        Key |= (((unsigned long long)Buffer.back()) << 24);
        Buffer.pop_back();
        Key |= (((unsigned long long)Buffer.back()) << 32);
        Buffer.pop_back();
        Key |= (((unsigned long long)Buffer.back()) << 40);
        Buffer.pop_back();
        Key |= (((unsigned long long)Buffer.back()) << 48);
        Buffer.pop_back();
        Key |= (((unsigned long long)Buffer.back()) << 56);
        Buffer.pop_back();

        printf("%llx\n", Key);

        RNG = minstd_rand((Key ^ (Key >> 32)) & 0xffffffff);

        for(int i = 0; i < Buffer.size(); ++i)
        {
            Buffer[i] ^= RNG() ^ (Key >> RNG() % 57);
        }

        Path.erase(Path.end() - 4, Path.end());

        std::cout << Path;
        File.open(Path, fstream::trunc | fstream::out | fstream::binary);

        File.write((char *)Buffer.data(), Buffer.size());

        File.close();
    }else
    {
        vector<unsigned char> Buffer ((istreambuf_iterator<char>(File)), (istreambuf_iterator<char>()));
        File.close();
        
        long long Key = Buffer[RNG() % Buffer.size()] | ((long long)Buffer[RNG() % Buffer.size()] << 8) |
        ((long long)Buffer[RNG() % Buffer.size()] << 16) | ((long long)Buffer[RNG() % Buffer.size()] << 24) |
        ((long long)Buffer[RNG() % Buffer.size()] << 32) | ((long long)Buffer[RNG() % Buffer.size()] << 40) |
        ((long long)Buffer[RNG() % Buffer.size()] << 48) | ((long long)Buffer[RNG() % Buffer.size()] << 56);

        Key ^= ((long long)RNG() << 32) | RNG();

        RNG = minstd_rand((Key ^ (Key >> 32)) & 0xffffffff);

        for(int i = 0; i < Buffer.size(); ++i)
        {
            Buffer[i] ^= RNG() ^ (Key >> RNG() % 57);
        }

/*
       int Swaps = Key % Buffer.size();

        RNG = minstd_rand((Key & (Key >> 8) | (Key >> 16) ^ (Key >> 24) ^ 0x55555555) & 0xffffffff);
        for(int i = 0; i < Swaps; ++i)
        {
            unsigned x = RNG() % Buffer.size();
            unsigned y = RNG() % Buffer.size();
            char Temp = Buffer[x];
            Buffer[x] = Buffer[y];
            Buffer[y] = Temp;
        }
*/
        Buffer.push_back((char)(Key >> 56));
        Buffer.push_back((char)(Key >> 48));
        Buffer.push_back((char)(Key >> 40));
        Buffer.push_back((char)(Key >> 32));
        Buffer.push_back((char)(Key >> 24));
        Buffer.push_back((char)(Key >> 16));
        Buffer.push_back((char)(Key >> 8));
        Buffer.push_back((char)(Key));

        printf("%llx\n", Key);

        Path.append(".cmp");
        std::cout << Path;
        File.open(Path, fstream::trunc | fstream::out | fstream::binary);

        File.write((char *)Buffer.data(), Buffer.size());

        File.close();
    }
}
