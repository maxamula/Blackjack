#include "animations.h"
#include "utils.h"

namespace blackjack
{
    std::vector<Animation*> Animation::s_animations;

    class AnimationPathEaseInOut : public Animation
    {
    public:
        AnimationPathEaseInOut(GameObject obj, int toX, int toY, float playTime, FnAnimationCallback callback)
            : Animation(obj), playTime(playTime), toX(toX), toY(toY)
        {
            CMP_TRANSFORMATION& tr = m_object.GetTransformation();
            startX = tr.x; // Assuming 'x' and 'y' are the current position components of the object
            startY = tr.y;

            // Calculate the distance to move
            distanceX = toX - startX;
            distanceY = toY - startY;
        }

        bool Update(float dt) override
        {
            CMP_TRANSFORMATION& tr = m_object.GetTransformation();

            // Check if the animation has ended
            if (currentTime >= playTime)
            {
                // Set the object's position to the final position
                tr.x = toX;
                tr.y = toY;

                // Call the animation callback function
                if (callback != nullptr)
                    callback();

                return true; // Exit the animation loop
            }

            // Calculate the animation progress (value between 0 and 1 based on the elapsed time)
            float progress = currentTime / playTime;

            // Apply a smooth curve (e.g., ease-in-out) to the animation progress
            progress = (progress < 0.5f) ? 2 * progress * progress : (-2 * progress * progress) + (4 * progress) - 1;

            // Calculate the new position of the object based on the animation progress
            int currentX = startX + static_cast<int>(distanceX * progress);
            int currentY = startY + static_cast<int>(distanceY * progress);

            // Set the object's position
            tr.x = currentX;
            tr.y = currentY;

            currentTime += dt;
            return false;
        }

    private:
        int startX, startY;
        int toX, toY;
        int distanceX, distanceY;
        float currentTime = 0;
        float playTime;
        FnAnimationCallback callback;
    };

    void PlayAnimationMove(GameObject object, int toX, int toY, float playTime, FnAnimationCallback callback = nullptr)
    {
        new AnimationPathEaseInOut(object, toX, toY, playTime, callback);
    }
}
