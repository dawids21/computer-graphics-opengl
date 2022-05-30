#if !defined(OBJ_LOADER)
#define OBJ_LOADER

#include <stdlib.h>
#include <string>

#include <vector>

class ObjLoader {
   private:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;

   public:
    ObjLoader();
    ~ObjLoader();
    void load(std::string filename, std::string path);
    std::vector<float> getVertices();
    std::vector<float> getNormals();
    std::vector<float> getTextcoords();
};

#endif  // OBJ_LOADER
