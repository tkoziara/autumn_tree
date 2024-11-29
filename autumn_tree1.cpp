/* Generated with Cursor editor, using claude-3.5-sonnect and the following prompt:
 * ---
 * Could you create a plain c++ program that outputs a 1024x1024 BMP file within which
 * there is a painting of a beautiful autumn tree?
 * ---
 * Manual changes:
 * modified output file name to have index 1
 * */

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

const int WIDTH = 1024;
const int HEIGHT = 1024;
const int BYTES_PER_PIXEL = 3;
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

// Function to create BMP file header
unsigned char* createBitmapFileHeader(int fileSize) {
    static unsigned char fileHeader[] = {
        0, 0,     // signature
        0, 0, 0, 0,     // image file size in bytes
        0, 0, 0, 0,     // reserved
        0, 0, 0, 0,     // start of pixel array
    };

    fileHeader[0] = (unsigned char)('B');
    fileHeader[1] = (unsigned char)('M');
    fileHeader[2] = (unsigned char)(fileSize);
    fileHeader[3] = (unsigned char)(fileSize >> 8);
    fileHeader[4] = (unsigned char)(fileSize >> 16);
    fileHeader[5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

// Function to create BMP info header
unsigned char* createBitmapInfoHeader() {
    static unsigned char infoHeader[] = {
        0, 0, 0, 0,     // header size
        0, 0, 0, 0,     // image width
        0, 0, 0, 0,     // image height
        0, 0,           // number of color planes
        0, 0,           // bits per pixel
        0, 0, 0, 0,     // compression
        0, 0, 0, 0,     // image size
        0, 0, 0, 0,     // horizontal resolution
        0, 0, 0, 0,     // vertical resolution
        0, 0, 0, 0,     // colors in color table
        0, 0, 0, 0,     // important color count
    };

    infoHeader[0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[4] = (unsigned char)(WIDTH);
    infoHeader[5] = (unsigned char)(WIDTH >> 8);
    infoHeader[6] = (unsigned char)(WIDTH >> 16);
    infoHeader[7] = (unsigned char)(WIDTH >> 24);
    infoHeader[8] = (unsigned char)(HEIGHT);
    infoHeader[9] = (unsigned char)(HEIGHT >> 8);
    infoHeader[10] = (unsigned char)(HEIGHT >> 16);
    infoHeader[11] = (unsigned char)(HEIGHT >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL * 8);

    return infoHeader;
}

// Function to draw a branch recursively
void drawBranch(unsigned char* image, int x, int y, double angle, int length, 
                int depth, std::mt19937& gen, std::uniform_real_distribution<>& dis) {
    if (length < 4) return;

    int endX = x + static_cast<int>(cos(angle) * length);
    int endY = y + static_cast<int>(sin(angle) * length);

    // Draw branch
    for (int i = 0; i < length; i++) {
        int px = x + static_cast<int>(cos(angle) * i);
        int py = y + static_cast<int>(sin(angle) * i);
        
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
            int index = (py * WIDTH + px) * 3;
            // Brown color for branches
            image[index] = 101;     // B
            image[index + 1] = 67;  // G
            image[index + 2] = 33;  // R
        }
    }

    // Draw leaves at the end of smaller branches
    if (length < 30) {
        for (int i = -15; i < 15; i++) {
            for (int j = -15; j < 15; j++) {
                if (i*i + j*j > 225) continue;
                int px = endX + i;
                int py = endY + j;
                if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
                    int index = (py * WIDTH + px) * 3;
                    // Random autumn colors for leaves
                    double r = dis(gen);
                    if (r < 0.33) {
                        // Red
                        image[index] = 30;
                        image[index + 1] = 30;
                        image[index + 2] = 220;
                    } else if (r < 0.66) {
                        // Orange
                        image[index] = 30;
                        image[index + 1] = 140;
                        image[index + 2] = 255;
                    } else {
                        // Yellow
                        image[index] = 30;
                        image[index + 1] = 255;
                        image[index + 2] = 255;
                    }
                }
            }
        }
    }

    if (depth > 0) {
        drawBranch(image, endX, endY, angle - 0.5 - dis(gen) * 0.2, length * 0.7, depth - 1, gen, dis);
        drawBranch(image, endX, endY, angle + 0.5 + dis(gen) * 0.2, length * 0.7, depth - 1, gen, dis);
    }
}

int main() {
    const int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + WIDTH * HEIGHT * BYTES_PER_PIXEL;
    unsigned char* image = new unsigned char[WIDTH * HEIGHT * BYTES_PER_PIXEL];

    // Fill background with sky blue
    for (int i = 0; i < WIDTH * HEIGHT * 3; i += 3) {
        image[i] = 235;     // B
        image[i + 1] = 206; // G
        image[i + 2] = 135; // R
    }

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Draw the tree
    drawBranch(image, WIDTH/2, HEIGHT-100, -M_PI/2, 200, 12, gen, dis);

    // Create and write to file
    std::ofstream file("autumn_tree1.bmp", std::ios::binary);
    
    unsigned char* fileHeader = createBitmapFileHeader(fileSize);
    unsigned char* infoHeader = createBitmapInfoHeader();
    
    file.write(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE);
    file.write(reinterpret_cast<char*>(infoHeader), INFO_HEADER_SIZE);
    file.write(reinterpret_cast<char*>(image), WIDTH * HEIGHT * BYTES_PER_PIXEL);
    
    file.close();
    delete[] image;
    
    std::cout << "BMP file created successfully!" << std::endl;
    return 0;
}