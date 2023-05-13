#pragma once
#include "entt/entt.hpp"
#include "components.h"

namespace blackjack
{
	class Scene;
	class GameObject
	{
		friend class Scene;
	public:
		GameObject() = default;

		inline bool IsValid() const { return m_id != INVALID_ID; }

		// Components managing stuff here

		CMP_TRANSFORMATION& GetTransformation() const;		
		void SetSprite(const CMP_SPRITE& sprite);
		CMP_SPRITE& GetSprite() const;
		bool HasSprite() const;

	private:
		uint32_t m_id = INVALID_ID;
	};

	class Scene
	{
		friend class GameObject;
	public:
		Scene() = default;
		DISABLE_COPY(Scene);

		GameObject CreateObject();
		void DestroyObject(const GameObject& object);

		void Render() const;

	private:
		entt::registry m_reg;						// components pool
		 std::vector<GameObject> m_root;			// children
	};
}