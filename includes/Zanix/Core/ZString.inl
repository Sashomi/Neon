namespace Zx {
	inline ZString::String::String(std::size_t newSize) : size(newSize), str(new char[newSize + 1])
	{
		str[newSize] = '\0';
	}
}