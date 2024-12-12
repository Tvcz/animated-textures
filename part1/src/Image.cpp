#include "Image.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <memory>
#include <vector>
#include <bitset>

// From Professor Shah's example code

// Constructor
Image::Image(std::string filepath) : m_filepath(filepath)
{
}

// Destructor
Image::~Image()
{
    // Delete our pixel data.
    // Note: We could actually do this sooner
    // in our rendering process.
    if (m_pixelData != NULL)
    {
        delete[] m_pixelData;
    }
}

// Little function for loading the pixel data
// from a PPM image.
// TODO: Expects a very specific version of PPM!
//
// flip - Will flip the pixels upside down in the data
//        If you use this be consistent.
void Image::LoadPPM(bool flip)
{

    // Open an input file stream for reading a file
    std::ifstream ppmFile(m_filepath.c_str());
    // If our file successfully opens, begin to process it.
    if (ppmFile.is_open())
    {
        // line will store one line of input
        std::string line;
        // Our loop invariant is to continue reading input until
        // we reach the end of the file and it reads in a NULL character
        std::cout << "Reading in ppm file: " << m_filepath << std::endl;
        unsigned int iteration = 0;
        unsigned int pos = 0;
        while (getline(ppmFile, line))
        {
            // Ignore comments in the file
            if (line[0] == '#')
            {
                continue;
            }
            if (line[0] == 'P')
            {
                magicNumber = line;
            }
            else if (iteration == 1)
            {
                // Returns first token
                char *token = strtok((char *)line.c_str(), " ");
                m_width = atoi(token);
                token = strtok(NULL, " ");
                m_height = atoi(token);
                std::cout << "PPM width,height=" << m_width << "," << m_height << "\n";
                if (m_width > 0 && m_height > 0)
                {
                    m_pixelData = new uint8_t[m_width * m_height * 3];
                    if (m_pixelData == NULL)
                    {
                        std::cout << "Unable to allocate memory for ppm" << std::endl;
                        exit(1);
                    }
                }
                else
                {
                    std::cout << "PPM not parsed correctly, width and/or height dimensions are 0" << std::endl;
                    exit(1);
                }
            }
            else if (iteration == 2)
            {
                // max color range is stored here
                // TODO: Can be stored optionally
            }
            else
            {
                m_pixelData[pos] = (uint8_t)atoi(line.c_str());
                ++pos;
            }
            iteration++;
        }
        ppmFile.close();
    }
    else
    {
        std::cout << "Unable to open ppm file:" << m_filepath << std::endl;
    }

    // Flip all of the pixels
    if (flip)
    {
        flipData(m_pixelData);
    }
}

void Image::flipData(uint8_t *data)
{
    // Copy all of the data to a temporary stack-allocated array
    uint8_t *copyData = new uint8_t[m_width * m_height * 3];
    for (int i = 0; i < m_width * m_height * 3; ++i)
    {
        copyData[i] = data[i];
    }
    unsigned int pos = (m_width * m_height * 3) - 1;
    for (int i = 0; i < m_width * m_height * 3; i += 3)
    {
        data[pos] = copyData[i + 2];
        data[pos - 1] = copyData[i + 1];
        data[pos - 2] = copyData[i];
        pos -= 3;
    }
    delete[] copyData;
}

enum class GifState
{
    HEADER,
    LOGICAL_SCREEN_DESCRIPTOR,
    GLOBAL_COLOR_TABLE,
    CONTENT_BLOCK,
    IMAGE_DESCRIPTOR,
    LOCAL_COLOR_TABLE,
    IMAGE_DATA,
    EXTENSION_BLOCK,
    GRAPHIC_CONTROL_EXTENSION,
    PLAIN_TEXT_OR_APPLICATION_EXTENSION,
    COMMENT_EXTENSION,
    TRAILER
};

