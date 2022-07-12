#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>


int main(int argc, const char* argv[])
{
    if(argc != 3)return -1;
    std::string strbuf;
    strbuf = argv[1];
    if(strbuf.size() != 8)
    {
        if (strbuf.size() != 10)
        {
            std::cout << "ERROR: Is argment {address data.bin}?\n";
            return -1;
        }
        strbuf.erase(strbuf.begin(), strbuf.begin() + 2);
    }

    std::filesystem::path filepath = argv[2];
    std::fstream binarydata;
    binarydata.open(filepath.generic_string<char>(), std::ios::binary | std::ios::in);
    if(!binarydata)
    {
        std::cout << "ERROR: Can't open file.\n";
        return -2;
    }

    binarydata.seekg(0, std::ios::end);
    auto binarydata_end = binarydata.tellg();
    binarydata.seekg(0, std::ios::beg);

    std::uint_least32_t* valueCut32;
    valueCut32 = new std::uint32_t[(const int)(binarydata_end - binarydata.tellg())];
    int i = 0;
    do
    {
        std::uint_least32_t cash[4] = {};
        for (int i2 = 0; i2 < 4; i2++)binarydata.read(&(char &)cash[i2], sizeof(char));
        valueCut32[i] = 0;
        valueCut32[i] = (cash[0] << 24) + (cash[1] << 16) + (cash[2] << 8) + cash[3];
        i++;
    } while (binarydata.tellg() != binarydata_end);

    binarydata.close();

    unsigned int int_sstreambuf;
    std::stringstream sstreambuf;
    sstreambuf << std::hex << argv[1];
    sstreambuf >> int_sstreambuf;

    sstreambuf.str("");
    sstreambuf.clear(std::stringstream::goodbit);
    sstreambuf << std::hex;
    sstreambuf << std::setfill('0') << std::right;
    sstreambuf << std::endl;

    for (unsigned int i2 = 0; i2 != i; i2++)
    {
        sstreambuf << "0x";
        sstreambuf << std::setw(8) << (int_sstreambuf + i2);
        sstreambuf << ":dword:0x";
        sstreambuf << std::setw(8) << valueCut32[i2];
        sstreambuf << "\n";
    }

    binarydata.open("out.txt", std::ios::out);
    if(binarydata)
    {
        binarydata << std::string(sstreambuf.str());
        std::cout << "Export txt file out.txt.\n";
    }else{
        std::cout << "Can't export txt file.\n";
    }
    binarydata.close();

    std::cout << sstreambuf.str();
    std::cout << std::setfill(' ');
    delete valueCut32;
    
    return 0;
}