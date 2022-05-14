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

#include "allmodels.h"
#include "constants.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "lodepng.h"
#include "shaderprogram.h"

float speed = 0;  //[radians/s]
float fishSpeed = 0;

// Error processing callback procedure
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

// Procedura obs�ugi klawiatury
void key_callback(GLFWwindow* window, int key,
                  int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) speed = -PI;  // Je�eli wci�ni�to klawisz "w lewo" ustaw pr�dko�� na -PI
        if (key == GLFW_KEY_RIGHT) speed = PI;  // Je�eli wci�ni�to klawisz "w prawo" ustaw pr�dko�� na PI
        if (key == GLFW_KEY_A) fishSpeed = -PI;
        if (key == GLFW_KEY_D) fishSpeed = PI;
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) speed = 0;
        if (key == GLFW_KEY_A || key == GLFW_KEY_D) fishSpeed = 0;
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
    glfwSetKeyCallback(window, key_callback);
}

// Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Place any code here that needs to be executed once, after the main loop ends************
}

// returns matrix on the top of the table, at center point
glm::mat4 table(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 legMatrix = translate(initMatrix, vec3(0.0f, -1.0f, 0.0f));
    mat4 scaledLegMatrix = scale(legMatrix, vec3(0.125f, 0.5f, 0.125f));
    glUniform4f(spConstant->u("color"), 1, 1, 1, 1);
    glUniformMatrix4fv(spConstant->u("M"), 1, false, value_ptr(scaledLegMatrix));
    Models::cube.drawSolid();

    mat4 tableMatrix = legMatrix;
    tableMatrix = translate(tableMatrix, vec3(0.0f, 0.625f, 0.0f));
    mat4 scaledTableMatrix = scale(tableMatrix, vec3(1.0f, 0.125f, 1.0f));
    glUniformMatrix4fv(spConstant->u("M"), 1, false, value_ptr(scaledTableMatrix));
    Models::cube.drawSolid();

    return translate(tableMatrix, vec3(0.0f, 0.125f, 0.0f));
}

// returns matrix at the center of the aquarium
glm::mat4 aquarium(glm::mat4 initMatrix) {
    using namespace glm;

    mat4 aquariumMatrix = translate(initMatrix, vec3(0.0f, 0.5f, 0.0f));
    mat4 scaledAquariumMatrix = scale(aquariumMatrix, vec3(0.9f, 0.5f, 0.9f));
    glUniform4f(spConstant->u("color"), 0, 0, 1, 0.5f);
    glUniformMatrix4fv(spConstant->u("M"), 1, false, value_ptr(scaledAquariumMatrix));
    Models::cube.drawSolid();

    return aquariumMatrix;
}

glm::mat4 fish(glm::mat4 initMatrix, float angle) {
    using namespace glm;

    mat4 fishMatrix = rotate(initMatrix, angle, vec3(0.0f, 1.0f, 0.0f));
    fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));

    glUniform4f(spConstant->u("color"), 0, 1, 0, 1);
    glUniformMatrix4fv(spConstant->u("M"), 1, false, value_ptr(fishMatrix));
    Models::Sphere sphere(0.8, 12, 12);
    sphere.drawSolid();

    return fishMatrix;
}

// Drawing procedure
void drawScene(GLFWwindow* window, float angle, float fishAngle) {
    //************Place any code here that draws something inside the window******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers

    glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);                                             // Compute projection matrix
    glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -7.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Compute view matrix

    spConstant->use();  // Aktywacja programu cieniuj�cego
    glUniformMatrix4fv(spConstant->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(spConstant->u("V"), 1, false, glm::value_ptr(V));

    glm::mat4 unitMatrix = glm::mat4(1.0f);
    unitMatrix = glm::rotate(unitMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 tableMatrix = table(unitMatrix);
    glm::mat4 aquariumMatrix = aquarium(tableMatrix);
    fish(aquariumMatrix, fishAngle);

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
    glfwSetTime(0);                         // clear internal timer
    while (!glfwWindowShouldClose(window))  // As long as the window shouldnt be closed yet...
    {
        double time = glfwGetTime();
        angle += speed * time;  // Compute an angle by which the object was rotated during the previous frame
        fishAngle += fishSpeed * time;
        glfwSetTime(0);                        // clear internal timer
        drawScene(window, angle, fishAngle);   // Execute drawing procedure
        glfwPollEvents();                      // Process callback procedures corresponding to the events that took place up to now
    }
    freeOpenGLProgram(window);

    glfwDestroyWindow(window);  // Delete OpenGL context and the window.
    glfwTerminate();            // Free GLFW resources
    exit(EXIT_SUCCESS);
}
