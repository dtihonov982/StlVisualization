#include "Common/Script.h"

int main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    Record rec = Record::load(argv[1]);
    return 0;
}