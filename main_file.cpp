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
FishLoader fishLoader;
FishAnimator fishAnimator;

// Error processing callback procedure
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

// Procedura obs�ugi klawiatury
void key_callback(GLFWwindow* window, int key,
                  int scancode, int action, int mod)
 {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_y = 2;
		if (key == GLFW_KEY_RIGHT) speed_y = -2;
		if (key == GLFW_KEY_PAGE_UP) speed_x = 2;
		if (key == GLFW_KEY_PAGE_DOWN) speed_x = -2;
		if (key == GLFW_KEY_UP) ws = 2;
		if (key == GLFW_KEY_DOWN) ws= -2;
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
    fishLoader.load(&textureLoader);
    glDisable(GL_CULL_FACE);
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

void floor(glm:: mat4 initMatrix){

    mat4 floorMatrix = translate(initMatrix, vec3(0,-2 * C_TABLE_HEIGHT, 0));
    floorMatrix = scale(floorMatrix, vec3(C_ROOM_SIZE, 0.1f, C_ROOM_SIZE));
    glUniform4f(spConstant->u("color"), 0.5, 0.8, 0.4, 1);
    glUniformMatrix4fv(spConstant->u("M"), 1, false, value_ptr(floorMatrix));
    Models::cube.drawSolid();
}

void walls(glm:: mat4 initMatrix){

    
    
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

void drawFish(FishType fishType, glm::mat4 matrix) {
    Fish fish = fishLoader.getFish(fishType);

    activateLambertTexturedShader();
    glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, value_ptr(matrix));

    glEnableVertexAttribArray(spLambertTextured->a("vertex"));
    glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, &fish.vertex[0]);
    glEnableVertexAttribArray(spLambertTextured->a("normal"));
    glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, &fish.normal[0]);
    glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
    glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, &fish.texCoord[0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureLoader.getTexture(fish.textureId));
    glUniform1i(spLambertTextured->u("tex"), 0);
    glDrawArrays(GL_TRIANGLES, 0, fish.vertex.size() / 4);
    glDisableVertexAttribArray(spLambertTextured->a("vertex"));
    glDisableVertexAttribArray(spLambertTextured->a("normal"));
    glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
}

glm::mat4 fish(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 fishMatrix = translate(initMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.3f, 0.0f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f));
    fishMatrix = fishAnimator.getAnimation(FULL, fishMatrix);
    mat4 scaledFishMatrix = scale(fishMatrix, vec3(0.05f, 0.05f, 0.05f));
    drawFish(BLUE, scaledFishMatrix);

    return fishMatrix;
}

glm::mat4 fish1(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 fishMatrix = translate(initMatrix, vec3(0.0f, -0.6f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f));
    fishMatrix = fishAnimator.getAnimation(DEPTH, fishMatrix);
    mat4 scaledFishMatrix = scale(fishMatrix, vec3(0.05f, 0.05f, 0.05f));
    drawFish(GOLD, scaledFishMatrix);

    return fishMatrix;
}

glm::mat4 fish2(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 fishMatrix = translate(initMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.3f, 0.2f, 0.0f));
    fishMatrix = rotate(fishMatrix, -PI / 2, vec3(0.0f, 1.0f, 0.0f));
    fishMatrix = fishAnimator.getAnimation(WIDTH, fishMatrix);
    mat4 scaledFishMatrix = scale(fishMatrix, vec3(0.05f, 0.05f, 0.05f));
    drawFish(SOLON, scaledFishMatrix);

    return fishMatrix;
}

glm::mat4 fish3(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 fishMatrix = translate(initMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.8f, 0.5f, (-C_AQUARIUM_DEPTH / 2.0f) + 1.2f));
    fishMatrix = rotate(fishMatrix, -PI / 2, vec3(0.0f, 1.0f, 0.0f));
    fishMatrix = fishAnimator.getAnimation(WIDTH, fishMatrix);
    mat4 scaledFishMatrix = scale(fishMatrix, vec3(0.05f, 0.05f, 0.05f));
    drawFish(BOESEMANI, scaledFishMatrix);

    return fishMatrix;
}

glm::mat4 fish4(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 fishMatrix = translate(initMatrix, vec3((C_AQUARIUM_WIDTH / 2.0f) - 0.8f, 0.2f, (-C_AQUARIUM_DEPTH / 2.0f) + 1.2f));
    fishMatrix = rotate(fishMatrix, -PI / 2, vec3(0.0f, 1.0f, 0.0f));
    fishMatrix = fishAnimator.getAnimation(DEPTH, fishMatrix);
    mat4 scaledFishMatrix = scale(fishMatrix, vec3(0.05f, 0.05f, 0.05f));
    drawFish(TURKUS, scaledFishMatrix);

    return fishMatrix;
}

glm::mat4 fish5(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 fishMatrix = translate(initMatrix, vec3((-C_AQUARIUM_WIDTH / 2.0f) + 1.2f, 0.3f, (-C_AQUARIUM_DEPTH / 2.0f) + 0.8f));
    fishMatrix = fishAnimator.getAnimation(DEPTH, fishMatrix);
    mat4 scaledFishMatrix = scale(fishMatrix, vec3(0.02f, 0.02f, 0.02f));
    drawFish(YELLOW, scaledFishMatrix);

    return fishMatrix;
}

// Drawing procedure
void drawScene(GLFWwindow* window, float angle) {
    //************Place any code here that draws something inside the window******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers

    glm::mat4 unitMatrix = glm::mat4(1.0f);
    unitMatrix = glm::rotate(unitMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    floor(unitMatrix);
    walls(unitMatrix);

    glm::mat4 tableMatrix = table(unitMatrix);
    glm::mat4 aquariumMatrix = aquariumNoDraw(tableMatrix);  // I have to draw the aquarium at the end because of the alpha channel
    fish(aquariumMatrix);
    fish1(aquariumMatrix);
    fish2(aquariumMatrix);
    fish3(aquariumMatrix);
    fish4(aquariumMatrix);
    fish5(aquariumMatrix);
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

        pos += ws * (float)time * mdir;

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
