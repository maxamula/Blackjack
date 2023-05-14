#pragma once
#include "Core/scene.h"

#define NUM_SUIT_CARDS 13

#undef GetObject

namespace blackjack
{
	namespace
	{
		const std::string g_suitNames[] = { "hearts", "spades", "diamonds", "clubs" };
		std::vector<CMP_SPRITE> g_cardSprites[4];
	} // anon namespace

	enum CARD_SUIT
	{
		CARD_SUIT_HEARTS,
		CARD_SUIT_SPADES,
		CARD_SUIT_DIAMONDS,
		CARD_SUIT_CLUBS,
		CARD_SUIT_COUNT
	};

	//static_assert(_countof(g_cardSuitsTextures) == CARD_TYPE_COUNT);

	class PlayCard
	{
	public:
		static void LoadSkinSet(std::string path)
		{
			// filename buffer
			char filename[MAX_PATH];
			// for each suit load skinset
			for (uint32_t i = 0; i < (uint32_t)CARD_SUIT_COUNT; i++)
			{
				g_cardSprites[i].clear();
				g_cardSprites[i].reserve(NUM_SUIT_CARDS);
				for (uint32_t card = 2; card <= NUM_SUIT_CARDS + 1; card++)
				{
					sprintf_s(filename, "%s\\%s\\%d.png", path.c_str(), g_suitNames[i].c_str(), card);
					g_cardSprites[i].emplace_back(CMP_SPRITE::CreateSprite(filename));
				}				
			}
		}

		static PlayCard CreateCard(const GameObject& object, CARD_SUIT suit, uint32_t value)
		{
			PlayCard card;
			card.m_cardObject = object;
			card.m_cardObject.SetSprite(g_cardSprites[suit][value - 2]);
			card.m_cardObject.GetTransformation().x = 0.0;
			card.m_cardObject.GetTransformation().y = 0.0;
			return card;
		}

		void RevealAndMoveToHand();

		inline GameObject& GetObject() { return m_cardObject; }

	private:
		GameObject m_cardObject;
		uint32_t m_value = 6;
		CARD_SUIT m_suit = CARD_SUIT_CLUBS;

	};

}	// namespace blackjack
