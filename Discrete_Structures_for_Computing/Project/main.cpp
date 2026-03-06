#include <iostream>
#include <fstream>
#include <string>
#include "bellman.h"
#include "tsm.h"

static bool loadMatrix(const std::string& path, int n, int a[60][60]) {
    std::ifstream fin(path);
    if (!fin) return false;
    for (int r=0;r<n;++r)
        for (int c=0;c<n;++c)
            if (!(fin >> a[r][c])) return false;
    return true;
}

int main(int argc, char** argv){
    // Usage:
    //   app bf <n> <matrix_file> <start> [iters]
    //   app bfpath <n> <matrix_file> <start> <goal>
    //   app tsm <n> <matrix_file> <start>
    if (argc < 5) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " bf <n> <matrix_file> <start> [iters]\n"
                  << "  " << argv[0] << " bfpath <n> <matrix_file> <start> <goal>\n"
                  << "  " << argv[0] << " tsm <n> <matrix_file> <start>\n";
        return 0;
    }

    std::string mode = argv[1];
    int n = std::stoi(argv[2]);
    if (n <= 0 || n > 60) { std::cerr << "n must be 1..60\n"; return 1; }

    std::string file = argv[3];
    char start = argv[4][0];

    static int Mat[60][60];
    if (!loadMatrix(file, n, Mat)) {
        std::cerr << "Failed to read matrix from " << file << "\n";
        return 1;
    }

    if (mode == "bf") {
        int iters = (argc >= 6) ? std::stoi(argv[5]) : 1;
        int BFValue[60], BFPrev[60];
        for (int i=0;i<n;++i){ BFValue[i]=-1; BFPrev[i]=-1; }
        for (int t=0;t<iters; ++t) BF(Mat, n, start, BFValue, BFPrev);

        std::cout << "BFValue:";
        for (int i=0;i<n;++i) std::cout << ' ' << BFValue[i];
        std::cout << "\nBFPrev:";
        for (int i=0;i<n;++i) std::cout << ' ' << BFPrev[i];
        std::cout << '\n';

    } else if (mode == "bfpath") {
        if (argc < 6) { std::cerr << "Need <goal>\n"; return 1; }
        char goal = argv[5][0];
        std::string path = BF_Path(Mat, n, start, goal);
        if (path.empty()) std::cout << "no path\n";
        else std::cout << path << '\n';

    } else if (mode == "tsm") {
        std::string tour = Traveling(Mat, n, start);
        if (tour.empty()) std::cout << "no tour\n";
        else std::cout << tour << '\n';

    } else {
        std::cerr << "Unknown mode\n";
        return 1;
    }
    return 0;
}
