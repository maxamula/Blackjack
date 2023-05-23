#pragma once
#include "Core/scene.h"

#define NUM_SUIT_CARDS 13

#undef GetObject

namespace blackjack
{
	enum CARD_SUIT
	{
		CARD_SUIT_HEARTS,
		CARD_SUIT_SPADES,
		CARD_SUIT_DIAMONDS,
		CARD_SUIT_CLUBS,
		CARD_SUIT_COUNT
	};

	class PlayCard
	{
	public:
		static void LoadSkinSet(std::string path);
		static void FreeSkinSet();
		PlayCard(const GameObject& object, CARD_SUIT suit, uint32_t value);

		inline uint32_t GetBJValue()
		{
			if (m_value == 14) return 11;
			return m_value < 10 ? m_value : 10;
		}
		inline CARD_SUIT Suit() const { return m_suit; }
		inline uint32_t Value() const { return m_value; }
		void Flip(bool visible);
		inline GameObject& GetObject() { return m_cardObject; }
	private:
		GameObject m_cardObject;
		uint32_t m_value = 6;
		CARD_SUIT m_suit = CARD_SUIT_CLUBS;
	};

}	// namespace blackjack
