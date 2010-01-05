/***************************************************************************/
/*                             Ada Web Server                              */
/*                                                                         */
/*                    Copyright (C) 2000-2010, AdaCore                     */
/*                                                                         */
/* This library is free software; you can redistribute it and/or modify    */
/* it under the terms of the GNU General Public License as published by    */
/* the Free Software Foundation; either version 2 of the License, or (at   */
/* your option) any later version.                                         */
/*                                                                         */
/* This library is distributed in the hope that it will be useful, but     */
/* WITHOUT ANY WARRANTY; without even the implied warranty of              */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       */
/* General Public License for more details.                                */
/*                                                                         */
/* You should have received a copy of the GNU General Public License       */
/* along with this library; if not, write to the Free Software Foundation, */
/* Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.          */
/*                                                                         */
/* As a special exception, if other files instantiate generics from this   */
/* unit, or you link this unit with other files to produce an executable,  */
/* this  unit  does not  by itself cause  the resulting executable to be   */
/* covered by the GNU General Public License. This exception does not      */
/* however invalidate any other reasons why the executable file  might be  */
/* covered by the  GNU Public License.                                     */
/***************************************************************************/

/* This program is used to get some OS specific settings */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __vxworks
#include "vxWorks.h"
#include "sockLib.h"
#include "inetLib.h"
#include "stdioLib.h"
#include "strLib.h"
#include "hostLib.h"
#include "ioLib.h"
#include <limits.h>
#endif

#ifdef _WIN32
#include <ws2tcpip.h>
#else

#ifndef __vxworks
#include <sys/poll.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#endif

#ifdef sun
#include <sys/filio.h>
#endif

FILE *fd;

void
P (const char *str, ...)
{
  va_list ap;

  va_start (ap, str);
#if defined(__vxworks)
  vprintf (str, ap);
#else
  vfprintf (fd, str, ap);
#endif
  va_end (ap);
}

/* For systems without IPv6 support */

#ifndef AI_PASSIVE
#define NO_IPV6_SUPPORT
#define AI_PASSIVE -1
#define AI_CANONNAME -1
#define AI_NUMERICHOST -1
#define EAI_SYSTEM -1
#define PF_INET6 -1
#define AF_INET6 -1
#endif

