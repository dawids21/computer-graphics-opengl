/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <cmath>
#include <string>

#include "FishAnimator.hpp"
#include "FishLoader.hpp"
#include "ObjLoader.hpp"
#include "TextureLoader.hpp"
#include "allmodels.h"
#include "config.hpp"
#include "constants.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"

using namespace glm;



float speed = 0;  //[radians/s]

// camera movement
float speed_y = 0; //[radiany/s]
float speed_x = 0; //[radiany/s]
float ws = 0;


GLuint tex;

vec3 pos = vec3(0, C_PERSON_HEIGHT, -5);
vec3 pos_prev = vec3(0, C_PERSON_HEIGHT, -5);
vec3 dir = vec3(0, 0, 1);

FishLoader fishLoader;
FishAnimator fishAnimator;

std::vector<ObjModel> aquariumModel;
std::vector<ObjModel> tableModel;


// Error processing callback procedure
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

// Procedura obs�ugi klawiatury
void key_callback(GLFWwindow* window, int key,
                  int scancode, int action, int mod)
 {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_y = 1;
		if (key == GLFW_KEY_RIGHT) speed_y = -1;
		if (key == GLFW_KEY_PAGE_UP) speed_x = 1;
		if (key == GLFW_KEY_PAGE_DOWN) speed_x = -1;
		if (key == GLFW_KEY_UP) ws = 1;
		if (key == GLFW_KEY_DOWN) ws= -1;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_y = 0;
		if (key == GLFW_KEY_RIGHT) speed_y = 0;	

		if (key == GLFW_KEY_UP) ws = 0;
		if (key == GLFW_KEY_DOWN) ws = 0;
		
		if (key == GLFW_KEY_PAGE_UP) speed_x = 0;
		if (key == GLFW_KEY_PAGE_DOWN) speed_x = -0;
	}
}
GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Read into computers memory
	std::vector<unsigned char> image;   //Allocate memory 
	unsigned width, height;   //Variables for image size
	//Read the image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import to graphics card memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate handle
	//Copy image to graphics cards memory reprezented by the active handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_MIN_FILTER,
				GL_NEAREST_MIPMAP_LINEAR);


	return tex;
}

// Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
    //************Place any code here that needs to be executed once, at the program start************
    glClearColor(0, 0, 0, 1);  // Set color buffer clear color
    glEnable(GL_DEPTH_TEST);   // Turn on pixel depth test based on depth buffer
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    fishLoader.load();
    ObjLoader objLoader;
    objLoader.load("./models/aquarium/12986_Freshwater_Aquarium_v1_l2.obj", "./models/aquarium");
    aquariumModel = objLoader.get();
    std::sort(aquariumModel.begin(), aquariumModel.end());
    objLoader.load("./models/table/table.obj", "./models/table");
    tableModel = objLoader.get();
    glDisable(GL_CULL_FACE);
    glfwSetKeyCallback(window, key_callback);
    tex = readTexture("./models/floor/floor.png");
}

// Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Place any code here that needs to be executed once, after the main loop ends************
}


void activateConstantShader() {
    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
    glm::mat4 V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));

    spConstant->use();
    glUniformMatrix4fv(spConstant->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(spConstant->u("V"), 1, false, glm::value_ptr(V));
}


void activateLambertShader() {
    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
    glm::mat4 V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));

    spLambert->use();
    glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));
}

void activateLambertTexturedShader() {
    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
    glm::mat4 V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));

    spLambertTextured->use();
    glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
}

void activateTexturedShader() {
    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
    glm::mat4 V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));

    spTextured->use();
    glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));
}

void activateSimplestShader() {
    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
    glm::mat4 V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));

    spSimplest->use();
    glUniformMatrix4fv(spSimplest->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(spSimplest->u("V"), 1, false, glm::value_ptr(V));
}

void activateSimplestTexturedShader() {
    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
    glm::mat4 V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));

    spSimplestTextured->use();
    glUniformMatrix4fv(spSimplestTextured->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(spSimplestTextured->u("V"), 1, false, glm::value_ptr(V));
}

