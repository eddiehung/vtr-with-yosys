/* antlr.h
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
#ifndef ANTLR_H
#define ANTLR_H

#include "config.h"

/*
 * Define all of the stack setup and manipulation of $i, #i variables.
 *
 *	Notes:
 *		The type 'Attrib' must be defined before entry into this .h file.
 */

#ifdef __USE_PROTOS
#include <stdlib.h>
#else
#ifdef VAXC
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#endif
#include <string.h>

typedef int ANTLRTokenType;
typedef unsigned char SetWordType;

typedef char ANTLRChar;

						/* G u e s s  S t u f f */

#ifdef ZZCAN_GUESS
#ifndef ZZINF_LOOK
#define ZZINF_LOOK
#endif
#endif

#ifdef ZZCAN_GUESS
typedef struct _zzjmp_buf {
			jmp_buf state;
		} zzjmp_buf;
#endif


/* can make this a power of 2 for more efficient lookup */
#ifndef ZZLEXBUFSIZE
#define ZZLEXBUFSIZE	2000
#endif

#define zzOvfChk														\
            if ( zzasp <= 0 )                                           \
            {                                                           \
                fprintf(stderr, zzStackOvfMsg, __FILE__, __LINE__);		\
                exit(PCCTS_EXIT_FAILURE);                                               \
            }

#ifndef ZZA_STACKSIZE
#define ZZA_STACKSIZE	400
#endif
#ifndef ZZAST_STACKSIZE
#define ZZAST_STACKSIZE	400
#endif

#ifndef zzfailed_pred
#define zzfailed_pred(_p)	\
	fprintf(stderr, "semantic error; failed predicate: '%s'\n",_p)
#endif

#ifdef LL_K
#define LOOKAHEAD												\
	int zztokenLA[LL_K];										\
	char zztextLA[LL_K][ZZLEXBUFSIZE];							\
	int zzlap = 0, zzlabase=0; /* labase only used for DEMAND_LOOK */
#else
#define LOOKAHEAD												\
	int zztoken;
#endif

#ifndef zzcr_ast
#define zzcr_ast(ast,attr,tok,text)
#endif

#ifdef DEMAND_LOOK
#define DemandLookData  int zzdirty=1;
#else
#define DemandLookData
#endif

						/* S t a t e  S t u f f */

#ifdef ZZCAN_GUESS
#define zzGUESS_BLOCK		zzantlr_state zzst; int zzrv;
#define zzGUESS				zzsave_antlr_state(&zzst); \
							zzguessing = 1; \
							zzrv = setjmp(zzguess_start.state);
#define zzGUESS_FAIL		longjmp(zzguess_start.state, 1)
#define zzGUESS_DONE		zzrestore_antlr_state(&zzst);
#define zzNON_GUESS_MODE	if ( !zzguessing )
#define zzGuessData                                     \
            zzjmp_buf zzguess_start;                    \
            int zzguessing;
#else
#define zzGUESS_BLOCK
#define zzGUESS
#define zzGUESS_FAIL
#define zzGUESS_DONE
#define zzNON_GUESS_MODE
#define zzGuessData
#endif

typedef struct _zzantlr_state {
#ifdef ZZCAN_GUESS
			zzjmp_buf guess_start;
			int guessing;
#endif
			int asp;
			int ast_sp;
#ifdef ZZINF_LOOK
			int inf_lap;	/* not sure we need to save this one */
			int inf_labase;
			int inf_last;

/* MR6 	Gunnar Rxnning (gunnar@candleweb.no)                                */
/* MR6	  Additional state needs to be saved/restored                       */
/* MR6    Matching changes in err.h                                         */

			int *inf_tokens;	                                     /* MR6 */
			char **inf_text;	                                     /* MR6 */
			char *inf_text_buffer;		                             /* MR6 */
			int *inf_line;		                                     /* MR6 */
#endif
#ifdef DEMAND_LOOK
			int dirty;
#endif

#ifdef LL_K
			int tokenLA[LL_K];
			char textLA[LL_K][ZZLEXBUFSIZE];
			int lap;
			int labase;
#else
			int token;
			char text[ZZLEXBUFSIZE];
#endif
		} zzantlr_state;


                 /* I n f i n i t e  L o o k a h e a d */


