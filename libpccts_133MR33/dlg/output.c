/* output.c, output generator for dlg
 *
 * Output Notes:
 *
 * DfaStates == number of dfa nodes in automaton (just a #define)
 * DfaState == type large enough to index every node in automaton
 *         <256 unsigned char, <65536 unsigned short, etc.
 *
 * Thus, the elements in each of the automaton states (st%d) are type DfaState
 * and are size appropriately, since they must be able to index the next
 * automaton state.
 *
 * dfa[] == a linear array that points to all the automaton states (st%d)
 *         (dfa_base[] should be the same, but isn't right now)
 *
 * accepts[] == Taking a closer look at this one, it probably shouldn't be type
 *         DfaState because there is no real requirement that the number of
 *         accepts states is less than the number of dfa state.  However, if
 *         the number of accept states was more than the number of DFA states
 *         then the lexical specification would be really ambiguous.
 *
 *         Another note. Is that is should be possible to fold accepts[] and
 *         actions[] together.  If this is done, I would suggest get rid of
 *         accept[] and make actions[] have an entry for each state (st%d) in
 *         the automaton.
 *
 * dfa_base[] == starting location for each lexical mode.  This should be
 *         Dfastate type (but isn't right now), since it points to the states
 *         in the automaton.
 *
 * dfa_class_no[] == indicates the number of columns each lexical mode has.
 *
 * b_class_no[] == pointer to the start of the translation array used to
 *         convert from input character to character class.  This could cause
 *         problems if there are more than 256 classes
 *
 * shift%d[] == the actual translation arrays that convert the input character
 *         into the character class.  These will have to change if there are
 *         more than 256 character classes.
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
 * DLG 1.33
 * Will Cohen
 * With mods by Terence Parr; AHPCRC, University of Minnesota
 * 1989-1995
 */

#include <stdio.h>
#include <string.h>
#include "dlg.h"
#ifdef MEMCHK
#include "trax.h"
#else
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif /* __STDC__ */
#endif

static char *mode_name[MAX_MODES];
static int mode_number[MAX_MODES];
static int cur_mode=0;

int operation_no = 0; /* used to mark nodes so that infinite loops avoided */
int dfa_basep[MAX_MODES]; 	/* start of each group of states */
int dfa_class_nop[MAX_MODES];	/* number of elements in each group of states*/

int gen_ansi = FALSE;		/* allows ansi code to be generated */

FILE *input_stream;	/* where to read description from */
FILE *output_stream;	/* where to put the output	  */
FILE *mode_stream;	/* where to put the mode.h stuff */
FILE *class_stream;	/* where to put the scan.h stuff (if gen_cpp) */

/* NOTE: This section is MACHINE DEPENDENT */
#define DIF_SIZE 4
#if defined(PC) && !defined(PC32)
long typesize[DIF_SIZE]  = { 0x7f, 0x7fff, 0x7fff, 0x7fffffff };
char t0[] = "unsigned char";
char t1[] = "unsigned short";
char t2[] = "unsigned int";
char t3[] = "unsigned long";
char *typevar[DIF_SIZE] = { t0, t1, t2, t3};
#else
long typesize[DIF_SIZE]  = { 0x7f, 0x7fff, 0x7fffffff, 0x7fffffff };
char t0[] = "unsigned char";
char t1[] = "unsigned short";
char t2[] = "unsigned int";
char t3[] = "unsigned long";
char *typevar[DIF_SIZE] = { t0, t1, t2, t3};
#endif

/* Added by TJP August 1994 */
/* Take in MyLexer and return MyLexer_h */
static char *
#ifdef __USE_PROTOS
gate_symbol(char *name)
#else
gate_symbol(name)
char *name;
#endif
{
	static char buf[100];
	sprintf(buf, "%s_h", name);
	return buf;
}

/* Added by TJP August 1994 */
static char *
#ifdef __USE_PROTOS
mystrdup(char *s)
#else
mystrdup(s)
char *s;
#endif
{
	char *p = (char *)malloc(strlen(s)+1);
	strcpy(p, s);
	return p;
}

