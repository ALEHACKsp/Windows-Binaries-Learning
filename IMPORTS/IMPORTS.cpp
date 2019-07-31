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


PVOID RvaToFileOffset(DWORD_PTR RVA, PIMAGE_NT_HEADERS header, PUCHAR imageBase)
{
    IMAGE_SECTION_HEADER* pFirstSection = IMAGE_FIRST_SECTION(header);

    for(IMAGE_SECTION_HEADER* pSection = pFirstSection; pSection < pFirstSection + header->FileHeader.NumberOfSections; pSection++)
    {
        if (RVA >= pSection->VirtualAddress && RVA < pSection->VirtualAddress + pSection->Misc.VirtualSize)
            return imageBase + pSection->PointerToRawData + (RVA - pSection->VirtualAddress);
    }

    return nullptr;
}

int main()
{
    auto fileBytes = ReadBytesFromFile(R"(D:\WINDOWS\injector.exe)");
    auto DOS_HEADER = (IMAGE_DOS_HEADER*)fileBytes.data();

    auto NT_HEADER = (IMAGE_NT_HEADERS64*)(fileBytes.data() + DOS_HEADER->e_lfanew);

    // SECTION HEADERS
    IMAGE_SECTION_HEADER* header;
    header = IMAGE_FIRST_SECTION(NT_HEADER);


    // -------------------------------------------------
    //STATIC LINKING VS DYNAMIC LINK
    //_____________________________


    /* STATIC LINKING -> Its include a copy of all helper functions that we are using inside the executable that we are generating.
     * ... Making the file bigger, and probaly with vulnerability or outdated if new versions come, since it would still use an older one.
    */ 

    // DYNAMIC LINKING -> Happens when we resolve the pointer to functions inside the libraries at runtime. Its smaller.

    // -------------------------------------------------


    auto OPTIONAL_HEADER = NT_HEADER->OptionalHeader;


    // EACH DLL HAS ONE
    auto IMPORTS = (IMAGE_IMPORT_DESCRIPTOR*)RvaToFileOffset(OPTIONAL_HEADER.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, NT_HEADER, fileBytes.data());

    // 0 FOR TERMINATING NULL IMPORT DESCRIPTO
    auto characteristics = IMPORTS->Characteristics;

    
    // RVA TO ORIGINAL UNBOUND  IAT (PIMAGE_THUNK_DATA). (INT) [POINTS TO THE FIRST ENTRY IN THE ARRAY][POINTS TO AN ARRAY OF IMAGE_THUNK_DATA]
    auto original_first_thunk = IMPORTS->OriginalFirstThunk;

    // 0 IF NOT BOUND. -1 IF BOUND, AND REAL DATE/TIME STAMP. O.W OLD BIND
    auto time_date_stamp = IMPORTS->TimeDateStamp;


    // -1 IF NO FOWARDERS
    auto fowarder_chain = IMPORTS->ForwarderChain;

    // NAME OF THE LIBRARY
    LPSTR ModuleName = nullptr;
    ModuleName = (LPSTR)RvaToFileOffset(IMPORTS->Name, NT_HEADER, fileBytes.data());

    // RVA TO IAT (IF BOUND THIS IAT HAS ACTUAL ADRESS)
    auto first_thunk = IMPORTS->FirstThunk; // (IAT) [POINTS TO AN ARRAY OF IMAGE_THUNK_DATA]

    // IS INTERPRETED AS POINTING AT IMAGE_IMPORT_BY_NAME STRUCTURES IF u1.AdressOfData EXISTS. THIS IS THE ACTUALLY RVA OF AN IMAGE_IMPORT_BY_NAME
    auto thunkDataEx = (IMAGE_THUNK_DATA64*)RvaToFileOffset(first_thunk, NT_HEADER, fileBytes.data());

    // IF ITS A FUNCTION. FOR EXAMPLE, THIS COULD BE THE ADRESS OF PRINTF
    auto function = thunkDataEx->u1.Function;

    //If its a name
    auto adress_of_data = (IMAGE_IMPORT_BY_NAME*)thunkDataEx->u1.AddressOfData;

    // SPECIFIES A POSSIBLE 'ORDINAL' NUMBER OF A IMPORTED FUNCTION. FOR BASICS, IT FINDS BY INDEX THAN NAME.
    auto hints = adress_of_data->Hint;


    // INT > IMPORT NAMES TABLE
    // IAT > IMPORT ADRESS TABLE

    // Actually name. ITS A NULL TERMINATED ASCII STRING.
    auto name = adress_of_data->Name;

}
