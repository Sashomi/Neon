#include <Zanix/Core/ZException.hpp>

namespace Zx
{
	/*
	@brief : Constructs a ZVector4 with 2 components
	@param : The first component (x)
	@param : The second component (y)
	*/
	template <typename T>
	ZVector4<T>::ZVector4(T newX, T newY, T newZ, T newW) : x(newX), y(newY), z(newZ), w(newW)
	{
	}

	/*
	@brief : Copy constructor : Copy another ZVector4
	@param : A constant reference to the ZVector4 to copy
	*/
	template <typename T>
	ZVector4<T>::ZVector4(const ZVector4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w)
	{
	}

	/*
	@brief : Movements constructor : Move the component of another ZVector4
	@param : A constant reference to the ZVector4 to move
	*/
	template <typename T>
	ZVector4<T>::ZVector4(ZVector4&& vec) noexcept
	{
		std::swap(x, vec.x);
		std::swap(y, vec.y);
		std::swap(z, vec.z);
		std::swap(w, vec.w);
	}

	/*
	@brief : Changes the value of the components (x, y)
	@param : The new value of the x component
	@param : The new value of the y component
	*/
	template <typename T>
	void ZVector4<T>::SetVector4(T newX, T newY, T newZ, T newW)
	{
		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	/*
	@brief : Returns the squared lenght of a ZVector4 (x² + y²)
	*/
	template <typename T>
	T ZVector4<T>::GetSquaredLenght() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	/*
	@brief : Returns the lenght of a ZVector4 (sqrt(x² + y²))
	*/
	template <typename T>
	T ZVector4<T>::GetLenght() const
	{
		return (std::sqrt(GetSquaredLenght()));
	}

	/*
	@brief : Returns a constant reference to this ZVector4
	*/
	template <typename T>
	const ZVector4<T>& ZVector4<T>::operator+() const
	{
		return (*this);
	}

	/*
	@brief : Returns the addition with this ZVector4 and another ZVector4
	@param : A constant reference to the second ZVector4 to addition
	*/
	template <typename T>
	ZVector4<T> ZVector4<T>::operator+(const ZVector4& vec) const
	{
		return (ZVector4<T>(x + vec.x, y + vec.y, z + vec.z, w + vec.w));
	}

	/*
	@brief : Returns the negates of the x and y components
	*/
	template <typename T>
	ZVector4<T> ZVector4<T>::operator-() const
	{
		return (ZVector4<T>(-x, -y, -z, -w));
	}

	/*
	@brief : Returns a the difference with this ZVector4 and another ZVector4
	@param : A constant reference to the second ZVector4 to substract
	*/
	template <typename T>
	ZVector4<T> ZVector4<T>::operator-(const ZVector4& vec) const
	{
		return (ZVector4<T>(x - vec.x, y - vec.y, z - vec.z, w - vec.w));
	}

	/*
	@brief : Returns a constant reference of the addition of this ZVector4 and another ZVector4 (Macro +=)
	@param : A constant reference to the second ZVector4 to addition
	*/
	template <typename T>
	const ZVector4<T>& ZVector4<T>::operator+=(const ZVector4& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;

		return (*this);
	}

	/*
	@brief : Returns a constant reference of the substract of this ZVector4 and another ZVector4 (Macro -=)
	@param : A constant reference to the second ZVector4 to substract
	*/
	template <typename T>
	const ZVector4<T>& ZVector4<T>::operator-=(const ZVector4& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return (*this);
	}

	/*
	@brief : Returns the result of the multiplication of this ZVector4 with a scalar
	@param : The value to multiply
	*/
	template <typename T>
	ZVector4<T> ZVector4<T>::operator*(T v) const
	{
		return (ZVector4<T>(x * v, y * v, z * v, w * v));
	}

	/*
	@brief : Returns the result of the division of this ZVector4 with a scalar
	@param : The value to divide
	*/
	template <typename T>
	ZVector4<T> ZVector4<T>::operator/(T v) const
	{
		ZAssert(v == 0, "Division by zero");
		return (ZVector4<T>(x / v, y / v, z / v, w / v));
	}

	/*
	@brief : Returns true if this ZVector4 is equal with an another ZVector4, false otherwise
	@param : A constant reference to the other ZVector4 to compare
	*/
	template <typename T>
	bool ZVector4<T>::operator==(const ZVector4& vec) const
	{
		return ((x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w));
	}

	/*
	@brief : Returns true if this ZVector4 is different with an another ZVector4, false otherwise
	@param : A constant reference to the other ZVector4 to compare
	*/
	template <typename T>
	bool ZVector4<T>::operator!=(const ZVector4& vec) const
	{
		return ((x != vec.x) && (y != vec.y) && (z != vec.z) && (w != vec.w));
	}
}

/*
@brief : Displays in the out stream a ZVector4
@param : The stream to display
@param : A constant reference to the ZVector4 to display
*/
template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::ZVector4<T>& vec)
{
	return (stream << "Vector : (" << vec.x << " ; " << vec.y << " ; " << vec.z << " ; " << vec.w << " )");
}