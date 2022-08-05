#include "../include/fit.h"

Fit::Fit(const char *fname){
    filename = fname;
}

void Fit::ReadFile() {
    cout << "Reading " << filename << endl;
    ifstream file(filename);
    char val;
    while (file.peek() != EOF) {
        file.get(val);
        fitbytes.push_back((uint8_t)val);
    }
    file.close();
    cout << "File length: " << fitbytes.size() << endl;
}

void Fit::PrintHeader() {
    cout << setfill('0');
    cout << "Header: ";
    for (int i = 0; i < (int)fitbytes[0]; i++) {
        cout << setw(2) << hex << (int)fitbytes[i] << " ";
    }
    cout << endl;
    cout << "Protocol version: " << dec << (int)fitbytes[1] << endl;
    cout << "Profile version: " << ((int)fitbytes[3] << 8 | (int)fitbytes[2]) << endl;
    cout << "Data size: " << ((int)fitbytes[7] << 24 | (int)fitbytes[6] << 16 | (int)fitbytes[5] << 8 | (int)fitbytes[4]) << endl;
}

void Fit::DecodeFile() {
    vector<uint8_t> buf;
    int byte_no = 14;
    int record_num {0};
    int len {0};
    uint8_t header {0};

    while (byte_no < fitbytes.size()-2) {
        if ((int)(fitbytes[byte_no]&0x40) == 0x40) {
            header = fitbytes[byte_no];
            for (int i = 0; i < 6; i++) {
                buf.push_back(fitbytes[byte_no+i]);
            }
            headers.push_back(buf);
            byte_no+=6;
            cout << "Record header: " << bitset<8>((int)buf[0]) << endl;
            cout << "Full record:" << setfill('0');
            for (int i = 0; i < 6; i++) {
                cout << " " << hex << setw(2) << (int)buf[i];
            }
            cout << endl;
            len = (int)buf[5];
            buf.clear();
            cout << "Data definition:" << endl;
            for (int i = 0; i < len; i++) {
                for (int k = 0; k < 3; k++) {
                    cout << setw(2) << hex << (int)fitbytes[byte_no] << " ";
                    buf.push_back(fitbytes[byte_no++]);
                }
                cout << endl;
            }
            definitions.push_back(buf);
            byte_no += ReadRecord(byte_no);
            buf.clear();
            record_num++;
        } else {
            byte_no += ReadRecord(byte_no);
            record_num++;
        }
    }
}

int Fit::ReadRecord(int byte_no) {
    int record_id,size;
    size = 1;
    record_id = (int)fitbytes[byte_no++];
    cout << "Data:" << endl;
    cout << "No. " << hex << setw(2) << record_id << endl;
    for (int i = 1; i < definitions[record_id].size(); i = i +3) {
        size = size + (int)(definitions[record_id][i]);
        for (int k = 0; k < (int)(definitions[record_id][i]); k++) {
            cout << hex << setw(2) << (int)fitbytes[byte_no++] << " ";
        }
        cout << endl;
    }
    cout << "--------" << endl;
    return size;
}