/*
 * lex.c	--	Generate all of the lexical type files: parser.dlg tokens.h
 *
 * $Id: lex.c,v 1.4 95/09/26 12:58:36 parrt Exp $
 * $Revision: 1.4 $
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 *
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * ANTLR 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-1995
 */
#include <stdio.h>
#include <ctype.h>
/* MR1						                                                */
/* MR1  10-Apr-97  MR1	Replace use of __STDC__ with __USE_PROTOS	    */
/* MR1				                                                        */
#include "config.h"
#include "set.h"
#include "syn.h"
#include "hash.h"
#include "generic.h"

#define DLGErrorString "invalid token"

/* Generate a complete lexical description of the lexemes found in the grammar */
void
#ifdef __USE_PROTOS
genLexDescr( void )
#else
genLexDescr( )
#endif
{
	ListNode *p;
	FILE *dlgFile = fopen(OutMetaName(DlgFileName), "w");
	require(dlgFile!=NULL, eMsg1("genLexFile: cannot open %s", OutMetaName(DlgFileName)) );
#if SPECIAL_FOPEN
	special_fopen_actions(OutMetaName(DlgFileName));	             /* MR1 */
#endif
	fprintf(dlgFile, "<<\n");
	fprintf(dlgFile, "/* %s -- DLG Description of scanner\n", DlgFileName);
	fprintf(dlgFile, " *\n");
	fprintf(dlgFile, " * Generated from:");
	{int i; for (i=0; i<NumFiles; i++) fprintf(dlgFile, " %s", FileStr[i]);}
	fprintf(dlgFile, "\n");
	fprintf(dlgFile, " *\n");
	fprintf(dlgFile, " * Terence Parr, Will Cohen, and Hank Dietz: 1989-1994\n");
	fprintf(dlgFile, " * Purdue University Electrical Engineering\n");
	fprintf(dlgFile, " * With AHPCRC, University of Minnesota\n");
	fprintf(dlgFile, " * ANTLR Version %s\n", Version);
	fprintf(dlgFile, " */\n\n");
	if ( GenCC )
	{
		if ( !UserDefdTokens ) fprintf(dlgFile, "#include \"%s\"\n", DefFileName);
		else fprintf(dlgFile, "#include %s\n", UserTokenDefsFile);
		fprintf(dlgFile, "#include \"%s\"\n", ATOKEN_H);
		if ( GenAST ) fprintf(dlgFile, "#include \"%s\"\n", ASTBASE_H);
		if ( HdrAction != NULL ) dumpAction( HdrAction, dlgFile, 0, -1, 0, 1 );
	}
	else
	{
		fprintf(dlgFile, "#include <stdio.h>\n");
		fprintf(dlgFile, "#define ANTLR_VERSION	%s\n", VersionDef);
		if ( strcmp(ParserName, DefaultParserName)!=0 )
			fprintf(dlgFile, "#define %s %s\n", DefaultParserName, ParserName);
		if ( strcmp(ParserName, DefaultParserName)!=0 )
			fprintf(dlgFile, "#include \"%s\"\n", RemapFileName);
		if ( HdrAction != NULL ) dumpAction( HdrAction, dlgFile, 0, -1, 0, 1 );
		if ( FoundGuessBlk )
		{
			fprintf(dlgFile, "#define ZZCAN_GUESS\n");
			fprintf(dlgFile, "#include <setjmp.h>\n");
		}
		if ( OutputLL_k > 1 ) fprintf(dlgFile, "#define LL_K %d\n", OutputLL_k);
		if ( DemandLookahead ) fprintf(dlgFile, "#define DEMAND_LOOK\n");
		fprintf(dlgFile, "#include \"antlr.h\"\n");
		if ( GenAST ) {
			fprintf(dlgFile, "#include \"ast.h\"\n");
		}
		if ( UserDefdTokens )
			fprintf(dlgFile, "#include %s\n", UserTokenDefsFile);
		/* still need this one as it has the func prototypes */
		fprintf(dlgFile, "#include \"%s\"\n", DefFileName);
		fprintf(dlgFile, "#include \"dlgdef.h\"\n");
		fprintf(dlgFile, "LOOKAHEAD\n");
		fprintf(dlgFile, "void zzerraction()\n");
		fprintf(dlgFile, "{\n");
		fprintf(dlgFile, "\t(*zzerr)(\"%s\");\n", DLGErrorString);
		fprintf(dlgFile, "\tzzadvance();\n");
		fprintf(dlgFile, "\tzzskip();\n");
		fprintf(dlgFile, "}\n");
	}
	fprintf(dlgFile, ">>\n\n");

	/* dump all actions */

/* MR1									                                    */
/* MR1  11-Apr-97	Provide mechanism for inserting code into DLG class     */
/* MR1	 	   	  via <<%%lexmember ....>> & <<%%lexprefix ...>>            */
/* MR1				                					    */
          if (LexActions != NULL) {
            for (p = LexActions->next; p!=NULL; p=p->next)
		{
/* MR1 */	fprintf(dlgFile, "<<%%%%lexaction\n");
			dumpAction( (char *)p->elem, dlgFile, 0, -1, 0, 1 );
			fprintf(dlgFile, ">>\n\n");
		}
	  };

/* MR1 */ if (GenCC) {
/* MR1 */   fprintf(dlgFile,"<<%%%%parserclass %s>>\n\n",CurrentClassName);
/* MR1 */ };

/* MR1 */ if (LexPrefixActions != NULL) {
/* MR1 */   for (p = LexPrefixActions->next; p!=NULL; p=p->next)
/* MR1 */       {
/* MR1 */               fprintf(dlgFile, "<<%%%%lexprefix\n");
/* MR1 */               dumpAction( (char *)p->elem, dlgFile, 0, -1, 0, 1 );
/* MR1 */               fprintf(dlgFile, ">>\n\n");
/* MR1 */       }
/* MR1 */ };

/* MR1 */ if (LexMemberActions != NULL) {
/* MR1 */   for (p = LexMemberActions->next; p!=NULL; p=p->next)
/* MR1 */       {
/* MR1 */               fprintf(dlgFile, "<<%%%%lexmember\n");
/* MR1 */               dumpAction( (char *)p->elem, dlgFile, 0, -1, 0, 1 );
/* MR1 */               fprintf(dlgFile, ">>\n\n");
/* MR1 */       }
/* MR1 */ };

	/* dump all regular expression rules/actions (skip sentinel node) */
	if ( ExprOrder == NULL ) {
		warnNoFL("no regular expressions found in grammar");
	}
	else dumpLexClasses(dlgFile);
	fprintf(dlgFile, "%%%%\n");
	fclose( dlgFile );
}

