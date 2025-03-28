// ============================================================================
//        __
//   \\__/ o\    (C) 2017-2024  Robert Finch, Waterloo
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

int ENODE::segcount[16];
CSet ENODE::initializedSet;
CSet* ru;
CSet* rru;

List* sortedList(List* head, ENODE* root);

void swap_nodes(ENODE *node)
{
	ENODE *temp;
	temp = node->p[0];
	node->p[0] = node->p[1];
	node->p[1] = temp;
}

bool ENODE::IsEqualOperand(Operand *a, Operand *b)
{
	return (Operand::IsEqual(a, b));
};

char ENODE::fsize()
{
	switch (etype) {
	case bt_float:	return ('s');
	case bt_double:	return (' ');
	case bt_quad:	return ('q');
	case bt_posit: return (' ');
	default:	return (' ');
	}
}

bool ENODE::HasAssignop()
{
	if (this == nullptr)
		return (false);
	if (nodetype == en_asadd
		|| nodetype == en_asadd
		|| nodetype == en_asdiv
		|| nodetype == en_asdivu
		|| nodetype == en_asmod
		|| nodetype == en_asmodu
		|| nodetype == en_asmul
		|| nodetype == en_asmulu
		|| nodetype == en_asor
		|| nodetype == en_asand
		|| nodetype == en_asxor
		|| nodetype == en_assub
		|| nodetype == en_assign
		|| nodetype == en_fcall
		|| nodetype == en_ifcall
		)
		return (true);
	if (p[0]->HasAssignop())
		return (true);
	if (p[1]->HasAssignop())
		return(true);
	if (p[2]->HasAssignop())
		return (true);
	return(false);
}

bool ENODE::HasCall()
{
	if (this == nullptr)
		return (false);
	if (nodetype == en_fcall)
		return (true);
	if (p[0]->HasCall())
		return (true);
	if (p[1]->HasCall())
		return (true);
	if (p[2]->HasCall())
		return (true);
	return (false);
}

int64_t ENODE::GetReferenceSize()
{
	switch (nodetype)        /* get load size */
	{
	case en_ref:
	case en_fieldref:
//		return (tp->size);
		return (esize);
	case en_fpregvar:
		if (tp)
			return(tp->size);
		else
			return (cpu.sizeOfFPD);
	case en_pregvar:
		if (tp)
			return(tp->size);
		else
			return (cpu.sizeOfFPD);
	case en_type:
		return (tp->size);
	case en_regvar:
		return (cpu.sizeOfWord);
		//			return node->esize;
	}
	return (8);
}
// return the natural evaluation size of a node.

int64_t ENODE::GetNaturalSize()
{
	int64_t siz0, siz1, siz2;
	if (this == NULL)
		return 0;
	switch (nodetype)
	{
	case en_fieldref:
		return (tp->size);
	case en_icon:
		if (i >= -128 && i < 128)
			return (1);
		if (-32768 <= i && i <= 32767)
			return (2);
		if (-2147483648LL <= i && i <= 2147483647LL)
			return (4);
		return (cpu.sizeOfWord);
	case en_fcon:
		if (tp != nullptr)
			return (tp->precision / 8);
		else
			return (stdflt.precision / 8);
	case en_ubyt2tetra:
	case en_uwyde2tetra:
		return (4);
	case en_byt2octa: case en_ubyt2octa:
	case en_wyde2octa: case en_uwyde2octa:
	case en_tetra2octa: case en_utetra2octa:
		return (8);
	case en_ccwp: case en_cucwp:
	case en_sxb:	case en_sxc:	case en_sxh:
		return (cpu.sizeOfInt);
	case en_tcon: return (6);
	case en_labcon: case en_clabcon:
	case en_cnacon: case en_nacon:  case en_autocon: case en_classcon:
	case en_octa2hexi:
	case en_byt2hexi: case en_ubyt2hexi:
	case en_wyde2hexi: case en_uwyde2hexi:
	case en_tetra2hexi: case en_utetra2hexi:
	case en_uocta2hexi:
		return (cpu.sizeOfWord);
	case en_byt2ptr: case en_ubyt2ptr:
	case en_wyde2ptr: case en_uwyde2ptr:
		return (cpu.sizeOfPtr);
	case en_type:
		return (tp->size);
	case en_fcall:
		if (tp)
			return (tp->size);
		else
			return (cpu.sizeOfWord);
	case en_regvar:
	case en_fpregvar:
		if (tp)
			return (tp->size);
		else
			return (cpu.sizeOfWord);
	case en_autopcon:
	case en_autofcon:
		return (cpu.sizeOfWord);
	case en_ref:
		if (tp)
			return (tp->size);
		else
			return (cpu.sizeOfPtr);
	case en_byt2wyde: case en_ubyt2wyde: return (2);
	case en_byt2tetra:	return (4);
	case en_wyde2tetra:	return (4);
	case en_autovcon:
	case en_tempfpref:
		if (tp)
			return (tp->precision / 8);
		else
			return (cpu.sizeOfWord);
	case en_not:    case en_compl:
	case en_uminus: case en_assign:
		return p[0]->GetNaturalSize();
	case en_padd:	case en_psub:
	case en_pmul: case en_pdiv:
	case en_peq:	case en_plt:	case en_ple:
	case en_fadd:	case en_fsub:
	case en_fmul:	case en_fdiv:
	case en_fsadd:	case en_fssub:
	case en_fsmul:	case en_fsdiv:
	case en_vadd:	case en_vsub:
	case en_vmul:	case en_vdiv:
	case en_vadds:	case en_vsubs:
	case en_vmuls:	case en_vdivs:
	case en_add:    case en_sub:	case en_ptrdif:
	case en_ext:		case en_extu:
	case en_mul:    case en_mulu:	case en_mulf:
	case en_div:	case en_udiv:
	case en_mod:    case en_umod:
	case en_and:    case en_or:     case en_xor:
	case en_asl:
	case en_shl:    case en_shlu:
	case en_shr:	case en_shru:	case en_bitoffset:
	case en_asr:	case en_asrshu:
	case en_feq:    case en_fne:
	case en_flt:    case en_fle:
	case en_fgt:    case en_fge:
	case en_eq:     case en_ne:
	case en_lt:     case en_le:
	case en_gt:     case en_ge:
	case en_ult:	case en_ule:
	case en_ugt:	case en_uge:
	case en_land:   case en_lor:
	case en_land_safe:   case en_lor_safe:
	case en_asadd:  case en_assub:
	case en_asmul:  case en_asmulu:
	case en_asdiv:	case en_asdivu:
	case en_asmod:  case en_asmodu: case en_asand:
	case en_asor:   case en_asxor:	case en_aslsh:
	case en_asrsh:
		siz0 = p[0]->GetNaturalSize();
		siz1 = p[1]->GetNaturalSize();
		if (siz1 > siz0)
			return (siz1);
		else
			return (siz0);
	case en_and_and:
	case en_and_or:
	case en_or_or:
	case en_or_and:
		siz0 = p[0]->GetNaturalSize();
		siz1 = p[1]->GetNaturalSize();
		siz2 = p[2]->GetNaturalSize();
		if (siz0 > siz1 && siz0 > siz2)
			return (siz0);
		else if (siz1 > siz2)
			return (siz1);
		else
			return (siz2);
	case en_void:   case en_cond:	case en_safe_cond:
		return (p[1]->GetNaturalSize());
	case en_cast:
		if (p[0])
			return (p[0]->GetNaturalSize());
		else
			return (p[1]->GetNaturalSize());
	case en_bchk:
		return (p[0]->GetNaturalSize());
	case en_chk:
		return 8;
	case en_q2i:
	case en_t2i:
		return (cpu.sizeOfWord);
	case en_i2p:
	case en_i2d:
		return (cpu.sizeOfWord);
	case en_d2t:
		return (cpu.sizeOfFPT);
	case en_i2q:
	case en_d2q:
	case en_t2q:
		return (cpu.sizeOfFPQ);
	case en_object_list:
		return (p[0]->GetNaturalSize());
	case en_addrof:
		return (cpu.sizeOfPtr);
	case en_bytendx:
		return (1);
	case en_wydendx:
		return (2);
	case en_bmap:
		return (cpu.sizeOfWord);
	case en_sync:
		return (4);
	case en_aggregate:
	case en_end_aggregate:
		return (esize);
//		return (p[0]->GetNaturalSize());
	case en_list:
		if (p[1])
			return (p[0]->GetNaturalSize() + p[1]->GetNaturalSize());
		else
			return (p[0]->GetNaturalSize());
	case en_switch:
		if (tp)
			return (tp->size);
		return (cpu.sizeOfWord);
	case en_unknown:
		return (cpu.sizeOfWord);
	default:
		printf("DIAG - natural size error %d.\n", nodetype);
		break;
	}
	return (0);
}


bool ENODE::IsBitfield()
{
	if (nodetype == en_ref) {
		if (p[0]->nodetype == en_bitoffset)
			return (true);
	}
	if (this->tp)
		if (this->tp->type == bt_bit)
			return (true);
	return (nodetype == en_fieldref);
}

bool ENODE::IsSameType(ENODE *node1, ENODE *node2)
{
	if (node1 == nullptr && node2 == nullptr)
		return (true);
	// If we have a null pointer for the expression it may be a
	// void pointer.
	if (node1 == nullptr || node2 == nullptr)
		return (true);
	if (node1->nodetype == en_icon && node2->nodetype == en_icon)
		return (true);
	return (TYP::IsSameType(node1->tp, node2->tp, false));
}

//
// equalnode will return 1 if the expressions pointed to by
// node1 and node2 are equivalent.
//
bool ENODE::IsEqual(ENODE *node1, ENODE *node2, bool lit)
{
	ENODE *ep1, *ep2;

	if (node1 == nullptr && node2 == nullptr)
		return (true);
	if (!lit) {
		if (node1 == nullptr || node2 == nullptr) {
			return (false);
		}
	}
	else {
		if (node1 == nullptr || node2 == nullptr)
			return (true);
	}
	if (node1->nodetype != node2->nodetype) {
		return (false);
	}
	switch (node1->nodetype) {
	case en_fcon:
		return (Float128::IsEqual(&node1->f128, &node2->f128));
		//			return (node1->f == node2->f);
	case en_regvar:
		return (node1->rg == node2->rg);
	case en_fpregvar:
		return (node1->rg == node2->rg);
	case en_pregvar:
		return (node1->rg == node2->rg);
	//case en_type:
	case en_tempfpref:
	case en_icon:
	case en_labcon:
	case en_classcon:	// Check type ?
	case en_autocon:
	case en_autovcon:
	case en_autopcon:
	case en_autofcon:
	{
		// With nested methods, autocon nodes can end up with the same stack offset
		// value, but they are really pointing to different places in the stack.
		// It depends on the depth being the same.
		if (node1->sym == nullptr || node2->sym == nullptr)
			return (false);
		if (node1->sym->depth != node2->sym->depth)
			return (false);
		return (node1->i == node2->i);
	}
	case en_nacon: {
		return (node1->sp->compare(*node2->sp) == 0);
	}
	case en_cnacon:
		return (node1->sp->compare(*node2->sp) == 0);
	case en_aggregate:
	case en_end_aggregate:
		return (IsEqual(node1->p[0], node2->p[0], lit));
	case en_list:
		for (ep1 = node1->p[2],
				 ep2 = node2->p[2];
				 ep1 && ep2; ep1 = ep1->p[2], ep2 = ep2->p[2]) {
			if (!IsEqual(ep1->p[0], ep2->p[0], lit))
				return (false);
			if (!IsEqual(ep1->p[1], ep2->p[1], lit))
				return (false);
		}
		if (ep1 != nullptr && ep2 != nullptr)
			return (true);
		return(false);
	default:
		if (!IsEqual(node1->p[0], node2->p[0], lit))
			return (false);
		if (!IsEqual(node1->p[1], node2->p[1], lit))
			return (false);
		if (!IsEqual(node1->p[2], node2->p[2], lit))
			return (false);
		return (true);
		//if (IsLValue(node1) && IsEqual(node1->p[0], node2->p[0])) {
		//	//	        if( equalnode(node1->p[0], node2->p[0])  )
		//	return (true);
		//}
	}
	return (false);
}


// For purposes of optimization we only care about cloning the node being
// worked on, not the other nodes that are connected to it. So, clone doesn't
// do a full clone in order to improve the compiler's performance. A real
// clone would clone the whole tree, but it's not required in the current
// compiler.

ENODE *ENODE::Clone()
{
	ENODE *temp;
	int num;

	if (this == nullptr)
		return (ENODE *)nullptr;
	temp = allocEnode();
	num = temp->number;
	memcpy(temp, this, sizeof(ENODE));	// copy all the fields
	temp->number = num;
	/*
	if (p[0]) temp->p[0] = p[0]->Clone();
	if (p[1]) temp->p[1] = p[1]->Clone();
	if (p[2]) temp->p[2] = p[2]->Clone();
	if (p[3]) temp->p[3] = p[3]->Clone();
	if (p[4]) temp->p[4] = p[4]->Clone();
	*/
	return (temp);
}

