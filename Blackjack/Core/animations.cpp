#include "animations.h"
#include "utils.h"

namespace blackjack
{
    std::vector<std::unique_ptr<Animation>> Animation::s_animations;
    std::recursive_mutex Animation::s_animationsMutex;

    class AnimationPathEaseInOut : public Animation
    {
    public:
        AnimationPathEaseInOut(GameObject obj, int toX, int toY, float playTime, std::function<void()> callback)
            : Animation(obj), playTime(playTime), toX(toX), toY(toY), callback(callback)
        {
            if (m_object.IsValid())
            {
                CMP_TRANSFORMATION& tr = m_object.GetTransformation();
                startX = tr.x;
                startY = tr.y;
                distanceX = toX - startX;
                distanceY = toY - startY;
            }     
        }

        bool Update(float dt) override
        {
            if (!m_object.IsValid())
                return true;
            CMP_TRANSFORMATION& tr = m_object.GetTransformation();

            // Check if the animation has ended
            if (currentTime >= playTime)
            {
                tr.x = toX;
                tr.y = toY;

                if (callback != nullptr)
                    callback();

                return true; // indicate to remove finished animation
            }

            float progress = currentTime / playTime;
            progress = (progress < 0.5f) ? 2 * progress * progress : (-2 * progress * progress) + (4 * progress) - 1;

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
        std::function<void()> callback;
    };

    void PlayAnimationMove(GameObject object, int toX, int toY, float playTime, std::function<void()> callback)
    {
        auto animation = std::make_unique<AnimationPathEaseInOut>(object, toX, toY, playTime, callback);
        std::lock_guard lock(Animation::s_animationsMutex);
        Animation::s_animations.push_back(std::move(animation));
    }
}
