namespace Zx
{
	bool ZDevice::Queue::IsValidQueue()
	{
		//TODO : Opti
		return (indexFamily >= 0);
	}
}