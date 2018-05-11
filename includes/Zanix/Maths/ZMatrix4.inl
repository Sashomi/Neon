#include <limits>

#include <Zanix/Maths/ZVector3.hpp>
#include <Zanix/Maths/ZVector4.hpp>

namespace Zx
{
	/*
	@brief : Constructs a ZMatrix4 with the param components
	@param : The components of the ZMatrix4
	*/
	template <typename T>
	ZMatrix4<T>::ZMatrix4(T newC11 = 1, T newC12 = 0, T newC13 = 0, T newC14 = 0,
		T newC21 = 0, T newC22 = 1, T newC23 = 0, T newC24 = 0,
		T newC31 = 0, T newC32 = 0, T newC33 = 1, T newC34 = 0,
		T newC41 = 0, T newC42 = 0, T newC43 = 0, T newC44 = 1) :
		c11(newC11), c12(newC12), c13(newC13), c14(newC14),
		c21(newC21), c22(newC22), c23(newC23), c24(newC24),
		c31(newC31), c32(newC32), c33(newC33), c34(newC34),
		c41(newC41), c42(newC42), c43(newC43), c44(newC44)
	{
	}

	/*
	@brief : Sets this ZMatrix4 to the identity
	*/
	template <typename T>
	void ZMatrix4<T>::SetIdentity()
	{
		c11 = 1; c12 = 0; c13 = 0; c14 = 0;
		c21 = 0; c22 = 1; c23 = 0; c24 = 0;
		c31 = 0; c32 = 0; c33 = 1; c34 = 0;
		c41 = 0; c42 = 0; c43 = 0; c44 = 1;
	}

	/*
	@brief : Returns the determinant of this ZMatrix4
	*/
	template <typename T>
	T ZMatrix4<T>::GetDeterminant() const
	{
		T A = c22 * (c33 * c44 - c43 * c34) - c32 * (c23 * c44 - c43 * c24) + c42 * (c23 * c34 - c33 * c24);
		T B = c12 * (c33 * c44 - c43 * c34) - c32 * (c13 * c44 - c43 * c14) + c42 * (c13 * c34 - c33 * c14);
		T C = c12 * (c23 * c44 - c43 * c24) - c22 * (c13 * c44 - c43 * c14) + c42 * (c13 * c24 - c23 * c14);
		T D = c12 * (c23 * c34 - c33 * c24) - c22 * (c13 * c34 - c33 * c14) + c32 * (c13 * c24 - c23 * c14);

		return c11 * A - c21 * B + c31 * C - c41 * D;
	}

	/*
	@brief : Returns the transposed ZMatrix4 of this ZMatrix4
	*/
	template <typename T>
	ZMatrix4<T> ZMatrix4<T>::GetTranspose() const
	{
		return ZMatrix4<T>(c11, c21, c31, c41,
						c12, c22, c32, c42,
						c13, c23, c33, c43,
						c14, c24, c34, c44);
	}

