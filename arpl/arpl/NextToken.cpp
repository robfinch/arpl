// ============================================================================
//        __
//   \\__/ o\    (C) 2012-2024  Robert Finch, Waterloo
//    \  __ /    All rights reserved.
//     \/_//     robfinch<remove>@finitron.ca
//       ||
//
// CC64 - 'C' derived language compiler
//  - 64 bit CPU
//
// This source file is free software: you can redistribute it and/or modify 
// it under the terms of the GNU Lesser General Public License as published 
// by the Free Software Foundation, either version 3 of the License, or     
// (at your option) any later version.                                      
//                                                                          
// This source file is distributed in the hope that it will be useful,      
// but WITHOUT ANY WARRANTY; without even the implied warranty of           
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            
// GNU General Public License for more details.                             
//                                                                          
// You should have received a copy of the GNU General Public License        
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    
//                                                                          
// ============================================================================
//
#include "stdafx.h"

extern char *errtext(int);

int token_stack[5];
char *id_stack[5];
Float128 rval128_stack[5];
double rval_stack[5];
int64_t ival_stack[5];
char ch_stack[5];
int token_sp = 0;

int inComment = FALSE;
int      my_errno[80];
int      numerrs;
char     inpline[100000];
int             total_errors = 0;
extern char     *lptr;          /* shared with preproc */
extern std::ifstream *inclfile[10];  /* shared with preproc */
extern int      inclline[10];   /* shared with preproc */
extern int      incldepth;      /* shared with preproc */
char            *linstack[20];  /* stack for substitutions */
char            chstack[20];    /* place to save lastch */
int             lstackptr = 0;  /* substitution stack pointer */
static char numstr[100];
static char *numstrptr;
static char backup_token = 0;

int window_pos = -1;
char ch_window[20];
int token_window[20];
Float128 rval128_window[20];
double rval_window[20];
int64_t ival_window[20];
std::string id_window[20];

void reset_window()
{
}

void slide_window()
{
  int nn;

  for (nn = 1; nn < 20; nn++) {
    ch_window[nn] = ch_window[nn - 1];
    token_window[nn] = token_window[nn - 1];
    rval128_window[nn] = rval128_window[nn - 1];
    rval_window[nn] = rval_window[nn - 1];
    ival_window[nn] = ival_window[nn - 1];
    id_window[nn] = id_window[nn - 1];
    ch_window[0] = lastch;
    token_window[0] = lastst;
    rval128_window[0] = rval128;
    rval_window[0] = rval;
    ival_window[0] = ival;
    id_window[0] = std::string(compiler.lastid);
  }
}

void push_ellipsis()
{
  if (token_sp < 5) {
    ch_stack[token_sp] = '.';
    token_stack[token_sp] = ellipsis;
    rval128_stack[token_sp] = rval128;
    rval_stack[token_sp] = rval;
    ival_stack[token_sp] = ival;
    id_stack[token_sp] = nullptr;
    token_sp++;
  }
}

void push_token()
{
	if (token_sp < 5) {
		ch_stack[token_sp] = lastch;
		token_stack[token_sp] = lastst;
		rval128_stack[token_sp] = rval128;
		rval_stack[token_sp] = rval;
		ival_stack[token_sp] = ival;
		id_stack[token_sp] = _strdup(compiler.lastid);
		token_sp++;
	}
}

void pop_token()
{
	if (token_sp > 0) {
		token_sp--;
		lastch = ch_stack[token_sp];
		lastst = token_stack[token_sp];
		rval128 = rval128_stack[token_sp];
		rval = rval_stack[token_sp];
		ival = ival_stack[token_sp];
		strcpy_s(compiler.lastid, sizeof(compiler.lastid), id_stack[token_sp]);
	}
}

int isalnum(char c)
{       return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9');
}

int isidch(char c) { return isalnum(c) || c == '_' || c == '$'; }
int my_isspace(char c) { return c == ' ' || c == '\t' || c == '\n' || c=='\r'; }
int my_isdigit(char c) { return (c >= '0' && c <= '9'); }

