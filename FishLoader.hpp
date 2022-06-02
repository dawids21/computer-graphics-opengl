#if !defined(FISH_LOADER)
#define FISH_LOADER

#include <vector>

#include "ObjLoader.hpp"
#include "TextureLoader.hpp"
#include "glm/glm.hpp"

enum FishType {
    BLUE = 0,
    BOESEMANI = 1,
    CORAL = 2,
    GOLD = 3,
    SOLON = 4,
    TURKUS = 5,
    YELLOW = 6
};

class FishLoader {
   private:
    std::vector<ObjModel> fish;

   public:
    FishLoader();
    ~FishLoader();
    void load();
    ObjModel getFish(FishType fishType);
};
#endif  // FISH_LOADER
