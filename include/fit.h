#include <iostream>

using namespace std;

class Fit {
    public:
        Fit(const char *fname);
        ~Fit();
        void ReadFile();
    private:
        const char *filename;
};