int
main (int argc, char *argv[])
{
  const struct timeval tv;

#ifndef NO_IPV6_SUPPORT
  const struct addrinfo ai;

  const void *ai_ptr = &ai;

  const int ai_flags_offset     = (void *)&ai.ai_flags     - ai_ptr;
  const int ai_family_offset    = (void *)&ai.ai_family    - ai_ptr;
  const int ai_socktype_offset  = (void *)&ai.ai_socktype  - ai_ptr;
  const int ai_protocol_offset  = (void *)&ai.ai_protocol  - ai_ptr;
  const int ai_addrlen_offset   = (void *)&ai.ai_addrlen   - ai_ptr;
  const int ai_addr_offset      = (void *)&ai.ai_addr      - ai_ptr;
  const int ai_canonname_offset = (void *)&ai.ai_canonname - ai_ptr;
  const int ai_next_offset      = (void *)&ai.ai_next      - ai_ptr;
#else
  const int ai_flags_offset     = -8;
  const int ai_family_offset    = -7;
  const int ai_socktype_offset  = -6;
  const int ai_protocol_offset  = -5;
  const int ai_addrlen_offset   = -4;
  const int ai_addr_offset      = -3;
  const int ai_canonname_offset = -2;
  const int ai_next_offset      = -1;
#endif

#if defined(__FreeBSD__) || defined(_WIN32) || defined(__vxworks)
  const int s_nfds_t = sizeof (int) * 8;
#else
  const int s_nfds_t = sizeof (nfds_t) * 8;
#endif

#if defined(_WIN32)

  #if !defined SHUT_RD && defined SD_RECEIVE
  #define SHUT_RD SD_RECEIVE
  #endif
  #if !defined SHUT_WR && defined SD_SEND
  #define SHUT_WR SD_SEND
  #endif
  #if !defined SHUT_RDWR && defined SD_BOTH
  #define SHUT_RDWR SD_BOTH
  #endif

  #define ETIMEDOUT   WSAETIMEDOUT
  #define ENOTCONN    WSAENOTCONN
  #define EWOULDBLOCK WSAEWOULDBLOCK
  #define EINPROGRESS WSAEINPROGRESS
  #define EINTR       WSAEINTR
  #define ENOBUFS     WSAENOBUFS
  #define ENOMEM      WSAENOBUFS
  #define EAI_SYSTEM  EAI_AGAIN
  //  EAI_SYSTEM getaddrinfo error code does not supported by Win32, but need
  //  for correct error handling, we could use any Win32 socket error code
  //  instead, because Win32 use the same error codes in Errno report.

  const char *i_conv      = "Stdcall";
  const int s_fd_type     = sizeof (SOCKET) * 8;
  const int s_events_type = sizeof (short) * 8;
  const int s_socklen_t   = sizeof (size_t) * 8;
  const int v_POLLIN      = 1;
  const int v_POLLPRI     = 2;
  const int v_POLLOUT     = 4;
  const int v_POLLERR     = 8;
  const int v_POLLHUP     = 16;
  const int v_POLLNVAL    = 32;

#elif defined(__vxworks)
  const char *i_conv      = "C";
  const int s_fd_type     = sizeof (int) * 8;
  const int s_events_type = sizeof (short) * 8;
  const int s_socklen_t   = sizeof (size_t) * 8;
  const int v_POLLIN      = 1;
  const int v_POLLPRI     = 2;
  const int v_POLLOUT     = 4;
  const int v_POLLERR     = 8;
  const int v_POLLHUP     = 16;
  const int v_POLLNVAL    = 32;

#else
  const char *i_conv      = "C";
  const struct pollfd v_pollfd;
  const int s_fd_type     = sizeof (v_pollfd.fd) * 8;
  const int s_events_type = sizeof (v_pollfd.events) * 8;
  const int s_socklen_t   = sizeof (socklen_t) * 8;
  const int v_POLLIN      = POLLIN;
  const int v_POLLPRI     = POLLPRI;
  const int v_POLLOUT     = POLLOUT;
  const int v_POLLERR     = POLLERR;
  const int v_POLLHUP     = POLLHUP;
  const int v_POLLNVAL    = POLLNVAL;
#endif

#ifdef MSG_NOSIGNAL
  const int v_MSG_NOSIGNAL = MSG_NOSIGNAL;
#else
  const int v_MSG_NOSIGNAL = 0;
#endif

  const int v_FD_SETSIZE = FD_SETSIZE;
  const int s_timeval_s  = sizeof (tv.tv_sec) * 8;

#if !defined(__vxworks)
  /* Open output file in binary mode to ensure UNIX line ending */

  if (argc == 2)
    fd = fopen (argv[1], "wb");
  else
    {
      fprintf (stderr, "Syntax: check_config <filename>\n");
      exit (1);
    }
#endif

  P ("\n--  AUTOMATICALLY GENERATED, DO NOT EDIT THIS FILE\n\n");
  P ("--  Generator: check_config\n\n");

  P ("with Interfaces.C.Strings;\n");
  P ("with System;\n");
#ifndef _WIN32
  P ("with GNAT.OS_Lib;\n");
#endif
  P ("\n");

  P ("package AWS.OS_Lib is\n\n");
  P ("   use Interfaces;\n\n");

#ifdef _WIN32
  //  libws2_32.a need for getaddrinfo freeaddrinfo routines in Windows
  //  XP/2003.
  P ("   pragma Linker_Options (\"-lws2_32\");\n\n");

  P ("   Executable_Extension : constant String := \".exe\";\n");
  P ("   Directory_Separator  : constant Character := '\\';\n");
  P ("   Path_Separator       : constant Character := ';';\n\n");
#else
  P ("   Executable_Extension : constant String := \"\";\n");
  P ("   Directory_Separator  : constant Character := '/';\n");
  P ("   Path_Separator       : constant Character := ':';\n\n");
#endif

  /* POLL constants */

  P ("   FD_SETSIZE : constant := %d;\n", v_FD_SETSIZE);
  P ("   POLLIN     : constant := %d;\n", v_POLLIN);
  P ("   POLLPRI    : constant := %d;\n", v_POLLPRI);
  P ("   POLLOUT    : constant := %d;\n", v_POLLOUT);
  P ("   POLLERR    : constant := %d;\n", v_POLLERR);
  P ("   POLLHUP    : constant := %d;\n", v_POLLHUP);
  P ("   POLLNVAL   : constant := %d;\n\n", v_POLLNVAL);

  /* getaddrinfo constants */

  P ("   AI_PASSIVE     : constant := %d;\n", AI_PASSIVE);
  P ("   AI_CANONNAME   : constant := %d;\n", AI_CANONNAME);
  P ("   AI_NUMERICHOST : constant := %d;\n", AI_NUMERICHOST);
  P ("   EAI_SYSTEM     : constant := %d;\n\n", EAI_SYSTEM);

  /* getaddrname flags */

  P ("   NI_NAMEREQD    : constant := %d;\n", NI_NAMEREQD);
  P ("   NI_DGRAM       : constant := %d;\n", NI_DGRAM);
  P ("   NI_NOFQDN      : constant := %d;\n", NI_NOFQDN);
  P ("   NI_NUMERICHOST : constant := %d;\n", NI_NUMERICHOST);
  P ("   NI_NUMERICSERV : constant := %d;\n\n", NI_NUMERICSERV);

  /* other constants */

  P ("   IPPROTO_TCP  : constant := %d;\n", IPPROTO_TCP);
  P ("   IPPROTO_IP   : constant := %d;\n", IPPROTO_IP);
  P ("   PF_UNSPEC    : constant := %d;\n", PF_UNSPEC);
  P ("   PF_INET      : constant := %d;\n", PF_INET);
  P ("   PF_INET6     : constant := %d;\n", PF_INET6);
  P ("   AF_INET      : constant := %d;\n", AF_INET);
  P ("   AF_INET6     : constant := %d;\n", AF_INET6);
  P ("   SO_ERROR     : constant := %d;\n", SO_ERROR);
  P ("   SO_SNDBUF    : constant := %d;\n", SO_SNDBUF);
  P ("   SO_RCVBUF    : constant := %d;\n", SO_RCVBUF);
  P ("   SO_REUSEADDR : constant := %d;\n", SO_REUSEADDR);
  P ("   TCP_NODELAY  : constant := %d;\n", TCP_NODELAY);
  P ("   SOCK_STREAM  : constant := %d;\n", SOCK_STREAM);
  P ("   SOL_SOCKET   : constant := %d;\n", SOL_SOCKET);
  P ("   SHUT_RDWR    : constant := %d;\n", SHUT_RDWR);
  P ("   SHUT_RD      : constant := %d;\n", SHUT_RD);
  P ("   SHUT_WR      : constant := %d;\n", SHUT_WR);
  P ("   ETIMEDOUT    : constant := %d;\n", ETIMEDOUT);
  P ("   EWOULDBLOCK  : constant := %d;\n", EWOULDBLOCK);
  P ("   ENOTCONN     : constant := %d;\n", ENOTCONN);
  P ("   EINPROGRESS  : constant := %d;\n", EINPROGRESS);
  P ("   EINTR        : constant := %d;\n", EINTR);
  P ("   ENOBUFS      : constant := %d;\n", ENOBUFS);
  P ("   ENOMEM       : constant := %d;\n", ENOMEM);
  P ("   FIONBIO      : constant := %d;\n", FIONBIO);
  P ("   FIONREAD     : constant := %d;\n", FIONREAD);
  P ("   MSG_NOSIGNAL : constant := %d;\n\n", v_MSG_NOSIGNAL);

  /* nfds_t */

  P ("   type nfds_t is mod 2 ** %d;\n", s_nfds_t);
  P ("   for nfds_t'Size use %d;\n\n", s_nfds_t);

  /* FD_Type */

  P ("   type FD_Type is mod 2 ** %d;\n", s_fd_type);
  P ("   for FD_Type'Size use %d;\n\n", s_fd_type);

  /* Events_Type */

  P ("   type Events_Type is mod 2 ** %d;\n", s_events_type);
  P ("   for Events_Type'Size use %d;\n\n", s_events_type);

  /* socklen_t */

  P ("   type socklen_t is mod 2 ** %d;\n", s_socklen_t);
  P ("   for socklen_t'Size use %d;\n\n", s_socklen_t);

  /* timeval */

  P ("   type timeval_field_t is range -(2 ** %d) .. 2 ** %d - 1;\n",
     s_timeval_s - 1, s_timeval_s - 1);
  P ("   for timeval_field_t'Size use %d;\n\n", s_timeval_s);

  P ("   type Timeval is record\n");
  P ("      tv_sec  : timeval_field_t; -- Seconds\n");
  P ("      tv_usec : timeval_field_t; -- Microseconds\n");
  P ("   end record;\n");
  P ("   pragma Convention (C, Timeval);\n\n");

  /* Addr_Info */

  if (ai_flags_offset > 0
      || ai_family_offset    >= ai_socktype_offset
      || ai_socktype_offset  >= ai_protocol_offset
      || ai_addrlen_offset   >= ai_addr_offset
      || ai_addrlen_offset   >= ai_canonname_offset
      || ai_addr_offset      >= ai_next_offset
      || ai_canonname_offset >= ai_next_offset)
      /*  Broke source code because of */
      P ("   Unexpected addrinfo fields order.");

  P ("   type Addr_Info;\n");
  P ("   type Addr_Info_Access is access all Addr_Info;\n\n");

  P ("   type Addr_Info is record\n");
  P ("      ai_flags     : C.int;\n");
  P ("      ai_family    : C.int;\n");
  P ("      ai_socktype  : C.int;\n");
  P ("      ai_protocol  : C.int;\n");
  P ("      ai_addrlen   : socklen_t;\n");

  if (ai_canonname_offset < ai_addr_offset) {
    P ("      ai_canonname : C.Strings.chars_ptr;\n");
    P ("      ai_addr      : System.Address;\n");
  } else {
    P ("      ai_addr      : System.Address;\n");
    P ("      ai_canonname : C.Strings.chars_ptr;\n");
  }

  P ("      ai_next      : Addr_Info_Access;\n");
  P ("   end record;\n");
  P ("   pragma Convention (C, Addr_Info);\n\n");

  P ("   function GetAddrInfo\n");
  P ("     (node    : C.Strings.chars_ptr;\n");
  P ("      service : C.Strings.chars_ptr;\n");
  P ("      hints   : Addr_Info;\n");
  P ("      res     : not null access Addr_Info_Access) return C.int;\n\n");

  P ("   procedure FreeAddrInfo (res : Addr_Info_Access);\n\n");

  P ("   function GAI_StrError (ecode : C.int)");
  P (" return C.Strings.chars_ptr;\n\n");

  P ("   function Socket_StrError (ecode : Integer)");
  P (" return C.Strings.chars_ptr;\n\n");

  P ("   procedure FD_ZERO (Set : System.Address);\n");
  P ("   procedure FD_SET (FD : FD_Type; Set : System.Address);\n");
  P ("   function FD_ISSET (FD : FD_Type; Set : System.Address)");
  P (" return C.int;\n\n");

  P ("   function Set_Sock_Opt\n");
  P ("     (S       : C.int;\n");
  P ("      Level   : C.int;\n");
  P ("      OptName : C.int;\n");
  P ("      OptVal  : System.Address;\n");
  P ("      OptLen  : C.int) return C.int;\n\n");

  P ("   function C_Ioctl\n");
  P ("     (S   : C.int;\n");
  P ("      Req : C.int;\n");
  P ("      Arg : access C.int) return C.int;\n\n");

  P ("   function C_Close (Fd : C.int) return C.int;\n\n");

  P ("   procedure WSA_Startup\n");
  P ("     (Version : C.int; Data : System.Address)");
#ifdef _WIN32
  P (";\n");
  P ("   pragma Import (Stdcall, WSA_Startup, \"WSAStartup\");\n\n");
#else
  P (" is null;\n");
#endif

  P ("   function Socket_Errno return Integer");
#ifndef _WIN32
  P (" renames GNAT.OS_Lib.Errno");
#endif
  P (";\n\n");

  P ("private\n\n");

  P ("   pragma Import (%s, GetAddrInfo, \"getaddrinfo\");\n", i_conv);
  P ("   pragma Import (%s, FreeAddrInfo, \"freeaddrinfo\");\n", i_conv);

#ifdef _WIN32
  //  gai_strerror for Win32 inlined in WS2TCPIP.H and is not thread safe.
  //  We are using simple replacement in win32/gai_strerror.c

  P ("   pragma Import (C, GAI_StrError, \"AWS_gai_strerror\");\n");

  //  There is no way in Windows to get error message from socket error code.
  //  Use the own partial implementation of the POSIX strerror.

  P ("   pragma Import (C, Socket_StrError, \"socket_strerror\");\n");
  P ("   pragma Import (Stdcall, C_Ioctl, \"ioctlsocket\");\n");
  P ("   pragma Import (Stdcall, C_Close, \"closesocket\");\n");
  P ("   pragma Import (Stdcall, Socket_Errno, \"WSAGetLastError\");\n");
#else
  P ("   pragma Import (C, GAI_StrError, \"gai_strerror\");\n");
  P ("   pragma Import (C, Socket_StrError, \"strerror\");\n");
  P ("   pragma Import (C, C_Ioctl, \"ioctl\");\n");
  P ("   pragma Import (C, C_Close, \"close\");\n");
#endif

  P ("   pragma Import (%s, Set_Sock_Opt, \"setsockopt\");\n", i_conv);
  P ("   pragma Import (C, FD_ZERO, \"__aws_clear_socket_set\");\n");
  P ("   pragma Import (C, FD_SET, \"__aws_set_socket_in_set\");\n");
  P ("   pragma Import (C, FD_ISSET, \"__aws_is_socket_in_set\");\n\n");

  P ("end AWS.OS_Lib;\n");
  fclose (fd);

  return 0;
}