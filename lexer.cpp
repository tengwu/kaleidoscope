#include <cstdio>
#include <string>
#include "lexer.h"

/// gettok - Return the next token from standard input.
static int gettok() {
  static int LastChar = ' ';

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;
  
    if (IdentifierStr == "def")
      return tok_def;
    if (IdentifierStr == "extern")
      return tok_extern;
    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
    int InvalidCharCNT = 0;
    std::string NumStr;
    do {
      InvalidCharCNT += (LastChar == '.') || (!isdigit(LastChar) && LastChar != '.');
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    // Eat until the end of the token
    if (InvalidCharCNT > 1)
      return tok_unknown;
  
    NumVal = strtod(NumStr.c_str(), 0);
    return tok_number;
  }

  if (LastChar == '#') {
    // Comment until end of line.
    do
      LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
  
    if (LastChar != EOF)
      return gettok();
  }

    // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;
  
    // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

int main(int argc, char *argv[])
{
  std::string progname;
  if (argc < 2)
    progname = "testprog/prog.kalei";
  else
    progname = argv[1];
  int type;
  freopen(progname.c_str(), "r", stdin);
  while ((type = gettok()) != tok_eof)
    printf("token_type: %d\n", type);

  return 0;
}
