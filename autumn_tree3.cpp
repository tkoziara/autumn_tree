/* Generated with Cursor editor, using gpt-4o and the following prompt:
 * ---
 * Could you create a plain c++ program that outputs a 1024x1024 BMP file
 * within which there is a painting of a beautiful autumn tree?
 * ---
 * Manual changes:
 * added #include <cstdint>
 * modified output file name to have index 3
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t fileType{0x4D42};          // File type always BM which is 0x4D42
    uint32_t fileSize{0};               // Size of the file (in bytes)
    uint16_t reserved1{0};              // Reserved, always 0
    uint16_t reserved2{0};              // Reserved, always 0
    uint32_t offsetData{0};             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
    uint32_t size{0};                  // Size of this header (in bytes)
    int32_t width{0};                  // width of bitmap in pixels
    int32_t height{0};                 // width of bitmap in pixels
                                       // (if positive, bottom-up, with origin in lower left corner)
                                       // (if negative, top-down, with origin in upper left corner)
    uint16_t planes{1};                // No. of planes for the target device, this is always 1
    uint16_t bitCount{0};              // No. of bits per pixel
    uint32_t compression{0};           // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t sizeImage{0};             // 0 - for uncompressed images
    int32_t xPixelsPerMeter{0};
    int32_t yPixelsPerMeter{0};
    uint32_t colorsUsed{0};            // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colorsImportant{0};       // No. of colors used for displaying the bitmap. If 0 all colors are required
};
#pragma pack(pop)

struct Color {
    uint8_t r, g, b;
};

class BMP {
public:
    BMP(int32_t width, int32_t height) : width(width), height(height) {
        rowStride = width * 3;
        if (rowStride % 4 != 0) {
            rowStride += 4 - rowStride % 4;
        }
        data.resize(rowStride * height);
    }

    void setPixel(int32_t x, int32_t y, const Color& color) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        uint8_t* pixel = &data[(height - y - 1) * rowStride + x * 3];
        pixel[0] = color.b;
        pixel[1] = color.g;
        pixel[2] = color.r;
    }

    void write(const char* filename) {
        std::ofstream file(filename, std::ios::binary);

        BMPHeader header;
        BMPInfoHeader infoHeader;

        header.fileSize = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + data.size();
        header.offsetData = sizeof(BMPHeader) + sizeof(BMPInfoHeader);

        infoHeader.size = sizeof(BMPInfoHeader);
        infoHeader.width = width;
        infoHeader.height = height;
        infoHeader.bitCount = 24;
        infoHeader.sizeImage = data.size();

        file.write(reinterpret_cast<const char*>(&header), sizeof(header));
        file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }

private:
    int32_t width;
    int32_t height;
    uint32_t rowStride;
    std::vector<uint8_t> data;
};

int main() {
    const int32_t width = 1024;
    const int32_t height = 1024;
    BMP bmp(width, height);

    // Simple autumn tree drawing
    Color brown = {139, 69, 19};
    Color orange = {255, 165, 0};
    Color yellow = {255, 255, 0};
    Color green = {34, 139, 34};

    // Draw trunk
    for (int y = 400; y < 800; ++y) {
        for (int x = 480; x < 540; ++x) {
            bmp.setPixel(x, y, brown);
        }
    }

    // Draw leaves
    for (int y = 200; y < 400; ++y) {
        for (int x = 300; x < 740; ++x) {
            if ((x - 520) * (x - 520) + (y - 300) * (y - 300) < 40000) {
                bmp.setPixel(x, y, orange);
            }
            if ((x - 520) * (x - 520) + (y - 300) * (y - 300) < 30000) {
                bmp.setPixel(x, y, yellow);
            }
            if ((x - 520) * (x - 520) + (y - 300) * (y - 300) < 20000) {
                bmp.setPixel(x, y, green);
            }
        }
    }

    bmp.write("autumn_tree3.bmp");
    std::cout << "BMP file created: autumn_tree.bmp" << std::endl;

    return 0;
}