void Image::LoadGIF(bool flip)
{
    // Open an binary input file stream for reading a file
    std::ifstream file(m_filepath.c_str(), std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open GIF.\n";
        return;
    }

    GifState state = GifState::HEADER;

    // state machine based off diagram at https://giflib.sourceforge.net/whatsinagif/gif_file_stream.gif
    while (!file.eof())
    {
        switch (state)
        {
        case GifState::HEADER:
            std::cout << "Attempting to read GIF header.\n";
            parseHeader(file);
            std::cout << "Successfully read GIF header.\n";
            state = GifState::LOGICAL_SCREEN_DESCRIPTOR;
            break;
        case GifState::LOGICAL_SCREEN_DESCRIPTOR:
            std::cout << "Attempting to read logical screen descriptor.\n";
            parseLogicalScreenDescriptor(file);
            std::cout << "Successfully read logical screen descriptor.\n";
            if (m_has_global_color_table)
            {
                state = GifState::GLOBAL_COLOR_TABLE;
            }
            else
            {
                state = GifState::CONTENT_BLOCK;
            }
            break;
        case GifState::GLOBAL_COLOR_TABLE:
            std::cout << "Attempting to read global color table.\n";
            parseGlobalColorTable(file);
            std::cout << "Successfully read global color table.\n";
            state = GifState::CONTENT_BLOCK;
        case GifState::CONTENT_BLOCK:
            std::cout << "Attempting to read content block.\n";
            if (file.peek() == 0x21)
            {
                state = GifState::EXTENSION_BLOCK;
            }
            else if (file.peek() == 0x2C)
            {
                state = GifState::IMAGE_DESCRIPTOR;
            }
            else if (file.peek() == 0x3B)
            {
                state = GifState::TRAILER;
            }
            else
            {
                std::cerr << "Encountered unexpected byte in content block start " << file.peek() << "\n";
                exit(1);
            }
            std::cout << "Successfully read content block.\n";
            break;
        case GifState::IMAGE_DESCRIPTOR:
            std::cout << "Attempting to read image descriptor.\n";
            parseImageDescriptor(file);
            std::cout << "Successfully read image descriptor.\n";
            if (m_next_has_local_color_table)
            {
                state = GifState::LOCAL_COLOR_TABLE;
            }
            else
            {
                state = GifState::IMAGE_DATA;
            }
            break;
        case GifState::LOCAL_COLOR_TABLE:
            std::cout << "Attempting to read local color table.\n";
            parseLocalColorTable(file);
            std::cout << "Successfully read local color table.\n";
            state = GifState::IMAGE_DATA;
            break;
        case GifState::IMAGE_DATA:
            std::cout << "Attempting to read image data.\n";
            parseImageData(file);
            std::cout << "Successfully read image data.\n";
            state = GifState::CONTENT_BLOCK;
            break;
        case GifState::EXTENSION_BLOCK:
            std::cout << "Attempting to read extension block.\n";
            char extension_type[2];
            file.read(extension_type, 1);
            if (extension_type[1] == 0xF9)
            {
                state = GifState::GRAPHIC_CONTROL_EXTENSION;
            }
            else if (extension_type[1] == 0x01 || extension_type[1] == 0xFF)
            {
                state = GifState::PLAIN_TEXT_OR_APPLICATION_EXTENSION;
            }
            else
            {
                state = GifState::COMMENT_EXTENSION;
            }
            std::cout << "Successfully read extension block.\n";
            break;
        case GifState::GRAPHIC_CONTROL_EXTENSION:
            std::cout << "Attempting to read graphic control extension.\n";
            parseGraphicControlExtension(file);
            std::cout << "Successfully read graphic control extension.\n";
            state = GifState::CONTENT_BLOCK;
            break;
        case GifState::PLAIN_TEXT_OR_APPLICATION_EXTENSION:
            std::cout << "Attempting to read plain text or application extension.\n";
            // no useful information in these blocks
            char block_size;
            file.read(&block_size, 1);
            file.ignore(block_size);
            state = GifState::CONTENT_BLOCK;
            std::cout << "Successfully read plain text or application extension.\n";
            break;
        case GifState::COMMENT_EXTENSION:
            std::cout << "Attempting to read comment extension.\n";
            // no useful information in this block but does not have an explicit
            // block size so we need to skip subblocks until we reach a subblock
            // of size (see https://giflib.sourceforge.net/whatsinagif/comment_ext.gif)
            char sub_block_size;
            file.read(&sub_block_size, 1);
            while (sub_block_size != 0x00)
            {
                file.ignore(sub_block_size);
                file.read(&sub_block_size, 1);
            }
            state = GifState::CONTENT_BLOCK;
            std::cout << "Successfully read comment extension.\n";
            break;
        case GifState::TRAILER:
            std::cout << "Attempting to read GIF trailer.\n";
            char marker;
            file.read(&marker, 1);
            if (marker != 0x3B)
            {
                std::cerr << "Invalid GIF trailer.\n";
            }
            if (file.peek() != EOF)
            {
                std::cerr << "Trailer not at end of file.\n";
            }
            std::cout << "Successfully read GIF trailer.\n";
            break;
        }
    }

    file.close();

    // Flip all of the pixels
    if (flip)
    {
        for (Frame &frame : m_frames)
        {
            flipData(frame.data.data());
        }
    }

    return;
}