void initsym()
{
	lptr = inpline;
  inpline[0] = 0;
  numerrs = 0;
  total_errors = 0;
  lineno = 0;
}

int getline(int listflag)
{
	int rv;

    if(lineno > 0 && listflag) {
        lfs.printf((char *)"%6d\t%s",lineno,inpline);
		while(numerrs) {
			numerrs--;
			if (numerrs < 80)
				lfs.printf((char *)" *** error %d: %s\n",my_errno[numerrs],errtext(my_errno[numerrs]));
		}
        numerrs = 0;
    }
    ++lineno;
	memset(inpline, 0, sizeof(inpline));
    ifs->getline(inpline,sizeof(inpline)-2);
	strcat_s(inpline,sizeof(inpline),"\n");
	rv = ifs->gcount()==0;
	//printf("line:%.60s\r\n", inpline);
    if( rv && incldepth > 0 ) {
        ifs->close();
        ifs = inclfile[--incldepth];
        lineno = inclline[incldepth];
        return getline(0);
    }
    if( rv )
        return 1;
    lptr = inpline;
    if(inpline[0] == '#' && !inComment)
        return preprocess();
    return 0;
}

/*
 *      getch - basic get character routine.
 */
int getch()
{

	while( (lastch = *lptr++) == '\0') {
        if( lstackptr > 0 ) {
            lptr = linstack[--lstackptr];
            lastch = chstack[lstackptr];
            goto j1;
        }
        if(getline(incldepth == 0))
            return lastch = -1;
    }
 j1:
    if (numstrptr >= &numstr[0] && numstrptr < &numstr[98]) {
       *numstrptr = lastch;
       numstrptr++;
    }
    return lastch;
}
 
//
// getid - get an identifier.
//
// identifiers are any isidch conglomerate
// that doesn't start with a numeric character.
// this set INCLUDES keywords.
//
void getid()
{
	int    i;
    i = 0;
    compiler.lastid[0] = '_';
    while(isidch(lastch)) {
		if(i < 120) {
			lastkw[i] = lastch;
			i++;
			compiler.lastid[i] = lastch;
        }
		getch();
    }
    lastkw[i] = '\0';
    compiler.lastid[i+1] = '\0';
    lastst = id;
}
 
/*
 *      getsch - get a character in a quoted string.
 *
 *      this routine handles all of the escape mechanisms
 *      for characters in strings and character constants.
 */
int     getsch()        /* return an in-quote character */
{       register int    i, j;
        if(lastch == '\n')
                return -1;
        if(lastch != '\\' || !parseEsc) {
                i = lastch;
                getch();
                return i;
                }
        getch();        /* get an escaped character */
				if (lastch=='0')
        {
          for(j = i = 0; j < 6; ++j) {
            if(lastch <= '7' && lastch >= '0')
              i = (i << 3) + lastch - '0';
            else
              break;
            getch();
          }
          return i;
        }
				else if (my_isdigit(lastch)) {
					for (j = i = 0; j < 5; ++j) {
						if (lastch <= '9' && lastch >= '0')
							i = (i * 10) + lastch - '0';
						else
							break;
						getch();
					}
					return i;
				}
				else if (lastch == 'x' || lastch == 'X') {
					getch();
					for (j = i = 0; j < 4; ++j) {
						if (lastch <= '9' && lastch >= '0')
							i = (i << 4) + lastch - '0';
						else if (lastch >= 'A' && lastch <= 'F')
							i = (i << 4) + lastch - 'A' + 10;
						else if (lastch >= 'a' && lastch <= 'f')
							i = (i << 4) + lastch - 'a' + 10;
						else
							break;
						getch();
					}
					return i;
				}
        i = lastch;
        getch();
        switch(i) {
                case '\n':
                        getch();
                        return getsch();
                case 'b':
                        return '\b';
                case 'f':
                        return '\f';
                case 'n':
                        return '\n';
                case 'r':
                        return '\r';
				case 't':
						return '\t';
                default:
                        return i;
                }
}