mat4 floor(glm::mat4 initMatrix) {
    // activateConstantShader();

    // mat4 floorMatrix = translate(initMatrix, vec3(0, 0, 0));
    // mat4 scaledFloorMatrix = scale(floorMatrix, vec3(C_ROOM_SIZE, 0.1f, C_ROOM_SIZE));
    // glUniform4f(spConstant->u("color"), 0.5, 0.5, 0.5, 1);
    // glUniformMatrix4fv(spConstant->u("M"), 1, false, value_ptr(scaledFloorMatrix));
    // Models::cube.drawSolid();

    activateTexturedShader();

    mat4 floorMatrix = translate(initMatrix, vec3(0, 0, 0));
    mat4 scaledFloorMatrix = scale(floorMatrix, vec3(C_ROOM_SIZE, 0.1f, C_ROOM_SIZE));

    glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(scaledFloorMatrix));

    glEnableVertexAttribArray(spTextured->a("vertex"));
    glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);

    glEnableVertexAttribArray(spTextured->a("texCoord"));
    glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(spTextured->u("tex"), 0);

    glDrawArrays(GL_TRIANGLES, 18, 6);

    glDisableVertexAttribArray(spTextured->a("vertex"));
    glDisableVertexAttribArray(spTextured->a("texCoord"));

    return floorMatrix;
}

void walls(glm:: mat4 initMatrix){

    activateConstantShader();

    mat4 wallMatrix1 = translate(initMatrix, vec3(0, 0, C_ROOM_SIZE));
    wallMatrix1 = scale(wallMatrix1, vec3(C_ROOM_SIZE, 5.0f, 0.1f));
    glUniform4f(spLambert->u("color"), 0.2, 0.6, 0.4, 1);
    glUniformMatrix4fv(spLambert->u("M"), 1, false, value_ptr(wallMatrix1));
    Models::cube.drawSolid();

    mat4 wallMatrix2 = translate(initMatrix, vec3(0, 0, -1 * C_ROOM_SIZE));
    wallMatrix2 = scale(wallMatrix2, vec3(C_ROOM_SIZE, 5.0f, 0.1f));
    glUniform4f(spLambert->u("color"), 0.2, 0.9, 0.4, 1);
    glUniformMatrix4fv(spLambert->u("M"), 1, false, value_ptr(wallMatrix2));
    Models::cube.drawSolid();

    mat4 wallMatrix3 = translate(initMatrix, vec3(C_ROOM_SIZE, 0, 0));
    wallMatrix3 = scale(wallMatrix3, vec3(0.1f, 5.0f, C_ROOM_SIZE));
    glUniform4f(spLambert->u("color"), 0.9, 0.9, 0.4, 1);
    glUniformMatrix4fv(spLambert->u("M"), 1, false, value_ptr(wallMatrix3));
    Models::cube.drawSolid();

    mat4 wallMatrix4 = translate(initMatrix, vec3(-1 * C_ROOM_SIZE, 0, 0));
    wallMatrix4 = scale(wallMatrix4, vec3(0.1f, 5.0f, C_ROOM_SIZE));
    glUniform4f(spLambert->u("color"), 0.2, 0.2, 0.4, 1);
    glUniformMatrix4fv(spLambert->u("M"), 1, false, value_ptr(wallMatrix4));
    Models::cube.drawSolid();

}

