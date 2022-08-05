#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bitset>
using namespace std;

class Fit {
    public:
        Fit(const char *fname);
        ~Fit();
        void ReadFile();
        void PrintHeader();
        void DecodeFile();
    private:
        int ReadRecord(int byte_no);
        const char *filename;
        vector<uint8_t> fitbytes;
        vector<vector<uint8_t>> headers;
        vector<vector<uint8_t>> definitions;
};