#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_PORTS 8 // Maximum number of ports
#define MAX_LINE_LENGTH 100 // Maximum length of a line in the output file

// Packet structure
typedef struct {
    int arrivalTime;
    int arrivalPort;
    int destinationPort;
} Packet;

// Function to generate a random floating-point number between 0 and 1
double randomProbability() {
    return (double)rand() / RAND_MAX;
}

// Function to generate a random destination port based on the distribution type
int generateDestinationPort(int N, int arrivalPort, char distribution[]) {
    if (strcmp(distribution, "uniform") == 0) {
        return rand() % N; // Uniform distribution: Random destination port between 0 and N-1
    }
    else if (strcmp(distribution, "diagonal") == 0) {
        int random = rand() % 3; // Random number between 0 and 2

        if (random == 0) {
            return arrivalPort; // Probability of 2/3 to select the same port
        }
        else if (random == 1) {
            return (arrivalPort + 1) % N; // Probability of 1/3 to select the next port (modulo N)
        }
        else {
            return -1; // Invalid port
        }
    }
    else {
        return rand() % N; // Default: Uniform distribution
    }
}

// Function to compare packets based on arrival time for sorting
int comparePackets(const void* a, const void* b) {
    const Packet* packetA = (const Packet*)a;
    const Packet* packetB = (const Packet*)b;

    return packetA->arrivalTime - packetB->arrivalTime;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Insufficient arguments.\n");
        printf("Usage: tr_gen N T seed p [-d] > in_script.txt\n");
        exit(1);
    }

    int N = atoi(argv[1]); // Switch dimension (number of ports)
    int T = atoi(argv[2]); // Number of time steps
    int seed = atoi(argv[3]); // Random number generation seed
    double p = atof(argv[4]); // Arrival probability
    char* distribution = "uniform"; // Default distribution: Uniform

    // Check if distribution flag is provided
    if (argc == 6 && strcmp(argv[5], "-d") == 0) {
        distribution = "diagonal"; // Set distribution type to diagonal
    }

    srand(seed); // Seed the random number generator

    int packetCount = 0;

    for (int t = 0; t < T; t++) {
        for (int inputPort = 0; inputPort < N; inputPort++) {
            for (int outputPort = 0; outputPort < N; outputPort++) {
                if (randomProbability() < p) {
                    int destinationPort = generateDestinationPort(N, inputPort, distribution);

                    if (destinationPort != -1) {
                        packetCount++;
                    }
                }
            }
        }
    }

    Packet* packets = malloc(sizeof(Packet) * packetCount);
    int index = 0;

    for (int t = 0; t < T; t++) {
        for (int inputPort = 0; inputPort < N; inputPort++) {
            for (int outputPort = 0; outputPort < N; outputPort++) {
                if (randomProbability() < p) {
                    int destinationPort = generateDestinationPort(N, inputPort, distribution);

                    if (destinationPort != -1) {
                        if (index < packetCount && packets != NULL) { // Check index within bounds
                            packets[index].arrivalTime = t;
                            packets[index].arrivalPort = inputPort;
                            packets[index].destinationPort = destinationPort;
                            index++;
                        }
                        else {
                            break; // Stop populating packets if index exceeds packetCount
                        }
                    }
                }
            }
        }
    }

    // Sort packets only if packetCount is greater than 0
    if (packets > 0 && packetCount > 0) {
        qsort(packets, packetCount, sizeof(Packet), comparePackets);
    }

    if (packets != NULL) {
        for (int i = 0; i < packetCount; i++) {
            if (&packets[i] != NULL) {
                printf("%d %d %d\n", packets[i].arrivalTime, packets[i].arrivalPort, packets[i].destinationPort);
            }
        }

        free(packets);
    }

    return 0;
}