#if !defined(TEXUTRE_LOADER_HPP)
#define TEXUTRE_LOADER_HPP

#include <string>
#include <vector>

#include "GL/glew.h"

class TextureLoader {
   private:
    std::vector<GLuint> textures;

   public:
    TextureLoader();
    ~TextureLoader();
    int loadTexture(std::string filename);
    GLuint getTexture(int id);
};

#endif  // TEXUTRE_LOADER_HPP