/* For each lexical class, scan ExprOrder looking for expressions
 * in that lexical class.  Print out only those that match.
 * Each element of the ExprOrder list has both an expr and an lclass
 * field.
 */
void
#ifdef __USE_PROTOS
dumpLexClasses( FILE *dlgFile )
#else
dumpLexClasses( dlgFile )
FILE *dlgFile;
#endif
{
	int i;
	TermEntry *t;
	ListNode *p;
	Expr *q;

	for (i=0; i<NumLexClasses; i++)
	{
		fprintf(dlgFile, "\n%%%%%s\n\n", lclass[i].classnum);
		for (p=ExprOrder->next; p!=NULL; p=p->next)
		{
			q = (Expr *) p->elem;
			if ( q->lclass != i ) continue;
			lexmode(i);
			t = (TermEntry *) hash_get(Texpr, q->expr);
			require(t!=NULL, eMsg1("genLexDescr: rexpr %s not in hash table",q->expr) );
			if ( t->token == EpToken ) continue;
			fprintf(dlgFile, "%s\n\t<<\n", StripQuotes(q->expr));
			/* replace " killed by StripQuotes() */
			q->expr[ strlen(q->expr) ] = '"';
			if ( !GenCC ) {
				if ( TokenString(t->token) != NULL )
					fprintf(dlgFile, "\t\tNLA = %s;\n", TokenString(t->token));
				else
					fprintf(dlgFile, "\t\tNLA = %d;\n", t->token);
			}
			if ( t->action != NULL ) dumpAction( t->action, dlgFile, 2,-1,0,1 );
			if ( GenCC ) {
				if ( TokenString(t->token) != NULL )
					fprintf(dlgFile, "\t\treturn %s;\n", TokenString(t->token));
				else
					fprintf(dlgFile, "\t\treturn (ANTLRTokenType)%d;\n", t->token);
			}
			fprintf(dlgFile, "\t>>\n\n");
		}
	}
}

