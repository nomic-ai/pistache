/* peer.h
   Mathieu Stefani, 12 August 2015

  A class representing a TCP Peer
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <pistache/async.h>
#include <pistache/net.h>
#include <pistache/os.h>
#include <pistache/stream.h>

#ifdef PISTACHE_USE_SSL

#include <openssl/ssl.h>

#endif /* PISTACHE_USE_SSL */

namespace Pistache {
namespace Http {
namespace Private {
class ParserBase;
}
} // namespace Http
namespace Tcp {

class Transport;

class Peer {
public:
  friend class Transport;

  ~Peer();

  static std::shared_ptr<Peer> Create(Fd fd, const Address &addr);

  const Address &address() const;
  const std::string &hostname();
  Fd fd() const;

  void associateSSL(void *ssl);
  void *ssl(void) const;

  void putData(std::string name,
               std::shared_ptr<Pistache::Http::Private::ParserBase> data);
  std::shared_ptr<Pistache::Http::Private::ParserBase>
  getData(std::string name) const;
  std::shared_ptr<Pistache::Http::Private::ParserBase>
  tryGetData(std::string name) const;

  Async::Promise<ssize_t> send(const RawBuffer &buffer, int flags = 0);

protected:
  Peer(Fd fd, const Address &addr);

private:
  void associateTransport(Transport *transport);
  Transport *transport() const;

  Transport *transport_ = nullptr;
  Fd fd_ = -1;
  Address addr;

  std::string hostname_;
  std::unordered_map<std::string,
                     std::shared_ptr<Pistache::Http::Private::ParserBase>>
      data_;

  void *ssl_ = nullptr;
};

std::ostream &operator<<(std::ostream &os, Peer &peer);

} // namespace Tcp
} // namespace Pistache
