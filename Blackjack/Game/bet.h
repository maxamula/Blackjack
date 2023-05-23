#pragma once
#include "Core/scene.h"
#include "Core/animations.h"

#define OFFSET_BETWEEN_ROUNDS 20
#define OFFSET_BETWEEN_ROUNDS_VERT 0

namespace blackjack
{
	class Bet
	{
	public:
		Bet(Scene& scene, int x, int y)
			: m_scene(scene), m_x(x), m_y(y)
		{}

		struct Round
		{
			GameObject go;
			uint32_t val;
		};

		void Add10();
		void Add50();
		uint32_t Remove();
		uint32_t GetBetValue();

	private:
		int m_x = 0;
		int m_y = 0;
		uint32_t m_val;
		Scene& m_scene;
		std::vector<Round> m_hand;
	};
}