#include "FishLoader.hpp"

#include <iostream>
#include <string>

#include "ObjLoader.hpp"

using namespace std;

struct FishInfo {
    string objFilename;
    string texFilename;
    string path;
};

FishLoader::FishLoader() {
}

FishLoader::~FishLoader() {
}

void FishLoader::load(TextureLoader *textureLoader) {
    vector<struct FishInfo> info;
    info.push_back({"13006_Blue_Tang_v1_l3.obj", "13006_Blue_Tang_v1_diff.png", "./models/fish/blue"});
    info.push_back({"12999_Boesemani_Rainbow_v1_l2.obj", "12999_Boesemani_Rainbow_diff.png", "./models/fish/boesemani"});
    info.push_back({"13009_Coral_Beauty_Angelfish_v1_l3.obj", "13009_Coral_Beauty_Angelfish_v1_diff.png", "./models/fish/coral"});
    info.push_back({"12990_Black_Moor_Goldfish_v1_l2.obj", "12990_Black_Moor_Goldfish_diff.png", "./models/fish/gold"});
    info.push_back({"13013_Red_Head_Solon_Fairy_Wrasse_v1_l3.obj", "13013_Red-Head-Solon_diffuse.png", "./models/fish/solon"});
    info.push_back({"13007_Blue-Green_Reef_Chromis_v2_l3.obj", "13004_Bicolor_Blenny_v1_diff.png", "./models/fish/turkus"});
    info.push_back({"fish_yellow.obj", "fish_yellow.png", "./models/fish/yellow"});

    ObjLoader objLoader;
    FishType types[] = {BLUE, BOESEMANI, CORAL, GOLD, SOLON, TURKUS, YELLOW};
    for (size_t i = 0; i < size(types); i++) {
        FishType type = types[i];
        objLoader.load(info[type].path + "/" + info[type].objFilename, info[type].path);
        Fish fish = {objLoader.getVertices(0),
                     objLoader.getNormals(0),
                     objLoader.getTextcoords(0),
                     textureLoader->loadTexture(info[type].path + "/" + info[type].texFilename)};
        this->fish.push_back(fish);
    }
}

Fish FishLoader::getFish(FishType fishType) {
    return this->fish[fishType];
}