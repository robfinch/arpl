// ============================================================================
//        __
//   \\__/ o\    (C) 2024  Robert Finch, Waterloo
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
#include "..\..\stdafx.h"

//
// Returns the desirability of optimization for a subexpression.
//
// Immediate constants have low priority because small constants
// can be directly encoded in the instruction. There's no value to
// placing them in registers.

int starkCSE::OptimizationDesireability()
{
	int depth;

	if (exp==nullptr)
		return (0);
	if( voidf )
    return (0);
	if (exp->nodetype == en_icon) {// &&
		// zero can have r0 substituted.
		if (exp->i == 0)
			return (0);
		if (exp->i128.IsNBit(cpu.RIimmSize))
			return (0);
		if (exp->i128.IsNBit(48))
			return (uses);
		// Optimize large constants
		return (uses * 2);
	}
	/* added this line to disable register optimization of global variables.
    The compiler would assign a register to a global variable ignoring
    the fact that the value might change due to a subroutine call.
  */
	if (exp->nodetype == en_nacon)
		return (0);
	// Duh, lets not optimize to replace one regvar with another.
	if (exp->nodetype == en_regvar || exp->nodetype == en_fpregvar) {
		if (!IsTempReg(exp->rg))
			return (0);
		else
			return (100);
	}
	// No value to optimizing function call names, the called function
	// address will typically fit in a single 32/48 bit opcode. It's faster
	// to call a fixed label rather than an address in a register, because
	// the address is known at the fetch phase of the processor. A register
	// based address may be predicted by the processor and so is almost as
	// fast. Storing the address in a register can reduce the size of code.
	// If the function name can be optimized to a register then
	// a 16-bit compressed JAL can be used.
	if (exp->nodetype == en_cnacon)// && !opt_size)
		return (0);
	// If the expression is volatile eg. reading I/O we don't want to
	// replace it.
	if (exp->isVolatile)
		return (0);
	// Prevent Inline code from being allocated a pointer in a register.
	if (exp->sym) {
		if (exp->sym->fi) {
			if (exp->sym->fi->IsInline)
				return (0);
		}
	}
	// Depth is the distance in terms of nested methods from the var
	// being accessed to the current function. Distant variables may
	// require a lot of dereferencing so it is better to place them in
	// registers. The number of dereferences is multiplied by the usage.
	depth = 0;
		if (exp->sym)
			depth = abs(currentFn->depth - exp->sym->depth);
	depth = max(depth, 0);

	// Left values are worth more to optimization than right values.
  if( exp->IsLValue() )
	  return (2 * uses * (depth+1));
  return (uses * (depth+1));
}
