#include "bet.h"


namespace blackjack
{
	void Bet::Add10()
	{
		GameObject obj = m_hand.size() != 0 ? m_hand.back().go.CreateObject("Betting Round") : m_scene.CreateObject("Betting Round");
		obj.SetSprite(CMP_SPRITE::CreateSprite("assets\\round1.png"));
		auto& transform = obj.GetTransformation();
		transform.x = -500;
		transform.y = 500;
		PlayAnimationMove(obj, m_x + (m_hand.size() * OFFSET_BETWEEN_ROUNDS), m_y + (m_hand.size() * OFFSET_BETWEEN_ROUNDS_VERT), 500, nullptr);
		m_hand.push_back({ obj, 10 });
		m_val += 10;
	}

	void Bet::Add50()
	{
		GameObject obj = m_hand.size() != 0 ? m_hand.back().go.CreateObject("Betting Round") : m_scene.CreateObject("Betting Round");
		obj.SetSprite(CMP_SPRITE::CreateSprite("assets\\round2.png"));
		auto& transform = obj.GetTransformation();
		transform.x = -500;
		transform.y = 500;
		PlayAnimationMove(obj, m_x + (m_hand.size() * OFFSET_BETWEEN_ROUNDS), m_y + (m_hand.size() * OFFSET_BETWEEN_ROUNDS_VERT), 500, nullptr);
		m_hand.push_back({ obj, 50 });
		m_val += 50;
	}

	uint32_t Bet::Remove()
	{
		if (!m_hand.size()) return 0;
		Round r = m_hand.back();
		r.go.Destroy();
		m_hand.pop_back();
		return r.val;
	}

	uint32_t Bet::GetBetValue()
	{
		uint32_t sum = 0;
		for (auto& round : m_hand)
		{
			sum += round.val;
		}
		return sum;
	}
}