#ifdef ZZINF_LOOK
#define InfLookData	\
	int *zzinf_tokens;	\
	char **zzinf_text;	\
	char *zzinf_text_buffer;	\
	int *zzinf_line;        \
	int zzinf_labase;	\
	int zzinf_last;
#else
#define InfLookData
#endif

#ifdef ZZINF_LOOK

#ifndef ZZINF_DEF_TEXT_BUFFER_SIZE
#define ZZINF_DEF_TEXT_BUFFER_SIZE	    20000
#endif
#ifndef ZZINF_DEF_TOKEN_BUFFER_SIZE
#define ZZINF_DEF_TOKEN_BUFFER_SIZE	    2000
#endif
/* WARNING!!!!!!
 * ZZINF_BUFFER_TEXT_CHUNK_SIZE must be > sizeof(text) largest possible token.
 */
#ifndef ZZINF_BUFFER_TEXT_CHUNK_SIZE
#define ZZINF_BUFFER_TEXT_CHUNK_SIZE	5000
#endif
#ifndef ZZINF_BUFFER_TOKEN_CHUNK_SIZE
#define ZZINF_BUFFER_TOKEN_CHUNK_SIZE	1000
#endif

#if ZZLEXBUFSIZE > ZZINF_BUFFER_TEXT_CHUNK_SIZE
#define ZZINF_BUFFER_TEXT_CHUNK_SIZE	ZZLEXBUFSIZE+5
#endif

/* make inf_look user-access macros */
#ifdef LL_K
#define ZZINF_LA_VALID(i)	(((zzinf_labase+i-1)-LL_K+1) <= zzinf_last)
#define ZZINF_LA(i)			zzinf_tokens[(zzinf_labase+i-1)-LL_K+1]
#define ZZINF_LATEXT(i)		zzinf_text[(zzinf_labase+i-1)-LL_K+1]
/* MR6	In 1.33 vanilla the #define ZZINF_LINE(i) is was commented out	*/
#define ZZINF_LINE(i)       zzinf_line[(zzinf_labase+i-1)-LL_K+1]
#else
#define ZZINF_LA_VALID(i)	(((zzinf_labase+i-1)) <= zzinf_last)
#define ZZINF_LA(i)			zzinf_tokens[(zzinf_labase+i-1)]
#define ZZINF_LATEXT(i)		zzinf_text[(zzinf_labase+i-1)]
#endif

#define inf_zzgettok _inf_zzgettok()
extern void _inf_zzgettok();

#endif	/* ZZINF_LOOK */


#ifdef LL_K

#ifdef __USE_PROTOS
#define ANTLR_INFO												\
	Attrib zzempty_attr() {static Attrib a; return a;}		   \
	Attrib zzconstr_attr(int _tok, char *_text)			   \
		{Attrib a; zzcr_attr((&a),_tok,_text); return a;}	   \
	int zzasp=ZZA_STACKSIZE;					   \
	char zzStackOvfMsg[]="fatal: attrib/AST stack overflow %s(%d)!\n"; \
	Attrib zzaStack[ZZA_STACKSIZE]; DemandLookData			   \
	InfLookData                                                        \
    zzGuessData
#else
#define ANTLR_INFO												\
	Attrib zzempty_attr() {static Attrib a; return a;}		    \
	Attrib zzconstr_attr(_tok, _text) int _tok; char *_text;            \
		{Attrib a; zzcr_attr((&a),_tok,_text); return a;}	    \
	int zzasp=ZZA_STACKSIZE;					    \
	char zzStackOvfMsg[]="fatal: attrib/AST stack overflow %s(%d)!\n";  \
	Attrib zzaStack[ZZA_STACKSIZE]; DemandLookData			    \
	InfLookData                                                         \
    zzGuessData
