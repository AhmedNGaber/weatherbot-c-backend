// Stubs for telebot library functions

// Function to create a telebot instance
void* telebot_create(const char* token) {
    // Empty body
}

// Function to get information about the bot
void* telebot_get_me(void* bot) {
    // Empty body
}

// Function to destroy a telebot instance
void telebot_destroy(void* bot) {
    // Empty body
}

// Function to put the bot information
void telebot_put_me(void* bot, void* me) {
    // Empty body
}

// Function to get updates from the bot
void* telebot_get_updates(void* bot, int offset, int limit, int timeout) {
    // Empty body
}

// Function to get city temperature by geometry
float get_city_temperature_by_geometry(float latitude, float longitude) {
    // Empty body
}

// Function to check if a city name is valid
int is_city_name_valid(const char* city_name) {
    // Empty body
}

// Function to get city temperature by name
float get_city_temperature_by_name(const char* city_name) {
    // Empty body
}

// Function to send a message using the bot
void telebot_send_message(void* bot, const char* chat_id, const char* text) {
    // Empty body
}

// Function to put updates
void telebot_put_updates(void* updates) {
    // Empty body
}