#pragma once

#ifndef ZMATRIX4_HPP
#define ZMATRIX4_HPP

namespace Zx
{
	template <typename T>
	class ZVector3;

	template <typename T>
	class ZVector4;

	//Inspired by Laurent Gomila work
	template <typename T>
	class ZMatrix4
	{
	public :
		ZMatrix4(T newC11 = 1.0f, T C12 = 0.0f, T C13 = 0.0f, T C14 = 0.0f,
			T C21 = 0.0f, T C22 = 1.0f, T C23 = 0.0f, T C24 = 0.0f,
			T C31 = 0.0f, T C32 = 0.0f, T C33 = 1.0f, T C34 = 0.0f,
			T C41 = 0.0f, T C42 = 0.0f, T C43 = 0.0f, T C44 = 1.0f);

		void SetIdentity();

		T GetDeterminant() const;

		ZMatrix4 GetTranspose() const;
		ZMatrix4 GetInverse() const;

		void SetTranslationMatrix(const ZVector3<T>&);
		void SetScalingMatrix(const ZVector3<T>&);
		void SetRotationX(T angle);
		void SetRotationY(T angle);
		void SetRotationZ(T angle);

		ZVector3<T> GetTranslation() const;


		ZVector3<T> Transform(const ZVector3<T>&, T w = 1) const;
		ZVector4<T> Transform(const ZVector4<T>&) const;

		void SetOrtho(T left, T top, T right, T bottom);

		ZMatrix4 operator+() const;
		ZMatrix4 operator-() const;

		ZMatrix4 operator+(const ZMatrix4&) const;
		ZMatrix4 operator-(const ZMatrix4&) const;

		const ZMatrix4& operator+=(const ZMatrix4& m);
		const ZMatrix4& operator-=(const ZMatrix4& m);

		ZMatrix4 operator*(const ZMatrix4& m) const;
		const ZMatrix4& operator*=(const ZMatrix4& m);

		const ZMatrix4& operator*=(T scale);
		const ZMatrix4& operator/=(T scale);

		bool operator==(const ZMatrix4& m) const;
		bool operator!=(const ZMatrix4& m) const;

		T& operator ()(std::size_t i, std::size_t j);
		T operator ()(std::size_t i, std::size_t j) const;


		operator T*();
		operator const T*() const;


		T c11, c21, c31, c41,
		  c12, c22, c32, c42,
		  c13, c23, c33, c43,
		  c14, c24, c34, c44;
	};
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::ZMatrix4<T>& m);

#include "ZMatrix4.inl"

#endif //ZMATRIX4_HPP