/* Strip the leading path (if any) from a filename */
char *
#ifdef __USE_PROTOS
StripPath( char *fileName )
#else
StripPath( fileName )
char *fileName;
#endif
{
	char *p;
	static char dirSym[2] = DirectorySymbol;

	if(NULL != (p = strrchr(fileName, dirSym[0])))
		p++;
	else
		p = fileName;

	return(p);
}

/* Generate a list of #defines && list of struct definitions for
 * aggregate retv's */
void
#ifdef __USE_PROTOS
genDefFile( void )
#else
genDefFile( )
#endif
{
	int i;

	/* If C++ mode and #tokdef used, then don't need anything in here since
	 * C++ puts all definitions in the class file name.
	 */
	if ( GenCC && UserTokenDefsFile ) return;

	DefFile = fopen(OutMetaName(DefFileName), "w");
	require(DefFile!=NULL, eMsg1("genDefFile: cannot open %s", OutMetaName(DefFileName)) );
#if SPECIAL_FOPEN
	special_fopen_actions(OutMetaName(DefFileName));	             /* MR1 */
#endif
	fprintf(DefFile, "#ifndef %s\n", StripPath(gate_symbol(DefFileName)));
	fprintf(DefFile, "#define %s\n", StripPath(gate_symbol(DefFileName)));

	fprintf(DefFile, "/* %s -- List of labelled tokens and stuff\n", DefFileName);
	fprintf(DefFile, " *\n");
	fprintf(DefFile, " * Generated from:");
	for (i=0; i<NumFiles; i++) fprintf(DefFile, " %s", FileStr[i]);
	fprintf(DefFile, "\n");
	fprintf(DefFile, " *\n");
	fprintf(DefFile, " * Terence Parr, Will Cohen, and Hank Dietz: 1989-1994\n");
	fprintf(DefFile, " * Purdue University Electrical Engineering\n");
	fprintf(DefFile, " * ANTLR Version %s\n", Version);
	fprintf(DefFile, " */\n");

	if ( !GenCC && LexGen ) {
		fprintf(DefFile,"#define zzEOF_TOKEN %d\n",
				TokenInd!=NULL?TokenInd[EofToken]:EofToken);
	}

	if ( !UserDefdTokens )
	{
		int first=1;

		if ( GenCC ) fprintf(DefFile, "enum ANTLRTokenType {\n");
		for (i=1; i<TokenNum; i++)
		{
			/* Don't do EpToken or expr w/o labels */
			if ( TokenString(i)!=NULL && i != EpToken )
			{
				TermEntry *p;
				
				if ( WarningLevel>1 )
				{
					int j;
					/* look in all lexclasses for the reg expr */
					for (j=0; j<NumLexClasses; j++)
					{
						lexmode(j);
						if ( ExprString(i)!=NULL ) break;
					}
					if ( j>=NumLexClasses )
					{
						warnNoFL(eMsg1("token label has no associated rexpr: %s",TokenString(i)));
					}
				}
				require((p=(TermEntry *)hash_get(Tname, TokenString(i))) != NULL,
						"token not in sym tab when it should be");
				if ( !p->classname )
				{
					if ( GenCC ) {
						if ( !first ) fprintf(DefFile, ",\n");
						first = 0;
						fprintf(DefFile, "\t%s=%d", TokenString(i), i);
					}
					else
						fprintf(DefFile, "#define %s %d\n", TokenString(i), i);
				}
			}
		}
/* MR1									                                    */
/* MR1  10-Apr-97 133MR1	Prevent use of varying sizes of integer	    */
/* MR1				for the enum ANTLRTokenType                             */
/* MR1								           */
		if ( GenCC ) {				                                 /* MR1 */
		     fprintf(DefFile, ",\n\tDLGminToken=0");                 /* MR1 */
		     fprintf(DefFile, ",\n\tDLGmaxToken=9999};\n");          /* MR1 */
                };						                             /* MR1 */
	}

	if ( !GenCC ) GenRulePrototypes(DefFile, SynDiag);

	fprintf(DefFile, "\n#endif\n");
}

