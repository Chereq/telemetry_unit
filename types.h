typedef struct {
  unsigned long timestamp; // 4
  int current;  // 6
  int voltage; // 8
  float lat; // 12
  float lon; // 16
  int alt; // 18
  int fire_x; // 20
  int fire_y; // 22
  char crc; // 23
} telemetry_t; // 17-24 bytes are ideal https://www.youtube.com/watch?v=5Xhvphsj1ms

typedef struct {
int die_mf;
} ack_t;

typedef union {
  telemetry_t str;
  char b[sizeof(telemetry_t)];
} packet_t;

//const uint64_t pipe = 0xE8E8F0F0E1LL;
const uint64_t pipe = 0x6974732074696D65; // "its time"

const uint8_t CHANNEL = 15;
