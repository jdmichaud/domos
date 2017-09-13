This overlay add files used to easily configure the WiFi interface either as
a standard wifi client (necessitate an WiFi Access Point) or as a WiFi Access
Point.

On each root login, `.profile` executes `check_configuration` which will check
if `/etc/network/interfaces` contains the string {{SSID}}. If it does, it
launches `configure_wifi`.

`configure_wifi` then prompts the user to configure either as a WiFi client or
an AP. Depending on the choice, `configure_wifi` will write the proper
configuration files based on the templaces present in `/etc/ezwifi`, stop and
start the necessary processes/daemons (reboot can happen).
