namespace Zx {
	inline String::Str::Str(std::size_t newSize) : size(newSize), str(new char[newSize + 1])
	{
		str[newSize] = '\0';
	}
}