void
#ifdef __USE_PROTOS
GenRemapFile( void )
#else
GenRemapFile( )
#endif
{
	if ( strcmp(ParserName, DefaultParserName)!=0 )
	{
		FILE *f;
		int i;

		f = fopen(OutMetaName(RemapFileName), "w");
		require(f!=NULL, eMsg1("GenRemapFile: cannot open %s", OutMetaName(RemapFileName)) );
#ifdef SPECIAL_FOPEN
		special_fopen_actions(OutMetaName(RemapFileName));           /* MR1 */
#endif
		fprintf(f, "/* %s -- List of symbols to remap\n", RemapFileName);
		fprintf(f, " *\n");
		fprintf(f, " * Generated from:");
		for (i=0; i<NumFiles; i++) fprintf(f, " %s", FileStr[i]);
		fprintf(f, "\n");
		fprintf(f, " *\n");
		fprintf(f, " * Terence Parr, Will Cohen, and Hank Dietz: 1989-1994\n");
		fprintf(f, " * Purdue University Electrical Engineering\n");
		fprintf(f, " * ANTLR Version %s\n", Version);
		fprintf(f, " */\n");

		GenRuleFuncRedefs(f, SynDiag);
		GenPredefinedSymbolRedefs(f);
		if ( GenAST ) GenASTSymbolRedefs(f);
		GenSetRedefs(f);

		fclose(f);
	}
}

/* Generate a bunch of #defines that rename all functions to be "ParserName_func" */
void
#ifdef __USE_PROTOS
GenRuleFuncRedefs( FILE *f, Junction *p )
#else
GenRuleFuncRedefs( f, p )
FILE *f;
Junction *p;
#endif
{
	fprintf(f, "\n/* rename rule functions to be 'ParserName_func' */\n");
	while ( p!=NULL )
	{
		fprintf(f, "#define %s %s_%s\n", p->rname, ParserName, p->rname);
		p = (Junction *)p->p2;
	}
}

/* Generate a bunch of #defines that rename all standard symbols to be
 * "ParserName_symbol".  The list of standard symbols to change is in
 * globals.c.
 */
void
#ifdef __USE_PROTOS
GenPredefinedSymbolRedefs( FILE *f )
#else
GenPredefinedSymbolRedefs( f )
FILE *f;
#endif
{
	char **p;

	fprintf(f, "\n/* rename PCCTS-supplied symbols to be 'ParserName_symbol' */\n");
	for (p = &StandardSymbols[0]; *p!=NULL; p++)
	{
		fprintf(f, "#define %s %s_%s\n", *p, ParserName, *p);
	}
}

/* Generate a bunch of #defines that rename all AST symbols to be
 * "ParserName_symbol".  The list of AST symbols to change is in
 * globals.c.
 */
void
#ifdef __USE_PROTOS
GenASTSymbolRedefs( FILE *f )
#else
GenASTSymbolRedefs( f )
FILE *f;
#endif
{
	char **p;

	fprintf(f, "\n/* rename PCCTS-supplied AST symbols to be 'ParserName_symbol' */\n");
	for (p = &ASTSymbols[0]; *p!=NULL; p++)
	{
		fprintf(f, "#define %s %s_%s\n", *p, ParserName, *p);
	}
}

