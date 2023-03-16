#include "RFObject.h"
#include "Ref.h"

struct Specification
{
	int ID;
	std::string Name;
	bool Enabled;
};

class TestApp
{
public:
	TestApp(Specification& specification) : m_Specification(specification)
	{

	}

	void Start()
	{

	}
	Specification m_Specification;
private:
};

void TestCopy(Ref<TestApp> application)
{
	application->m_Specification.ID = 2;
	Ref<TestApp> application1 = application;
	application1->m_Specification.ID = 3;
}

void TestRef(Ref<TestApp>& application)
{
	Ref<TestApp> application1 = application;
}

int main()
{
	Specification specification;
	specification.ID = 1;
	specification.Name = "Sample App";
	specification.Enabled = true;

	{
		Ref<TestApp> application = MakeRef<TestApp>(specification);
		application->Start();

		TestCopy(application);

		TestRef(application);

		Ref<TestApp> application1 = application;
		application1->m_Specification.ID = 2;
		Ref<TestApp>& application2 = application;
		application2->m_Specification.ID = 3;
	}

	{
		std::shared_ptr<TestApp> application = std::make_shared<TestApp>(specification);
	}

	return 0;
}