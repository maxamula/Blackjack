#pragma once
#include "components.h"
#include "scene.h"

typedef void(_cdecl* FnAnimationCallback)(void);

namespace blackjack
{
    class Animation
    {
    public:
        Animation(const GameObject& obj)
            : m_object(obj)
        {
            s_animations.push_back(this);
        }
        virtual bool Update(float dt) = 0;

        static void UpdateAnimations(float dt)
        {
            for (auto it = s_animations.begin(); it != s_animations.end();)
            {
                if ((*it)->Update(dt))
                {
                    delete* it;
                    it = s_animations.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    protected:
        GameObject m_object;
    private:
        static std::vector<Animation*> s_animations;

    };

    void PlayAnimationMove(GameObject object, int toX, int toY, float playTime, FnAnimationCallback callback);
}
