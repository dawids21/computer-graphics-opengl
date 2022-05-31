#if !defined(FISH_ANIMATOR_HPP)
#define FISH_ANIMATOR_HPP

#include "glm/glm.hpp"

class FishAnimator {
   private:
    double time;

   public:
    FishAnimator();
    ~FishAnimator();
    void addTime(double time);
    void setTime(double time);
    glm::mat4 getAnimation1(glm::mat4 initMatrix);
    glm::mat4 getAnimation2(glm::mat4 initMatrix);
    glm::mat4 getAnimation3(glm::mat4 initMatrix);
};

#endif  // FISH_ANIMATOR_HPP
