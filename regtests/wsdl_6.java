//--------------------------------------------------------------------------
//                              Ada Web Server                              
//                                                                          
//                            Copyright (C) 2003                            
//                                ACT-Europe                                
//                                                                          
//  Authors: Dmitriy Anisimokv - Pascal Obry                                
//                                                                          
//  This library is free software; you can redistribute it and/or modify    
//  it under the terms of the GNU General Public License as published by    
//  the Free Software Foundation; either version 2 of the License, or (at   
//  your option) any later version.                                         
//                                                                          
//  This library is distributed in the hope that it will be useful, but     
//  WITHOUT ANY WARRANTY; without even the implied warranty of              
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       
//  General Public License for more details.                                
//                                                                          
//  You should have received a copy of the GNU General Public License       
//  along with this library; if not, write to the Free Software Foundation, 
//  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.          
//                                                                          
//  As a special exception, if other files instantiate generics from this   
//  unit, or you link this unit with other files to produce an executable,  
//  this  unit  does not  by itself cause  the resulting executable to be   
//  covered by the GNU General Public License. This exception does not      
//  however invalidate any other reasons why the executable file  might be  
//  covered by the  GNU Public License.                                     
//--------------------------------------------------------------------------
//
// $Id$
//
// This program can be used to test the AWS SOAP/WSDL implementation using the
// Axis application server based on Tomcat.
//
// First launch the AWS server:
//
//    $ wsdl_6_main -j
//
// Create the Java stub from WSDL document:
//
//   $ java org.apache.axis.wsdl.WSDL2Java wsdl_6.wsdl
//
// Compile generated code:
//
//   $ javac WSDL_6/aws/*.java
//
// Compile this Java client program:
//
//   $ javac wsdl_6.java
//
// Then execute it:
//
//   $ java wsdl_6
//
// The output should be similar to wsdl_6.out (except for the float format).

import WSDL_6.aws.*;

public class wsdl_6 {

    protected static void printlnRec (WSDL_6.aws.Rec rec) {
	System.out.println ("-----");
	System.out.println (rec.getA());
	System.out.println (rec.getB());
	System.out.println (rec.getC());
	System.out.println (rec.getD());
	System.out.println (rec.getE());
	System.out.println (rec.isF());
	System.out.println ("-----");
    }

    public static void main(String args[]) throws Exception {

        // Make a service
        WSDL_6_Service service = new WSDL_6_ServiceLocator();
 
        // Now use the service to get a stub which implements the SDI.
        WSDL_6_PortType port = service.getWSDL_6_Port();
 
        // Make the actual call
        // System.out.println (port.plus (12));
	System.out.println (port.next (Color.Red));

	// Call echo routines

	System.out.println (port.echo_Int (8));
	System.out.println (port.echo_Int (3));
	System.out.println (port.echo_Float ((float)89.12));
	System.out.println (port.echo_Double (998877.123456));
	System.out.println (port.echo_Boolean (true));
	System.out.println (port.echo_Boolean (false));

	// Rec

	Rec data = new Rec();
	data.setA (6);
	data.setB ((float)0.1);
	data.setC (0.2);

	WSDL_6.aws.Character c = new WSDL_6.aws.Character();
	c.setValue ("r");
	data.setD (c);

	data.setE ("pascal");
	data.setF (true);

	Rec res = port.echo_Rec (data);
	printlnRec (res);

	// New Rec
	New_Rec ndata = new New_Rec();
	ndata.setNC (Color.Blue);
	ndata.setNR (data);

	New_Rec nres = port.echo_New_Rec (ndata);

	System.out.println (nres.getNC());
	printlnRec (nres.getNR());
   }
}
