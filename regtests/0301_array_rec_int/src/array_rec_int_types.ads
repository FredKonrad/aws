------------------------------------------------------------------------------
--                              Ada Web Server                              --
--                                                                          --
--                       Copyright (C) 2017, AdaCore                        --
--                                                                          --
--  This is free software;  you can redistribute it  and/or modify it       --
--  under terms of the  GNU General Public License as published  by the     --
--  Free Software  Foundation;  either version 3,  or (at your option) any  --
--  later version.  This software is distributed in the hope  that it will  --
--  be useful, but WITHOUT ANY WARRANTY;  without even the implied warranty --
--  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU     --
--  General Public License for  more details.                               --
--                                                                          --
--  You should have  received  a copy of the GNU General  Public  License   --
--  distributed  with  this  software;   see  file COPYING3.  If not, go    --
--  to http://www.gnu.org/licenses for a complete copy of the license.      --
------------------------------------------------------------------------------

with SOAP.Utils;

package Array_Rec_Int_Types is

   type Integer_List_Type is array (Positive range <>) of Integer;

   type Integer_List_Access is access Integer_List_Type;

   package Integer_List_Type_Safe_Pointer is
      new SOAP.Utils.Safe_Pointers (Integer_List_Type, Integer_List_Access);

   type Array_Integer_Type is record
      Value : Integer_List_Type_Safe_Pointer.Safe_Pointer;
   end record;

end Array_Rec_Int_Types;
