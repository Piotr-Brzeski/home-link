# Home Link
Implementation of UDP-based smart home protocol.

## Build
Use [XcodeGen](https://github.com/yonaskolb/XcodeGen) to generate Xcode project
or [NinjaGen](https://github.com/Piotr-Brzeski/NinjaGen) to generate ninja build file.

## Configuration
In order to build example applications create `configuration.h` file in the root directory.

```
namespace configuration {

constexpr int port = 1;
constexpr char const* ssid = "";
constexpr char const* password = "";

}
```
`ssid` and `password` are required for ESP project only.
