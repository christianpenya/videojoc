#include <gtest/gtest.h>

#include <memory>

namespace unit_test
{
	class Valor
	{
	public:
		Valor() :valor(0)
		{
			m_valor.reset(new Valor);
		};
		~Valor()
		{
			valor = 0;
		}

	private:
		unsigned int valor;
		std::unique_ptr<Valor> m_valor;
	};

	TEST(TestRAII1)
	{

	}
}