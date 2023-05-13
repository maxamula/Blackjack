#pragma once
#include "animations.h"
#include "SDL_image.h"

namespace blackjack
{
	//class IScript
	//{
	//public:
	//	explicit IScript(GameObject& object)
	//		: m_object(object) {}

	//	virtual void OnUserCreate() = 0;
	//	virtual void OnUserUpdate(float fDeltaTime) = 0;

	//protected:
	//	GameObject m_object;
	//};

#pragma region Event Handlers

	class IEventHandler
	{
	public:
		virtual bool HandleEvent(SDL_Event& event) = 0;
	};

	class RectClickRegion : IEventHandler
	{
	public:
		RectClickRegion(const SDL_Rect& rect)
			: m_region(rect)
		{}

		~RectClickRegion()
		{

		}

		bool HandleEvent(SDL_Event& event) override
		{
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				if (mouseX >= m_region.x && mouseX <= m_region.x + m_region.w &&
					mouseY >= m_region.y && mouseY <= m_region.y + m_region.h)
				{

					return true;
				}
			}
			return false;
		}

	private:
		SDL_Rect m_region = {};
	};

#pragma endregion

	struct CMP_TRANSFORMATION	// normalized screen coordinates 
	{
		float x = 0.0f;
		float y = 0.0f;
		float scalex = 1.0f;
		float scaley = 1.0f;
		float rotx = 0.0f;
		float roty = 0.0f;
	};

	struct CMP_SCRIPT
	{
		//std::unique_ptr<IScript> script;
	};

	struct CMP_SPRITE
	{
		CMP_SPRITE() = default;

		static CMP_SPRITE CreateSprite(std::string path)
		{
			SDL_Texture* tex = IMG_LoadTexture(g_renderer, path.c_str());
			CMP_SPRITE sprite(tex);
			int textureWidth, textureHeight;
			SDL_QueryTexture(tex, nullptr, nullptr, &sprite.width, &sprite.height);
			return sprite;
		}

		CMP_SPRITE(SDL_Texture* texture, AnimationStateMachine* animation = nullptr)
			: sprite(texture), animation(animation)
		{}
		inline void Render() const
		{
			if (!animation)
				SDL_RenderCopy(g_renderer, sprite, nullptr, nullptr);
			else
				animation->Render(g_renderer);
		}
		int width;
		int height;
		SDL_Texture* sprite = nullptr;
		AnimationStateMachine* animation = nullptr;
	};

	struct CMPEventHandler
	{
		std::vector<std::unique_ptr<IEventHandler>> handlers;
	};
}

