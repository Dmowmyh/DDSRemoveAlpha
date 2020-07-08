#include "DDS10Header.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <bitset>
#include <set>
#include <vector>

std::vector<char> alphaBlock
{
    64,
   -64,
    31,
   -16,
     7,
    -4,
     1,
   127,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0
};

struct DDS_PIXELFORMAT
{
    unsigned int dwSize;
    unsigned int dwFlags;
    unsigned char dwFourCC[4];
    unsigned int dwRGBBitCount;
    unsigned int dwRBitMask;
    unsigned int dwGBitMask;
    unsigned int dwBBitMask;
    unsigned int dwABitMask;
};

typedef struct
{
    unsigned int dwSize;
    unsigned int dwFlags;
    unsigned int dwHeight;
    unsigned int dwWidth;
    unsigned int dwPitchOrLinearSize;
    unsigned int dwDepth;
    unsigned int dwMipMapCount;
    unsigned int dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    unsigned int dwCaps;
    unsigned int dwCaps2;
    unsigned int dwCaps3;
    unsigned int dwCaps4;
    unsigned int dwReserved2;
} DDS_HEADER;

typedef struct
{
    unsigned int dwMagic;
    DDS_HEADER header;
} DDS_LAYOUT;

typedef struct
{
    DXGI_FORMAT dxgiFormat;
    D3D10_RESOURCE_DIMENSION resourceDimension;
    unsigned int miscFlag;
    unsigned int arraySize;
    unsigned int miscFlags2;
} DDS_HEADER_DXT10;

int main()
{
    using namespace std::string_literals;

    DDS_LAYOUT layout;
    DDS_HEADER_DXT10 DXT10Layout;
    char buffer[sizeof(DDS_LAYOUT)];
    char bufferDX10[sizeof(DDS_HEADER_DXT10)];
    std::ifstream file("amazing_text_01.dds", std::ios::in | std::ios::binary);
    file.read(buffer, sizeof(buffer));
    file.read(bufferDX10, sizeof(bufferDX10));
    memcpy(&layout, buffer, sizeof(buffer));
    memcpy(&DXT10Layout, bufferDX10, sizeof(bufferDX10));

    std::ofstream newFile("amazin_test.dds", std::ios::out);

    int nTotalSize = std::max<unsigned int>(1, ((layout.header.dwWidth + 3) / 4)) *
                     std::max<unsigned int>(1, ((layout.header.dwHeight + 3) / 4)) * 16;

    char block[nTotalSize];
    std::vector<char> testtt;
    std::vector<char> testtt2;
    file.read(block, sizeof(block));
    unsigned int blockLengthInBits = 128;

    int bHeight = 0;
    int bWidth = 0;
    for (size_t j = 0; j < layout.header.dwHeight / 4; j++)
    {
        bool success = false;
        for (size_t i = 0; i < layout.header.dwWidth / 4; i++)
        {
            if (success)
            {
                break;
            }
            for (size_t k = 0; k < blockLengthInBits / 8; k++)
            {
                if (alphaBlock[k] != block[(j*layout.header.dwWidth*4) + (i * 16) + k])
                {
                    bHeight++;
                    success = true;
                    break;
                }
            }
        }
        if (success)
        {
            for (size_t i = 0; i < layout.header.dwWidth/4; i++)
            {
                for (size_t k = 0; k < blockLengthInBits / 8; k++)
                {
                    testtt.push_back(block[(j*layout.header.dwWidth*4) + (i * 16) + k]);
                }
            }
        }
    }
    layout.header.dwHeight = bHeight*4;
    for (size_t col = 0; col < layout.header.dwWidth / 4; col++)
    {
        bool success = false;
        for (size_t row = 0; row < layout.header.dwHeight / 4; row++)
        {
            if (success)
            {
                break;
            }
            for (size_t bit = 0; bit < blockLengthInBits / 8; bit++)
            {
                if (alphaBlock[bit] != testtt[(row * layout.header.dwWidth * 4) +
                                              (col * 16) + bit])
                {
                    bWidth++;
                    success = true;
                    break;
                }
            }
        }
        if (success)
        {
            for (size_t row = 0; row < layout.header.dwHeight/4; row++)
            {
                for (size_t bit = 0; bit < blockLengthInBits / 8; bit++)
                {
                    testtt2.push_back((row * layout.header.dwWidth * 4) +
                                      (col * 16) + bit);
                }
            }
        }
    }
    layout.header.dwWidth = bWidth*4;
    newFile.write((char*)&layout, sizeof(layout));
    newFile.write((char*)&DXT10Layout, sizeof(DXT10Layout));
    for (const auto& it: testtt2)
    {
        newFile << it;
    }
    newFile.close();

    return 0;
}