/* redefine all sets generated by ANTLR; WARNING:  'zzerr', 'setwd' must match
 * use in bits.c (DumpSetWd() etc...)
 */
void
#ifdef __USE_PROTOS
GenSetRedefs( FILE *f )
#else
GenSetRedefs( f )
FILE *f;
#endif
{
	int i;

	for (i=1; i<=wordnum; i++)
	{
		fprintf(f, "#define setwd%d %s_setwd%d\n", i, ParserName, i);
	}
	for (i=1; i<=esetnum; i++)
	{
		fprintf(f, "#define zzerr%d %s_err%d\n", i, ParserName, i);
	}
}

/* Find all return types/parameters that require structs and def
 * all rules with ret types.
 */
void
#ifdef __USE_PROTOS
GenRulePrototypes( FILE *f, Junction *p )
#else
GenRulePrototypes( f, p )
FILE *f;
Junction *p;
#endif
{
	int i;

	i = 1;
	while ( p!=NULL )
	{
		if ( p->ret != NULL )
		{
			if ( HasComma(p->ret) )
			{
				DumpRetValStruct(f, p->ret, i);
			}
			fprintf(f, "\n#ifdef __USE_PROTOS\n");
			if ( HasComma(p->ret) )
			{
				fprintf(f, "extern struct _rv%d", i);
			}
			else
			{
				fprintf(f, "extern ");
				DumpType(p->ret, f);
			}
			fprintf(f, " %s%s(", RulePrefix, p->rname);
			DumpANSIFunctionArgDef(f,p);
			fprintf(f, ";\n");
#ifdef OLD
			if ( p->pdecl != NULL || GenAST )
			{
				if ( GenAST ) {
					fprintf(f, "AST **%s",(p->pdecl!=NULL)?",":"");
				}
				if ( p->pdecl!=NULL ) fprintf(f, "%s", p->pdecl);
			}
			else fprintf(f, "void");
			fprintf(f, ");\n");
#endif
			fprintf(f, "#else\n");
			if ( HasComma(p->ret) )
			{
				fprintf(f, "extern struct _rv%d", i);
			}
			else
			{
				fprintf(f, "extern ");
				DumpType(p->ret, f);
			}
			fprintf(f, " %s%s();\n", RulePrefix, p->rname);
			fprintf(f, "#endif\n");
		}
		else
		{
			fprintf(f, "\n#ifdef __USE_PROTOS\n");
			fprintf(f, "void %s%s(", RulePrefix, p->rname);
			DumpANSIFunctionArgDef(f,p);
			fprintf(f, ";\n");
#ifdef OLD
			if ( p->pdecl != NULL || GenAST )
			{
				if ( GenAST ) {
					fprintf(f, "AST **%s",(p->pdecl!=NULL)?",":"");
				}
				if ( p->pdecl!=NULL ) fprintf(f, "%s", p->pdecl);
			}
			else fprintf(f, "void");
			fprintf(f, ");\n");
#endif
			fprintf(f, "#else\n");
			fprintf(f, "extern void %s%s();\n", RulePrefix, p->rname);
			fprintf(f, "#endif\n");
		}
		i++;
		p = (Junction *)p->p2;
	}
}

/* Define all rules in the class.h file; generate any required
 * struct definitions first, however.
 */
