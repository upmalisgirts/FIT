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
            // cout << "Record header: " << bitset<8>((int)buf[0]) << endl;
            // cout << "Full record:" << setfill('0');
            // for (int i = 0; i < 6; i++) {
            //     cout << " " << hex << setw(2) << (int)buf[i];
            // }
            // cout << endl;
            len = (int)buf[5];
            buf.clear();
            // cout << "Data definition:" << endl;
            for (int i = 0; i < len; i++) {
                for (int k = 0; k < 3; k++) {
                    // cout << setw(2) << hex << (int)fitbytes[byte_no] << " ";
                    buf.push_back(fitbytes[byte_no++]);
                }
                // cout << endl;
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
    uint16_t msg_num {0};
    vector<uint8_t> message_data;
    size = 1;
    record_id = (int)fitbytes[byte_no++];
    msg_num = (headers[record_id][4] << 8) | headers[record_id][3];
    // cout << "Data:" << endl;
    // cout << "No. " << hex << setw(2) << record_id << endl;
    // cout << "mesg_num (from header): " << dec << msg_num << endl;
    for (int i = 1; i < definitions[record_id].size(); i = i +3) {
        size = size + (int)(definitions[record_id][i]);
        for (int k = 0; k < (int)(definitions[record_id][i]); k++) {
            message_data.push_back(fitbytes[byte_no++]);
            // cout << hex << setw(2) << (int)fitbytes[byte_no++] << " ";
            // message_data.push_back(fitbytes[byte_no]);
        }
        // cout << endl;
    }
    // cout << "--------" << endl;
    // for (int i = 0; i < message_data.size(); i++) {
    //     cout << setfill('0') << hex << setw(2) << (int)message[0] << " ";
    // }
    // cout << endl;    
    HandleRecord(record_id, msg_num, message_data);
    return size;
}

void Fit::HandleRecord(int record_id, uint16_t msg_num, vector<uint8_t> message) {
    switch(msg_num) {
        case activity:
            cout << "This is activity record!" << endl;
            break;
        case lap:
            cout << "This is lap record!" << endl;
            DecodeLapMessage(record_id, message);
            break;
        // default:
        //     cout << "mesg_num " << msg_num << " is not supported yet!" << endl;
    }
}

void Fit::DecodeLapMessage(int record_id, vector<uint8_t> message) {
    // for (int i = 0; i < message.size(); i++) {
    //     cout << setfill('0') << hex << setw(2) << (int)message[i] << " ";
    // }
    // cout << endl;
    int x {0};
    int id {0};
    int len {0};
    int type {0};
    vector<uint8_t> data;
    for (int i = 0; i < definitions[record_id].size(); i++) {
        cout << hex << setw(2) << (int)definitions[record_id][i] << " ";
        if ((i+1)%3 == 0 || i == definitions[record_id].size() - 1) {
            cout << endl;
        }
    }
    // cout << endl;
    for (int i = 0; i < definitions[record_id].size(); i = i + 3) {
        id = (int)definitions[record_id][i];
        len = (int)definitions[record_id][i+1];
        type = (int)definitions[record_id][i+2];
        for (int k = 0; k < len; k++) {
            data.push_back(message[x]);
            cout << hex << setw(2) << (int)message[x++] << " ";
            // message_data.push_back(fitbytes[byte_no]);
        }
        cout << endl;
        DecodeLapRecord(id,data);
        data.clear();
    }
}

void Fit::DecodeLapRecord(int id, vector<uint8_t> data) {
    switch(id) {
        case 2:
            cout << dec << ReturnValue(data) << endl;
            break;
        case 3:
            cout << "Start position latitude: " <<
            dec << (double)ReturnValue(data)*180/((int64_t)1 << 31) << endl;
            break;
        case 4:
            cout << "Start position longitude: " <<
            dec << (double)ReturnValue(data)*180/((int64_t)1 << 31) << endl;
            break;
        case 5:
            cout << "End position latitude: " <<
            dec << (double)ReturnValue(data)*180/((int64_t)1 << 31) << endl;
            break;
        case 6:
            cout << "End position latitude: " <<
            dec << (double)ReturnValue(data)*180/((int64_t)1 << 31) << endl;
            break;   
    }
}

int Fit::ReturnValue(vector<uint8_t> data) {
    int value {0};
    int len = data.size();
    for (int i = len - 1; i >= 0; i--) {
        value = value | (data[i] << 8*i);
    }
    return value;
}