------------------------------------------------------------------------------
--                              Ada Web Server                              --
--                                                                          --
--                            Copyright (C) 2000                            --
--                               Pascal Obry                                --
--                                                                          --
--  This library is free software; you can redistribute it and/or modify    --
--  it under the terms of the GNU General Public License as published by    --
--  the Free Software Foundation; either version 2 of the License, or (at   --
--  your option) any later version.                                         --
--                                                                          --
--  This library is distributed in the hope that it will be useful, but     --
--  WITHOUT ANY WARRANTY; without even the implied warranty of              --
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       --
--  General Public License for more details.                                --
--                                                                          --
--  You should have received a copy of the GNU General Public License       --
--  along with this library; if not, write to the Free Software Foundation, --
--  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.          --
--                                                                          --
--  As a special exception, if other files instantiate generics from this   --
--  unit, or you link this unit with other files to produce an executable,  --
--  this  unit  does not  by itself cause  the resulting executable to be   --
--  covered by the GNU General Public License. This exception does not      --
--  however invalidate any other reasons why the executable file  might be  --
--  covered by the  GNU Public License.                                     --
------------------------------------------------------------------------------

--  $Id$

with Sockets;

with AWS.Connection;

package body AWS.Server is

   Accepting_Socket : Sockets.Socket_FD;
   Incoming_Socket  : Sockets.Socket_FD;

   Line : Connection.Line_Access;

   ----------------
   -- Web_Server --
   ----------------

   task body HTTP is
   begin
      Accepting_Socket := Sockets.Socket (Sockets.AF_INET,
                                          Sockets.SOCK_STREAM);
      Sockets.Setsockopt (Accepting_Socket,
                          Sockets.SOL_SOCKET,
                          Sockets.SO_REUSEADDR,
                          1);

      Sockets.Bind (Accepting_Socket, Port);

      Sockets.Listen (Accepting_Socket);

      loop
         Incoming_Socket := Sockets.Accept_Socket (Accepting_Socket);
         Line := new Connection.Line;
         Line.Start (Incoming_Socket, CB);
      end loop;
   end HTTP;

end AWS.Server;
