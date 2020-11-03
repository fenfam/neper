/*
                 __________                                      
    _____   __ __\______   \_____  _______  ______  ____ _______ 
   /     \ |  |  \|     ___/\__  \ \_  __ \/  ___/_/ __ \\_  __ \
  |  Y Y  \|  |  /|    |     / __ \_|  | \/\___ \ \  ___/ |  | \/
  |__|_|  /|____/ |____|    (____  /|__|  /____  > \___  >|__|   
        \/                       \/            \/      \/        
  Copyright (C) 2011 Ingo Berg

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
  
  
#include "muParserBase.h"
#include "muParserTemplateMagic.h"
  
//--- Standard includes ------------------------------------------------------------------------
#include <cassert>
#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>
#include <deque>
#include <sstream>
#include <locale>
  
#ifdef MUP_USE_OPENMP
#include <omp.h>
#endif  /* 
  

/** \file
    \brief This file contains the basic implementation of the muparser engine.
*/ 
  
{
  
    std::locale (std::locale::classic (),
                 new change_dec_sep < char_type > ('.'));
  
  
  
    //------------------------------------------------------------------------------
  /** \brief Identifiers for built in binary operators. 

      When defining custom binary operators with #AddOprt(...) make sure not to choose 
      names conflicting with these definitions. 
  */ 
  const char_type *ParserBase::c_DefaultOprt[] = 
    { 

_T (":"), 0 
  };
  
    //------------------------------------------------------------------------------
  /** \brief Constructor.
      \param a_szFormula the formula to interpret.
      \throw ParserException if a_szFormula is null.
  */ 
ParserBase::ParserBase () 
  m_pParseFormula (&ParserBase::ParseString) 
  m_vStringBuf () 
  m_InfixOprtDef () 
  m_VarDef () 
  m_sInfixOprtChars () 
  m_nFinalResultIdx (0) 
  {
    
  
    //---------------------------------------------------------------------------
  /** \brief Copy constructor. 

    The parser can be safely copy constructed but the bytecode is reset during
    copy construction.
  */ 
  ParserBase::
    ParserBase (const ParserBase & a_Parser) 
                                                                ParseString)
    
    m_PostOprtDef () 
    m_StrVarDef () 
    m_sOprtChars () 
  {
    
    
  
    //---------------------------------------------------------------------------
    ParserBase::~ParserBase () 
  {
  } 
    //---------------------------------------------------------------------------
  /** \brief Assignment operator. 

    Implemented by calling Assign(a_Parser). Self assignment is suppressed.
    \param a_Parser Object to copy to this.
    \return *this
    \throw nothrow
  */ 
    ParserBase & ParserBase::operator= (const ParserBase & a_Parser) 
  {
    
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Copy state of a parser object to this. 

    Clears Variables and Functions of this parser.
    Copies the states of all internal variables.
    Resets parse function to string parse mode.

    \param a_Parser the source object.
  */ 
  void ParserBase::Assign (const ParserBase & a_Parser) 
  {
    
      
    
      // Don't copy bytecode instead cause the parser to create new bytecode
      // by resetting the parse function.
      ReInit ();
    
    m_VarDef = a_Parser.m_VarDef;       // Copy user defined variables
    m_bBuiltInOp = a_Parser.m_bBuiltInOp;
    
    
    
    
    
    
    
    
      // Copy function and operator callbacks
      m_FunDef = a_Parser.m_FunDef;     // Copy function definitions
    m_PostOprtDef = a_Parser.m_PostOprtDef;     // post value unary operators
    m_InfixOprtDef = a_Parser.m_InfixOprtDef;   // unary operators for infix notation
    m_OprtDef = a_Parser.m_OprtDef;     // binary operators
    
    
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Set the decimal separator.
      \param cDecSep Decimal separator as a character value.
      \sa SetThousandsSep

      By default muparser uses the "C" locale. The decimal separator of this
      locale is overwritten by the one provided here.
  */ 
  void ParserBase::SetDecSep (char_type cDecSep) 
  {
    
      std::use_facet < change_dec_sep < char_type >
      >(s_locale).thousands_sep ();
    
      std::locale (std::locale ("C"),
                   new change_dec_sep < char_type > (cDecSep, cThousandsSep));
  
    //---------------------------------------------------------------------------
  /** \brief Sets the thousands operator. 
      \param cThousandsSep The thousands separator as a character
      \sa SetDecSep

      By default muparser uses the "C" locale. The thousands separator of this
      locale is overwritten by the one provided here.
  */ 
  void ParserBase::SetThousandsSep (char_type cThousandsSep) 
  {
    
      std::use_facet < change_dec_sep < char_type >
      >(s_locale).decimal_point ();
    
      std::locale (std::locale ("C"),
                   new change_dec_sep < char_type > (cDecSep, cThousandsSep));
  
    //---------------------------------------------------------------------------
  /** \brief Resets the locale. 

    The default locale used "." as decimal separator, no thousands separator and
    "," as function argument separator.
  */ 
  void ParserBase::ResetLocale () 
  {
    
      std::locale (std::locale ("C"), new change_dec_sep < char_type > ('.'));
    
  
    //---------------------------------------------------------------------------
  /** \brief Initialize the token reader. 

    Create new token reader object and submit pointers to function, operator,
    constant and variable definitions.

    \post m_pTokenReader.get()!=0
    \throw nothrow
  */ 
  void ParserBase::InitTokenReader () 
  {
    
  
    //---------------------------------------------------------------------------
  /** \brief Reset parser to string parsing mode and clear internal buffers.

      Clear bytecode, reset the token reader.
      \throw nothrow
  */ 
  void ParserBase::ReInit () const 
  {
    
    
    
    
    
  
    //---------------------------------------------------------------------------
  void ParserBase::OnDetectVar (string_type * /*pExpr */ , int & /*nStart */ ,
                                int & /*nEnd */ ) 
  {
  } 
    //---------------------------------------------------------------------------
  /** \brief Returns the version of muparser. 
      \param eInfo A flag indicating whether the full version info should be 
                   returned or not.

    Format is as follows: "MAJOR.MINOR (COMPILER_FLAGS)" The COMPILER_FLAGS
    are returned only if eInfo==pviFULL.
  */ 
  string_type ParserBase::GetVersion (EParserVersionInfo eInfo) const 
  {
    
    
    
      
    {
      
      
       
#ifdef _DEBUG
        ss << _T ("; DEBUG");
       
#else   /* 
        ss << _T ("; RELEASE");
       
#endif  /* 
       
#ifdef _UNICODE
        ss << _T ("; UNICODE");
       
#else   /* 
#ifdef _MBCS
        ss << _T ("; MBCS");
       
#else   /* 
        ss << _T ("; ASCII");
       
#endif  /* 
#endif  /* 
       
#ifdef MUP_USE_OPENMP
        ss << _T ("; OPENMP");
       
//#else
//      ss << _T("; NO_OPENMP");
#endif  /* 
       
#if defined(MUP_MATH_EXCEPTIONS)
        ss << _T ("; MATHEXC");
       
//#else
//      ss << _T("; NO_MATHEXC");
#endif  /* 
       
     
     
  
  
    //---------------------------------------------------------------------------
  /** \brief Add a value parsing function. 
      
      When parsing an expression muParser tries to detect values in the expression
      string using different valident callbacks. Thus it's possible to parse
      for hex values, binary values and floating point values. 
  */ 
  void ParserBase::AddValIdent (identfun_type a_pCallback) 
  {
    
  
    //---------------------------------------------------------------------------
  /** \brief Set a function that can create variable pointer for unknown expression variables. 
      \param a_pFactory A pointer to the variable factory.
      \param pUserData A user defined context pointer.
  */ 
  void ParserBase::SetVarFactory (facfun_type a_pFactory, void *pUserData) 
  {
    
  
    //---------------------------------------------------------------------------
  /** \brief Add a function or operator callback to the parser. */ 
  void ParserBase::AddCallback (const string_type & a_strName,
                                
                                
                                
  {
    
      
    
    
      // Check for conflicting operator or function names
      if (pFunMap != &m_FunDef
          && m_FunDef.find (a_strName) != m_FunDef.end ())
      
    
           && m_PostOprtDef.find (a_strName) != m_PostOprtDef.end ())
      
    
           && m_InfixOprtDef.find (a_strName) != m_InfixOprtDef.end ())
      
    
           && m_OprtDef.find (a_strName) != m_OprtDef.end ())
      
    
    
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Check if a name contains invalid characters. 

      \throw ParserException if the name contains invalid characters.
  */ 
  void ParserBase::CheckOprt (const string_type & a_sName,
                              
                              
  {
    
         (a_sName.find_first_not_of (a_szCharSet) != string_type::npos) || 
         (a_sName[0] >= '0' && a_sName[0] <= '9'))
      
    {
      
        
      {
      
        Error (ecINVALID_POSTFIX_IDENT, -1, a_sName);
        
        
      
     
   
   
    //---------------------------------------------------------------------------
  /** \brief Check if a name contains invalid characters. 

      \throw ParserException if the name contains invalid characters.
  */ 
  void ParserBase::CheckName (const string_type & a_sName,
                              
  {
    
         (a_sName.find_first_not_of (a_szCharSet) != string_type::npos) || 
         (a_sName[0] >= '0' && a_sName[0] <= '9'))
      
    {
      
    
   
   
    //---------------------------------------------------------------------------
  /** \brief Set the formula. 
      \param a_strFormula Formula as string_type
      \throw ParserException in case of syntax errors.

      Triggers first time calculation thus the creation of the bytecode and
      scanning of used variables.
  */ 
  void ParserBase::SetExpr (const string_type & a_sExpr) 
  {
    
      // Check locale compatibility
      std::locale loc;
    
         char_type > >(loc).decimal_point ())
      
    
      // <ibg> 20060222: Bugfix for Borland-Kylix:
      // adding a space to the expression will keep Borlands KYLIX from going wild
      // when calling tellg on a stringstream created from the expression after 
      // reading a value at the end of an expression. (mu::Parser::IsVal function)
      // (tellg returns -1 otherwise causing the parser to ignore the value)
      string_type sBuf (a_sExpr + _T (" "));
    
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Get the default symbols used for the built in operators. 
      \sa c_DefaultOprt
  */ 
  const char_type **ParserBase::GetOprtDef () const 
  {
    
  
    //---------------------------------------------------------------------------
  /** \brief Define the set of valid characters to be used in names of
             functions, variables, constants.
  */ 
  void ParserBase::DefineNameChars (const char_type * a_szCharset) 
  {
    
  
    //---------------------------------------------------------------------------
  /** \brief Define the set of valid characters to be used in names of
             binary operators and postfix operators.
  */ 
  void ParserBase::DefineOprtChars (const char_type * a_szCharset) 
  {
    
  
    //---------------------------------------------------------------------------
  /** \brief Define the set of valid characters to be used in names of
             infix operators.
  */ 
  void ParserBase::DefineInfixOprtChars (const char_type * a_szCharset) 
  {
    
  
    //---------------------------------------------------------------------------
  /** \brief Virtual function that defines the characters allowed in name identifiers. 
      \sa #ValidOprtChars, #ValidPrefixOprtChars
  */ 
  const char_type *ParserBase::ValidNameChars () const 
  {
    
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Virtual function that defines the characters allowed in operator definitions. 
      \sa #ValidNameChars, #ValidPrefixOprtChars
  */ 
  const char_type *ParserBase::ValidOprtChars () const 
  {
    
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Virtual function that defines the characters allowed in infix operator definitions.
      \sa #ValidNameChars, #ValidOprtChars
  */ 
  const char_type *ParserBase::ValidInfixOprtChars () const 
  {
    
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Add a user defined operator. 
      \post Will reset the Parser to string parsing mode.
  */ 
  void ParserBase::DefinePostfixOprt (const string_type & a_sName,
                                      
  {
    
                  
                                   cmOPRT_POSTFIX), 
                  
  
    //---------------------------------------------------------------------------
  /** \brief Initialize user defined functions. 
   
    Calls the virtual functions InitFun(), InitConst() and InitOprt().
  */ 
  void ParserBase::Init () 
  {
    
    
    
    
  
    //---------------------------------------------------------------------------
  /** \brief Add a user defined operator. 
      \post Will reset the Parser to string parsing mode.
      \param [in] a_sName  operator Identifier 
      \param [in] a_pFun  Operator callback function
      \param [in] a_iPrec  Operator Precedence (default=prSIGN)
      \param [in] a_bAllowOpt  True if operator is volatile (default=false)
      \sa EPrec
  */ 
  void ParserBase::DefineInfixOprt (const string_type & a_sName,
                                    
                                    
  {
    
                  
                                   cmOPRT_INFIX), 
                  
  
    //---------------------------------------------------------------------------
  /** \brief Define a binary operator. 
      \param [in] a_sName The identifier of the operator.
      \param [in] a_pFun Pointer to the callback function.
      \param [in] a_iPrec Precedence of the operator.
      \param [in] a_eAssociativity The associativity of the operator.
      \param [in] a_bAllowOpt If this is true the operator may be optimized away.
      
      Adds a new Binary operator the the parser instance. 
  */ 
  void ParserBase::DefineOprt (const string_type & a_sName, 
                               
                               
                               
  {
    
      // Check for conflicts with built in operator names
      for (int i = 0; m_bBuiltInOp && i < cmENDIF; ++i)
      
        
    
                    
                                     a_eAssociativity), 
                    
  
  
    //---------------------------------------------------------------------------
  /** \brief Define a new string constant.
      \param [in] a_strName The name of the constant.
      \param [in] a_strVal the value of the constant. 
  */ 
  void ParserBase::DefineStrConst (const string_type & a_strName,
                                   const string_type & a_strVal) 
  {
    
      // Test if a constant with that names already exists
      if (m_StrVarDef.find (a_strName) != m_StrVarDef.end ())
      
    
    
    m_StrVarDef[a_strName] = m_vStringVarBuf.size () - 1;       // bind buffer index to variable name
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Add a user defined variable. 
      \param [in] a_sName the variable name
      \param [in] a_pVar A pointer to the variable value.
      \post Will reset the Parser to string parsing mode.
      \throw ParserException in case the name contains invalid signs or a_pVar is NULL.
  */ 
  void ParserBase::DefineVar (const string_type & a_sName,
                              value_type * a_pVar) 
  {
    
      
    
      // Test if a constant with that names already exists
      if (m_ConstDef.find (a_sName) != m_ConstDef.end ())
      
    
    
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Add a user defined constant. 
      \param [in] a_sName The name of the constant.
      \param [in] a_fVal the value of the constant.
      \post Will reset the Parser to string parsing mode.
      \throw ParserException in case the name contains invalid signs.
  */ 
  void ParserBase::DefineConst (const string_type & a_sName,
                                value_type a_fVal) 
  {
    
    
    
  
    //---------------------------------------------------------------------------
  /** \brief Get operator priority.
      \throw ParserException if a_Oprt is no operator code
  */ 
  int ParserBase::GetOprtPrecedence (const token_type & a_Tok) const 
  {
    
      
    {
      
        // built in operators
    case cmEND:
      return -5;
      
      
      
      
      
      
        cmEQ:return prCMP;
      
      
      
      
        // user defined binary operators
      case cmOPRT_INFIX:
      
      
    
   
   
    //---------------------------------------------------------------------------
  /** \brief Get operator priority.
      \throw ParserException if a_Oprt is no operator code
  */ 
   
    EOprtAssociativity ParserBase::
    GetOprtAssociativity (const token_type & a_Tok) const 
  {
    
      
    {
    
    
    
    
    
    
    
    
    
    
    
    
    
      return oaLEFT;
      
      
      
    
   
   
    //---------------------------------------------------------------------------
  /** \brief Return a map containing the used variables only. */ 
  const varmap_type & ParserBase::GetUsedVar () const 
  {
    
    {
      
      
      // may contain references to nonexisting variables.
      m_pParseFormula = &ParserBase::ParseString;
      
    
    {
      
        // Make sure to stay in string parse mode, dont call ReInit()
        // because it deletes the array with the used variables
        m_pParseFormula = &ParserBase::ParseString;
      
      
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Return a map containing the used variables only. */ 
  const varmap_type & ParserBase::GetVar () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Return a map containing all parser constants. */ 
  const valmap_type & ParserBase::GetConst () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Return prototypes of all parser functions.
      \return #m_FunDef
      \sa FunProt
      \throw nothrow
      
      The return type is a map of the public type #funmap_type containing the prototype
      definitions for all numerical parser functions. String functions are not part of 
      this map. The Prototype definition is encapsulated in objects of the class FunProt
      one per parser function each associated with function names via a map construct.
  */ 
  const funmap_type & ParserBase::GetFunDef () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Retrieve the formula. */ 
  const string_type & ParserBase::GetExpr () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Execute a function that takes a single string argument.
      \param a_FunTok Function token.
      \throw exception_type If the function token is not a string function
  */ 
   
    ParserBase::token_type ParserBase::
    ApplyStrFunc (const token_type & a_FunTok,
                  
  {
    
      
              a_FunTok.GetAsString ());
    
    
    
    
    {
      
        // Check function arguments; write dummy value into valtok to represent the result
        switch (a_FunTok.GetArgCount ())
        
      {
      
        valTok.SetVal (1);
        a_vArg[0].GetAsString ();
        break;
        
        a_vArg[1].GetAsString ();
        a_vArg[0].GetVal ();
        break;
        
        a_vArg[2].GetAsString ();
        a_vArg[1].GetVal ();
        a_vArg[0].GetVal ();
        break;
        
      
     
     
    {
      
              a_FunTok.GetAsString ());
    
    
      // string functions won't be optimized
      m_vRPN.AddStrFun (pFunc, a_FunTok.GetArgCount (),
                        a_vArg.back ().GetIdx ());
    
      // Push dummy value representing the function result to the stack
      return valTok;
  
  
    //---------------------------------------------------------------------------
  /** \brief Apply a function token. 
      \param iArgCount Number of Arguments actually gathered used only for multiarg functions.
      \post The result is pushed to the value stack
      \post The function token is removed from the stack
      \throw exception_type if Argument count does not match function requirements.
  */ 
  void ParserBase::ApplyFunc (ParserStack < token_type > &a_stOpt,
                              
                              
  {
    
    
      // Operator stack empty or does not contain tokens with callback functions
    if (a_stOpt.empty () || a_stOpt.top ().GetFuncAddr () == 0)
      
    
    
    
      // Binary operators must rely on their internal operator number
      // since counting of operators relies on commas for function arguments
      // binary operators do not have commas in their expression
    int iArgCount =
      (funTok.GetCode () == cmOPRT_BIN) ? funTok.GetArgCount () : a_iArgCount;
     
      // determine how many parameters the function needs. To remember iArgCount includes the 
      // string parameter whilst GetArgCount() counts only numeric parameters.
    int iArgRequired =
      funTok.GetArgCount () + ((funTok.GetType () == tpSTR) ? 1 : 0);
     
      // Thats the number of numerical parameters
    int iArgNumerical = iArgCount - ((funTok.GetType () == tpSTR) ? 1 : 0);
     
      
     
       
               funTok.GetAsString ());
     
       
               funTok.GetAsString ());
     
      
              funTok.GetAsString ());
     
      // Collect the numeric function arguments from the value stack and store them
      // in a vector
      std::vector < token_type > stArg;
     
       
    {
      
      
        
                funTok.GetAsString ());
    
     
      
    {
    
      
      
        
                funTok.GetAsString ());
      
      
    
      
      
    
    
    
    
      
        
                funTok.GetAsString ());
      
                        (funTok.GetArgCount () ==
                         -1) ? -iArgNumerical : iArgNumerical);
      
    
    
      // Push dummy value representing the function result to the stack
      token_type token;
    
    
  
  
    //---------------------------------------------------------------------------
  void ParserBase::ApplyIfElse (ParserStack < token_type > &a_stOpt,
                                
  {
    
      // Check if there is an if Else clause to be calculated
    while (a_stOpt.size () && a_stOpt.top ().GetCode () == cmELSE)
      
    {
      
      
      
        // Take the value associated with the else branch from the value stack
      token_type vVal2 = a_stVal.pop ();
      
      
      
        // it then else is a ternary operator Pop all three values from the value s
        // tack and just return the right value
      token_type vVal1 = a_stVal.pop ();
      
      
      
      
      
      
    
  }
   
    //---------------------------------------------------------------------------
  /** \brief Performs the necessary steps to write code for
             the execution of binary operators into the bytecode. 
  */ 
  void ParserBase::ApplyBinOprt (ParserStack < token_type > &a_stOpt,
                                 
  {
    
      // is it a user defined binary operator?
    if (a_stOpt.top ().GetCode () == cmOPRT_BIN)
      
    {
      
    
     
    else
       
    {
      
      
        a_stVal.pop (), 
      
             || 
        
                optTok.GetAsString ());
      
        
      {
        
          
        
      
      
      else
        
      
      
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Apply a binary operator. 
      \param a_stOpt The operator stack
      \param a_stVal The value stack
  */ 
  void ParserBase::ApplyRemainingOprt (ParserStack < token_type > &stOpt,
                                       
                                       &stVal) const 
  {
    
            && 
      
    {
      
      
        
      {
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
        
          
        
        else
          
        
        
        
        
      
     
   
   
    //---------------------------------------------------------------------------
  /** \brief Parse the command code.
      \sa ParseString(...)

      Command code contains precalculated stack positions of the values and the
      associated operators. The Stack is filled beginning from index one the 
      value at index zero is not used at all.
  */ 
    value_type ParserBase::ParseCmdCode () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Evaluate the RPN. 
      \param nOffset The offset added to variable addresses (for bulk mode)
      \param nThreadID OpenMP Thread id of the calling thread
  */ 
    value_type ParserBase::ParseCmdCodeBulk (int nOffset, int nThreadID) const 
  {
    
    
      // Note: The check for nOffset==0 and nThreadID here is not necessary but 
      //       brings a minor performance gain when not in bulk mode.
    value_type * Stack = ((nOffset == 0)
                          && (nThreadID ==
                              0)) ? &m_vStackBuffer[0] :
      &m_vStackBuffer[nThreadID *
                      (m_vStackBuffer.size () / s_MaxNumOpenMPThreads)];
    
    
     
           ++pTok)
       
    {
      
        
      {
        
          // built in binary operators
      case cmLE:
        --sidx;
        Stack[sidx] = Stack[sidx] <= Stack[sidx + 1];
        continue;
        
        Stack[sidx] = Stack[sidx] >= Stack[sidx + 1];
        continue;
        
        Stack[sidx] = Stack[sidx] != Stack[sidx + 1];
        continue;
        
        Stack[sidx] = Stack[sidx] == Stack[sidx + 1];
        continue;
        
        Stack[sidx] = Stack[sidx] < Stack[sidx + 1];
        continue;
        
        Stack[sidx] = Stack[sidx] > Stack[sidx + 1];
        continue;
        
        Stack[sidx] += Stack[1 + sidx];
        continue;
        
        Stack[sidx] -= Stack[1 + sidx];
        continue;
        
        Stack[sidx] *= Stack[1 + sidx];
        continue;
        
        
#if defined(MUP_MATH_EXCEPTIONS)
        if (Stack[1 + sidx] == 0)
          
        
#endif  /* 
        Stack[sidx] /= Stack[1 + sidx];
        
        
        Stack[sidx] =
          MathImpl < value_type >::Pow (Stack[sidx], Stack[1 + sidx]);
        
        
        Stack[sidx] = Stack[sidx] && Stack[sidx + 1];
        continue;
        
        Stack[sidx] = Stack[sidx] || Stack[sidx + 1];
        continue;
        
          // Bugfix for Bulkmode:
          // for details see:
          //    https://groups.google.com/forum/embed/?place=forum/muparser-dev&showsearch=true&showpopout=true&showtabs=false&parenturl=http://muparser.beltoforion.de/mup_forum.html&afterlogin&pli=1#!topic/muparser-dev/szgatgoHTws
        --sidx;
        Stack[sidx] = *(pTok->Oprt.ptr + nOffset) = Stack[sidx + 1];
        continue;
        
          // original code:
          //--sidx; Stack[sidx] = *pTok->Oprt.ptr = Stack[sidx+1]; continue;
        
          //case  cmBO:  // unused, listed for compiler optimization purposes
          //case  cmBC:
          //      MUP_FAIL(INVALID_CODE_IN_BYTECODE);
          //      continue;
        
          
        
        
        
        
        
          //case  cmARG_SEP:
          //      MUP_FAIL(INVALID_CODE_IN_BYTECODE);
          //      continue;
        
          // value and variable tokens
        case cmVAR:Stack[++sidx] = *(pTok->Val.ptr + nOffset);
        continue;
        
        continue;
        
        
        
        
        
        
        
        
        
        
          *(pTok->Val.ptr + nOffset) * pTok->Val.data + pTok->Val.data2;
        
        
          // Next is treatment of numeric functions
        case cmFUNC:
        {
          
           
            // switch according to argument count
            switch (iArgCount)
            
          {
          
            sidx += 1;
            Stack[sidx] = (*(fun_type0) pTok->Fun.ptr) ();
            continue;
            
            continue;
            
            Stack[sidx] =
              (*(fun_type2) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1]);
            continue;
            
            Stack[sidx] =
              (*(fun_type3) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1],
                                            Stack[sidx + 2]);
            continue;
            
            Stack[sidx] =
              (*(fun_type4) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1],
                                            Stack[sidx + 2], Stack[sidx + 3]);
            continue;
            
            Stack[sidx] =
              (*(fun_type5) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1],
                                            Stack[sidx + 2], Stack[sidx + 3],
                                            Stack[sidx + 4]);
            continue;
            
            Stack[sidx] =
              (*(fun_type6) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1],
                                            Stack[sidx + 2], Stack[sidx + 3],
                                            Stack[sidx + 4], Stack[sidx + 5]);
            continue;
            
            Stack[sidx] =
              (*(fun_type7) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1],
                                            Stack[sidx + 2], Stack[sidx + 3],
                                            Stack[sidx + 4], Stack[sidx + 5],
                                            Stack[sidx + 6]);
            continue;
            
            Stack[sidx] =
              (*(fun_type8) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1],
                                            Stack[sidx + 2], Stack[sidx + 3],
                                            Stack[sidx + 4], Stack[sidx + 5],
                                            Stack[sidx + 6], Stack[sidx + 7]);
            continue;
            
            Stack[sidx] =
              (*(fun_type9) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1],
                                            Stack[sidx + 2], Stack[sidx + 3],
                                            Stack[sidx + 4], Stack[sidx + 5],
                                            Stack[sidx + 6], Stack[sidx + 7],
                                            Stack[sidx + 8]);
            continue;
            
            Stack[sidx] =
              (*(fun_type10) pTok->Fun.ptr) (Stack[sidx], Stack[sidx + 1],
                                             Stack[sidx + 2], Stack[sidx + 3],
                                             Stack[sidx + 4], Stack[sidx + 5],
                                             Stack[sidx + 6], Stack[sidx + 7],
                                             Stack[sidx + 8],
                                             Stack[sidx + 9]);
            continue;
            
              Error (ecINTERNAL_ERROR, 1);
            
            
              (*(multfun_type) pTok->Fun.ptr) (&Stack[sidx], -iArgCount);
            
          
         
         
          // Next is treatment of string functions
        case cmFUNC_STR:
        {
          
          
            // The index of the string argument in the string table
          int iIdxStack = pTok->Fun.idx;
          
                       && iIdxStack < (int) m_vStringBuf.size ());
          
          {
          
            Stack[sidx] =
              (*(strfun_type1) pTok->Fun.ptr) (m_vStringBuf[iIdxStack].
                                               c_str ());
            continue;
          
            Stack[sidx] =
              (*(strfun_type2) pTok->Fun.ptr) (m_vStringBuf[iIdxStack].
                                               c_str (), Stack[sidx]);
            continue;
          
            Stack[sidx] =
              (*(strfun_type3) pTok->Fun.ptr) (m_vStringBuf[iIdxStack].
                                               c_str (), Stack[sidx],
                                               Stack[sidx + 1]);
            continue;
          
          
        
      
        
        {
          
          
            // switch according to argument count
            switch (iArgCount)
            
          {
          
            sidx += 1;
            Stack[sidx] =
              (*(bulkfun_type0) pTok->Fun.ptr) (nOffset, nThreadID);
            continue;
          
            Stack[sidx] =
              (*(bulkfun_type1) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx]);
            continue;
          
            sidx -= 1;
            Stack[sidx] =
              (*(bulkfun_type2) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx], Stack[sidx + 1]);
            continue;
          
            sidx -= 2;
            Stack[sidx] =
              (*(bulkfun_type3) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx], Stack[sidx + 1],
                                                Stack[sidx + 2]);
            continue;
          
            sidx -= 3;
            Stack[sidx] =
              (*(bulkfun_type4) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx], Stack[sidx + 1],
                                                Stack[sidx + 2],
                                                Stack[sidx + 3]);
            continue;
          
            sidx -= 4;
            Stack[sidx] =
              (*(bulkfun_type5) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx], Stack[sidx + 1],
                                                Stack[sidx + 2],
                                                Stack[sidx + 3],
                                                Stack[sidx + 4]);
            continue;
          
            sidx -= 5;
            Stack[sidx] =
              (*(bulkfun_type6) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx], Stack[sidx + 1],
                                                Stack[sidx + 2],
                                                Stack[sidx + 3],
                                                Stack[sidx + 4],
                                                Stack[sidx + 5]);
            continue;
          
            sidx -= 6;
            Stack[sidx] =
              (*(bulkfun_type7) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx], Stack[sidx + 1],
                                                Stack[sidx + 2],
                                                Stack[sidx + 3],
                                                Stack[sidx + 4],
                                                Stack[sidx + 5],
                                                Stack[sidx + 6]);
            continue;
          
            sidx -= 7;
            Stack[sidx] =
              (*(bulkfun_type8) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx], Stack[sidx + 1],
                                                Stack[sidx + 2],
                                                Stack[sidx + 3],
                                                Stack[sidx + 4],
                                                Stack[sidx + 5],
                                                Stack[sidx + 6],
                                                Stack[sidx + 7]);
            continue;
          
            sidx -= 8;
            Stack[sidx] =
              (*(bulkfun_type9) pTok->Fun.ptr) (nOffset, nThreadID,
                                                Stack[sidx], Stack[sidx + 1],
                                                Stack[sidx + 2],
                                                Stack[sidx + 3],
                                                Stack[sidx + 4],
                                                Stack[sidx + 5],
                                                Stack[sidx + 6],
                                                Stack[sidx + 7],
                                                Stack[sidx + 8]);
            continue;
          
            sidx -= 9;
            Stack[sidx] =
              (*(bulkfun_type10) pTok->Fun.ptr) (nOffset, nThreadID,
                                                 Stack[sidx], Stack[sidx + 1],
                                                 Stack[sidx + 2],
                                                 Stack[sidx + 3],
                                                 Stack[sidx + 4],
                                                 Stack[sidx + 5],
                                                 Stack[sidx + 6],
                                                 Stack[sidx + 7],
                                                 Stack[sidx + 8],
                                                 Stack[sidx + 9]);
            continue;
          
            
            
          
        
      
        
        
      
    }                           // for all bytecode tokens
    
  
  
    //---------------------------------------------------------------------------
  void ParserBase::CreateRPN () const 
  {
    
      
    
    
     
    token_type val, tval;       // for storing value
     
     
      // The outermost counter counts the number of separated items
      // such as in "a=10,b=20,c=c+a"
      stArgCount.push (1);
     
       
    {
      
      
        
      {
        
          //
          // Next three are different kind of value entries
          //
      case cmSTRING:
        
        stVal.push (opt);
        
        break;
        
        
        
        
        
        
        
        
          
        
        
        
        
        
          
        
+stArgCount.top ();
        
          // fallthrough intentional (no break!)
        
        
        
        {
          
            // The argument count for parameterless functions is zero
            // by default an opening bracket sets parameter count to 1
            // in preparation of arguments to come. If the last token
            // was an opening bracket we know better...
            if (opta.GetCode () == cmBO)
            
          
          
            // Check if the bracket content has been evaluated completely
          if (stOpt.size () && stOpt.top ().GetCode () == cmBO)
            
          {
            
              // if opt is ")" and opta is "(" the bracket has been evaluated, now its time to check
              // if there is either a function or a sign pending
              // neither the opening nor the closing bracket will be pushed back to
              // the operator stack
              // Check if a function is standing in front of the opening bracket, 
              // if yes evaluate it afterwards check for infix operators
              assert (stArgCount.size ());
            
             
             
                                    (stOpt.top ().GetCode () != cmFUNC
                                     && 
                                     cmFUNC_BULK
                                     && 
                                     cmFUNC_STR)))
              
             
              // The opening bracket was popped from the stack now check if there
              // was a function before this bracket
            if (stOpt.size () && 
                && 
                && 
               
            {
              
            
           
         
        break;
        
          //
          // Next are the binary operator entries
          //
          //case cmAND:   // built in binary operators
          //case cmOR:
          //case cmXOR:
      case cmIF:
        
        
          // fallthrough intentional (no break!)
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
        
          // A binary operator (user defined or built in) has been found. 
          while (stOpt.size () && 
                 && 
                 && 
          
        {
          
            GetOprtPrecedence (opt);
          
            
          {
            
              // Deal with operator associativity
              EOprtAssociativity eOprtAsct = GetOprtAssociativity (opt);
            
                 (eOprtAsct == oaLEFT && (nPrec1 < nPrec2)))
              
            {
              
            
          
          
          else if (nPrec1 < nPrec2)
            
          {
            
              // In case the operators are not equal the precedence decides alone...
              break;
          
          
            
          
          else
            
        
        
          
        
          // The operator can't be evaluated right now, push back to the operator stack
          stOpt.push (opt);
        
        
          //
          // Last section contains functions and operators implicitly mapped to functions
          //
      case cmBO:
        
        
        
      
      
      
      
        
        
      
        
        
        break;
      
        Error (ecINTERNAL_ERROR, 3);
      
      
      
        
      {
        
        
      
      
        
      {
        
        
      
    
    
      
    
      
    
      // get the last value (= final result) from the stack
      MUP_ASSERT (stArgCount.size () == 1);
    
    
      
    
      
    
      
    
                              s_MaxNumOpenMPThreads);
  
  
    //---------------------------------------------------------------------------
  /** \brief One of the two main parse functions.
      \sa ParseCmdCode(...)

    Parse expression from input string. Perform syntax checking and create 
    bytecode. After parsing the string and creating the bytecode the function 
    pointer #m_pParseFormula will be changed to the second parse routine the 
    uses bytecode instead of string parsing.
  */ 
  value_type ParserBase::ParseString ()const 
  {
    
    {
      
      
      
    
     
    {
      
      
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Create an error containing the parse error position.

    This function will create an Parser Exception object containing the error text and
    its position.

    \param a_iErrc [in] The error code of type #EErrorCodes.
    \param a_iPos [in] The position where the error was detected.
    \param a_strTok [in] The token string representation associated with the error.
    \throw ParserException always throws thats the only purpose of this function.
  */ 
  void ParserBase::Error (EErrorCodes a_iErrc, int a_iPos,
                          const string_type & a_sTok) const 
  {
    
                           a_iPos);
  
    //------------------------------------------------------------------------------
  /** \brief Clear all user defined variables.
      \throw nothrow

      Resets the parser to string parsing mode by calling #ReInit.
  */ 
  void ParserBase::ClearVar () 
  {
    
    
  
    //------------------------------------------------------------------------------
  /** \brief Remove a variable from internal storage.
      \throw nothrow

      Removes a variable if it exists. If the Variable does not exist nothing will be done.
  */ 
  void ParserBase::RemoveVar (const string_type & a_strVarName) 
  {
    
    
      
    {
      
      
    
  
  
    //------------------------------------------------------------------------------
  /** \brief Clear all functions.
      \post Resets the parser to string parsing mode.
      \throw nothrow
  */ 
  void ParserBase::ClearFun () 
  {
    
    
  
    //------------------------------------------------------------------------------
  /** \brief Clear all user defined constants.

      Both numeric and string constants will be removed from the internal storage.
      \post Resets the parser to string parsing mode.
      \throw nothrow
  */ 
  void ParserBase::ClearConst () 
  {
    
    
    
  
    //------------------------------------------------------------------------------
  /** \brief Clear all user defined postfix operators.
      \post Resets the parser to string parsing mode.
      \throw nothrow
  */ 
  void ParserBase::ClearPostfixOprt () 
  {
    
    
  
    //------------------------------------------------------------------------------
  /** \brief Clear all user defined binary operators.
      \post Resets the parser to string parsing mode.
      \throw nothrow
  */ 
  void ParserBase::ClearOprt () 
  {
    
    
  
    //------------------------------------------------------------------------------
  /** \brief Clear the user defined Prefix operators. 
      \post Resets the parser to string parser mode.
      \throw nothrow
  */ 
  void ParserBase::ClearInfixOprt () 
  {
    
    
  
    //------------------------------------------------------------------------------
  /** \brief Enable or disable the formula optimization feature. 
      \post Resets the parser to string parser mode.
      \throw nothrow
  */ 
  void ParserBase::EnableOptimizer (bool a_bIsOn) 
  {
    
    
  
    //---------------------------------------------------------------------------
  /** \brief Enable the dumping of bytecode and stack content on the console. 
      \param bDumpCmd Flag to enable dumping of the current bytecode to the console.
      \param bDumpStack Flag to enable dumping of the stack content is written to the console.

     This function is for debug purposes only!
  */ 
  void ParserBase::EnableDebugDump (bool bDumpCmd, bool bDumpStack) 
  {
    
    
  
    //------------------------------------------------------------------------------
  /** \brief Enable or disable the built in binary operators.
      \throw nothrow
      \sa m_bBuiltInOp, ReInit()

    If you disable the built in binary operators there will be no binary operators
    defined. Thus you must add them manually one by one. It is not possible to
    disable built in operators selectively. This function will Reinitialize the
    parser by calling ReInit().
  */ 
  void ParserBase::EnableBuiltInOprt (bool a_bIsOn) 
  {
    
    
  
    //------------------------------------------------------------------------------
  /** \brief Query status of built in variables.
      \return #m_bBuiltInOp; true if built in operators are enabled.
      \throw nothrow
  */ 
  bool ParserBase::HasBuiltInOprt ()const 
  {
    
  
   
    //------------------------------------------------------------------------------
  /** \brief Get the argument separator character. 
  */ 
    char_type ParserBase::GetArgSep () const 
  {
    
  
   
    //------------------------------------------------------------------------------
  /** \brief Set argument separator. 
      \param cArgSep the argument separator character.
  */ 
  void ParserBase::SetArgSep (char_type cArgSep) 
  {
    
  
    //------------------------------------------------------------------------------
  /** \brief Dump stack content. 

      This function is used for debugging only.
  */ 
  void ParserBase::StackDump (const ParserStack < token_type > &a_stVal,
                              
                              &a_stOprt) const 
  {
    
    
    
      
    {
      
      
        
      
      else
        
    
     
    
      
    {
      
        
      {
        
          <<ParserBase::c_DefaultOprt[stOprt.top ().GetCode ()] 
          <<_T ("\" \n");
      
      
      else
        
      {
        
          
        {
        
          mu::console () << _T ("VAR\n");
          break;
        
          mu::console () << _T ("VAL\n");
          break;
        
          mu::console () << _T ("FUNC \"") 
            <<stOprt.top ().GetAsString () 
          break;
        
          mu::console () << _T ("FUNC_BULK \"") 
            <<stOprt.top ().GetAsString () 
          break;
        
          mu::console () << _T ("OPRT_INFIX \"") 
            <<stOprt.top ().GetAsString () 
          break;
        
          mu::console () << _T ("OPRT_BIN \"") 
            <<stOprt.top ().GetAsString () 
          break;
        
          mu::console () << _T ("FUNC_STR\n");
          break;
        
          mu::console () << _T ("END\n");
          break;
        
          mu::console () << _T ("UNKNOWN\n");
          break;
        
          mu::console () << _T ("BRACKET \"(\"\n");
          break;
        
          mu::console () << _T ("BRACKET \")\"\n");
          break;
        
          mu::console () << _T ("IF\n");
          break;
        
          mu::console () << _T ("ELSE\n");
          break;
        
          mu::console () << _T ("ENDIF\n");
          break;
        
          mu::console () << stOprt.top ().GetCode () << _T (" ");
          break;
        
      
      
    
    
  
  
    //------------------------------------------------------------------------------
  /** \brief Evaluate an expression containing comma separated subexpressions 
      \param [out] nStackSize The total number of results available
      \return Pointer to the array containing all expression results

      This member function can be used to retrieve all results of an expression
      made up of multiple comma separated subexpressions (i.e. "x+y,sin(x),cos(y)")
  */ 
  value_type * ParserBase::Eval (int &nStackSize) const 
  {
    
    
    
      // (for historic reasons the stack starts at position 1)
    return &m_vStackBuffer[1];
  
   
    //---------------------------------------------------------------------------
  /** \brief Return the number of results on the calculation stack. 
  
    If the expression contains comma separated subexpressions (i.e. "sin(y), x+y"). 
    There may be more than one return value. This function returns the number of 
    available results.
  */ 
  int ParserBase::GetNumResults () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Calculate the result.

    A note on const correctness: 
    I consider it important that Calc is a const function.
    Due to caching operations Calc changes only the state of internal variables with one exception
    m_UsedVar this is reset during string parsing and accessible from the outside. Instead of making
    Calc non const GetUsedVar is non const because it explicitly calls Eval() forcing this update. 

    \pre A formula must be set.
    \pre Variables must have been set (if needed)

    \sa #m_pParseFormula
    \return The evaluation result
    \throw ParseException if no Formula is set or in case of any other error related to the formula.
  */ 
    value_type ParserBase::Eval () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  void ParserBase::Eval (value_type * results, int nBulkSize) 
  {
    
/* <ibg 2014-09-24/> Commented because it is making a unit test impossible

    // Parallelization does not make sense for fewer than 10000 computations 
    // due to thread creation overhead. If the bulk size is below 2000
    // computation is refused. 
    if (nBulkSize<2000)
    {
      throw ParserError(ecUNREASONABLE_NUMBER_OF_COMPUTATIONS);
    }
*/ 
      CreateRPN ();
    
    
#ifdef MUP_USE_OPENMP
//#define DEBUG_OMP_STUFF
#ifdef DEBUG_OMP_STUFF
    int *pThread = new int[nBulkSize];
    
    
#endif  /* 
    
      std::min (omp_get_max_threads (), s_MaxNumOpenMPThreads);
    
    
    
#pragma omp parallel for schedule(static, nBulkSize/nMaxThreads) private(nThreadID)
      for (i = 0; i < nBulkSize; ++i)
      
    {
      
      
      
#ifdef DEBUG_OMP_STUFF
#pragma omp critical
      {
        
        
        
      
      
#endif  /* 
    }
    
#ifdef DEBUG_OMP_STUFF
      FILE * pFile = fopen ("bulk_dbg.txt", "w");
    
      
    {
      
    
    
    
    
    
#endif  /* 
      
#else   /* 
      for (i = 0; i < nBulkSize; ++i)
      
    {
      
    
    
#endif  /* 
  