	/*
	@brief : Returns the inversed ZMatrix4 of this ZMatrix4
	*/
	template <typename T>
	ZMatrix4<T> ZMatrix4<T>::GetInverse() const
	{
		ZMatrix4<T> ret;
		T det = GetDeterminant();

		if (fabs(det) > std::numeric_limits<T>::epsilon())
		{
			ret.c11 = (c22 * (c33 * c44 - c34 * c43) - c32 * (c23 * c44 - c43 * c24) + c42 * (c23 * c34 - c33 * c24)) / det;
			ret.c12 = -(c12 * (c33 * c44 - c43 * c34) - c32 * (c13 * c44 - c43 * c14) + c42 * (c13 * c34 - c33 * c14)) / det;
			ret.c13 = (c12 * (c23 * c44 - c43 * c24) - c22 * (c13 * c44 - c43 * c14) + c42 * (c13 * c24 - c23 * c14)) / det;
			ret.c14 = -(c12 * (c23 * c34 - c33 * c24) - c22 * (c13 * c34 - c33 * c14) + c32 * (c13 * c24 - c23 * c14)) / det;

			ret.c21 = -(c21 * (c33 * c44 - c34 * c43) - c23 * (c31 * c44 - c34 * c41) + c24 * (c31 * c43 - c33 * c41)) / det;
			ret.c22 = (c11 * (c33 * c44 - c34 * c43) - c13 * (c31 * c44 - c34 * c41) + c14 * (c31 * c43 - c33 * c41)) / det;
			ret.c23 = -(c11 * (c23 * c44 - c24 * c43) - c13 * (c21 * c44 - c24 * c41) + c14 * (c21 * c43 - c23 * c41)) / det;
			ret.c24 = (c11 * (c23 * c34 - c24 * c33) - c13 * (c21 * c34 - c24 * c31) + c14 * (c21 * c33 - c23 * c31)) / det;

			ret.c31 = (c21 * (c32 * c44 - c34 * c42) - c22 * (c31 * c44 - c34 * c41) + c24 * (c31 * c42 - c32 * c41)) / det;
			ret.c32 = -(c11 * (c32 * c44 - c34 * c42) - c12 * (c31 * c44 - c34 * c41) + c14 * (c31 * c42 - c32 * c41)) / det;
			ret.c33 = (c11 * (c22 * c44 - c24 * c42) - c12 * (c21 * c44 - c24 * c41) + c14 * (c21 * c42 - c22 * c41)) / det;
			ret.c34 = -(c11 * (c22 * c34 - c24 * c32) - c12 * (c21 * c34 - c24 * c31) + c14 * (c21 * c32 - c22 * c31)) / det;

			ret.c41 = -(c21 * (c32 * c43 - c33 * c42) - c22 * (c31 * c43 - c33 * c41) + c23 * (c31 * c42 - c32 * c41)) / det;
			ret.c42 = (c11 * (c32 * c43 - c33 * c42) - c12 * (c31 * c43 - c33 * c41) + c13 * (c31 * c42 - c32 * c41)) / det;
			ret.c43 = -(c11 * (c22 * c43 - c23 * c42) - c12 * (c21 * c43 - c23 * c41) + c13 * (c21 * c42 - c22 * c41)) / det;
			ret.c44 = (c11 * (c22 * c33 - c23 * c32) - c12 * (c21 * c33 - c23 * c31) + c13 * (c21 * c32 - c22 * c31)) / det;
		}

		return ret;
	}

	/*
	@brief : Sets a translation ZMatrix4
	@param : A constant reference to the ZVector3 represent the translation components
	*/
	template <typename T>
	void ZMatrix4<T>::SetTranslationMatrix(const ZVector3<T>& vec)
	{
		c11 = 1; c12 = 0; c13 = 0; c14 = vec.x;
		c21 = 0; c22 = 1; c23 = 0; c24 = vec.y;
		c31 = 0; c32 = 0; c33 = 1; c34 = vec.z;
		c41 = 0; c42 = 0; c43 = 0; c44 = 1;
	}

	/*
	@brief : Sets the components of this ZMatrix from a scale
	@param : A constant reference to the ZVector3 represent the scaling components
	*/
	template <typename T>
	void ZMatrix4<T>::SetScalingMatrix(const ZVector3<T>& vec)
	{
		c11 = vec.x; c12 = 0; c13 = 0; c14 = 0;
		c21 = 0; c22 = vec.y; c23 = 0; c24 = 0;
		c31 = 0; c32 = 0; c33 = vec.z; c34 = 0;
		c41 = 0; c42 = 0; c43 = 0; c44 = 1;
	}

	/*
	@brief : Sets a rotation in the x component
	@param : The angle of the rotation
	*/
	template <typename T>
	void ZMatrix4<T>::SetRotationX(T angle)
	{
		T cos = std::cos(angle);
		T sin = std::sin(angle);

		c11 = 1; c12 = 0; c13 = 0; c14 = 0;
		c21 = 0; c22 = cos;  c23 = sin;  c24 = 0;
		c31 = 0; c32 = -sin; c33 = cos;  c34 = 0;
		c41 = 0; c42 = 0; c43 = 0; c44 = 1;
	}

	/*
	@brief : Sets a rotation in the y component
	@param : The angle of the rotation
	*/
	template <typename T>
	void ZMatrix4<T>::SetRotationY(T angle)
	{
		T cos = std::cos(angle);
		T sin = std::sin(angle);

		a11 = cos a12 = 0; a13 = -sin;	a14 = 0;
		a21 = 0; a22 = 1; a23 = 0;	a24 = 0;
		a31 = sin a32 = 0; a33 = cos;    a34 = 0;
		a41 = 0; a42 = 0; a43 = 0; a44 = 1;
	}

