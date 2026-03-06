#ifndef BELLMAN_H
#define BELLMAN_H

#include <string>

void BF(int graph[][60], int size, char start_vertices,
        int BFValue[], int BFPrev[]);

std::string BF_Path(int graph[][60], int size,
                    char start_vertices, char end_vertices);

#endif // BELLMAN_H
