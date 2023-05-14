#pragma once
#include "entt/entt.hpp"
#include "components.h"

namespace blackjack
{
	class Scene;

	class EventHandler;
	struct CMP_EVENT_HANDLER
	{
		std::unique_ptr<EventHandler> handler;
	};

	class GameObject
	{
		friend class Scene;
	public:
		GameObject() = default;

		inline bool operator==(const GameObject& o) const { return m_id == o.m_id; }

		inline bool IsValid() const { return m_id != uint32_invalid; }

		// Components managing stuff here

		CMP_TRANSFORMATION& GetTransformation() const;		
		void SetSprite(const CMP_SPRITE& sprite);
		CMP_SPRITE& GetSprite() const;
		bool HasSprite() const;
		
		void SetHandler(std::unique_ptr<EventHandler> handler);
		EventHandler* GetHandler() const;
	private:
		uint32_t m_id = uint32_invalid;
	};

#pragma region Event Handler

	class EventHandler
	{
	public:
		EventHandler() = delete;
		EventHandler(const GameObject& obj)
			: m_object(obj)
		{}
		virtual bool HandleEvent(SDL_Event& event) = 0;
	protected:
		GameObject m_object;
	};

#pragma endregion

	class Scene
	{
		friend class GameObject;
	public:
		Scene() = default;
		DISABLE_COPY(Scene);
		void Destroy();

		GameObject CreateObject();
		void DestroyObject(const GameObject& object);

		void Render() const;
		void HandleEvent(SDL_Event& e) const;

	private:
		entt::registry m_reg;						// components pool
		 std::vector<GameObject> m_root;			// children
	};
}