// ============================================================================
// ============================================================================
// Parsing
// ============================================================================
// ============================================================================

// AddToList used to build aggregate types.

void ENODE::AddToList(ENODE* ele)
{
	ENODE* p, * pp;

	p = this;
	pp = nullptr;
	while (p) {
		pp = p;
		p = p->p[2];
	}
	if (pp) {
		pp->p[2] = ele;
	}
	else
		this->p[2] = ele;
}

static void ListNodes(ENODE* node)
{
	int level, nn;
	List* lst;

	level = 0;
	for (lst = (List*)node->p[1]; lst; lst = lst->nxt) {
		if (lst->node == nullptr)
			break;
		for (nn = 0; nn < level * 2; nn++)
			dfs.printf(" ");
		dfs.printf("Node %d:\n", lst->node->number);
		for (nn = 0; nn < level * 2; nn++)
			dfs.printf(" ");
		dfs.printf("nodetype: %d: ", lst->node->nodetype);
		dfs.printf("%s\n", (char*)lst->node->nodetypeStr().c_str());
		for (nn = 0; nn < level * 2; nn++)
			dfs.printf(" ");
		dfs.printf("rg: %d\n", lst->node->rg);
		if (lst->node->nodetype == en_aggregate) {
			level += 3;
			ListNodes(lst->node);
			level -= 3;
		}
	}
}

// Assign a type to a whole list of nodes.

static void AssignTypeToArray(ENODE* node, TYP* btp)
{
	ENODE* ep;
	List* lst, *hlst;

	if (node == nullptr)
		return;

	hlst = lst = node->ReverseList(node);

	for (; lst; lst = lst->nxt) {
		ep = lst->node;
		if (ep == nullptr)
			continue;
		ep->tp = btp;
		ep->esize = btp->size;
		if (!ep->constflag)
			isConst = false;
		if (btp->isArray) {
			ep->tp->btpp = btp->btpp;
			if (!ep->AssignTypeToList(btp))
				isConst = false;
		}
		else if (btp->IsAggregateType()) {
			if (!ep->AssignTypeToList(btp))
				isConst = false;
		}
	}
	delete[] hlst;
}

static void AssignTypeToAggregateHelper(ENODE* node, Symbol* thead)
{
	ENODE* ep;
	List* lst, *hlst;

	if (node == nullptr)
		return;

	hlst = lst = node->ReverseList(node);
	for (; lst; lst = lst->nxt) {
		ep = node;
		if (TYP::IsSameType(ep->tp, thead->tp->btpp, false)) {
			ep->tp = thead->tp;
			ep->esize = thead->tp->size;
		}
		else
			break;
	}
	delete[] hlst;
}

static void AssignTypeToAggregate(ENODE* node, Symbol* thead)
{
	ENODE* ep;
	List* lst, *hlst; 

	// If we run out of aggregate initializers, okay.
	if (node == nullptr)
		return;
	if (thead == nullptr)
		return;

	hlst = lst = node->ReverseList(node);
	for (; lst; lst = lst->nxt) {
		ep = lst->node;
		if (ep) {
			if (!ep->constflag)
				isConst = false;
			if (thead->tp->isArray || thead->tp->IsAggregateType()) {
				if (ep->nodetype == en_end_aggregate) {
					if (!ep->AssignTypeToList(thead->tp))
						isConst = false;
					ep->tp = thead->tp;
					ep->esize = thead->tp->size;
				}
				else if (TYP::IsSameType(ep->tp, thead->tp->btpp, false))
					AssignTypeToAggregateHelper(ep, thead);
			}
			else
				ep->tp = thead->tp;
			thead = thead->nextp;
			if (thead == nullptr)
				break;
		}
	}
	delete[] hlst;
}

bool ENODE::AssignTypeToList(TYP *ptp)
{
	ENODE *ep;
	bool isConst = true;
	TYP *btp;
	int level;
	bool ary;

	if (this == nullptr)
		return (false);
	if (nodetype != en_end_aggregate) {
		if (this) {
			this->tp = ptp;
			return (this->constflag);
		}
		else
			return (false);
	}

	esize = 0;
	ary = tp->isArray;
	if (ptp->isArray) {
		btp = ptp->btpp;
		ep = this;
		AssignTypeToArray(ep, btp);
	//if (cnt < tp->numele) {
		//	esize += (tp->numele - cnt) * btp->size;
		//}
		tp = ptp;
		esize = ptp->size;
	}
	else if (ptp->IsAggregateType()) {
		Symbol *thead;

		ep = this;
		thead = ptp->lst.headp;
		// Thought it was a struct during parsing, but it is really a union, fixup.
		if (tp->type == bt_struct && ptp->type == bt_union)
			tp->type = bt_union;
		AssignTypeToAggregate(ep, thead);
		tp = ptp;
		esize = ptp->size;
	}
	// Else there is just one type to assign.
	else {
		Symbol* thead;

		thead = tp->lst.headp;
		ep = this;
		if (ep) {
			if (thead) {
				ep->tp = thead->tp;
				ep->tp->btpp = thead->tp->btpp;
				ep->esize = thead->tp->size;
			}
			else {
				ep->tp = &stdint;
				ep->esize = 8;
			}
		}
	}
	//this->Dump(0);
	level = 0;
	ListNodes(this);
	return (isConst);
}

void ENODE::DumpAggregate()
{
	static int level = 0;
	List* hlst, * lst;
	ENODE* node;

	if (level == 0)
		dfs.puts("<Aggregate>");
	dfs.puts("{ ");
	hlst = this->ReverseList(this);
	for (lst = hlst; lst; lst = lst->nxt) {
		node = lst->node;
		if (node != nullptr) {
			if (node->nodetype == en_end_aggregate){// && lst != hlst) {
				level++;
				node->DumpAggregate();
				level--;
			}
			else if (node->nodetype != en_end_aggregate && node->nodetype != en_void) {
				node->PutConstant(dfs, 0, 0, false, 0);
				dfs.puts(", ");
			}
		}
	}
	dfs.puts(" }");
	if (level == 0)
		dfs.puts("</Aggregate>\n");
	delete[] hlst;
}


List* ENODE::ReverseList(ENODE* node)
{
	List* lst;
	ENODE* pnode;
	int64_t count, cnt2;

	List::numele = 0;
	// Trap single non-list items.
	if (node->nodetype != en_end_aggregate) {
		lst = new List[1];
		lst[0].node = node;
		lst[0].nxt = nullptr;
		return (lst);
	}

	// Count the number of node we need.
	count = 0;
	for (pnode = node; pnode; pnode = pnode->p[0])
		count++;

	// Allocate
	lst = new List[count];
	cnt2 = count - 1;
	// Copy node pointers to array in reverse order.
	for (pnode = node; pnode && cnt2 >= 0; pnode = pnode->p[0]) {
		lst[cnt2].node = pnode->p[1];
		lst[cnt2].nxt = &lst[cnt2 + 1];
		cnt2 = cnt2 - 1;
	}
	lst[count - 1].nxt = nullptr;
	List::numele = count;
	return (&lst[0]);
}


// Function to create Linked list from given binary tree
List* sortedList(List* head, ENODE* root)
{
	if (root == nullptr)
		return (head);

	// First make the sorted linked list
	// of the left sub-tree
	head = sortedList(head, root->p[0]);
	List* newNode = new List(root);
	List* temp = head;
	List* prev = nullptr;

	// If linked list is empty add the
	// node to the head
	if (temp == nullptr)
		head = newNode;
	else {

		// Find the correct position of the node
		// in the given linked list
		while (temp != NULL) {
			if (temp->node->order > root->order) {
				break;
			}
			else {
				prev = temp;
				temp = temp->nxt;
			}
		}

		// Given node is to be attached
		// at the end of the list
		if (temp == NULL) {
			prev->nxt = newNode;
		}
		else {

			// Given node is to be attached
			// at the head of the list
			if (prev == NULL) {
				newNode->nxt = temp;
				head = newNode;
			}
			else {

				// Insertion in between the list
				newNode->nxt = temp;
				prev->nxt = newNode;
			}
		}
	}

	// Now add the nodes of the right sub-tree
	// to the sorted linked list
	head = sortedList(head, root->p[1]);
	return (head);
}

// Driver code
/*
int main()
{
	   Tree:
			 10
			/  \
		15    2
	 /  \
	1    5

	treeNode* root = new treeNode(10);
	root->left = new treeNode(15);
	root->right = new treeNode(2);
	root->left->left = new treeNode(1);
	root->left->right = new treeNode(5);

	Node* head = sortedList(NULL, root);
	print(head);

	return 0;
}
*/
// ============================================================================
// ============================================================================
// Optimization
// ============================================================================
// ============================================================================

//	repexpr will replace all allocated references within an expression
//	with tempref nodes.

void ENODE::repexpr()
{
	CSE *csp;
	ENODE *ep;

	if (this == nullptr)
		return;
	switch (nodetype) {
	case en_temppref:
		if ((csp = currentFn->csetbl->Search(this)) != nullptr) {
			//csp->isPosit = TRUE; //**** a kludge
			if (!csp->voidf) {
				if (csp->reg > 0) {
					nodetype = en_pregvar;
					rg = csp->reg;
					ru->add(rg);
					rru->add(nregs - 1 - rg);
				}
			}
		}
		break;
	case en_fcon:
	case en_tempfpref:
		if ((csp = currentFn->csetbl->Search(this)) != nullptr) {
			//csp->isfp = TRUE; //**** a kludge
			if (!csp->voidf) {
				if (csp->reg > 0) {
					nodetype = en_fpregvar;
					rg = csp->reg;
					ru->add(rg);
					rru->add(nregs - 1 - rg);
				}
			}
		}
		break;
	// Autofcon resolve to *pointers* which are stored in integer registers.
	case en_autopcon:
		if ((csp = currentFn->csetbl->Search(this)) != nullptr) {
//			csp->isPosit = false; //**** a kludge
			if (csp->reg > 0) {
				nodetype = en_pregvar;
				rg = csp->reg;
				ru->add(rg);
				rru->add(nregs - 1 - rg);
			}
		}
		break;
	case en_autofcon:
		if ((csp = currentFn->csetbl->Search(this)) != nullptr) {
//			csp->isfp = FALSE; //**** a kludge
			if (!csp->voidf) {
				if (csp->reg > 0) {
					nodetype = en_fpregvar;
					rg = csp->reg;
					ru->add(rg);
					rru->add(nregs - 1 - rg);
				}
			}
		}
		break;
		/*
		if( (csp = SearchCSEList(node)) != NULL ) {
		if( csp->reg > 0 ) {
		node->nodetype = en_fpregvar;
		node->i = csp->reg;
		}
		}
		break;
		*/
	case en_icon:
		if ((csp = currentFn->csetbl->Search(this)) != NULL) {
			if (!csp->voidf) {
				if (csp->reg > 0) {
					nodetype = en_regvar;
					rg = csp->reg;
					ru->add(rg);
					rru->add(nregs - 1 - rg);
				}
			}
		}
		break;
	case en_nacon:
	case en_labcon:
	case en_pcon:
	case en_autovcon:
	case en_autocon:
	case en_classcon:
	case en_cnacon:
	case en_clabcon:
		if ((csp = currentFn->csetbl->Search(this)) != NULL) {
			if (!csp->voidf) {
				if (csp->reg > 0) {
					nodetype = en_regvar;
					rg = csp->reg;
					ru->add(rg);
					rru->add(nregs - 1 - rg);
				}
			}
		}
		break;
	case en_type:
		break;
	/*
	case en_c_ref:
	case en_uc_ref:
	case en_w_ref:
	case en_wp_ref:
		if (p[0]->IsAutocon()) {
			if ((csp = currentFn->csetbl->Search(this)) != NULL) {
				if (!csp->voidf) {
					if (csp->reg > 0) {
						new_nodetype = en_regvar;
						rg = csp->reg;
						p[0]->repexpr();
					}
					else
						p[0]->repexpr();
				}
				else
					p[0]->repexpr();
				//while (csp = currentFn->csetbl->SearchNext(this)) {
				//	if (!csp->voidf) {
				//		if (csp->reg > 0) {
				//			new_nodetype = en_regvar;
				//			rg = csp->reg;
				//		}
				//		else
				//			p[0]->repexpr();
				//	}
				//}
			}
			else
				p[0]->repexpr();
			break;
		}
		p[0]->repexpr();
		p[1]->repexpr();
		break;
	*/
	case en_fieldref:
		bit_offset->repexpr();
		bit_width->repexpr();
	case en_ref:
		if ((csp = currentFn->csetbl->Search(this)) != nullptr) {
			if (!csp->voidf) {
				if (csp->reg > 0) {
					nodetype = csp->isfp ? en_fpregvar : csp->isPosit ? en_pregvar : en_regvar;
					//nodetype = en_regvar;
					rg = csp->reg;
					ru->add(rg);
					rru->add(nregs - 1 - rg);
				}
				else {
					p[0]->repexpr();
					vmask->repexpr();
				}
			}
			else {
				p[0]->repexpr();
				vmask->repexpr();
			}
		}
		else {
			p[0]->repexpr();
			vmask->repexpr();
		}
		break;
	case en_ubyt2tetra: case en_uwyde2tetra:
	case en_byt2wyde: case en_ubyt2wyde: case en_ubyt2octa:
	case en_byt2tetra: case en_uwyde2octa:
	case en_byt2octa: case en_utetra2octa:
	case en_tetra2hexi:
	case en_ccwp: case en_cucwp:
	case en_wyde2tetra:
	case en_wyde2octa:
	case en_wyde2hexi:
	case en_tetra2octa:
	case en_octa2hexi:
	case en_uocta2hexi:
	case en_uminus:
	case en_abs:
	case en_sxb: case en_sxh: case en_sxc:
		p[0]->repexpr();
		vmask->repexpr();
		break;
	case en_chk:
		p[0]->repexpr();
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_compl:
		p[0]->repexpr();
		vmask->repexpr();
		break;
	case en_not:
		p[0]->repexpr();
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_i2p:
	case en_i2d:
		p[0]->repexpr();
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_i2q:
	case en_d2i:
	case en_q2i:
	case en_s2q:
	case en_d2q:
	case en_t2q:
		p[0]->repexpr();
		vmask->repexpr();
		break;
	case en_asadd:
	case en_add:    
		p[0]->repexpr();
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_and_or:
	case en_and_and:
	case en_or_and:
	case en_or_or:
		p[0]->repexpr();
		p[1]->repexpr();
		p[2]->repexpr();
		vmask->repexpr();
		break;
	case en_bmap:
	case en_bytendx:	case en_wydendx:
	case en_sub:
	case en_ext: case en_extu:
	case en_mulf:   case en_mul:    case en_mulu:   case en_div:	case en_udiv:
	case en_mod:    case en_umod:
	case en_shl:	case en_asl:
	case en_shlu:	case en_shru:	case en_asr: case en_bitoffset:
	case en_shr:
	case en_and:
	case en_or:     case en_xor:
	case en_land:   case en_lor:
	case en_land_safe:   case en_lor_safe:
	case en_eq:     case en_ne:
	case en_lt:     case en_le:
	case en_gt:     case en_ge:
	case en_ult:	case en_ule:
	case en_ugt:	case en_uge:

	case en_feq:    case en_fne:
	case en_flt:    case en_fle:
	case en_fgt:    case en_fge:
	case en_fdmul:  case en_fddiv:
	case en_fdadd:  case en_fdsub:
	case en_fadd: case en_fsub:
	case en_fmul: case en_fdiv:
	case en_padd:	case en_psub:
	case en_pmul:	case en_pdiv:
	case en_peq:  case en_plt: case en_ple:
	case en_veq:    case en_vne:
	case en_vlt:    case en_vle:
	case en_vgt:    case en_vge:
	case en_vadd: case en_vsub:
	case en_vmul: case en_vdiv:
	case en_vadds: case en_vsubs:
	case en_vmuls: case en_vdivs:

	case en_safe_cond:
	case en_assub:
	case en_asmul:  case en_asmulu:
	case en_asdiv:  case en_asdivu:
	case en_asor:   case en_asand:    case en_asxor:
	case en_asmod:  case en_aslsh:
	case en_asrsh:
	case en_aggregate:
	case en_end_aggregate:
	case en_void:
		p[0]->repexpr();
	case en_cast:
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_fcall:
		p[0]->repexpr();
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_assign:
		p[0]->repexpr();
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_cond:
		p[0]->repexpr();
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_list:
		for (ep = p[2]; ep; ep = ep->p[2]) {
			ep->p[0]->repexpr();
			ep->p[1]->repexpr();
			ep->vmask->repexpr();
		}
		break;
	case en_regvar:
	case en_fpregvar:
	case en_pregvar:
		p[0]->repexpr();
		p[1]->repexpr();
		vmask->repexpr();
		break;
	case en_ptrdif:
	case en_bchk:
		p[0]->repexpr();
		p[1]->repexpr();
		p[2]->repexpr();
		vmask->repexpr();
		break;
	case en_isnullptr:
	case en_addrof:
		p[0]->repexpr();
		vmask->repexpr();
		break;
	default:
		dfs.printf("Uncoded node in repexr():%d\r\n", nodetype);
	}
	if (pfl)
		pfl->repexpr();
}


