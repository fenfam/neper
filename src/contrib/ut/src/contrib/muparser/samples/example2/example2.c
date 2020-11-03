#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "muParserDLL.h"
  
#define PARSER_CONST_PI  3.141592653589793238462643
#define PARSER_CONST_E   2.718281828459045235360287
#define PARSER_MAXVARS		10
  
#ifndef _UNICODE
#define _T(x) x
#define myprintf printf
#define mystrlen strlen
#define myfgets fgets
#define mystrcmp strcmp
#else   /* 
#define _T(x) L ##x
#define myprintf wprintf
#define mystrlen wcslen
#define myfgets fgetws
#define mystrcmp wcscmp
#endif  /* 


//---------------------------------------------------------------------------
// Callbacks for postfix operators
  muFloat_t Mega (muFloat_t a_fVal) 
{
  



{
  



{
  
  



{
  
  myprintf (_T ("%d,%2.2f\n"), nBulkIdx, v1);
  



//---------------------------------------------------------------------------
// Callbacks for infix operators
  muFloat_t Not (muFloat_t v)
{
  return v == 0;
}


//---------------------------------------------------------------------------
// Function callbacks
  muFloat_t Rnd (muFloat_t v)
{
  return v * rand () / (muFloat_t) (RAND_MAX + 1.0);
}


{
  
    
  {
    
  
  



{
  
  
  
    
  



//---------------------------------------------------------------------------
// Binarty operator callbacks
  muFloat_t Add (muFloat_t v1, muFloat_t v2) 
{
  



{
  



//---------------------------------------------------------------------------
// Factory function for creating new parser variables
// This could as well be a function performing database queries.
  muFloat_t * AddVariable (const muChar_t * a_szName, void *pUserData) 
{
  
  static int iVal = 0;          // so i used this buffer
  
             ("Generating new variable \"%s\" (slots left: %d; context pointer: 0x%x)\n"),
             a_szName, PARSER_MAXVARS - iVal, (int) pUserData);
  
  
    
  {
    
    
  
  



//---------------------------------------------------------------------------
  void
Intro (muParserHandle_t hParser) 
{
  
             ("                 __________                                       \n"));
  
             ("    _____   __ __\\______   \\_____  _______  ______  ____ _______\n"));
  
             ("   /     \\ |  |  \\|     ___/\\__  \\ \\_  __ \\/  ___/_/ __ \\\\_  __ \\ \n"));
  
             ("  |  Y Y  \\|  |  /|    |     / __ \\_|  | \\/\\___ \\ \\  ___/ |  | \\/ \n"));
  
             ("  |__|_|  /|____/ |____|    (____  /|__|  /____  > \\___  >|__|    \n"));
  
             ("        \\/                       \\/            \\/      \\/         \n"));
  
  
#ifdef _UNICODE
    myprintf (_T ("  Sample build with UNICODE support\n"));
  
#else   /* 
    myprintf (_T ("  Sample build with ASCII support\n"));
  
#endif  /* 
    myprintf (_T ("  (C) 2015 Ingo Berg\n"));
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  


//---------------------------------------------------------------------------
// Callback function for parser errors
  void
OnError (muParserHandle_t hParser) 
{
  
  
  
  
  
  


//---------------------------------------------------------------------------
  void
ListVar (muParserHandle_t a_hParser) 
{
  
  
  
    
  {
    
    
  
  
  
  
  
    
  {
    
    
    
    
               (long long) pVar);


//---------------------------------------------------------------------------
  void
ListExprVar (muParserHandle_t a_hParser) 
{
  
  
    
  {
    
    
  
  
  
  
  
  
    
  {
    
    
    
    


//---------------------------------------------------------------------------
  void
ListConst (muParserHandle_t a_hParser) 
{
  
  
    
  {
    
    
  
  
  
  
  
    
  {
    
    
    
    


//---------------------------------------------------------------------------
/** \brief Check for external keywords.
*/ 
  int
CheckKeywords (const muChar_t * a_szLine, muParserHandle_t a_hParser) 
{
  
    
  {
    
  
  
  else if (!mystrcmp (a_szLine, _T ("list var")))
    
  {
    
    
  
  
  else if (!mystrcmp (a_szLine, _T ("list exprvar")))
    
  {
    
    
  
  
  else if (!mystrcmp (a_szLine, _T ("list const")))
    
  {
    
    
  
  
  else if (!mystrcmp (a_szLine, _T ("locale de")))
    
  {
    
               ("Setting german locale: ArgSep=';' DecSep=',' ThousandsSep='.'\n"));
    
    
    
    
  
  
  else if (!mystrcmp (a_szLine, _T ("locale en")))
    
  {
    
               ("Setting english locale: ArgSep=',' DecSep='.' ThousandsSep=''\n"));
    
    
    
    
  
  
  else if (!mystrcmp (a_szLine, _T ("locale reset")))
    
  {
    
    
    
  
  
  else if (!mystrcmp (a_szLine, _T ("test bulk")))
    
  {
    
    
    
  
  



//---------------------------------------------------------------------------
  void
CalcBulk () 
{
  
  
  
  
  
  
    
  {
    
    
    
  
  
  
  
  
  
  
    
  {
    
    
    
    
    
    
    
  
  
    
  {
    
               r[i]);
    
    
  
  
  
  



//---------------------------------------------------------------------------
  void
Calc () 
{
  
  
  {
  1, 2};                        // Values of the parser variables
  muParserHandle_t hParser;
  
  Intro (hParser);
  
    // Set an error handler [optional]
    // the only function that does not take a parser instance handle
    mupSetErrorHandler (hParser, OnError);
  
    //#define GERMAN_LOCALS
#ifdef GERMAN_LOCALS
    mupSetArgSep (hParser, ';');
  
  
  
#else   /* 
    mupSetArgSep (hParser, ',');
  
  
#endif  /* 
    
    // Set a variable factory
    mupSetVarFactory (hParser, AddVariable, NULL);
  
    // Define parser variables and bind them to C++ variables [optional]
    mupDefineConst (hParser, _T ("const1"), 1);
  
  
  
    // Define parser variables and bind them to C++ variables [optional]
    mupDefineVar (hParser, _T ("a"), &afVarVal[0]);
  
  
    // Define postfix operators [optional]
    mupDefinePostfixOprt (hParser, _T ("M"), Mega, 0);
  
  
    // Define infix operator [optional]
    mupDefineInfixOprt (hParser, _T ("!"), Not, 0);
  
    // Define functions [optional]
    //  mupDefineStrFun(hParser, "query", SampleQuery, 0); // Add an unoptimizeable function 
    mupDefineFun0 (hParser, _T ("zero"), ZeroArg, 0);
  
  mupDefineFun1 (hParser, _T ("rnd2"), Rnd, 1);
  
  
    // Define binary operators [optional]
    mupDefineOprt (hParser, _T ("add"), Add, 0, muOPRT_ASCT_LEFT, 0);
  
  
    
  {
    
    
      
    {
    
      break;                    // no keyword found; parse the line
    case 1:
      continue;                 // A Keyword was found do not parse the line
    case -1:
      return;                   // abort the application
    }
    
    
    
      // Without an Error handler function 
      // you must use this for error treatment:
      //if (mupError(hParser))
      //{
      //  printf("\nError:\n");
      //  printf("------\n");
      //  printf("Message:  %s\n", mupGetErrorMsg(hParser) );
      //  printf("Token:    %s\n", mupGetErrorToken(hParser) );
      //  printf("Position: %s\n", mupGetErrorPos(hParser) );
      //  printf("Errc:     %d\n", mupGetErrorCode(hParser) );
      //  continue;
      //}
      
      
  
  
    // finalle free the parser ressources
    mupRelease (hParser);



//---------------------------------------------------------------------------
  int
main (int argc, char *argv[]) 
{
  
    // The next line is just for shutting up the compiler warning
    // about unused variables without getting another warning about not
    // beeing able to use type lists in function declarations.
    myprintf (_T ("Executing \"%s\" (argc=%d)\n"), argv[0], argc);
  
  
