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
	RFObject(Ts&... Args)
	{
		m_IObject = new T(std::forward<Ts>(Args)...);
		s_RFObjectLib.push_back(this);
	}

	RFObject(T* object)
	{
		m_IObject = (T*)malloc(sizeof(T));
		std::memcpy(m_IObject, object, sizeof(T));
		s_RFObjectLib.push_back(this);
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
			LOG("Ref count of object: %p is 0", m_IObject);
	}

	uint32_t GetRef() const
	{
		return m_RefCount;
	}

	T* GetIObject() const
	{
		return m_IObject;
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