uint16_t littleEndianToBigEndian(uint8_t value1, uint8_t value2)
{
    return (value2 << 8) | value1;
}

void Image::parseHeader(std::ifstream &stream)
{
    char header[6];
    stream.read(header, 6);
    if (strncmp(header, "GIF87a", 6) != 0 && strncmp(header, "GIF89a", 6) != 0)
    {
        std::cerr << "Invalid GIF header.\n";
        return;
    }
}

void Image::parseLogicalScreenDescriptor(std::ifstream &stream)
{
    char descriptor[7];
    stream.read(descriptor, 7);
    m_width = littleEndianToBigEndian(descriptor[0], descriptor[1]);
    m_height = littleEndianToBigEndian(descriptor[2], descriptor[3]);
    std::cout << "GIF width,height=" << m_width << "," << m_height << "\n";
    uint8_t packed_field = descriptor[4];
    m_has_global_color_table = packed_field & 0b10000000;
    m_global_color_resolution = (packed_field & 0b00000111);
    std::cout << "Global color resolution " << m_global_color_resolution << "\n";
}

// https://giflib.sourceforge.net/whatsinagif/global_color_table.gif
void Image::parseGlobalColorTable(std::ifstream &stream)
{
    parseColorTable(stream, m_global_color_table, m_global_color_resolution);
}

void Image::parseLocalColorTable(std::ifstream &stream)
{
    Frame last_frame = m_frames.back();
    parseColorTable(stream, last_frame.color_table, last_frame.color_resolution);
}

void Image::parseColorTable(std::ifstream &stream, std::vector<Color> &table, int color_resolution)
{
    int numColors = 1 << (color_resolution + 1);
    for (int i = 0; i < numColors; i++)
    {
        uint8_t color[3];
        stream.read((char *)color, 3);
        table.push_back(Color(color[0], color[1], color[2]));
    }
}

// https://giflib.sourceforge.net/whatsinagif/graphic_control_ext.gif
void Image::parseGraphicControlExtension(std::ifstream &stream)
{
    // skip block size
    stream.ignore(1);
    char block[4];
    stream.read(block, 4);
    m_next_delay_ms = littleEndianToBigEndian(block[1], block[2]) * 10;
}

// https://giflib.sourceforge.net/whatsinagif/image_descriptor_block.gif
void Image::parseImageDescriptor(std::ifstream &stream)
{
    char descriptor_block[10];
    stream.read(descriptor_block, 10);
    Frame frame;
    frame.width = littleEndianToBigEndian(descriptor_block[5], descriptor_block[6]);
    frame.height = littleEndianToBigEndian(descriptor_block[7], descriptor_block[8]);
    uint8_t packed_field = descriptor_block[9];
    m_next_has_local_color_table = packed_field & 0b10000000;
    if (m_next_has_local_color_table)
    {
        frame.color_resolution = packed_field & 0b00000111;
    }
    else
    {
        frame.color_table = m_global_color_table;
        frame.color_resolution = m_global_color_resolution;
    }
    frame.interlaced = packed_field & 0b01000000;
    frame.delay_ms = m_next_delay_ms;
    m_frames.push_back(frame);
}

