------------------------------------------------------------------------------
--                              Ada Web Server                              --
--                                                                          --
--                            Copyright (C) 2006                            --
--                                 AdaCore                                  --
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

with AWS.Net.Thin;

package AWS.Net.Poll_Events is

   type Set (Initial_Size : Natural) is new FD_Set with private;

   overriding procedure Add
     (FD_Set : in out Set;
      FD     : in     FD_Type;
      Event  : in     Wait_Event_Set);

   overriding procedure Set_Mode
     (FD_Set : in out Set; Index : in Positive; Mode : in Wait_Event_Set);

   overriding procedure Reallocate
     (FD_Set : in out Set; Size : in Natural);

   overriding procedure Remove (FD_Set : in out Set; Index : in Positive);

   overriding function Length (FD_Set : in Set) return Natural;

   overriding function Size (FD_Set : in Set) return Natural;

   overriding procedure Wait
     (FD_Set : in out Set; Timeout : in Duration; Count : out Natural);

   overriding procedure Next (FD_Set : in Set; Index : in out Positive);

   overriding function Status
     (FD_Set : in Set; Index : in Positive) return Event_Set;

   overriding procedure Free (FD_Set : in out Set);

private

   type Poll_Set is array (Positive range <>) of Thin.Pollfd;
   pragma Pack (Poll_Set);

   type Poll_Set_Access is access Poll_Set;

   type Set (Initial_Size : Natural) is new FD_Set with record
      Length : Natural := 0;
      Size   : Natural := Initial_Size;
      Fds    : Poll_Set_Access := new Poll_Set (1 .. Initial_Size);
   end record;

end AWS.Net.Poll_Events;