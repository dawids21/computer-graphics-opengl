#include "ObjLoader.hpp"

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION

#include "TextureLoader.hpp"
#include "tiny_obj_loader.h"

using namespace std;

ObjLoader::ObjLoader() {
}

ObjLoader::~ObjLoader() {
}

size_t ObjLoader::load(std::string filename, std::string path) {
    this->models.clear();

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = path;  // Path to material files
    reader_config.triangulate = false;
    tinyobj::ObjReader reader;

    TextureLoader textureLoader;

    if (!reader.ParseFromFile(filename, reader_config)) {
        if (!reader.Error().empty()) {
            cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        vector<float> vertices;
        vector<float> normals;
        vector<float> texcoords;
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);
                vertices.push_back(1.0f);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    normals.push_back(nx);
                    normals.push_back(ny);
                    normals.push_back(nz);
                    normals.push_back(0.0f);
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    texcoords.push_back(tx);
                    texcoords.push_back(ty);
                }
            }
            index_offset += fv;
        }
        ObjModel model;
        model.vertices = vertices;
        model.normals = normals;
        model.texcoords = texcoords;
        model.ambient = {materials[s].ambient[0], materials[s].ambient[1], materials[s].ambient[2]};
        model.diffuse = {materials[s].diffuse[0], materials[s].diffuse[1], materials[s].diffuse[2]};
        model.specular = {materials[s].specular[0], materials[s].specular[1], materials[s].specular[2]};
        if (materials[s].diffuse_texname == "") {
            model.texture = 0;
            model.textureAvailable = false;
        } else {
            model.texture = textureLoader.load(path + "/" + materials[s].diffuse_texname);
            model.textureAvailable = true;
        }
        model.shininess = materials[s].shininess;
        this->models.push_back(model);
    }
    return shapes.size();
}

std::vector<ObjModel> ObjLoader::get() {
    return this->models;
}
