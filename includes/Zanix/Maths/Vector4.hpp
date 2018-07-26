#pragma once

#ifndef Vector4_HPP
#define Vector4_HPP

#include <iostream>

namespace Zx
{
	template <typename T>
	class Vector4
	{
	public:
		Vector4() = default;
		Vector4(T newX, T newY, T newZ, T newW = 1);
		Vector4(const Vector4&);
		Vector4(Vector4&&) noexcept;

		~Vector4() = default;

		void SetVector4(T newX, T newY, T newZ, T newW);

		T GetSquaredLenght() const;
		T GetLenght() const;

		const Vector4& operator+() const;
		Vector4 operator+(const Vector4&) const;

		Vector4 operator-() const;
		Vector4 operator-(const Vector4&) const;

		const Vector4& operator+=(const Vector4&);
		const Vector4& operator-=(const Vector4&);

		Vector4 operator*(T v) const;
		Vector4 operator/(T v) const;

		bool operator==(const Vector4&) const;
		bool operator!=(const Vector4&) const;

		T x, y, z, w;
	};
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::Vector4<T>&);

#include "Vector4.inl"

#endif //Vector4_HPP
