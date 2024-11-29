/* Generated with Cursor editor, using cursor-small and the following prompt:
 * ---
 * Could you create a plain c++ program that outputs a 1024x1024 BMP file
 * within which there is a painting of a beautiful autumn tree?
 * ---
 * Manual changes:
 * added #include <cstdint>
 * modified output file name to have index 4
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t fileType{0x4D42}; // BM
    uint32_t fileSize{0};       // Size of the file (to be filled later)
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offsetData{54};    // Offset to start of pixel data
};

struct DIBHeader {
    uint32_t size{40};          // Size of this header
    int32_t width{1024};        // Width of the bitmap in pixels
    int32_t height{1024};       // Height of the bitmap in pixels
    uint16_t planes{1};         // Number of color planes
    uint16_t bitCount{24};      // Bits per pixel
    uint32_t compression{0};    // Compression type
    uint32_t sizeImage{0};      // Image size (can be 0 for uncompressed)
    int32_t xPixelsPerMeter{0}; // Horizontal resolution
    int32_t yPixelsPerMeter{0}; // Vertical resolution
    uint32_t colorsUsed{0};     // Number of colors in the palette
    uint32_t colorsImportant{0}; // Important colors
};
#pragma pack(pop)

void createBMP(const std::string& filename) {
    BMPHeader bmpHeader;
    DIBHeader dibHeader;

    // Create a 1024x1024 image filled with white
    std::vector<uint8_t> pixelData(1024 * 1024 * 3, 255); // White background

    // Draw a simple tree (trunk and leaves)
    // Trunk
    for (int y = 600; y < 800; ++y) {
        for (int x = 490; x < 530; ++x) {
            pixelData[(y * 1024 + x) * 3 + 0] = 139; // Brown color (R)
            pixelData[(y * 1024 + x) * 3 + 1] = 69;  // Brown color (G)
            pixelData[(y * 1024 + x) * 3 + 2] = 19;  // Brown color (B)
        }
    }

    // Leaves (simple representation)
    for (int y = 400; y < 600; ++y) {
        for (int x = 400; x < 624; ++x) {
            if ((x - 512) * (x - 512) + (y - 500) * (y - 500) < 10000) { // Circle for leaves
                pixelData[(y * 1024 + x) * 3 + 0] = 255; // Red
                pixelData[(y * 1024 + x) * 3 + 1] = 165; // Orange
                pixelData[(y * 1024 + x) * 3 + 2] = 0;   // Yellow
            }
        }
    }

    // Calculate file size
    bmpHeader.fileSize = sizeof(BMPHeader) + sizeof(DIBHeader) + pixelData.size();

    // Write to file
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader));
    file.write(reinterpret_cast<const char*>(&dibHeader), sizeof(dibHeader));
    file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());
    file.close();
}

int main() {
    createBMP("autumn_tree6.bmp");
    std::cout << "BMP file created: autumn_tree.bmp" << std::endl;
    return 0;
}