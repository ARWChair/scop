#include <exception>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#pragma once

class BMP {
    public:
        BMP(std::string path, std::string name);

        std::string get_name();
        bool is_missing();
        int get_bits_per_pixel();
        int get_offset();
        int get_height();
        int get_width();
        int get_lsb();
        int get_msb();
        const std::vector<unsigned char>& get_pixels() const;
    private:
        void load_pixels();
        bool check_file();
        bool load_content();
        std::string filename;
        bool missing, done;
        int width, height, lsb, msb, offset, bits_per_pixel;
        std::vector<unsigned char> pixel_data;
};