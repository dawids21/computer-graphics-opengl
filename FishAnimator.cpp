#include "FishAnimator.hpp"

#include <cmath>

#include "config.hpp"
#include "constants.h"
#include "glm/gtc/matrix_transform.hpp"

FishAnimator::FishAnimator() {
}

FishAnimator::~FishAnimator() {
}

void FishAnimator::setTime(double time) {
    this->time = time;
}

void FishAnimator::addTime(double time) {
    this->time += time;
}

static float stepTime(float time, float startTime, float stepTime) {
    return std::min((time - startTime) / stepTime, 1.0f);
}

glm::mat4 FishAnimator::getAnimation(AnimationType type, glm::mat4 initMatrix) {
    if (type == FULL) {
        return this->getAnimationFull(initMatrix);
    } else if (type == WIDTH) {
        return this->getAnimationWidth(initMatrix);
    } else if (type == DIAGONAL) {
        return this->getAnimationDiagonal(initMatrix);
    }
    return initMatrix;
}

glm::mat4 FishAnimator::getAnimationFull(glm::mat4 initMatrix) {
    using namespace glm;

    float currentTime = std::fmod(this->time, 6.0f);

    float steps[] = {0.0f, 1.0f, 1.5f, 2.5f, 3.0f, 4.0f, 4.5f, 5.5f};
    float stepTimes[] = {1.0f, 0.5f, 1.0f, 0.5f, 1.0f, 0.5f, 1.0f, 0.5f};
    int counter = 0;

    mat4 fishMatrix = initMatrix;
    fishMatrix = translate(fishMatrix, vec3(0.0f, 0.0f, (C_AQUARIUM_WIDTH - 1.6f) * stepTime(currentTime, steps[counter], stepTimes[counter])));
    counter++;
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(-0.5f, 0.0f, 0.0f));
        fishMatrix = rotate(fishMatrix, -PI / 2.0f * stepTime(currentTime, steps[counter], stepTimes[counter]), vec3(0.0f, 1.0f, 0.0f));
        fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(0.0f, 0.0f, (C_AQUARIUM_WIDTH - 1.6f) * stepTime(currentTime, steps[counter], stepTimes[counter])));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(-0.5f, 0.0f, 0.0f));
        fishMatrix = rotate(fishMatrix, -PI / 2.0f * stepTime(currentTime, steps[counter], stepTimes[counter]), vec3(0.0f, 1.0f, 0.0f));
        fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(0.0f, 0.0f, (C_AQUARIUM_WIDTH - 1.6f) * stepTime(currentTime, steps[counter], stepTimes[counter])));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(-0.5f, 0.0f, 0.0f));
        fishMatrix = rotate(fishMatrix, -PI / 2.0f * stepTime(currentTime, steps[counter], stepTimes[counter]), vec3(0.0f, 1.0f, 0.0f));
        fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(0.0f, 0.0f, (C_AQUARIUM_WIDTH - 1.6f) * stepTime(currentTime, steps[counter], stepTimes[counter])));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(-0.5f, 0.0f, 0.0f));
        fishMatrix = rotate(fishMatrix, -PI / 2.0f * stepTime(currentTime, steps[counter], stepTimes[counter]), vec3(0.0f, 1.0f, 0.0f));
        fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
    }
    return fishMatrix;
}

glm::mat4 FishAnimator::getAnimationWidth(glm::mat4 initMatrix) {
    using namespace glm;

    float currentTime = std::fmod(this->time, 4.0f);

    float steps[] = {0.0f, 1.0f, 2.0f, 3.0f};
    float stepTimes[] = {1.0f, 1.0f, 1.0f, 1.0f};
    int counter = 0;

    mat4 fishMatrix = initMatrix;
    fishMatrix = translate(fishMatrix, vec3(0.0f, 0.0f, (C_AQUARIUM_WIDTH - 1.6f) * stepTime(currentTime, steps[counter], stepTimes[counter])));
    counter++;
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(-0.5f, 0.0f, 0.0f));
        fishMatrix = rotate(fishMatrix, -PI * stepTime(currentTime, steps[counter], stepTimes[counter]), vec3(0.0f, 1.0f, 0.0f));
        fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(0.0f, 0.0f, (C_AQUARIUM_WIDTH - 1.6f) * stepTime(currentTime, steps[counter], stepTimes[counter])));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(-0.5f, 0.0f, 0.0f));
        fishMatrix = rotate(fishMatrix, -PI * stepTime(currentTime, steps[counter], stepTimes[counter]), vec3(0.0f, 1.0f, 0.0f));
        fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
    }
    return fishMatrix;
}

glm::mat4 FishAnimator::getAnimationDiagonal(glm::mat4 initMatrix) {
    using namespace glm;

    float currentTime = std::fmod(this->time, 4.0f);

    float steps[] = {0.0f, 1.0f, 2.0f, 3.0f};
    float stepTimes[] = {1.0f, 1.0f, 1.0f, 1.0f};
    int counter = 0;

    mat4 fishMatrix = initMatrix;
    fishMatrix = translate(fishMatrix, vec3(0.0f, 0.0f, (C_AQUARIUM_WIDTH - 1.2f) * stepTime(currentTime, steps[counter], stepTimes[counter])));
    counter++;
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(-0.5f, 0.0f, 0.0f));
        fishMatrix = rotate(fishMatrix, -PI * stepTime(currentTime, steps[counter], stepTimes[counter]), vec3(0.0f, 1.0f, 0.0f));
        fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(0.0f, 0.0f, (C_AQUARIUM_WIDTH - 1.2f) * stepTime(currentTime, steps[counter], stepTimes[counter])));
        counter++;
    }
    if (currentTime > steps[counter]) {
        fishMatrix = translate(fishMatrix, vec3(-0.5f, 0.0f, 0.0f));
        fishMatrix = rotate(fishMatrix, -PI * stepTime(currentTime, steps[counter], stepTimes[counter]), vec3(0.0f, 1.0f, 0.0f));
        fishMatrix = translate(fishMatrix, vec3(0.5f, 0.0f, 0.0f));
    }
    return fishMatrix;
}