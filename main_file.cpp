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
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

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

using namespace glm;

float speed = 0;  //[radians/s]

// camera movement
float speed_y = 0; //[radiany/s]
float speed_x = 0; //[radiany/s]
float ws = 0;

vec3 pos = vec3(0, C_PERSON_HEIGHT, -5);
vec3 pos_prev = vec3(0, C_PERSON_HEIGHT, -5);
vec3 dir = vec3(0, 0, 1);

TextureLoader textureLoader;

std::vector<float> fishVertices;
std::vector<float> fishNormals;
std::vector<float> fishTexcoords;
int fishTextureId;

std::string objFilename = "13006_Blue_Tang_v1_l3.obj";
std::string objTexFilename = "13006_Blue_Tang_v1_diff.png";
std::string path = "./models/fish/blue";

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

// Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
    //************Place any code here that needs to be executed once, at the program start************
    glClearColor(0, 0, 0, 1);  // Set color buffer clear color
    glEnable(GL_DEPTH_TEST);   // Turn on pixel depth test based on depth buffer
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    ObjLoader objLoader;
    objLoader.load(path + "/" + objFilename, path);
    fishVertices = objLoader.getVertices();
    fishNormals = objLoader.getNormals();
    fishTexcoords = objLoader.getTextcoords();
    fishTextureId = textureLoader.loadTexture(path + "/" + objTexFilename);
    glfwSetKeyCallback(window, key_callback);
}

// Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Place any code here that needs to be executed once, after the main loop ends************
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

// returns matrix on the top of the table, at center point
glm::mat4 table(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 legMatrix = translate(initMatrix, vec3(0.0f, -1.0f, 0.0f));
    mat4 scaledLegMatrix = scale(legMatrix, vec3(0.125f, (C_TABLE_HEIGHT - 0.25f) / 2.0f, 0.125f));

    activateLambertShader();
    glUniform4f(spLambert->u("color"), 1, 1, 1, 1);
    glUniformMatrix4fv(spLambert->u("M"), 1, false, value_ptr(scaledLegMatrix));
    Models::cube.drawSolid();

    mat4 tableMatrix = legMatrix;
    tableMatrix = translate(tableMatrix, vec3(0.0f, C_TABLE_HEIGHT / 2.0f, 0.0f));
    mat4 scaledTableMatrix = scale(tableMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) + 0.1f, 0.125f, (C_AQUARIUM_DEPTH / 2.0f) + 0.1f));
    glUniformMatrix4fv(spLambert->u("M"), 1, false, value_ptr(scaledTableMatrix));
    Models::cube.drawSolid();

    return translate(tableMatrix, vec3(0.0f, 0.125f, 0.0f));
}

// returns matrix at the center of the aquarium
glm::mat4 aquariumNoDraw(glm::mat4 initMatrix) {
    using namespace glm;
    return translate(initMatrix, vec3(0.0f, C_AQUARIUM_HEIGHT / 2.0f, 0.0f));
}

void aquariumDraw(glm::mat4 aquariumMatrix) {
    using namespace glm;
    mat4 scaledAquariumMatrix = scale(aquariumMatrix, vec3(C_AQUARIUM_WIDTH / 2.0f, C_AQUARIUM_HEIGHT / 2.0f, C_AQUARIUM_DEPTH / 2.0f));

    activateLambertShader();
    glUniform4f(spLambert->u("color"), 0.5f, 1.0f, 1.0f, 0.3f);
    glUniformMatrix4fv(spLambert->u("M"), 1, false, value_ptr(scaledAquariumMatrix));
    Models::cube.drawSolid();
}

glm::mat4 fish(glm::mat4 initMatrix, float angle) {
    using namespace glm;

    mat4 fishMatrix = rotate(initMatrix, -PI / 2, vec3(1.0f, 0.0f, 0.0f));
    fishMatrix = rotate(fishMatrix, angle, vec3(0.0f, 0.0f, 1.0f));
    fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
    mat4 scaledFishMatrix = scale(fishMatrix, vec3(0.05f, 0.05f, 0.05f));

    activateLambertTexturedShader();
    glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, value_ptr(scaledFishMatrix));

    glEnableVertexAttribArray(spLambertTextured->a("vertex"));
    glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, &fishVertices[0]);
    glEnableVertexAttribArray(spLambertTextured->a("normal"));
    glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, &fishNormals[0]);
    glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
    glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, &fishTexcoords[0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureLoader.getTexture(fishTextureId));
    glUniform1i(spLambertTextured->u("tex"), 0);
    glDrawArrays(GL_TRIANGLES, 0, fishVertices.size() / 4);
    glDisableVertexAttribArray(spLambertTextured->a("vertex"));
    glDisableVertexAttribArray(spLambertTextured->a("normal"));
    glDisableVertexAttribArray(spLambertTextured->a("texCoord"));

    return fishMatrix;
}

// Drawing procedure
void drawScene(GLFWwindow* window, float angle, float fishAngle) {
    //************Place any code here that draws something inside the window******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers

    glm::mat4 unitMatrix = glm::mat4(1.0f);
    unitMatrix = glm::rotate(unitMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 tableMatrix = table(unitMatrix);
    glm::mat4 aquariumMatrix = aquariumNoDraw(tableMatrix);  // I have to draw the aquarium at the end because of the alpha channel
    fish(aquariumMatrix, fishAngle);
    aquariumDraw(aquariumMatrix);

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
    float angle = 0;                        // declare variable for storing current rotation angle
    float fishAngle = 0;


    float angle_y = 0;
    float angle_x = 0;

    glfwSetTime(0);                         // clear internal timer
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

        pos += ws * (float)time * mdir;

        if (abs(pos.x) < C_AQUARIUM_WIDTH + 0.1f && abs(pos.z) < C_AQUARIUM_DEPTH + 0.1f) {
            pos = vec3(pos_prev.x, pos.y, pos_prev.z);
        }

        angle += speed * time;  // Compute an angle by which the object was rotated during the previous frame
        fishAngle += -PI * time;


        pos_prev = pos;
        glfwSetTime(0);                        // clear internal timer
        drawScene(window, angle, fishAngle);   // Execute drawing procedure
        glfwPollEvents();                      // Process callback procedures corresponding to the events that took place up to now
    }
    freeOpenGLProgram(window);

    glfwDestroyWindow(window);  // Delete OpenGL context and the window.
    glfwTerminate();            // Free GLFW resources
    exit(EXIT_SUCCESS);
}