void p_class_hdr()						
{
	if ( class_stream == NULL ) return;
	fprintf(class_stream, "#ifndef %s\n", gate_symbol(ClassName("")));
	fprintf(class_stream, "#define %s\n", gate_symbol(ClassName("")));
	fprintf(class_stream, "/*\n");
	fprintf(class_stream, " * D L G L e x e r  C l a s s  D e f i n i t i o n\n");
	fprintf(class_stream, " *\n");
	fprintf(class_stream, " * Generated from:");
	fprintf(class_stream, " %s", file_str[0]);
	fprintf(class_stream, "\n");
	fprintf(class_stream, " *\n");
	fprintf(class_stream, " * 1989-1994 by  Will Cohen, Terence Parr, and Hank Dietz\n");
	fprintf(class_stream, " * Purdue University Electrical Engineering\n");
	fprintf(class_stream, " * DLG Version %s\n", version);
	fprintf(class_stream, " */\n\n");
	fprintf(class_stream, "\n");
	fprintf(class_stream, "#include \"%s\"\n", DLEXERBASE_H);
}

/* MR1									*/
/* MR1 16-Apr-97  Split printing of class header up into several parts  */
/* MR1		    so that #lexprefix <<...>>and #lexmember <<...>>	*/
/* MR1		    can be inserted in the appropriate spots		*/
/* MR1									*/

void p_class_def1()
{
	if ( class_stream == NULL ) return;
	fprintf(class_stream, "\nclass %s : public DLGLexerBase {\n", ClassName(""));
	fprintf(class_stream, "public:\n");
}

void p_class_def2()
{
	int i, m;
	if ( class_stream == NULL ) return;
	fprintf(class_stream, "public:\n");
	fprintf(class_stream, "\tstatic const int MAX_MODE;\n");
	fprintf(class_stream, "\tstatic const int DfaStates;\n");
	for (i=0; i<cur_mode; i++) {
		fprintf(class_stream, "\tstatic const int %s;\n", mode_name[i]);
	}

	fprintf(class_stream, "\ttypedef %s DfaState;\n\n", minsize(dfa_allocated));
	fprintf(class_stream, "\t%s(DLGInputStream *in,\n",ClassName(""));

/*  06/09/03  jkyl  : TR47713 - Increase bufsize from 2000 to 5000 */

	fprintf(class_stream, "\t\tunsigned bufsize=5000)\n");
	fprintf(class_stream, "\t\t: DLGLexerBase(in, bufsize, %d)\n", interactive);
	fprintf(class_stream, "\t{\n");
	fprintf(class_stream, "\t;\n");
	fprintf(class_stream, "\t}\n");
	fprintf(class_stream, "\tvoid	mode(int);\n");
	fprintf(class_stream, "\tANTLRTokenType nextTokenType(void);\n");
	fprintf(class_stream, "\tvoid	advance(void);\n");

	fprintf(class_stream, "protected:\n");
	for (i=1; i<=action_no; ++i) {
		fprintf(class_stream, "\tANTLRTokenType act%d();\n", i);
	}

	for(m=0; m<(mode_counter-1); ++m){
		for(i=dfa_basep[m]; i<dfa_basep[m+1]; ++i)
			fprintf(class_stream, "\tstatic DfaState st%d[%d];\n", i-1, dfa_class_nop[m]+1);
	}
	for(i=dfa_basep[m]; i<=dfa_allocated; ++i)
		fprintf(class_stream, "\tstatic DfaState st%d[%d];\n", i-1, dfa_class_nop[m]+1);

	fprintf(class_stream, "\tstatic DfaState *dfa[%d];\n", dfa_allocated);
	fprintf(class_stream, "\tstatic DfaState dfa_base[];\n");
/*	fprintf(class_stream, "\tstatic int dfa_base_no[];\n"); */
	fprintf(class_stream, "\tstatic unsigned char *b_class_no[];\n");
	fprintf(class_stream, "\tstatic DfaState accepts[%d];\n",dfa_allocated+1);
	fprintf(class_stream, "\tstatic DLGChar alternatives[%d];\n",dfa_allocated+1);
	/* WARNING: should be ANTLRTokenType for action table, but g++ 2.5.6 is hosed */
	fprintf(class_stream, "\tstatic ANTLRTokenType (%s::*actions[%d])();\n", ClassName(""), action_no+1);
	for(m=0; m<mode_counter; ++m) {
		fprintf(class_stream, "\tstatic unsigned char shift%d[%d];\n",
			m, CHAR_RANGE);
	}
	if (comp_level)
		fprintf(class_stream, "\tint ZZSHIFT(int c) { return b_class_no[automaton][1+c]; }\n");
	else
		fprintf(class_stream, "\tint ZZSHIFT(int c) { return 1+c; }\n");

/* MR1									  */
/* MR1 11-APr-97   Kludge to allow inclusion of user-defined code in	  */
/* MR1			 DLGLexer class header				  */
/* MR1		   Deprecated in favor of 133MR1 addition #lexmember <<>> */
/* MR1									  */
/* MR1 */	fprintf(class_stream,"//\n");
/* MR1 */	fprintf(class_stream,
/* MR1 */	   "// 133MR1 Deprecated feature to allow inclusion of ");
/* MR1 */	fprintf(class_stream,
/* MR1 */	   "user-defined code in DLG class header\n");
/* MR1 */	fprintf(class_stream,"//\n");
/* MR1 */
/* MR1 */	fprintf(class_stream,"#ifdef DLGLexerIncludeFile\n");
/* MR1 */	fprintf(class_stream,"#include DLGLexerIncludeFile\n");
/* MR1 */	fprintf(class_stream,"#endif\n");

	fprintf(class_stream, "};\n");

	fprintf(class_stream, "typedef ANTLRTokenType (%s::*Ptr%sMemberFunc)();\n",
			ClassName(""), ClassName(""));

	fprintf(class_stream, "#endif\n");
}

