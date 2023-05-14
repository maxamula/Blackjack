#include "resource.h"

namespace blackjack
{
	ResourceManager g_resourceManager;

	Resource::~Resource()
	{
		g_resourceManager._RemoveFromLoaded(m_resourceTableHash);
		_Free();
	}



}