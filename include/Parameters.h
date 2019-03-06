#pragma once
namespace Parameters {
	enum DataType { DataTypeUniform, DataTypeZipfian, DataTypeCounts };

	class Tests {
		Tests() = delete;

	public:
		const DataType type;
		const unsigned int n_groups;
		const unsigned int n_data;
		const double theta;
		constexpr Tests(unsigned int n_groups, unsigned int n_data, double theta, DataType type = DataTypeZipfian) noexcept
			: n_groups(n_groups), n_data(n_data), theta(theta), type(type) { }
	};
	constexpr unsigned int n_tests = 8;
	constexpr Tests tests[n_tests] = { {10,100000000,1.f}, {10,100000000,1.5f}, {10,100000000,2.f},
		  {10,100000000,2.5f}, {10,100000000,3.f}, {10,100000000,3.5f}, {20,100000000,1.f}, {3, 123456789, 6.f} };
	constexpr unsigned int n_groups = 3;
	constexpr unsigned int n_data = 123456789;
}