// returns matrix on the top of the table, at center point
glm::mat4 table(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 matrix = scale(initMatrix, vec3(C_TABLE_SCALE_FACTOR));

    activateSimplestTexturedShader();
    glUniformMatrix4fv(spSimplestTextured->u("M"), 1, false, value_ptr(matrix));

    for (int i = tableModel.size() - 1; i >= 0; i--) {
        RGB kd = tableModel[i].diffuse;
        RGB ka = tableModel[i].ambient;
        RGB ks = tableModel[i].specular;
        glUniform4f(spSimplestTextured->u("kd"), kd.r, kd.g, kd.b, tableModel[i].dissolve);
        glUniform4f(spSimplestTextured->u("ka"), ka.r, ka.g, ka.b, 0.0f);
        glUniform4f(spSimplestTextured->u("ks"), ks.r, ks.g, ks.b, 0.0f);
        glUniform1f(spSimplestTextured->u("alpha"), tableModel[i].shininess);
        glEnableVertexAttribArray(spSimplestTextured->a("vertex"));
        glVertexAttribPointer(spSimplestTextured->a("vertex"), 4, GL_FLOAT, false, 0, &(tableModel[i].vertices)[0]);
        glEnableVertexAttribArray(spSimplestTextured->a("normal"));
        glVertexAttribPointer(spSimplestTextured->a("normal"), 4, GL_FLOAT, false, 0, &(tableModel[i].normals)[0]);
        if (tableModel[i].textureAvailable) {
            glEnableVertexAttribArray(spSimplestTextured->a("texCoord"));
            glVertexAttribPointer(spSimplestTextured->a("texCoord"), 2, GL_FLOAT, false, 0, &(tableModel[i].texcoords)[0]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tableModel[i].texture);
            glUniform1i(spSimplestTextured->u("textureMap"), 0);
        }
        glDrawArrays(GL_TRIANGLES, 0, tableModel[i].vertices.size() / 4);
        glDisableVertexAttribArray(spSimplestTextured->a("vertex"));
        glDisableVertexAttribArray(spSimplestTextured->a("normal"));
        if (tableModel[i].textureAvailable) {
            glDisableVertexAttribArray(spSimplestTextured->a("texCoord"));
        }
    }

    return translate(initMatrix, vec3(0.0f, C_TABLE_HEIGHT, 0.0f));
}

// returns matrix at the center of the aquarium
glm::mat4 aquariumNoDraw(glm::mat4 initMatrix) {
    using namespace glm;
    return translate(initMatrix, vec3(0.0f, C_AQUARIUM_HEIGHT / 2.0f, 0.0f));
}

