#pragma once
#include "card.h"
#include "deck.h"
#include "Core/animations.h"

#define OFFSET_BETWEEN_CARDS 50
#define OFFSET_BETWEEN_CARDS_VERT 0

namespace blackjack
{
	class Hand
	{
	public:
		Hand(Scene& scene, int x, int y);
		PlayCard Recieve(CARD_INFO info, std::function<void()> callback = nullptr);
		void Reveal();
		uint32_t GetHandSum();
	private:
		int m_x = 0;
		int m_y = 0;
		Scene& m_scene;
		std::vector<PlayCard> m_hand;
	};
}