#ifndef ORACLE_MACROS
#define ORACLE_MACROS

#define EVENT_KP_OF(key, state) ( ((((state) << 1) + 1) << 5) | (key) )

#define EVENT_TYPE_KP_UP 1
#define EVENT_TYPE_KP_DOWN 3

#define EVENT_ID_KP_UP (EVENT_TYPE_KP_UP << 5)
#define EVENT_ID_KP_DOWN (EVENT_TYPE_KP_DOWN << 5)

// Convert serial letter to int (uint8_t)
#define sltoi(chr) ( (chr) - 'a' )
// Convert int to serial letter
#define itosl(uintv) ( (char)((uintv) + 'a') )

// Convert higher serial letter to int (uint8_t)
#define sltoih(chr) ( sltoi(chr) << 4 )
// Convert int to lower serial letter
#define itosll(uintv) ( itosl(uintv & 0b1111) )
// Convert int to higher serial letter
#define itoslh(uintv) ( itosl(uintv >> 4) )

// Extract small integer from serial letter array at index
#define asltoi(arr, idx) ( sltoi(arr[idx]) )
// Store small integer to serial letter array at index
#define stitosl(arr, idx, uintv) arr[idx] = itosl(uintv)

// Extract big integer from serial letter array at index
#define asltoi2(arr, idx) ( sltoih(arr[idx]) + sltoi(arr[idx + 1]) )
// Store big integer to serial letter array at index
#define stitosl2(arr, idx, uintv) arr[idx] = itoslh(uintv); arr[idx+1] = itosll(uintv)

#endif