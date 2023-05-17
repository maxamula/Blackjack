#pragma once
#include "resource.h"

namespace blackjack
{
	struct CMP_TRANSFORMATION	// normalized screen coordinates 
	{
		int x = 0;
		int y = 0;
		float scalex = 1.0f;
		float scaley = 1.0f;
		float rotx = 0.0f;
		float roty = 0.0f;
	};

	struct CMP_SPRITE
	{
		CMP_SPRITE() = default;

		void operator=(const CMP_SPRITE& o)
		{
			width = o.width;
			height = o.height;
			sprite = o.sprite;
			visible = o.visible;
		}

		static CMP_SPRITE CreateSprite(std::string path)
		{
			//SDL_Texture* tex = IMG_LoadTexture(g_renderer, path.c_str());
			std::shared_ptr<Texture> tex = Resources().GetResource<Texture>(path.c_str());
			SDL_SetTextureScaleMode(tex->GetTexture(), SDL_ScaleModeBest);
			CMP_SPRITE sprite(tex);
			SDL_QueryTexture(tex->GetTexture(), nullptr, nullptr, &sprite.width, &sprite.height);
			return sprite;
		}

		CMP_SPRITE(std::shared_ptr<Texture> texture)
			: sprite(texture)
		{}
		int width;
		int height;
		std::shared_ptr<Texture> sprite;
		bool visible = true;
		uint8_t m_pad[3];
	};

	struct CMP_IMGUI
	{
		std::function<void(void)> func;
	};
}