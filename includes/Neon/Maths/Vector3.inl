#include <Neon/Core/Exception.hpp>

namespace Zx
{
	/*
	@brief : Constructs a Vector3 with 3 components
	@param : The first component (x)
	@param : The second component (y)
	@param : The third component (z)
	*/
	template <typename T>
	Vector3<T>::Vector3(T newX, T newY, T newZ) : x(newX), y(newY), z(newZ)
	{
	}

	/*
	@brief : Copy constructor : Copy another Vector3
	@param : A constant reference to the Vector3 to copy
	*/
	template <typename T>
	Vector3<T>::Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z)
	{
	}

	/*
	@brief : Movement constructor : Moves the component of another Vector3
	@param : The Vector3 to move
	*/
	template <typename T>
	Vector3<T>::Vector3(Vector3&& vec) noexcept
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
	void Vector3<T>::SetVector3(T newX, T newY, T newZ) 
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	/*
	@brief : Returns the squared lenght of a Vector3(x² + y² + z²)
	*/
	template <typename T>
	T Vector3<T>::GetSquaredLenght() const
	{
		return (x * x + y * y + z * z);
	}

	/*
	@brief : Returns the lenght of a Vector3 (sqrt(x² + y² + z²))
	*/
	template <typename T>
	T Vector3<T>::GetLenght() const
	{
		return std::sqrt(GetSquaredLenght());
	}

	/*
	@brief : Returns a constant reference to this Vector3
	*/
	template <typename T>
	const Vector3<T>& Vector3<T>::operator+() const
	{
		return (*this);
	}

	/*
	@brief : Returns the addition with this Vector3 and another Vector3
	@param : A constant reference to the second Vector3 to addition
	*/
	template <typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3& vec) const
	{
		return (Vector3<T>(x + vec.x, y + vec.y, z + vec.z));
	}

	/*
	@brief : Returns the negates of the x, y and z components
	*/
	template <typename T>
	Vector3<T> Vector3<T>::operator-() const
	{
		return (Vector3<T>(-x, -y, -z));
	}

	/*
	@brief : Returns a the difference with this Vector3 and another Vector3
	@param : A reference constant to thhe second Vector3 to substract
	*/
	template <typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3& vec) const
	{
		return (Vector3<T>(x - vec.x, y - vec.y, z - vec.z));
	}

	/*
	@brief : Returns a constant reference of the addition of this Vector3 and another Vector3 (Macro +=)
	@param : A constant reference to the second Vector3 to addition
	*/
	template <typename T>
	const Vector3<T>& Vector3<T>::operator+=(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return (*this);
	}

	/*
	@brief : Returns a constant reference of the substract of this Vector3 and another Vector3 (Macro -=)
	@param : A constant reference to the second Vector3 to substract
	*/
	template <typename T>
	const Vector3<T>& Vector3<T>::operator-=(const Vector3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return (*this);
	}

	/*
	@brief : Returns the result of the multiplication of this Vector3 with a scalar
	@param : The value to multiply
	*/
	template <typename T>
	Vector3<T> Vector3<T>::operator*(T v) const
	{
		return (Vector3<T>(x * v, y * v, z * v));
	}

	/*
	@brief : Returns the result of the division of this Vector3 with a scalar
	@param : The value to divide
	*/
	template <typename T>
	Vector3<T> Vector3<T>::operator/(T v) const
	{
		ZAssert(v != 0, "Division by zero");
		return (Vector3<T>(x / v, y / v, z / v));
	}

	/*
	@brief : Returns true if this Vector3 is equal with an another Vector3, false otherwise
	@param : A constant reference to the other Vector3 to compare
	*/
	template <typename T>
	bool Vector3<T>::operator==(const Vector3& vec)
	{
		return ((x == vec.x) && (y == vec.y) && (z == vec.z));
	}

	/*
	@brief : Returns true if this Vector3 is different with an another Vector3, false otherwise
	@param : A constant reference to the other Vector3 to compare
	*/
	template <typename T>
	bool Vector3<T>::operator!=(const Vector3& vec)
	{
		return ((x != vec.x) && (y != vec.y) && (z != vec.z));
	}
}

/*
@brief : Displays in the out stream a Vector3
@param : The stream to display
@param : A constant reference to the Vector3 to display
*/
template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::Vector3<T>& vec)
{
	return (stream << "Vector : (" << vec.x << " ; " << vec.y << " ; " << vec.z << " )");
}