	/*
	@brief : Sets a rotation in the z component
	@param : The angle of the rotation
	*/
	template <typename T>
	void ZMatrix4<T>::SetRotationZ(T angle)
	{
		T cos = std::cos(angle);
		T cos = std::sin(angle);

		a11 = cos;  a12 = sin;  a13 = 0; a14 = 0;
		a21 = -sin; a22 = cos;  a23 = 0; a24 = 0;
		a31 = 0; a32 = 0; a33 = 1; a34 = 0;
		a41 = 0; a42 = 0; a43 = 0; a44 = 1;
	}

	/*
	@brief : Returns a ZVector3 of the translation of this ZMatrix4
	*/
	template <typename T>
	ZVector3<T> ZMatrix4<T>::GetTranslation() const
	{
		return ZVector3<T>(c14, c24, c34);
	}

	/*
	@brief : Returns a ZVector who is the result of the transform of a ZVector3
	@param : A constant reference to the ZVector3
	@param : The w component
	*/
	template <typename T>
	ZVector3<T> ZMatrix4<T>::Transform(const ZVector3<T>& vec, T w) const
	{
		return ZVector3<T>(vec.x * c11 + vec.y * c21 + vec.z * c31 + w * c41,
						   vec.x * c12 + vec.y * c22 + vec.z * c32 + w * c42,
						   vec.x * c13 + vec.y * c23 + vec.z * c33 + w * c43);
	}

	/*
	@brief : Returns a ZVector who is the result of the transform of a ZVector4
	@param : A constant reference to the ZVector4
	*/
	template <typename T>
	ZVector4<T> ZMatrix4<T>::Transform(const ZVector4<T>& vec) const
	{
		return ZVector4<T>(vec.x * c11 + vec.y * c21 + vec.z * c31 + vec.w * c41,
						 vec.x * c12 + vec.y * c22 + vec.z * c32 + vec.w * c42,
						 vec.x * c13 + vec.y * c23 + vec.z * c33 + vec.w * c43,
						 vec.x * c14 + vec.y * c24 + vec.z * c34 + vec.w * c44);
	}

	/*
	@brief : Sets this ZMatrix4 orthogonal
	@param : The distance between the center and the left
	@param : The distance between the center and the tpp
	@param : The distance between the center and the right
	@param : The distance between the center and the bottom
	*/
	template <typename T>
	void ZMatrix4<T>::SetOrtho(T left, T top, T right, T bottom)
	{
		c11 = 2 / (right - left); c12 = 0;               c13 = 0; c14 = (left + right) / (left - right);
		c21 = 0;               c22 = 2 / (top - bottom); c23 = 0; c24 = (bottom + top) / (bottom - top);
		c31 = 0;               c32 = 0;               c33 = 1; c34 = 0;
		c41 = 0;               c42 = 0;               c43 = 0; c44 = 1;
	}

	/*
	@brief : Returns this ZMatrix4
	*/
	template <typename T>
	ZMatrix4<T> ZMatrix4<T>::operator+() const
	{
		 return (*this);
	}

	/*
	@brief : Returns the negate of this ZMatrix4
	*/
	template <typename T>
	ZMatrix4<T> ZMatrix4<T>::operator-() const
	{
		return ZMatrix4<T>(-c11, -c12, -c13, -c14,
						   -c21, -c22, -c23, -c24,
						   -c31, -c32, -c33, -c34,
					       -c41, -c42, -c43, -c44);
	}

	/*
	@brief : Returns the result of the addition of this ZMatrix4 and another ZMatrix4
	@param : A constant reference to the other ZMatrix4
	*/
	template <typename T>
	ZMatrix4<T> ZMatrix4<T>::operator+(const ZMatrix4<T>& m) const
	{
		return ZMatrix4<T>(c11 + m.c11, c12 + m.c12, c13 + m.c13, c14 + m.c14,
						   c21 + m.c21, c22 + m.c22, c23 + m.c23, c24 + m.c24,
						   c31 + m.c31, c32 + m.c32, c33 + m.c33, c34 + m.c34,
					       c41 + m.c41, c42 + m.c42, c43 + m.c43, c44 + m.c44);
	}

	/*
	@brief : Returns the result of the substract of this ZMatrix4 and another ZMatrix4
	@param : A constant reference to the other ZMatrix4
	*/
	template <typename T>
	ZMatrix4<T> ZMatrix4<T>::operator-(const ZMatrix4<T>& m) const
	{
		return ZMatrix4<T>(c11 - m.c11, c12 - m.c12, c13 - m.c13, c14 - m.c14,
						   c21 - m.c21, c22 - m.c22, c23 - m.c23, c24 - m.c24,
		                   c31 - m.c31, c32 - m.c32, c33 - m.c33, c34 - m.c34,
			               c41 - m.c41, c42 - m.c42, c43 - m.c43, c44 - m.c44);
	}

