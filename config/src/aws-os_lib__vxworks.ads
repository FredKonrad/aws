------------------------------------------------------------------------------
--                              Ada Web Server                              --
--                                                                          --
--                       Copyright (C) 2012, AdaCore                        --
--                                                                          --
--  This library is free software;  you can redistribute it and/or modify   --
--  it under terms of the  GNU General Public License  as published by the  --
--  Free Software  Foundation;  either version 3,  or (at your  option) any --
--  later version. This library is distributed in the hope that it will be  --
--  useful, but WITHOUT ANY WARRANTY;  without even the implied warranty of --
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                    --
--                                                                          --
--  As a special exception under Section 7 of GPL version 3, you are        --
--  granted additional permissions described in the GCC Runtime Library     --
--  Exception, version 3.1, as published by the Free Software Foundation.   --
--                                                                          --
--  You should have received a copy of the GNU General Public License and   --
--  a copy of the GCC Runtime Library Exception along with this program;    --
--  see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see   --
--  <http://www.gnu.org/licenses/>.                                         --
--                                                                          --
--  As a special exception, if other files instantiate generics from this   --
--  unit, or you link this unit with other files to produce an executable,  --
--  this  unit  does not  by itself cause  the resulting executable to be   --
--  covered by the GNU General Public License. This exception does not      --
--  however invalidate any other reasons why the executable file  might be  --
--  covered by the  GNU Public License.                                     --
------------------------------------------------------------------------------

--  AUTOMATICALLY GENERATED, DO NOT EDIT THIS FILE

--  Generator: check_config
--  TARGET:    vxworks

with Interfaces.C.Strings;
with System;

with GNAT.OS_Lib;

package AWS.OS_Lib is

   use Interfaces;

   Executable_Extension : constant String := "";
   Directory_Separator  : constant Character := '/';
   Path_Separator       : constant Character := ':';

   FD_SETSIZE : constant := 2048;
   POLLIN     : constant := 1;
   POLLPRI    : constant := 2;
   POLLOUT    : constant := 4;
   POLLERR    : constant := 8;
   POLLHUP    : constant := 16;
   POLLNVAL   : constant := 32;

   AI_PASSIVE     : constant := 1;
   AI_CANONNAME   : constant := 2;
   AI_NUMERICHOST : constant := 4;
   EAI_SYSTEM     : constant := 11;

   NI_NAMEREQD    : constant := 4;
   NI_DGRAM       : constant := 16;
   NI_NOFQDN      : constant := 1;
   NI_NUMERICHOST : constant := 2;
   NI_NUMERICSERV : constant := 8;

   IPPROTO_TCP  : constant := 6;
   IPPROTO_IP   : constant := 0;
   PF_UNSPEC    : constant := 0;
   PF_INET      : constant := 2;
   PF_INET6     : constant := 28;
   AF_INET      : constant := 2;
   AF_INET6     : constant := 28;
   SO_ERROR     : constant := 4103;
   SO_SNDBUF    : constant := 4097;
   SO_RCVBUF    : constant := 4098;
   SO_REUSEADDR : constant := 4;
   TCP_NODELAY  : constant := 1;
   SOCK_STREAM  : constant := 1;
   SOL_SOCKET   : constant := 65535;
   SHUT_RDWR    : constant := 2;
   SHUT_RD      : constant := 0;
   SHUT_WR      : constant := 1;
   ETIMEDOUT    : constant := 60;
   EWOULDBLOCK  : constant := 70;
   ENOTCONN     : constant := 57;
   EINPROGRESS  : constant := 68;
   EINTR        : constant := 4;
   ENOBUFS      : constant := 55;
   ENOMEM       : constant := 12;
   FIONBIO      : constant := 16;
   FIONREAD     : constant := 1;
   MSG_NOSIGNAL : constant := 0;

   type nfds_t is mod 2 ** 32;
   for nfds_t'Size use 32;

   type FD_Type is mod 2 ** 32;
   for FD_Type'Size use 32;

   type Events_Type is mod 2 ** 16;
   for Events_Type'Size use 16;

   type socklen_t is mod 2 ** 32;
   for socklen_t'Size use 32;

   type timeval_field_t is range -(2 ** 31) .. 2 ** 31 - 1;
   for timeval_field_t'Size use 32;

   type Timeval is record
      tv_sec  : timeval_field_t; -- Seconds
      tv_usec : timeval_field_t; -- Microseconds
   end record;
   pragma Convention (C, Timeval);

   type Addr_Info;
   type Addr_Info_Access is access all Addr_Info;

   type Addr_Info is record
      ai_flags     : C.int;
      ai_family    : C.int;
      ai_socktype  : C.int;
      ai_protocol  : C.int;
      ai_addrlen   : socklen_t;
      ai_canonname : C.Strings.chars_ptr;
      ai_addr      : System.Address;
      ai_next      : Addr_Info_Access;
   end record;
   pragma Convention (C, Addr_Info);

   function GetAddrInfo
     (node    : C.Strings.chars_ptr;
      service : C.Strings.chars_ptr;
      hints   : Addr_Info;
      res     : not null access Addr_Info_Access) return C.int;

   procedure FreeAddrInfo (res : Addr_Info_Access);

   function GAI_StrError (ecode : C.int) return C.Strings.chars_ptr;

   function Socket_StrError (ecode : Integer) return C.Strings.chars_ptr;

   procedure FD_ZERO (Set : System.Address);
   procedure FD_SET (FD : FD_Type; Set : System.Address);
   function FD_ISSET (FD : FD_Type; Set : System.Address) return C.int;

   function Set_Sock_Opt
     (S       : C.int;
      Level   : C.int;
      OptName : C.int;
      OptVal  : System.Address;
      OptLen  : C.int) return C.int;

   function C_Ioctl
     (S   : C.int;
      Req : C.int;
      Arg : access C.int) return C.int;

   function C_Close (Fd : C.int) return C.int;

   procedure WSA_Startup
     (Version : C.int; Data : System.Address) is null;
   function Socket_Errno return Integer renames GNAT.OS_Lib.Errno;

private

   pragma Import (C, GetAddrInfo, "getaddrinfo");
   pragma Import (C, FreeAddrInfo, "freeaddrinfo");
   pragma Import (C, GAI_StrError, "gai_strerror");
   pragma Import (C, Socket_StrError, "strerror");
   pragma Import (C, C_Ioctl, "ioctl");
   pragma Import (C, C_Close, "close");
   pragma Import (C, Set_Sock_Opt, "setsockopt");
   pragma Import (C, FD_ZERO, "__aws_clear_socket_set");
   pragma Import (C, FD_SET, "__aws_set_socket_in_set");
   pragma Import (C, FD_ISSET, "__aws_is_socket_in_set");

end AWS.OS_Lib;
