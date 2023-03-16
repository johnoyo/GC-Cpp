#pragma once

#include "Log.h"
#include "RFObject.h"

#include <string>
#include <utility>

template<typename T>
class Ref
{
public:
	Ref()
	{

	}

	Ref(const Ref& otherRef)
	{
		m_RFObject = new RFObject<T>(otherRef.m_RFObject->GetIObject());
		m_RFObject->IncrRef();
	}

	~Ref()
	{
		if (m_RFObject)
		{
			m_RFObject->DecrRef();
		}
	}

	void Set(RFObject<T>* rfObject)
	{
		m_RFObject = rfObject;
	}

	auto Get()
	{
		return m_RFObject->GetIObject();
	}

	auto operator->()
	{
		return m_RFObject->GetIObject();
	}

private:
	RFObject<T>* m_RFObject = nullptr;
};

template<typename T, typename... Ts>
static Ref<T> MakeRef(Ts... Args)
{
	RFObject<T>* rfObject = new RFObject<T>(std::forward<Ts>(Args)...);
	Ref<T> ref;
	ref.Set(rfObject);
	return ref;
}