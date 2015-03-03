#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

class ParkingLot;

class Vehicle
{
public:
	Vehicle(const char icon, const unsigned int size)
		: _icon(icon),
		  _size(size)
	{
	}

	const char icon() const { return _icon; }
	
	void park(ParkingLot &lot);
	void leave(ParkingLot &lot);

	const unsigned int size() const { return _size; }
	const bool isParked() const { return _spaces.size() != 0; }
	
protected:
	char _icon;
	unsigned int _row;
	std::vector<unsigned int> _spaces;

private:
	unsigned int _size;
};

class ParkingSpace
{
public:
	ParkingSpace()
		: _occupied(nullptr)
	{
	}
	
	void park(Vehicle *parked)
	{
		_occupied = parked;
	}
	
	void leave()
	{
		_occupied = nullptr;
	}
	
	const bool occupied(Vehicle **occupant) const
	{
		*occupant = _occupied;
		return !isFree();
	}
	
	const bool isFree() const
	{
		return _occupied == nullptr;
	}
	
private:
	Vehicle *_occupied;
};

class ParkingLot
{
public:
	ParkingLot(const unsigned int rows, const unsigned int stallsPerRow)
		: _rowCount(rows),
		  _stallsPerRow(stallsPerRow),
		  _stalls(rows),
		  _startSearchRow(0),
		  _startSearchSpace(rows)
	{
		for(unsigned int i = 0; i < rows; i++)
		{
			_stalls[i] = std::vector<ParkingSpace>(stallsPerRow);
			_startSearchSpace[i] = 0;
		}
	}
	
	const bool findSpace(const unsigned int size, unsigned int *row, std::vector<unsigned int> &spaces)
	{
		const unsigned int iStart = _startSearchRow;
		bool iFirstLoop = true;

		for(unsigned int i = iStart; iFirstLoop || (i != iStart); i = (i + 1) % _rowCount)
		{
			iFirstLoop = false;
			
			const unsigned int kStart = _startSearchSpace[i];
			bool kFirstLoop = true;

			for(unsigned int k = kStart; kFirstLoop || (k != kStart); k = (k + 1) % _stallsPerRow)
			{
				kFirstLoop = false;

				if(fits(_stalls[i], k, size))
				{
					rememberWhereToStartSearch(i, k);
					
					*row = i;
					
					const unsigned int endSpace = k + size;
					for(unsigned int spot = k; spot < endSpace; spot++)
					{
						spaces.push_back(spot);
					}

					return true;
				}
			}
		} 
		
		return false;
	}
	
	void park(Vehicle *vehicle, const unsigned int row, const std::vector<unsigned int> &spaces)
	{
		for(unsigned int spot: spaces)
		{
			if(!_stalls[row][spot].isFree())
			{
				throw "Vehicle attempted to park in occupied parking space!";
			}
			else
			{
				_stalls[row][spot].park(vehicle);
			}
		}
	}
	
	void leave(Vehicle *vehicle, const unsigned int row, const std::vector<unsigned int> &spaces)
	{
		for(unsigned int spot: spaces)
		{
			Vehicle *parkedVehicle = nullptr;
			
			if(!_stalls[row][spot].occupied(&parkedVehicle))
			{
				throw "Vehicle attempting to leave unoccupied parking space!";
			}
			else if(parkedVehicle != vehicle)
			{
				throw "Mismatched vehicle attempting to leave spot it didn't park in!";
			}
			else
			{
				_stalls[row][spot].leave();
			}
		}	
	}
	
	void print() const
	{		
		printf("\n");
		for(unsigned int i = 0; i < _rowCount; i++)
		{
			for(const ParkingSpace &space: _stalls[i])
			{
				printf("%c", icon(space));
			}
			printf("\n");
		}
	}
	
private:
	static const char icon(const ParkingSpace &space)
	{
		Vehicle *parkedVehicle = nullptr;
		if(space.occupied(&parkedVehicle))
		{
			return parkedVehicle->icon();
		}
		else
		{
			return '_';
		}
	}
	
	static const bool fits(const std::vector<ParkingSpace> &row,
	                       const unsigned int start,
	                       const unsigned int size)
	{
		const unsigned int stallCount = row.size();
		const unsigned int end = start + size;
		
		for(unsigned int i = start; i < end; i++)
		{
			if(i >= stallCount)
			{
				return false;
			}
			else if(!row[i].isFree())
			{
				return false;
			}
		}
		
		return true;
	}
	
	void rememberWhereToStartSearch(const unsigned int row, const unsigned int col)
	{
		if(row != _stallsPerRow - 1)
		{
			_startSearchRow = row;
		}
		else
		{
			_startSearchRow = (row + 1) % _rowCount;
		}

		_startSearchSpace[row] = (col + 1) % _stallsPerRow;
	}
	
	unsigned int _rowCount;
	unsigned int _stallsPerRow;
	std::vector<std::vector<ParkingSpace> > _stalls;
	unsigned int _startSearchRow;
	std::vector<unsigned int> _startSearchSpace;
};

void Vehicle::park(ParkingLot &lot)
{	
	if(lot.findSpace(size(), &_row, _spaces))
	{
		lot.park(this, _row, _spaces);
	}
	else
	{
		throw "Lot is full :-(";
	}
}

void Vehicle::leave(ParkingLot &lot)
{
	lot.leave(this, _row, _spaces);
	
	_row = 0;
	_spaces.clear();
}

class Car : public Vehicle
{
public:
	Car() : Vehicle('c', 1) {}
};

void SimulateParkingLot()
{
	srand(time(NULL));
	ParkingLot lot(5, 20);
	std::vector<Car> testCars(100);
	
	for(unsigned int i = 0; i < 1000; i++)
	{
		Car *randomCar = &(testCars[rand() % testCars.size()]);
		
		try
		{
			if(randomCar->isParked())
			{
				randomCar->leave(lot);
			}
			else
			{
				randomCar->park(lot);
			}
		}
		catch(const char *const exceptionMessage)
		{
			printf("Car %u threw exception -- \"%s\"\n", i, exceptionMessage);
		}
	}
	
	lot.print();
}
