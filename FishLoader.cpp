#include "FishLoader.hpp"

#include <string>

#include "ObjLoader.hpp"

using namespace std;

struct FishInfo {
    string objFilename;
    string texFilename;
    string path;
};

FishLoader::FishLoader(TextureLoader textureLoader) {
    vector<struct FishInfo> info = {{"13006_Blue_Tang_v1_l3.obj", "13006_Blue_Tang_v1_diff.png", "./models/fish/blue"}};

    ObjLoader objLoader;
    FishType types[] = {BLUE};
    for (size_t i = 0; i < sizeof(types) / sizeof(FishType); i++) {
        FishType type = types[i];
        objLoader.load(info[type].path + "/" + info[type].objFilename, info[type].path);
        Fish fish = {objLoader.getVertices(),
                     objLoader.getNormals(),
                     objLoader.getTextcoords(),
                     textureLoader.loadTexture(info[type].path + "/" + info[type].texFilename)};
        this->fish.push_back(fish);
    }
}

FishLoader::~FishLoader() {
}

Fish FishLoader::getFish(FishType fishType) {
    return this->fish[fishType];
}

glm::mat4 FishLoader::getMatrix(FishType fishType, glm::mat4 matrix) {
    return glm::mat4(1.0f);
};