void Image::parseImageData(std::ifstream &stream)
{
    u_int8_t lzw_min_code_size;
    stream.read(reinterpret_cast<char *>(&lzw_min_code_size), 1);
    uint8_t sub_block_size;
    stream.read(reinterpret_cast<char *>(&sub_block_size), 1);
    std::vector<uint8_t> lzw_data;
    while (sub_block_size != 0x00)
    {
        // std::cout << "Read subblock of size " << (int)sub_block_size << "\n";
        char *block = new char[sub_block_size];
        stream.read(block, sub_block_size);
        for (int i = 0; i < sub_block_size; i++)
        {
            lzw_data.push_back(block[i]);
        }
        delete[] block;
        stream.read(reinterpret_cast<char *>(&sub_block_size), 1);
    }
    std::cout << "Encountered block terminator: " << (int)sub_block_size << "\n";
    std::cout << "LZW min code size: " << (int)lzw_min_code_size << "\n";
    std::cout << "LZW data size: " << lzw_data.size() << " bytes\n";
    // std::vector<uint16_t> codes = bytesToCodes(lzw_data, lzw_min_code_size);
    // std::cout << "Reinterpreted compressed LZW data, number of codes: " << codes.size() << " codes\n";
    std::vector<uint8_t> decompressed_data = decompressLZW(lzw_data, lzw_min_code_size);
    std::cout << "Decompressed data, result size: " << decompressed_data.size() << " color indices\n";
    std::vector<uint8_t> rawColorData = mapIndexData(decompressed_data, m_frames.back().color_table);
    std::cout << "Translated color index data to raw color data\n";
    std::cout << "Raw color data size: " << rawColorData.size() << " bytes\n";
    Frame &last_frame = m_frames.back();
    last_frame.data = rawColorData;
    if (last_frame.interlaced)
    {
        std::cerr << "Interlaced image, not implemented\n";
    }
}

std::vector<uint8_t> Image::mapIndexData(std::vector<uint8_t> data, std::vector<Color> color_table)
{
    std::vector<uint8_t> mapped_data;
    for (int i = 0; i < data.size(); i++)
    {
        Color color = color_table[data[i]];
        mapped_data.push_back(color.r);
        mapped_data.push_back(color.g);
        mapped_data.push_back(color.b);
    }
    return mapped_data;
}

// collects codes from bytes and decompresses them to color indices
// https://giflib.sourceforge.net/whatsinagif/lzw_decoding_bytes.gif
std::vector<uint8_t> Image::decompressLZW(std::vector<uint8_t> bytes, uint8_t lzw_min_code_size)
{

    int cur_code_size = lzw_min_code_size + 1;
    const int max_code_size = 12;
    int bit_index = 0;
    uint16_t clear_code = 1 << lzw_min_code_size;
    uint16_t end_of_info_code = clear_code + 1;

    std::vector<uint8_t> decompressed_data;

    std::cout << "Initializing code table\n";
    const uint16_t max_table_size = 1 << 12; // 12-bit maximum for GIF LZW
    std::vector<std::vector<uint8_t>> code_table((1 << lzw_min_code_size) + 2, std::vector<uint8_t>());
    for (int i = 0; i < (1 << lzw_min_code_size); ++i)
    {
        code_table[i] = {static_cast<uint8_t>(i)};
    }

    std::cout << "Clear code: " << clear_code << ", End of info code: " << end_of_info_code << "\n";

    std::cout << "Decompressing LZW data" << std::endl;
    uint16_t read_clear_code = getNextCode(bytes, bit_index, cur_code_size);
    if (read_clear_code != clear_code)
    {
        throw std::runtime_error("Read clear code does not match expected clear code");
    }
    uint16_t prev_code = getNextCode(bytes, bit_index, cur_code_size);
    if (prev_code >= code_table.size())
    {
        throw std::runtime_error("First code out of range");
    }
    // std::cout << prev_code << " ";
    decompressed_data.insert(decompressed_data.end(), code_table[prev_code].begin(), code_table[prev_code].end());

    bool just_reset = false;
    while (bit_index < bytes.size() * 8)
    {
        uint16_t curr_code = getNextCode(bytes, bit_index, cur_code_size);
        // std::cout << curr_code << " ";

        // std::cout << "Processing code: " << curr_code << ", size of decompressed_data: " << decompressed_data.size() << std::endl;

        if (curr_code == clear_code)
        {
            // std::cout << "Clear code encountered, resetting code table\n";
            code_table.clear();
            code_table.resize((1 << lzw_min_code_size) + 2, std::vector<uint8_t>());
            for (int i = 0; i < (1 << lzw_min_code_size); ++i)
            {
                code_table[i] = {static_cast<uint8_t>(i)};
            }
            cur_code_size = lzw_min_code_size + 1;
            prev_code = getNextCode(bytes, bit_index, cur_code_size);
            decompressed_data.insert(decompressed_data.end(), code_table[prev_code].begin(), code_table[prev_code].end());
            continue;
        }
        else if (curr_code == end_of_info_code)
        {
            // std::cout << "End of info code encountered, stopping decompression\n";
            break;
        }

        if (curr_code < code_table.size())
        {
            // std::cout << "Code " << curr_code << " in table, adding to decompressed data\n";
            decompressed_data.insert(decompressed_data.end(), code_table[curr_code].begin(), code_table[curr_code].end());
            int k = code_table[curr_code][0];
            std::vector<uint8_t> new_entry = code_table[prev_code];
            new_entry.push_back(k);
            code_table.push_back(new_entry);
            // std::cout << "Added code " << code_table.size() - 1 << " to table\n";
        }
        else
        {
            // std::cout << "Code " << curr_code << " not in table, adding to table\n";
            int k = code_table[prev_code][0];
            std::vector<uint8_t> new_entry = code_table[prev_code];
            new_entry.push_back(k);
            code_table.push_back(new_entry);
            decompressed_data.insert(decompressed_data.end(), new_entry.begin(), new_entry.end());
            // std::cout << "Added code " << code_table.size() - 1 << " to table\n";
        }

        if (code_table.size() == (2 << (cur_code_size - 1)) && cur_code_size < max_code_size)
        {
            // std::cout << "Code size increased\n";
            cur_code_size++;
        }

        prev_code = curr_code;
    }
    std::cout << "Total codes: " << code_table.size() << std::endl;

    return decompressed_data;
}

