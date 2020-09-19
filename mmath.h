#pragma once
#include <functional>

#include "mgeneral.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct matrix;

struct MatrixColumn                                                       // ������� �������
{
	matrix* m_;                                                           // ��������� �� �������
	size_t x_;                                                            // ����� �������

	void MinMax(double* mi, double* ma);                                  // ����� ��������
	double Delta(size_t y = 1);                                           // ������� 2-� ��������� [y] - [y-1]
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct matrix
{
	friend MatrixColumn;

	matrix() = default;
	matrix(const matrix& a) noexcept;                                     // ����������� �����
	matrix(matrix&& a) noexcept;                                          // ����������� ��������
	matrix(size_t ry, size_t rx) noexcept;                                // ����� ������ ������ �������
	matrix(size_t ry, size_t rx, double z) noexcept;                      // ����� ������ ������ ������� � ��������� ������
	matrix(size_t ry, size_t rx, const std::function<double(size_t, size_t)> &fun ) noexcept; // ����� ������ ������ ������� � ������ �� �������
	matrix(size_t ry) noexcept;                                           // ����� ������ ������ �������
	matrix(size_t ry, const std::function<double(size_t)>& fun) noexcept; // ����� ������ ������ ������� � ������ �� �������
	~matrix();

	void operator=(const matrix& a) noexcept;                             // ������������
	void operator=(matrix&& a) noexcept;                                  // ������������ ���������
	void operator+=(const matrix& a) noexcept;                            // �������� �������

	void set_diagonal_matrix(size_t n, double dz) noexcept;               // ������ ������� ������������� ����

	inline double* operator[](size_t n) { return data_ + (n * rx_); }     // ������ �������
	void resize(size_t ry, size_t rx);                                    // �������� ������ ������ ! ������ �� �����������
	inline bool empty() const noexcept { return rx_ * ry_ == 0; }         // �������� �� ������� ������
	inline size_t size_x() const noexcept { return rx_; }                 // ������ �� x
	inline size_t size_y() const noexcept { return ry_; }                 // ������ �� y
	inline size_t size() const noexcept { return ry_ * rx_; }             // ������
	inline MatrixColumn Column(size_t x) { return { this, x }; }          // ������� �������

	void MinMax(double* mi, double* ma);                                  // ����� ��������

	matrix transpose() const noexcept;                                    // ����������������� �������
	matrix this_mul_transpose() const noexcept;                           // ������� ������� �������� �� ����������������� ����
	matrix pseudoinverse() const noexcept;                                // �������������� �������

	void FindAllEigenVectors(matrix& R, matrix& A) const noexcept;        // ����������� ������� � ����������� �����
	matrix linear_prediction(const matrix &k, size_t start, size_t n, size_t ots = 0) const noexcept; // ��������� ������������ �� ������ �������������
	double linear_prediction(const matrix& k) const noexcept;             // ��������� ������������ �� ������ �������������

	matrix operator<<(const matrix& a) const noexcept;                    // ������� ������� (���������� ����� ������ ���� ����������)
	matrix operator*(const matrix& a) const noexcept;                     // ��������� ������
	matrix operator-(const matrix& a) const noexcept;                     // �������� ������

	void Push(Stack* mem);                                                // ��������� � ����
	void Pop(Stack* mem);                                                 // ������� �� �����

private:
	double* data_ = nullptr;                                              // ������
	size_t ry_ = 0;                                                       // ���������� �����
	size_t rx_ = 0;                                                       // ���������� ��������
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
matrix get_noise(size_t n, double k);                      // ������������� ���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