CSE* ENODE::OptInsertAutocon(int duse)
{
	int nn;
	CSE* csp;
	bool first;

	if (this == nullptr)
		return (nullptr);
	if (sym) {
		if (sym->IsParameter) {
			if (this->sym->depth < currentFn->depth)
				return (nullptr);
		}
		else {
			if (this->sym->depth < currentFn->depth + 1)	// was + 1
				return (nullptr);
		}
	}
	csp = nullptr;
	nn = currentFn->csetbl->voidauto2(this);
	csp = currentFn->csetbl->InsertNode(this, duse, &first);
	if (csp && nn >= 0) {
		csp->duses += loop_active + nn;
		csp->uses = csp->duses - loop_active;
	}
	return (csp);
}

CSE *ENODE::OptInsertRef(int duse)
{
	CSE *csp;
	bool first;
	ENODE *ep;

	csp = nullptr;
	// Search the chain of refs.
//	for (ep = p[0]; ep && ep->IsRefType(); ep = ep->p[0])
//		if (ep == nullptr)
//			return (nullptr);
	if (this == nullptr)
		return (nullptr);
//	if (sym)
//		if (sym->depth < currentFn->depth)
//			return (nullptr);
	ep = p[0];
	if (ep && ep->IsAutocon()) {
		csp = currentFn->csetbl->InsertNode(this, duse, &first);
		// take care: the non-derereferenced use of the autocon node may
		// already be in the list. In this case, set voidf to 1
		if (currentFn->csetbl->Search(p[0]) != NULL
			|| currentFn->csetbl->Search(p[1]) != NULL
			|| currentFn->csetbl->Search(p[2]) != NULL) {	// might be a triple
			csp->voidf = 1;
			p[0]->scanexpr(1);
			vmask->scanexpr(1);
			if (pfl)
				pfl->scanexpr(1);
		}
		else {
			if (csp->voidf) {
				p[0]->scanexpr(1);
				vmask->scanexpr(1);
				if (pfl)
					pfl->scanexpr(1);
			}
			if (first) {
				// look for register nodes
				int j = p[0]->rg;
				if ((p[0]->nodetype == en_regvar || p[0]->nodetype==en_fpregvar || p[0]->nodetype==en_pregvar) && IsSavedReg(j))
				{
					csp->voidf = false;
					csp->AccUses(3);
					csp->AccDuses(1);
				}
				if ((p[0]->nodetype == en_regvar || p[0]->nodetype == en_fpregvar || p[0]->nodetype == en_pregvar) && IsArgReg(j))
				{
					csp->voidf = false;
				}
				// even if that item must not be put in a register,
				// it is legal to put its address therein
				if (csp->voidf) {
					p[0]->scanexpr(1);
					vmask->scanexpr(1);
				}
			}
		}
	}
	else
	{
	//	//			csp = currentFn->csetbl->InsertNode(this, duse);
		p[0]->scanexpr(1);
		p[1]->scanexpr(1);
		p[2]->scanexpr(1);
		vmask->scanexpr(1);
		if (pfl)
			pfl->scanexpr(1);
	}
	return (csp);
}

/*
*      scanexpr will scan the expression pointed to by node for optimizable
*      subexpressions. when an optimizable expression is found it is entered
*      into the tree. if a reference to an autocon node is scanned the
*      corresponding auto dereferenced node will be voided. duse should be
*      set if the expression will be dereferenced.
*/
void ENODE::scanexpr(int duse)
{
	CSE *csp;
	bool first;
	ENODE *ep;

	if (this == nullptr)
		return;

	switch (nodetype) {
	case en_fpregvar:
	case en_pregvar:
	case en_regvar:
		currentFn->csetbl->InsertNode(this, duse, &first);
		break;
	case en_icon:
		currentFn->csetbl->InsertNode(this, duse, &first);
		break;
	case en_cnacon:
	case en_clabcon:
	case en_fcon:
	case en_labcon:
	case en_nacon:
		currentFn->csetbl->InsertNode(this, duse, &first);
		break;
	case en_autopcon:
	case en_autofcon:
	case en_temppref:
	case en_tempfpref:
		if (csp = OptInsertAutocon(duse)) {
			csp->isfp = false;
			csp->isPosit = false;
		}
		break;
	case en_autovcon:
	case en_autocon:
	case en_classcon:
		OptInsertAutocon(duse);
		break;
	case en_type:
		break;
	case en_byt2wyde: case en_ubyt2wyde:
	case en_ubyt2tetra: case en_ubyt2octa: case en_ubyt2hexi:
	case en_byt2tetra:
	case en_uwyde2tetra: case en_uwyde2octa: case en_uwyde2hexi:
	case en_byt2octa: case en_utetra2octa: case en_byt2hexi: case en_utetra2hexi:
	case en_ccwp: case en_cucwp: case en_wyde2ptr: case en_uwyde2ptr:
	case en_wyde2tetra:
	case en_wyde2octa: case en_wyde2hexi:
	case en_tetra2octa: case en_tetra2hexi:
	case en_octa2hexi: case en_uocta2hexi:
		p[0]->scanexpr(duse);
		vmask->scanexpr(duse);
		break;
	case en_fieldref:
		bit_offset->scanexpr(1);
		bit_width->scanexpr(1);
	case en_ref:
		OptInsertRef(1);
		break;
	case en_uminus:
	case en_abs:
	case en_sxb: case en_sxc: case en_sxh:
	case en_zxb: case en_zxc: case en_zxh:
	case en_compl:
	case en_chk:
		p[0]->scanexpr(duse);
		vmask->scanexpr(duse);
		break;
	case en_not:
		p[0]->scanexpr(duse);
		vmask->scanexpr(duse);
		break;
	case en_i2p:
	case en_i2d:
		p[0]->scanexpr(duse);
		vmask->scanexpr(duse);
		break;
	case en_i2q:
	case en_d2i:
	case en_q2i:
	case en_s2q:
	case en_d2q:
	case en_t2q:
		p[0]->scanexpr(duse);
		vmask->scanexpr(duse);
		break;
	case en_asadd:  
	case en_assub:
	case en_add:    case en_sub:
		p[0]->scanexpr(duse);
		p[1]->scanexpr(duse);
		vmask->scanexpr(duse);
		break;
	case en_and_or:
	case en_or_and:
	case en_and_and:
	case en_or_or:
		p[0]->scanexpr(duse);
		p[1]->scanexpr(duse);
		p[2]->scanexpr(duse);
		vmask->scanexpr(duse);
		break;
	case en_bmap:
	case en_ext: case en_extu:
	case en_mulf:		case en_bytendx:	case en_wydendx: case en_bitoffset:
	case en_mul:    case en_mulu:   case en_div:	case en_udiv:
	case en_shl:    case en_asl:	case en_shlu:	case en_shr:	case en_shru:	case en_asr:
	case en_mod:    case en_umod:   case en_and:
	case en_or:     case en_xor:
	case en_lor:    case en_land:
	case en_lor_safe:    case en_land_safe:
	case en_eq:     case en_ne:
	case en_gt:     case en_ge:
	case en_lt:     case en_le:
	case en_ugt:    case en_uge:
	case en_ult:    case en_ule:

	case en_feq:    case en_fne:
	case en_flt:    case en_fle:
	case en_fgt:    case en_fge:
	case en_fdmul:  case en_fddiv:
	case en_fdadd:  case en_fdsub:
	case en_fadd: case en_fsub:
	case en_fmul: case en_fdiv:

	case en_peq:	case en_plt:	case en_ple:
	case en_padd:	case en_psub:
	case en_pmul:	case en_pdiv:

	case en_veq:    case en_vne:
	case en_vlt:    case en_vle:
	case en_vgt:    case en_vge:
	case en_vadd: case en_vsub:
	case en_vmul: case en_vdiv:
	case en_vadds: case en_vsubs:
	case en_vmuls: case en_vdivs:

	case en_asmul:  case en_asmulu:
	case en_asdiv:	case en_asdivu:
	case en_asmod:  case en_aslsh:
	case en_asrsh:
	case en_asand:	case en_asxor: case en_asor:
	case en_cond:	case en_safe_cond:
	case en_void:
	case en_aggregate:
	case en_end_aggregate:
		p[0]->scanexpr(0);
	case en_cast:
		p[1]->scanexpr(0);
		vmask->scanexpr(0);
		break;
	case en_list:
		for (ep = p[2]; ep; ep = ep->p[2]) {
			ep->p[0]->scanexpr(0);
			ep->p[1]->scanexpr(0);
			vmask->scanexpr(0);
		}
		break;
	case en_assign:
		p[0]->scanexpr(0);
		p[1]->scanexpr(0);
		vmask->scanexpr(0);
		break;
	case en_fcall:
		p[0]->scanexpr(1);
		p[1]->scanexpr(0);
		vmask->scanexpr(0);
		break;
	case en_ptrdif:
	case en_bchk:
		p[0]->scanexpr(0);
		p[1]->scanexpr(0);
		p[2]->scanexpr(0);
		vmask->scanexpr(0);
		break;
	case en_isnullptr:
	case en_addrof:
		p[0]->scanexpr(0);
		vmask->scanexpr(0);
		break;
	default: dfs.printf("Uncoded node in ENODE::scanexpr():%d\r\n", nodetype);
	}
	if (pfl)
		pfl->scanexpr(0);
}