#endif

#else

#ifdef __USE_PROTOS
#define ANTLR_INFO												\
	Attrib zzempty_attr() {static Attrib a; return a;}			\
	Attrib zzconstr_attr(int _tok, char *_text)				\
		{Attrib a; zzcr_attr((&a),_tok,_text); return a;}		\
	int zzasp=ZZA_STACKSIZE;						\
	char zzStackOvfMsg[]="fatal: attrib/AST stack overflow %s(%d)!\n";      \
	Attrib zzaStack[ZZA_STACKSIZE]; DemandLookData				\
	InfLookData                                                             \
    zzGuessData
#else
#define ANTLR_INFO												\
	Attrib zzempty_attr() {static Attrib a; return a;}			\
	Attrib zzconstr_attr(_tok, _text) int _tok; char *_text;                \
		{Attrib a; zzcr_attr((&a),_tok,_text); return a;}		\
	int zzasp=ZZA_STACKSIZE;						\
	char zzStackOvfMsg[]="fatal: attrib/AST stack overflow %s(%d)!\n";      \
	Attrib zzaStack[ZZA_STACKSIZE]; DemandLookData				\
	InfLookData                                                             \
    zzGuessData
#endif
	
#endif /* LL_k */


#ifdef ZZINF_LOOK

#ifdef LL_K
#ifdef DEMAND_LOOK
#define zzPrimeLookAhead  {zzdirty=LL_K; zzlap = zzlabase = 0;}
#else
#define zzPrimeLookAhead  {zzlap = zzlabase = 0; zzfill_inf_look();\
						  {int _i;  for(_i=1;_i<=LL_K; _i++)		\
										{zzCONSUME;} zzlap = zzlabase = 0;}}
#endif

#else /* LL_K */

#ifdef DEMAND_LOOK
#define zzPrimeLookAhead  zzfill_inf_look(); zzdirty=1
#else
#define zzPrimeLookAhead  zzfill_inf_look(); inf_zzgettok

#endif
#endif	/* LL_K */

#else	/* ZZINF_LOOK */

#ifdef LL_K
#ifdef DEMAND_LOOK
#define zzPrimeLookAhead  {zzdirty=LL_K; zzlap = zzlabase = 0;}
#else
#define zzPrimeLookAhead  {int _i; zzlap = 0; for(_i=1;_i<=LL_K; _i++)		\
										{zzCONSUME;} zzlap = 0;}
#endif

#else

#ifdef DEMAND_LOOK
#define zzPrimeLookAhead  zzdirty=1
#else
#define zzPrimeLookAhead  zzgettok()
#endif
#endif	/* LL_K */

#endif	/* ZZINF_LOOK */


#ifdef LL_K
#define zzenterANTLRs(s)                            \
        zzlextext = &(zztextLA[0][0]); zzrdstr( s ); zzPrimeLookAhead;
#define zzenterANTLRf(f)							\
		zzlextext = &(zztextLA[0][0]); zzrdfunc( f ); zzPrimeLookAhead;
#define zzenterANTLR(f)							\
		zzlextext = &(zztextLA[0][0]); zzrdstream( f ); zzPrimeLookAhead;
#ifdef ZZINF_LOOK
#define zzleaveANTLR(f)			free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#define zzleaveANTLRf(f)	   	free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#define zzleaveANTLRs(f)		free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#else
#define zzleaveANTLR(f)
#define zzleaveANTLRf(f)
#define zzleaveANTLRs(f)
#endif

#else

#define zzenterANTLRs(s)                            \
        {static char zztoktext[ZZLEXBUFSIZE];   \
        zzlextext = zztoktext; zzrdstr( s ); zzPrimeLookAhead;}
#define zzenterANTLRf(f)							\
		{static char zztoktext[ZZLEXBUFSIZE];	\
		zzlextext = zztoktext; zzrdfunc( f ); zzPrimeLookAhead;}
