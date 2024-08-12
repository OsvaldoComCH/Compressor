#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstring>
#include <random>

using namespace std;

int main(int argc, char ** argv)
{
    if(argc == 1){return 1;}
    fstream File;
    File.open(argv[1], fstream::in | fstream::out | fstream::app | fstream::binary);
    File.unsetf(fstream::skipws);
    if(File.fail()){return 1;}
    std::minstd_rand RNG (std::chrono::system_clock::now().time_since_epoch().count());
    if(argc > 2 && !std::strcmp(argv[2], "-d"))
    {
        std::cout << "-d\n";
        File.close();
    }else
    {
        vector<char> Buffer ((istreambuf_iterator<char>(File)), (istreambuf_iterator<char>()));
        File.close();
        
        long long Key = Buffer[RNG() % Buffer.size()] | ((long long)Buffer[RNG() % Buffer.size()] << 8) |
        ((long long)Buffer[RNG() % Buffer.size()] << 16) | ((long long)Buffer[RNG() % Buffer.size()] << 24) |
        ((long long)Buffer[RNG() % Buffer.size()] << 32) | ((long long)Buffer[RNG() % Buffer.size()] << 40) |
        ((long long)Buffer[RNG() % Buffer.size()] << 48) | ((long long)Buffer[RNG() % Buffer.size()] << 56);

        Key ^= ((long long)RNG() << 32) | RNG();

        std::cout << Key << "\n\n";

        for(int i = 0; i < Buffer.size() - 1; ++i)
        {
            Buffer[i] ^= Buffer[i+1];
        }
        Buffer.push_back((char)(Key >> 56));
        Buffer.push_back((char)(Key >> 48));
        Buffer.push_back((char)(Key >> 40));
        Buffer.push_back((char)(Key >> 32));
        Buffer.push_back((char)(Key >> 24));
        Buffer.push_back((char)(Key >> 16));
        Buffer.push_back((char)(Key >> 8));
        Buffer.push_back((char)(Key));
        
        for(int i = 0; i < Buffer.size(); ++i)
        {
            std::cout << Buffer[i];
        }

        std::cout << "\n\n";
        for(int i = Buffer.size() - 10; i >= 0; --i)
        {
            Buffer[i] ^= Buffer[i+1];
        }
        for(int i = 0; i < Buffer.size(); ++i)
        {
            std::cout << Buffer[i];
        }
    }
}
