#include "TextureLoader.hpp"

#include "GL/glew.h"
#include "lodepng.h"
using namespace std;

TextureLoader::TextureLoader() {
}

TextureLoader::~TextureLoader() {
}

GLuint TextureLoader::load(std::string filename) {
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);

    // Read the file into computers memory
    std::vector<unsigned char> image;  // Allocate a vector for storing the image
    unsigned width, height;            // Variables which will contain the image size
    // Read the image
    unsigned error = lodepng::decode(image, width, height, filename);

    // Import the image into graphics cards memory
    glGenTextures(1, &tex);             // Initialize one handle
    glBindTexture(GL_TEXTURE_2D, tex);  // Activate handle (bind it to the active texturing unit)
    // Import the image into the GC memory associated with the handle
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return tex;
}