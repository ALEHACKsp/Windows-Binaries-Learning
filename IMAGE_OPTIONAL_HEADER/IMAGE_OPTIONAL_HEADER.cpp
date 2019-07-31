
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

    auto IMAGE_FILE_HEADER = NT_HEADER->FileHeader;

    auto IMAGE_OPTIONAL_HEADER = NT_HEADER->OptionalHeader;

    // ITS NOT OPTIONAL, ITS DEFINE IF IS A PE32 OR PE32+ (0x10B) = x32 AND (0X20B) = x64
    auto magic = IMAGE_OPTIONAL_HEADER.Magic;

    
    // ITS SPECIFIES THE RVA (RELATIVE ADRESS) WHERE THE LOADER START EXECUTING CODE AFTER THE BINARY IS FULLY LOADED TO MEM
    auto adress_of_entry_point = IMAGE_OPTIONAL_HEADER.AddressOfEntryPoint;

    // ITS HOLD THE ENTIRE SIZE OF THE BINARY THAT NEEDED TO BE ALLOCATED IN MEMORY
    auto size_of_image = IMAGE_OPTIONAL_HEADER.SizeOfCode;

    // SPECIFIES THAT SECTION SHOULD BE ALIGNED ON BOUNDARIES WHICH ARE MULTIPLE OF THIS VALUE.
    auto section_alignment = IMAGE_OPTIONAL_HEADER.SectionAlignment;

    // ITS SOMETHING LIKE (IF YOU DON'T HAVE MULTIPLES OF 0X200, YOU NEED TO PADDING BEFORE YOU PUT THE NEXT SECTION)
    auto file_alignment = IMAGE_OPTIONAL_HEADER.FileAlignment;

    // TELLS THE OS THAT WE PREFERR TO BE ALLOCATED IN VIRTUAL MEM AT X ADRESS
    // IF THE VIRTUAL ADRESS IS ALREADY BEING USED, WE HAVE TO RELOC THE IMAGE. THIS RELOAC INFORMATION ITS UNDER .RELOAC SECTION
    auto image_base = IMAGE_OPTIONAL_HEADER.ImageBase;

    // SPECIFIES SECURITY OPTIONS, AND THIS ITS NOT LIMITED TO DLLS

    /*
     * IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE > TELLS THE OS THAT THIS BINARY SUPPORT ASLR.
     * IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY > CHECK AT LOAD TIME WETHER THE DLL IS SIGNED OR NOT.
     * IMAGE_DLLCHARACTERISTICS_NX_COMPAT > TELLS THE LOADER THAT THIS IMAGE IS COMPATIBLE WITH DATA EXECUTION PREVENTIN (DEP) AND THAT
     * ...NON EXECUTABLE SECTION SHOULD HAVE AN NX FLAG SET IN MEMORY
     * IMAGE_DLLCHARACTERISTICS_NO_SEH > TELLS THAT BINARY DOEST NOT USE SEH HANDLER STRUCUTED EXECEPTIONS, AND THE BEFORE NONE SHOULD BE CREATED.
     * 
     */
    auto dll_characteristics = IMAGE_OPTIONAL_HEADER.DllCharacteristics;

    // DataDirectory holds pointer to all other data structs like reloc, imports, exports, etc...
    auto data_directory = IMAGE_OPTIONAL_HEADER.DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES].Size;


}