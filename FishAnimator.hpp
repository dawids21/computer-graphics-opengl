#if !defined(FISH_ANIMATOR_HPP)
#define FISH_ANIMATOR_HPP

class FishAnimator {
   private:
    double time;

   public:
    FishAnimator();
    ~FishAnimator();
    void addTime(double time);
    void setTime(double time);
    double getTime();
};

#endif  // FISH_ANIMATOR_HPP
