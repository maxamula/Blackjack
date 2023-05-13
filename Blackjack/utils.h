#pragma once
#include <deque>
#include <array>
#include <optional>
#include <stack>
#include <functional>
#include <mutex>

namespace blackjack
{
    template<typename ...Args>
    class Event
    {
    public:
        typedef std::function<void(Args...)> EventCallback;

        void operator+=(const EventCallback& callback)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_callbacks.emplace_back(callback);
        }

        void operator -= (const EventCallback& callback)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_callbacks.erase(std::remove(m_callbacks.begin(), m_callbacks.end(), callback), m_callbacks.end());
        }

        void operator()(Args... args)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            for (auto& callback : m_callbacks)
                callback(args...);
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_callbacks.clear();
        }

    private:
        std::vector<EventCallback> m_callbacks;
        std::mutex m_mutex;
    };

    template <typename T>
    class ItemManager
    {
    public:
        uint32_t Add(const T& item)
        {
            uint32_t id;
            if (m_freelist.empty())
            {
                id = ++m_lastUsedId;
                m_items.push_back(item);
            }
            else
            {
                id = m_freelist.front();
                m_freelist.pop_front();
                m_items[id] = item;
            }
            m_size++;
            return id;
        }

        void Remove(const uint32_t id)
        {
            if (IsValid(id))
            {
                if (id == m_lastUsedId)
                {
                    m_lastUsedId--;
                    m_size--;
                    m_items.pop_back();
                    return;
                }
                m_freelist.push_back(id);
                m_size--;
            }
            else throw std::out_of_range::exception();
        }

        T& Get(const uint32_t id)
        {
            if (IsValid(id))
            {
                return m_items[id];
            }
            else throw std::out_of_range::exception();
        }

        std::optional<T&> TryGet(const uint32_t id) const
        {
            if (IsValid(id))
                return m_items[id];
            else return std::nullopt;
        }

        inline bool IsValid(const uint32_t id) const
        {
            return std::find(m_freelist.begin(), m_freelist.end(), id) == m_freelist.end() && id <= m_lastUsedId;
        }

        inline uint32_t Size() const
        {
            return m_size;
        }

    private:
        uint32_t m_lastUsedId = -1;
        uint32_t m_size = 0;
        std::deque<uint32_t> m_freelist;
        std::vector<T> m_items;
    };
}

