# Home Link
Implementation of UDP-based smart home protocol.

## Configuration
In order to build samples create `configuration.h` file in the root directory.

```
namespace configuration {

constexpr int port = 1;
constexpr char const* ssid = "";
constexpr char const* password = "";

}
```
`ssid` and `password` are required for ESP project only.
