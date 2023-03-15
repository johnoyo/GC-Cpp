#pragma once

#include "Log.h"

#include <string>
#include <utility>

template<typename T>
class Ref
{
public:
	Ref()
	{
		if (m_RFObject)
		{
			LOG("Constructor: Ref count -> %d.", m_RFObject->GetRef());
		}
	}

	Ref(const Ref& ref)
	{
		if (m_RFObject == nullptr)
			m_RFObject = ref.m_RFObject;

		m_RFObject->IncrRef();
		LOG("Copy Constructor: Ref count -> %d.", m_RFObject->GetRef());
	}

	~Ref()
	{
		if (m_RFObject)
		{
			m_RFObject->DecrRef();
			LOG("Destructor: Ref count -> %d.", m_RFObject->GetRef());
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
		LOG("Operator -> called.");
		return m_RFObject->GetIObject();
	}

private:
	RFObject<T>* m_RFObject = nullptr;
};

template<typename T, typename... Ts>
static Ref<T>& MakeRef(Ts... Args)
{
	RFObject<T>* rfObject = new RFObject<T>(std::forward<Ts>(Args)...);
	Ref<T> ref;
	ref.Set(rfObject);
	return ref;
}