	/*
	@brief : Adds to this ZMatrix4 another ZMatrix4 (macro +=)
	@param : A constant reference to the other ZMatrix4
	*/
	template <typename T>
	const ZMatrix4<T>& ZMatrix4<T>::operator+=(const ZMatrix4<T>& m)
	{
		c11 += m.c11; c12 += m.c12; c13 += m.c13; c14 += m.c14;
		c21 += m.c21; c22 += m.c22; c23 += m.c23; c24 += m.c24;
		c31 += m.c31; c32 += m.c32; c33 += m.c33; c34 += m.c34;
		c41 += m.c41; c42 += m.c42; c43 += m.c43; c44 += m.c44;

		return (*this);
	}

	/*
	@brief : Substracts to this ZMatrix4 another ZMatrix4 (macro -=)
	@param : A constant reference to the other ZMatrix4
	*/
	template <typename T>
	const ZMatrix4<T>& ZMatrix4<T>::operator-=(const ZMatrix4<T>& m)
	{
		c11 -= m.c11; c12 -= m.c12; c13 -= m.c13; c14 -= m.c14;
		c21 -= m.c21; c22 -= m.c22; c23 -= m.c23; c24 -= m.c24;
		c31 -= m.c31; c32 -= m.c32; c33 -= m.c33; c34 -= m.c34;
		c41 -= m.c41; c42 -= m.c42; c43 -= m.c43; c44 -= m.c44;

		return (*this);
	}

	/*
	@brief : Returns the result of the multiplication of this ZMatrix4 and another ZMatrix4
	@param : A constant reference to the other ZMatrix4
	*/
	template <typename T>
	ZMatrix4<T> ZMatrix4<T>::operator*(const ZMatrix4<T>& m) const
	{
		return ZMatrix4<T>(c11 * m.c11 + c21 * m.c12 + c31 * m.c13 + c41 * m.c14,
					       c12 * m.c11 + c22 * m.c12 + c32 * m.c13 + c42 * m.c14,
						   c13 * m.c11 + c23 * m.c12 + c33 * m.c13 + c43 * m.c14,
						   c14 * m.c11 + c24 * m.c12 + c34 * m.c13 + c44 * m.c14,

						   c11 * m.c21 + c21 * m.c22 + c31 * m.c23 + c41 * m.c24,
						   c12 * m.c21 + c22 * m.c22 + c32 * m.c23 + c42 * m.c24,
						   c13 * m.c21 + c23 * m.c22 + c33 * m.c23 + c43 * m.c24,
						   c14 * m.c21 + c24 * m.c22 + c34 * m.c23 + c44 * m.c24,

						   c11 * m.c31 + c21 * m.c32 + c31 * m.c33 + c41 * m.c34,
						   c12 * m.c31 + c22 * m.c32 + c32 * m.c33 + c42 * m.c34,
						   c13 * m.c31 + c23 * m.c32 + c33 * m.c33 + c43 * m.c34,
						   c14 * m.c31 + c24 * m.c32 + c34 * m.c33 + c44 * m.c34,

						   c11 * m.c41 + c21 * m.c42 + c31 * m.c43 + c41 * m.c44,
						   c12 * m.c41 + c22 * m.c42 + c32 * m.c43 + c42 * m.c44,
						   c13 * m.c41 + c23 * m.c42 + c33 * m.c43 + c43 * m.c44,
						   c14 * m.c41 + c24 * m.c42 + c34 * m.c43 + c44 * m.c44);
	}

	/*
	@brief : Returns the result of the multiplication of this ZMatrix4 and another ZMatrix4 (macro *=)
	@param : A constant reference to the other ZMatrix4
	*/
	template <typename T>
	const ZMatrix4<T>& ZMatrix4<T>::operator*=(const ZMatrix4<T>& m)
	{
		*this = *this * m;

		return (*this);
	}

