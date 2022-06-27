#ifndef MYCUBE_H_INCLUDED
#define MYCUBE_H_INCLUDED

int myCubeVertexCount = 6;

float myCubeVertices[] = {
    // Wall 4
    -1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, -1.0f, 1.0f};

float myCubeTexCoords[] = {
    5.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f,
    5.0f, 0.0f, 5.0f, 5.0f, 0.0f, 5.0f};

float myCubeNormals[] = {

    // Wall 4
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f

};

#endif // MYCUBE_H_INCLUDED
