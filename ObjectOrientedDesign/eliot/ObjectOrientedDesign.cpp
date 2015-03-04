#include <stdio.h>

void PlayBlackJack();
void CallCenter();
void SimulateJukebox();
void SimulateParkingLot();
void SimulateEReader();

int main(int argc, char *argv[])
{
	PlayBlackJack();
	printf("\n-------------------------------------------------------\n");
	CallCenter();
	printf("\n-------------------------------------------------------\n");
	SimulateJukebox();
	printf("\n-------------------------------------------------------\n");
	SimulateParkingLot();
	printf("\n-------------------------------------------------------\n");
	SimulateEReader();

	return 0;
}
