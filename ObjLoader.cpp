#include "ObjLoader.hpp"

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

using namespace std;

ObjLoader::ObjLoader() {
}

ObjLoader::~ObjLoader() {
}

size_t ObjLoader::load(std::string filename, std::string path) {
    this->vertices.clear();
    this->normals.clear();
    this->texcoords.clear();

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = path;  // Path to material files
    reader_config.triangulate = false;
    tinyobj::ObjReader reader;

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
                this->vertices.push_back(vx);
                this->vertices.push_back(vy);
                this->vertices.push_back(vz);
                this->vertices.push_back(1.0f);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    this->normals.push_back(nx);
                    this->normals.push_back(ny);
                    this->normals.push_back(nz);
                    this->normals.push_back(0.0f);
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    this->texcoords.push_back(tx);
                    this->texcoords.push_back(ty);
                }
            }
            index_offset += fv;
        }
    }
    return shapes.size();
}

std::vector<float> ObjLoader::getVertices() {
    return this->vertices;
}

std::vector<float> ObjLoader::getNormals() {
    return this->normals;
}

std::vector<float> ObjLoader::getTextcoords() {
    return this->texcoords;
}