/* generate required header on output */

void p_head()
{
	fprintf(OUT, "/*\n");
	fprintf(OUT, " * D L G tables\n");
	fprintf(OUT, " *\n");
	fprintf(OUT, " * Generated from:");
	fprintf(OUT, " %s", file_str[0]);
	fprintf(OUT, "\n");
	fprintf(OUT, " *\n");
	fprintf(OUT, " * 1989-1994 by  Will Cohen, Terence Parr, and Hank Dietz\n");
	fprintf(OUT, " * Purdue University Electrical Engineering\n");
	fprintf(OUT, " * DLG Version %s\n", version);
	fprintf(OUT, " */\n\n");
	if ( gen_cpp ) fprintf(OUT, "#include <stdio.h>\n");
	if ( !gen_cpp ) fprintf(OUT, "#include \"%s\"\n\n", mode_file);
	fprintf(OUT,"\n");
}

void p_includes()
{
	int i;

	fprintf(OUT, "#include \"%s\"\n", APARSER_H);
	fprintf(OUT, "#include \"%s\"\n", DLEXERBASE_H);
	fprintf(OUT, "#include \"%s\"\n", ClassName(".h"));
}

/* generate code to tie up any loose ends */

void p_tail()						/* MR1 */
{
	if ( gen_cpp ) {
		if ( strcmp(ClassName(""), DEFAULT_CLASSNAME)!=0 )
			fprintf(OUT, "#define DLGLexer %s\n", ClassName(""));
		fprintf(OUT, "#include \"%s\"\n", DLEXER_C);
		return;
	}
	fprintf(OUT, "\n");
	fprintf(OUT, "\n");
	if (comp_level)
		fprintf(OUT, "#define ZZSHIFT(c) (b_class_no[zzauto][1+c])\n");
	else
		fprintf(OUT, "#define ZZSHIFT(c) (1+c)\n");
	if ( !gen_cpp ) fprintf(OUT, "#define MAX_MODE %d\n",mode_counter);
	fprintf(OUT, "#include \"dlgauto.h\"\n");
}


/* output the table of DFA for general use */

