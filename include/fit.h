#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bitset>

#define TIMESTAMP_BASE 631058400

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
        void HandleRecord(int record_id, uint16_t msg_num, vector<uint8_t> message);
        void DecodeLapMessage(int record_id, vector<uint8_t> message);
        void DecodeLapRecord(int id, vector<uint8_t> data);
        int ReturnValue(vector<uint8_t> data);
        const char *filename;
        vector<uint8_t> fitbytes;
        vector<vector<uint8_t>> headers;
        vector<vector<uint8_t>> definitions;
};

enum mesg_num {
    file_id,
    capabilities,
    device_settings,
    user_profile,
    hrm_profile,
    sdm_profile,
    bike_profile,
    zones_target,
    hr_zone,
    power_zone,
    met_zone,
    sport = 12,
    goal = 15,
    session = 18,
    lap,
    record,
    event,
    device_info = 23,
    workout = 26,
    workout_step,
    schedule,
    weight_scale = 30,
    course,
    course_point,
    totals,
    activity
};

enum fit_base_type {
    type_enum,
    type_sint8,
    type_uint8,
    type_string = 7,
    type_uint8z = 10,
    type_byte = 13,
    type_sint16 = 131,
    type_uint16,
    type_sint32,
    type_uint32,
    type_float32 = 136,
    type_float64,
    type_uint16z = 139,
    type_uint32z,
    type_sint64 = 142,
    type_uint64,
    type_uint64z
};

enum lap_records {
    lap_event,
    lap_event_type,
    lap_start_time,
    lap_start_position_lat,
    lap_start_position_long,
    lap_end_position_lat,
    lap_end_position_long,
    lap_total_elapsed_time,
    lap_total_timer_time,
    lap_total_distance
};