#define zzenterANTLR(f)							\
		{static char zztoktext[ZZLEXBUFSIZE];	\
		zzlextext = zztoktext; zzrdstream( f ); zzPrimeLookAhead;}
#ifdef ZZINF_LOOK
#define zzleaveANTLR(f)			free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#define zzleaveANTLRf(f)	   	free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#define zzleaveANTLRs(f)		free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#else
#define zzleaveANTLR(f)
#define zzleaveANTLRf(f)
#define zzleaveANTLRs(f)
#endif

#endif

#define ANTLR(st, f)	zzbufsize = ZZLEXBUFSIZE;	\
						zzenterANTLR(f);			\
						st; ++zzasp;				\
						zzleaveANTLR(f);
						
#define ANTLRm(st, f, _m)	zzbufsize = ZZLEXBUFSIZE;	\
						zzmode(_m);				\
						zzenterANTLR(f);			\
						st; ++zzasp;				\
						zzleaveANTLR(f);
						
#define ANTLRf(st, f)	zzbufsize = ZZLEXBUFSIZE;	\
						zzenterANTLRf(f);			\
						st; ++zzasp;				\
						zzleaveANTLRf(f);

#define ANTLRs(st, s)   zzbufsize = ZZLEXBUFSIZE;	\
                        zzenterANTLRs(s);           \
                        st; ++zzasp;				\
                        zzleaveANTLRs(s);

#ifdef LL_K
#define zztext		(&(zztextLA[zzlap][0]))
#else
#define zztext		zzlextext
#endif


					/* A r g u m e n t  A c c e s s */

#define zzaCur			(zzaStack[zzasp])
#define zzaRet			(*zzaRetPtr)
#define zzaArg(v,n)		zzaStack[v-n]
#define zzMakeAttr		{ zzNON_GUESS_MODE {zzOvfChk; --zzasp; zzcr_attr(&(zzaStack[zzasp]),LA(1),LATEXT(1));}}
#ifdef zzdef0
#define zzMake0			{ zzOvfChk; --zzasp; zzdef0(&(zzaStack[zzasp]));}
#else
#define zzMake0			{ zzOvfChk; --zzasp;}
#endif
#define zzaPush(_v)		{ zzOvfChk; zzaStack[--zzasp] = _v;}
#ifndef zzd_attr
#define zzREL(t)		zzasp=(t);		/* Restore state of stack */
#else
#define zzREL(t)		for (; zzasp<(t); zzasp++)				\
						{ zzd_attr(&(zzaStack[zzasp])); }
#endif

#define zzsetmatch(_es)						\
	if ( !_zzsetmatch(_es, &zzBadText, &zzMissText, &zzMissTok, &zzBadTok, &zzMissSet) ) goto fail;
#define zzsetmatch_wsig(_es, handler)		\
	if ( !_zzsetmatch_wsig(_es) ) {_signal=MismatchedToken; goto handler;}

#ifdef __USE_PROTOS
extern int _zzsetmatch(SetWordType *, char **, char **, int *, int *, SetWordType **);
extern int _zzsetmatch_wsig(SetWordType *);
#else
extern int _zzsetmatch();
extern int _zzsetmatch_wsig();
#endif

#define zzmatch(_t)							\
	if ( !_zzmatch(_t, &zzBadText, &zzMissText, &zzMissTok, &zzBadTok, &zzMissSet) ) goto fail;
#define zzmatch_wsig(_t,handler)			\
	if ( !_zzmatch_wsig(_t) ) {_signal=MismatchedToken; goto handler;}

#ifdef __USE_PROTOS
extern int _zzmatch(int, char **, char **, int *, int *, SetWordType **);
extern int _zzmatch_wsig(int);
#else
extern int _zzmatch();
extern int _zzmatch_wsig();
#endif

#define zzmatch_wdfltsig(_t,_f)			\
	if ( !_zzmatch_wdfltsig(_t,_f) ) _signal=MismatchedToken;
