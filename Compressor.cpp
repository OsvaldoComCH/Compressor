#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

int main(int argc, char ** argv)
{
    if(argc == 1){return 1;}
    fstream File;
    File.open(argv[1], fstream::in | fstream::out | fstream::app | fstream::binary);
    File.unsetf(fstream::skipws);
    if(File.fail()){return 1;}
    if(argc > 2 && !std::strcmp(argv[2], "-d"))
    {
        std::cout << "-d\n";
        File.close();
    }else
    {
        vector<char> Buffer ((istreambuf_iterator<char>(File)), (istreambuf_iterator<char>()));
        File.close();
        for(int i = 0; i < Buffer.size(); ++i)
        {
            std::cout << Buffer[i];
            Buffer[i] ^= 0xC7;
        }
        for(int i = 0; i < Buffer.size(); ++i)
        {
            std::cout << Buffer[i];
        }
    }
}
