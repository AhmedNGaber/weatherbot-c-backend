#ifndef WEATHERBOT_RUST_WRAPPER_H
#define WEATHERBOT_RUST_WRAPPER_H

extern void set_city_name(const char* city_name);
extern void set_city_geometry(float lat, float lng);
extern int is_city_valid(void);
extern float get_city_temperature(void);

#endif // WEATHERBOT_RUST_WRAPPER_H