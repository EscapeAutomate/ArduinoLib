#pragma once

#include <WebSocket/tiny_websockets/ws_config_defs.hpp>
#include <string>
#include <Arduino.h>

namespace websockets {
    typedef std::string WSString;
    typedef String WSInterfaceString;

    namespace internals {
        WSString fromInterfaceString(const WSInterfaceString& str);
        WSString fromInterfaceString(const WSInterfaceString&& str);
        WSInterfaceString fromInternalString(const WSString& str);
        WSInterfaceString fromInternalString(const WSString&& str);
    }
}

#define PLATFORM_DOES_NOT_SUPPORT_BLOCKING_READ

#include <WebSocket/tiny_websockets/network/esp32_tcp.hpp>
#define WSDefaultSecuredTcpClient websockets::network::SecuredEsp32TcpClient
