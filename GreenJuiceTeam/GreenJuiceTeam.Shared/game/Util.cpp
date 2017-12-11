#include "Util.h"

std::vector<glImage*> Util::textures=std::vector<glImage*>(Util::SIZE);
const int Util::CAR_LENGTH = 200;

bool Util::accel=false;
bool Util::music=true;
bool Util::sound=true;

std::vector<int> Util::generateRandomArray(int size){
	int r = rand();
	if(r < 0) r = -r;
	int n = r % size;

	std::vector<int> result(size);

	int index = 0;
	for (int i = n; i < size; i++) {
		result[index++] = i;
	}
	for (int i = 0; i < n; i++) {
		result[index++] = i;
	}
	return result;
}

int Util::random(int min, int max){
	int r = rand();
	if(r < 0) r = -r;
	return (max) - (r %(max-min+1));
}

void Util::quickSort(std::vector<int>* elements) {
	if (elements->size() > 0) {
		quickSort(elements, 0, elements->size() - 1);
	}
}

void Util::quickSort(std::vector<int>* elements, int lowIndex, int highIndex) {
	int lowToHighIndex;
	int highToLowIndex;
	int pivotIndex;
	int pivotValue; // values are <int> in this demo, change to suit
	// your application
	int lowToHighValue;
	int highToLowValue;

	int newLowIndex;
	int newHighIndex;

	lowToHighIndex = lowIndex;
	highToLowIndex = highIndex;
	/*
	* Choose a pivot, remember it's value No special action for the pivot
	* element itself. It will be treated just like any other element.
	*/
	pivotIndex = (lowToHighIndex + highToLowIndex) / 2;
	pivotValue = (*elements)[pivotIndex];

	/*
	* Split the array in two parts.
	* 
	* The lower part will be lowIndex - newHighIndex, containing elements <=
	* pivot Value
	* 
	* The higher part will be newLowIndex - highIndex, containting elements >=
	* pivot Value
	*/
	newLowIndex = highIndex + 1;
	newHighIndex = lowIndex - 1;
	// loop until low meets high
	while ((newHighIndex + 1) < newLowIndex) // loop until partition
		// complete
	{ // loop from low to high to find a candidate for swapping
		lowToHighValue = (*elements)[lowToHighIndex];
		while (lowToHighIndex < newLowIndex
			& lowToHighValue < pivotValue) {
				newHighIndex = lowToHighIndex; // add element to lower part
				lowToHighIndex++;
				lowToHighValue = (*elements)[lowToHighIndex];
		}

		// loop from high to low find other candidate for swapping
		highToLowValue = (*elements)[highToLowIndex];
		while (newHighIndex <= highToLowIndex
			& (highToLowValue > pivotValue)) {
				newLowIndex = highToLowIndex; // add element to higher part
				highToLowIndex--;
				highToLowValue = (*elements)[highToLowIndex];
		}

		// swap if needed
		if (lowToHighIndex == highToLowIndex) // one last element, may go
			// in either part
		{
			newHighIndex = lowToHighIndex; // move element arbitrary to
			// lower part
		} else if (lowToHighIndex < highToLowIndex) // not last element yet
		{
			if (lowToHighValue >= highToLowValue) // low >= high, swap, even if equal
			{
				int parking = lowToHighValue;
				(*elements)[lowToHighIndex] = highToLowValue;
				(*elements)[highToLowIndex] = parking;

				newLowIndex = highToLowIndex;
				newHighIndex = lowToHighIndex;

				lowToHighIndex++;
				highToLowIndex--;
			}
		}
	}

	// Continue recursion for parts that have more than one element
	if (lowIndex < newHighIndex) {
		quickSort(elements, lowIndex, newHighIndex); // sort lower
		// subpart
	}
	if (newLowIndex < highIndex) {
		quickSort(elements, newLowIndex, highIndex); // sort higher subpart
	}
}