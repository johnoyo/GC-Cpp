#pragma once

#include "Log.h"
#include "RFObject.h"

#include <string>
#include <utility>
#include <thread>
#include <chrono>

template<typename T>
class Ref
{
public:
	Ref() = default;

	Ref(const Ref& otherRef)
	{
		m_RFObject = otherRef.m_RFObject;
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

	T& operator*()
	{
		return *(m_RFObject->GetIObject());
	}

	void operator=(const Ref<T>& otherRef)
	{
		if (m_RFObject)
			m_RFObject->DecrRef();

		m_RFObject = otherRef.m_RFObject;
		m_RFObject->IncrRef();
	}

	void operator=(T* otherRef)
	{
		m_RFObject->DecrRef();

		auto& library = RFObject<T>::GetRFObjectLib();

		for (auto& obj : library)
		{
			if (obj->GetIObject() == otherRef)
			{
				m_RFObject = obj;
				m_RFObject->IncrRef();

				return;
			}
		}

		m_RFObject->SetIObject(otherRef);
		m_RFObject->IncrRef();
	}

	void operator[](int index)
	{
		LOG("operator[]");
	}

private:
	RFObject<T>* m_RFObject = nullptr;
};

template<typename T, typename... Ts>
static Ref<T> MakeRef(Ts&&... Args)
{
	RFObject<T>* rfObject = new RFObject<T>(std::forward<Ts>(Args)...);
	Ref<T> ref;
	ref.Set(rfObject);
	return ref;
}