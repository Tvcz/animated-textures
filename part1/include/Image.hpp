/** @file Image.hpp
 *  @brief Sets up an OpenGL camera..
 *
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>
#include <SDL2/SDL_timer.h>

// From Professor Shah's example code

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

struct Frame
{
    std::vector<uint8_t> data;
    int width;
    int height;
    int color_resolution;
    std::vector<Color> color_table;
    int delay_ms = 100;
    bool interlaced;
};

class Image
{
public:
    // Constructor for creating an image
    Image(std::string filepath);
    // Destructor
    ~Image();
    // Loads a PPM from file.
    void LoadPPM(bool flip);
    void flipData(uint8_t *data);
    // Loads a GIF from file.
    void LoadGIF(bool flip);
    // Return the width
    inline int GetWidth()
    {
        return m_width;
    }
    // Return the height
    inline int GetHeight()
    {
        return m_height;
    }
    // Bytes per pixel
    inline int GetBPP()
    {
        return m_BPP;
    }
    // Set a pixel a particular color in our data
    void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    // Display the pixels
    void PrintPixels();
    // Retrieve raw array of pixel data
    uint8_t *GetPixelDataPtr();
    // Returns the red component of a pixel
    inline unsigned int GetPixelR(int x, int y)
    {
        return m_pixelData[(x * 3) + m_height * (y * 3)];
    }
    // Returns the green component of a pixel
    inline unsigned int GetPixelG(int x, int y)
    {
        return m_pixelData[(x * 3) + m_height * (y * 3) + 1];
    }
    // Returns the blue component of a pixel
    inline unsigned int GetPixelB(int x, int y)
    {
        return m_pixelData[(x * 3) + m_height * (y * 3) + 2];
    }

private:
    void parseHeader(std::ifstream &stream);
    void parseLogicalScreenDescriptor(std::ifstream &stream);
    void parseGlobalColorTable(std::ifstream &stream);
    void parseLocalColorTable(std::ifstream &stream);
    void parseColorTable(std::ifstream &stream, std::vector<Color> &table, int color_resolution);
    void parseGraphicControlExtension(std::ifstream &stream);
    void parseImageDescriptor(std::ifstream &stream);
    void parseImageData(std::ifstream &stream);
    std::vector<uint8_t> mapIndexData(std::vector<uint8_t> data, std::vector<Color> color_table);
    std::vector<uint8_t> decompressLZW(std::vector<uint8_t> bytes, uint8_t lzw_min_code_size);
    void updateFrame();
    // std::vector<uint16_t> bytesToCodes(std::vector<uint8_t> bytes, uint8_t lzw_min_code_size);
    uint16_t getNextCode(std::vector<uint8_t> bytes, int &bit_index, int cur_code_size);
    // Filepath to the image loaded
    std::string m_filepath;
    // Raw pixel data
    uint8_t *m_pixelData;
    // Size and format of image
    int m_width{0};          // Width of the image
    int m_height{0};         // Height of the image
    int m_BPP{0};            // Bits per pixel (i.e. how colorful are our pixels)
    std::string magicNumber; // magicNumber if any for image format
    // for gif frames processing
    bool m_has_global_color_table;
    int m_global_color_resolution;
    std::vector<Color> m_global_color_table;
    int m_next_delay_ms = 100;
    std::vector<Frame> m_frames;
    bool m_next_has_local_color_table;
    u_int32_t m_last_refresh_time_ms = SDL_GetTicks();
    int m_cur_frame_index = 0;
};

#endif