#define zzsetmatch_wdfltsig(tw,tt,wf)		\
	if ( !_zzsetmatch_wdfltsig(tw,tt,wf) ) _signal=MismatchedToken;

#ifdef __USE_PROTOS
extern int _zzmatch_wdfltsig(int, SetWordType *);
extern int _zzsetmatch_wdfltsig(SetWordType *tokensWanted,
								int tokenTypeOfSet,
								SetWordType *whatFollows);
#else
extern int _zzmatch_wdfltsig();
extern int _zzsetmatch_wdfltsig();
#endif

#ifdef GENAST
#define zzRULE		Attrib *zzaRetPtr = &(zzaStack[zzasp-1]);	\
					SetWordType *zzMissSet=NULL; int zzMissTok=0;		\
					int zzBadTok=0; char *zzBadText="";		\
					int zzErrk=1;								\
					char *zzMissText=""; zzASTVars
#else
#define zzRULE		Attrib *zzaRetPtr = &(zzaStack[zzasp-1]);	\
					int zzBadTok=0; char *zzBadText="";		\
					int zzErrk=1;								\
					SetWordType *zzMissSet=NULL; int zzMissTok=0; char *zzMissText=""
#endif

#ifdef GENAST
#define zzBLOCK(i)	int i = zzasp - 1; int zztsp = zzast_sp
#define zzEXIT(i)	zzREL(i); zzastREL; zzNON_GUESS_MODE { zzastPush(*_root); }
#define zzLOOP(i)	zzREL(i); zzastREL
#else
#define zzBLOCK(i)	int i = zzasp - 1
#define zzEXIT(i)	zzREL(i)
#define zzLOOP(i)	zzREL(i)
#endif

#ifdef LL_K

#ifdef DEMAND_LOOK
#define LOOK(_k)	{int i,stop=_k-(LL_K-zzdirty); for (i=1; i<=stop; i++)	\
					zzCONSUME;}
#define zzCONSUME	{zzgettok(); zzdirty--;							\
					zzlap = (zzlap+1)&(LL_K-1);						\
					zzlextext = &(zztextLA[zzlap][0]);}
#else
#ifdef ZZINF_LOOK
#define zzCONSUME	{inf_zzgettok; 									\
					zzlap = (zzlap+1)&(LL_K-1);						\
					zzlextext = &(zztextLA[zzlap][0]);				\
					}
#else
#define zzCONSUME	{zzgettok(); 									\
					zzlap = (zzlap+1)&(LL_K-1);						\
					zzlextext = &(zztextLA[zzlap][0]);}
#endif /* ZZINF_LOOK */
#endif /* DEMAND_LOOK */

#else /* LL_K */

#ifdef DEMAND_LOOK
#define LOOK(_k)	if ( zzdirty) zzCONSUME;
#ifdef ZZINF_LOOK
#define zzCONSUME	inf_zzgettok; zzdirty=0;
#else
#define zzCONSUME	zzgettok(); zzdirty=0;
#endif /* ZZINF_LOOK */

#else  /* DEMAND_LOOK */

#ifdef ZZINF_LOOK
#define zzCONSUME	inf_zzgettok
#else
#define zzCONSUME	zzgettok();
#endif

#endif /* DEMAND_LOOK */

#endif /* LL_K */

#ifdef LL_K
#define NLA			zztokenLA[zzlap&(LL_K-1)]	/* --> next LA */
#define NLATEXT		zztextLA[zzlap&(LL_K-1)]	/* --> next text of LA */
#ifdef DEMAND_LOOK
#define LA(i)       zztokenLA[(zzlabase+(i)-1)&(LL_K-1)]
#define LATEXT(i)   (&(zztextLA[(zzlabase+(i)-1)&(LL_K-1)][0]))
#else
#define LA(i)       zztokenLA[(zzlap+(i)-1)&(LL_K-1)]
#define LATEXT(i)   (&(zztextLA[(zzlap+(i)-1)&(LL_K-1)][0]))
#endif
#else
#define NLA			zztoken
#define NLATEXT		zztext
#define LA(i)       zztoken
#define LATEXT(i)   zztext
#endif


           /* S t a n d a r d  S i g n a l s */

