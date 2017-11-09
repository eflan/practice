#include <stdio.h>

void PlayBlackJack();
void CallCenter();
void SimulateJukebox();
void SimulateParkingLot();
void SimulateEReader();
void HashtableTest();
void JigsawPuzzleTest();

int main(int argc, char *argv[])
{
	PlayBlackJack();
	printf("\n-------------------------------------------------------\n");
	CallCenter();
	printf("\n-------------------------------------------------------\n");
	SimulateParkingLot();
	printf("\n-------------------------------------------------------\n");
	SimulateEReader();
	printf("\n-------------------------------------------------------\n");
	HashtableTest();
	printf("\n-------------------------------------------------------\n");
	JigsawPuzzleTest();

	return 0;
}
