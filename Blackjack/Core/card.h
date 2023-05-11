#pragma once
#

namespace blackjack
{
	namespace
	{
		SDL_Texture* g_cardSuitsTextures[] = { nullptr, nullptr, nullptr, nullptr };
		SDL_Texture* g_cardRangsTextures[] = { nullptr };

	} // anon namespace

	enum CARD_TYPE
	{
		CARD_TYPE_HEARTS,
		CARD_TYPE_SPADES,
		CARD_TYPE_DIAMONDS,
		CARD_TYPE_CLUBS,
		CARD_TYPE_COUNT
	};

	static_assert(_countof(g_cardSuitsTextures) == CARD_TYPE_COUNT);


}	// namespace blackjack
