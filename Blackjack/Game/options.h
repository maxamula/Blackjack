#pragma once
#include "Core/scene.h"
#include "imgui/imgui.h"
#include "Game/card.h"
#include <functional>

namespace blackjack
{
	class Options : public Scene
	{
	public:
		Options();

        ~Options()
        {
            m_overlay.Destroy();
        }

	private:
		GameObject m_overlay;
	};
}