// Debugging use
void ENODE::update()
{
	if (this == nullptr)
		return;
	if (IsAutocon() || IsRefType())
	{
		if (new_nodetype != en_unknown)
			nodetype = new_nodetype;
	}
	p[0]->update();
	p[1]->update();
	p[2]->update();
	if (pfl)
		pfl->update();
}

// ============================================================================
// ============================================================================
// Code Generation
// ============================================================================
// ============================================================================

bool ENODE::FindLoopVar(int64_t ii)
{
	if (p[0])
		if (p[0]->FindLoopVar(ii))
			return (true);
	if (p[1])
		if (p[1]->FindLoopVar(ii))
			return (true);
	if (p[2])
		if (p[2]->FindLoopVar(ii))
			return (true);
	if (p[3])
		if (p[3]->FindLoopVar(ii))
			return (true);
	if (nodetype == en_regvar) {
		if (rg == ii)
			return (true);
	}
	return (false);
}

Operand *ENODE::MakeDataLabel(int lab, int ndxreg)
{
	return (compiler.of.MakeDataLabel(lab, ndxreg));
}

Operand *ENODE::MakeCodeLabel(int lab)
{
	return (compiler.of.MakeCodeLabel(lab));
}

Operand *ENODE::MakeStringAsNameConst(char *s, e_sg seg)
{
	return (compiler.of.MakeStringAsNameConst(s, seg));
}

Operand *ENODE::MakeString(char *s)
{
	return (compiler.of.MakeString(s));
}

Operand *ENODE::MakeImmediate(int64_t i)
{
	return (compiler.of.MakeImmediate(i));
}

Operand *ENODE::MakeIndirect(int i)
{
	return (compiler.of.MakeIndirect(i));
}

Operand *ENODE::MakeIndexed(int64_t o, int i)
{
	return (compiler.of.MakeIndexed(o, i));
}

Operand *ENODE::MakeDoubleIndexed(int i, int j, int scale)
{
	return (compiler.of.MakeDoubleIndexed(i, j, scale));
}

Operand *ENODE::MakeDirect(ENODE *node)
{
	return (compiler.of.MakeDirect(node));
}

Operand *ENODE::MakeIndexed(ENODE *node, int rg)
{
	return (compiler.of.MakeIndexed(node, rg));
}

void ENODE::GenerateHint(int num)
{
	GenerateMonadic(op_hint, 0, MakeImmediate(num));
}

void ENODE::GenerateLoad(Operand *ap3, Operand *ap1, int64_t ssize, int64_t size)
{
	cg.GenerateLoad(ap3, ap1, ssize, size);
}
void ENODE::GenStore(Operand *ap1, Operand *ap3, int64_t size)
{
	cg.GenerateStore(ap1, ap3, size);
}

void ENODE::GenMemop(int op, Operand *ap1, Operand *ap2, int64_t ssize, int typ)
{
	cg.GenMemop(op, ap1, ap2, ssize, typ);
}

void ENODE::GenRedor(Operand *ap1, Operand *ap2)
{
	GenerateDiadic(op_not, 0, ap1, ap2);
	GenerateDiadic(op_not, 0, ap1, ap1);
}

Operand* ENODE::GenerateRegRegIndex()
{
	Operand* ap1, * ap2;

	// Don't need to free ap2 here. It is included in ap1.
	GenerateHint(8);
	ap1 = cg.GenerateExpression(p[0], am_reg, cpu.sizeOfWord, 0);
	ap2 = cg.GenerateExpression(p[1], am_reg, cpu.sizeOfWord, 0);
	GenerateHint(9);
	if (cpu.SupportsIndexed) {
		ap1->mode = am_indx2;
		ap1->sreg = ap2->preg;
		ap1->deep2 = ap2->deep2;
		ap1->offset = makeinode(en_icon, 0);
		ap1->scale = scale;
		ap1->isUnsigned = ap2->isUnsigned;
	}
	else {
		cg.GenerateAdd(ap1, ap1, ap2);
		ap1->mode = am_indx;
		ap1->deep2 = ap2->deep2;
		ap1->offset = makeinode(en_icon, 0);
		ap1->scale = scale;
		ap1->isUnsigned = ap2->isUnsigned;
	}
	return (ap1);
}

Operand* ENODE::GenerateImmExprIndex(Operand* ap1, bool neg)
{
	Operand* ap2;

	ap2 = cg.GenerateExpression(p[1], am_reg | am_imm, cpu.sizeOfInt, 1);
	if (ap2->mode == am_reg && ap2->preg == 0) {	// value is zero
		ap1->mode = am_direct;
		if (ap1->offset)
			DataLabels[ap1->offset->i]++;
		return (ap1);
	}
	ap2->isConst = ap2->mode == am_imm;
	if (ap2->mode == am_imm && ap2->offset->i != 0)
		ap2->offset2 = ap2->offset;
	else
		ap2->offset2 = nullptr;
	GenerateHint(9);
	ap2->mode = am_indx;
	ap2->offset = ap1->offset;
	if (neg && pass == 1) {
		ap2->offset->i = -ap2->offset->i;
		if (ap2->offset2)
			ap2->offset2->i = -ap2->offset2->i;
	}
	ap2->isUnsigned = ap1->isUnsigned;
	return (ap2);
}

// Do we have reg+imm? If so make am_indx

Operand* ENODE::GenerateRegImmIndex(Operand* ap1, Operand* ap2, bool neg)
{
	ap1->mode = am_indx;
	//ap2->preg = ap1->preg;
	//ap2->deep = ap1->deep;
	ap1->offset = ap2->offset;
	ap1->offset2 = ap2->offset2;
	if (neg && pass == 1) {
		ap1->offset->i = -ap1->offset->i;
		if (ap1->offset2)
			ap1->offset2->i = -ap1->offset2->i;
	}
	// Scale a constant index by the type size.
	// ????
	/*
	if (!ap1->is_scaled && pass == 1 && ap1->tp && ap1->tp->isArray) {
		int sz;
		if (ap1->tp) {
			if (ap1->tp->type == bt_pointer)
				sz = ap1->tp->btpp->size;
			else
				sz = ap1->tp->size;
			Int128::Mul(&ap1->offset->i128, &ap1->offset->i128, Int128::MakeInt128(sz));
			ap1->is_scaled = true;
		}
	}
	*/
	return (ap1);
}

// ----------------------------------------------------------------------------
// Generate code to evaluate an index node (^+) and return the addressing mode
// of the result. This routine takes no flags since it always returns either
// am_ind or am_indx.
//
// No reason to ReleaseTempReg() because the registers used are transported
// forward.
// 
// If we have nested indexes we really want the value from the inner index
// array to use. Hence it is loaded.
// ----------------------------------------------------------------------------
Operand *ENODE::GenerateIndex(bool neg)
{
	Operand *ap1, *ap2;
	static int ndxlvl = 0;

	ndxlvl++;
//	if ((p[0]->nodetype == en_type || p[0]->nodetype == en_regvar)
//		&& (p[1]->nodetype == en_type || p[1]->nodetype == en_regvar))
	/* Both nodes are registers? */
	if (p[0]->nodetype == en_regvar && p[1]->nodetype == en_regvar) {
		ap1 = GenerateRegRegIndex();
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap1->preg), ap1, esize, esize);
			ap1->mode = am_reg;
		}
		ndxlvl--;
		return (ap1);
	}

	GenerateHint(8);
//	GenerateHint(begin_index);
	ap1 = cg.GenerateExpression(p[0], am_reg | am_imm, cpu.sizeOfInt, 1);
	if (ap1->mode == am_imm) {
		ap1 = GenerateImmExprIndex(ap1, neg);
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap1->preg), ap1, esize, esize);
			ap1->mode = am_reg;
		}
		ndxlvl--;
		return (ap1);
	}

	ap2 = cg.GenerateExpression(p[1], am_all, cpu.sizeOfWord, 1);   /* get right op */
//	GenerateHint(end_index);
	GenerateHint(9);

	// Do we have reg+imm? If so make am_indx
	if (ap2->mode == am_imm) {
		ap1 = GenerateRegImmIndex(ap1, ap2, neg);
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap1->preg), ap1, esize, esize);
			ap1->mode = am_reg;
		}
		ndxlvl--;
		return (ap1);
	}

	if (ap2->mode == am_ind && ap1->mode == am_reg) {
		if (cpu.SupportsIndexed && ndxlvl==1) {
			ap2->mode = am_indx2;
			ap2->sreg = ap1->preg;
			ap2->deep2 = ap1->deep;
		}
		else {
			//GenerateTriadic(op_mulu, 0, ap2, ap2, MakeImmediate(this->esize));
			cg.GenerateAdd(ap2, ap1, ap2);
			ap2->mode = am_indx;
			ap2->deep2 = ap1->deep;
			ap2->offset = makeinode(en_icon, 0);
			if (ndxlvl > 1) {
				GenerateLoad(makereg(ap2->preg), ap2, esize, esize);
				ap2->mode = am_reg;
			}
		}
		ap2->scale = 1;
		ndxlvl--;
		return (ap2);
	}

	// Direct plus index register equals register indirect addressing.
	if (ap2->mode == am_direct && ap1->mode == am_reg) {
		ap2->mode = am_indx;
		ap2->preg = ap1->preg;
		ap2->deep = ap1->deep;
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap2->preg), ap2, esize, esize);
			ap2->mode = am_reg;
		}
		ndxlvl--;
		return (ap2);
	}

	// ap1->mode must be am_reg
	ap2->MakeLegal(am_reg, ap2->tp ? ap2->tp->size : cpu.sizeOfWord);
	if (cpu.SupportsIndexed && ndxlvl==1) {
		ap1->mode = am_indx2;            /* make indexed */
		ap1->sreg = ap2->preg;
		ap1->deep2 = ap2->deep;
		ap1->offset = makeinode(en_icon, 0);
		ap1->scale = scale;
	}
	else {
		ap1->mode = am_indx2;            /* make indexed */
		ap1->sreg = ap2->preg;
		ap1->deep2 = ap2->deep;
		ap1->offset = makeinode(en_icon, 0);
		//ap1->scale = 1;
		/*
		ap4 = GetTempRegister();
		ap3 = GetTempRegister();
		ap2->MakeLegal(am_reg, this->esize);
		GenerateTriadic(op_mulu, 0, ap3, ap2, MakeImmediate(this->esize));
		GenerateTriadic(op_add, 0, ap4, ap1, ap3);
		ReleaseTempRegister(ap3);
		*/
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap1->preg), ap1, esize, esize);
			ap1->mode = am_reg;
		}
//		ap1->mode = am_indx;            /* make indexed */
//		ap1->deep2 = ap2->deep;
//		ap1->offset = makeinode(en_icon, 0);
		ndxlvl--;
		return (ap1);                     /* return indexed */
	}
	ndxlvl--;
	return (ap1);                     /* return indexed */
}

Operand* ENODE::GenerateScaledIndexing(int flags, int64_t size, int rhs)
{
	Operand* ap1;
	Operand* ap3;
	Operand* ap4;

	ap1 = GetTempRegister();
	ap4 = cg.GenerateExpression(p[0], flags, cpu.sizeOfWord, rhs);	// base (constant)
	ap3 = cg.GenerateExpression(p[1], flags, cpu.sizeOfWord, rhs);
	ap1->mode = am_indx2;
	ap1->sreg = ap3->preg;
	ap1->deep2 = ap3->deep2;
	ap1->offset = ap4->offset->Clone();
	ap1->isUnsigned = true;
	return (ap1);
}

