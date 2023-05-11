#pragma once


namespace blackjack
{
	//class IScript
	//{
	//public:
	//	explicit IScript(GameObject& object)
	//		: m_object(object) {}

	//	virtual void OnUserCreate() = 0;
	//	virtual void OnUserUpdate(float fDeltaTime) = 0;

	//protected:
	//	GameObject m_object;
	//};

	struct CMPTransformation
	{
		float x = 0.0;
		float y = 0.0;
	};

	struct CMPScript
	{
		//std::unique_ptr<IScript> script;
	};
}

