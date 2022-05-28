#if !defined(TEXUTRE_LOADER_HPP)
#define TEXUTRE_LOADER_HPP

#include <vector>

#include "GL/glew.h"

class TextureLoader {
   private:
    std::vector<GLuint> textures;

   public:
    TextureLoader();
    ~TextureLoader();
    int loadTexture(const char* filename);
    GLuint getTexture(int id);
};

#endif  // TEXUTRE_LOADER_HPP
