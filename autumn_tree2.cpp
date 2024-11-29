/* Generated with Cursor editor, using gpt-4o-mini and the following prompt:
 * ---
 * Could you create a plain c++ program that outputs a 1024x1024 BMP file
 * within which there is a painting of a beautiful autumn tree?
 * ---
 * Manual changes:
 * added #include <cstdint>
 * moved struct BMPheader definition to be after the BITMAPINFOHEADER
 * modified output file name to have index 2
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BITMAPINFOHEADER {
    uint32_t biSize = sizeof(BITMAPINFOHEADER);
    int32_t biWidth = 1024;
    int32_t biHeight = 1024;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;
    int32_t biXPelsPerMeter = 0;
    int32_t biYPelsPerMeter = 0;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
};
struct BMPHeader {
    uint16_t bfType = 0x4D42; // 'BM'
    uint32_t bfSize;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = sizeof(BMPHeader) + sizeof(BITMAPINFOHEADER);
};
#pragma pack(pop)

void drawAutumnTree(std::vector<uint8_t>& pixels) {
    // Simple representation of an autumn tree
    for (int y = 0; y < 1024; ++y) {
        for (int x = 0; x < 1024; ++x) {
            // Background color (sky)
            pixels[(y * 1024 + x) * 3 + 0] = 135; // Blue
            pixels[(y * 1024 + x) * 3 + 1] = 206; // Green
            pixels[(y * 1024 + x) * 3 + 2] = 235; // Red

            // Draw tree trunk
            if (x >= 460 && x <= 564 && y >= 600 && y <= 800) {
                pixels[(y * 1024 + x) * 3 + 0] = 139; // Dark brown
                pixels[(y * 1024 + x) * 3 + 1] = 69;  // Dark brown
                pixels[(y * 1024 + x) * 3 + 2] = 19;  // Dark brown
            }

            // Draw leaves (autumn colors)
            if ((x - 512) * (x - 512) + (y - 400) * (y - 400) < 20000) {
                pixels[(y * 1024 + x) * 3 + 0] = 255; // Red
                pixels[(y * 1024 + x) * 3 + 1] = 165; // Orange
                pixels[(y * 1024 + x) * 3 + 2] = 0;   // Yellow
            }
        }
    }
}

int main() {
    std::vector<uint8_t> pixels(1024 * 1024 * 3);
    drawAutumnTree(pixels);

    BMPHeader bmpHeader;
    BITMAPINFOHEADER bmpInfoHeader;

    bmpHeader.bfSize = sizeof(BMPHeader) + sizeof(BITMAPINFOHEADER) + pixels.size();
    
    std::ofstream file("autumn_tree2.bmp", std::ios::binary);
    file.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader));
    file.write(reinterpret_cast<const char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    file.close();

    std::cout << "BMP file created: autumn_tree.bmp" << std::endl;
    return 0;
}