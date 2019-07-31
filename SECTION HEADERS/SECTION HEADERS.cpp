
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

    // SECTION > GROUP OF CODE OR DATA THAT SHOULD SHAVE SAME PERMISSION 

    // COMMON SECTION NAMES

    /*
     * .text -> Code that never should be paged out of memory to disk.
     * .data -> read/write data (globals)
     * .rdata -> read-onlydata (strings)
     * .bss -> Block started by symbol or Block Storage Segement (Uninitiliazed data), generaly is merged at .data section
     * .idata -> import adress table. Generaly merged in .text or .rdata
     * .edata -> export information
     * PAGE* -> CODE/DATA WHICH IS SAFE TO PAGE OUT TO DISK, WHEN RUNNING ON LOW MEMORY, GENERALLY USED FOR KERNEL DRIVERS.
     * .reloc -> REALOCATION INFORMATION FOR MODIFY HARDCODED ADRESS 
     * .rsrc -> RESOURCES. COULD BE ICON, EMBEDDED BINARIES, IMAGES, ETC... THE SECTION HAS STRUCTURES ORGANIZING IT, LIKE A FILESYSTEM
     * .pdata -> ARM, MIPS, SH windows, CE compilers, USE pdata STRUCTURES TO AIDS IN STACK WALKING AT RUN-TIME, ITS AIDS IN DEBUGGINGAND EXCEPTION PROCESSING.
     * 
     */


    // SECTION HEADERS
    IMAGE_SECTION_HEADER* header;
    header = IMAGE_FIRST_SECTION(NT_HEADER);

    // NAME OF THE SECTION AKA .TEXT FOR EXAMPLE. 8 BYTES ARRAY ASCII. DOEST NOT GUARANTEE OF NULL TERMINATED.
    auto name = header->Name;

    // IS THE RVA (RELATIVE VIRTUAL ADRESS) OF THE SECTION RELATIVE TO 'OptionalHeader.ImageBase' (IN THE MEMORY)
    auto virtual_adress = header->VirtualAddress;

    // RELATIVE OFFSET FROM THE BEGIN OF THE FILE, WHICH TELLS WHERE  THE ACTUAL SECTION IS STORED. (IN THE FILE)
    auto pointer_to_raw_data = header->PointerToRawData;

    // SOMETIMES IS GREAT THAN SIZEOFRAWDATA. BECAUSE THE DATA FROM .BSS THAT WAS UNITILIAZED, FOR MAKING THE BINARY SMALLER ON DISK, IS HERE ALLOCATED TO MEM.
    auto virtual_size = header->Misc.VirtualSize;

    // THIS COULD BE GRATE THAN VIRTUAL_SIZE BECAUSE OF FIELD 'OptionalHeader.FileAlignment' aka Padding. But in memory it will be reduced to virtual_size.
    auto size_of_raw_data = header->SizeOfRawData;

    /*
     * IMAGE_SCN_CODE > SECTION THAT CONTAIN CODE (0X20)
     * IMAGE_SCN_INITILIAZED_DATA > SECTION CONTAIN INITILIAZED DATA (0X40)
     * IMAGE_SCN_UNINITILIAZED_DATA > SECTION CONTAIN UNINITILIAZED DATA (0X80)
     * IMAGE_SCN_MEM_DISCARDABLE > DO NOT CACHE THIS SECTION  (0X02000000)
     * IMAGE_SCN_MEM_NOT_CACHED > SECTION CAN BE DISCARTED (0X04000000)
     * IMAGE_SCN_MEM_NOT_PAGED > SECTION IS NOT PAGEABLE (0X08000000)
     * IMAGE_SCN_MEM_SHARED > SECTION IS SHAREABLE (0X10000000)
     * IMAGE_SCN_MEM_EXECUTE > EXECUTE (0X20000000) 
     * IMAGE_SCN_MEM_READ > READ (0X40000000) 
     * IMAGE_SCN_MEM_WRITE > WRITE (0X80000000) 
     */

    auto charctertistics = header->Characteristics;

}