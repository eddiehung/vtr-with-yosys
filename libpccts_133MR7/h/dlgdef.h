/* dlgdef.h
 * Things in scanner produced by dlg that should be visible to the outside
 * world
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

#ifndef ZZDLGDEF_H
#define ZZDLGDEF_H

#include "config.h"

#ifndef zzchar_t
#ifdef ZZWCHAR_T
#define zzchar_t wchar_t
#else
#define zzchar_t char
#endif
#endif

struct zzdlg_state {
	FILE *stream;
	int (*func_ptr)();
	zzchar_t *str;
	int auto_num;
	int add_erase;
	int lookc;
	int char_full;
	int begcol, endcol;
	int line;
	zzchar_t *lextext, *begexpr, *endexpr;
	int bufsize;
	int bufovf;
	zzchar_t *nextpos;
	int	class_num;
};

extern zzchar_t	*zzlextext;  	/* text of most recently matched token */
extern zzchar_t	*zzbegexpr;	/* beginning of last reg expr recogn. */
extern zzchar_t	*zzendexpr;	/* beginning of last reg expr recogn. */
extern int	zzbufsize;	/* how long zzlextext is */
extern int	zzbegcol;	/* column that first character of token is in*/
extern int	zzendcol;	/* column that last character of token is in */
extern int	zzline;		/* line current token is on */
extern int	zzreal_line;		/* line of 1st portion of token that is not skipped */
extern int	zzchar;		/* character to determine next state */
extern int	zzbufovf;	/* indicates that buffer too small for text */
#ifdef __USE_PROTOS
extern void	(*zzerr)(const char *);/* pointer to error reporting function */
#else
extern void	(*zzerr)();
#endif

#ifdef USER_ZZMODE_STACK
extern int     zzauto;
#endif

#ifdef __USE_PROTOS
extern void	zzadvance(void);
extern void	zzskip(void);	/* erase zzlextext, look for antoher token */
extern void	zzmore(void);	/* keep zzlextext, look for another token */
extern void	zzmode(int k);	/* switch to automaton 'k' */
extern void	zzrdstream(FILE *);/* what stream to read from */
extern void	zzclose_stream(void);/* close the current input stream */
extern void	zzrdfunc(int (*)());/* what function to get char from */
extern void zzrdstr( zzchar_t * );
extern void	zzgettok(void);	/* get next token */
extern void	zzreplchar(zzchar_t c);/* replace last recognized reg. expr. with
					a character */
extern void	zzreplstr(zzchar_t *s);/* replace last recognized reg. expr. with
					a string */
extern void zzsave_dlg_state(struct zzdlg_state *);
extern void zzrestore_dlg_state(struct zzdlg_state *);
extern int zzerr_in(void);
extern void	zzerrstd(const char *);
extern void zzerraction();

#else

extern void	zzadvance();
extern void	zzskip();	/* erase zzlextext, look for antoher token */
extern void	zzmore();	/* keep zzlextext, look for another token */
extern void	zzmode(/*k*/);	/* switch to automaton 'k' */
extern void	zzrdstream();	/* what stream to read from */
extern void	zzclose_stream();/* close the current input stream */
extern void	zzrdfunc();	/* what function to get char from */
extern void zzrdstr();
extern void	zzgettok();	/* get next token */
extern void	zzreplchar();	/* replace last recognized reg. expr. with
					a character */
extern void	zzreplstr();	/* replace last recognized reg. expr. with
					a string */
extern void zzsave_dlg_state();
extern void zzrestore_dlg_state();
extern int zzerr_in();
extern void	zzerrstd();
extern void zzerraction();
#endif

#endif
