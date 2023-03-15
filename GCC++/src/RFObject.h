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
		LOG("RFObject Constructed.");
		m_IObject = new T(std::forward<Ts>(Args)...);
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