#pragma once
#include <string>
class Certificate;
class ConnectionRequest;
class LoginPacket {
public:
  // char filler[40];
  std::unique_ptr<ConnectionRequest, std::default_delete<ConnectionRequest>> *getRq() {
    return (std::unique_ptr<ConnectionRequest, std::default_delete<ConnectionRequest>> *) ((char *) this + 40);
  }
};
class ConnectionRequest {
public:
  std::string toString();
  Certificate *getCertificate() const;
  std::string getDeviceOS();
};
class ExtendedCertificate {
public:
  static std::string getIdentityName(Certificate const &);
  static std::string getXuid(Certificate const &);
};
