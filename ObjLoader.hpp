#if !defined(OBJ_LOADER)
#define OBJ_LOADER

#include <stdlib.h>

#include <string>
#include <vector>

#include "GL/glew.h"

struct RGB {
    float r;
    float g;
    float b;
};

typedef struct RGB RGB;

struct TexName {
    bool available;
    std::string name;
};

typedef struct TexName TexName;

struct ObjModel {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;
    GLuint texture;
    bool textureAvailable;
    RGB ambient;
    RGB diffuse;
    RGB specular;
    float shininess;
};

typedef struct ObjModel ObjModel;

class ObjLoader {
   private:
    std::vector<ObjModel> models;

   public:
    ObjLoader();
    ~ObjLoader();
    size_t load(std::string filename, std::string path);
    std::vector<ObjModel> get();
};

#endif  // OBJ_LOADER
