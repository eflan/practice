#include <stdio.h>
#include <vector>

class ParkingLot;

class Renderable
{
public:
	Renderable(const size_t width)
		: _width(width)
	{
	}
	
	const size_t width() const
	{
		return _width;
	}
	
	virtual const char icon(const size_t x) const = 0;

private:
	size_t _width;
};

class Vehicle : public Renderable
{
public:
	Vehicle(const size_t w) : Renderable(w) {}
	
	virtual void park(ParkingLot &lot) = 0;	
};

class ParkingSpace : public Renderable
{
public:
	ParkingSpace()
		: Renderable(1),
		  _occupied(nullptr)
	{
	}
	
	void park(Vehicle *parked)
	{
		_occupied = parked;
	}
	
	const bool occupied(const Vehicle **occupant) const
	{
		*occupant = _occupied;
		return _occupied != nullptr;
	}
	
	virtual const char icon(const size_t x) const
	{
		if(_occupied == nullptr)
		{
			return '=';
		}
		else
		{
			return _occupied->icon(x);
		}
	}
	
private:
	Vehicle *_occupied;
};

class Lane : public Renderable
{
public:
	Lane()
		: Renderable(2),
		  _left(nullptr),
		  _right(nullptr)
	{
	}

	virtual const char icon(const size_t x) const
	{
		if(x == 0)
		{
			return iconFor(x, _left);
		}
		else
		{
			return iconFor(x, _right);
		}
	}
	
private:
	const char iconFor(const size_t x, Vehicle *v) const
	{
		if(v == nullptr)
		{
			return '.';
		}
		else
		{
			return v->icon(x);
		}
	}
	
	Vehicle *_left;
	Vehicle *_right;
};

class ParkingLot
{
public:
	ParkingLot(size_t width, size_t height)
		: _stalls(width),
		  _lanes(width - 1)
	{
		for(size_t i = 0; i < width; i++)
		{
			_stalls[i] = std::vector<ParkingSpace>(height);
			if(i != (width - 1))
			{
				_lanes[i] = std::vector<Lane>(height);
			}
		}
	}
	
	void render() const
	{
		const size_t width = _stalls.size();
		const size_t height = _stalls[0].size();
		
		for(size_t k = 0; k < height; k++)
		{			
			for(size_t i = 0; i < width; i++)
			{
				render(_stalls[i][k]);
			
				if(i != (width - 1))
				{
					render(_lanes[i][k]);
				}
			}
			printf("\n");
		}
		printf("\n\n");
	}
	
private:
	void render(const Renderable &obj) const
	{
		const size_t width = obj.width();
		
		for(size_t x = 0; x < width; x++)
		{
			printf("%c", obj.icon(x));
		}
	}
	
	std::vector<std::vector<ParkingSpace> > _stalls;
	std::vector<std::vector<Lane> > _lanes;	
};

class Car : public Vehicle
{
public:
	Car() : Vehicle(1) {}

	virtual const char icon(const size_t x) const
	{
		return 'c';
	}
	
	virtual void park(ParkingLot &lot)
	{
	}
};

void SimulateParkingLot()
{
	ParkingLot lot(5, 20);
	lot.render();
}
