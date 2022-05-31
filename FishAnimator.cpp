#include "FishAnimator.hpp"

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

double FishAnimator::getTime() {
    return this->time;
}