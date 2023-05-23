#pragma once
#include "entt/entt.hpp"
#include "components.h"
#include <mutex>

namespace blackjack
{
	class Scene;
	class Blueprint;
	struct CMP_BLUEPRINT
	{
		std::shared_ptr<Blueprint> bp;
	};

	class GameObject
	{
		friend class Scene;
	public:
		GameObject() = default;
		inline bool operator==(const GameObject& o) const { return m_id == o.m_id; }
		bool IsValid() const;
		Scene& GetScene() const;
		std::string GetName() const;
		GameObject CreateObject(std::string name);
		void Destroy();



		// Components managing stuff here
		CMP_TRANSFORMATION& GetTransformation() const;		
		void SetSprite(const CMP_SPRITE& sprite);
		CMP_SPRITE& GetSprite() const;
		bool HasSprite() const;
		
		void SetBlueprint(std::shared_ptr<Blueprint> handler);
		Blueprint* GetBlueprint() const;

		void SetOverlay(std::function<void(void)> func);
	private:
		void _Destroy(bool lock);
		uint32_t m_id = uint32_invalid;
	};

#pragma region Blueprint

	class Blueprint // defines Game object behaviour
	{
	public:
		Blueprint(const GameObject& object)
			: m_object(object)
		{}

		// Event handler
		virtual bool HandleEvent(SDL_Event& e) { return false; };

		// Init
		virtual void Initialize() {};

		// Update (called every frame)
		virtual void Update(float fDeletaTime) {};

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

		GameObject CreateObject(std::string name);

		void Render() const;
		void DrawDebugOverlay() const;
		void Overlay() const;
		void HandleEvent(SDL_Event& e) const;

	private:
		void _DrawSceneGraphNode(const GameObject& object) const;

		entt::registry m_reg;					// components pool
		std::vector<GameObject> m_root;			// children
	};
}