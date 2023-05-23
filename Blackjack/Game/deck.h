#pragma once
#include "card.h"
#include <stack>
#include <random>

namespace blackjack
{
	struct CARD_INFO
	{
		CARD_SUIT suit;
		uint32_t val;
	};

	class Deck
	{
	public:
		Deck()
		{
			for (uint32_t suit = 0; suit < 4; suit++) {
				for (uint32_t value = 2; value <= 14; value++) {
					m_stack.emplace(CARD_INFO{ (CARD_SUIT)suit, value });
				}
			}

			Shuffle();
		}

		CARD_INFO PullCard()
		{
			auto card = m_stack.top();
			m_stack.pop();
			return card;
		}

		void Shuffle()
		{
			// create a random number generator and distribution
			std::mt19937 rng{ std::random_device{}() };
			std::uniform_int_distribution<size_t> dist{ 0, m_stack.size() - 1 };

			auto cards = std::vector<CARD_INFO>{};
			while (!m_stack.empty()) {
				cards.push_back(m_stack.top());
				m_stack.pop();
			}

			// shuffle the vector using the Fisher-Yates algorithm
			for (size_t i = cards.size() - 1; i > 0; i--) {
				size_t j = dist(rng) % (i + 1);
				std::swap(cards[i], cards[j]);
			}

			for (auto&& card : cards) {
				m_stack.push(card);
			}
		}

	private:
		std::stack<CARD_INFO> m_stack;
	};
}