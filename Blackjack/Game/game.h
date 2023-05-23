#pragma once
#include "hand.h"
#include "card.h"
#include "deck.h"
#include "player.h"
#include "bet.h"
#include "imgui/imgui.h"

#define DEFAULT_BUTTON_WIDTH 200
#define DEFAULT_BUTTON_HEIGHT 70

namespace blackjack
{
	class BtnBlueprint;

	class Game : public Scene
	{
	public:
		DISABLE_COPY(Game);
		Game(Player& p);
		~Game();
		void Destroy();	
		void DrawDebugOverlay();
		void DispenseInitialCards();
		void Hit();
		void Stand();
	private:
		// logical elements
		Player& m_player;
		Hand m_hand;
		Hand m_dealer;
		Deck m_deck;
		Bet m_bet;

		// Buttons and HUD
		GameObject m_handLabelScoreObject;

		GameObject m_hitButton;
		GameObject m_hitButtonLabel;
		GameObject m_standButton;
		GameObject m_standButtonLabel;
		GameObject m_optionsButton;
		GameObject m_optionsButtonLabel;

		GameObject m_overlay;
	};
}
