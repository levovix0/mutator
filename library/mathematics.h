#pragma once

#include "basic.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _matrix;

struct matrix_column // ������� �������
{
	_matrix* m; // ��������� �� �������
	i64 x; // ����� �������

	bool min_max(double* mi, double* ma); // ����� ��������
//	double Delta(size_t y = 1); // ������� 2-� ��������� [y] - [y-1]
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _matrix
{
	double* data = nullptr;
	_isize size;

	_matrix() = default;
	_matrix(const _matrix& a) noexcept;
	_matrix(_matrix&& a) noexcept;
	_matrix(i64 ry, i64 rx) noexcept;
	//	matrix(size_t ry, size_t rx, double z) noexcept;// ��������� ������ z
	//	matrix(size_t ry, size_t rx, const std::function<double(size_t, size_t)>& fun) noexcept; // ������ �� ������� fun
	//	matrix(size_t ry) noexcept; // ������
	_matrix(i64 ry, const std::function<double(i64)>& fun) noexcept; // ������ ������ �� �������
	~_matrix() { delete[] data; }

	void operator=(const _matrix& a) noexcept;
	void operator=(_matrix&& a) noexcept;
	//	void operator+=(const matrix& a) noexcept;

	//	void set_diagonal_matrix(size_t n, double dz) noexcept; // ������ ������� ������������� ����

	double* operator[](i64 n) { return data + (n * size.x); } // ������ �������
	void resize(_isize r); // �������� ������ ������� ! ������ �� �����������
//	inline bool empty() const noexcept { return rx_ * ry_ == 0; } // �������� �� ������� ������
//	inline size_t size_x() const noexcept { return rx_; } // ������ �� x
//	inline size_t size_y() const noexcept { return ry_; } // ������ �� y
//	inline size_t size() const noexcept { return ry_ * rx_; } // ������
	matrix_column column(i64 x) { return { this, x }; } // ������� �������

//	void MinMax(double* mi, double* ma); // ����� ��������

//	matrix transpose() const noexcept; // ����������������� �������
//	matrix this_mul_transpose() const noexcept; // ������� ������� �������� �� ����������������� ����
//	matrix pseudoinverse() const noexcept; // �������������� �������

//	void FindAllEigenVectors(matrix& R, matrix& A) const noexcept; // ����������� ������� � ����������� �����
	 // �������� ������������ �� ������ �������������
//	matrix linear_prediction(const matrix& k, size_t start, size_t n, size_t ots = 0) const noexcept;
//	double linear_prediction(const matrix& k) const noexcept; // �������� ������������ �� ������ �������������

//	matrix operator<<(const matrix& a) const noexcept; // ������� ������� (���������� ����� ������ ���� ����������)
//	matrix operator*(const matrix& a) const noexcept;
//	matrix operator-(const matrix& a) const noexcept;

//	void Push(_stack* mem); // ��������� � ����
//	void Pop(_stack* mem); // ������� �� �����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//matrix get_noise(size_t n, double k); // ������������� ���

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _one_stat // ������� ����������
{
	i64 value;   // �������� / ����
	i64 number;  // ����������

	bool operator!=(_one_stat a) const noexcept { return ((value != a.value) || (number != a.number)); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _basic_statistics // ������� ����������� ���������� � ������
{
	std::vector<i64> data;
	i64 start = 0;

	void push(i64 x); // �������� ����� � ����������
	i64 number() const noexcept;
	i64 number(i64 be, i64 en) const noexcept; // ���������� � ��������� [be,en)
	i64 operator[](i64 x) const noexcept;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _statistics // ������ ����������
{
	std::vector<_one_stat> data;
	typedef std::vector<_one_stat>::iterator _it;
	typedef std::vector<_one_stat>::const_iterator _cit;

	_statistics() = default;
	_statistics(const _basic_statistics& a) { *this = a; }

	void clear() { data.clear(); }
	i64 min_value() const noexcept { return (data.empty()) ? 0 : data.front().value; }
	i64 max_value() const noexcept { return (data.empty()) ? 0 : data.back().value; }
	i64 number(_cit be, _cit en) const noexcept; // ���������� � ��������� [be,en)
	i64 number(i64 be, i64 en) const noexcept; // ���������� � ��������� [be,en)
	i64 number() const noexcept { return number(data.begin(), data.end()); } // ����� ����������

	i64 first_zero(); // ����� ������� �������� �������� ������� �� start (-1 ���� �� �������)
	i64 number_not_zero() { return data.size(); } // ���������� �������� � ��������� �����������
	double arithmetic_size(_it be, _it en); // �������������� ������ � �����
	double arithmetic_size() { return arithmetic_size(data.begin(), data.end()); }

	void operator=(const _basic_statistics& a);
	void operator+=(const _statistics& a);
	bool operator==(const _statistics& a) const noexcept;

	i64 operator[](i64 n) const noexcept; // value -> number

	_matrix to_matrix(); // ��� ������� - x � y
	_matrix to_matrix(i64 mi, i64 ma); // ��� ������� - x � y
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
