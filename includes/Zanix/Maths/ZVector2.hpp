#pragma once

#ifndef ZVECTOR2_HPP
#define ZVECTOR2_HPP

#include <iostream>

namespace Zx
{
	template <typename T>
	class ZVector2
	{
	public :
		ZVector2() = default;
		ZVector2(T newX, T newY);
		ZVector2(const ZVector2&);
		ZVector2(ZVector2&&) noexcept;

		~ZVector2() = default;

		void SetVector2(T newX, T newY);

		T GetSquaredLenght() const;
		T GetLenght() const;

		const ZVector2& operator+() const;
		ZVector2 operator+(const ZVector2&) const;
		
		ZVector2 operator-() const;
		ZVector2 operator-(const ZVector2&) const;

		const ZVector2& operator+=(const ZVector2&);
		const ZVector2& operator-=(const ZVector2&);

		ZVector2 operator*(T v) const;
		ZVector2 operator/(T v) const;

		bool operator==(const ZVector2&) const;
		bool operator!=(const ZVector2&) const;

		T x, y;
	};
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::ZVector2<T>&);

#include "ZVector2.inl"

#endif //ZVECTOR2_HPP