void getFilename()
{
	int i;
	char j;

    for(i = 0;i < MAX_STRLEN;++i) {
            if(lastch == '>')
                    break;
            if((j = getsch()) == -1)
                    break;
            else
                    laststr[i] = j;
            }
    laststr[i] = 0;
    lastst = sconst;
    if(lastch != '>')
            error(ERR_SYNTAX);
    else
            getch();
}

int64_t radix36(char c)
{
	if(isdigit(c))
            return c - '0';
    if(c >= 'a' && c <= 'z')
            return c - 'a' + 10;
    if(c >= 'A' && c <= 'Z')
            return c - 'A' + 10;
    return -1;
}
 
/*
 *      getbase - get an integer in any base.
 */
void getbase(int64_t b)
{       
  register int64_t i0, i1, i2;
  register int64_t i, j;
  int k;
  bool isUnsigned = false;
  Int128 i128, b128, j128;
  b128 = Int128::Convert(b);
  i = 0;
  i128 = *Int128::Zero();
  i0 = 0;
  i1 = 0;
        while(isalnum(lastch)) {
                if((j = radix36(lastch)) < b) {
                  j128 = Int128::Convert(j);
                  i128.Mul(&i128, &i128, &b128);
                  i128.Add(&i128, &i128, &j128);
                        i = i * b + j;
                        i2 = i0;
                        for (k = 0; k < b; k++) {
                            i0 = i0 + i2;
                            if (i0 & 0x1000000000000000LL) {
                               i0 = i0 - 0x1000000000000000LL;
                               i1 = i1 + 1;
                            }
                        }
                        i0 = i0 + j;
                        if (i0 & 0x1000000000000000LL) {
                            i0 = i0 - 0x1000000000000000LL;
                            i1 = i1 + 1;
                        }
                        getch();
                        }
                else break;
                }
        if (lastch == 'L' || lastch == 'U' || lastch == 'l' || lastch == 'u' || lastch == 'S' || lastch == 's') { // ignore a 'L'ong suffix and 'U'nsigned
          if (lastch == 'u' || lastch == 'U')
            isUnsigned = true;
          if (lastch != 'U' && lastch != 'u')
            int_precision = toupper(lastch);
          getch();
          if (lastch == 'L' || lastch == 'U' || lastch == 'l' || lastch == 'u' || lastch == 'S' || lastch == 's') { // ignore a 'L'ong suffix and 'U'nsigned
            getch();
          }
        }
    ival128 = i128;
        ival = i;
/*
        rval.exp = 0x804E;
        rval.man1 = (i1 >> 32) & 0xffffL;
        rval.man2 = (i1 >> 16) & 0xffffL;
        rval.man3 = i1 & 0xffffL;
        rval.man4 = (i0 >> 16) & 0ffffL;
        rval.man5 = i0 & 0xffffL;
        // normalize the number
        while (!(rval.man1 & 0x8000)) {
             rval.exp--;
             rval.man1 = (rval.man1 << 1) | (rval.man2 & 0x8000) ? 1 : 0;
             rval.man2 = (rval.man2 << 1) | (rval.man3 & 0x8000) ? 1 : 0;
             rval.man3 = (rval.man3 << 1) | (rval.man4 & 0x8000) ? 1 : 0;
             rval.man4 = (rval.man4 << 1) | (rval.man5 & 0x8000) ? 1 : 0;
             rval.man5 = (rval.man5 << 1);
        }
*/
  lastst = iconst;
  if (isUnsigned)
    lastst = uiconst;
}
 
