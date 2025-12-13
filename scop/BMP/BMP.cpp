#include "BMP.hpp"

BMP::BMP(std::string path, std::string name) {
    missing = true;
    done = false;

    this->filename = path + name;
    if (check_file() == true)
        return;
    if (load_content() == false)
        return;
    load_pixels();
    done = true;
}

bool BMP::check_file() {
    std::ifstream file(filename);

    if (!file.good()) {
        missing = true;
        return true; 
    }
    file.close();
    return false;
}

bool BMP::load_content() {
    std::ifstream file(filename, std::ios::binary);

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    file.close();

    if (buffer.size() == 0 || (buffer[0] != 'B' || buffer[1] != 'M')) {
        missing = true;
        return false;
    }
    missing = false;
    uint32_t dataOffset = 
        static_cast<unsigned char>(buffer[10]) |
        (static_cast<unsigned char>(buffer[11]) << 8) |
        (static_cast<unsigned char>(buffer[12]) << 16) |
        (static_cast<unsigned char>(buffer[13]) << 24);
    int32_t bmpWidth = 
        static_cast<unsigned char>(buffer[18]) |
        (static_cast<unsigned char>(buffer[19]) << 8) |
        (static_cast<unsigned char>(buffer[20]) << 16) |
        (static_cast<unsigned char>(buffer[21]) << 24);
    int32_t bmpHeight = 
        static_cast<unsigned char>(buffer[22]) |
        (static_cast<unsigned char>(buffer[23]) << 8) |
        (static_cast<unsigned char>(buffer[24]) << 16) |
        (static_cast<unsigned char>(buffer[25]) << 24);
    uint16_t bitsPerPixel = 
        static_cast<unsigned char>(buffer[28]) |
        (static_cast<unsigned char>(buffer[29]) << 8);

    lsb = buffer[10];
    msb = buffer[13];
    width = bmpWidth;
    height = bmpHeight;
    this->bits_per_pixel = bitsPerPixel;
    offset = dataOffset;
    return true;
}

void BMP::load_pixels() {
    std::ifstream file(filename, std::ios::binary);

    file.seekg(offset, std::ios::beg);
    int row_size = ((width * bits_per_pixel + 31) / 32) * 4;
    int image_size = row_size * height;
    
    pixel_data.resize(image_size);
    file.read(reinterpret_cast<char*>(pixel_data.data()), image_size);
    std::vector<unsigned char> flippedData(image_size);

    for (int y = 0; y < height; y++) {
        int src_y = height - 1 - y;
        std::memcpy(&flippedData[y * row_size], &pixel_data[src_y * row_size], row_size);
    }
    pixel_data = std::move(flippedData);
    file.close();
}

bool BMP::is_missing() {
    return missing;
}

std::string BMP::get_name() {
    return filename;
}

int BMP::get_bits_per_pixel() {
    return bits_per_pixel;
}

int BMP::get_offset() {
    return offset;
}

int BMP::get_width() {
    return width;
}

int BMP::get_height() {
    return height;
}

int BMP::get_lsb() {
    return lsb;
}

int BMP::get_msb() {
    return msb;
}

const std::vector<unsigned char>& BMP::get_pixels() const {
    return pixel_data;
}
