#ifndef WEATHERBOT_RUST_WRAPPER_H
#define WEATHERBOT_RUST_WRAPPER_H

#define INVALID_CITY_NAME    (-1)
#define INVALID_LATITUDE     (-2)
#define INVALID_LONGITUDE    (-3)

extern float get_city_temperature_by_geometry(float lat, float lng);
extern int is_city_name_valid(const char* city_name);
extern int is_city_geometry_valid(float lat, float lng);
extern float get_city_temperature_by_name(const char* city_name);



#endif // WEATHERBOT_RUST_WRAPPER_H