glm::mat4 drawSingleFish(glm::mat4 position, FishType fishType, float scaleFactor, AnimationType animationType, AnimationDir direction) {
    using namespace glm;

    mat4 fishMatrix = fishAnimator.getAnimation(animationType, position, direction);
    mat4 scaledFishMatrix = scale(fishMatrix, vec3(scaleFactor));
    ObjModel fish = fishLoader.getFish(fishType);

    activateSimplestTexturedShader();
    glUniformMatrix4fv(spSimplestTextured->u("M"), 1, false, value_ptr(scaledFishMatrix));
    RGB kd = fish.diffuse;
    RGB ka = fish.ambient;
    RGB ks = fish.specular;
    glUniform4f(spSimplestTextured->u("kd"), kd.r, kd.g, kd.b, fish.dissolve);
    glUniform4f(spSimplestTextured->u("ka"), ka.r, ka.g, ka.b, 0.0f);
    glUniform4f(spSimplestTextured->u("ks"), ks.r, ks.g, ks.b, 0.0f);
    glUniform1f(spSimplestTextured->u("alpha"), fish.shininess);

    glEnableVertexAttribArray(spSimplestTextured->a("vertex"));
    glVertexAttribPointer(spSimplestTextured->a("vertex"), 4, GL_FLOAT, false, 0, &fish.vertices[0]);
    glEnableVertexAttribArray(spSimplestTextured->a("normal"));
    glVertexAttribPointer(spSimplestTextured->a("normal"), 4, GL_FLOAT, false, 0, &fish.normals[0]);

    if (fish.textureAvailable) {
        glEnableVertexAttribArray(spSimplestTextured->a("texCoord"));
        glVertexAttribPointer(spSimplestTextured->a("texCoord"), 2, GL_FLOAT, false, 0, &fish.texcoords[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fish.texture);
        glUniform1i(spSimplestTextured->u("tex"), 0);
    }
    glDrawArrays(GL_TRIANGLES, 0, fish.vertices.size() / 4);
    glDisableVertexAttribArray(spSimplestTextured->a("vertex"));
    glDisableVertexAttribArray(spSimplestTextured->a("normal"));
    if (fish.textureAvailable) {
        glDisableVertexAttribArray(spSimplestTextured->a("texCoord"));
    }

    return fishMatrix;
}

glm::mat4 aquariumDraw(glm::mat4 matrix) {
    using namespace glm;

    mat4 m = scale(matrix, vec3(C_AQUARIUM_SCALE_FACTOR));

    activateLambertShader();
    glUniformMatrix4fv(spLambert->u("M"), 1, false, value_ptr(m));

    for (int i = aquariumModel.size() - 1; i >= 0; i--) {
        RGB color = aquariumModel[i].diffuse;
        glUniform4f(spLambert->u("color"), color.r, color.g, color.b, aquariumModel[i].dissolve);
        glEnableVertexAttribArray(spLambert->a("vertex"));
        glVertexAttribPointer(spLambert->a("vertex"), 4, GL_FLOAT, false, 0, &(aquariumModel[i].vertices)[0]);
        glEnableVertexAttribArray(spLambert->a("normal"));
        glVertexAttribPointer(spLambert->a("normal"), 4, GL_FLOAT, false, 0, &(aquariumModel[i].normals)[0]);
        // if (aquariumModel[i].textureAvailable) {
        //     glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
        //     glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, &(aquariumModel[i].texcoords)[0]);
        //     glActiveTexture(GL_TEXTURE0);
        //     glBindTexture(GL_TEXTURE_2D, aquariumModel[i].texture);
        //     glUniform1i(spLambertTextured->u("tex"), 0);
        // }
        glDrawArrays(GL_TRIANGLES, 0, aquariumModel[i].vertices.size() / 4);
        glDisableVertexAttribArray(spLambert->a("vertex"));
        glDisableVertexAttribArray(spLambert->a("normal"));
        // if (aquariumModel[i].textureAvailable) {
        //     glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
        // }
    }

    return matrix;
}

void drawFish(glm::mat4 aquariumMatrix) {
    drawSingleFish(
        translate(aquariumMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.3f, 0.0f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f)),
        BLUE,
        0.05f,
        FULL_DEPTH,
        CLK);
    drawSingleFish(
        rotate(translate(aquariumMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.3f, -0.2f, (C_AQUARIUM_DEPTH / 2.0f) - 0.8f)), PI, vec3(0.0f, 1.0f, 0.0f)),
        CORAL,
        0.5f,
        FULL_DEPTH,
        CCLK);
    drawSingleFish(
        rotate(translate(aquariumMatrix, vec3((-C_AQUARIUM_WIDTH / 2.0f) + 0.8f, 0.6f, (C_AQUARIUM_DEPTH / 2.0f) - 0.3f)), PI / 2, vec3(0.0f, 1.0f, 0.0f)),
        GOLD,
        0.08f,
        FULL_WIDTH,
        CCLK);
    drawSingleFish(
        rotate(translate(aquariumMatrix, vec3((-C_AQUARIUM_WIDTH / 2.0f) + 0.8f, -0.4f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.3f)), PI / 2, vec3(0.0f, 1.0f, 0.0f)),
        YELLOW,
        0.02f,
        FULL_WIDTH,
        CLK);
    drawSingleFish(
        translate(aquariumMatrix, vec3(0.0f, -0.6f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f)),
        TURKUS,
        0.05f,
        DEPTH,
        CLK);
    drawSingleFish(
        translate(aquariumMatrix, vec3((-C_AQUARIUM_WIDTH / 2.0f) + 1.2f, 0.2f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f)),
        SOLON,
        0.05f,
        DEPTH,
        CLK);
    drawSingleFish(
        translate(aquariumMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 1.2f, 0.5f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f)),
        BOESEMANI,
        0.05f,
        DEPTH,
        CCLK);
    drawSingleFish(
        rotate(translate(aquariumMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.7f, 0.5f, (C_AQUARIUM_DEPTH / 2.0f) - 0.8f)), PI, vec3(0.0f, 1.0f, 0.0f)),
        BLUE,
        0.05f,
        DEPTH,
        CCLK);
    drawSingleFish(
        rotate(translate(aquariumMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.8f, 0.2f, (-C_AQUARIUM_DEPTH / 2.0f) + 1.2f)), -PI / 2, vec3(0.0f, 1.0f, 0.0f)),
        TURKUS,
        0.06f,
        WIDTH,
        CLK);
    drawSingleFish(
        rotate(translate(aquariumMatrix, vec3((-C_AQUARIUM_WIDTH / 2.0f) + 0.8f, 0.3f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f)), PI / 2, vec3(0.0f, 1.0f, 0.0f)),
        YELLOW,
        0.02f,
        WIDTH,
        CLK);
    drawSingleFish(
        rotate(translate(aquariumMatrix, vec3((-C_AQUARIUM_WIDTH / 2.0f) + 0.8f, 0.6f, (C_AQUARIUM_DEPTH / 2.0f) - 0.3f)), PI / 2, vec3(0.0f, 1.0f, 0.0f)),
        SOLON,
        0.05f,
        WIDTH,
        CCLK);
    drawSingleFish(
        rotate(translate(aquariumMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.8f, -0.5f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f)), -PI / 2, vec3(0.0f, 1.0f, 0.0f)),
        GOLD,
        0.08f,
        WIDTH,
        CCLK);
}

