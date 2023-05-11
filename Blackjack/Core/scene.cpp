#include "scene.h"
#include "utils.h"

namespace blackjack
{
	namespace
	{
		struct GAME_OBJECT_NODE
		{
			entt::entity entityId;					// user for components lookup
			Scene* scene;							// Parent scene reference
			GameObject parent;						// Parent object (invalid if object is root object)
			std::vector<GameObject> children;		// scenegraph impl
		};

		ItemManager<GAME_OBJECT_NODE> g_objects;
		std::mutex g_objMutex;
	} // Anon namespace

	GameObject Scene::CreateObject()
	{
		entt::entity entityId = m_reg.create();
		// Every game object has transformation component by default and cannot be removed
		m_reg.emplace_or_replace<CMPTransformation>(entityId);
		GameObject object;
		std::lock_guard lock(g_objMutex);
		GAME_OBJECT_NODE node{ entityId, this, GameObject(), {} };
		object.m_id = g_objects.Add(std::move(node));
		m_root.emplace_back(object);
		return object;

	}

	void Scene::DestroyObject(const GameObject& object)
	{

	}

	CMPTransformation& GameObject::GetTransformation()
	{
		if (!IsValid())
			throw new std::exception("Game object is invalid");
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		return reg.get<CMPTransformation>(g_objects.Get(m_id).entityId);
	}

} // namespace blackjack