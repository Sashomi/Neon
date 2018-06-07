namespace Zx
{
	bool ZDevice::Queue::IsValidQueue()
	{
		return (indexFamily >= 0);
	}
}