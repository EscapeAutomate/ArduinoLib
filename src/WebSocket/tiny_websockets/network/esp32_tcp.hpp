#pragma once

#include <WebSocket/tiny_websockets/internals/ws_common.hpp>
#include <WebSocket/tiny_websockets/network/tcp_client.hpp>
#include <WebSocket/tiny_websockets/network/generic_esp_clients.hpp>

#include <WiFi.h>
#include <HTTPClient.h>

namespace websockets { namespace network {  
  class SecuredEsp32TcpClient : public GenericEspTcpClient<WiFiClientSecure> {
  public:
    void setCACert(const char* ca_cert) {
      this->client.setCACert(ca_cert);
    }

    void setCertificate(const char* client_ca) {
      this->client.setCertificate(client_ca);
    }

    void setPrivateKey(const char* private_key) {
        this->client.setPrivateKey(private_key);
    }

    void setInsecure() {
        this->client.setInsecure();
    }
  };
}} // websockets::network