// Changed ?? to mux operator
// Making this dead code for now.
// Generate code to evaluate a condition operator node (??:)
//
Operand *ENODE::GenerateSafeHook(int flags, int64_t size)
{
	Operand *ap1, *ap2, *ap3, *ap4;
	int false_label, end_label;
	OCODE *ip1;
	int n1;
	ENODE *node;

	false_label = nextlabel++;
	end_label = nextlabel++;
	//flags = (flags & am_reg) | am_volatile;
	flags |= am_volatile;
	/*
	if (p[0]->constflag && p[1]->constflag) {
	GeneratePredicateMonadic(hook_predreg,op_op_ldi,MakeImmediate(p[0]->i));
	GeneratePredicateMonadic(hook_predreg,op_loadi,MakeImmediate(p[0]->i));
	}
	*/
	ip1 = currentFn->pl.tail;
	/* This can't work because it returns -1,0,+1 not -1,+1 */
	/*
	if (p[0]->nodetype == en_lt) {
		ap2 = GetTempRegister();
		ap3 = cg.GenerateExpression(p[1]->p[0], am_all, size);
		ap4 = cg.GenerateExpression(p[1]->p[1], am_all, size);
		if (ap3->mode == am_imm && ap4->mode == am_imm) {
			if (ap3->offset->i == -1 && ap4->offset->i == 1) {
				ap3 = cg.GenerateExpression(p[0]->p[0], am_reg, size);
				ap4 = cg.GenerateExpression(p[0]->p[1], am_reg | am_imm, size);
				GenerateTriadic(op_slt, 0, ap2, ap3, ap4);
				GenerateTriadic(op_asl, 0, ap2, ap2, MakeImmediate(1));	// * 2	result will be 0 or 2
				GenerateTriadic(op_sub, 0, ap2, ap2, MakeImmediate(1));	// - 1	result will be -1 or 1
				ReleaseTempReg(ap4);
				ReleaseTempReg(ap3);
				ap2->MakeLegal(flags, size);
				return (ap2);
			}
		}
	}
	*/
	currentFn->pl.tail = ip1;
	// cmovenz integer only
	if (!opt_nocgo) {
		ap4 = GetTempRegister();
		ap1 = cg.GenerateExpression(p[0], am_reg, size, 0);
		ap2 = cg.GenerateExpression(p[1]->p[0], am_reg, size, 1);
		ap3 = cg.GenerateExpression(p[1]->p[1], am_reg, size, 1);
		if (ap2->tp->IsFloatType() || ap3->tp->IsFloatType())
			goto j1;
		if (ap2->tp->IsPositType() || ap3->tp->IsPositType())
			goto j1;
		n1 = currentFn->pl.Count(ip1);
		if (n1 < 20 && !currentFn->pl.HasCall(ip1)) {
			Generate4adic(op_cmovenz, 0, ap4, ap1, ap2, ap3);
			ReleaseTempReg(ap3);
			ReleaseTempReg(ap2);
			ReleaseTempReg(ap1);
			ap4->MakeLegal(flags,size);
			return (ap4);
		}
j1:
		ReleaseTempReg(ap3);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		ReleaseTempReg(ap4);
		currentFn->pl.tail = ip1;
		currentFn->pl.tail->fwd = nullptr;
	}
	ap2 = cg.GenerateExpression(p[1]->p[1], flags, size, 1);
	n1 = currentFn->pl.Count(ip1);
	if (opt_nocgo)
		n1 = 9999;
	if (n1 > 4 || currentFn->pl.HasCall(ip1))
	{
		ReleaseTempReg(ap2);
		currentFn->pl.tail = ip1;
		currentFn->pl.tail->fwd = nullptr;
		cg.GenerateFalseJump(p[0], false_label, 0);
		node = p[1];
		ap1 = cg.GenerateExpression(node->p[0], flags, size, 0);
		GenerateDiadic(op_branch, 0, MakeCodeLabel(end_label), 0);
		GenerateLabel(false_label);
		ap2 = cg.GenerateExpression(node->p[1], flags, size, 1);
		if (!IsEqualOperand(ap1, ap2))
		{
			GenerateMonadic(op_hint, 0, MakeImmediate(2));
			switch (ap1->mode)
			{
			case am_reg:
				switch (ap2->mode) {
				case am_reg:
					cg.GenerateMove(ap1, ap2);
					break;
				case am_imm:
					cg.GenerateLoadConst(ap2, ap1);
					if (ap2->isPtr)
						ap1->isPtr = true;
					break;
				default:
					GenerateLoad(ap1, ap2, size, size);
					break;
				}
				break;
			case am_fpreg:
				switch (ap2->mode) {
				case am_fpreg:
					cg.GenerateMove(ap1, ap2);
					break;
				case am_imm:
					ap4 = GetTempRegister();
					GenerateDiadic(cpu.ldi_op, 0, ap4, ap2);
					cg.GenerateMove(ap1, ap4);
					if (ap2->isPtr)
						ap1->isPtr = true;
					break;
				default:
					GenerateLoad(ap1, ap2, size, size);
					break;
				}
				break;
			case am_imm:
				break;
			default:
				GenStore(ap2, ap1, size);
				break;
			}
		}
		GenerateLabel(end_label);
		ap1->MakeLegal(flags, size);
		return (ap1);
	}
	// N1 <= 4
	cg.GenerateFalseJump(p[0], false_label, 0);
	node = p[1];
	ap1 = cg.GenerateExpression(node->p[0], flags, size, 0);
	GenerateDiadic(op_branch, 0, MakeCodeLabel(end_label), 0);
	GenerateLabel(false_label);
	if (!IsEqualOperand(ap1, ap2))
	{
		GenerateMonadic(op_hint, 0, MakeImmediate(2));
		switch (ap1->mode)
		{
		case am_reg:
			switch (ap2->mode) {
			case am_reg:
				cg.GenerateMove(ap1, ap2);
				break;
			case am_imm:
				cg.GenerateLoadConst(ap2, ap1);
				if (ap2->isPtr)
					ap1->isPtr = true;
				break;
			default:
				GenerateLoad(ap1, ap2, size, size);
				break;
			}
			break;
		case am_fpreg:
			switch (ap2->mode) {
			case am_fpreg:
				cg.GenerateMove(ap1, ap2);
				break;
			case am_imm:
				ap4 = GetTempRegister();
				GenerateDiadic(cpu.ldi_op, 0, ap4, ap2);
				cg.GenerateMove(ap1, ap4);
				if (ap2->isPtr)
					ap1->isPtr = true;
				break;
			default:
				GenerateLoad(ap1, ap2, size, size);
				break;
			}
			break;
		case am_imm:
			break;
		default:
			GenStore(ap2, ap1, size);
			break;
		}
	}
	GenerateLabel(end_label);
	ap1->MakeLegal(flags, size);
	return (ap1);
}

/* Generate code for a shift operation. A warning message is displayed if the
* shift is known to be by too many bits.
* 
* Parameters:
*		flags (input) the legal address modes to use for the result.
*		size (input) the size of the result in bytes.
*		op (input) the shift opcode.
* 
* Returns:
*		an operand referencing the shift value.
*/
// ToDo: ShiftBitfield
Operand* ENODE::GenerateShift(int flags, int64_t size, int op)
{
	Operand* ap1, * ap2, * ap3;
	Int128 val;

	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(p[0], am_reg, size, 0);
	ap2 = cg.GenerateExpression(p[1], am_reg | am_ui6, cpu.sizeOfWord, 1);
	if (ap2->GetConstValue(&val))
	{
		Int128 sz, eight;
		eight = Int128::Convert(8LL);
		sz = Int128::Convert(TYP::GetSize(ap1->tp->type));
		Int128::Mul(&sz, &sz, &eight);
		if (Int128::IsGT(&val, &sz))
			error(ERR_SHIFT_TOOMANYBITS);
	}
	GenerateTriadic(op, size==cpu.sizeOfWord ?0 : (int)size, ap3, ap1, ap2);
	// Rotates automatically sign extend
	if ((op == op_rol || op == op_ror) && ap2->isUnsigned && !ap1->tp->IsVectorType())
		switch (size) {
		case 1:	ap1 = ap2->GenerateBitfieldClear(8, cpu.sizeOfWord*8-9); break;
		case 2:	ap1 = ap2->GenerateBitfieldClear(16, cpu.sizeOfWord * 8 - 17); break;
		case 4:
			if (cpu.sizeOfWord > 4)
				ap1 = ap2->GenerateBitfieldClear(32, cpu.sizeOfWord * 8 - 33); break;
		case 8:
			if (cpu.sizeOfWord > 8)
				ap1 = ap2->GenerateBitfieldClear(64, cpu.sizeOfWord * 8 - 65); break;
		default:;
		}
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	ap3->MakeLegal(flags, size);
	return (ap3);
}

// ToDo: AssignShiftBitfield
Operand *ENODE::GenerateAssignShift(int flags, int64_t size, int op)
{
	Operand *ap1, *ap2, *ap3;
	MachineReg *mr;

	//size = GetNaturalSize(node->p[0]);
	ap3 = cg.GenerateExpression(p[0], am_all & ~am_imm, size, 0);
	ap2 = cg.GenerateExpression(p[1], am_reg | am_ui6, size, 1);
	if (ap3->mode == am_reg) {
		GenerateTriadic(op, (int)size, ap3, ap3, ap2);
		mr = &regs[ap3->preg];
		if (mr->assigned)
			mr->modified = true;
		mr->assigned = true;
		mr->isConst = ap3->isConst && ap2->isConst;
		ReleaseTempRegister(ap2);
		ap3->MakeLegal(flags, size);
		return (ap3);
	}
	ap1 = GetTempRegister();
	GenerateLoad(ap1, ap3, size, size);
	GenerateTriadic(op, (int)size, ap1, ap1, ap2);
	GenStore(ap1, ap3, size);
	ReleaseTempRegister(ap1);
	ReleaseTempRegister(ap2);
	ap3->MakeLegal(flags, size);
	return (ap3);
}


//
//      generate code to evaluate a mod operator or a divide
//      operator.
//
Operand *ENODE::GenerateDivMod(int flags, int64_t size, int op)
{
	Operand *ap1, *ap2, *ap3, *ap4;

	//if( node->p[0]->nodetype == en_icon ) //???
	//	swap_nodes(node);
	if (op == op_fdiv) {
		ap3 = GetTempFPRegister();
		ap1 = cg.GenerateExpression(p[0], am_reg, cpu.sizeOfFPD, 0);
		ap2 = cg.GenerateExpression(p[1], am_reg, cpu.sizeOfFPD, 1);
	}
	else {
		ap3 = GetTempRegister();
		ap1 = cg.GenerateExpression(p[0], am_reg, 8, 0);
		if (op == op_modu)	// modu only supports register mode
			ap2 = cg.GenerateExpression(p[1], am_reg, 8, 1);
		else
			ap2 = cg.GenerateExpression(p[1], am_reg | am_imm, 8, 1);
	}
	if (op == op_fdiv) {
		// Generate a convert operation ?
		if (ap1->fpsize() != ap2->fpsize()) {
			if (ap2->fpsize() == 's')
				GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
		}
		GenerateTriadic(op, ap1->fpsize(), ap3, ap1, ap2);
	}
	else {
		if (op == op_rem) {
			ap4 = GetTempRegister();
			GenerateTriadic(op_div, 0, ap4, ap1, ap2);
			GenerateTriadic(op_mul, 0, ap4, ap4, ap2);
			GenerateTriadic(op_sub, 0, ap3, ap1, ap4);
			ReleaseTempReg(ap4);
		}
		else
			GenerateTriadic(op, 0, ap3, ap1, ap2);
	}
	//    GenerateDiadic(op_ext,0,ap3,0);
	ap3->MakeLegal( flags, 2);
	ReleaseTempReg(ap2);
	ReleaseTempReg(ap1);
	return (ap3);
}


//
//      generate code to evaluate a multiply node.
//
Operand *ENODE::GenerateMultiply(int flags, int64_t size, int op)
{
	Operand *ap1, *ap2, *ap3, *vap3;
	bool square = false;
	bool vec = false;

	//Enter("Genmul");
	ap2 = nullptr;
	vap3 = GetTempVectorRegister();
	if (p[0]->nodetype == en_icon || p[0]->nodetype == en_fcon || p[0]->nodetype == en_pcon)
		swap_nodes(this);
	if (IsEqual(p[0], p[1]))
		square = !opt_nocgo;
	if (op == op_fmul) {
		ap3 = GetTempFPRegister();
		ap1 = cg.GenerateExpression(p[0], am_reg|am_vreg, size, 0);
		if (!square)
			ap2 = cg.GenerateExpression(p[1], am_reg|am_vreg|am_imm, size, 1);
		if (ap1->mode == am_vreg || (ap2 && ap2->mode == am_vreg)) {
			vec = true;
			op = op_vfmul;
		}
	}
	else {
		ap3 = GetTempRegister();
		ap1 = cg.GenerateExpression(p[0], am_reg|am_vreg, cpu.sizeOfWord, 0);
		if (!square)
			ap2 = cg.GenerateExpression(p[1], am_reg | am_vreg | am_imm, cpu.sizeOfWord, 1);
		if (ap1->mode == am_vreg || (ap2 && ap2->mode == am_vreg)) {
			vec = true;
			op = op_vmul;
		}
	}
	if (op == op_fmul) {
		// Generate a convert operation ?
		if (!square) {
			if (ap1->fpsize() != ap2->fpsize()) {
				if (ap2->fpsize() == 's')
					GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
			}
		}
		if (square)
			GenerateTriadic(op, ap1->fpsize(), ap3, ap1, ap1);
		else
			GenerateTriadic(op, ap1->fpsize(), ap3, ap1, ap2);
	}
	else {
		if (square)
			GenerateTriadic(op, 0, ap3, ap1, ap1);
		else
			GenerateTriadic(op, 0, ap3, ap1, ap2);
	}
	if (!square)
		ReleaseTempReg(ap2);
	ReleaseTempReg(ap1);
	if (vec) {
		ReleaseTempRegister(ap3);
		//		ap3->MakeLegal(flags, 2);
		//Leave("Genmul", 0);
		return (vap3);
	}
	else {
		ReleaseTempRegister(vap3);
		ap3->MakeLegal(flags, 2);
		//Leave("Genmul", 0);
		return (ap3);
	}
}