//
//      getfrac - get fraction part of a floating number.
//
void getfrac()
{
	Float128 frmul128,tmp,ch128,fract128;
  Posit64 pmul64, pch64, pfract64, p10;
  Posit64 pmu, pot;

	double frmul;
	double fract = 0.0;
	Float128::Assign(&frmul128,Float128::One());
  pmul64.One();
  pmu.One();
  pfract64.Zero();
  p10.Ten();
  pot.OneTenth();
  frmul = 1.0;
  while(my_isdigit(lastch)) {
		Float128::IntToFloat(&ch128,lastch-'0');
		Float128::Mul(&tmp,&fract128,Float128::Ten());
		Float128::Add(&fract128,&ch128,&tmp);

        fract = fract * 10.0 + (lastch - '0');
        pch64 = pch64.IntToPosit(lastch - '0');
        pfract64 = pfract64.Multiply(pfract64, p10);
        pfract64 = pfract64.Add(pch64, pfract64);
        getch();
        frmul *= 10.0;
		Float128::Mul(&frmul128,&frmul128,Float128::Ten());

    pmul64 = pmul64.Multiply(pmul64, p10);
    pmu = pmu.Multiply(pmu, pot);
  }
	fract = fract / frmul;
	rval += fract;
	Float128::Div(&fract128,&fract128,&frmul128);
	Float128::Add(&rval128,&rval128,&fract128);
  pfract64 = pfract64.Divide(pfract64, pmul64);
  //pfract64 = pfract64.Multiply(pfract64, pmu);
  pval64 = pval64.Add(pval64, pfract64);
}

/*
 *      getexp - get exponent part of floating number.
 *
 *      this algorithm is primative but usefull.  Floating
 *      exponents are limited to +/-255 but most hardware
 *      won't support more anyway.
 */
void getexp()
{       
		double  expo, exmul;
		Float128 exp128, exmul128;
    Posit64 pexp64, pexmul64;
		
    pexp64.One();
		Float128::Assign(&exp128,Float128::One());
        expo = 1.0;
        if (lastst != rconst) {
          pval64 = pval64.IntToPosit(ival);
          rval = (double)ival;
        }
        if(lastch == '-') {
			Float128::Assign(&exmul128,Float128::OneTenth());
      pexmul64.OneTenth();
                exmul = 0.1;
                getch();
                }
		else if (lastch=='+') {
			getch();
			Float128::Assign(&exmul128,Float128::Ten());
      pexmul64.Ten();
                exmul = 10.0;
		}
        else {
			Float128::Assign(&exmul128,Float128::Ten());
      pexmul64.Ten();
                exmul = 10.0;
		}
        getbase(10);
        if(ival > 32767)
                error(ERR_FPCON);
        else
                while(ival--) {
                  pexp64 = pexp64.Multiply(pexp64, pexmul64);
					Float128::Mul(&exp128,&exp128,&exmul128);
                        expo *= exmul;
				}
        pval64 = pval64.Multiply(pval64, pexp64);
		Float128::Mul(&rval128,&rval128,&exp128);

        rval *= expo;
}
 
/*
 *      getnum - get a number from input.
 *
 *      getnum handles all of the numeric input. it accepts
 *      decimal, octal, hexidecimal, and floating point numbers.
 */
