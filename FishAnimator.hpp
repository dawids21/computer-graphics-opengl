#if !defined(FISH_ANIMATOR_HPP)
#define FISH_ANIMATOR_HPP

#include "glm/glm.hpp"

enum AnimationType {
    FULL,
    WIDTH,
    DIAGONAL,
    FULL_REVERSE,
    WIDTH_REVERSE,
    DIAGONAL_REVERSE
};

class FishAnimator {
   private:
    double time;
    glm::mat4 getAnimationFull(glm::mat4 initMatrix);
    glm::mat4 getAnimationWidth(glm::mat4 initMatrix);
    glm::mat4 getAnimationDiagonal(glm::mat4 initMatrix);
    glm::mat4 getAnimationFullReverse(glm::mat4 initMatrix);
    glm::mat4 getAnimationWidthReverse(glm::mat4 initMatrix);
    glm::mat4 getAnimationDiagonalReverse(glm::mat4 initMatrix);

   public:
    FishAnimator();
    ~FishAnimator();
    void addTime(double time);
    void setTime(double time);
    glm::mat4 getAnimation(AnimationType type, glm::mat4 initMatrix);
};

#endif  // FISH_ANIMATOR_HPP
