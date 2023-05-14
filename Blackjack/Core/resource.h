#pragma once
#include "SDL_image.h"

namespace blackjack
{
	class ResourceManager;

	extern ResourceManager g_resourceManager;

	class Resource
	{
		friend class ResourceManager;
	public:
		virtual ~Resource();

	protected:
		virtual void _Load(std::string path) = 0;
		virtual void _Free() {};

		uint64_t m_resourceTableHash = uint64_invalid;
	};

	class ResourceManager
	{
		friend class Resource;
	public:
		template <class T, typename std::enable_if<std::is_base_of<Resource, T>::value>::type* = nullptr>
		std::shared_ptr<T> GetResource(std::string path)
		{
			std::lock_guard lock(m_mutex);
			uint64_t hash = std::hash<std::string>()(path);
			auto it = m_resources.find(hash);
			if (it != m_resources.end())		// if resource is already loaded
			{
				return std::dynamic_pointer_cast<T>(it->second.lock());
			}
			else
			{
				std::shared_ptr<T> resource = std::make_shared<T>();
				reinterpret_cast<Resource*>(resource.get())->_Load(path);
				reinterpret_cast<Resource*>(resource.get())->m_resourceTableHash = hash;
				std::weak_ptr<Resource> weak = resource;
				m_resources.insert(std::make_pair(hash, weak));
				return resource;
			}
		}

	private:

		inline void _RemoveFromLoaded(uint64_t hash)
		{
			std::lock_guard lock(m_mutex);
			auto it = m_resources.find(hash);
			if (it != m_resources.end()) // if resource is loaded
			{
				m_resources.erase(it); // remove resource from map
			}
		}
		std::mutex m_mutex;
		std::unordered_map<uint64_t, std::weak_ptr<Resource>> m_resources;
	};

	class Texture : public Resource
	{
	public:
		SDL_Texture* GetTexture() const { return m_tex; }
	private:
		SDL_Texture* m_tex = nullptr;
	protected:
		void _Load(std::string path) override
		{
			m_tex = IMG_LoadTexture(g_renderer, path.c_str());
		}

		void _Free() override
		{
			SDL_DestroyTexture(m_tex);
		}
	};
}
