#pragma once

#ifndef ZVECTOR2_HPP
#define ZVECTOR2_HPP

#include <iostream>

namespace Zx
{
	template <typename T>
	class Vector2
	{
	public :
		Vector2() = default;
		Vector2(T newX, T newY);
		Vector2(const Vector2&);
		Vector2(Vector2&&) noexcept;

		~Vector2() = default;

		void SetVector2(T newX, T newY);

		T GetSquaredLenght() const;
		T GetLenght() const;

		const Vector2& operator+() const;
		Vector2 operator+(const Vector2&) const;
		
		Vector2 operator-() const;
		Vector2 operator-(const Vector2&) const;

		const Vector2& operator+=(const Vector2&);
		const Vector2& operator-=(const Vector2&);

		Vector2 operator*(T v) const;
		Vector2 operator/(T v) const;

		bool operator==(const Vector2&) const;
		bool operator!=(const Vector2&) const;

		T x, y;
	};
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::Vector2<T>&);

#include "Vector2.inl"

#endif //ZVECTOR2_HPP