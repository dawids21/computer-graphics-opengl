#if !defined(FISH_ANIMATOR_HPP)
#define FISH_ANIMATOR_HPP

#include "glm/glm.hpp"

enum AnimationType {
    FULL,
    WIDTH,
    DIAGONAL
};

enum AnimationDir {
    CLK = -1,
    CCLK = 1
};

class FishAnimator {
   private:
    double time;
    glm::mat4 getAnimationFull(glm::mat4 initMatrix, AnimationDir direction);
    glm::mat4 getAnimationWidth(glm::mat4 initMatrix, AnimationDir direction);
    glm::mat4 getAnimationDiagonal(glm::mat4 initMatrix, AnimationDir direction);

   public:
    FishAnimator();
    ~FishAnimator();
    void addTime(double time);
    void setTime(double time);
    glm::mat4 getAnimation(AnimationType type, glm::mat4 initMatrix, AnimationDir direction);
};

#endif  // FISH_ANIMATOR_HPP