void p_tables()
{
	if ( !gen_cpp ) {
		fprintf(OUT, "#define DfaStates\t%d\n", dfa_allocated);
		fprintf(OUT, "typedef %s DfaState;\n\n", minsize(dfa_allocated));
	}

	if ( gen_cpp ) {
		int i;
		fprintf(OUT, "\n");
		fprintf(OUT, "const int %s::MAX_MODE=%d;\n",
				ClassName(""),
				mode_counter);
		fprintf(OUT, "const int %s::DfaStates=%d;\n",
				ClassName(""),
				dfa_allocated);
		for (i=0; i<cur_mode; i++) {
			fprintf(OUT, "const int %s::%s=%d;\n",
					ClassName(""), mode_name[i], mode_number[i]);
		}
		fprintf(OUT, "\n");
	}

	p_node_table();
	p_dfa_table();
	p_accept_table();
	p_action_table();
	p_base_table();
	p_class_table();

	/*
		08/11/97  tomf  : Changed this so that the p_bshift_table routine 
		                  gets called normally in the presence of comp_level.
								Otherwise, the result of the call gets generated 
								each time but is protected from causing problems
								on the sun by an ifdef.
	*/

	if (comp_level) {
		p_bshift_table();
	}
	else {
/*
    02-22-99 jkyl : Commented these calls out because they caus bind time problems on the HP

		fprintf( OUT, "#ifdef hpux\n" );
			p_bshift_table();
		fprintf( OUT, "#endif\n\n" );
*/
	}

	if (interactive || gen_cpp )
		p_alternative_table();
}


/* figures out the smallest variable type that will hold the transitions
 */
char *minsize(elements)
int elements;
{
	int i = 0;

	while (elements > typesize[i])
		++i;
	return typevar[i];
}


void p_node_table()
{
	register int	i;
	register int	m = 0;

	for(m=0; m<(mode_counter-1); ++m){
		for(i=dfa_basep[m]; i<dfa_basep[m+1]; ++i)
			p_single_node(i,dfa_class_nop[m]);
	}
	for(i=dfa_basep[m]; i<=dfa_allocated; ++i)
		p_single_node(i,dfa_class_nop[m]);
}


void p_single_node(i,classes)
int i,classes;
{
	register int	j;
	register int	trans, items_on_line;

#if 1
	/* extra state (classes+1) for invalid characters */
	fprintf(OUT, "%sDfaState %sst%d[%d] = {\n  ",
		gen_cpp?ClassName("::"):"static ",
		gen_cpp?ClassName("::"):"",(i-1), (classes+1));
#else
	fprintf(OUT, "static DfaState st%d[%d] = {\n  ", (i-1), classes);
#endif
	items_on_line = MAX_ON_LINE;
	for(j=0; j<classes; ++j){
		DAWDLE;
		trans = DFA(i)->trans[j];
		if (trans == NIL_INDEX)
			trans = dfa_allocated+1;
		/* all of DFA moved down one in array */
		fprintf(OUT, "%d", trans-1);
		fprintf(OUT, ", ");
		if (!(--items_on_line)){
			fprintf(OUT, "\n  ");
			items_on_line = MAX_ON_LINE;
		}
	}
#if 1
	/* put in jump to error state */
	fprintf(OUT, "%d\n};\n\n", dfa_allocated);
#else
	fprintf(OUT, "\n};\n\n");
#endif
}


void p_dfa_table()
{
	register int	i;

	fprintf(OUT, "\n%sDfaState *%sdfa[%d] = {\n",
		gen_cpp?ClassName("::"):"",gen_cpp?ClassName("::"):"", dfa_allocated);
	for (i=0; i<(dfa_allocated-1); ++i){
		fprintf(OUT, "\tst%d,\n", i);
	}
	fprintf(OUT, "\tst%d\n", i);
	fprintf(OUT, "};\n\n");
}