void getnum()
{
  register int    i;
  bool isPosit = false;
  bool isUnsigned = false;

  i = 0;
  float_precision = ' ';
  int_precision = ' ';
  ival = 0;
  rval = 0.0;
  pval64.Zero();
	Float128::Assign(&rval128,Float128::Zero());
  numstrptr = &numstr[0];
  *numstrptr = lastch;
  numstrptr++; 
  if(lastch == '0') {
    getch();
    if (lastch=='.')
      goto j1;
    if(lastch == 'x' || lastch == 'X') {
      getch();
      getbase(16);
    }
    else getbase(8);
    // Ignore 'U' unsigned suffix
    if (lastch == 'U' || lastch == 'u') {
      isUnsigned = true;
      getch();
    }
    // Ignore 'L' unsigned suffix
    if (lastch == 'L' || lastch == 'l' || lastch == 'S' || lastch == 's') {
      int_precision = lastch;
      getch();
      if (lastch == 'L' || lastch == 'l') {
        getch();
        if (lastch == 'U' || lastch == 'u') {
          isUnsigned = true;
          getch();
        }
      }
    }
  }
  else {
    getbase(10);
    // Ignore 'U' unsigned suffix
    if (lastch == 'U' || lastch == 'u') {
      isUnsigned = true;
      getch();
    }
    // Ignore 'L' unsigned suffix
    if (lastch == 'L' || lastch == 'l' || lastch == 'S' || lastch == 's') {
      int_precision = lastch;
      getch();
      if (lastch == 'L' || lastch == 'l') {
        getch();
        if (lastch == 'U' || lastch == 'u') {
          isUnsigned = true;
          getch();
        }
      }
    }
j1:
    if(lastch == '.') {
      if (lptr[0] == '.' && lptr[1] == '.') {
        lptr--;
        lastch = '.';
        goto j2;
      }
      getch();
      rval = (double)ival;    /* float the integer part */
			Float128::IntToFloat(&rval128, (__int64)ival);
      pval64 = pval64.Posit64::IntToPosit(ival);
      getfrac();      /* add the fractional part */
      lastst = rconst;
    }
j2:
    if (lastch == 'p' || lastch == 'P') {
      getch();
      isPosit = true;
      lastst = pconst;
      if (lastch == 's' || lastch == 'h' || lastch=='S' || lastch=='H') {
        float_precision = tolower(lastch);
        getch();
      }
    }
    if(lastch == 'e' || lastch == 'E') {
      getch();
      getexp();       /* get the exponent */
		// This must be reset because getting the exponent
		// calls getbase() which will set lastst=iconst
  		lastst = isPosit ? pconst : rconst;
    }
				
		if (lastst==rconst && (lastch=='Q' || lastch=='q' ||
			lastch=='D' || lastch=='d' || lastch=='s' || lastch=='S' || lastch=='T' || lastch=='t')) {
			float_precision = tolower(lastch);
			getch();
		}
		else if (!isPosit)
			float_precision = 'd';
				
		// Ignore 'U' unsigned suffix
		if (lastch=='U' || lastch=='u') {
      isUnsigned = true;
			getch();
		}
    // Ignore 'L' unsigned suffix
    if (lastch == 'L' || lastch == 'l') {
      getch();
      if (lastch == 'L' || lastch == 'l') {
        getch();
        if (lastch == 'U' || lastch == 'u') {
          isUnsigned = true;
          getch();
        }
      }
    }
  }
  numstrptr[-1]='\0';
  numstrptr = NULL;
//    dd_real::read(numstr,rval);
//    printf("leave getnum=%s\r\n", numstr);
  if (lastst == iconst && isUnsigned)
    lastst = uiconst;
}

void SkipSpaces()
{
    while( my_isspace(lastch) ) {
        getch(); 
    }
}
/*
 *      NextToken - get next symbol from input stream.
 *
 *      NextToken is the basic lexical analyzer.  It builds
 *      basic tokens out of the characters on the input
 *      stream and sets the following global variables:
 *
 *      lastch:         A look behind buffer.
 *      lastst:         type of last symbol read.
 *      laststr:        last string constant read.
 *      compiler.lastid:         last identifier read.
 *      ival:           last integer constant read.
 *      rval:           last real constant read.
 *
 *      NextToken should be called for all your input needs...
 */
