#pragma once
#include "components.h"
#include "scene.h"

typedef void(* FnAnimationCallback)(void);

namespace blackjack
{
    void PlayAnimationMove(GameObject object, int toX, int toY, float playTime, std::function<void()> callback);

    class Animation
    {
        friend void PlayAnimationMove(GameObject object, int toX, int toY, float playTime, std::function<void()> callback);
    public:
        Animation(const GameObject& obj)
            : m_object(obj)
        {  }

        virtual bool Update(float dt) = 0;
        inline GameObject GetObject() const { return m_object; }

        static void DestroyAnimations(GameObject obj)
        {
            std::lock_guard lock(s_animationsMutex);
            s_animations.erase(std::remove_if(s_animations.begin(), s_animations.end(), [&](std::unique_ptr<Animation>& anim) { return obj == anim->GetObject(); }), s_animations.end());
        }

        static void UpdateAnimations(float dt)
        {
            std::lock_guard lock(s_animationsMutex);
            s_animations.erase(std::remove_if(s_animations.begin(), s_animations.end(), [&](std::unique_ptr<Animation>& anim) {
                bool remove = anim->Update(dt);
                if (remove)
                    anim.reset();
                return remove;
                }), s_animations.end());
        }
    protected:
        GameObject m_object;    
    private:
        static std::vector<std::unique_ptr<Animation>> s_animations;
        static std::recursive_mutex s_animationsMutex;
    };
}
