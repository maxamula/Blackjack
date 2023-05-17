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
		Hand(Scene& scene, int x, int y)
			: m_scene(scene), m_x(x), m_y(y)
		{}

		~Hand()
		{
			Reset();
		}

		PlayCard& Recieve(CARD_INFO info)
		{
			// TODO play animation

			// Set object hierarhy
			GameObject obj = m_hand.size() != 0 ? m_hand.back().GetObject().CreateObject("HandCard") : m_scene.CreateObject("HandCard");
			PlayCard card(obj, info.suit, info.val);
			auto& transform = card.GetObject().GetTransformation();
			transform.x = -500;
			transform.y = 500;

			PlayAnimationMove(obj, m_x + (m_hand.size() * OFFSET_BETWEEN_CARDS), m_y + (m_hand.size() * OFFSET_BETWEEN_CARDS_VERT), 500, nullptr);

			//transform.y = m_y + (m_hand.size() * OFFSET_BETWEEN_CARDS_VERT);
			//transform.x = m_x + (m_hand.size() * OFFSET_BETWEEN_CARDS);
			
			card.GetObject().GetSprite().visible = true;
			m_hand.push_back(card);
			return card;
		}

		void Reveal()
		{
			for (auto& card : m_hand)
			{
				card.Flip(true);
			}
		}

		void Reset()
		{
			for (auto& card : m_hand)
			{
				card.GetObject().Destroy();
			}
			m_hand.clear();
		}

		uint32_t GetHandSum()
		{
			uint32_t sum = 0;
			for (auto& card : m_hand)
			{
				sum += card.GetBJValue();
			}
			return sum;
		}

	private:
		int m_x = 0;
		int m_y = 0;
		Scene& m_scene;
		std::vector<PlayCard> m_hand;
	};
}