uint16_t Image::getNextCode(std::vector<uint8_t> bytes, int &bit_index, int cur_code_size)
{
    int byte_index, bit_byte_index;
    uint16_t code = 0;
    int bit_pos = 0;
    for (int i = 0; i < cur_code_size; i++)
    {
        byte_index = bit_index / 8;
        bit_byte_index = bit_index % 8;
        uint8_t bit = (bytes[byte_index] >> bit_byte_index) & 0b00000001;
        code = code | (bit << bit_pos);
        bit_pos++;
        // also increase the bit_index var!!
        bit_index++;
    }
    if (code > 4095)
    {
        throw std::runtime_error("Invalid code size encountered: " + std::to_string(code));
    }
    return code;
}

void Image::updateFrame()
{
    if (SDL_GetTicks() - m_last_refresh_time_ms > m_frames[m_cur_frame_index].delay_ms)
    {
        m_cur_frame_index = (m_cur_frame_index + 1) % m_frames.size();
        m_last_refresh_time_ms = SDL_GetTicks();
    }
}

/*  ===============================================
Desc: Sets a pixel in our array a specific color
Precondition:
Post-condition:
=============================================== */
void Image::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x > m_width || y > m_height)
    {
        return;
    }
    else
    {
        /*std::cout << "modifying pixel at "
                  << x << "," << y << "from (" <<
                  (int)color[x*y] << "," << (int)color[x*y+1] << "," <<
    (int)color[x*y+2] << ")";*/
        m_pixelData[(x * 3) + m_height * (y * 3)] = r;
        m_pixelData[(x * 3) + m_height * (y * 3) + 1] = g;
        m_pixelData[(x * 3) + m_height * (y * 3) + 2] = b;
        /*    std::cout << " to (" << (int)color[x*y] << "," << (int)color[x*y+1] << ","
        << (int)color[x*y+2] << ")" << std::endl;*/
    }
}

/*  ===============================================
Desc:
Precondition:
Post-condition:
=============================================== */
void Image::PrintPixels()
{
    for (int x = 0; x < m_width * m_height * 3; ++x)
    {
        std::cout << " " << (int)m_pixelData[x];
    }
    std::cout << "\n";
}

/*  ===============================================
Desc: Returns pixel data for our image
Precondition:
Post-condition:
=============================================== */
uint8_t *Image::GetPixelDataPtr()
{
    if (m_filepath.substr(m_filepath.find_last_of(".") + 1) == "ppm")
    {
        std::cout << "Returning pixel data for a PPM image" << std::endl;
        return m_pixelData;
    }
    else if (m_filepath.substr(m_filepath.find_last_of(".") + 1) == "gif")
    {
        std::cout << "Returning pixel data for a GIF" << std::endl;
        // for (int i = 0; i < last_frame.width * last_frame.height; i++)
        // {
        //     int r = last_frame.data[i * 3];
        //     int g = last_frame.data[i * 3 + 1];
        //     int b = last_frame.data[i * 3 + 2];
        //     std::cout << r << " " << g << " " << b << std::endl;
        // }
        updateFrame();
        std::cout << "Current frame index: " << m_cur_frame_index << std::endl;
        return m_frames[m_cur_frame_index].data.data();
    }
    else
    {
        std::cout << "Unsupported file type" << std::endl;
        return nullptr;
    }
}