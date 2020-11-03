/*
                 __________                                      
    _____   __ __\______   \_____  _______  ______  ____ _______ 
   /     \ |  |  \|     ___/\__  \ \_  __ \/  ___/_/ __ \\_  __ \
  |  Y Y  \|  |  /|    |     / __ \_|  | \/\___ \ \  ___/ |  | \/
  |__|_|  /|____/ |____|    (____  /|__|  /____  > \___  >|__|   
        \/                       \/            \/      \/        
  Copyright (C) 2004-2011 Ingo Berg

  Permission is hereby granted, free of charge, to any person obtaining a copy of this 
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify, 
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
  permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or 
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*/ 
  
  
#ifndef MU_PARSER_CALLBACK_H
#define MU_PARSER_CALLBACK_H
  
#include "muParserDef.h"
  
/** \file
    \brief Definition of the parser callback class.
*/ 
  
{
  
/** \brief Encapsulation of prototypes for a numerical parser function.

    Encapsulates the prototyp for numerical parser functions. The class
    stores the number of arguments for parser functions as well
    as additional flags indication the function is non optimizeable.
    The pointer to the callback function pointer is stored as void* 
    and needs to be casted according to the argument count.
    Negative argument counts indicate a parser function with a variable number
    of arguments. 

    \author (C) 2004-2011 Ingo Berg
*/ 
    class ParserCallback 
  {
  
    
                     -1, ECmdCode a_iCode = cmFUNC);
    
                     EOprtAssociativity a_eAssociativity);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
  
    
    /** \brief Number of numeric function arguments
    
        This number is negative for functions with variable number of arguments. in this cases
        they represent the actual number of arguments found.
    */ 
    int m_iArgc;
    
    EOprtAssociativity m_eOprtAsct;     ///< Operator associativity; Valid only for binary operators 
    ECmdCode m_iCode;
    
    
  };
  
//------------------------------------------------------------------------------
/** \brief Container for Callback objects. */ 
  typedef std::map < string_type, ParserCallback > funmap_type;



#endif  /* 
  