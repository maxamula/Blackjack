#include "card.h"

namespace blackjack
{
	namespace
	{
		const std::string g_suitNames[] = { "hearts", "spades", "diamonds", "clubs" };
		std::vector<CMP_SPRITE> g_cardSprites[4];
		CMP_SPRITE g_backSprite{};
	}

	void PlayCard::LoadSkinSet(std::string path)
	{
		// filename buffer
		char filename[MAX_PATH];
		// for each suit load skinset
		for (uint32_t i = 0;  i < (uint32_t)CARD_SUIT_COUNT; i++)
		{
			g_cardSprites[i].clear();
			g_cardSprites[i].reserve(NUM_SUIT_CARDS);
			for (uint32_t card = 2; card <= NUM_SUIT_CARDS + 1; card++)
			{
				sprintf_s(filename, "%s\\%s\\%d.png", path.c_str(), g_suitNames[i].c_str(), card);
				g_cardSprites[i].emplace_back(CMP_SPRITE::CreateSprite(filename));
			}
		}
		g_backSprite = CMP_SPRITE::CreateSprite(path + "\\back.png");
	}

	void PlayCard::FreeSkinSet()
	{
		for (uint32_t i = 0; i < 4; i++)
		{
			g_cardSprites[i].clear();
		}
		g_backSprite = CMP_SPRITE{};
	}

	PlayCard::PlayCard(const GameObject& object, CARD_SUIT suit, uint32_t value)
		: m_suit(suit), m_value(value)
	{
		m_cardObject = object;
		m_cardObject.GetTransformation().x = 0.0;
		m_cardObject.GetTransformation().y = 0.0;
		Flip(false);
		m_cardObject.GetSprite().visible = false;
	}

	void PlayCard::Flip(bool visible)
	{
		//auto j =g_cardSprites[m_suit][m_value - 2];
		if (visible)
			m_cardObject.SetSprite(g_cardSprites[m_suit][m_value - 2]);
		else
			m_cardObject.SetSprite(g_backSprite);
	}
}