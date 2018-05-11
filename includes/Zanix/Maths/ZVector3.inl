#include <Zanix/Core/ZException.hpp>

namespace Zx
{
	/*
	@brief : Constructs a ZVector3 with 3 components
	@param : The first component (x)
	@param : The second component (y)
	@param : The third component (z)
	*/
	template <typename T>
	ZVector3<T>::ZVector3(T newX, T newY, T newZ) : x(newX), y(newY), z(newZ)
	{
	}

	/*
	@brief : Copy constructor : Copy another ZVector3
	@param : A constant reference to the ZVector3 to copy
	*/
	template <typename T>
	ZVector3<T>::ZVector3(const ZVector3& vec) : x(vec.x), y(vec.y), z(vec.z)
	{
	}

	/*
	@brief : Movement constructor : Moves the component of another ZVector3
	@param : The ZVector3 to move
	*/
	template <typename T>
	ZVector3<T>::ZVector3(ZVector3&& vec) noexcept
	{
		std::swap(x, vec.x);
		std::swap(y, vec.y);
		std::swap(z, vec.z);
	}

	/*
	@brief : Changes the value of the components (x, y)
	@param : The new value of the x component
	@param : The new value of the y component
	@param : The new value of the z component
	*/
	template <typename T>
	void ZVector3<T>::SetVector3(T newX, T newY, T newZ) 
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	/*
	@brief : Returns the squared lenght of a ZVector3(x² + y² + z²)
	*/
	template <typename T>
	T ZVector3<T>::GetSquaredLenght() const
	{
		return (x * x + y * y + z * z);
	}

	/*
	@brief : Returns the lenght of a ZVector3 (sqrt(x² + y² + z²))
	*/
	template <typename T>
	T ZVector3<T>::GetLenght() const
	{
		return std::sqrt(GetSquaredLenght());
	}

	/*
	@brief : Returns a constant reference to this ZVector3
	*/
	template <typename T>
	const ZVector3<T>& ZVector3<T>::operator+() const
	{
		return (*this);
	}

	/*
	@brief : Returns the addition with this ZVector3 and another ZVector3
	@param : A constant reference to the second ZVector3 to addition
	*/
	template <typename T>
	ZVector3<T> ZVector3<T>::operator+(const ZVector3& vec) const
	{
		return (ZVector3<T>(x + vec.x, y + vec.y, z + vec.z));
	}

	/*
	@brief : Returns the negates of the x, y and z components
	*/
	template <typename T>
	ZVector3<T> ZVector3<T>::operator-() const
	{
		return (ZVector3<T>(-x, -y, -z));
	}

	/*
	@brief : Returns a the difference with this ZVector3 and another ZVector3
	@param : A reference constant to thhe second ZVector3 to substract
	*/
	template <typename T>
	ZVector3<T> ZVector3<T>::operator-(const ZVector3& vec) const
	{
		return (ZVector3<T>(x - vec.x, y - vec.y, z - vec.z));
	}

	/*
	@brief : Returns a constant reference of the addition of this ZVector3 and another ZVector3 (Macro +=)
	@param : A constant reference to the second ZVector3 to addition
	*/
	template <typename T>
	const ZVector3<T>& ZVector3<T>::operator+=(const ZVector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return (*this);
	}

	/*
	@brief : Returns a constant reference of the substract of this ZVector3 and another ZVector3 (Macro -=)
	@param : A constant reference to the second ZVector3 to substract
	*/
	template <typename T>
	const ZVector3<T>& ZVector3<T>::operator-=(const ZVector3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return (*this);
	}

	/*
	@brief : Returns the result of the multiplication of this ZVector3 with a scalar
	@param : The value to multiply
	*/
	template <typename T>
	ZVector3<T> ZVector3<T>::operator*(T v) const
	{
		return (ZVector3<T>(x * v, y * v, z * v));
	}

	/*
	@brief : Returns the result of the division of this ZVector3 with a scalar
	@param : The value to divide
	*/
	template <typename T>
	ZVector3<T> ZVector3<T>::operator/(T v) const
	{
		ZAssert(v == 0, "Division by zero");
		return (ZVector3<T>(x / v, y / v, z / v));
	}

	/*
	@brief : Returns true if this ZVector3 is equal with an another ZVector3, false otherwise
	@param : A constant reference to the other ZVector3 to compare
	*/
	template <typename T>
	bool ZVector3<T>::operator==(const ZVector3& vec)
	{
		return ((x == vec.x) && (y == vec.y) && (z == vec.z));
	}

	/*
	@brief : Returns true if this ZVector3 is different with an another ZVector3, false otherwise
	@param : A constant reference to the other ZVector3 to compare
	*/
	template <typename T>
	bool ZVector3<T>::operator!=(const ZVector3& vec)
	{
		return ((x != vec.x) && (y != vec.y) && (z != vec.z));
	}
}

/*
@brief : Displays in the out stream a ZVector3
@param : The stream to display
@param : A constant reference to the ZVector3 to display
*/
template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::ZVector3<T>& vec)
{
	return (stream << "Vector : (" << vec.x << " ; " << vec.y << " ; " << vec.z << " )");
}