//
// Generate code to evaluate a unary minus or complement.
//
Operand *ENODE::GenerateUnary(int flags, int64_t size, int op)
{
	Operand *ap, *ap2;

	if (IsFloatType()) {
		ap2 = GetTempFPRegister();
		ap = cg.GenerateExpression(p[0], am_reg, size, 0);
		if (op == op_neg)
			op = op_fneg;
		GenerateDiadic(op, fsize(), ap2, ap);
	}
	else if (IsVectorType()) {
		ap2 = GetTempVectorRegister();
		ap = cg.GenerateExpression(p[0], am_vreg, size, 0);
		GenerateDiadic(op, 0, ap2, ap);
	}
	else {
		ap2 = GetTempRegister();
		ap = cg.GenerateExpression(p[0], am_reg, size, 0);
		/*
		ip = currentFn->pl.tail;
		// Invert Set Operation inline code
		// The following code may be incorrect because the unary op should
		// apply to the next statements not the previous one.
		if (ip->insn->IsSetInsn()) {
			ReleaseTempReg(ap2);
			switch(ip->opcode) {
			case op_seq:
				ip->opcode = op_sne;
				break;
			case op_sne:
				ip->opcode = op_seq;
				break;
			case op_slt:
				ip->opcode = op_sge;
				break;
			case op_sle:
				ip->opcode = op_sgt;
				break;
			case op_sge:
				ip->opcode = op_slt;
				break;
			case op_sgt:
				ip->opcode = op_sle;
				break;
			case op_sltu:
				ip->opcode = op_sgeu;
				break;
			case op_sleu:
				ip->opcode = op_sgtu;
				break;
			case op_sgeu:
				ip->opcode = op_sltu;
				break;
			case op_sgtu:
				ip->opcode = op_sleu;
				break;
			}
			//ip->insn = Instruction::Get(ip->insn->InvertSet());
			ip->insn = Instruction::Get(ip->opcode);
			//ip->opcode = ip->insn->opcode;
			//ap->MakeLegal(flags, size);
			return (ap);
		}
		else
		*/
		{
			GenerateHint(3);
			GenerateDiadic(op, 0, ap2, ap);
		}
	}
	ReleaseTempReg(ap);
	ap2->MakeLegal(flags, size);
	return (ap2);
}

// Generate code for a binary expression

Operand *ENODE::GenerateBinary(int flags, int64_t size, int op)
{
	return (cg.GenerateBinary(this, flags, size, op));
}

Operand *ENODE::GenerateAssignAdd(int flags, int64_t size, int op)
{
	return (cg.GenerateAssignAdd(this, flags, size, op));
}

Operand *ENODE::GenerateAssignLogic(int flags, int64_t size, int op)
{
	Operand *ap1, *ap2, *ap3;
	int64_t ssize;
	MachineReg *mr;

	ssize = p[0]->GetNaturalSize();
	if (ssize > size)
		size = ssize;
	if (p[0]->IsBitfield()) {
		ap3 = GenerateBitfieldAssignLogic(flags, size, op);
		return (ap3);
	}
	ap1 = cg.GenerateExpression(p[0], am_all, ssize, 0);
	// Some of the logic operations don't support immediate mode, so we check
	ap2 = cg.GenerateExpression(p[1], Instruction::Get(op)->amclass3, size, 1);
	if (ap1->mode == am_reg) {
		// Check if an immediate value will fit into the 21-bit immediate field. If
		// not it needs to be loaded into a register.
		if (ap2->mode == am_imm) {
			if (ap2->offset) {
				if (!ap2->offset->i128.IsNBit(21)) {
					ap3 = GetTempRegister();
					cg.GenerateLoadConst(ap2, ap3);
					GenerateTriadic(op, 0, ap1, ap1, ap3);
					ReleaseTempRegister(ap3);
					goto j1;
				}
			}
		}
		GenerateTriadic(op, 0, ap1, ap1, ap2);
j1:
		mr = &regs[ap1->preg];
		if (mr->assigned)
			mr->modified = true;
		mr->assigned = true;
		mr->isConst = ap1->isConst && ap2->isConst;
	}
	else {
		GenMemop(op, ap1, ap2, ssize, etype);
	}
	ReleaseTempRegister(ap2);
	if (!ap1->isUnsigned && !(flags & am_novalue)) {
		if (size > ssize) {
			if (ap1->mode != am_reg) {
				ap1->MakeLegal( am_reg, ssize);
			}
			switch (ssize) {
			case 1:	GenerateDiadic(op_sxb, 0, ap1, ap1); break;
			case 2:	GenerateDiadic(op_sxw, 0, ap1, ap1); break;
			case 4:	GenerateDiadic(op_sxt, 0, ap1, ap1); break;
			}
			ap1->MakeLegal( flags, size);
			return (ap1);
		}
		ap1 = ap1->GenerateSignExtend(ssize, size, flags);
	}
	ap1->MakeLegal( flags, size);
	return (ap1);
}

Operand *ENODE::GenerateLand(int flags, int op, bool safe)
{
	Operand *ap1, *ap2;
	int lab0, lab1;

	if (safe)
		return (cg.GenerateSafeLand(this, flags, op));
	lab0 = nextlabel++;
	lab1 = nextlabel++;
	ap1 = GetTempRegister();
	ap2 = cg.GenerateExpression(this, flags, cpu.sizeOfWord, 1);
	ap1 = cg.MakeBoolean(ap2);
	ReleaseTempReg(ap2);
	/*
	GenerateDiadic(op_loadi, 0, ap1, MakeImmediate(1));
	cg.GenerateFalseJump(this, lab0, 0);
	GenerateDiadic(op_loadi, 0, ap1, MakeImmediate(0));
	GenerateLabel(lab0);
	*/
	ap1->MakeLegal(flags, 8);
	ap1->isBool = true;
	return (ap1);
}

// return the natural evaluation size of a node.
/* Dead Code
int GetNaturalSize(ENODE *node)
{ 
	int siz0, siz1;
	if( node == NULL )
		return 0;
	switch( node->nodetype )
	{
	case en_fieldref:
		return (node->tp->size);
	case en_icon:
		if (node->i >= -128 && node->i < 128)
			return (1);
		if( -32768 <= node->i && node->i <= 32767 )
			return (2);
		if (-2147483648LL <= node->i && node->i <= 2147483647LL)
			return (4);
		return (8);
	case en_fcon:
		return (node->tp->precision / 16);
	case en_tcon: return (6);
	case en_labcon: case en_clabcon:
	case en_cnacon: case en_nacon:  case en_autocon: case en_classcon:
	case en_type:
	case en_byt2octa: case en_ubyt2octa:
	case en_wyde2octa: case en_uwyde2octa:
	case en_tetra2octa: case en_utetra2octa:
	case en_cbu: case en_tetra2hexi:
	case en_cubu: case en_cucu: case en_cuhu:
	case en_ccwp: case en_cucwp:
	case en_sxb:	case en_sxc:	case en_sxh:
		return (cpu.sizeOfWord);
	case en_wyde2hexi:
		return (2);
	case en_fcall:
	case en_regvar:
	case en_fpregvar:
		if (node->tp)
			return (node->tp->size);
		else
			return (cpu.sizeOfWord);
	case en_autofcon:
		return (cpu.sizeOfWord);
	case en_ref:
		return (node->tp->size);
	case en_byt2wyde:
	case en_byt2tetra:	return (4);
	case en_wyde2tetra:	return (4);
	case en_autovcon:
	case en_tempfpref:
	if (node->tp)
		return (node->tp->precision/16);
	else
		return (8);
	case en_not:    case en_compl:
	case en_uminus: case en_assign:
		return node->p[0]->GetNaturalSize();
	case en_fadd:	case en_fsub:
	case en_fmul:	case en_fdiv:
	case en_fsadd:	case en_fssub:
	case en_fsmul:	case en_fsdiv:
	case en_vadd:	case en_vsub:
	case en_vmul:	case en_vdiv:
	case en_vadds:	case en_vsubs:
	case en_vmuls:	case en_vdivs:
	case en_add:    case en_sub:	case en_ptrdif:
	case en_mul:    case en_mulu:
	case en_div:	case en_udiv:
	case en_mod:    case en_umod:
	case en_and:    case en_or:     case en_xor:
	case en_asl:
	case en_shl:    case en_shlu:
	case en_shr:	case en_shru:
	case en_asr:	case en_asrshu:
	case en_feq:    case en_fne:
	case en_flt:    case en_fle:
	case en_fgt:    case en_fge:
	case en_eq:     case en_ne:
	case en_lt:     case en_le:
	case en_gt:     case en_ge:
	case en_ult:	case en_ule:
	case en_ugt:	case en_uge:
	case en_land:   case en_lor:
	case en_land_safe:   case en_lor_safe:
	case en_asadd:  case en_assub:
	case en_asmul:  case en_asmulu:
	case en_asdiv:	case en_asdivu:
	case en_asmod:  case en_asmodu: case en_asand:
	case en_asor:   case en_asxor:	case en_aslsh:
	case en_asrsh:
		siz0 = node->p[0]->GetNaturalSize();
		siz1 = node->p[1]->GetNaturalSize();
		if( siz1 > siz0 )
			return (siz1);
		else
			return (siz0);
	case en_void:   case en_cond:	case en_safe_cond:
		return (node->p[1]->GetNaturalSize());
	case en_bchk:
		return (node->p[0]->GetNaturalSize());
	case en_chk:
		return 8;
	case en_q2i:
	case en_t2i:
		return (cpu.sizeOfWord);
	case en_i2d:
		return (cpu.sizeOfWord);
	case en_i2t:
	case en_d2t:
		return (cpu.sizeOfFPT);
	case en_i2q:
	case en_d2q:
	case en_t2q:
		return (cpu.sizeOfFPQ);
	case en_object_list:
		return (node->p[0]->GetNaturalSize());
	case en_addrof:
		return (cpu.sizeOfPtr);
	default:
		printf("DIAG - natural size error %d.\n", node->nodetype);
		break;
	}
	return (0);
}
*/

static const char* NodeTypeStr[] = {
	"unknown", "void", "nop",
	"list", "aggregate",
	"cbu", "ccu", "chu",
	"cubu", "cucu", "cuhu",
	"cbw", "ccw", "chw",
	"cubw", "cucw", "cuhw",
	"cucwp", "ccwp", "cuc",

	"cbc", "cbh", "cbuc", "cubc",
	"cch",
	"cwl", "cld", "cfd",
	"sxb", "sxc", "sxh",
	"zxb", "zxc", "zxh",
	"i2p", "p2i",
			"icon", "fcon", "fqcon", "dcon", "tcon", "scon", "labcon", "nacon",
			"autocon", "autofcon", "autopcon", "classcon",
	"clabcon", "cnacon",
	"dlabcon", "dnacon", // 30<-

			 "fcall","ifcall",
			 "tempref","regvar","fpregvar","pregvar","tempfpref","temppref",
	"add","sub","mul","mod",
	"ftadd","ftsub","ftmul","ftdiv",
	"fdadd","fdsub","fdmul","fddiv",
	"fsadd","fssub","fsmul","fsdiv",
	"fadd","fsub","fmul","fdiv",
	"padd","psub","pmul","pdiv","ptoi","itop","peq","pne","plt","ple","pcon","pgt","pge",
	"d2t","d2q","t2q",
	"i2d","i2t","i2q","d2i","q2i","s2q","t2i",// 63<-
			"div","asl","shl","shlu","shr","shru","asr","rol","ror","ext","extu",
	"cond","safe_cond","assign",
			"asadd","assub","asmul","asdiv","asdivu","asmod","asmodu",
			"asfmul",
	"asrsh","asrshu","asmulu",//81
			"aslsh","asand","asor","asxor","uminus","not","compl",
			"eq","ne","lt","le","gt","ge",
			"feq","fne","flt","fle","fgt","fge",
			"veq","vne","vlt","vle","vgt","vge",
	"and","or","land","lor","land_safe","lor_safe",//104
			"xor","mulu","udiv","umod","ugt",
			"uge","ule","ult",
	"ref","fieldref","ursh",
	"bchk","chk","bytendx","bitoffset",
	"abs","max","min","addrof","ptrdif","wydendx",
	// Vector
	"autovcon","autovmcon","vector_ref","vex","veins",
	"vadd","vsub","vmul","vdiv",
	"vadds","vsubs","vmuls","vdivs",
	"mulf","isnullptr",
	"en_object_list",
	nullptr
};

char* NodetypeToString(e_node n)
{
	return ((char *)NodeTypeStr[n]);
}

e_node StringToNodetype(char* str)
{
	int n = en_unknown;

	while (NodeTypeStr[n]) {
		if (strncmp(str, NodeTypeStr[n], strlen(NodeTypeStr[n])) == 0)
			return ((e_node)n);
		n++;
	}
	return ((e_node)-1);
};

