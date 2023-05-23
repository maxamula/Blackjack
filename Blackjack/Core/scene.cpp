#include "scene.h"
#include "utils.h"
#include <queue>
#include "imgui/imgui.h"
#include "animations.h"

#define CHECK_VALID if(!IsValid()) throw new std::exception("Ivalid game object")

namespace blackjack
{
	namespace
	{
		struct GAME_OBJECT_NODE
		{
			entt::entity entityId;					// user for components lookup
			std::string name;
			Scene* scene;							// Parent scene reference
			GameObject parent;						// Parent object (invalid if object is root object)
			std::vector<GameObject> children;		// scenegraph impl
		};

		ItemManager<GAME_OBJECT_NODE> g_objects;
		std::recursive_mutex g_objMutex;
	} // Anon namespace

	void RenderGameObject(const GameObject& object);

#pragma region Scene
	void Scene::Destroy()
	{
		std::lock_guard lock(g_objMutex);
		auto rm = m_root;
		for (auto& object : rm)
		{
		    object._Destroy(false);
		}
		m_root.clear();
	}

	GameObject Scene::CreateObject(std::string name)
	{
		entt::entity entityId = m_reg.create();
		// Every game object has transformation component by default and cannot be removed
		m_reg.emplace_or_replace<CMP_TRANSFORMATION>(entityId);
		GameObject object;
		std::lock_guard lock(g_objMutex);
		GAME_OBJECT_NODE node{ entityId, name, this, GameObject(), {} };
		object.m_id = g_objects.Add(std::move(node));
		m_root.emplace_back(object);
		return object;
	}

	void Scene::Render() const
	{
		std::queue<GameObject> queue;
		for (const auto& object : m_root)
		{
			queue.push(object);
		}

		while (!queue.empty())
		{
			const auto& object = queue.front();
			RenderGameObject(object);
			queue.pop();

			const auto& node = g_objects.Get(object.m_id);
			for (const auto& child : node.children)
			{
				queue.push(child);
			}
		}
	}

	void Scene::DrawDebugOverlay() const
	{
		if(ImGui::CollapsingHeader("Scene Graph"))
		{
			for (const auto& object : m_root)
			{
				_DrawSceneGraphNode(object);
			}
		}
		
	}

	void Scene::Overlay() const
	{
		auto view = m_reg.view<CMP_IMGUI>();
		for (auto [entity, overlay] : view.each()) 
		{
			overlay.func();
		}
	}

	void Scene::_DrawSceneGraphNode(const GameObject& object) const
	{
		// Render a node for the current object
		bool nodeOpen = ImGui::TreeNodeEx(
			(void*)(intptr_t)object.m_id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow,
			"%s", (object.GetName() + "(" + std::to_string(object.m_id) + ")").c_str());

		// Recursively render the children of this object
		if (nodeOpen)
		{
			const auto& node = g_objects.Get(object.m_id);
			for (const auto& child : node.children)
			{
				_DrawSceneGraphNode(child);
			}

			ImGui::TreePop();
		}
	}

	void Scene::HandleEvent(SDL_Event& e) const
	{
		auto view = m_reg.view<CMP_BLUEPRINT>();
		for (auto [entity, bp] : view.each())
		{
			bp.bp->HandleEvent(e);
		}
	}

#pragma endregion
#pragma region GameObject
	bool GameObject::IsValid() const
	{
		std::lock_guard lock(g_objMutex);
		bool ret = false;
		ret |= m_id != uint32_invalid;
		if (ret)
		{
			GAME_OBJECT_NODE node = g_objects.Get(m_id);
			ret |= node.scene->m_reg.valid(node.entityId);
		}
		return ret;
	}

	Scene& GameObject::GetScene() const
	{
		std::lock_guard lock(g_objMutex);
		return *g_objects.Get(m_id).scene;
	}

	std::string GameObject::GetName() const
	{
		std::lock_guard lock(g_objMutex);
		return g_objects.Get(m_id).name;
	}

	GameObject GameObject::CreateObject(std::string name)	// Create relative to current object
	{
		std::lock_guard lock(g_objMutex);
		entt::registry& m_reg = g_objects.Get(m_id).scene->m_reg;
		entt::entity entityId = m_reg.create();
		// Every game object has transformation component by default and cannot be removed
		m_reg.emplace_or_replace<CMP_TRANSFORMATION>(entityId);
		GameObject object;	
		GAME_OBJECT_NODE node{ entityId, name, g_objects.Get(m_id).scene, *this, {} };
		object.m_id = g_objects.Add(std::move(node));
		g_objects.Get(m_id).children.emplace_back(object);
		return object;
	}

	void GameObject::Destroy()
	{
		_Destroy(true);
		//g_destroyQueue.push_back(*this);
	}

	void GameObject::_Destroy(bool lock)
	{	
		if (!IsValid())
			return;
		if(lock)
			g_objMutex.lock();
		auto& node = g_objects.Get(m_id);
		auto scene = node.scene;
		// recursive delete children and objects down the graph
		for (auto& child : node.children)
		{
			child._Destroy(false);
		}
		// remove from parent
		if (node.parent.m_id != uint32_invalid)	// NOT root object (parent is another object)
		{
			auto& parentNode = g_objects.Get(node.parent.m_id);
			auto it = std::find(parentNode.children.begin(), parentNode.children.end(), *this);
			if (it != parentNode.children.end())
				parentNode.children.erase(it);
		}
		else  // Object is root object (Parent - scene)
		{
			auto it = std::find(scene->m_root.begin(), scene->m_root.end(), *this);
			if (it != scene->m_root.end())
				scene->m_root.erase(it);
		}
		// Remove from components registry
		scene->m_reg.destroy(node.entityId);
		// remove from global registry
		g_objects.Remove(m_id);
		m_id = uint32_invalid;
		if (lock)
			g_objMutex.unlock();
	}

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

	void GameObject::SetBlueprint(std::shared_ptr<Blueprint> handler)
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		handler->Initialize();
		reg.emplace_or_replace<CMP_BLUEPRINT>(g_objects.Get(m_id).entityId, std::move(handler));
	}

	Blueprint* GameObject::GetBlueprint() const
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		CMP_BLUEPRINT* component = reg.try_get<CMP_BLUEPRINT>(g_objects.Get(m_id).entityId);
		return component ? component->bp.get() : nullptr;
	}

	void GameObject::SetOverlay(std::function<void(void)> func)
	{
		CHECK_VALID;
		std::lock_guard lock(g_objMutex);
		entt::registry& reg{ g_objects.Get(m_id).scene->m_reg };
		reg.emplace_or_replace<CMP_IMGUI>(g_objects.Get(m_id).entityId, func);
	}


#pragma endregion

	void RenderGameObject(const GameObject& object)
	{
		if (object.HasSprite())
		{
			const CMP_TRANSFORMATION& transform = object.GetTransformation();
			const CMP_SPRITE& sprite = object.GetSprite();
			if (sprite.visible)
			{
				float x = transform.x - sprite.width / 2.0f + 640;
				float y = -transform.y - sprite.height / 2.0f + 360;
				SDL_Rect dstRect = { static_cast<int>(x), static_cast<int>(y), sprite.width, sprite.height };
				SDL_RenderCopy(g_renderer, sprite.sprite->GetTexture(), nullptr, &dstRect);
			}
		}
	}

} // namespace blackjack