#include "scene.h"
#include "utils.h"

#define CHECK_VALID if(!IsValid()) throw new std::exception("Ivalid game object")

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

	void RenderGameObject(const GameObject& object);

#pragma region Scene
	GameObject Scene::CreateObject()
	{
		entt::entity entityId = m_reg.create();
		// Every game object has transformation component by default and cannot be removed
		m_reg.emplace_or_replace<CMP_TRANSFORMATION>(entityId);
		GameObject object;
		std::lock_guard lock(g_objMutex);
		GAME_OBJECT_NODE node{ entityId, this, GameObject(), {} };
		object.m_id = g_objects.Add(std::move(node));
		m_root.emplace_back(object);
		return object;
	}

	void Scene::DestroyObject(const GameObject& object)
	{
		std::lock_guard lock(g_objMutex);
		auto& node = g_objects.Get(object.m_id);
		auto scene = node.scene;
		// recursive delete children and objects down the graph
		for (auto& child : node.children)
		{
			//object.DestroyChild(child);
		}
		// remove from parent
		if (node.parent.m_id != uint32_invalid)	// NOT root object (parent is another object)
		{
			auto& parentNode = g_objects.Get(node.parent.m_id);
			auto it = std::find(parentNode.children.begin(), parentNode.children.end(), object);
			if (it != parentNode.children.end())
				parentNode.children.erase(it);
		}
		else  // Object is root object (Parent - scene)
		{
			auto it = std::find(scene->m_root.begin(), scene->m_root.end(), object);
			if (it != scene->m_root.end())
				scene->m_root.erase(it);
		}
		// Remove from components registry
		scene->m_reg.destroy(node.entityId);
	}

	void Scene::Render() const
	{
		for (auto& object : m_root)
		{
			RenderGameObject(object);
		}
	}

	void Scene::HandleEvent(SDL_Event& e) const
	{
		for (auto& object : m_root)
		{
			EventHandler* handler = object.GetHandler();
			if (handler)
				handler->HandleEvent(e);
		}
	}

#pragma endregion
#pragma region GameObject

	CMP_TRANSFORMATION& GameObject::GetTransformation() const
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		return reg.get<CMP_TRANSFORMATION>(g_objects.Get(m_id).entityId);
	}

	void GameObject::SetSprite(const CMP_SPRITE& sprite)
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		reg.emplace_or_replace<CMP_SPRITE>(g_objects.Get(m_id).entityId, sprite);
	}

	CMP_SPRITE& GameObject::GetSprite() const
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		return reg.get<CMP_SPRITE>(g_objects.Get(m_id).entityId);
	}

	bool GameObject::HasSprite() const
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		return reg.try_get<CMP_SPRITE>(g_objects.Get(m_id).entityId) != nullptr;
	}

	void GameObject::SetHandler(std::unique_ptr<EventHandler> handler)
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		reg.emplace_or_replace<CMP_EVENT_HANDLER>(g_objects.Get(m_id).entityId, std::move(handler));
	}

	EventHandler* GameObject::GetHandler() const
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		CMP_EVENT_HANDLER* component = reg.try_get<CMP_EVENT_HANDLER>(g_objects.Get(m_id).entityId);
		return component ? component->handler.get() : nullptr;
	}

#pragma endregion

	void RenderGameObject(const GameObject& object)
	{
		if (object.HasSprite())
		{
			const CMP_TRANSFORMATION& transform = object.GetTransformation();
			const CMP_SPRITE& sprite = object.GetSprite();
			if (!sprite.animation)
			{
				float x = 1280 - (transform.x + 640) - sprite.width / 2.0f;
				float y = 720 - (transform.y + 360) - sprite.height / 2.0f;
				SDL_Rect dstRect = { static_cast<int>(x), static_cast<int>(y), sprite.width, sprite.height };
				SDL_RenderCopy(g_renderer, sprite.sprite->GetTexture(), nullptr, &dstRect);
			}
			else
			{
				sprite.animation->Render(g_renderer);
			}
		}
	}

} // namespace blackjack