void
#ifdef __USE_PROTOS
GenRuleMemberDeclarationsForCC( FILE *f, Junction *q )
#else
GenRuleMemberDeclarationsForCC( f, q )
FILE *f;
Junction *q;
#endif
{
	Junction *p = q;
	int i;

	fprintf(f, "private:\n");

	/* Dump dflt handler declaration */
	fprintf(f, "\tvoid zzdflthandlers( int _signal, int *_retsignal );\n\n");

	fprintf(f, "public:\n");

	/* Dump return value structs */
	i = 1;
	while ( p!=NULL )
	{
		if ( p->ret != NULL )
		{
			if ( HasComma(p->ret) )
			{
				DumpRetValStruct(f, p->ret, i);
			}
		}
		i++;
		p = (Junction *)p->p2;
	}

	/* Dump member func defs && CONSTRUCTOR */
	fprintf(f, "\t%s(ANTLRTokenBuffer *input);\n", CurrentClassName);
/*
	fprintf(f, "\t%s(ANTLRTokenBuffer *input, ANTLRTokenType eof);\n",
			   CurrentClassName);
*/

	i = 1;
	p = q;
	while ( p!=NULL )
	{
		if ( p->ret != NULL )
		{
			if ( HasComma(p->ret) )
			{
				fprintf(f, "\tstruct _rv%d", i);
			}
			else
			{
				fprintf(f, "\t");
				DumpType(p->ret, f);
			}
			fprintf(f, " %s(", p->rname);
			DumpANSIFunctionArgDef(f,p);
			fprintf(f, ";\n");
#ifdef OLD
			if ( p->pdecl != NULL || GenAST )
			{
				if ( GenAST ) fprintf(f, "ASTBase **%s",(p->pdecl!=NULL)?",":"");
				if ( p->pdecl!=NULL ) fprintf(f, "%s", p->pdecl);
			}
			fprintf(f, ");\n");
#endif
		}
		else
		{
			fprintf(f, "\tvoid %s(", p->rname);
			DumpANSIFunctionArgDef(f,p);
			fprintf(f, ";\n");
#ifdef OLD
			if ( p->pdecl != NULL || GenAST )
			{
				if ( GenAST ) fprintf(f, "ASTBase **%s",(p->pdecl!=NULL)?",":"");
				if ( p->pdecl!=NULL ) fprintf(f, "%s", p->pdecl);
			}
			fprintf(f, ");\n");
#endif
		}
		i++;
		p = (Junction *)p->p2;
	}
}

/* Given a list of ANSI-style parameter declarations, print out a
 * comma-separated list of the symbols (w/o types).
 * Basically, we look for a comma, then work backwards until start of
 * the symbol name.  Then print it out until 1st non-alnum char.  Now,
 * move on to next parameter.
 *
 */

/* MR5 	Jan Mikkelsen 26-May-97 - added initalComma parameter              */
void
#ifdef __USE_PROTOS
DumpListOfParmNames( char *pdecl, FILE *output, int initialComma )  /* MR5 */
#else
DumpListOfParmNames( pdecl, output, initialComma )		    /* MR5 */
char *pdecl;	             		 			  	    /* MR5 */
FILE *output;							                            /* MR5 */
int initialComma;			                                        /* MR5 */
#endif
{
	int firstTime = 1, done = 0;
	require(output!=NULL, "DumpListOfParmNames: NULL parm");

	if ( pdecl == NULL ) return;
	while ( !done )
	{
		if ( !firstTime || initialComma ) putc(',', output);        /* MR5 */
		done = DumpNextNameInDef(&pdecl, output);
		firstTime = 0;
	}
}

/* given a list of parameters or return values, dump the next
 * name to output.  Return 1 if last one just printed, 0 if more to go.
 */
int
#ifdef __USE_PROTOS
DumpNextNameInDef( char **q, FILE *output )
#else
DumpNextNameInDef( q, output )
char **q;
FILE *output;
#endif
{
	char *p = *q;		/* where did we leave off? */
	int done=0;

	while ( *p!='\0' && *p!=',' ) p++;		/* find end of decl */
	if ( *p == '\0' ) done = 1;
	while ( !isalnum(*p) && *p!='_' ) --p;	/* scan back until valid var character */
	while ( isalnum(*p) || *p=='_' ) --p;	/* scan back until beginning of variable */
	p++;						/* move to start of variable */
	while ( isalnum(*p) || *p=='_'  ) {putc(*p, output); p++;}
	while ( *p!='\0' && *p!=',' ) p++;		/* find end of decl */
	p++;				/* move past this parameter */

	*q = p;				/* record where we left off */
	return done;
}

