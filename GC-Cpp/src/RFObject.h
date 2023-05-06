#pragma once

#include "Log.h"

#include <string>
#include <vector>
#include <utility>

template<typename T>
class RFObject
{
public:
	template <typename... Ts>
	RFObject(Ts&&... Args)
	{
		m_IObject = new T(std::forward<Ts>(Args)...);
		m_RefCount++;
		s_RFObjectLib.push_back(this);
	}

	~RFObject()
	{
		if (m_IObject != nullptr)
		{
			LOG("   Freeing memory of object: %p", m_IObject);
			delete m_IObject;
		}
	}

	void IncrRef()
	{
		m_RefCount++;
	}

	void DecrRef()
	{
		if (m_RefCount)
			m_RefCount--;

		if (m_RefCount == 0)
		{
			LOG("Ref count of object: %p is 0", m_IObject);
			LOG("Freeing memory of object: %p", this);
			delete this;
		}
	}

	uint32_t GetRef() const
	{
		return m_RefCount;
	}

	T* GetIObject() const
	{
		return m_IObject;
	}

	void SetIObject(T* object)
	{
		m_IObject = object;
	}

	static std::vector<RFObject*>& GetRFObjectLib()
	{
		return s_RFObjectLib;
	}

private:
	uint32_t m_RefCount = 0;
	T* m_IObject = nullptr;
	inline static std::vector<RFObject*> s_RFObjectLib;
};