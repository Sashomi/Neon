#pragma once

#ifndef ZVECTOR3_HPP
#define ZVECTOR3_HPP

namespace Zx
{
	template <typename T>
	class Vector3
	{
	public :
		Vector3() = default;
		Vector3(T newX, T newY, T newZ);
		Vector3(const Vector3&);
		Vector3(Vector3&&) noexcept;

		~Vector3() = default;

		void SetVector3(T newX, T newY, T newZ);

		T GetSquaredLenght() const;
		T GetLenght() const;

		const Vector3& operator+() const;
		Vector3 operator+(const Vector3&) const;

		Vector3 operator-() const;
		Vector3 operator-(const Vector3&) const;

		const Vector3& operator+=(const Vector3&);
		const Vector3& operator-=(const Vector3&);

		Vector3 operator*(T v) const;
		Vector3 operator/(T v) const;

		bool operator==(const Vector3&);
		bool operator!=(const Vector3&);

		T x, y, z;
	};
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::Vector3<T>&);

#include "Vector3.inl"

#endif //ZVECTOR3_HPP