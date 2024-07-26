#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char ** argv)
{
    if(argc == 0){return;}
    std::fstream File;
    File.open(argv[0], std::fstream::in | std::fstream::out | std::fstream::app);
    if(File.fail()){return;}
    File << "Bom dia";
}
