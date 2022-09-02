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
    LAP_EVENT,
    LAP_EVENT_TYPE,
    LAP_START_TIME,
    LAP_START_POSITION_LAT,
    LAP_START_POSITION_LONG,
    LAP_END_POSITION_LAT,
    LAP_END_POSITION_LONG,
    LAP_TOTAL_ELAPSED_TIME,
    LAP_TOTAL_TIMER_TIME,
    LAP_TOTAL_DISTANCE,
    LAP_TOTAL_CYCLES,
    LAP_TOTAL_CALORIES,
    LAP_TOTAL_FAT_CALORIES,
    LAP_AVG_SPEED,
    LAP_MAX_SPEED,
    LAP_AVG_HEART_RATE,
    LAP_MAX_HEART_RATE,
    LAP_AVG_CADENCE,
    LAP_MAX_CADENCE,
    LAP_AVG_POWER,
    LAP_MAX_POWER,
    LAP_TOTAL_ASCENT,
    LAP_TOTAL_DESCENT,
    LAP_INTENSITY,
    LAP_TRIGGER,
    LAP_SPORT,
    LAP_EVENT_GROUP,
    LAP_NUM_LENGTHS = 32,
    LAP_NORMALIZED_POWER,
    LAP_LEFT_RIGHT_BALANCE,
    LAP_TOTAL_WORK = 41,
    LAP_AVG_ALTITUDE,
    LAP_MAX_ALTITUDE,
    LAP_GPS_ACCURACY,
    LAP_AVG_GRADE,
    LAP_POS_GRADE,
    LAP_NEG_GRADE,
    LAP_MAX_POS_GRADE,
    LAP_MAX_NEG_GRADE,
    LAP_AVG_TEMPERATURE,
    LAP_MAX_TEMPERATURE,
    LAP_TOTAL_MOVING_TIME,
    LAP_TIMESTAMP = 253,
    LAP_MESSAGE_INDEX
};