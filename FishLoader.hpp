#if !defined(FISH_LOADER)
#define FISH_LOADER

#include <vector>

#include "TextureLoader.hpp"
#include "glm/glm.hpp"

struct Fish {
    std::vector<float> vertex;
    std::vector<float> normal;
    std::vector<float> texCoord;
    int textureId;
};

typedef struct Fish Fish;

enum FishType {
    BLUE = 0
};

class FishLoader {
   private:
    std::vector<Fish> fish;

   public:
    FishLoader(TextureLoader textureLoader);
    ~FishLoader();
    Fish getFish(FishType fishType);
    glm::mat4 getMatrix(FishType fishType, glm::mat4 matrix);
};
#endif  // FISH_LOADER
