#if !defined(OBJ_LOADER)
#define OBJ_LOADER

#include <stdlib.h>

#include <string>
#include <vector>

class ObjLoader {
   private:
    std::vector<float> vertices;
    std::vector<float> normals;

   public:
    ObjLoader();
    ~ObjLoader();
    void load(std::string filename);
    std::vector<float> getVertices();
    std::vector<float> getNormals();
};

#endif  // OBJ_LOADER
