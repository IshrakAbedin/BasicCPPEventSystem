#ifndef EVENT_H
#define EVENT_H

#pragma once

#include <vector>
#include <functional>

namespace ish
{
    template <typename... params>
    class Event
    {
    private:
        std::vector<std::function<void(params...)>> m_SubscriberList;

    public:
        Event() = default;
        ~Event() = default;
        Event(const Event &other) = delete;
        Event operator=(const Event &other) = delete;
        bool operator==(const Event &other) = delete;

        // Subscribe to event, operator overload +=
        Event &operator+=(const std::function<void(params...)> subscriber);

        // Subscribe to event, operator overload <<
        Event &operator<<(const std::function<void(params...)> subscriber);

        // Unsubscribe from event, operator overload -=
        Event &operator-=(const std::function<void(params...)> subscriber);

        // Unsubscribe from event, operator overload >>
        Event &operator>>(const std::function<void(params...)> subscriber);

        // Subscribe to event
        Event &Subscribe(const std::function<void(params...)> subscriber);

        // Unsubscribe from event
        Event &Unsubscribe(const std::function<void(params...)> subscriber);

        // Unsubscribe all subscribers
        void UnsubscribeAll() noexcept;

        // Fire event
        void Fire(params... args) const;

		// Get Subscriber Count
		size_t GetSubscriberCount() const noexcept;
    };

    template <typename... params>
    inline Event<params...> &Event<params...>::operator+=(const std::function<void(params...)> subscriber)
    {
        return Subscribe(subscriber);
    }

    template <typename... params>
    inline Event<params...> &Event<params...>::operator<<(const std::function<void(params...)> subscriber)
    {
        return Subscribe(subscriber);
    }

    template <typename... params>
    inline Event<params...> &Event<params...>::operator-=(const std::function<void(params...)> subscriber)
    {
        return Unsubscribe(subscriber);
    }

    template <typename... params>
    inline Event<params...> &Event<params...>::operator>>(const std::function<void(params...)> subscriber)
    {
        return Unsubscribe(subscriber);
    }

    template <typename... params>
    inline Event<params...> &Event<params...>::Subscribe(const std::function<void(params...)> subscriber)
    {
        m_SubscriberList.push_back(subscriber);
        return *this;
    }

    template <typename... params>
    Event<params...> &Event<params...>::Unsubscribe(const std::function<void(params...)> subscriber)
    {
        for (int i = 0; i < m_SubscriberList.size(); i++)
        {
            if (m_SubscriberList[i].target_type() == subscriber.target_type())
            {
                m_SubscriberList.erase(m_SubscriberList.begin() + i);
                break;
            }
        }
        return *this;
    }

    template <typename... params>
    inline void Event<params...>::UnsubscribeAll() noexcept
    {
        m_SubscriberList.clear();
    }

    template <typename... params>
    void Event<params...>::Fire(params... args) const
    {
        for (auto &subscriber : m_SubscriberList)
        {
            subscriber(args...);
        }
    }

	template <typename... params>
	inline size_t Event<params...>::GetSubscriberCount() const noexcept
	{
		return m_SubscriberList.size();
	}
} // namespace ish

#endif