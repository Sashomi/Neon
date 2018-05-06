#pragma once

#ifndef ZVECTOR3_HPP
#define ZVECTOR3_HPP

namespace Zx
{
	template <typename T>
	class ZVector3
	{
	public :
		ZVector3() = default;
		ZVector3(T newX, T newY, T newZ);
		ZVector3(const ZVector3&);
		ZVector3(ZVector3&&) noexcept;

		~ZVector3() = default;

		void SetVector3(T newX, T newY, T newZ);

		T GetSquaredLenght() const;
		T GetLenght() const;

		const ZVector3& operator+() const;
		ZVector3 operator+(const ZVector3&) const;

		ZVector3 operator-() const;
		ZVector3 operator-(const ZVector3&) const;

		const ZVector3& operator+=(const ZVector3&);
		const ZVector3& operator-=(const ZVector3&);

		ZVector3 operator*(T v) const;
		ZVector3 operator/(T v) const;

		bool operator==(const ZVector3&);
		bool operator!=(const ZVector3&);

		T x, y, z;
	};
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::ZVector3<T>&);

#include "ZVector3.inl"

#endif //ZVECTOR3_HPP