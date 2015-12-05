#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../../include/udp/udp_socket.h"
#include "../../include/inet_address.h"
#include "../../include/logging.h"

namespace collie {
namespace udp {

UDPSocket::UDPSocket(std::shared_ptr<InetAddress> local_addr)
    : Descriptor(-1, false), local_address_(local_addr) {
  CreateImpl();
}

UDPSocket::~UDPSocket() {
  CloseImpl();
}

void UDPSocket::Create() { CreateImpl(); }

void UDPSocket::Close() { CloseImpl(); }

void UDPSocket::CreateImpl() noexcept {
  if (is_init_) return;
  fd_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (fd_ < 0) {
    LOG(WARNING) << "UDPSocket: " << GetSystemError();
  } else {
    is_init_ = true;
  }
}

void UDPSocket::CloseImpl() noexcept {
  if (is_init_ && !is_close_) {
    ::close(fd_);
    is_close_ = true;
  }
}

void UDPSocket::Listen() {
  CHECK(local_address_ && local_address_->ip_version() != IP::UNKNOWN);
  CHECK_SYS(fd_ != -1);
  if (local_address_->ip_version() == IP::V4) {
    ListenV4();
  } else {
    ListenV6();
  }
}

void UDPSocket::ListenV4() {
  LOG(DEBUG) << "UDP Socket is listening";
  struct sockaddr_in serv_address = local_address_->addr_v4();

  int ret = ::bind(fd_, (struct sockaddr *)&serv_address, sizeof(serv_address));
  CHECK_SYS(ret != -1);
}

void UDPSocket::ListenV6() {
  // TODO
}

void UDPSocket::Connect(IP ip_version) {
  CHECK(ip_version != IP::UNKNOWN);
  if (ip_version == IP::V4) {
    ConnectV4();
  } else {
    ConnectV6();
  }
}

void UDPSocket::ConnectV4() {
  LOG(DEBUG) << "UDP Socket is connecting";
  if (!is_init_) Create();
  CHECK_SYS(fd_ != -1);
}

void UDPSocket::ConnectV6() {
  // TODO
}
}
}
