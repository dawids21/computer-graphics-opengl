#if !defined(TEXUTRE_LOADER_HPP)
#define TEXUTRE_LOADER_HPP

#include <string>
#include <vector>

#include "GL/glew.h"

class TextureLoader {
   private:
   public:
    TextureLoader();
    ~TextureLoader();
    GLuint load(std::string filename);
};

#endif  // TEXUTRE_LOADER_HPP
