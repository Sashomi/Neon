#pragma once

#ifndef ZVector4_HPP
#define ZVector4_HPP

#include <iostream>

namespace Zx
{
	template <typename T>
	class ZVector4
	{
	public:
		ZVector4() = default;
		ZVector4(T newX, T newY, T newZ, T newW = 1);
		ZVector4(const ZVector4&);
		ZVector4(ZVector4&&) noexcept;

		~ZVector4() = default;

		void SetVector4(T newX, T newY, T newZ, T newW);

		T GetSquaredLenght() const;
		T GetLenght() const;

		const ZVector4& operator+() const;
		ZVector4 operator+(const ZVector4&) const;

		ZVector4 operator-() const;
		ZVector4 operator-(const ZVector4&) const;

		const ZVector4& operator+=(const ZVector4&);
		const ZVector4& operator-=(const ZVector4&);

		ZVector4 operator*(T v) const;
		ZVector4 operator/(T v) const;

		bool operator==(const ZVector4&) const;
		bool operator!=(const ZVector4&) const;

		T x, y, z, w;
	};
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::ZVector4<T>&);

#include "ZVector4.inl"

#endif //ZVector4_HPP