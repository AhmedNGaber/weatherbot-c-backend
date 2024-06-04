# weatherbot-c-backend
"weatherbot-c-backend" repo is the backend for WeatherBot @https://t.me/weathocastbot that can provide the Temperature of a given City Name or Geometry code [Latitude:Londitude]]

This application is dependent on [APR1 library](https://apr.apache.org) , [telebot lib](https://github.com/smartnode/telebot) and another rust library [weatherbot-rust-c-wrapper](https://github.com/AhmedNGaber/weatherbot-rust-c-wrapper) is used to wrap using  [open-meteo-api v0.1.3]([http://example.com](https://crates.io/crates/open-meteo-api/0.1.3)) 

## Usage
There are dependencies for extrnal libraries
```
sudo apt install libapr1-dev libcurl4-openssl-dev libjson-c-dev cmake binutils make rustc
```

And after cloning this repo, you need to clone telebot lib repo inside the root directory as well.
No need to do anything for telebotlib as it shall be built via CMake of this application.

Also a rust lib [weatherbot-rust-c-wrapper](https://github.com/AhmedNGaber/weatherbot-rust-c-wrapper)  is needed to be available for successfull linking.

### Clone instructions
```
git clone https://github.com/AhmedNGaber/weatherbot-c-backend.git
cd weatherbot-c-backend
git clone https://github.com/smartnode/telebot.git
git clone https://github.com/AhmedNGaber/weatherbot-rust-c-wrapper.git
```


### Build instructions
#### The required Rust wrapper Lib
```
cd weatherbot-rust-c-wrapper
cargo build
cd ..
```

#### Telebot lib and Main Application

```
mkdir build && cd build
cmake ..
make telebot
make
```
