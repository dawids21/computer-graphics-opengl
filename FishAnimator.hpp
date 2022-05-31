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
    double getTime();
    glm::mat4 getAnimation(glm::mat4 initMatrix);
};

#endif  // FISH_ANIMATOR_HPP
