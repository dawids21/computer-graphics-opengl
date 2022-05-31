#if !defined(FISH_ANIMATOR_HPP)
#define FISH_ANIMATOR_HPP

#include "glm/glm.hpp"

enum AnimationType {
    FULL,
    WIDTH,
    DEPTH
};

class FishAnimator {
   private:
    double time;
    glm::mat4 getAnimationFull(glm::mat4 initMatrix);
    glm::mat4 getAnimationWidth(glm::mat4 initMatrix);
    glm::mat4 getAnimationDepth(glm::mat4 initMatrix);

   public:
    FishAnimator();
    ~FishAnimator();
    void addTime(double time);
    void setTime(double time);
    glm::mat4 getAnimation(AnimationType type, glm::mat4 initMatrix);
};

#endif  // FISH_ANIMATOR_HPP