// Drawing procedure
void drawScene(GLFWwindow* window, float angle) {
    //************Place any code here that draws something inside the window******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers

    glm::mat4 unitMatrix = glm::mat4(1.0f);
    unitMatrix = glm::rotate(unitMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 floorMatrix = floor(unitMatrix);
    walls(unitMatrix);

    glm::mat4 tableMatrix = table(floorMatrix);
    glm::mat4 aquariumMatrix = aquariumNoDraw(tableMatrix);  // I have to draw the aquarium at the end because of the alpha channel
    drawFish(aquariumMatrix);
    // aquariumDraw(tableMatrix);

    glfwSwapBuffers(window);  // Copy back buffer to the front buffer
}

int main(void) {
    GLFWwindow* window;  // Pointer to object that represents the application window

    glfwSetErrorCallback(error_callback);  // Register error processing callback procedure

    if (!glfwInit()) {  // Initialize GLFW library
        fprintf(stderr, "Can't initialize GLFW.\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  // Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it.

    if (!window)  // If no window is opened then close the program
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);  // Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
    glfwSwapInterval(1);             // During vsync wait for the first refresh

    GLenum err;
    if ((err = glewInit()) != GLEW_OK) {  // Initialize GLEW library
        fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window);  // Call initialization procedure

    // Main application loop
    float angle = 0;  // declare variable for storing current rotation angle

    float angle_y = 0;
    float angle_x = 0;

    glfwSetTime(0);                         // clear internal timer
    fishAnimator.setTime(0.0f);
    while (!glfwWindowShouldClose(window))  // As long as the window shouldnt be closed yet...
    {

        angle_y += speed_y * glfwGetTime();
        angle_x += speed_x * glfwGetTime();

		mat4 Mc = rotate(mat4(1.0f), angle_y, vec3(0, 1, 0));
		Mc = rotate(Mc, angle_x, vec3(1, 0, 0));
		vec4 dir_ = Mc*vec4(0, 0, 1, 0);
		dir = vec3(dir_);


		vec3 mdir = normalize(vec3(dir.x, 0, dir.z));

        double time = glfwGetTime();

        pos += ws * (float)time * mdir * C_MOVEMENT_SPEED;

        if (abs(pos.x) < C_AQUARIUM_WIDTH + 0.1f && abs(pos.z) < C_AQUARIUM_DEPTH + 0.1f) {
            pos = vec3(pos_prev.x, pos.y, pos_prev.z);
        }

        angle += speed * time;  // Compute an angle by which the object was rotated during the previous frame
        fishAnimator.addTime(time);

        pos_prev = pos;
        glfwSetTime(0);                        // clear internal timer
        drawScene(window, angle);              // Execute drawing procedure
        glfwPollEvents();                      // Process callback procedures corresponding to the events that took place up to now
    }
    freeOpenGLProgram(window);

    glfwDestroyWindow(window);  // Delete OpenGL context and the window.
    glfwTerminate();            // Free GLFW resources
    exit(EXIT_SUCCESS);
}
