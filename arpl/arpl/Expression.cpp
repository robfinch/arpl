#include "stdafx.h"
using namespace std;
#include <list>
#include <array>

extern int defaultcc;
extern Symbol* currentClass;
extern ENODE* makenodei(int nt, ENODE* v1, int i);
extern ENODE* makefcnode(int nt, ENODE* v1, ENODE* v2, Symbol* sp);
extern ENODE* makefqnode(int nt, Float128* f128);
extern TYP* CondAddRef(ENODE** node, TYP* tp);
extern bool IsBeginningOfTypecast(int st);
extern int NumericLiteral(ENODE*);
int64_t List::numele;

Expression::Expression()
{
	int nn;

	head = (TYP*)nullptr;
	tail = (TYP*)nullptr;
	LHSType = (TYP*)nullptr;
	sizeof_flag = 0;
	totsz = 0;
	got_pa = false;
	cnt = 0;
	numdimen = 0;
	pep1 = (ENODE*)nullptr;
	isMember = false;
	for (nn = 0; nn < 10; nn++)
		sa[nn] = 0;
	parsingAggregate = 0;
}

Function* Expression::MakeFunction(int symnum, Symbol* sym, bool isPascal) {
	Function* fn = compiler.ff.MakeFunction(symnum, sym, isPascal);
	return (fn);
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

ENODE* Expression::SetIntConstSize(TYP* tptr, int64_t val)
{
	ENODE* pnode;

	pnode = makeinode(en_icon, val);
	pnode->constflag = true;
	if (val >= -128LL && ival < 128LL)
		pnode->esize = 1LL;
	else if (val >= -32768LL && val < 32768LL)
		pnode->esize = 2LL;
	else if (val >= -2147483648LL && val < 2147483648LL)
		pnode->esize = 4LL;
	else
		pnode->esize = 8LL;
/*
* ???
	else if (val >= -54975581300LL && val < 54975581300LL)
		pnode->esize = 5;
	else
		pnode->esize = 2;
*/
	pnode->SetType(tptr);
	return (pnode);
}


TYP* Expression::ParseCharConst(ENODE** node, int sz)
{
	ENODE* pnode;
	TYP* tptr;

	tptr = &stdchar;
	pnode = makeinode(en_icon, ival);
	pnode->constflag = true;
	pnode->esize = sz;
	pnode->SetType(tptr);
	NextToken();
	*node = pnode;
	return (tptr);
}

TYP* Expression::ParseFloatMax(ENODE** node)
{
	ENODE* pnode;
	TYP* tptr;

	tptr = &stdquad;
	pnode = compiler.ef.Makefqnode(en_fcon, rval128);
	pnode->constflag = true;
	pnode->SetType(tptr);
	//pnode->i = NumericLiteral(Float128::FloatMax());
	if (parsingAggregate == 0 && sizeof_flag == 0)
		pnode->i = NumericLiteral(pnode);
	NextToken();
	*node = pnode;
	return (tptr);
}

TYP* Expression::ParseRealConst(ENODE** node)
{
	ENODE* pnode;
	TYP* tptr;
	int32_t *w;

	w = (int32_t*)&rval128;
	pnode = makefqnode(en_fcon, &rval128);
	/*
	if (rval128.IsSingle()) {
		Float128::Float128ToSingle(&f, &rval128);
		w = (int32_t*)&f;
		pnode = makenode(en_pfx0, nullptr, makeinode(en_icon, (int64_t)w[0]));
		pnode->constflag = true;
		pnode->segment = codeseg;
	}
	else if (rval128.IsDouble()) {
		Float128::Float128ToDouble(&d, &rval128);
		w = (int32_t*)&d;
		pnode = makenode(en_pfx0, nullptr, makeinode(en_icon,(int64_t)w[0]));
		pnode->constflag = true;
		pnode->segment = codeseg;
		pnode = makenode(en_pfx1, pnode, makeinode(en_icon, (int64_t)w[1]));
		pnode->constflag = true;
		pnode->segment = codeseg;
	}
	else {
		w = (int32_t*)&rval128;
		pnode = makenode(en_pfx0, nullptr, makeinode(en_icon, (int64_t)w[0]));
		pnode->constflag = true;
		pnode->segment = codeseg;
		pnode = makenode(en_pfx1, pnode, makeinode(en_icon, (int64_t)w[1]));
		pnode->constflag = true;
		pnode->segment = codeseg;
		pnode = makenode(en_pfx2, pnode, makeinode(en_icon, (int64_t)w[2]));
		pnode->constflag = true;
		pnode->segment = codeseg;
		pnode = makenode(en_pfx3, pnode, makeinode(en_icon, (int64_t)w[3]));
		pnode->constflag = true;
		pnode->segment = codeseg;
	}
	*/
	//pnode->i = quadlit(&rval128);
	//if (parsingAggregate == 0 && sizeof_flag == 0)
#ifdef RISCV
	pnode->i = NumericLiteral(pnode);
#endif
	pnode->segment = codeseg;
	switch (float_precision) {
	case 'Q': case 'q':
		tptr = &stdquad;
		break;
	case 'D': case 'd':
		tptr = &stddouble;
		break;
	case 'S': case 's':
		tptr = &stdflt;
		break;
	default:
		tptr = &stddouble;
		break;
	}
	*node = pnode;
	pnode->SetType(tptr);
	NextToken();
	return (tptr);
}

TYP* Expression::ParsePositConst(ENODE** node)
{
	ENODE* pnode;
	TYP* tptr;

	pnode = compiler.ef.MakePositNode(en_pcon, pval64);
	pnode->constflag = true;
	pnode->posit = pval64;
	//if (parsingAggregate==0 && sizeof_flag == 0)
	//	pnode->i = NumericLiteral(pnode);
	pnode->segment = use_iprel ? codeseg : rodataseg;
	tptr = &stdposit;
	switch (float_precision) {
	case 'D': case 'd':
		tptr = &stdposit;
		pnode->esize = 8;
		break;
	case 'S': case 's':
		tptr = &stdposit32;
		pnode->esize = 4;
		break;
	case 'H': case 'h':
		tptr = &stdposit16;
		pnode->esize = 2;
		break;
	default:
		tptr = &stdposit;
		pnode->esize = 8;
		break;
	}
	*node = pnode;
	pnode->SetType(tptr);
	NextToken();
	return (tptr);
}

TYP* Expression::ParseStringConst(ENODE** node, Symbol* sym)
{
	char* str;
	ENODE* pnode;
	TYP* tptr;
	Symbol* sp;

	str = GetStrConst();
//	if (sizeof_flag) {
		tptr = (TYP*)TYP::Make(bt_pointer, 0);
		tptr->size = strlen(str);
		tptr->btpp = TYP::Make(bt_char, 2);
		tptr->val_flag = true;
		tptr->isUnsigned = true;
//	}
	/*
	else {
		tptr = &stdstring;
	}
	*/
	pnode = makenodei(en_labcon, (ENODE*)NULL, 0);
	if (sizeof_flag == 0) {
		sp = SymbolFactory::Make(*sym->name, sym->tp, sym->parentp, sym->depth, sc_const);
		pnode->i = stringlit(str, sp);
		pnode->sp = new std::string(&str[1]);
		if (sym)
			sym->value.i = pnode->i;
	}
	free(str);
	*node = pnode;
	pnode->etype = bt_pointer;
	pnode->esize = cpu.sizeOfPtr;
	pnode->constflag = true;
	pnode->segment = use_iprel ? codeseg : rodataseg;
	pnode->SetType(tptr);
	return (tptr);
}

ENODE* Expression::ParseInlineStringConst(ENODE** node, Symbol* sym)
{
	ENODE* pnode;
	TYP* tptr;
	char* str;

	str = GetStrConst();
	if (sizeof_flag) {
		tptr = (TYP*)TYP::Make(bt_pointer, 0);
		tptr->size = strlen(str) + (int64_t)1;
		tptr->btpp = TYP::Make(bt_ichar, 2);
		tptr->val_flag = true;
		tptr->isUnsigned = true;
	}
	else {
		tptr = &stdistring;
	}
	pnode = makenodei(en_labcon, (ENODE*)NULL, 0);
	if (sizeof_flag == 0)
		pnode->i = stringlit(str, sym);
	free(str);
	pnode->etype = bt_pointer;
	pnode->esize = 2;
	pnode->constflag = true;
	pnode->segment = use_iprel ? codeseg : rodataseg;
	pnode->SetType(tptr);
	return (pnode);
}

ENODE* Expression::ParseStringConstWithSizePrefix(ENODE** node, Symbol* sym)
{
	ENODE* pnode;
	TYP* tptr;
	char* str;

	str = GetStrConst();
	if (sizeof_flag) {
		tptr = (TYP*)TYP::Make(bt_pointer, 0);
		tptr->size = strlen(str) + (int64_t)1;
		switch (str[0]) {
		case 'B':
			tptr->btpp = TYP::Make(bt_byte, 1);
			break;
		case 'W':
			tptr->btpp = TYP::Make(bt_char, 2);
			break;
		case 'T':
			tptr->btpp = TYP::Make(bt_short, 4);
			break;
		case 'O':
			tptr->btpp = TYP::Make(bt_int, 8);
			break;
		}
		tptr->val_flag = true;
		tptr->isUnsigned = true;
	}
	else {
		tptr = &stdastring;
	}
	pnode = makenodei(en_labcon, (ENODE*)NULL, 0);
	if (sizeof_flag == 0) {
		pnode->i = stringlit(str, sym);
		pnode->sym = sym;
		if (sym)
			sym->value.i = pnode->i;
	}
	switch (str[0]) {
	case 'B': pnode->esize = 1; break;
	case 'W': pnode->esize = 2; break;
	case 'T': pnode->esize = 4; break;
	case 'O': pnode->esize = 8; break;
	}
	free(str);
	pnode->etype = bt_pointer;
	pnode->constflag = true;
	pnode->segment = use_iprel ? codeseg : rodataseg;
	pnode->SetType(tptr);
	return (pnode);
}

ENODE* Expression::ParseThis(ENODE** node)
{
	dfs.puts("<ExprThis>");
	ENODE* pnode;
	TYP* tptr;
	TYP* tptr2;

	tptr2 = TYP::Make(bt_class, 0);
	if (currentClass == nullptr) {
		error(ERR_THIS);
	}
	else {
		// This does not make copies of contained classes / structures / unions
		// It uses the existing references. See TYP::Copy()
		memcpy(tptr2, currentClass->tp, sizeof(TYP));
	}
	NextToken();
	tptr = TYP::Make(bt_pointer, cpu.sizeOfPtr);
	tptr->btpp = tptr2;
	tptr->isUnsigned = TRUE;
	dfs.puts((char*)tptr->btpp->sname->c_str());
	pnode = makeinode(en_regvar, regCLP);
	pnode->SetType(tptr);
	dfs.puts("</ExprThis>");
	return (pnode);
}

void Expression::ParseAggregateArray(ENODE** node, ENODE* cnode, Symbol* symi, TYP* tp)
{
	ENODE* pnode;
	//ENODE** node_array;
	int64_t count;
	int64_t maxcount;
	int64_t at_node;
	int64_t asz;
	TYP* array_type;
	ENODE** node_array;
	ENODE** new_array;

//	pnode = *node;
	asz = 1000;
	pnode = nullptr;
	array_type = tp->btpp;
	node_array = new ENODE * [asz];
	memset(node_array, 0, asz * sizeof(ENODE*));
	/*
	node_array = new ENODE * [tp->numele];
	for (count = 0; count < tp->numele; count++) {
		node_array[count] = allocEnode();
		node_array[count]->order = count;
		node_array[count]->SetType(symi->tp->btpp);
	}
	*/
	for (count = 0; tp->numele==0 ? true: count < tp->numele; count++) {
		at_node = 0;
		if (count >= asz) {
			asz += 1000;
			new_array = new ENODE * [asz];
			memset(node_array, 0, asz * sizeof(ENODE*));
			memcpy(new_array, node_array, (asz - 1000) * sizeof(ENODE*));
			delete[] node_array;
			node_array = new_array;
		}
		if (lastst == e_sym::begin)
			ParseAggregate(&node_array[count], symi, tp->btpp);
		else {
			if (lastst == openbr) {
				NextToken();
				at_node = GetConstExpression(nullptr, symi).low;
				needpunc(closebr, 49);
				needpunc(assign, 50);
			}
			else
				at_node = count;
			if (at_node > asz) {
				asz = at_node + 1000;
				new_array = new ENODE * [asz];
				memset(node_array, 0, asz * sizeof(ENODE*));
				memcpy(new_array, node_array, (asz - 1000) * sizeof(ENODE*));
				delete[] node_array;
				node_array = new_array;
			}
			ParseNonCommaExpression(&node_array[at_node], symi);
			opt_const(&node_array[at_node]);
			lastst;
		}
		if (tp->numele && at_node > tp->numele)
			error(ERR_ILLINIT);
		if (lastst == comma)
			NextToken();
		else
			break;
	}
	maxcount = max(count+1,tp->numele);
	for (count = 0; count < maxcount; count++) {
		if (node_array[count] == nullptr)
			node_array[count] = makenode(en_void, nullptr, nullptr);
		pnode = makenode(en_void, pnode, node_array[count]);
		if (node_array[count]->tp == nullptr) {
			if (tp->type == bt_pointer)
				node_array[count]->nodetype = en_icon;
			else
				node_array[count]->nodetype = en_icon;
			node_array[count]->tp = tp->btpp;
			node_array[count]->i = 0;
			node_array[count]->i128.low = 0;
			node_array[count]->i128.high = 0;
			node_array[count]->f = 0.0;
			node_array[count]->f128 = *Float128::Zero();
		}
	}
	/*
	pnode = makenode(en_end_aggregate, pnode, nullptr);
	pnode->SetType(TYP::Copy(&stdptr));
	pnode->tp->val_flag = true;
	pnode->tp->btpp = tp;
	pnode->tp->numele = tp->numele;
	*/
	*node = pnode;
	if (tp->numele == 0) {
		tp->numele = maxcount;
//		tp->size = tp->numele * tp->size;
	}
}

bool Expression::ParseAggregateStruct(ENODE** node, ENODE* cnode, Symbol* symi, TYP* tp)
{
	Int128 n128;
	int64_t count, ndx, maxcount, at_node;
	Symbol* lst, * tmp, * hlst;
	ENODE* pnode, *qnode;
	ENODE** node_array;
	TYP* tptr, * tptr2;
	bool consistentType = true;
	bool found = false;
	bool is_array = false;
	bool is_union = false;
	int64_t ne;
	List* pplist;

	pnode = nullptr;
	tptr2 = nullptr;
	tptr = tp;
	if (tp->type == bt_pointer)
		hlst = lst = tp->btpp->lst.headp;
	else {
		hlst = lst = tp->lst.headp;
	}
	if (tp->type == bt_union) {
		is_union = true;
		maxcount = 1;
	}
	// Find the largest array in a union
	ne = 1;
	if (is_union) {
		for (maxcount = 0; lst != nullptr; maxcount++, lst = lst->nextp) {
			if (lst->tp->type == bt_array || (lst->tp->type == bt_pointer && lst->tp->val_flag))
				ne = max(ne, lst->tp->numele);
		}
	}
	// Or find the number of struct members
	else {
		for (maxcount = 0; lst != nullptr; maxcount++, lst = lst->nextp)
			;
		ne = maxcount;
	}
	// Allocate array and ENODEs
	node_array = new ENODE * [ne];
	pplist = new List[ne];
	for (count = 0; count < ne; count++) {
		node_array[count] = allocEnode();
		node_array[count]->order = count;
		pplist[count].node = node_array[count];
	}

	// If it is a struct, loop through the struct member, otherwise if it is a
	// union count the number of elements in case an array.
	lst = hlst;
	for (count = 0;
		lastst != e_sym::end && lst != nullptr || tp->type == bt_union;
		count++, tp->type != bt_union ? lst = lst->nextp : 0) {
		at_node = count;
		found = false;
		tmp = nullptr;
		tptr = nullptr;
		// Check for an indexing operation.
		if (lastst == dot) {
			NextToken();
			if (lastst != id)
				error(ERR_IDEXPECT);
			else
				NextToken();
			needpunc(assign, 56);
			ndx = 0;
			for (tmp = tp->lst.headp; tmp; tmp = tmp->nextp, ndx++) {
				if (strcmp(compiler.lastid, tmp->name->c_str()) == 0) {
					at_node = ndx;
					found = true;
					break;
				}
			}
			if (!found)
				error(ERR_NOMEMBER);
		}
		// Check for indexing operation.
		// We allow this for a struct member access too. Why error?
		else if (lastst == openbr) {
			NextToken();
			n128 = GetIntegerExpression(&qnode, symi, 0);
			needpunc(closebr, 78);
			needpunc(assign, 79);
			at_node = n128.low;
			ndx = 0;
			for (tmp = tp->lst.headp; tmp && ndx != at_node; tmp = tmp->nextp, ndx++)
				;
			if (ndx != at_node) {
				tmp = nullptr;
				found = false;
			}
		}
		else {
			tmp = lst;
			found = true;
		}

		// Parse the element, it may be another level of aggregate or just an
		// expression.
		if (at_node < ne) {
			if (lastst == e_sym::begin) {
				if (lst->tp->type != bt_struct && lst->tp->type != bt_union &&
					lst->tp->type != bt_array &&
					!(lst->tp->type == bt_pointer && lst->tp->val_flag))
					error(ERR_MISMATCH);
				tptr = ParseAggregate(&node_array[count], lst, lst->tp);
			}
			else {
				tptr = ParseNonCommaExpression(&node_array[at_node], found ? tmp : nullptr);
				opt_const(&node_array[at_node]);
			}
			node_array[at_node]->SetType(tptr);
		}
		// Check for type consistency. It might be an array.
		if (tptr2 != nullptr && !TYP::IsSameType(tptr, tptr2, false))
			consistentType = false;
		tptr2 = tptr;
		if (lastst == comma) {
			NextToken();
			is_array = true;
		}
		else
			break;
	}

	if (is_array) {
		maxcount = ne;
		// Find the union member matching the array, we need this to set the type.
		if (is_union)
			for (lst = symi->tp->lst.headp; lst; lst = lst->nextp) {
				if (lst->tp->isArray) {
					if (TYP::IsSameType(tptr, lst->tp->btpp, false)) {
						tptr = lst->tp;
						break;
					}
				}
			}
	}
//	pnode = makenode(en_void, nullptr, nullptr);
	// Create linked list of node in reverse order. The output routine requires
	// them in the order they were encountered during parse.
	for (count = 0; count < maxcount; count++)
		pnode = makenode(en_void, pnode, node_array[count]);
	pnode = makenode(en_end_aggregate, pnode, nullptr);

	// Set resulting type.
	if (is_array) {
		pnode->SetType(TYP::Copy(&stdptr));
		pnode->tp->val_flag = true;
		pnode->tp->btpp = tptr;
		pnode->tp->numele = ne;
	}
	else
		pnode->SetType(tptr);
	*node = pnode;
	return (consistentType);
}

void Expression::ParseAggregateHelper(ENODE** node, ENODE* cnode)
{
	bool cnst = true;
	bool consistentType = true;
	ENODE* pnode;

	pnode = *node;
	if (cnode == nullptr)
		return;
	if (!cnode->constflag)
		cnst = false;
	if (pnode == nullptr) {
		pnode = makenode(en_void, nullptr, cnode);
		cnode->order = 0;
		pnode->order = 1;
		pnode->SetType(cnode->tp);
	}
	else {
		cnode->order = pnode->order + 1;
		pnode = makenode(en_void, pnode, cnode);
		pnode->order = cnode->order + 1;
		pnode->SetType(pnode->p[0]->tp);
	}

	*node = pnode;
}

/*
*/

static int updateStringLit(int i, char* str, char ch)
{
	slit* lit;
	std::string t;

	for (lit = strtab; lit; lit = lit->next) {
		if (lit->label == i) {
			t = lit->str;
			if (str)
				t.append(str);
			else if (ch)
				t.append(1,ch);
			lit->str = my_strdup((char *)t.c_str());
		}
	}
	return (i);
}

ENODE* Expression::ParseAggregateStringConst(ENODE** node, Symbol* symi)
{
	TYP* tptr;
	ENODE* pnode;
	bool cont = false;
	slit* lit;
	int prevstr = 0;
	int firststr = 0;
	bool first = true;
	int lst;

	do {
		tptr = ParseNonCommaExpression(&pnode, symi);
		if (!(tptr->type == bt_pointer  && tptr->btpp->IsCharType()) && !tptr->IsIntegralType()) {
			error(ERR_MISMATCH);
			return (nullptr);
		}
		if (firststr == 0) {
			firststr = pnode->i;
			lst = lastst;
		}
		else {
			if (tptr->type==bt_pointer)
				string_exclude.add(pnode->i);
		}
		cont = lastst == comma;
		if (*node == nullptr) {
			*node = pnode;
			if (!cont)
				break;
			NextToken();
			continue;
		}
		// If a character is found, append to string
		if (tptr->IsIntegralType())
			updateStringLit(firststr, nullptr, pnode->i128.low);
		// was a string, add two strings together
		else
			updateStringLit(firststr,(char *)(pnode->sp->c_str()), 0);
		// skip over comma
		if (cont)
			NextToken();
	} while (cont);
//	sp = SymbolFactory::Make(*sym->name, sym->tp, sym->parentp, sym->depth, sc_const);
	(*node)->i = firststr;// stringlit((char*)(*node)->sp->c_str(), symi);
	return (*node);
}

/*
* On Entry:
*		The parser has just found and skipped past an opening brace bracket '{'. An
*	aggregate value must be following.
* 
* Parameters:
*		symi: (input)
*   tp:		(input) the type of the aggregate up to the current processing point.
*					Must be an aggregate type or an error message will result. One of
*					struct, union, array or pointer.
*		node: (returned) a pointer to a pointer to a node containing the aggregate.
*/

TYP* Expression::ParseAggregate(ENODE** node, Symbol* symi, TYP* tp)
{
	ENODE* pnode;
	TYP* tptr, *btpp;
	int64_t sz = 0;
	ENODE* cnode, *hnode;
	Symbol* lst;
	bool cnst = true;
	bool consistentType = true;
	TYP* tptr2;
	int64_t pos = 0;
	static int level = 0;
	bool is_array = false;
	bool is_struct = false;
	std::string str;

	NextToken(0);

	// A string constant enclosed in aggregate brackets is treated as just a
	// string constant. A string constant in this manner may be made up of a
	// comma separated list of strings and characters.

	if (tp->type == bt_pointer && tp->btpp && tp->btpp->IsCharType()) {
		if (lastst == sconst || lastst == asconst) {
			tptr = ParseAggregateStringConst(node, symi)->tp;
			needpunc(e_sym::end, 79);
			return (tptr);
		}
	}

	// {0} is allowed to be specified as the value of an intrinsic type. It is not
	// really an aggregate.

	if (tp->IsIntrinsicType() && !tp->IsStructType()) {
		tptr = ParseExpression(node, symi);
		needpunc(e_sym::end, 80);
		return (tptr);
	}

	// Now, there is an aggregate of some sort. Create a node for it. Note the
	// beginning node is the oldest one processed, the end node will be the latest
	// one processed.

	hnode = pnode = makenode(en_aggregate, nullptr, nullptr);
	pnode->order = 0;
	parsingAggregate++;
	head = tail = nullptr;
	tptr = tptr2 = nullptr;
	cnode = nullptr;

	// The input type must match. If the type does not match an error
	// is output, but processing continues on to absorb the aggregate.
	if (tp->type != bt_struct && tp->type != bt_union &&
		tp->type != bt_array && tp->type != bt_pointer)
//		!(tp->type == bt_pointer && tp->val_flag))
		error(ERR_MISMATCH);

	// Flag node as struct or array type. Convenience for later processing.
	is_struct = tp->IsStructType();
	is_array = tp->isArray;
	if (tp->type == bt_pointer) {
		is_array = tp->val_flag == true;
		lst = tp->lst.headp;
		is_struct = tp->btpp->IsStructType();
	}
	else
		lst = tp->lst.headp;

	// Parse the inside of the aggregate according to what type is expected.
	// Handle an array
	if (is_array) {
		ParseAggregateArray(&pnode, cnode, symi, tp);
		consistentType = true;
	}
	else {
		ParseAggregateStruct(&pnode, cnode, symi, tp);
		consistentType = false;
	}
	needpunc(e_sym::end, 56);
/*
	else {
		tptr = ParseNonCommaExpression(&pnode, lst);
		opt_const(&pnode);
		lastst;
		if (lastst == comma)
			NextToken();
	}
	*/
	// If we specifed an aggregate with just a single value, return the type of
	// the value, not an aggregate list.
	/*
	if (count < 2) {
		*node = pnode;
		if (pnode != nullptr && pnode->tp)
			return (pnode->tp);
		else
			return (nullptr);
	}
	*/
	sz = tp->size;
	tptr = pnode->tp;

	// Create node denoting the end of the aggregate.

	pnode = makenode(en_end_aggregate, pnode, nullptr);
	if (tp->type == bt_union) {
		hnode->SetType(tp);
		pnode->SetType(tp);
		tptr = tp;
		// union will pick up types from the symbol table
		btpp = nullptr;
	}
	else
	{
//		hnode->SetType(tptr = TYP::Make(consistentType ? bt_array : bt_struct, sz));
//		pnode->SetType(tptr = TYP::Make(consistentType ? bt_array : bt_struct, sz));
		hnode->SetType(tp);
		pnode->SetType(tp);
		/*
		if (consistentType) {
			btpp = tp;
		}
		else {
			btpp = nullptr;
			// Need to initialize the symbol table with the struct elements.
			// A struct of what? Could loop through the node of the aggregate
			// but, the type is passed in in tp. It shoulkd match.
			//hnode->tp->lst = tp->lst;
			//pnode->tp->lst = tp->lst;
		}
		*/
	}
	/*
	if (consistentType) {
	*/
		/*
		hnode->tp->btpp = btpp;
		pnode->tp->btpp = btpp;
		hnode->tp->isArray = true;
		pnode->tp->isArray = true;
		*/
	/*
		count = 0;
		for (qnode = pnode->p[0]; qnode; qnode = qnode->p[0])
			count++;
		hnode->tp->numele = tp->numele;// count;
		pnode->tp->numele = tp->numele;// count;
//		hnode->tp->size = (count) * tptr->size;
//		pnode->tp->size = (count) * tptr->size;
	}
	*/
	hnode->esize = tp->size;
	pnode->esize = tp->size;
	str = "";
//	str.append(*currentFn->sym->GetFullName());

	str.append(GetPrivateNamespace());
	hnode->i = litlist(pnode, (char*)str.c_str());
	pnode->i = litlist(pnode, (char*)str.c_str());

	// There are values for the aggregate so it needs to go into an initialized
	// data area. This would be one of code, rodata, or data segments. Note if
	// aggregate is not declared as a constant, it is placed in the initialized
	// data area. It is updateable. If it is declared as a constant then it is
	// placed in a read-only area.
	hnode->segment = cnst ? (use_iprel ? codeseg : rodataseg) : dataseg;
	pnode->segment = cnst ? (use_iprel ? codeseg : rodataseg) : dataseg;
	hnode->constflag = true;
	pnode->constflag = true;
	parsingAggregate--;

	// Return the node with the aggregate values.
	*node = pnode;
	if (parsingAggregate==0)
		pnode->DumpAggregate();
	return (pnode->tp);
}

TYP* Expression::ParseNameRef(ENODE** node, Symbol* symi, int nt)
{
	ENODE* pnode;
	TYP* tptr;

	// Try and find the name, if it is not found then see if it could be the
	// current symbol.
	tptr = nameref(&pnode, nt, symi);
	if (tptr == nullptr) {
		if (currentSym) {
			if (currentSym->name->compare(compiler.lastid) == 0) {
				tptr = currentSym->tp;
			}
		}
	}
	// Convert a reference to a constant to a constant. Need this for
	// GetIntegerExpression().
	if (pnode->IsRefType()) {
		if (pnode->p[0]) {
			if (pnode->p[0]->nodetype == en_icon) {
				pnode = SetIntConstSize(tptr, pnode->p[0]->i);
			}
		}
		//else if (pnode->p[0]->nodetype == en_fcon) {
		//	rval = pnode->p[0]->f;
		//	rval128 = pnode->p[0]->f128;
		//	goto j2;
		//}
	}
	//pnode->p[3] = (ENODE *)tptr->size;
	//				if (pnode->nodetype==en_nacon)
	//					pnode->p[0] = makenode(en_list,tptr->BuildEnodeTree(),nullptr);
			//else if (sp = gsyms->Find(compiler.lastid, false)) {
			//	if (TABLE::matchno > 1) {
			//		for (i = 0; i < TABLE::matchno) {
			//			sp = TABLE::match[i];
			//		}
			//	}
			//	if (sp->tp == &stdconst) {
			//		ival = sp->value.i;
			//		lastst = iconst;
			//		return;
			//	}
			//}

	/*
			// Try and find the symbol, if not found, assume a function
			// but only if it's followed by a (
			if (TABLE::matchno==0) {
				while( my_isspace(lastch) )
					getch();
				if( lastch == '(') {
					NextToken();
					tptr = ExprFunction(nullptr, &pnode);
				}
				else {
					tptr = nameref(&pnode,TRUE);
				}
			}
			else
	*/

	// If tptr is still null, assume an integer.
	if (tptr==nullptr) {
		tptr = allocTYP();
		tptr->type = bt_int;
		tptr->typeno = bt_int;
		tptr->alignment = 8;
		tptr->bit_offset = 0;
		tptr->btpp = nullptr;
		tptr->isArray = false;
		tptr->isIO = false;
		tptr->isShort = false;
		tptr->isUnsigned = false;
		tptr->size = 8;
		tptr->sname = new std::string(compiler.lastid);
	}

	if (pnode == nullptr)
		return (nullptr);
	pnode->SetType(tptr);
	//pnode->constflag = false; ???
	*node = pnode;
	return (tptr);
}

TYP* Expression::ParseMinus(ENODE** node, Symbol* symi)
{
	ENODE* ep1;
	TYP* tp;

	NextToken();
	tp = ParseCastExpression(&ep1, symi);
	if (tp == NULL) {
		error(ERR_IDEXPECT);
		return (nullptr);
	}
	else if (ep1->constflag && (ep1->nodetype == en_icon)) {
		ep1->i = -ep1->i;
		Int128::Sub(&ep1->i128, Int128::Zero(), &ep1->i128);
	}
	else if (ep1->constflag && (ep1->nodetype == en_fcon)) {
		ep1->f = -ep1->f;
		ep1->f128.sign = !ep1->f128.sign;
		// A new literal label is required.
		//ep1->i = quadlit(&ep1->f128);
		ep1->i = NumericLiteral(ep1);
	}
	else
	{
		ep1 = makenode(en_uminus, ep1, (ENODE*)NULL);
		ep1->constflag = ep1->p[0]->constflag;
		ep1->isUnsigned = ep1->p[0]->isUnsigned;
		ep1->esize = tp->size;
		ep1->etype = (e_bt)tp->type;
	}
	*node = ep1;
	ep1->SetType(tp);
	return (tp);
}

TYP* Expression::ParseNot(ENODE** node, Symbol* symi)
{
	ENODE* ep1;
	TYP* tp;

	NextToken();
	tp = ParseCastExpression(&ep1, symi);
	if (tp == NULL) {
		error(ERR_IDEXPECT);
		return (nullptr);
	}
	ep1 = makenode(en_not, ep1, (ENODE*)NULL);
	ep1->constflag = ep1->p[0]->constflag;
	ep1->isUnsigned = ep1->p[0]->isUnsigned;
	ep1->SetType(tp);
	ep1->esize = tp->size;
	*node = ep1;
	return (tp);
}

ENODE* Expression::ParseCom(Symbol* symi)
{
	ENODE* ep1;
	TYP* tp;

	NextToken();
	tp = ParseCastExpression(&ep1, symi);
	if (tp == NULL) {
		error(ERR_IDEXPECT);
		return (nullptr);
	}
	ep1 = makenode(en_compl, ep1, (ENODE*)NULL);
	ep1->constflag = ep1->p[0]->constflag;
	ep1->isUnsigned = ep1->p[0]->isUnsigned;
	ep1->SetType(tp);
	ep1->esize = tp->size;
	return (ep1);
}

TYP* Expression::ParseStar(ENODE** node, Symbol* symi)
{
	ENODE* ep1;
	TYP* tp, *tp1;
	int typ;
	bool sub = false;

	*node = nullptr;
	NextToken();
	tp = ParseCastExpression(&ep1, symi);
	if (tp == NULL) {
		error(ERR_IDEXPECT);
		return (nullptr);
	}
	if (tp->btpp == nullptr)
		error(ERR_DEREF);
	else {
		// A star before a function pointer just means that we want to
		// invoke the function. We want to retain the pointer to the
		// function as the type.
		if (tp->btpp->type != bt_func && tp->btpp->type != bt_ifunc) {
			tp = tp->btpp;
		}
		else
			goto j1;
		//else {
		//	tp1 = tp;
		//	break;	// Don't derefence the function pointer
		//}
	}
	tp1 = tp;
	// Debugging?
	if (tp->type == bt_pointer)
		if (tp->btpp)
			typ = tp->btpp->type;
	// C accepts an autoinc / autodec here.
	if (lastst == autodec || lastst == autoinc) {
		if (lastst == autodec)
			sub = true;
		NextToken();
		ep1 = Autoincdec(tp, &ep1, sub, false);
	}
	tp = CondAddRef(&ep1, tp);
j1:
	*node = ep1;
	ep1->SetType(tp);
	return (tp);
}

ENODE* Expression::ParseSizeof(Symbol* symi)
{
	Declaration decl;
	ENODE* ep1;
	TYP* tp, * tp1;
	Symbol* sp;
	bool flag2 = false;

	NextToken();
	
	if (lastst == openpa) {
		flag2 = true;
		NextToken();
	}
	
//	ParseCastExpression(&ep1);
	if (flag2 && IsBeginningOfTypecast(lastst)) {
		tp = head;
		tp1 = tail;
		decl.ParseSpecifier(0, &sp, sc_none);
		decl.ParsePrefix(FALSE, nullptr, false, nullptr);
		if (decl.head != NULL)
			ep1 = makeinode(en_icon, decl.head->size);
		else {
			error(ERR_IDEXPECT);
			ep1 = makeinode(en_icon, 1);
		}
		head = tp;
		tail = tp1;
	}
	else if (flag2) {
		sizeof_flag++;
		tp = ParseCastExpression(&ep1, symi);
		sizeof_flag--;
		if (tp == 0) {
			error(ERR_SYNTAX);
			ep1 = makeinode(en_icon, 1);
		}
		else
			ep1 = makeinode(en_icon, (long)tp->size);
	}
	else {
		sizeof_flag++;
		tp = ParseUnaryExpression(&ep1, false, symi);
		sizeof_flag--;
		if (tp == 0) {
			error(ERR_SYNTAX);
			ep1 = makeinode(en_icon, 1);
		}
		else
			ep1 = makeinode(en_icon, (long)tp->size);
	}
	if (flag2)
		needpunc(closepa, 2);
	ep1->constflag = TRUE;
	ep1->esize = cpu.sizeOfWord;//??? 8?
	tp = &stdint;
	ep1->SetType(tp);
	return (ep1);
}

ENODE* Expression::ParseAlignof(Symbol* symi)
{
	Declaration decl;
	ENODE* ep1;
	TYP* tp, * tp1;
	Symbol* sp;
	bool flag2 = false;

	NextToken();

	if (lastst == openpa) {
		flag2 = true;
		NextToken();
	}

	//	ParseCastExpression(&ep1);
	if (flag2 && IsBeginningOfTypecast(lastst)) {
		tp = head;
		tp1 = tail;
		decl.ParseSpecifier(0, &sp, sc_none);
		decl.ParsePrefix(FALSE, nullptr, false, nullptr);
		if (decl.head != NULL)
			ep1 = makeinode(en_icon, decl.head->alignment);
		else {
			error(ERR_IDEXPECT);
			ep1 = makeinode(en_icon, 1);
		}
		head = tp;
		tail = tp1;
	}
	else if (flag2) {
		sizeof_flag++;
		tp = ParseCastExpression(&ep1, symi);
		sizeof_flag--;
		if (tp == 0) {
			error(ERR_SYNTAX);
			ep1 = makeinode(en_icon, 1);
		}
		else
			ep1 = makeinode(en_icon, (long)tp->alignment);
	}
	else {
		sizeof_flag++;
		tp = ParseUnaryExpression(&ep1, false, symi);
		sizeof_flag--;
		if (tp == 0) {
			error(ERR_SYNTAX);
			ep1 = makeinode(en_icon, 1);
		}
		else
			ep1 = makeinode(en_icon, (long)tp->alignment);
	}
	if (flag2)
		needpunc(closepa, 2);
	ep1->constflag = TRUE;
	ep1->esize = cpu.sizeOfWord;//??? 8?
	tp = &stdint;
	ep1->SetType(tp);
	return (ep1);
}

ENODE* Expression::ParseTypenum()
{
	ENODE* ep1;
	TYP* tp, * tp1;
	Declaration decl;
	Symbol* sp;

	NextToken();
	needpunc(openpa, 3);
	tp = head;
	tp1 = tail;
	decl.ParseSpecifier(0, &sp, sc_none);
	decl.ParsePrefix(FALSE, nullptr, false, nullptr);
	if (head != NULL)
		ep1 = makeinode(en_icon, head->GetHash());
	else {
		error(ERR_IDEXPECT);
		ep1 = makeinode(en_icon, 1);
	}
	head = tp;
	tail = tp1;
	ep1->constflag = TRUE;
	ep1->esize = 2;
	tp = &stdint;
	if (ep1) ep1->SetType(tp);
	needpunc(closepa, 4);
	return (ep1);
}

ENODE* Expression::ParseNew(bool autonew, Symbol* symi)
{
	ENODE* ep1, *ep2, * ep3, * ep4, * ep5;
	TYP* tp, * tp1;
	Declaration decl;
	Symbol* sp;

	std::string* name = new std::string(autonew ? "__autonew" : "__new");

	currentFn->UsesNew = TRUE;
	currentFn->IsLeaf = FALSE;
	NextToken();
	if (IsBeginningOfTypecast(lastst)) {

		tp = head;
		tp1 = tail;
		decl.ParseSpecifier(0, &sp, sc_none);
		decl.ParsePrefix(FALSE, nullptr, false, nullptr);
		if (head != NULL)
			ep1 = makeinode(en_icon, head->size + 64);
		else {
			error(ERR_IDEXPECT);
			ep1 = makeinode(en_icon, 65);
		}
		ep4 = nullptr;
		ep2 = makeinode(en_icon, head->GetHash());
		ep3 = makenode(en_object_list, nullptr, nullptr);
		//ep4 = makeinode(en_icon, head->typeno);
		ep5 = makenode(en_void, ep1, nullptr);
		//ep5 = nullptr;
		//ep5 = makenode(en_void,ep2,ep5);
		//ep5 = makenode(en_void,ep3,ep5);
		//ep5 = makenode(en_void, ep4, ep5);
		ep2 = makesnode(en_cnacon, name, name, 0);
		ep1 = makefcnode(en_fcall, ep2, ep5, nullptr);
		head = tp;
		tail = tp1;
	}
	else {
		sizeof_flag++;
		tp = ParseUnaryExpression(&ep1, got_pa, symi);
		sizeof_flag--;
		if (tp == 0) {
			error(ERR_SYNTAX);
			ep1 = makeinode(en_icon, 65);
		}
		else
			ep1 = makeinode(en_icon, (int64_t)tp->size+64);
		ep3 = makenode(en_void, ep1, nullptr);
		ep2 = makesnode(en_cnacon, name, name, 0);
		ep1 = makefcnode(en_fcall, ep2, ep3, nullptr);
	}
	ep1->isAutonew = autonew;
	if (autonew)
		currentFn->hasAutonew = true;
	if (ep1) ep1->SetType(tp);
	return (ep1);
}

ENODE* Expression::ParseDelete(Symbol* symi)
{
	ENODE* ep1, *ep2;
	TYP* tp;
	bool needbr = false;

	currentFn->IsLeaf = FALSE;
	NextToken();
	{
		std::string* name = new std::string("__delete");

		if (lastst == openbr)
			NextToken();
		tp = ParseCastExpression(&ep1, symi);
		if (needbr)
			needpunc(closebr, 50);
		tp = AddRef(&ep1, tp);
		ep2 = makesnode(en_cnacon, name, name, 0);
		ep1 = makefcnode(en_fcall, ep2, ep1, nullptr);
		if (ep1) ep1->SetType(tp);
	}
	return (ep1);
}

ENODE* Expression::ParseAddressOf(Symbol* symi)
{
	ENODE* ep1, * ep2;
	TYP* tp, * tp1;

	NextToken();
	tp = ParseCastExpression(&ep1, symi);
	if (tp == NULL) {
		error(ERR_IDEXPECT);
		return (nullptr);
	}
	if (ep1) {
		/*
						t = ep1->tp->type;
		//				if (IsLValue(ep1) && !(t == bt_pointer || t == bt_struct || t == bt_union || t == bt_class)) {
						if (t == bt_struct || t == bt_union || t == bt_class) {
							////ep1 = ep1->p[0];
							//if (ep1) {
							//	ep1 = makenode(en_addrof, ep1, nullptr);
							//	ep1->esize = 8;     // converted to a pointer so size is now 8
							//}
						}
						else */
		ep2 = ep1;
		if (ep1->IsLValue()) {
			if (ep1->nodetype != en_add) {	// array or pointer manipulation
				if (ep1->p[0])	// Cheesy hack
					ep1 = ep1->p[0];
			}
		}
		ep1->esize = cpu.sizeOfPtr;		// converted to a pointer so size is now 8
		tp1 = TYP::Make(bt_pointer, cpu.sizeOfPtr);
		tp1->btpp = tp;
		tp1->val_flag = FALSE;
		tp1->isUnsigned = TRUE;
		tp = tp1;
	}
	if (ep1) ep1->SetType(tp);
	return (ep1);
}

TYP* Expression::ParseVm(ENODE** node, Symbol* symi, int vmn)
{
	ENODE* pnode;
	Symbol* sp;
	TYP* tptr;
	char buf[8];

	NextToken();

	// Do we have: vmn(<expr>)
	if (lastst == openpa) {
		NextToken();
		tptr = expression(&pnode, symi);
		needpunc(closepa, 81);

		// Get the associated vector mask variable
		strcpy_s(buf, sizeof(buf), "_vm?");
		buf[3]= '0' + vmn;
		sp = gsearch2(buf, bt_int, nullptr, false);

		// Better be able to find the compiler defined global variable. It is
		// probably a compiler error if it cannot.
		if (sp == nullptr)
			//error(ERR_VM);
			return (tptr);

		pnode->mask = vmn;
	}
	// Else: process as a variable reference. Easy as it is always a register
	// variable that is an long int.
	else {
		if (symi && symi->fi)
			currentSym = symi;
		tptr = &stdulong;
		pnode = makenode(en_regvar, nullptr, nullptr);
		pnode->rg = 32 + vmn;
		pnode->SetType(tptr);
	}
	if (node)
		*node = pnode;
	return (tptr);
}

ENODE* Expression::ParseMulf(Symbol* symi)
{
	ENODE* ep1, * ep2;
	TYP* tp, * tp1, * tp2;

	NextToken();
	needpunc(openpa, 46);
	ep1 = nullptr;
	tp1 = ParseNonCommaExpression(&ep1, symi);
	needpunc(comma, 47);
	ep2 = nullptr;
	tp2 = ParseNonCommaExpression(&ep2, symi);
	needpunc(closepa, 48);
	ep1 = makenode(en_mulf, ep1, ep2);
	ep1->isUnsigned = TRUE;
	ep1->esize = cpu.sizeOfWord;
	tp = &stduint;
	ep1->SetType(tp);
	return (ep1);
}

ENODE* Expression::ParseSync(Symbol* symi)
{
	ENODE* ep1;
	TYP* tp, * tp1;

	NextToken();
	needpunc(openpa, 46);
	ep1 = nullptr;
	tp1 = ParseNonCommaExpression(&ep1, symi);
	needpunc(closepa, 48);
	ep1 = makenode(en_sync, ep1, nullptr);
	ep1->isUnsigned = TRUE;
	ep1->esize = cpu.sizeOfWord;
	tp = &stduint;
	ep1->SetType(tp);
	return (ep1);
}

ENODE* Expression::ParseBytndx(Symbol* symi)
{
	ENODE* ep1, * ep2;
	TYP* tp, * tp1, * tp2;

	NextToken();
	needpunc(openpa, 46);
	ep1 = nullptr;
	tp1 = ParseNonCommaExpression(&ep1, symi);
	needpunc(comma, 47);
	ep2 = nullptr;
	tp2 = ParseNonCommaExpression(&ep2, symi);
	needpunc(closepa, 48);
	ep1 = makenode(en_bytendx, ep1, ep2);
	ep1->esize = cpu.sizeOfWord;
	tp = &stdint;
	if (ep1) ep1->SetType(tp);
	return (ep1);
}

ENODE* Expression::ParseWydndx(Symbol* symi)
{
	ENODE* ep1, * ep2;
	TYP* tp, * tp1, * tp2;

	NextToken();
	needpunc(openpa, 46);
	ep1 = nullptr;
	tp1 = ParseNonCommaExpression(&ep1, symi);
	needpunc(comma, 47);
	ep2 = nullptr;
	tp2 = ParseNonCommaExpression(&ep2, symi);
	needpunc(closepa, 48);
	ep1 = makenode(en_wydendx, ep1, ep2);
	ep1->esize = cpu.sizeOfWord;
	tp = &stdint;
	if (ep1) ep1->SetType(tp);
	return (ep1);
}

ENODE* Expression::ParseBmap(Symbol* symi)
{
	ENODE* ep1, * ep2;
	TYP* tp, * tp1, * tp2;

	NextToken();
	needpunc(openpa, 46);
	ep1 = nullptr;
	ep2 = nullptr;
	tp1 = ParseNonCommaExpression(&ep1, symi);
	needpunc(comma, 47);
	tp2 = ParseNonCommaExpression(&ep2, symi);
	needpunc(closepa, 48);
	ep1 = makenode(en_bmap, ep1, ep2);
	ep1->esize = cpu.sizeOfWord;
	tp = &stdint;
	if (ep1) ep1->SetType(tp);
	return (ep1);
}

ENODE* Expression::ParseSaveContext(Symbol* symi)
{
	ENODE* ep1;
	TYP* tp;

	NextToken();
	needpunc(openpa, 81);
	needpunc(closepa, 82);
	ep1 = makenode(en_save_context, nullptr, nullptr);
	ep1->esize = cpu.sizeOfWord;
	tp = &stdint;
	if (ep1) ep1->SetType(tp);
	return (ep1);
}

// Parsing other intrinsics

/*
case kw_abs:
	NextToken();
	if (lastst==openpa) {
		flag2 = TRUE;
		NextToken();
	}
			tp = ParseCastExpression(&ep1);
			if( tp == NULL ) {
					error(ERR_IDEXPECT);
					return (TYP *)NULL;
			}
			ep1 = makenode(en_abs,ep1,(ENODE *)NULL);
			ep1->constflag = ep1->p[0]->constflag;
	ep1->isUnsigned = ep1->p[0]->isUnsigned;
	ep1->esize = tp->size;
	if (flag2)
		needpunc(closepa,2);
	break;

case kw_max:
case kw_min:
	{
		TYP *tp1, *tp2, *tp3;

		flag2 = lastst==kw_max;
		NextToken();
		needpunc(comma,2);
		tp1 = ParseCastExpression(&ep1);
		if( tp1 == NULL ) {
			error(ERR_IDEXPECT);
			return (TYP *)NULL;
		}
		needpunc(comma,2);
		tp2 = ParseCastExpression(&ep2);
		if( tp1 == NULL ) {
			error(ERR_IDEXPECT);
			return (TYP *)NULL;
		}
		if (lastst==comma) {
			NextToken();
			tp3 = ParseCastExpression(&ep3);
			if( tp1 == NULL ) {
				error(ERR_IDEXPECT);
				return (TYP *)NULL;
			}
		}
		else
			tp3 = nullptr;
		tp = forcefit(&ep2,tp2,&ep1,tp1,1);
		tp = forcefit(&ep3,tp3,&ep2,tp,1);
		ep1 = makenode(flag2 ? en_max : en_min,ep1,ep2);
		ep1->p[2] = ep3;
		ep1->constflag = ep1->p[0]->constflag & ep2->p[0]->constflag & ep3->p[0]->constflag;
		ep1->isUnsigned = ep1->p[0]->isUnsigned;
		ep1->esize = tp->size;
		needpunc(closepa,2);
	}
	break;
*/

Symbol* Expression::FindMember(TABLE* tbl, char* name)
{
	int ii;
	Symbol* sp, * first, * mbr;
	TYP* tp;

	ii = tbl->FindRising(name);
	if (ii == 0)
		goto j1;
	sp = TABLE::match[ii - 1];
	sp = sp->FindRisingMatch();
	if (sp != nullptr)
		return (sp);
	if (sp == NULL) {
		goto j1;
	}
	if (sp->IsPrivate && sp->parent != currentFn->sym->parent) {
		error(ERR_PRIVATE);
		return (nullptr);
	}
j1:
	first = sp = tbl->headp;
	do {
		if (sp == nullptr)
			break;
		if (sp->name->compare(name) == 0) {
			return (sp);
		}
		sp = sp->nextp;
	} while (sp != first);
	first = sp = tbl->headp;
	do {
		if (sp == nullptr)
			break;
		tp = sp->tp;
		mbr = FindMember(&tp->lst, name);
		if (mbr)
			return (mbr);
		sp = sp->nextp;
	} while (sp != first);
	return (nullptr);
}

Symbol* Expression::FindMember(TYP* tp1, char *name)
{
	int ii;
	Symbol* sp, * first, *mbr;
	TYP* tp;

	ii = tp1->lst.FindRising(name);
	if (ii == 0)
		goto j1;
	sp = TABLE::match[ii - 1];
	sp = sp->FindRisingMatch();
	if (sp != nullptr)
		return (sp);
	if (sp == NULL) {
		goto j1;
	}
	if (sp->IsPrivate && sp->parent != currentFn->sym->parent) {
		error(ERR_PRIVATE);
		return (nullptr);
	}
j1:
	first = sp = tp1->lst.headp;
	do {
		if (sp == nullptr)
			break;
		if (sp->name->compare(name) == 0) {
			return (sp);
		}
		sp = sp->nextp;
	} while (sp != first);
	first = sp = tp1->lst.headp;
	do {
		if (sp == nullptr)
			break;
		tp = sp->tp;
		mbr = FindMember(tp, name);
		if (mbr)
			return (mbr);
		sp = sp->nextp;
	} while (sp != first);
	mbr = FindMember(&tagtable, name);
	return (mbr);
}


ENODE* Expression::ParseDotOperator(TYP* tp1, ENODE *ep1, Symbol* symi)
{
	TypeArray typearray;
	ENODE* ep2, * ep3, * qnode, *n1, *parent;
	TYP* ptp1, * ptp2;
	Symbol* sp, *psp, *sp_parent;
	char* name;
	int ii;
	bool iu;
	Function* fn;
	bool skip_id = false;

	ExpressionHasReference = true;
	parent = ep1;
	NextToken();       /* past -> or . */
	if (tp1 == nullptr) {
		error(ERR_UNDEFINED);
		goto xit;
	}
	if (!tp1->IsStructType() && !(tp1->type == bt_pointer)) {
		error(ERR_NOT_STRUCT);
		goto xit;
	}
	if (tp1->IsVectorType()) {
		ParseNonAssignExpression(&qnode, symi);
		ep2 = makenode(en_shl, qnode, makeinode(en_icon, 3));
		// The dot operation will deference the result below so the
		// old dereference operation isn't needed. It is stripped 
		// off by using ep->p[0] rather than ep.
		ep1 = makenode(en_add, ep1->p[0], ep2);
		tp1 = tp1->btpp;
		tp1 = CondAddRef(&ep1, tp1);
		goto xit;
	}
	if (lastst != id) {
		error(ERR_IDEXPECT);
		goto xit;
	}
	dfs.printf("dot search: %p\r\n", (char*)&tp1->lst);
	ptp1 = tp1;
	pep1 = ep1;
	name = compiler.lastid;
	//sp = FindMember(tp1, name);
	
	// If nothing is found under the type passed in, check the second type field
	// to see if the identifier can be found there. Pointers to structs and 
	// functions use the 'b' type field to store the pointed to type.
	ii = tp1->lst.FindRising(name);
	if (ii == 0) {
		ptp2 = tp1->btpp;
		while (ptp2 != nullptr) {
			ii = ptp2->lst.FindRising(name);
			if (ii != 0)
				break;
			ptp2 = ptp2->btpp;
		}
	}
	n1 = ep1;
	sp_parent = nullptr;
	if (n1) {
		if (n1->nodetype == en_ref) {
			if (n1->sym) {
				sp_parent = n1->sym;
				if (n1->sym->name->compare(name) == 0) {
					sp = n1->sym;
					goto j1;
				}
			}
		}
	}
	if (ii == 0) {
		dfs.printf("Nomember1");
		error(ERR_NOMEMBER);
		goto xit;
	}
// It is possible to get multiple matches of the member, so choose the best
	// match.
	sp = TABLE::match[ii - 1];
	sp = sp->FindRisingMatch();
	if (sp == NULL) {
		dfs.printf("Nomember2");
		error(ERR_NOMEMBER);
		goto xit;
	}
j1:
	// Check the accessibility of the field.
	if (sp->IsPrivate && sp->parent != currentFn->sym->parent) {
		error(ERR_PRIVATE);
		goto xit;
	}
	
	if (sp == nullptr) {
		dfs.printf("Nomember1");
		error(ERR_NOMEMBER);
		goto xit;
	}
	tp1 = sp->tp;
	dfs.printf("tp1->type:%d", tp1->type);
	if (tp1 == nullptr)
		throw new ArplException(ERR_NULLPOINTER, 5);
	if (tp1->type == bt_ifunc || tp1->type == bt_func) {
		// build the name vector and create a nacon node.
		dfs.printf("%s is a func\n", (char*)sp->name->c_str());
		NextToken();
		if (lastst == openpa) {
			NextToken();
			ep2 = ParseArgumentList(pep1, &typearray, symi);
			typearray.Print();
			fn = Function::FindExactMatch(ii, name, bt_int, &typearray);
			psp = sp;
			sp = nullptr;
			if (fn)
				sp = fn->sym;
			if (sp) {
				//						sp = TABLE::match[TABLE::matchno-1];
				ep3 = makesnode(en_cnacon, sp->name, sp->mangledName, sp->value.i);
				ep3->sym = psp;
				ep3->isPascal = sp->fi->IsPascal;
				ep1 = makenode(en_fcall, ep3, ep2);
				ep1->p[2] = parent;
				ep1->isPascal = ep3->isPascal;
				tp1 = sp->tp->btpp;
				currentFn->IsLeaf = FALSE;
			}
			else {
				error(ERR_METHOD_NOTFOUND);
				goto xit;
			}
			ep1->SetType(tp1);
			goto xit;
		}
		// Else: we likely wanted the addres of the function since the
		// function is referenced without o parameter list indicator. Goto
		// the regular processing code.
		goto j2;
	}
	else {
		skip_id = true;
	j2:
		dfs.printf("tp1->type:%d", tp1->type);
		if (tp1)
			iu = tp1->isUnsigned;
		else
			iu = ep1->isUnsigned;
		//if (sp->value.i)
		{
			qnode = makeinode(en_icon, sp->value.i);
			qnode->constflag = TRUE;
			if (sp->tp->bit_offset) {
				qnode->bit_offset = makeinode(en_icon, sp->tp->bit_offset->i);
				qnode->bit_width = makeinode(en_icon, sp->tp->bit_width->i);
			}
			ep1 = makenode(en_add, ep1, qnode);
			ep1->bit_offset = qnode->bit_offset;
			ep1->bit_width = qnode->bit_width;
			ep1->isPascal = ep1->p[0]->isPascal;
			ep1->constflag = ep1->p[0]->constflag;
		}
		/*
		else if (ep1)
			ep1->constflag = TRUE;
		*/
		if (ep1) {
			ep1->sym = sp;
			ep1->isUnsigned = iu;
			ep1->esize = cpu.sizeOfWord;
		}
//		ep1->p[2] = pep1;
		//if (tp1->type==bt_pointer && (tp1->btpp->type==bt_func || tp1->btpp->type==bt_ifunc))
		//	dfs.printf("Pointer to func");
		//else
		tp1 = CondAddRef(&ep1, tp1);
		if (ep1)
			ep1->SetType(tp1);
		dfs.printf("tp1->type:%d", tp1->type);
	}
	if (skip_id)
		NextToken();       /* past id */
	dfs.printf("B");
xit:
	if (ep1) {
		ep1->tp = tp1;
	}
	return (ep1);
}

ENODE* Expression::ParsePointsTo(TYP* tp1, ENODE* ep1)
{
	TYP* tp2;
	ENODE* ep2;

	ep2 = ep1;
	tp2 = tp1;
	if (tp1 == NULL) {
		error(ERR_UNDEFINED);
		goto xit;
	}
	if (tp1->IsStructType()) {
		//printf("hello");
		//ep1 = makenode(reftype, ep1, (ENODE *)NULL);
	}
	else
j1:
	if (tp1->type != bt_pointer) {
		if (tp1->type == bt_func || 
			tp1->type == bt_ifunc ||
			tp1->type == bt_struct ||
			tp1->type == bt_union ||
			tp1->type == bt_class) {
			tp2 = tp1;
			tp1 = tp1->btpp;
			goto j1;
		}
		error(ERR_NOPOINTER);
	}
	else {
		tp1 = tp1->btpp;
		if (tp1->val_flag == false && false) {
			ep1 = makenode(en_ref, ep1, (ENODE*)NULL);
			ep1->isPascal = ep1->p[0]->isPascal;
		}
		ep1->tp = tp2->btpp;
		//if (ep1->tp == nullptr)
		//	printf("hi");
	}
xit:
	//	if (ep1) ep1->tp = tp1;
	return (ep1);
}

ENODE* Expression::ParseOpenpa(TYP* tp1, ENODE* ep1, Symbol* symi)
{
	TypeArray typearray;
	ENODE* ep2, * ep3, * ep4;
	TYP* tp2, * tp3;
	Symbol* sp;
	char* name;
	std::string nme;

	NextToken();
	if (tp1 == NULL) {
		error(ERR_UNDEFINED);
		goto xit;
	}
	tp2 = ep1->tp;
	if (tp2 == nullptr) {
		error(ERR_UNDEFINED);
		goto xit;
	}
	if (tp2->type == bt_vector_mask) {
		NextToken();
		tp1 = expression(&ep2, symi);
		needpunc(closepa, 9);
		ApplyVMask(ep2, ep1);
		ep1 = ep2;
		goto xit;
	}
	if (tp2->type == bt_pointer) {
		dfs.printf("Got function pointer.\n");
	}
	dfs.printf("tp2->type=%d", tp2->type);
	name = compiler.lastid;
	//NextToken();
	tp3 = tp1->btpp;
	ep4 = nullptr;
	if (tp3) {
		if (tp3->type == bt_struct || tp3->type == bt_union || tp3->type == bt_class)
			ep4 = makenode(en_regvar, NULL, NULL);
	}
	//ep2 = ArgumentList(ep1->p[2],&typearray);
	ep2 = ParseArgumentList(ep4, &typearray, symi);
	typearray.Print();
	dfs.printf("Got Type: %d", tp1->type);
	if (tp1->type == bt_pointer) {
		dfs.printf("Got function pointer.\n");
		ep1 = makefcnode(en_fcall, ep1, ep2, nullptr);
		if (currentFn)
			currentFn->IsLeaf = FALSE;
		goto xit;
	}
	dfs.printf("openpa calling gsearch2");
	sp = ep1->sym;
	/*
	sp = nullptr;
	ii = tp1->lst.FindRising(name);
	if (ii) {
		sp = Function::FindExactMatch(TABLE::matchno, name, bt_long, &typearray)->sym;
	}
	if (!sp)
		sp = gsearch2(name,bt_long,&typearray,true);
	*/
	if (sp == nullptr) {
		sp = Symbol::alloc();
		sp->fi = MakeFunction(sp->id, sp, defaultcc == 1);
		sp->storage_class = sc_external;
		sp->SetName(name);
		sp->tp = TYP::Make(bt_func, 0);
		sp->tp->btpp = TYP::Make(bt_int, cpu.sizeOfInt);
		sp->fi->AddProto(&typearray);
		sp->mangledName = sp->fi->BuildSignature();
		gsyms[0].insert(sp);
	}
	else if (sp->IsUndefined) {
		sp->tp = TYP::Make(bt_func, 0);
		sp->tp->btpp = TYP::Make(bt_int, cpu.sizeOfInt);
		if (!sp->fi) {
			sp->fi = MakeFunction(sp->id, sp, defaultcc == 1);
		}
		sp->fi->AddProto(&typearray);
		sp->mangledName = sp->fi->BuildSignature();
		gsyms[0].insert(sp);
		sp->IsUndefined = false;
	}
	if (sp->tp->type == bt_pointer) {
		dfs.printf("Got function pointer");
		ep1 = makefcnode(en_fcall, ep1, ep2, sp);
		currentFn->IsLeaf = FALSE;
	}
	else {
		dfs.printf("Got direct function %s ", (char*)sp->name->c_str());
		ep3 = makesnode(en_cnacon, sp->name, sp->mangledName, sp->value.i);
		ep1 = makefcnode(en_fcall, ep3, ep2, sp);
		//if (sp->fi)
		{
			if (currentFn == nullptr)
				currentFn = sp->fi;
			if (sp->fi == nullptr)
				error(ERR_NOFUNC);
			else if (!sp->fi->IsInline)
				currentFn->IsLeaf = FALSE;
		}
		//else
		//	currentFn->IsLeaf = FALSE;
	}
	tp1 = sp->tp->btpp;
	//			tp1 = ExprFunction(tp1, &ep1);
xit:
	if (ep1) ep1->tp = tp1;
	return (ep1);
}

/* Dead code.
* 
* GetSizesForArray fills in the Expression member variable sa, for the array
* size array. 
* 
* Parameter:
*		tp: (input) the array type
* Returns:
*		none
*/

void Expression::GetSizesForArray(TYP* tp)
{
	TYP* tp4;
	int cnt2;
	int elesize;

	return;
	if (tp->type == bt_pointer && !tp->val_flag) {
		elesize = cpu.GetTypeSize(tp->btpp->type);
		numdimen = tp->btpp->dimen;
		//sa[numdimen + 1] = ep1->esize;
	}
	else {
		elesize = cpu.GetTypeSize(tp->type);
		numdimen = tp->dimen;
	}
	// Track down the size of each dimension.
	if (cnt == 0) {
		cnt2 = 0;
		for (tp4 = tp; tp4; tp4 = tp4->btpp) {
			sa[cnt2] = max(tp4->size, 1);
			elesize = cpu.GetTypeSize(tp4->type);
			cnt2++;
			if (cnt2 > 9) {
				error(ERR_TOOMANYDIMEN);
				break;
			}
		}
		if (numdimen == 0)
			numdimen = cnt2-1;
		sa[numdimen] = elesize;
		//sa[0] = sa[numdimen];
	}
}

/* Parses a bit field specification on a scalar intrinsic type.
*  A bitfield spec looks like a[1:4] or to select just a single bit a[3].
* 
* Parameter:
*		pnode: (input) pointer to node of expression tree.
* Returns:
*		a node containing the bitfield spec.
*/

ENODE* Expression::ParseBitfieldSpec(TYP* tp3, ENODE* pnode, ENODE* varnode)
{
	TYP* tp1;
	ENODE* qnode;
	ENODE* rnode;
	ENODE* snode;
	ENODE* ep1;

	rnode = nullptr;
	ep1 = nullptr;
	if (lastst == colon) {
		NextToken();
		tp3 = ParseExpression(&qnode, nullptr);
		snode = qnode;
		qnode = compiler.ef.Makenode(en_sub, pnode->Clone(), qnode);
		//qnode = compiler.ef.Makenode(en_sub, qnode, makeinode(en_icon, 1));
		//ep1 = compiler.ef.Makenode(pnode->isUnsigned ? en_extu : en_ext, rnode, pnode, qnode);
		rnode = makenode(en_fieldref, nullptr, nullptr);
		rnode->bit_offset = snode;
		rnode->bit_width = qnode;
		rnode->esize = tp3->size;
		//ep1 = compiler.ef.Makenode(en_void, rnode, nullptr);
	}
	else {
		snode = pnode->Clone();
		qnode = makeinode(en_icon, 0);
		//ep1 = compiler.ef.Makenode(pnode->isUnsigned ? en_extu : en_ext, rnode, pnode->Clone(), qnode->Clone());
		rnode = makenode(en_fieldref, varnode, nullptr);
		rnode->bit_offset = pnode;
		rnode->bit_width = qnode;
		rnode->esize = tp3->size;
		//ep1 = compiler.ef.Makenode(en_void, rnode, nullptr);
	}
	ep1 = rnode;//compiler.ef.Makenode(en_bitoffset, rnode, snode, qnode);
	//rnode->bit_offset = pnode;
	//rnode->bit_width = qnode;
	//ep1->bit_offset = pnode->Clone();
	//ep1->bit_width = qnode->Clone();
	needpunc(closebr, 9);
	tp1 = TYP::Make(bt_bitfield, cpu.sizeOfWord);
	tp1->bit_offset = snode;
	tp1->bit_width = qnode;
	ep1->tp = tp1;
	tp1 = CondAddRef(&ep1, tp1);
	return (ep1);
}

ENODE* Expression::ParseOpenbr(TYP* tp1, ENODE* ep1)
{
	ENODE* pnode, * rnode, * qnode;
	TYP* tp2, * tp3, * tp4;
	Symbol* sp1;
	int64_t elesize, sz1;
	bool cf = false;	// constant flag
	bool uf = false;	// unsigned flag
	bool undimensioned = false;

	rnode = nullptr;
	pnode = ep1;
	if (tp1 == nullptr) {
		error(ERR_UNDEFINED);
		return (ep1);
	}
	NextToken();
	if (lastst == closebr)
		undimensioned = true;
	if (!undimensioned) {
		/* one must be a pointer, the other a integral type */
		if (tp1->type == bt_pointer) {
			tp2 = ParseExpression(&rnode, nullptr);
			tp3 = tp1;
			tp4 = tp1;
			if (rnode == nullptr) {
				error(ERR_EXPREXPECT);
				throw new ArplException(ERR_EXPREXPECT, 9);
			}
			if (!tp2->IsIntegralType()) {
				error(ERR_BADARRAYNDX);
				throw new ArplException(ERR_BADARRAYNDX, 12);
			}
		}
		else {
			tp2 = tp1;
			rnode = pnode;
			tp3 = ParseExpression(&pnode, nullptr);
			// Type transfers need to be improved for this to work.
			if (tp3->type != bt_pointer) {
				//error(ERR_NOPOINTER);
				//throw new C64PException(ERR_NOPOINTER, 11);
			}
			if (tp3 == NULL) {
				error(ERR_UNDEFINED);
				throw new ArplException(ERR_UNDEFINED, 10);
				goto xit;
			}
			tp1 = tp3;
			tp4 = tp1;
		}
	}
	else {
		tp3 = tp1;
		tp4 = tp1;
	}
	
	// Debug
	if (cnt == 0)
		totsz = tp1->size;
	if (tp1->type != bt_pointer)
		return (ParseBitfieldSpec(tp3, pnode, rnode));

	// Track down the size of each dimension.
//	GetSizesForArray(tp1);

	if (tp1->btpp)
		tp1 = tp1->btpp;
	//if (cnt==0) {
	//	switch(numdimen) {
	//	case 1: sz1 = sa[numdimen+1]; break;
	//	case 2: sz1 = sa[1]*sa[numdimen+1]; break;
	//	case 3: sz1 = sa[1]*sa[2]*sa[numdimen+1]; break;
	//	default:
	//		sz1 = sa[numdimen+1];	// could be a void = 0
	//		for (cnt2 = 1; cnt2 < numdimen; cnt2++)
	//			sz1 = sz1 * sa[cnt2];
	//	}
	//}
	//else if (cnt==1) {
	//	switch(numdimen) {
	//	case 2:	sz1 = sa[numdimen+1]; break;
	//	case 3: sz1 = sa[1]*sa[numdimen+1]; break;
	//	default:
	//		sz1 = sa[numdimen+1];	// could be a void = 0
	//		for (cnt2 = 1; cnt2 < numdimen-1; cnt2++)
	//			sz1 = sz1 * sa[cnt2];
	//	}
	//}
	//else if (cnt==2) {
	//	switch(numdimen) {
	//	case 3: sz1 = sa[numdimen+1]; break;
	//	default:
	//		sz1 = sa[numdimen+1];	// could be a void = 0
	//		for (cnt2 = 1; cnt2 < numdimen-2; cnt2++)
	//			sz1 = sz1 * sa[cnt2];
	//	}
	//}
	//else
	sz1 = tp1->size;

	qnode = makeinode(en_icon, sz1);
	qnode->etype = bt_ushort;
	qnode->esize = cpu.sizeOfWord;
	qnode->isUnsigned = TRUE;
	cf = qnode->constflag;
	/*
	if (numdimen == 1 && cf && pnode->nodetype==en_nacon) {
		pnode = compiler.ef.Makenode(en_scndx, qnode, rnode, pnode);
	}
	else
	*/
	// qnode contains the size of the element multiplied by the index amount.
	// The base size of an element was stuffed in sa[numdimen].
	elesize = sz1;// sa[numdimen];
	if (cf && qnode->i == elesize &&
		(elesize==1 || elesize==2 || elesize==4 || elesize==8 || elesize==16)) {
		qnode = rnode;
		qnode->scale = (char)elesize;
	}
	else {
		qnode = makenode(en_mulu, qnode, rnode);
		qnode->scale = 1;
	}
	qnode->etype = bt_short;
	qnode->esize = cpu.sizeOfWord;
	qnode->constflag = cf & rnode->constflag;
	qnode->isUnsigned = rnode->isUnsigned;
	if (rnode->sym)
		qnode->sym = rnode->sym;

	//(void) cast_op(&qnode, &tp_int32, tp1);
	cf = pnode->constflag;
	uf = pnode->isUnsigned;
	sp1 = pnode->sym;

	pnode = makenode(en_add, pnode, qnode);
	pnode->etype = bt_pointer;
	pnode->esize = cpu.sizeOfPtr;
	pnode->constflag = cf & qnode->constflag;
	pnode->isUnsigned = uf & qnode->isUnsigned;
	if (pnode->sym == nullptr)
		pnode->sym = sp1;
	if (pnode->sym == nullptr)
		pnode->sym = qnode->sym;

	tp1 = CondAddRef(&pnode, tp1);
	pnode->tp = tp1;
	ep1 = pnode;
	needpunc(closebr, 9);
	cnt++;
xit:
	if (ep1 == nullptr)
		printf("jo");
	ep1->tp = tp1;
	return (ep1);
}

bool Expression::ParseGenericCase(ENODE** node, TYP* tp1, Symbol* symi, int count, 
	short int* defcount, ENODE** ep_def, TYP** head, TYP** tp2, ENODE** ep4)
{
	bool found = false;
	Declaration decl;
	Symbol* sp;
	TYP* tph, *tpt;

	if (lastst == kw_default) {
		*defcount = count;
		NextToken();
	}
	else
		needpunc(kw_case, 59);

	decl.itable = nullptr;
	decl.head = nullptr;
	decl.tail = nullptr;
	decl.istorage_class = sc_member;
	decl.ParseSpecifier(0, &sp, sc_none);
	decl.ParsePrefix(false, nullptr, false, nullptr);
	tph = decl.head;
	tpt = decl.tail;

	needpunc(colon, 60);
	*ep4 = nullptr;
	do {
		*tp2 = ParseExpression(ep4, symi);
		if (*ep4 == nullptr)
			*tp2 = &stdint;
		else
			*tp2 = (*ep4)->tp;
		if (lastst == semicolon)
			NextToken();
		if (lastst == kw_break) {
			NextToken();
			if (lastst == semicolon)
				NextToken();
			break;
		}
		if (lastst == e_sym::end)
			break;
		if (lastst == my_eof) {
			error(ERR_EOF_REACHED);
			break;
		}
	} while (false);
	if (count == *defcount)
		*ep_def = *ep4;
	if (TYP::IsSameType(tph, tp1, true)) {
		found = true;
		if (ep4) {
			*node = (*ep4)->Clone();
			(*node)->SetType(tph);
		}
		else
			*node = nullptr;
	}
	return (found);
}

TYP* Expression::ParseGenericSwitch(ENODE** node, Symbol* symi)
{
	bool got_pap = false;
	bool got_begin = false;
	bool found = false;
	Declaration decl;
	TYP* tp1, *tp2, *tprh;
	TYP* tp_found;
	TYP* tph[100];
	ENODE* ep1, * ep2, * ep4, *ep_def;
	ENODE* ep_found;
	short int count, defcount;
	Statement* stmt;

	tp1 = tp2 = nullptr;
	tp_found = nullptr;
	ep1 = ep_def = nullptr;
	stmt = nullptr;
	defcount = -1;

	needpunc(e_sym::begin, 58);
	ZeroMemory(tph, sizeof(tph));
	ep2 = nullptr;
	ep_found = nullptr;
	for (count = 0; count < 100; count++) {
		if (lastst == e_sym::end) {
			NextToken();
			break;
		}
		if (ParseGenericCase(node, tp1, symi, count, &defcount,
			&ep_def, &tph[count], &tp2, &ep4)) {
			found = true;
			ep_found = ep4->Clone();
			tp_found = ep4->tp;
		}
	}
	if (!found && defcount >= 0) {
		tprh = tph[defcount];
		//ep_def->tp = forcefit(&ep_def, tp2, node, tp1, false, true);
		//ep_def->SetType(ep_def->tp);
		*node = ep_def;
		return (ep_def->tp);
	}
	if (found) {
		//(*node)->tp = forcefit(&ep_found, tp_found, node, tp1, false, true);
		*node = ep_found;
		(*node)->SetType(tp1);
		return ((*node)->tp);
	}
	*node = makeinode(en_icon, 0);
	return (&stdint);
}

TYP* Expression::ParseSwitchExpr(ENODE** node, Symbol* symi)
{
	int64_t* cv;
	TYP* tp, *first_tp;
	ENODE* ep1, *ep2;

	ep1 = nullptr;
	ep2 = nullptr;
	first_tp = nullptr;
	needpunc(e_sym::begin, 66);
	while (lastst == kw_case || lastst == kw_default) {
		if (lastst == kw_case) {
			cv = Statement::GetCasevals();
			tp = ParseExpression(&ep2, symi);
			ep1 = makenode(en_case, ep1, ep2);
			if (ep2)
				ep2->p[2] = (ENODE *)cv;
		}
		// Default case
		else {
			NextToken();
			if (lastst == colon)
				NextToken();
			tp = ParseExpression(&ep2, symi);
			ep1 = makenode(en_default, ep1, ep2);
		}
		if (first_tp == nullptr)
			first_tp = tp;
		needpunc(semicolon, 68);
	}
	needpunc(e_sym::end, 67);
	if (first_tp == nullptr)
		first_tp = &stdint;
	if (node) {
		*node = ep1;
		(*node)->SetType(first_tp);
	}
	return (first_tp);
}

TYP* Expression::ParseGenericCase(ENODE** node, Symbol* symi, TYP* tp1)
{
	Declaration decl;
	TYP* tph, * tpt;
	TYP* tp;
	ENODE* ep1, *defcase;
	Symbol* sp;
	bool found = false;

	defcase = nullptr;
	sp = allocSYM();
	do {
		if (lastst == closepa)
			break;
		if (lastst == kw_default) {
			NextToken();
			if (lastst == colon)
				NextToken();
			tp = ParseNonCommaExpression(&defcase, symi);
		}
		else {
			decl.itable = nullptr;
			decl.head = nullptr;
			decl.tail = nullptr;
			decl.istorage_class = sc_member;
			decl.ParseSpecifier(0, &sp, sc_none);
			decl.ParsePrefix(false, nullptr, false, nullptr);
			tph = decl.head;
			tpt = decl.tail;
			needpunc(colon, 72);
			tp = ParseNonCommaExpression(&ep1, symi);
			needpunc(comma, 73);
			if (TYP::IsSameType(tph, tp1, true)) {
				found = true;
				if (ep1) {
					*node = ep1->Clone();
					(*node)->SetType(tph);
				}
				else
					*node = nullptr;
			}
		}
	} while (lastst != closepa);
	if (!found && defcase) {
		*node = defcase;
		(*node)->SetType(defcase->tp);
	}
	return ((*node)->tp);
}

TYP* Expression::ParseGeneric(ENODE** node, Symbol* symi)
{
	TYP* tp;
	ENODE* ep1;

	NextToken();
	needpunc(openpa,69);
	tp = ParseNonCommaExpression(&ep1, symi);
	needpunc(comma, 71);
	tp = ParseGenericCase(node, symi, tp);
	needpunc(closepa,70);
	return (tp);
}

TYP* Expression::ParseMux(ENODE** node, Symbol* symi)
{
	ENODE* ep1, *ep2;
	TYP* tp1;

	ep1 = nullptr;
	do {
		tp1 = ParseExpression(&ep2, symi);
		ep1 = makenode(en_void, ep1, ep2);
		if (lastst == colon)
			NextToken();
		else
			break;
	} while (true);
	*node = ep1;
	return (tp1);
}

TYP* Expression::ParseSwitch(ENODE** node, Symbol* symi)
{
	bool got_pap = false;
	bool nkd = false;
	bool genric = false;
	ENODE* ep1, *sw_ep, *sw_case;
	TYP* tp1;

	NextToken();	// skip past "switch"
	if (lastst == openpa) {
		got_pap = true;
		NextToken();
	}
	tp1 = ParseExpression(&sw_ep, symi);
	if (lastst == semicolon) {
		NextToken();
		do {
			if (lastst == kw_generic) {
				genric = true;
				NextToken();
			}
			else if (lastst == kw_naked) {
				nkd = true;
				NextToken();
			}
		} while (lastst==kw_generic || lastst == kw_naked);
	}
	if (got_pap)
		needpunc(closepa, 57);
	if (genric) {
		tp1 = ParseGenericSwitch(&sw_case, symi);
		ep1 = makenode(en_switch, sw_ep, sw_case);
		ep1->SetType(tp1);
		*node = ep1;
		return (tp1);
	}
	tp1 = ParseSwitchExpr(&sw_case, symi);
	ep1 = makenode(en_switch, sw_ep, sw_case);
	ep1->SetType(tp1);
	*node = ep1;
	return (tp1);
}

void Expression::force_type(ENODE* dstnode, ENODE* srcnode, TYP* tp)
{
	dstnode->isUnsigned = tp->isUnsigned;
	dstnode->constflag = true;
	if (tp != nullptr) {
		// Copy a scalar.
		if (tp->IsScalar()) {
			dstnode->nodetype = en_icon;
			// Convert float to integer?
			if (srcnode->tp->IsFloatType()) {
				Float128::FloatToInt(&dstnode->i, &srcnode->f128);
				dstnode->i128.low = srcnode->i;
				if (dstnode->i & 0x8000000000000000LL)
					dstnode->i128.high = 0xFFFFFFFFFFFFFFFFLL;
				else
					dstnode->i128.high = 0LL;
			}
			// Copy integer value
			else {
				dstnode->i = srcnode->i;
				dstnode->i128 = srcnode->i128;
				dstnode->f = srcnode->f;
				dstnode->f128 = srcnode->f128;
				dstnode->p[0] = nullptr;
				dstnode->p[1] = nullptr;
			}
		}
		// Copy float constant.
		else if (dstnode->p[1]->tp->IsFloatType()) {
			dstnode->nodetype = en_fcon;
			if (srcnode->tp->IsScalar()) {
				Float128::IntToFloat(&dstnode->f128, srcnode->i);
				dstnode->f = (double)srcnode->i;
				dstnode->i = srcnode->i;	// i=literal index
				dstnode->i128 = srcnode->i128;
			}
			else {
				dstnode->i = srcnode->i;	// i=literal index
				dstnode->i128 = srcnode->i128;
				dstnode->f = srcnode->f;
				dstnode->f128 = srcnode->f128;
			}
			dstnode->p[0] = nullptr;
			dstnode->p[1] = nullptr;
		}
	}
	tp = dstnode->tp;
}

ENODE* Expression::MakeStaticNameNode(Symbol* sp)
{
	std::string stnm;
	ENODE* node;

	if (sp->tp->type == bt_func || sp->tp->type == bt_ifunc) {
		//strcpy(stnm,GetNamespace());
		//strcat(stnm,"_");
		stnm = "";
		stnm += *sp->name;
		node = makesnode(en_cnacon, new std::string(stnm), sp->fi->BuildSignature(), sp->value.i);
		node->isPascal = sp->fi->IsPascal;
		node->constflag = TRUE;
		node->esize = 8;
		//*node = makesnode(en_nacon,sp->name);
		//(*node)->constflag = TRUE;
	}
	else {
		node = makeinode(en_labcon, sp->value.i);
		node->constflag = false;
		node->esize = sp->tp->size;//8;
		node->segment = sp->segment;
	}
	if (sp->tp->isUnsigned) {
		node->isUnsigned = TRUE;
		node->esize = sp->tp->size;
	}
	node->etype = bt_pointer;//sp->tp->type;
	return (node);
}

ENODE* Expression::MakeThreadNameNode(Symbol* sp)
{
	ENODE* node;

	node = makeinode(en_labcon, sp->value.i);
	node->segment = tlsseg;
	node->constflag = FALSE;
	node->esize = sp->tp->size;
	node->etype = bt_pointer;//sp->tp->type;
	if (sp->tp->isUnsigned)
		node->isUnsigned = TRUE;
	return (node);
}

ENODE* Expression::MakeGlobalNameNode(Symbol* sp)
{
	ENODE* node;

	if (sp->tp->type == bt_func || sp->tp->type == bt_ifunc) {
		node = makesnode(en_cnacon, sp->name, sp->mangledName, sp->value.i);
		node->isPascal = sp->fi->IsPascal;
	}
	else {
		node = makesnode(en_nacon, sp->name, sp->mangledName, sp->value.i);
//		node->segment = dataseg;
	}
	node->constflag = FALSE;
	if (sp->tp->btpp)
		node->esize = sp->tp->btpp->size;
	else
		node->esize = sp->tp->size;
	node->etype = bt_pointer;//sp->tp->type;
	node->isUnsigned = TRUE;// sp->tp->isUnsigned;
	node->segment = sp->segment;
	return (node);
}

ENODE* Expression::MakeExternNameNode(Symbol* sp)
{
	ENODE* node;

	if (sp->tp->type == bt_func || sp->tp->type == bt_ifunc) {
		node = makesnode(en_cnacon, sp->name, sp->mangledName, sp->value.i);
		if (sp->fi)
			node->isPascal = sp->fi->IsPascal;
		node->constflag = TRUE;
	}
	else {
		node = makesnode(en_nacon, sp->name, sp->mangledName, sp->value.i);
		node->segment = dataseg;
		node->constflag = FALSE;
	}
	node->esize = sp->tp->size;
	node->etype = bt_pointer;//sp->tp->type;
	node->isUnsigned = TRUE;// sp->tp->isUnsigned;
	return (node);
}

ENODE* Expression::MakeConstNameNode(Symbol* sp)
{
	ENODE* node;

	if (sp->tp->type == bt_quad)
		node = makefqnode(en_fqcon, &sp->f128);
	else if (sp->tp->type == bt_float || sp->tp->type == bt_double)
		node = compiler.ef.Makefnode(en_fcon, sp->value.f);
	else if (sp->tp->type == bt_posit)
		node = compiler.ef.Makepnode(en_pcon, sp->p);
	else {
		node = makeinode(en_icon, sp->value.i);
		if (sp->tp->isUnsigned)
			node->isUnsigned = TRUE;
	}
	node->constflag = TRUE;
	node->esize = sp->tp->size;
	node->segment = use_iprel ? codeseg : rodataseg;
	return (node);
}

ENODE* Expression::MakeMemberNameNode(Symbol* sp)
{
	ENODE* node;

	// If it's a member we need to pass r25 the class pointer on
	// the stack.
	isMember = true;
	node = nullptr;
	if ((sp->tp->type == bt_func || sp->tp->type == bt_ifunc)
		|| (sp->tp->type == bt_pointer && (sp->tp->btpp->type == bt_func || sp->tp->btpp->type == bt_ifunc)))
	{
		if (sp->fi) {
			node = makesnode(en_cnacon, sp->name, sp->fi->BuildSignature(), 25);
			node->isPascal = sp->fi->IsPascal;
		}
	}
	else {
		node = makeinode(en_classcon, sp->value.i);
	}
	if (node) {
		if (sp->tp->isUnsigned || sp->tp->type == bt_pointer)
			node->isUnsigned = TRUE;
		node->esize = sp->tp->size;
		switch (node->nodetype) {
		case en_regvar:		node->etype = bt_int;	break;//sp->tp->type;
		case en_fpregvar:	node->etype = sp->tp->type;	break;//sp->tp->type;
		case en_pregvar:	node->etype = sp->tp->type;	break;//sp->tp->type;
		default:			node->etype = bt_pointer; break;//sp->tp->type;
		}
	}
	return (node);
}

ENODE* Expression::MakeNameNode(Symbol *sp)
{
	ENODE* node;

	node = nullptr;
	switch (sp->storage_class) {

	case sc_static:
		node = MakeStaticNameNode(sp);
		break;

	case sc_thread:
		node = MakeThreadNameNode(sp);
		break;

	case sc_global:
		node = MakeGlobalNameNode(sp);
		break;

	case sc_external:
		node = MakeExternNameNode(sp);
		break;

	case sc_const:
		node = MakeConstNameNode(sp);
		break;

	case sc_member:
		node = MakeMemberNameNode(sp);
		break;

	case sc_auto:
		node = MakeAutoNameNode(sp);
		break;

	case sc_label:
		node = makesnode(en_cnacon, sp->name, sp->mangledName, sp->value.i);
		break;

	default:        /* auto and any errors */
		error(ERR_ILLCLASS);
	}
	return (node);
}

ENODE* Expression::MakeAutoNameNode(Symbol* sp)
{
	ENODE* node;

	if (sp == nullptr)
		return (nullptr);
	if (sp->tp->IsVectorType())
		node = makeinode(en_autovcon, sp->value.i);
	else if (sp->tp->type == bt_vector_mask)
		node = makeinode(en_autovmcon, sp->value.i);
	else if (sp->tp->IsFloatType())
		node = makeinode(en_autofcon, sp->value.i);
	else if (sp->tp->IsPositType())
		node = makeinode(en_autopcon, sp->value.i);
	else {
		node = makeinode(en_autocon, sp->value.i);
		node->bit_offset = sp->tp->bit_offset;
		node->bit_width = sp->tp->bit_width;
		if (sp->tp->isUnsigned)
			node->isUnsigned = TRUE;
	}
	if (sp->IsRegister) {
		if (sp->tp->IsFloatType())
			node->nodetype = en_fpregvar;
		else if (sp->tp->IsPositType())
			node->nodetype = en_pregvar;
		else
			node->nodetype = en_regvar;
		//(*node)->i = sp->reg;
		node->rg = sp->reg;
		node->tp = sp->tp;
		//(*node)->tp->val_flag = TRUE;
	}
	node->esize = sp->tp->size;
	switch (node->nodetype) {
	case en_regvar:		node->etype = bt_int;	break;//sp->tp->type;
	case en_fpregvar:	node->etype = sp->tp->type;	break;//sp->tp->type;
	case en_pregvar:	node->etype = sp->tp->type;	break;//sp->tp->type;
	default:			node->etype = bt_pointer;break;//sp->tp->type;
	}
	return (node);
}

ENODE* Expression::MakeUnknownFunctionNameNode(std::string nm, TYP** tp, TypeArray* typearray, ENODE* args)
{
	ENODE* node, * namenode;
	Symbol* sp;

	sp = Symbol::alloc();
	sp->fi = compiler.ff.MakeFunction(sp->id, sp, defaultcc == 1);
	sp->tp = TYP::Copy(&stdfunc);
	sp->tp->btpp = TYP::Copy(&stdint);
	sp->SetName(*(new std::string(nm)));
	sp->storage_class = sc_external;
	sp->IsUndefined = TRUE;
	dfs.printf("Insert at nameref\r\n");
	typearray->Print();
	    gsyms[0].insert(sp);
	*tp = &stdfunc;
	namenode = makesnode(en_cnacon, sp->name, sp->BuildSignature(1), sp->value.i);
	node = makefcnode(en_fcall, namenode, args, sp);
	node->constflag = TRUE;
	node->sym = sp;
	if (sp->tp->isUnsigned)
		node->isUnsigned = TRUE;
	node->esize = cpu.sizeOfInt;
	node->isPascal = sp->fi->IsPascal;
	return (node);
}