/* Given a list of ANSI-style parameter declarations, dump K&R-style
 * declarations, one per line for each parameter.  Basically, convert
 * comma to semi-colon, newline.
 */
void
#ifdef __USE_PROTOS
DumpOldStyleParms( char *pdecl, FILE *output )
#else
DumpOldStyleParms( pdecl, output )
char *pdecl;
FILE *output;
#endif
{
	require(output!=NULL, "DumpOldStyleParms: NULL parm");

	if ( pdecl == NULL ) return;
	while ( *pdecl != '\0' )
	{
		if ( *pdecl == ',' )
		{
			pdecl++;
			putc(';', output); putc('\n', output);
			while ( *pdecl==' ' || *pdecl=='\t' || *pdecl=='\n' ) pdecl++;
		}
		else {putc(*pdecl, output); pdecl++;}
	}
	putc(';', output);
	putc('\n', output);
}

/* Take in a type definition (type + symbol) and print out type only */
void
#ifdef __USE_PROTOS
DumpType( char *s, FILE *f )
#else
DumpType( s, f )
char *s;
FILE *f;
#endif
{
	char *p, *end;
	require(s!=NULL, "DumpType: invalid type string");

	p = &s[strlen(s)-1];		/* start at end of string and work back */
	/* scan back until valid variable character */
	while ( !isalnum(*p) && *p!='_' ) --p;
	/* scan back until beginning of variable */
	while ( isalnum(*p) || *p=='_' ) --p;
	if ( p<=s )
	{
		warnNoFL(eMsg1("invalid parameter/return value: '%s'",s));
		return;
	}
	end = p;					/* here is where we stop printing alnum */
	p = s;
	while ( p!=end ) {putc(*p, f); p++;} /* dump until just before variable */
	while ( *p!='\0' )					 /* dump rest w/o variable */
	{
		if ( !isalnum(*p) && *p!='_' ) putc(*p, f);
		p++;
	}
}

/* check to see if string e is a word in string s */
int
#ifdef __USE_PROTOS
strmember( char *s, char *e )
#else
strmember( s, e )
char *s;
char *e;
#endif
{
    register char *p;
    require(s!=NULL&&e!=NULL, "strmember: NULL string");

    if ( *e=='\0' ) return 1;   /* empty string is always member */
    do {
	while ( *s!='\0' && !isalnum(*s) && *s!='_' )
	++s;
	p = e;
	while ( *p!='\0' && *p==*s ) {p++; s++;}
	if ( *p=='\0' ) {
	    if ( *s=='\0' ) return 1;
	    if ( !isalnum (*s) && *s != '_' ) return 1;
	}
	while ( isalnum(*s) || *s == '_' )
	++s;
    } while ( *s!='\0' );
    return 0;
}

int
#ifdef __USE_PROTOS
HasComma( char *s )
#else
HasComma( s )
char *s;
#endif
{
	while (*s!='\0')
		if ( *s++ == ',' ) return 1;
	return 0;
}

void
#ifdef __USE_PROTOS
DumpRetValStruct( FILE *f, char *ret, int i )
#else
DumpRetValStruct( f, ret, i )
FILE *f;
char *ret;
int i;
#endif
{
	fprintf(f, "\nstruct _rv%d {\n", i);
	while ( *ret != '\0' )
	{
		 while ( *ret==' ' || *ret=='\t' ) ret++; /* ignore white */
		 putc('\t', f);
		 while ( *ret!=',' && *ret!='\0' ) {putc(*ret,f); ret++;}
		 if ( *ret == ',' ) {putc(';', f); putc('\n', f); ret++;}
	}
	fprintf(f, ";\n};\n");
}

/* given "s" yield s -- DESTRUCTIVE (we modify s if starts with " else return s) */
char *
#ifdef __USE_PROTOS
StripQuotes( char *s )
#else
StripQuotes( s )
char *s;
#endif
{
	if ( *s == '"' )
	{
		s[ strlen(s)-1 ] = '\0';    /* remove last quote */
		return( s+1 );				/* return address past initial quote */
	}
	return( s );
}

