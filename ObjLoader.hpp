#if !defined(OBJ_LOADER)
#define OBJ_LOADER

#include <stdlib.h>
#include <string>

#include <vector>

class ObjLoader {
   private:
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<float>> texcoords;

   public:
    ObjLoader();
    ~ObjLoader();
    size_t load(std::string filename, std::string path);
    std::vector<float> getVertices(size_t i);
    std::vector<float> getNormals(size_t i);
    std::vector<float> getTextcoords(size_t i);
};

#endif  // OBJ_LOADER
