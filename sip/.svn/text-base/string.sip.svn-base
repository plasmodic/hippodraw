// -*- mode:c++ -*-
// This is the SIP interface definition for std::string
//
// Copyright (C)  2002  The Board of Trustees of  
// The Leland Stanford Junior University.  All Rights Reserved.
//
// This file is part of sihippo
//
// $Id: string.sip,v 1.5 2005/05/09 23:42:29 panetta Exp $
//
// Author: Paul_Kunz@slac.stanford.edu
//

%MappedType std::string
{
%TypeHeaderCode
#include <string>
using std::string;
%End

%ConvertFromTypeCode
     const char *s = sipCpp->c_str();
     return PyString_FromString(s);
%End

%ConvertToTypeCode
         // Allow a Python string whenever a string is expected.
         if (sipIsErr == NULL)
             return PyString_Check(sipPy);
         if (sipPy == Py_None) {
             *sipCppPtr = NULL;
             return 0;
         }
         if (PyString_Check(sipPy)) {
             *sipCppPtr = new string(PyString_AS_STRING(sipPy));
             return 1;
         }
         *sipCppPtr = (string *)sipForceConvertTo_std_string(sipPy,sipIsErr);
         return 0;
%End

};
