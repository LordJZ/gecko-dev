/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_ipc_streamsocket_h
#define mozilla_ipc_streamsocket_h

#include "ConnectionOrientedSocket.h"

namespace mozilla {
namespace ipc {

class StreamSocketIO;
class UnixSocketConnector;

class StreamSocket : public ConnectionOrientedSocket
{
public:
  StreamSocket();

  /**
   * Method to be called whenever data is received. This is only called on the
   * main thread.
   *
   * @param aBuffer Data received from the socket.
   */
  virtual void ReceiveSocketData(nsAutoPtr<UnixSocketBuffer>& aBuffer) = 0;

  /**
   * Convenience function for sending strings to the socket (common in bluetooth
   * profile usage). Converts to a UnixSocketRawData struct. Can only be called
   * on originating thread.
   *
   * TODO: Move this method into Bluetooth module.
   *
   * @param aMessage String to be sent to socket
   *
   * @return true if data is queued, false otherwise (i.e. not connected)
   */
  bool SendSocketData(const nsACString& aMessage);

  /**
   * Starts a task on the socket that will try to connect to a socket in a
   * non-blocking manner.
   *
   * @param aConnector Connector object for socket type specific functions
   * @param aAddress Address to connect to.
   * @param aDelayMs Time delay in milli-seconds.
   *
   * @return true on connect task started, false otherwise.
   */
  bool Connect(UnixSocketConnector* aConnector,
               const char* aAddress,
               int aDelayMs = 0);

  /**
   * Queues the internal representation of socket for deletion. Can be called
   * from main thread.
   */
  void Close();

  /**
   * Get the current sockaddr for the socket
   */
  void GetSocketAddr(nsAString& aAddrStr);

  // Methods for |DataSocket|
  //

  void SendSocketData(UnixSocketIOBuffer* aBuffer) override;

  // Methods for |SocketBase|
  //

  void CloseSocket() override;

protected:
  virtual ~StreamSocket();

  // Prepares an instance of |StreamSocket| in DISCONNECTED state
  // for accepting a connection. Subclasses implementing |GetIO|
  // need to call this method.
  ConnectionOrientedSocketIO* PrepareAccept(UnixSocketConnector* aConnector);

private:
  StreamSocketIO* mIO;
};

} // namespace ipc
} // namepsace mozilla

#endif // mozilla_ipc_streamsocket_h