void NextToken(int typ)
{
  int i, j;
  Symbol *sp;
  int tch;
restart:        /* we come back here after comments */
  if (window_pos >= 0) {
    lastch = ch_window[window_pos];
    strncpy_s(compiler.lastid, sizeof(compiler.lastid), id_window[window_pos].c_str(), sizeof(compiler.lastid));
    rval128 = rval128_window[window_pos];
    rval = rval_window[window_pos];
    ival = ival_window[window_pos];
    lastst = token_window[window_pos];
    window_pos--;
    return;
  }
  else
    slide_window();
  if (token_sp > 0) {
		pop_token();
		return;
	}
  if (backup_token) {
     backup_token = 0;
     lastch = '(';
     lastst = openpa;
     return;
  }
	SkipSpaces();
  if( lastch == -1) {
    lastst = my_eof;
    dfs.printf("Returning EOF from NextToken.\n");
  }
  else if(my_isdigit(lastch)) {
    getnum();
  }
  else if(isidch(lastch)) {
		getid();
		if (lastch == '"' && compiler.lastid[0] == '_' && (compiler.lastid[1]=='B' || compiler.lastid[1]=='W' || compiler.lastid[1]=='T' || compiler.lastid[1]=='O'
			|| compiler.lastid[1] == 'b' || compiler.lastid[1] == 'w' || compiler.lastid[1] == 't' || compiler.lastid[1] == 'o' || compiler.lastid[1]=='L' || compiler.lastid[1]=='l')) {
			getch();
			laststr[0] = toupper(compiler.lastid[1]);
			for (i = 1; i < MAX_STRLEN; ++i) {
				if (lastch == '\"')
					break;
				if ((j = getsch()) == -1)
					break;
				else
					laststr[i] = j;
			}
			laststr[i] = 0;
			lastst = asconst;
			if (lastch != '\"')
				error(ERR_SYNTAX);
			else
				getch();
		}
		else if (lastch == '"' && compiler.lastid[0]=='_' && compiler.lastid[1]=='I' && compiler.lastid[2]=='\0') {
			getch();
			laststr[0] = 'W';
			for (i = 1; i < MAX_STRLEN; ++i) {
				if (lastch == '\"')
					break;
				if ((j = getsch()) == -1)
					break;
				else
					laststr[i] = j;
			}
			laststr[i] = 0;
			lastst = isconst;
			if (lastch != '\"')
				error(ERR_SYNTAX);
			else
				getch();
		}
		else {

			if ((sp = defsyms.Find(compiler.lastid, false)) != NULL) {
				tch = lastch;
				if (!(lastch == ')' && sp->value.s[0] == '(')) {
					if (lstackptr < 19) {
						linstack[lstackptr] = lptr;
						chstack[lstackptr++] = tch;
						lptr = sp->value.s;
					}
					getch();
					goto restart;
				}
			}
		}
  }
  else {
  switch(lastch) {
  case '+':
    getch();
    if(lastch == '+') {
            getch();
            lastst = autoinc;
            }
    else if(lastch == '=') {
            getch();
            lastst = asplus;
            }
    else
		lastst = plus;
    break;
                case '-':
                        getch();
                        if(lastch == '-') {
                                getch();
                                lastst = autodec;
                                }
                        else if(lastch == '=') {
                                getch();
                                lastst = asminus;
                                }
                        else if(lastch == '>') {
                                getch();
                                lastst = pointsto;
                                }
                        else  
							lastst = minus;
                        break;
                case '*':
                        getch();
                        if(lastch == '=') {
                                getch();
                                lastst = astimes;
                                }
                        else lastst = star;
                        break;
                case '/':
                        getch();
                        if(lastch == '=') {
                          getch();
                          lastst = asdivide;
                        }
                        else if(lastch == '*') {
                          inComment = TRUE;
                          getch();
                          for(;;) {
                            if(lastch == '*') {
                              getch();
                              if(lastch == '/') {
                                getch();
														  inComment = FALSE;
                              goto restart;
                            }
                          }
                          else
                            getch();
                          }
                        }
	    			else if (lastch == '/') {

							inComment = TRUE;
							for(;;) {
								getch();
								if (lastch=='\n' || lastch==-1) {
									//getch();
									inComment = FALSE;
									goto restart;
								}
							}

						}
                        else lastst = divide;
                        break;
                case '^':
                        getch();
                        if( lastch == '=') {
							lastst = asxor;
							getch();
                        }
						else
							lastst = uparrow;
                        break;
                case ';':
                        getch();
                        lastst = semicolon;
                        break;
                case ':':
                        getch();
						if (lastch==':') {
							lastst = double_colon;
							getch();
						}
						else
							lastst = colon;
                        break;
                case '=':
                        getch();
                        if(lastch == '=') {
                                getch();
                                lastst = eq;
                                }
                        else lastst = assign;
                        break;
                case '>':
                        getch();
                        if(lastch == '=') {
                            getch();
                            lastst = geq;
                        }
                        else if(lastch == '>') {
                            getch();
                            if(lastch == '=') {
                                getch();
                                lastst = asrshift;
                            }
							else if (lastch=='>') {
								getch();
								if (lastch=='=') {
									getch();
									lastst = asrrot;
								}
								else
									lastst = rrot;
							}
                            else lastst = rshift;
                        }
                        else lastst = gt;
                        break;
                case '<':
                        getch();
                        if(lastch == '=') {
                                getch();
                                lastst = leq;
                                }
                        else if(lastch == '<') {
                                getch();
                                if(lastch == '=') {
                                    getch();
                                    lastst = aslshift;
                                }
								else if (lastch=='<') {
									getch();
									if (lastch=='=') {
										getch();
										lastst = aslrot;
									}
									else
										lastst = lrot;
								}
								else
									lastst = lshift;
						}
						else if (lastch == '>') {
							getch();
							lastst = neq;
						}
                        else lastst = lt;
                        break;
                case '\'':
                        getch();
                        ival = getsch();        /* get a string char */
                        if(lastch != '\'')
                                error(ERR_SYNTAX);
                        else
                                getch();
                        lastst = cconst;
                        break;
                case '\"':
                        getch();
												laststr[0] = 'W';
                        for(i = 1;i < MAX_STRLEN;++i) {
                                if(lastch == '\"')
                                        break;
                                if((j = getsch()) == -1)
                                        break;
                                else
                                        laststr[i] = j;
                                }
                        laststr[i] = 0;
                        lastst = sconst;
                        if(lastch != '\"')
                                error(ERR_SYNTAX);
                        else
                                getch();
                        break;
                case '!':
                        getch();
                        if(lastch == '=') {
                                getch();
                                lastst = neq;
                                }
                        else lastst = nott;
                        break;
                case '%':
                        getch();
                        if(lastch == '=') {
                                getch();
                                lastst = asmodop;
                                }
                        else lastst = modop;
                        break;
                case '~':
                        getch();
                        lastst = cmpl;
                        break;
                case '.':
                        getch();
                        lastst = dot;
						if (lastch=='.') {
							getch();
							if (lastch=='.') {
								getch();
								lastst = ellipsis;
								strcpy_s(compiler.lastid, sizeof(compiler.lastid), "...");
							}
						}
                        break;
                case ',':
                        getch();
                        lastst = comma;
                        break;
                case '&':
                        getch();
                        if( lastch == '&') {
                                lastst = land;
                                getch();
																if (lastch == '&') {
																	lastst = land_safe;
																	getch();
																}
                                }
                        else if( lastch == '=') {
                                lastst = asand;
                                getch();
                                }
                        else
                                lastst = bitandd;
                        break;
                case '|':
                        getch();
                        if(lastch == '|') {
                                lastst = lor;
                                getch();
																if (lastch == '|') {
																	lastst = lor_safe;
																	getch();
																}
                                }
                        else if( lastch == '=') {
                                lastst = asor;
                                getch();
                                }
                        else
                                lastst = bitorr;
                        break;
                case '(':
                        getch();
                        lastst = openpa;
                        break;
                case ')':
                        getch();
                        lastst = closepa;
                        break;
                case '[':
                        getch();
                        lastst = openbr;
                        break;
                case ']':
                        getch();
                        lastst = closebr;
                        break;
                case '{':
                        getch();
                        lastst = begin;
                        break;
                case '}':
                        getch();
                        lastst = end;
                        break;
                case '?':
                        getch();
                        lastst = hook;
												if (lastch == '?') {
													getch();
													lastst = mux_hook;
												}
                        break;
                case '\\':
                        getch();
                        goto restart;
                default:
                        getch();
                        error(ERR_ILLCHAR);
                        goto restart;   /* get a real token */
                }
              }
  if (lastst == id) {
    if (typ == 1)
      IdentifyPrecision();
    else if (typ == 2) {
      if (!IdentifyKeyword())
        IdentifyPrecision();
    }
    else
      IdentifyKeyword();
  }

//	printf("token: %d",lastst);
//	if (lastst==id)
//		printf("compiler.lastid=%s| ", compiler.lastid);
}

void needpunc(enum e_sym p,int clue)
{
	if( lastst == p)
    NextToken();
	else {
		//printf("%d %s\r\n", lineno, inpline);
		printf("*************clue:%d************\r\n",clue);
    error(ERR_PUNCT);
	}
}

void backup() {
    backup_token = 1;
}

