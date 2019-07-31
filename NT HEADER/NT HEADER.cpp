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
    auto fileBytes = ReadBytesFromFile(
        R"(D:\Downloads\2012_LoB_all_1\2012_LoB_Code\Code\BinaryScavengerHunt_03-17-2013\pythondemo\R1Bins\Round1Q0.exe)");

    auto DOS_HEADER = (IMAGE_DOS_HEADER*)fileBytes.data();

    // HOLDS FILE HEADER AND OPTIONAL HEADER
    auto NT_HEADER = (IMAGE_NT_HEADERS64*)(fileBytes.data() + DOS_HEADER->e_lfanew);

    // TELLS IF ITS AN PE FILE OR ANY OTHER TYPE
    auto signature = NT_HEADER->Signature; // AKA PE\0\0 (4550)

    auto IMAGE_FILE_HEADER = NT_HEADER->FileHeader;

    // TELL US WHAT ARCHITECTURE THAT PROGRAM SUPPOST TO RAM PE32(014c) AKA 32BITS AND PE32+(8664) 64bits
    auto machine = IMAGE_FILE_HEADER.Machine;

    // GIVE US THE LINK TIME THAT PROGRAM WAS GENERATE. Data is in UNIX timestamp and return how many seconds since 1970 has passed.
    auto timeStamp = IMAGE_FILE_HEADER.TimeDateStamp;

    auto days = timeStamp / 86400;

    auto years = days / 360;


    // GIVE US THE NUMBER OF SECTION THAT WE ARE GOING TO HAVE LATER.
    auto numberOfSections = IMAGE_FILE_HEADER.NumberOfSections;

    // SPECIFY THINGS LIKE
    /*
     * IMAGE_FILE_EXECUTABLE > FILE IS EXECUTABLE
     * IMAGE_FILE_NUMS_STRIPPED > LINES THAT STRIPPED FROM THE FILE
     * IMAGE_FILE_LARGE_ADRESS_AWARE > APP CAN HANDLE MORE THAN 2GB +
     * IMAGE_FILE_32BIT_MACHINE > 32BIT COMPUTER
     * IMAGE_fILE_SYSTEM > SYSTEM FILES
     * IMAGE_FILE_DLL > FILE IS A DLL
     */


    auto Characteristics = IMAGE_FILE_HEADER.Characteristics;
}
