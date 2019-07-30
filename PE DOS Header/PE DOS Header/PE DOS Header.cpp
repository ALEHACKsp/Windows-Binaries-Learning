// PE DOS Header.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <Windows.h>

using namespace std;

std::vector<uint8_t> ReadBytesFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::vector<uint8_t> bytes;

    // CHECK IF FILE IS GOOD
    if(!file.is_open())
    {
        cout << "Failed to open file. Error: 0x.." << endl;
        return bytes;
    }

    // GO TO END OF FILE
    file.seekg(0, file.end);

    // GET THE CURRENT POSITION OF STREAM (NOW END OF FILE)
    uint32_t size = file.tellg();

    // RETURN TO BEGIN of file
    file.seekg(0, file.beg);

    for(auto i = 0; i<size; i++)
    {
        char tmpChar;
        file.read(&tmpChar, 1);
        bytes.push_back(tmpChar);
    }

    return bytes;
}


int main()
{
    auto fileBytes = ReadBytesFromFile(R"(D:\Windows\Dll2.dll)");
    auto rawBytes = fileBytes.data();

    auto DOS_HEADER = (IMAGE_DOS_HEADER*)rawBytes;
    cout << "File Size: " << fileBytes.size() << endl;

    cout << "MagicNumber: " << hex << DOS_HEADER->e_magic << endl; // AKA MZ

    cout << "PE header offset: 0x" << hex << DOS_HEADER->e_lfanew << endl; // OFFSET TO THE NEXT STRUCT IN MEMORY 




}