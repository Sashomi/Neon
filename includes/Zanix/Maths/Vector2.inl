#include <Zanix/Core/Exception.hpp>

namespace Zx
{
	/*
	@brief : Constructs a Vector2 with 2 components
	@param : The first component (x)
	@param : The second component (y)
	*/
	template <typename T>
	Vector2<T>::Vector2(T newX, T newY) : x(newX), y(newY)
	{
	}

	/*
	@brief : Copy constructor : Copy another Vector2
	@param : A constant reference to the Vector2 to copy
	*/
	template <typename T>
	Vector2<T>::Vector2(const Vector2& vec) : x(vec.x), y(vec.y)
	{
	}

	/*
	@brief : Movement constructor : Moves the component of another Vector2
	@param : A constant reference to the Vector2 to move
	*/
	template <typename T>
	Vector2<T>::Vector2(Vector2&& vec) noexcept
	{
		std::swap(x, vec.x);
		std::swap(y, vec.y);
	}

	/*
	@brief : Changes the value of the components (x, y)
	@param : The new value of the x component
	@param : The new value of the y component
	*/
	template <typename T>
	void Vector2<T>::SetVector2(T newX, T newY)
	{
		x = newX;
		y = newY;
	}

	/*
	@brief : Returns the squared lenght of a Vector2 (x² + y²)
	*/
	template <typename T>
	T Vector2<T>::GetSquaredLenght() const
	{
		return (x * x + y * y);
	}

	/*
	@brief : Returns the lenght of a Vector2 (sqrt(x² + y²))
	*/
	template <typename T>
	T Vector2<T>::GetLenght() const
	{
		return (std::sqrt(GetSquaredLenght()));
	}

	/*
	@brief : Returns a constant reference to this Vector2
	*/
	template <typename T>
	const Vector2<T>& Vector2<T>::operator+() const
	{
		return (*this);
	}

	/*
	@brief : Returns the addition with this Vector2 and another Vector2
	@param : A constant reference to the second Vector2 to addition
	*/
	template <typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2& vec) const
	{
		return (Vector2<T>(x + vec.x, y + vec.y));
	}

	/*
	@brief : Returns the negates of the x and y components
	*/
	template <typename T>
	Vector2<T> Vector2<T>::operator-() const
	{
		return (Vector2<T>(-x, -y));
	}

	/*
	@brief : Returns a the difference with this Vector2 and another Vector2
	@param : A constant reference to the second Vector2 to substract
	*/
	template <typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2& vec) const
	{
		return (Vector2<T>(x - vec.x, y - vec.y));
	}

	/*
	@brief : Returns a constant reference of the addition of this Vector2 and another Vector2 (Macro +=)
	@param : A constant reference to the second Vector2 to addition
	*/
	template <typename T>
	const Vector2<T>& Vector2<T>::operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;

		return (*this);
	}

	/*
	@brief : Returns a constant reference of the substract of this Vector2 and another Vector2 (Macro -=)
	@param : A constant reference to the second Vector2 to substract
	*/
	template <typename T>
	const Vector2<T>& Vector2<T>::operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;

		return (*this);
	}

	/*
	@brief : Returns the result of the multiplication of this Vector2 with a scalar
	@param : The value to multiply
	*/
	template <typename T>
	Vector2<T> Vector2<T>::operator*(T v) const
	{
		return (Vector2<T>(x * v, y * v));
	}

	/*
	@brief : Returns the result of the division of this Vector2 with a scalar
	@param : The value to divide
	*/
	template <typename T>
	Vector2<T> Vector2<T>::operator/(T v) const
	{	
		ZAssert(v != 0, "Division by zero");
		return (Vector2<T>(x / v, y / v));
	}

	/*
	@brief : Returns true if this Vector2 is equal with an another Vector2, false otherwise
	@param : A constant reference to the other Vector2 to compare
	*/
	template <typename T>
	bool Vector2<T>::operator==(const Vector2& vec) const
	{
		return ((x == vec.x) && (y == vec.y));
	}

	/*
	@brief : Returns true if this Vector2 is different with an another Vector2, false otherwise
	@param : A constant reference to the other Vector2 to compare
	*/
	template <typename T>
	bool Vector2<T>::operator!=(const Vector2& vec) const
	{
		return ((x != vec.x) && (y != vec.y));
	}
}

/*
@brief : Displays in the out stream a Vector2
@param : The stream to display
@param : A constant reference to the Vector2 to display
*/
template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::Vector2<T>& vec)
{
	return (stream << "Vector : (" << vec.x << " ; " << vec.y << " )");
}