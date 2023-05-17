#include "resource.h"

namespace blackjack
{
	ResourceManager& Resources()
	{
		static ResourceManager s_resourceManager;
		return s_resourceManager;
	}

	Resource::~Resource()
	{
		Resources()._RemoveFromLoaded(m_resourceTableHash);
		_Free();
	}
}