std::string* ENODE::GetLabconLabel(int64_t ii)
{
	char buf[400];
	int iii = (int)ii;

#ifdef LOCAL_LABELS
	sprintf_s(buf, sizeof(buf), "%s.%05lld", ""/*(char*)currentFn->sym->GetFullName()->c_str()*/, i);
#else
	//		sprintf_s(buf, sizeof(buf), "%s.%05lld", (char*)currentFn->sym->GetFullName()->c_str(), i);
	if (DataLabelMap[iii])
		sprintf_s(buf, sizeof(buf), "%s.%05lld", DataLabelMap[iii]->c_str(), ii);
	else {
		if (sym && sym->storage_class == sc_static)
			DataLabelMap[iii] = new std::string(GetPrivateNamespace());
		else
			DataLabelMap[iii] = new std::string(GetNamespace());
		sprintf_s(buf, sizeof(buf), "%s.%05lld", (char*)DataLabelMap[iii]->c_str(), ii);
	}
#endif
	return (new std::string(buf));
}

void ENODE::PutConstant(txtoStream& ofs, unsigned int lowhigh, unsigned int rshift, bool opt, int display_opt)
{
	// ASM statment text (up to 3500 chars) may be placed in the following buffer.
	static char buf[4000];
	int64_t ndx;
	Posit16 pos16;
	Posit32 pos32;

	if (this == nullptr)
		return;
	if (i128.high != 0 && i128.high != 0xffffffffffffffffLL)
		display_opt = 16;

	// Used only by lea for subtract
	if (isNeg)
		ofs.write("-");
	switch (nodetype)
	{
	case en_autopcon:
	case en_autofcon:
		sprintf_s(buf, sizeof(buf), "%lld", i);
		ofs.write(buf);
		break;
	case en_fcon:
		// Thor: Floats support immediate mode
#ifdef RISCV
		if (!opt) {
			sprintf_s(buf, sizeof(buf), "%s_%lld", (char*)currentFn->sym->GetFullName()->c_str(), i);
			DataLabels[i]++;
			ofs.write(buf);
			break;
		}
#endif
#ifdef THOR
		// The following spits out a warning, but is okay.
		if (true || this->tp->type == bt_quad)
 			sprintf_s(buf, sizeof(buf), "%.16s", f128.ToCompressedString());
		else
			sprintf_s(buf, sizeof(buf), "0x%llx", f);
		ofs.write(buf);
#endif
#ifdef QUPLS
		// The following spits out a warning, but is okay.
		if (true || this->tp->type == bt_quad)
			sprintf_s(buf, sizeof(buf), "%.16s", f128.ToCompressedString());
		else
			sprintf_s(buf, sizeof(buf), "0x%llx", CopyRawDouble(f));
		ofs.write(buf);
#endif
#ifdef LB650
		// The following spits out a warning, but is okay.
		if (true || this->tp->type == bt_quad)
			sprintf_s(buf, sizeof(buf), "%.16s", f128.ToCompressedString());
		else
			sprintf_s(buf, sizeof(buf), "0x%llx", CopyRawDouble(f));
		ofs.write(buf);
#endif
		break;
#ifdef STARK
		// The following spits out a warning, but is okay.
		if (true || this->tp->type == bt_quad)
			sprintf_s(buf, sizeof(buf), "%.16s", f128.ToCompressedString());
		else
			sprintf_s(buf, sizeof(buf), "0x%llx", CopyRawDouble(f));
		ofs.write(buf);
#endif
		break;

	case en_pcon:
		//if (!opt)
		//	goto j1;
		switch (tp->precision) {
		case 16:	pos16 = posit.ConvertTo16();  sprintf_s(buf, sizeof(buf), "0x%04x", pos16.val); break;
		case 32:	pos32 = posit.ConvertTo32();  sprintf_s(buf, sizeof(buf), "0x%08x", pos32.val); break;
		default:
			// The following spits out a warning, but is okay.
			sprintf_s(buf, sizeof(buf), "0x%I64x", posit.val);
		}
		ofs.write(buf);
		break;
	case en_autovcon:
	case en_autocon:
	case en_icon:
		if (lowhigh == 2) {
			sprintf_s(buf, sizeof(buf), "%lld", i & 0xffffff);
			ofs.write(buf);
		}
		else if (lowhigh == 3) {
			sprintf_s(buf, sizeof(buf), "%lld", (i >> 24) & 0xffffff);
			ofs.write(buf);
		}
		else {
			char buf[150];

			switch (display_opt) {
			case 1:
				ofs.write("%");
				ZeroMemory(buf, sizeof(buf));
				if (i128.high != 0 && i128.high != 0xffffffffffffffffLL)
					_itoa_s((int)i128.high, buf, sizeof(buf), 2);
				ndx = strlen(buf);
				_itoa_s((int)i128.low, &buf[ndx], sizeof(buf) - 66, 2);
				if (strlen(&buf[ndx]) < 64) {
					memmove(&buf[ndx + 64 - strlen(&buf[ndx])], &buf[ndx], strlen(&buf[ndx]));
					memset(&buf[ndx], '0', 64 - strlen(&buf[ndx]));
				}
				ofs.write(buf);
				break;
			case 16:
				if (syntax == MOT) {
					buf[0] = '$';
					ndx = 1;
				}
				else {
					buf[0] = '0';
					buf[1] = 'x';
					ndx = 2;
				}
				if (i128.high != 0 && i128.high != 0xffffffffffffffffLL)
					sprintf_s(&buf[ndx], sizeof(buf) - 3, "%08I64X%08I64X", i128.high, i128.low);
				else
					sprintf_s(&buf[ndx], sizeof(buf) - 3, "%08I64X", i128.low);
				break;
			default:
				sprintf_s(buf, sizeof(buf), "%lld", i128.low);
			}
			ofs.write(buf);
		}
		if (rshift > 0) {
			sprintf_s(buf, sizeof(buf), ">>%d", rshift);
			ofs.write(buf);
		}
		break;
	case en_labcon:
		DataLabels[i]++;
		ofs.write(GetLabconLabel(i)->c_str());
		if (rshift > 0) {
			sprintf_s(buf, sizeof(buf), ">>%d", rshift);
			ofs.write(buf);
		}
		break;
	case en_clabcon:
#ifdef LOCAL_LABELS
//		sprintf_s(buf, sizeof(buf), ".C%s_%lld", GetNamespace(), i);
#else
//		sprintf_s(buf, sizeof(buf), ".C%s_%lld", GetNamespace(), i);
#endif
		sprintf_s(buf, sizeof(buf), ".%05d", (int)i);
		DataLabels[i]++;
		ofs.write(buf);
		if (rshift > 0) {
			sprintf_s(buf, sizeof(buf), ">>%d", rshift);
			ofs.write(buf);
		}
		break;
	case en_nacon:
#ifdef QUPLS
		
		if (lowhigh == 1)
			ofs.write("<");
		else if (lowhigh == 2)
			ofs.write("?");
		else if (lowhigh == 3)
			ofs.write(">");
		
#endif
		/* 
		* The following uses a private namespace if sym is set, which is set for 
		* string constants while parsing expressions. String constants end up as
		* rodata items and are made local to the file with GetPrivateNamespace().
		* A matching label is output when the literal table is dumped.
		*/
		if (sym) {
			Symbol* s = sym;
			ENODE* pn = this;
			e_sc sc;

			sc = s->storage_class;
 			if (sc == sc_member) {
				sc = pn->sc;
			}
			if (sc == sc_global || sc==sc_external)
				sprintf_s(buf, sizeof(buf), "%s", (char*)sp->c_str());
			else
				sprintf_s(buf, sizeof(buf), "%.400s.%05I64d", (char*)GetPrivateNamespace(), sym->value.i);
		}
		else
			sprintf_s(buf, sizeof(buf), "%s", (char*)sp->c_str());
		ofs.write(buf);
		if (lowhigh == 3) {
			sprintf_s(buf, sizeof(buf), ">>16");
			ofs.write(buf);
		}
		if (rshift > 0) {
			sprintf_s(buf, sizeof(buf), ">>%d", rshift);
			ofs.write(buf);
		}
		break;
	case en_scon:
		sprintf_s(buf, sizeof(buf), "\"%s\",0", (char *)sp->c_str());
		ofs.write(buf);
		break;
	case en_cnacon:
		if (mangledNames)
			sprintf_s(buf, sizeof(buf), "%s", (char *)msp->c_str());
		else
			sprintf_s(buf, sizeof(buf), "%s", (char*)sp->c_str());
		ofs.write(buf);
		if (rshift > 0) {
			sprintf_s(buf, sizeof(buf), ">>%d", rshift);
			ofs.write(buf);
		}
		break;
	case en_add:
		p[0]->PutConstant(ofs, 0, 0);
		ofs.write("+");
		p[1]->PutConstant(ofs, 0, 0);
		break;
	case en_sub:
		p[0]->PutConstant(ofs, 0, 0);
		ofs.write("-");
		p[1]->PutConstant(ofs, 0, 0);
		break;
	case en_ptrdif:
		ofs.printf("(");
		p[0]->PutConstant(ofs, 0, 0);
		ofs.write("-");
		p[1]->PutConstant(ofs, 0, 0);
		ofs.printf(") >> ");
		p[4]->PutConstant(ofs, 0, 0);
		break;
	case en_uminus:
		ofs.write("-");
		p[0]->PutConstant(ofs, 0, 0);
		break;
	case en_regvar:
		ofs.write(cpu.RegMoniker(rg));
		break;
	default:
		if (nodetype != en_assign && nodetype != en_unknown)
			printf("DIAG - illegal constant node.\n");
		break;
	}
}

extern int outcol;
extern int gentype;

void ENODE::GenerateShort(txtoStream& tfs)
{
	Int128 i128, t128;

	if (gentype == halfgen && outcol < 60) {
		tfs.printf(",");
		if (p[1]) {
			i128 = p[1]->i128;
			t128 = Int128(p[0]->tp->size);
			Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
			PutConstant(tfs, 0, 0, false, 0);
			p[1]->i128 = i128;
			p[1]->i = i128.low;
		}
		else
			PutConstant(tfs, 0, 0, false, 0);
		outcol += 10;
	}
	else {
		nl(tfs);
		if (syntax == MOT) {
			ofs.printf("\tdc.l\t");
			if (p[1]) {
				i128 = p[1]->i128;
				t128 = Int128(p[0]->tp->size);
				Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
				PutConstant(ofs, 0, 0, false, 0);
				p[1]->i128 = i128;
				p[1]->i = i128.low;
			}
			else
				PutConstant(tfs, 0, 0, false, 0);
		}
		else {
			ofs.printf("\t.4byte\t");
			if (p[1]) {
				i128 = p[1]->i128;
				t128 = Int128(p[0]->tp->size);
				Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
				PutConstant(tfs, 0, 0, false, 0);
				p[1]->i128 = i128;
				p[1]->i = i128.low;
			}
			else
				PutConstant(tfs, 0, 0, false, 0);
		}
		gentype = halfgen;
		outcol = 25;
	}
	genst_cumulative += 4;
}

void ENODE::GenerateInt(txtoStream& tfs)
{
	Int128 i128, t128;

	if (gentype == halfgen && outcol < 60) {
		if (p[0]) {
			if (p[0]->tp) {
				tfs.printf(",");
				if (p[1]) {
					i128 = p[1]->i128;
					t128 = Int128(p[0]->tp->size);
					Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
					PutConstant(tfs, 0, 0, false, 0);
					p[1]->i128 = i128;
					p[1]->i = i128.low;
				}
				else
					PutConstant(tfs, 0, 0, false, 0);
				outcol += 10;
			}
		}
	}
	else {
		nl(tfs);
		if (syntax == MOT) {
			tfs.printf("\tdc.d\t");
			if (p[1]) {
				i128 = p[1]->i128;
				t128 = Int128(p[0]->tp->size);
				Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
				PutConstant(tfs, 0, 0, false, 0);
				p[1]->i128 = i128;
				p[1]->i = i128.low;
			}
			else
				PutConstant(tfs, 0, 0, false, 0);
		}
		else {
			if (p[0]) {
				if (p[0]->tp) {
					tfs.printf("\t.8byte\t");
					if (p[1]) {
						i128 = p[1]->i128;
						t128 = Int128(p[0]->tp->size);
						Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
						PutConstant(tfs, 0, 0, false, 0);
						p[1]->i128 = i128;
						p[1]->i = i128.low;
					}
					else
						PutConstant(tfs, 0, 0, false, 0);
				}
			}
		}
		gentype = halfgen;
		outcol = 25;
	}
	genst_cumulative += 4;
}

