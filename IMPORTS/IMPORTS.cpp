
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
    auto fileBytes = ReadBytesFromFile(R"(D:\WINDOWS\injector.exe)");
    auto DOS_HEADER = (IMAGE_DOS_HEADER*)fileBytes.data();

    auto NT_HEADER = (IMAGE_NT_HEADERS64*)(fileBytes.data() + DOS_HEADER->e_lfanew);

     // SECTION HEADERS
    IMAGE_SECTION_HEADER* header;
    header = IMAGE_FIRST_SECTION(NT_HEADER);

  

}