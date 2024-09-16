//
// Courtney Ramatowski
// Vending machine map maker. 
//		• Makes a map of <number> vending machines
//		• Randomly creates a connection to other vending machines
//			• Can set probability of any vending machine connecting to another vending machine
//		• Can display matrix view of vending machines and connections
//		• Prints results to file for use in tests for CS465 experiment


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

// Estimated min and max separation (in meters) for vending machine layouts.
const int MAX_DIST = 500;
const int MIN_DIST = 30;
const int PRECISION = 2;

float** createGraph(int numberOfMachines);
string printGraph(int numberOfMachines, float** graph);
float** fillGraph(int numberOfMachines, float** graph, int probability);
float randomFloat();
float** validateGraph(int numberOfMachines, float** graph);
void writeToFile(int numberOfMachines, float** graph, string fileName);

void main(int argc, char* argv[]) {

	// Get args
	int numberOfMachines = (argc > 1) ? (atoi(argv[1])) : (10);
	int probability = (argc > 2) ? (atoi(argv[2])) : (30);
	string fileName = (argc > 3) ? (string (argv[3])) : ("VM_TestData_M" + to_string(numberOfMachines) + "_p" + to_string(probability) + ".txt");
	
	//cout << "numberOfMachines: " << numberOfMachines << endl;
	//cout << "probability: " << probability << endl;
	//cout << "fileName: " << fileName << endl;


	// Create double array to represent edges/distances between vending machines/nodes
	float** graph = createGraph(numberOfMachines);

	// Fill graph
	graph = fillGraph(numberOfMachines, graph, probability);
	
	// Show graph
	//cout << printGraph(numberOfMachines, graph);

	// Write graph to file
	writeToFile(numberOfMachines, graph, fileName);

	return;
}

void writeToFile(int numberOfMachines, float** graph, string fileName) {
	// Writes graph data to a txt file
	
	ofstream outFile(fileName);

	for (int i = 0; i < numberOfMachines; i++) {
		outFile << i;

		for (int j = 0; j < numberOfMachines; j++) {
			
			if (graph[i][j] > MIN_DIST) {
				outFile << " " << j << " " << fixed << setprecision(2) << graph[i][j];
			}
		}

		outFile << endl;
	}

	outFile.close();

	return;
}

float randomFloat() {
	// This is not terribly reliable for decimal precision above 3 or 4, but it does the trick for this project

	float retFloat = 0;
	int divisor = 1;

	// Calculate divisor to get the number of digits past the decimal that matches precision
	for (int i = 0; i < PRECISION; i++) {
		divisor *= 10;
	}

	// Ranomly generate a float
	retFloat = float(rand()/float(divisor));

	// Remove the left side of the decimal (RAND_MAX is too low to give values over 30k -ish)
	retFloat -= int(retFloat);

	// Generate a value between min and max distance 
	retFloat += (rand() % (MAX_DIST - MIN_DIST) + MIN_DIST);

	return retFloat;
}

float** fillGraph(int numberOfMachines, float** graph, int probability) {
	// Randomly generate distanecs between vending machines.
	//		• Probability refers to the chance that any vending machine is connected to another. 

	// Must seed for "randomness"
	srand(time(0));

	// Iterate through the graph to set connections between vending machines.
	for (int i = 0; i < numberOfMachines; i++) {
		for (int j = 0; j < numberOfMachines; j++) {
			if ((graph[i][j]) < MIN_DIST && (i != j) && (i < j)) { 
				if ((rand() % 100) < probability) {
					graph[i][j] = randomFloat();
					graph[j][i] = graph[i][j];
				}
			}
		}
	}

	graph = validateGraph(numberOfMachines, graph);

	return graph;
}

float** validateGraph(int numberOfMachines, float** graph) {
	// If the probability is set too low, some machines won't be connected to other machines. 
	// This function makes sure that each vending machine is connected to at least on other machine in the graph.

	for (int i = 0; i < numberOfMachines; i++) {
		
		bool hasConnection = false;

		// Check if each machine has a connection to at least one other machine 
		for (int j = 0; j < numberOfMachines; j++) {

			if (graph[i][j] > 0) {
				hasConnection = true;
			}
		}
		
		// If there is no connection, assign a connection to a random machine
		if (!hasConnection) {
			int randJ = rand() % numberOfMachines;

			// Make sure that the machine isn't connecting to itself
			while (randJ == i) {
				randJ = rand() % numberOfMachines;
			}

			graph[i][randJ] = randomFloat();
			graph[randJ][i] = graph[i][randJ];		
		} 
	}

	return graph;
}

string printGraph(int numberOfMachines, float** graph) {
	// Human readable matrix representation of connecting paths between vending machines. 

	stringstream retStream; 

	retStream << "       |";

	for (int i = 0; i < numberOfMachines; i++) {
		retStream.width(6);
		retStream << (i + 1) << "  ";
	}

	retStream << endl << "-------+" ;
	
	for (int i = 0; i < numberOfMachines; i++) {
		retStream << "--------";
	}

	retStream << endl;

	for (int i = 0; i < numberOfMachines; i++) {
		retStream.width(6);
		retStream << (i+1) << " | ";
		for (int j = 0; j < numberOfMachines; j++) {
			retStream.width(6);
			retStream << fixed << setprecision(2) << graph[i][j];
			//retStream << (numberOfMachines > j) ? (", ") : (" ");
			if ((numberOfMachines - 1) > j) {
				retStream << ", ";
			}
		}
		retStream << "\n";
	}

	return retStream.str();
}

float** createGraph(int numberOfMachines) {
	// Create's a 2 dimensional array (filled with all 0's) to represent the graph/map of connecting paths between vending machines. 

	// Make an array of pointers 
	float** graph = new float* [numberOfMachines];

	// Point the pointers at arrays 
	for (int i = 0; i < numberOfMachines; i++) {
		graph[i] = new float[numberOfMachines];
	}

	// Set all values to 0
	for (int i = 0; i < numberOfMachines; i++) {
		for (int j = 0; j < numberOfMachines; j++) {
			graph[i][j] = 0;
		}
	}

	return graph;
}
