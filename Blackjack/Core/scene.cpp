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

	void RenderGameObject(const GameObject& object, uint32_t vpWidth, uint32_t vpHeight);

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

	}

	void Scene::Render() const
	{
		// Get viewport size
		int windowWidth, windowHeight;
		SDL_GetWindowSize(g_mainWindow, &windowWidth, &windowHeight);
		int top, left, bottom, right;
		SDL_GetWindowBordersSize(g_mainWindow, &top, &left, &bottom, &right);
		uint32_t vpWidth = windowWidth - left - right;
		uint32_t vpHeight = windowHeight - top - bottom;

		for (auto& object : m_root)
		{
			if (object.HasSprite())
			{
				//object.GetSprite().Render();
				RenderGameObject(object, vpWidth, vpHeight);
			}
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
#pragma endregion

	void RenderGameObject(const GameObject& object, uint32_t vpWidth, uint32_t vpHeight)
	{
		if (object.HasSprite())
		{
			const CMP_TRANSFORMATION& transform = object.GetTransformation();
			const CMP_SPRITE& sprite = object.GetSprite();
			if (!sprite.animation)
			{
				int newWidth = static_cast<int>(sprite.width * transform.scalex);
				int newHeight = static_cast<int>(sprite.height * transform.scaley);
				int x = static_cast<int>((vpWidth * transform.x) - (newWidth / 2));
				int y = static_cast<int>((vpHeight * transform.y) - (newHeight / 2));
				SDL_Rect dstRect = { x, y, newWidth, newHeight };
				SDL_RenderCopy(g_renderer, sprite.sprite, nullptr, &dstRect);
			}
			else
				sprite.animation->Render(g_renderer);
		}
	}

} // namespace blackjack