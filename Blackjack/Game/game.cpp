#include "game.h"

namespace blackjack
{
	void Game::DispenseInitialCards()
	{
		m_hand.Recieve(m_deck.PullCard()).Flip(true);
		m_hand.Recieve(m_deck.PullCard()).Flip(true);
		m_dealer.Recieve(m_deck.PullCard()).Flip(true);
		m_dealer.Recieve(m_deck.PullCard());
#ifdef ENABLE_DEALER_MUSTHAVE
		while (m_dealer.GetHandSum() < 17)
			m_dealer.Recieve(m_deck.PullCard());
#endif
		if (m_dealer.GetHandSum() > 21)
			::Beep(1000, 100); // player win
	};
}