void p_accept_table()
{
	register int	i = 1;
	register int	items_on_line = 0;
	int		true_interactive = TRUE;

	/* make sure element for one past (zzerraction) -WEC 12/16/92 */
	fprintf(OUT,"\n%sDfaState %saccepts[%d] = {\n  ",
			gen_cpp?ClassName("::"):"",
			gen_cpp?ClassName("::"):"",
			dfa_allocated+1);
	/* don't do anything if no dfa nodes */
	if (i>dfa_allocated) goto skip_accepts;
	while (TRUE){
		int accept;
		set accept_set;
		set nfa_states;
		unsigned int *t, *nfa_i;
		unsigned int *q, *regular_expr;

		accept_set = empty;
		nfa_states = DFA(i)->nfa_states;
		t = nfa_i = set_pdq(nfa_states);
		/* NOTE: picks lowest accept because accepts monotonic	*/
		/*	with respect to nfa node numbers and set_pdq	*/
		/*	returns in that order				*/
		while((*nfa_i != nil) && (!(accept = NFA(*nfa_i)->accept))){
			nfa_i++;
		}

		/* figure out if more than one accept state there */
		if (warn_ambig ){
			set_orel(accept, &accept_set);
			while(*nfa_i != nil){
				set_orel(NFA(*nfa_i)->accept, &accept_set);
				nfa_i++;
			}
			/* remove error action from consideration */
			set_rm(0, accept_set);

			if( set_deg(accept_set)>1){
				fprintf(stderr, "dlg warning: ambiguous regular expression ");
				q = regular_expr = set_pdq(accept_set);
				while(*regular_expr != nil){
					fprintf(stderr," %d ", *regular_expr);
					++regular_expr;
				}
				fprintf(stderr, "\n");
				free(q);
			}
		}

		if ((DFA(i)->alternatives) && (accept != 0)){
			true_interactive = FALSE;
		}
		fprintf(OUT, "%d, ", accept);

		/* free up memory before we "break" below -ATG 4/6/95 */
		free(t);
		set_free(accept_set);

		if ((++i)>dfa_allocated)
			break;
		if ((++items_on_line)>=MAX_ON_LINE){
			fprintf(OUT,"\n  ");
			items_on_line = 0;
		}
/*
		free(t);
		set_free(accept_set);
*/
	}
	/* make sure element for one past (zzerraction) -WEC 12/16/92 */
skip_accepts:
	fprintf(OUT, "0\n};\n\n");
}


void p_action_table()
{
	register int	i;
	char* className = ClassName("");

	if ( gen_cpp )
		fprintf(OUT, "Ptr%sMemberFunc %s::actions[%d] = {\n", className,
					className, action_no+1);
	else
		fprintf(OUT, "void (*actions[%d])() = {\n", action_no+1);
	if ( gen_cpp )
/*		fprintf(OUT, "\t(Ptr%sMemberFunc)&%s::erraction,\n", className, className);*/
		fprintf(OUT, "\t&%s::erraction,\n", className);
	else
		fprintf(OUT, "\tzzerraction,\n");
	for (i=1; i<action_no; ++i) {
		if ( gen_cpp )
/*			fprintf(OUT,"\t(Ptr%sMemberFunc)&%s::act%d,\n", className, className, i);*/
			fprintf(OUT,"\t&%s::act%d,\n", className, i);
		else
			fprintf(OUT,"\tact%d,\n", i);
		DAWDLE;
	}
	if ( gen_cpp )
/*		fprintf(OUT,"\t(Ptr%sMemberFunc)&%s::act%d\n", className, className, i);*/
		fprintf(OUT,"\t&%s::act%d\n", className, i);
	else
		fprintf(OUT,"\tact%d\n", i);
	fprintf(OUT, "};\n\n");
}


void p_shift_table(m)						/* MR1 */
int m;
{
	register int	i = 0, j;
	register int	items_on_line = 0;

	fprintf(OUT, "%s unsigned char %sshift%d[%d] = {\n  ",
		gen_cpp?"":"static",
		gen_cpp?ClassName("::"):"", m, CHAR_RANGE);
	while (TRUE){
		/* find which partition character i is in */
		for (j=0; j<dfa_class_nop[mode_counter]; ++j){
			if (set_el(i,class_sets[j]))
				break;
			}
		fprintf(OUT,"%d",j);
		if ((++i)>=CHAR_RANGE)
			break;
		fprintf(OUT,", ");
		if ((++items_on_line)>=MAX_ON_LINE){
			fprintf(OUT,"\n  ");
			items_on_line = 0;
			}
		}
	fprintf(OUT, "\n};\n\n");
}


void p_base_table()
{
	register int m;

	fprintf(OUT, "%sDfaState %sdfa_base[] = {\n",
			gen_cpp?ClassName("::"):"static ",
			gen_cpp?ClassName("::"):"");
	for(m=0; m<(mode_counter-1); ++m)
		fprintf(OUT, "\t%d,\n", dfa_basep[m]-1);
	fprintf(OUT, "\t%d\n};\n\n", dfa_basep[m]-1);
}


void p_class_table()						/* MR1 */
{
#if 0
	register int m;

	fprintf(OUT,"%s int %sdfa_class_no[] = {\n",
			gen_cpp?"":"static",
			gen_cpp?ClassName("::"):"");
	for(m=0; m<(mode_counter-1); ++m)
		fprintf(OUT,"\t%d,\n", dfa_class_nop[m]);
	fprintf(OUT,"\t%d\n};\n\n", dfa_class_nop[m]);
#endif
}


