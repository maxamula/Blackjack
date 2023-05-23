#include "hand.h"


namespace blackjack
{
	Hand::Hand(Scene& scene, int x, int y)
		: m_scene(scene), m_x(x), m_y(y)
	{}

	PlayCard Hand::Recieve(CARD_INFO info, std::function<void()> callback)
	{
		// TODO play animation

		// Set object hierarhy
		GameObject obj = m_hand.size() != 0 ? m_hand.back().GetObject().CreateObject("HandCard") : m_scene.CreateObject("HandCard");
		PlayCard card(obj, info.suit, info.val);
		auto& transform = card.GetObject().GetTransformation();
		transform.x = -500;
		transform.y = 500;

		PlayAnimationMove(obj, m_x + (m_hand.size() * OFFSET_BETWEEN_CARDS), m_y + (m_hand.size() * OFFSET_BETWEEN_CARDS_VERT), 500, callback);

		//transform.y = m_y + (m_hand.size() * OFFSET_BETWEEN_CARDS_VERT);
		//transform.x = m_x + (m_hand.size() * OFFSET_BETWEEN_CARDS);

		card.GetObject().GetSprite().visible = true;
		m_hand.push_back(card);
		return card;
	}

	void Hand::Reveal()
	{
		for (auto& card : m_hand)
		{
			card.Flip(true);
		}
	}

	uint32_t Hand::GetHandSum()
	{
		uint32_t sum = 0;
		for (auto& card : m_hand)
		{
			sum += card.GetBJValue();
		}
		return sum;
	}
}