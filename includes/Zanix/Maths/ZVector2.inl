namespace Zx
{
	/*
	@brief : Construct a ZVector2 with 2 components
	@param : The first component (x)
	@param : The second component (y)
	*/
	template <typename T>
	ZVector2<T>::ZVector2(T x, T y) : x(x), y(y)
	{
	}

	/*
	@brief : Copy constructor : Copy another ZVector2
	@param : The ZVector2 to copy
	*/
	template <typename T>
	ZVector2<T>::ZVector2(const ZVector2& vec) : x(vec.x), y(vec.y)
	{
	}

	/*
	@brief : Movement constructor : Move the component of another ZVector2
	@param : The ZVector2 to move
	*/
	template <typename T>
	ZVector2<T>::ZVector2(ZVector2&& vec) noexcept
	{
		std::swap(x, vec.x);
		std::swap(y, vec.y);
	}

	/*
	@brief : Return the squared lenght of a ZVector2 (x� + y�)
	*/
	template <typename T>
	T ZVector2<T>::GetSquaredLenght() const
	{
		return (x * x + y * y);
	}

	/*
	@brief : Return the lenght of a ZVector2 (sqrt(x� + y�))
	*/
	template <typename T>
	T ZVector2<T>::GetLenght() const
	{
		return (std::sqrt(GetSquaredLenght()));
	}

	/*
	@brief : Return a constant reference to this ZVector2
	*/
	template <typename T>
	const ZVector2<T>& ZVector2<T>::operator+() const
	{
		return (*this);
	}

	/*
	@brief : Return the addition with this ZVector2 and another ZVector2
	@param : The second ZVector2 to addition
	*/
	template <typename T>
	ZVector2<T> ZVector2<T>::operator+(const ZVector2& vec) const
	{
		return (ZVector2<T>(x + vec.x, y + vec.y));
	}

	/*
	@brief : Return the negates of the components x and y
	*/
	template <typename T>
	ZVector2<T> ZVector2<T>::operator-() const
	{
		return (ZVector2<T>(-x, -y));
	}

	/*
	@brief : Return a the difference with this ZVector2 and another ZVector2
	@param : The second ZVector2 to substract
	*/
	template <typename T>
	ZVector2<T> ZVector2<T>::operator-(const ZVector2& vec) const
	{
		return (ZVector2<T>(x - vec.x, y - vec.y));
	}

	/*
	@brief : Return a constant reference to this ZVector2 (Macro +=)
	@param : The second ZVector2 to addition
	*/
	template <typename T>
	ZVector2<T>& ZVector2<T>::operator+=(const ZVector2& vec)
	{
		x += vec.m_x;
		y += vec.m_y;

		return (*this);
	}

	/*
	@brief : Return a reference constant to this ZVector2 (Macro -=)
	@param : The second ZVector2 to substract
	*/
	template <typename T>
	ZVector2<T>& ZVector2<T>::operator-=(const ZVector2& vec)
	{
		x -= vec.m_x;
		y -= vec.m_y;

		return (*this);
	}

	/*
	@brief : Return the result of the multiplication of this ZVector2 with a scalar
	@param : The value to multiply
	*/
	template <typename T>
	ZVector2<T> ZVector2<T>::operator*(T v) const
	{
		return (ZVector2<T>(x * v, y * v));
	}

	/*
	@brief : Return the result of the division of this ZVector2 with a scalar
	@param : The value to divide
	*/
	template <typename T>
	ZVector2<T> ZVector2<T>::operator/(T v) const
	{	
		ZAssert(v == 0, "Division by zero");
		return (ZVector2<T>(x / v, y / v));
	}

	/*
	@brief : Return true if this ZVector2 is equal with an another ZVector2, false otherwise
	@param : The other ZVector2 to compare
	*/
	template <typename T>
	bool ZVector2<T>::operator==(const ZVector2& vec) const
	{
		return ((x == vec.x) && (y == vec.y));
	}

	/*
	@brief : Return true if this ZVector2 is different with an another ZVector2, false otherwise
	@param : The other ZVector2 to compare
	*/
	template <typename T>
	bool ZVector2<T>::operator!=(const ZVector2& vec) const
	{
		return ((x != vec.x) && (y != vec.y));
	}
}

/*
@brief : Display in the out stream a ZVector2
*/
template <typename T>
std::ostream& operator<<(std::ostream& stream, const Zx::ZVector2<T>& vec)
{
	return (stream << vec.x << " " << vec.y);
}