	/*
	@brief : Returns the result of the multiplication of this ZMatrix4 and a scale (macro *=)
	@param : The scale
	*/
	template <typename T>
	const ZMatrix4<T>& ZMatrix4<T>::operator*=(T scale)
	{
		c11 *= scale; c12 *= scale; c13 *= scale; c14 *= scale;
		c21 *= scale; c22 *= scale; c23 *= scale; c24 *= scale;
		c31 *= scale; c32 *= scale; c33 *= scale; c34 *= scale;
		c41 *= scale; c42 *= scale; c43 *= scale; c44 *= scale;

		return (*this);
	}

	/*
	@brief : Returns the result of the division of this ZMatrix4 and a scale (macro /=)
	@param : The scale
	*/
	template <typename T>
	const ZMatrix4<T>& ZMatrix4<T>::operator/=(T scale)
	{
		c11 /= scale; c12 /= scale; c13 /= scale; c14 /= scale;
		c21 /= scale; c22 /= scale; c23 /= scale; c24 /= scale;
		c31 /= scale; c32 /= scale; c33 /= scale; c34 /= scale;
		c41 /= scale; c42 /= scale; c43 /= scale; c44 /= scale;

		return (*this);
	}

	/*
	@brief : Compares two ZMatrix4, return true if they are equal, false otherwise
	@param : The ZMatrix4 to compare
	*/
	template <typename T>
	bool ZMatrix4<T>::operator==(const ZMatrix4<T>& m) const
	{
		return ((fabs(c11 - m.c11) < std::numeric_limits<T>::epsilon()) && (fabs(c12 - m.c12) < std::numeric_limits<T>::epsilon()) &&
				(fabs(c13 - m.c13) < std::numeric_limits<T>::epsilon()) && (fabs(c14 - m.c14) < std::numeric_limits<T>::epsilon()) &&
				(fabs(c21 - m.c21) < std::numeric_limits<T>::epsilon()) && (fabs(c22 - m.c22) < std::numeric_limits<T>::epsilon()) &&
				(fabs(c23 - m.c23) < std::numeric_limits<T>::epsilon()) && (fabs(c24 - m.c24) < std::numeric_limits<T>::epsilon()) &&
				(fabs(c31 - m.c31) < std::numeric_limits<T>::epsilon()) && (fabs(c32 - m.c32) < std::numeric_limits<T>::epsilon()) &&
				(fabs(c33 - m.c33) < std::numeric_limits<T>::epsilon()) && (fabs(c34 - m.c34) < std::numeric_limits<T>::epsilon()) &&
				(fabs(c41 - m.c41) < std::numeric_limits<T>::epsilon()) && (fabs(c42 - m.c42) < std::numeric_limits<T>::epsilon()) &&
				(fabs(c43 - m.c43) < std::numeric_limits<T>::epsilon()) && (fabs(c44 - m.c44) < std::numeric_limits<T>::epsilon()));
	}

	/*
	@brief : Compares two ZMatrix4, return true if they are not equal, false otherwise
	@param : The ZMatrix4 to compare
	*/
	template <typename T>
	bool ZMatrix4<T>::operator!=(const ZMatrix4<T>& m) const
	{
		return !(*this == m);
	}

	/*
	@brief : Returns a reference to the component (i, j)
	*/
	template <typename T>
	T& ZMatrix4<T>::operator()(std::size_t i, std::size_t j)
	{
		return (&c11)[j * 4 + i];
	}

	/*
	@brief : Returns the component (i, j)
	*/
	template <typename T>
	T ZMatrix4<T>::operator ()(std::size_t i, std::size_t j) const
	{
		return (&c11)[j * 4 + i];
	}

	/*
	@brief : Returns a constant pointer to the ZMatrix4 components
	*/
	template <typename T>
	ZMatrix4<T>::operator const T*() const
	{
		return &c11;
	}

	/*
	@brief : Returns a pointer to the ZMatrix4 components
	*/
	template <typename T>
	ZMatrix4<T>::operator T*()
	{
		return &c11;
	}
}

/*
@brief : Displays on the output stream a ZMatrix4
@param : The output stream
@param : The ZMatrix4 to display
*/
template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::ZMatrix4<T>& m)
{
	stream << m.c11 << " " << m.c12 << " " << m.c13 << " " << m.c14 << std::endl;
	stream << m.c21 << " " << m.c22 << " " << m.c23 << " " << m.c24 << std::endl;
	stream << m.c31 << " " << m.c32 << " " << m.c33 << " " << m.c34 << std::endl;
	stream << m.c41 << " " << m.c42 << " " << m.c43 << " " << m.c44 << std::endl;

	return stream;
}