void p_bshift_table()						/* MR1 */
{
	register int m;

	fprintf(OUT,"%s unsigned char *%sb_class_no[] = {\n",
		gen_cpp?"":"static",
		gen_cpp?ClassName("::"):"");
	for(m=0; m<(mode_counter-1); ++m)
		fprintf(OUT, "\tshift%d,\n", m);
	fprintf(OUT, "\tshift%d\n};\n\n", m);
}


void p_alternative_table()					/* MR1 */
{
	register int i;

	if ( !gen_cpp ) fprintf(OUT, "#define ZZINTERACTIVE\n\n");
	if ( gen_cpp )
		fprintf(OUT, "DLGChar %salternatives[%sDfaStates+1] = {\n",
				ClassName("::"),
				ClassName("::"));
	else
		fprintf(OUT, "static %s zzalternatives[DfaStates+1] = {\n",
				minsize(dfa_allocated));

	for(i=1; i<=dfa_allocated; ++i)
		fprintf(OUT, "\t%d,\n", DFA(i)->alternatives);
	fprintf(OUT, "/* must have 0 for zzalternatives[DfaStates] */\n");
	fprintf(OUT, "\t0\n};\n\n");
}


void p_mode_def(s,m)					/* MR1 */
char *s;
int m;
{
	if ( gen_cpp )
	{
		mode_name[cur_mode] = mystrdup(s);
		mode_number[cur_mode] = m;
		cur_mode++;
	}
	else
		fprintf(mode_stream, "#define %s %d\n", s, m);
}

char *
ClassName(suffix)
char *suffix;
{
	static char buf[200];
	extern char *class_name;

	sprintf(buf, "%s%s", class_name, suffix);
	return buf;
}

#ifdef DEBUG
/* print out a particular nfa node that is pointed to by p */
p_nfa_node(p)
nfa_node *p;
{
	 register nfa_node *t;

	if (p != NIL_INDEX){
		printf("NFA state : %d\naccept state : %d\n",
			NFA_NO(p),p->accept);
		if (p->trans[0] != NIL_INDEX){
			printf("trans[0] => %d on ", NFA_NO(p->trans[0]));
			p_set(p->label);
			printf("\n");
			}
		else
			printf("trans[0] => nil\n");
		if (p->trans[1] != NIL_INDEX)
			printf("trans[1] => %d on epsilon\n",
				NFA_NO(p->trans[1]));
		else
			printf("trans[1] => nil\n");
		printf("\n");
		}
}
#endif

#ifdef  DEBUG
/* code to print out special structures when using a debugger */

p_nfa(p)
nfa_node *p;	/* state number also index into array */
{
/* each node has a marker on it so it only gets printed once */

	operation_no++; /* get new number */
	s_p_nfa(p);
}

s_p_nfa(p)
nfa_node *p;	/* state number also index into array */
{
	if ((p != NIL_INDEX) && (p->nfa_set != operation_no)){
		/* so it is only printed once */
		p->nfa_set = operation_no;
		p_nfa_node(p);
		s_p_nfa(p->trans[0]);
		s_p_nfa(p->trans[1]);
		}
}

p_dfa_node(p)
dfa_node *p;
{
	int i;

	if (p != NIL_INDEX){
		printf("DFA state :%d\n",NFA_NO(p));
		if (p->done)
			printf("done\n");
		else
			printf("undone\n");
		printf("from nfa states : ");
		p_set(p->nfa_states);
		printf("\n");
		/* NOTE: trans arcs stored as ints rather than pointer*/
		for (i=0; i<class_no; i++){
			printf("%d ",p->trans[i]);
			}
		printf("\n\n");
		}
}

p_dfa()
{
/* prints out all the dfa nodes actually allocated */

	int i;

	for (i = 1; i<=dfa_allocated; i++)
		p_dfa_node(NFA(i));
}


/* print out numbers in the set label */
p_set(label)
set label;
{
	unsigned *t, *e;

	if (set_nil(label)){
		printf("epsilon\n");
	}else{
		t = e = set_pdq(label);
		while(*e != nil){
			printf("%d ", (*e+MIN_CHAR));
			e++;
		}
		printf("\n");
		free(t);
	}
	
}
#endif

