// PE DOS Header.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <Windows.h>

using namespace std;

std::vector<uint8_t> ReadBytesFromFile(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    std::vector<uint8_t> bytes;

    // CHECK IF FILE IS GOOD
    if (!file.is_open())
    {
        cout << "Failed to open file. Error: 0x.." << endl;
        return bytes;
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});

    return buffer;
}


int main()
{
    auto fileBytes = ReadBytesFromFile(R"(D:\Windows\Dll2.dll)");
    auto rawBytes = fileBytes.data();

    auto DOS_HEADER = (IMAGE_DOS_HEADER*)rawBytes;
    cout << "File Size: " << fileBytes.size() << endl;
    cout << "MagicNumber: " << hex << DOS_HEADER->e_magic << endl; // AKA MZ 54D4
    cout << "PE header offset: 0x" << hex << DOS_HEADER->e_lfanew << endl; // OFFSET TO THE NEXT STRUCT IN MEMORY 





}