#define NoSignal			0
#define MismatchedToken		1
#define NoViableAlt			2
#define NoSemViableAlt		3

/* MR7  Allow more control over signalling                                  */
/*        by adding "Unwind" and "zzsetSignal"                              */

#define Unwind              4
#define zzsetSignal(newValue) *_retsignal=_signal=(newValue)
#define zzsuppressSignal *_retsignal=_signal=0
#define zzexportSignal    *_retsignal=_signal

           /* F u n c t i o n  T r a c i n g */

#ifndef zzTRACEIN
#define zzTRACEIN(r)	fprintf(stderr, "enter rule \"%s\"\n", r);
#endif
#ifndef zzTRACEOUT
#define zzTRACEOUT(r)	fprintf(stderr, "exit rule \"%s\"\n", r);
#endif

#ifdef ZZWCHAR_T
#define zzchar_t wchar_t
#else
#define zzchar_t char
#endif

				/* E x t e r n  D e f s */

#ifdef __USE_PROTOS
extern Attrib zzempty_attr();
extern Attrib zzconstr_attr(int, char *);
extern void zzsyn(char *, int, char *, SetWordType *, int, int, char *);
extern int zzset_el(unsigned, SetWordType *);
extern int zzset_deg(SetWordType *);
extern void zzedecode(SetWordType *);
extern void zzFAIL(int k, ...);
extern void zzresynch(SetWordType *, SetWordType);
extern void zzsave_antlr_state(zzantlr_state *);
extern void zzrestore_antlr_state(zzantlr_state *);
extern void zzfill_inf_look(void);
extern void zzconsumeUntil(SetWordType *st);                         /* MR7 */
extern void zzconsumeUntilToken(int t);                              /* MR7 */
#ifdef EXCEPTION_HANDLING
extern void zzdflthandlers(int, int *);
#endif
#else
extern Attrib zzempty_attr();
extern Attrib zzconstr_attr();
extern void zzsyn();
extern int zzset_el();
extern int zzset_deg();
extern void zzedecode();
extern void zzFAIL();
extern void zzresynch();
extern void zzsave_antlr_state();
extern void zzrestore_antlr_state();
extern void zzfill_inf_look();
extern void zzconsumeUntil();                                        /* MR7 */
extern void zzconsumeUntilToken();                                   /* MR7 */
#ifdef EXCEPTION_HANDLING
extern void zzdflthandlers();
#endif
#endif

				/* G l o b a l  V a r i a b l e s */

/* Define a parser; user should do a "#parser myname" in their grammar file */
/*extern struct pccts_parser zzparser;*/

extern char *zztokens[];
#ifdef LL_K
extern int zztokenLA[];
extern char zztextLA[][ZZLEXBUFSIZE];
extern int zzlap;
extern int zzlabase;
#else
extern int zztoken;
#endif

extern char zzStackOvfMsg[];
extern int zzasp;
extern Attrib zzaStack[];
#ifdef ZZINF_LOOK
extern int *zzinf_tokens;
extern char **zzinf_text;
extern char *zzinf_text_buffer;
extern int *zzinf_line;
extern int zzinf_labase;
extern int zzinf_last;
#endif
#ifdef DEMAND_LOOK
extern int zzdirty;
#endif
#ifdef ZZCAN_GUESS
extern int zzguessing;
extern zzjmp_buf zzguess_start;
#endif

/* Define global veriables that refer to values exported by the scanner.
 * These declarations duplicate those in dlgdef.h, but are needed
 * if ANTLR is not to generate a .dlg file (-gx); PS, this is a hack.
 */
extern zzchar_t *zzlextext;     /* text of most recently matched token */
extern int      zzbufsize;      /* how long zzlextext is */

#endif