void ENODE::GenerateLong(txtoStream& tfs)
{
	Int128 i128, t128;

	if (gentype == longgen && outcol < 60) {
		tfs.printf(",");
		if (p[1]) {
			i128 = p[1]->i128;
			t128 = Int128(p[0]->tp->size);
			Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
			PutConstant(tfs, 0, 0, false, 0);
			p[1]->i128 = i128;
			p[1]->i = i128.low;
		}
		else
			PutConstant(tfs, 0, 0, false, 0);
		outcol += 10;
	}
	else {
		nl(tfs);
		if (syntax == MOT) {
			ofs.printf("\tdc.q\t");
			if (p[1]) {
				i128 = p[1]->i128;
				t128 = Int128(p[0]->tp->size);
				Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
				PutConstant(tfs, 0, 0, false, 0);
				p[1]->i128 = i128;
				p[1]->i = i128.low;
			}
			else
				PutConstant(tfs, 0, 0, false, 0);
		}
		else {
			tfs.printf("\t.8byte\t");
			if (p[1]) {
				i128 = p[1]->i128;
				t128 = Int128(p[0]->tp->size);
				Int128::Mul(&p[1]->i128, &p[1]->i128, &t128);
				PutConstant(tfs, 0, 0, false, 0);
				p[1]->i128 = i128;
				p[1]->i = i128.low;
			}
			else
				PutConstant(tfs, 0, 0, false, 0);
		}
		gentype = longgen;
		outcol = 25;
	}
	genst_cumulative += 4;
}

ENODE *ENODE::GetConstantHex(std::ifstream& ifs)
{
	static char buf[4000];
	ENODE *ep;

	ep = allocEnode();
	ifs.read(buf, 2);
	buf[2] = '\0';
	ep->nodetype = (e_node)strtol(buf, nullptr, 16);
	switch (ep->nodetype) {
	case en_autopcon:
	case en_autofcon:
		ifs.read(buf, 8);
		buf[8] = '\0';
		ep->i = strtol(buf, nullptr, 16);
		break;
	case en_fcon:
		goto j1;
	case en_autovcon:
	case en_autocon:
	case en_icon:
		ifs.read(buf, 8);
		buf[8] = '\0';
		ep->i = strtol(buf, nullptr, 16);
		break;
	case en_labcon:
j1:		;
	}
	return (ep);
}

void ENODE::PutConstantHex(txtoStream& ofs, unsigned int lowhigh, unsigned int rshift)
{
	// ASM statment text (up to 3500 chars) may be placed in the following buffer.
	static char buf[4000];

	ofs.printf("N%02X", nodetype);
	switch (nodetype)
	{
	case en_autopcon:
	case en_autofcon:
		sprintf_s(buf, sizeof(buf), "%08I64X", i);
		ofs.write(buf);
		break;
	case en_pcon:
	case en_fcon:
		sprintf_s(buf, sizeof(buf), "%s_%lld:", GetNamespace(), i);
		ofs.write(buf);
		break;
	case en_autovcon:
	case en_autocon:
	case en_icon:
		sprintf_s(buf, sizeof(buf), "%08llX", i);
		ofs.write(buf);
		break;
	case en_labcon:
		sprintf_s(buf, sizeof(buf), "%s_%lld:", GetNamespace(), i);
		ofs.write(buf);
		break;
	case en_clabcon:
		sprintf_s(buf, sizeof(buf), "%s_%lld:", GetNamespace(), i);
		ofs.write(buf);
		break;
	case en_nacon:
		sprintf_s(buf, sizeof(buf), "%s:", (char *)sp->c_str());
		ofs.write(buf);
		break;
	case en_cnacon:
		sprintf_s(buf, sizeof(buf), "%s:", (char *)msp->c_str());
		ofs.write(buf);
		break;
	case en_add:
		p[0]->PutConstantHex(ofs, 0, 0);
		ofs.write("+");
		p[1]->PutConstantHex(ofs, 0, 0);
		break;
	case en_sub:
		p[0]->PutConstantHex(ofs, 0, 0);
		ofs.write("-");
		p[1]->PutConstantHex(ofs, 0, 0);
		break;
	case en_uminus:
		ofs.write("-");
		p[0]->PutConstantHex(ofs, 0, 0);
		break;
	default:
		if (nodetype != en_assign)
			printf("DIAG - illegal constant node.\n");
		break;
	}
}

void ENODE::storeHex(txtoStream& ofs)
{
	if (this == nullptr) {
		ofs.puts("x");
		return;
	}
	ofs.puts("X");
	ofs.writeAsHex((char *)this, sizeof(ENODE));
	if (tp)
		ofs.printf("%05X:", tp->typeno);
	else
		ofs.printf("FFFFF:");
	if (sym)
		ofs.printf("%05X:", sym->number);
	else
		ofs.printf("FFFFF:");
	vmask->store(ofs);
	if (sp) {
		ofs.printf("%03X:", (int)sp->length());
		ofs.writeAsHex(sp->c_str(), sp->length());
	}
	else
		ofs.printf("000:");
	if (msp) {
		ofs.printf("%03X:", (int)msp->length());
		ofs.writeAsHex(msp->c_str(), msp->length());
	}
	else
		ofs.printf("000:");
	if (udnm) {
		ofs.printf("%03X:", (int)udnm->length());
		ofs.writeAsHex(udnm->c_str(), udnm->length());
	}
	else
		ofs.printf("000:");
	p[0]->store(ofs);
	p[1]->store(ofs);
	p[2]->store(ofs);
}

void ENODE::loadHex(txtiStream& ifs)
{
	int nn;
	static char buf[8000];

	ifs.readAsHex(this, sizeof(ENODE));
	ifs.read(buf, 6);
	nn = strtoul(buf, nullptr, 16);
	if (nn < 65535)
		tp = &compiler.typeTable[nn];
	ifs.read(buf, 6);
	nn = strtoul(buf, nullptr, 16);
	if (nn < 10*32768)
		sym = &compiler.symTables[nn >> 15][nn & 0x7fff];
	vmask = allocEnode();
	vmask->load(ifs);
	ifs.read(buf, 4);
	nn = strtoul(buf, nullptr, 16);
	ifs.readAsHex(buf, nn * 2);
	buf[nn * 2] = '\0';
	sp = new std::string(buf);
	ifs.read(buf, 4);
	nn = strtoul(buf, nullptr, 16);
	ifs.readAsHex(buf, nn * 2);
	buf[nn * 2] = '\0';
	msp = new std::string(buf);
	ifs.read(buf, 4);
	nn = strtoul(buf, nullptr, 16);
	ifs.readAsHex(buf, nn * 2);
	buf[nn * 2] = '\0';
	udnm = new std::string(buf);
	ifs.read(buf, 1);	// should be 'X'
	if (buf[0]=='X')
		p[0]->load(ifs);
	ifs.read(buf, 1);
	if (buf[0] == 'X')
		p[1]->load(ifs);
	ifs.read(buf, 1);
	if (buf[0] == 'X')
		p[2]->load(ifs);
}

void ENODE::store(txtoStream& ofs)
{
	if (this == nullptr)
		return;
	ofs.printf("<ENODE><number:%d>", number);
	ofs.printf("<nodetype:%s>", NodetypeToString(nodetype));
	p[0]->store(ofs);
	ofs.printf(",");
	p[1]->store(ofs);
	ofs.printf(",");
	p[2]->store(ofs);
	ofs.printf(",");
	p[3]->store(ofs);
	ofs.printf("</ENODE>");
}

void ENODE::load(txtiStream& ifs)
{
	static char buf[8000];
	int nt;

	ifs.read(buf, 16);
	if (buf[0] == '(') {
		nt = StringToNodetype(&buf[1]);
		if (nt >= 0) {
			p[0]->load(ifs);
			p[1]->load(ifs);
			p[2]->load(ifs);
			p[3]->load(ifs);
		}
	}
}

int ENODE::load(char *buf)
{
	int ndx;

	if (buf[0] == '\0')
		return (0);
	if (strncmp(&buf[0],"<number:",8)==0) {
		number = atoi(&buf[8]);
		for (ndx = 8; buf[ndx]; ndx++)
			if (buf[ndx] == '>')
				break;
		if (buf[ndx] == '\0')
			return (ndx);
		ndx++;
		if (strncmp(&buf[ndx], "<nodetype:", 10) == 0) {
			nodetype = StringToNodetype(&buf[ndx+10]);
		}
		p[0] = compiler.ef.Makenode();
		ndx += p[0]->load(&buf[ndx]);
		p[1] = compiler.ef.Makenode();
		ndx += p[1]->load(&buf[ndx]);
		p[2] = compiler.ef.Makenode();
		ndx += p[2]->load(&buf[ndx]);
		p[3] = compiler.ef.Makenode();
		ndx += p[3]->load(&buf[ndx]);
	}
	return (0);
}

int64_t ENODE::PutStructConst(txtoStream& ofs)
{
	int64_t n, k;
	ENODE *ep1, *ep2;
	ENODE *ep = this;
	bool isStruct;
	bool isArray;

	if (ep == nullptr)
		return (0);
	if (ep->nodetype != en_end_aggregate)
		return (0);

	isStruct = ep->tp->IsStructType();
	isArray = ep->tp->type == bt_array;
	if (isArray)
		ofs.printf("\talign %ld\n", (int)ep->p[0]->p[1]->tp->walignment());
	//lst = (List*)p[1];
	ep2 = p[1];
//	for (n = 0; lst; lst = lst->nxt) {
	for (n = 0; ep2; ep2 = ep2->p[0]) {
//			ep1 = lst->node;
		ep1 = ep2->p[1];
		if (ep1->nodetype == en_end_aggregate) {
			k = ep1->PutStructConst(ofs);
		}
		else {
			if (isStruct) {
				switch (ep1->tp->walignment()) {
				case 1:	break;
				default: ofs.printf("align %ld\t", (int)ep1->tp->walignment()); break;
				}
				k = ep1->tp->struct_offset + ep1->esize;
			}
			else if (isArray)
				k = ep1->tp->struct_offset + ep1->esize;
			else
				k = ep1->esize;
			switch (ep1->esize) {
			case 1:	ofs.printf("\tdcb\t");	ep1->PutConstant(ofs, 0, 0, true); ofs.printf("\n"); break;
			case 2:	ofs.printf("\tdcw\t");	ep1->PutConstant(ofs, 0, 0, true); ofs.printf("\n"); break;
			case 4:	ofs.printf("\tdct\t");	ep1->PutConstant(ofs, 0, 0, true); ofs.printf("\n"); break;
			case 8:	ofs.printf("\tdco\t");	ep1->PutConstant(ofs, 0, 0, true); ofs.printf("\n"); break;
			default:
				ofs.printf("\tfill.b %ld,0x00\n", ep1->esize - 1);
				ofs.printf("\tdcb\t");
				ep1->PutConstant(ofs, 0, 0, true);
				ofs.printf("\n");
				break;
			}
		}
		if (isStruct|isArray)
			n = k;
		else
			n = n + k;
	}
	if (n < ep->esize) {
		ofs.printf("fill.b %ld,0x00\n", ep->esize - n);
	}
	return (n);
}

// ============================================================================
// ============================================================================
// Debugging
// ============================================================================
// ============================================================================

std::string ENODE::nodetypeStr()
{
	switch (nodetype) {
	case en_regvar:	return "en_regvar";
	case en_autocon: return "en_autocon";
	case en_cond:	return "en_cond";
	case en_void: return "en_void";
	case en_cast: return "en_cast";
	case en_asadd: return "en_asadd";
	case en_icon: return "en_icon";
	case en_fcon: return "en_fcon";
	case en_assign: return "en_assign";
	case en_eq: return "en_eq";
	case en_land: return "en_land";
	case en_not: return "en_not";
	case en_octa2hexi: return "en_octa2hexi";
	case en_add: return "en_add";
	case en_sub: return "en_sub";
	case en_assub: return "en_assub";
	case en_div: return "en_div";
	case en_fmul: return "en_fmul";
	case en_type: return "en_type";
	case en_compl: return "en_compl";
	case en_i2q: return "en_i2q";
	case en_list: return "en_list";
	default:
		if (IsRefType()) {
			return "en_ref";
		}
		break;
	}
	return "???";
}

void ENODE::Dump(int pn)
{
	int nn;
	static int level = 0;
	List* lst;

	//return;
	if (this == nullptr)
		return;
	for (nn = 0; nn < level * 2; nn++)
		dfs.printf(" ");
	dfs.printf("Node%d:", pn);
	dfs.printf("%d\n", number);
	for (nn = 0; nn < level * 2; nn++)
		dfs.printf(" ");
	dfs.printf("nodetype: %d: ", nodetype);
	dfs.printf("%s\n", (char *)nodetypeStr().c_str());
	for (nn = 0; nn < level * 2; nn++)
		dfs.printf(" ");
	dfs.printf("rg: %d\n", rg);
	if (p[0])
		p[0]->Dump(0);
	if (nodetype != en_aggregate) {
		if (p[1])
			p[1]->Dump(1);
	}
	else {
		for (lst = (List*)p[1]; lst; lst = lst->nxt) {
			level += 3;
			lst->node->Dump(0);
			level -= 3;
		}
	}
}

void ENODE::CountSegments()
{
	if (this) {
		segcount[segment]++;
		if (p[0])	p[0]->CountSegments();
		if (nodetype != en_aggregate && nodetype != en_icon) {
			if (p[1])	p[1]->CountSegments();
			if (p[2])	p[2]->CountSegments();
			if (p[3])	p[3]->CountSegments();
		}
	}
}

void ENODE::GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width)
{
	cg.GenerateBitfieldInsert(ap1, ap2, offset, width);
}
