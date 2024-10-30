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

typedef unsigned long long uint64;

void PlaceKey(std::fstream * FileOut, uint64 Key)
{
    FileOut->put((char)(Key));
    FileOut->put((char)(Key >> 8));
    FileOut->put((char)(Key >> 16));
    FileOut->put((char)(Key >> 24));
    FileOut->put((char)(Key >> 32));
    FileOut->put((char)(Key >> 40));
    FileOut->put((char)(Key >> 48));
    FileOut->put((char)(Key >> 56));
}

uint64 ExtractKey(std::fstream * FileIn)
{
    Buffer->seekg(-8, ios_base::end);
    uint64 Key = 0;

    Key |= (((unsigned long long)Buffer->get()));
    Key |= (((unsigned long long)Buffer->get()) << 8);
    Key |= (((unsigned long long)Buffer->get()) << 16);
    Key |= (((unsigned long long)Buffer->get()) << 24);
    Key |= (((unsigned long long)Buffer->get()) << 32);
    Key |= (((unsigned long long)Buffer->get()) << 40);
    Key |= (((unsigned long long)Buffer->get()) << 48);
    Key |= (((unsigned long long)Buffer->get()) << 56);

    return Key;
}

void Encryption(std::fstream * FileIn, std::fstream * FileOut, uint64 Key, bool IsEncrypted)
{
    std::minstd_rand RNG ((Key ^ (Key >> 32)) & 0xffffffff);

    if(IsEncrypted)
    {
        FileIn->seekg(-8, ios_base::end);
    }else
    {
        FileIn->seekg(0, ios_base::end);
    }

    uint64 Size = FileIn->tellg();
    FileIn->seekg(0);

    for(int i = 0; i < Size; ++i)
    {
        FileOut->put(FileIn.get() ^ (RNG() ^ (Key >> RNG() % 57)));
    }
}

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
        vector<unsigned char> Buffer((istreambuf_iterator<char>(File)), (istreambuf_iterator<char>()));
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

        Buffer.push_back((char)(Key >> 56));
        Buffer.push_back((char)(Key >> 48));
        Buffer.push_back((char)(Key >> 40));
        Buffer.push_back((char)(Key >> 32));
        Buffer.push_back((char)(Key >> 24));
        Buffer.push_back((char)(Key >> 16));
        Buffer.push_back((char)(Key >> 8));
        Buffer.push_back((char)(Key));

        Path.append(".cmp");

        std::cout << Path;
        File.open(Path, fstream::trunc | fstream::out | fstream::binary);

        File.write((char *)Buffer.data(), Buffer.size());

        File.close();
    }
}
