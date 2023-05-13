#pragma once
#include "card.h"
#include <stack>

namespace blackjack
{
	class Deck
	{
	public:
		Deck(const GameObject& object)
			: m_object(object)
		{
			auto& transform = m_object.GetTransformation();
			m_object.SetSprite(CMP_SPRITE::CreateSprite("C:\\deck.png"));
			transform.x = 0.7;
			transform.y = 0.2;
		}

	private:
		GameObject m_object;
		std::stack<PlayCard> m_stack;
	};
}