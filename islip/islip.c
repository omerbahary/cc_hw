#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int arrivalTime;
    int arrivalPort;
    int destinationPort;
} Packet;

void iSLIP(int N, int k, int r) {
    // Allocate memory for input and output buffers
    Packet** inputBuffers = (Packet**)malloc(sizeof(Packet*) * N);
    Packet** outputBuffers = (Packet**)malloc(sizeof(Packet*) * N);
    for (int i = 0; i < N; i++) {
        inputBuffers[i] = (Packet*)malloc(sizeof(Packet) * N);
        outputBuffers[i] = (Packet*)malloc(sizeof(Packet) * N);
    }

    // Initialize matching array and round-robin counters
    int* matching = (int*)malloc(sizeof(int) * N);
    int* rrCounters = (int*)malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++) {
        matching[i] = -1;
        rrCounters[i] = 0;
    }

    // Read packets from stdin and process for each time step
    for (int t = 0; t < k; t++) {
        // Clear output buffers for each time step
        for (int outputPort = 0; outputPort < N; outputPort++) {
            for (int i = 0; i < N; i++) {
                outputBuffers[outputPort][i].arrivalTime = -1;
                outputBuffers[outputPort][i].arrivalPort = -1;
                outputBuffers[outputPort][i].destinationPort = -1;
            }
        }

        // Read input packets from stdin for this time step
        int packetCount = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                scanf("%d %d %d", &inputBuffers[i][j].arrivalTime, &inputBuffers[i][j].arrivalPort, &inputBuffers[i][j].destinationPort);
                packetCount++;
            }
        }

        // Request Step
        for (int requestingPort = 0; requestingPort < N; requestingPort++) {
            // Determine the output port for this round-robin cycle
            int outputPort = rrCounters[requestingPort];
            rrCounters[requestingPort] = (rrCounters[requestingPort] + 1) % N;

            // Send request from each input port to the corresponding output port
            // You can modify this step to prioritize requests based on specific criteria
            // For example, based on arrival time or other packet attributes
            // Implement your custom logic here

            // Example: Priority based on arrival time
            if (inputBuffers[requestingPort][0].arrivalTime == t) {
                // Make a request from the input port to the output port
                // Update the matching array accordingly
                matching[requestingPort] = outputPort;
            }
        }

        // Grant Step
        for (int requestingPort = 0; requestingPort < N; requestingPort++) {
            // Determine the grant port based on the round-robin counters and lowest priority
            int grantPort = -1;
            for (int i = 0; i < N; i++) {
                int currentPort = (requestingPort + i) % N;
                if (matching[currentPort] == -1) {
                    grantPort = currentPort;
                    break;
                }
            }

            if (grantPort != -1) {
                // Grant the request and update the matching array
                matching[grantPort] = requestingPort;

                // Update the output buffer accordingly
                outputBuffers[grantPort][0] = inputBuffers[requestingPort][0];
            }
        }

        // Accept Step
        for (int requestingPort = 0; requestingPort < N; requestingPort++) {
            int grantedPort = matching[requestingPort];
            if (grantedPort != -1) {
                // Accept the granted packet and update the matching array
                // Move packets in the input buffer to maintain FIFO order
                for (int i = 0; i < N - 1; i++) {
                    inputBuffers[requestingPort][i] = inputBuffers[requestingPort][i + 1];
                }

                // Clear the last entry in the input buffer
                inputBuffers[requestingPort][N - 1].arrivalTime = -1;
                inputBuffers[requestingPort][N - 1].arrivalPort = -1;
                inputBuffers[requestingPort][N - 1].destinationPort = -1;

                matching[requestingPort] = -1;
            }
        }

        // Output packets for this time step
        for (int outputPort = 0; outputPort < N; outputPort++) {
            for (int i = 0; i < N; i++) {
                if (outputBuffers[outputPort][i].arrivalTime != -1) {
                    printf("%d %d %d\n", outputBuffers[outputPort][i].arrivalTime, outputBuffers[outputPort][i].arrivalPort, outputBuffers[outputPort][i].destinationPort);
                }
            }
        }
    }

    // Free memory
    for (int i = 0; i < N; i++) {
        free(inputBuffers[i]);
        free(outputBuffers[i]);
    }
    free(inputBuffers);
    free(outputBuffers);
    free(matching);
    free(rrCounters);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage: %s N k r\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);  // Switch dimension
    int k = atoi(argv[2]);  // Number of iSLIP iterations
    int r = atoi(argv[3]);  // Run ID

    iSLIP(N, k, r);

    return 0;
}