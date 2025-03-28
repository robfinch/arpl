// ============================================================================
//        __
//   \\__/ o\    (C) 2012-2025  Robert Finch, Waterloo
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

void makename_s(char *s, size_t ln, char *e);
void summary();
int options(char *);
int openfiles(char *);
void closefiles();
int PreProcessFile(char *);
extern void initRegs();
bool append = false;

char irfile[256],
	infile[256],
                listfile[256],
                outfile[256],
				outfileG[256];
std::string	dbgfile;

extern TABLE    tagtable;
int		mainflag;
extern int      total_errors;
int uctran_off;
extern int lstackptr;

int64_t rand64()
{
	int64_t r;

	r = 0x0000000000000000LL;
	r |= (int64_t)rand() << 48LL;
	r |= (int64_t)rand() << 32LL;
	r |= (int64_t)rand() << 16LL;
	r |= (int64_t)rand();
	return (r);
}


int main(int argc, char **argv)
{
	Posit64 pst;
	Posit64 a, b, c, d, e;
	txtoStream ofs;
	Int128 aa, bb, cc, qq, rr;
	int xx;

	save_mask = new CSet();
	fpsave_mask = new CSet();
	psave_mask = new CSet();

	syntax = STD;
	aa = Int128::Convert(0x769bdd5fLL);
	bb = Int128::Convert(0xbcc6f09eLL);
	Int128::Mul(&cc, &aa, &bb);
	/*
	aa.low = 0;
	aa.high = 100;
	bb.low = 20;
	bb.high = 0;
	cc.Div(&qq, &rr, &aa, &bb);
	
	pst = pst.IntToPosit(100);
	a = a.IntToPosit(50);
	b = b.IntToPosit(50);
	c = a.Add(a, b);
	a = a.IntToPosit(100);
	b = b.IntToPosit(10);
	c = c.Divide(a, b);
	ofs.open("d:/cores2020/rtf64/v2/software/examples/positTest.txt", std::ios::out | std::ios::trunc);
	for (cnt = 0; cnt < 30000; cnt++) {
		switch (cnt) {
		case 0:
			a = a.IntToPosit(10);
			b = b.IntToPosit(10);
			break;
		case 1:
			a = a.IntToPosit(10);
			b = b.IntToPosit(1);
			break;
		case 2:
			a = a.IntToPosit(1);
			b = b.IntToPosit(10);
			break;
		case 3:
			a = a.IntToPosit(100);
			b = b.IntToPosit(10);
			break;
		case 4:
			a = a.IntToPosit(2);
			b = b.IntToPosit(2);
			break;
		case 5:
			a = a.IntToPosit(-10);
			b = b.IntToPosit(-10);
			break;
		default:
			a.val = rand64();
			b.val = rand64();
		}
		ofs.printf("%08I64X ", a.val);
		ofs.printf("%08I64X ", b.val);
		c = c.Add(a, b);
		ofs.printf("%08I64X ", c.val);
		d = d.Multiply(a, b);
		ofs.printf("%08I64X ", d.val);
		e = e.Divide(a, b);
		ofs.printf("%08I64X ", e.val);
		ofs.printf("\n");
	}
	ofs.flush();
	ofs.close();
	exit(0);
	*/
	opt_nopeep = FALSE;
	uctran_off = 0;
	optimize =1;
	compiler.exceptions=0;
	compiler.nogcskips = true;
	compiler.os_code = false;
#ifdef STARK
	cpu.NumTmpCrRegs = 3;
	cpu.fileExt = ".asm";
	cpu.lea_op = op_lda;
	cpu.pagesize = 14;
	cpu.RIimmSize = 32;
	cpu.cache_line_size = 6;
	cpu.code_align = 6;					// power of two
	cpu.SupportsBand = false;
	cpu.SupportsBor = false;
	cpu.SupportsBBC = true;
	cpu.SupportsBBS = true;
	cpu.SupportsPop = false;
	cpu.SupportsPush = false;
	cpu.SupportsLink = false;
	cpu.SupportsUnlink = false;
	cpu.SupportsBitfield = false;
	cpu.SupportsLDM = false;
	cpu.SupportsSTM = false;
	cpu.SupportsPtrdif = false;
	cpu.SupportsEnter = true;
	cpu.SupportsLeave = true;
	cpu.SupportsIndexed = true;
	cpu.SupportsTrinary = true;
	cpu.Addsi = false;
	cpu.ext_op = op_ext;
	cpu.extu_op = op_extu;
	cpu.mov_op = op_move;
	cpu.ldi_op = op_loadi;
	cpu.ldbu_op = op_ldbu;
	cpu.ldb_op = op_ldb;
	cpu.ldo_op = op_ldo;
	cpu.ldtu_op = op_ldtu;
	cpu.ldt_op = op_ldt;
	cpu.ldwu_op = op_ldwu;
	cpu.ldw_op = op_ldw;
	cpu.ldd_op = op_ldd;
	cpu.stb_op = op_stb;
	cpu.sto_op = op_sto;
	cpu.stt_op = op_stt;
	cpu.stw_op = op_stw;
	cpu.std_op = op_std;
#endif
#ifdef LB650
	cpu.fileExt = ".asm";
	cpu.lea_op = op_lda;
	cpu.pagesize = 14;
	cpu.RIimmSize = 32;
	cpu.cache_line_size = 6;
	cpu.code_align = 6;					// power of two
	cpu.SupportsBand = false;
	cpu.SupportsBor = false;
	cpu.SupportsBBC = true;
	cpu.SupportsBBS = true;
	cpu.SupportsPop = false;
	cpu.SupportsPush = false;
	cpu.SupportsLink = false;
	cpu.SupportsUnlink = false;
	cpu.SupportsBitfield = false;
	cpu.SupportsLDM = false;
	cpu.SupportsSTM = false;
	cpu.SupportsPtrdif = false;
	cpu.SupportsEnter = true;
	cpu.SupportsLeave = true;
	cpu.SupportsIndexed = true;
	cpu.SupportsTrinary = true;
	cpu.Addsi = false;
	cpu.ext_op = op_ext;
	cpu.extu_op = op_extu;
	cpu.mov_op = op_move;
	cpu.ldi_op = op_loadi;
	cpu.ldbu_op = op_ldbu;
	cpu.ldb_op = op_ldb;
	cpu.ldo_op = op_ldo;
	cpu.ldtu_op = op_ldtu;
	cpu.ldt_op = op_ldt;
	cpu.ldwu_op = op_ldwu;
	cpu.ldw_op = op_ldw;
	cpu.ldd_op = op_ldd;
	cpu.stb_op = op_stb;
	cpu.sto_op = op_sto;
	cpu.stt_op = op_stt;
	cpu.stw_op = op_stw;
	cpu.std_op = op_std;
#endif
#ifdef QUPLS
	cpu.fileExt = ".asm";
	cpu.lea_op = op_lda;
	cpu.pagesize = 14;
	cpu.RIimmSize = 32;
	cpu.cache_line_size = 6;
	cpu.code_align = 6;					// power of two
	cpu.SupportsBand = false;
	cpu.SupportsBor = false;
	cpu.SupportsBBC = true;
	cpu.SupportsBBS = true;
	cpu.SupportsPop = false;
	cpu.SupportsPush = false;
	cpu.SupportsLink = false;
	cpu.SupportsUnlink = false;
	cpu.SupportsBitfield = false;
	cpu.SupportsLDM = false;
	cpu.SupportsSTM = false;
	cpu.SupportsPtrdif = false;
	cpu.SupportsEnter = true;
	cpu.SupportsLeave = true;
	cpu.SupportsIndexed = true;
	cpu.SupportsTrinary = true;
	cpu.Addsi = false;
	cpu.pushpop_multiple = 4;	// Max number of register supported by a push / pop operation
	cpu.ext_op = op_ext;
	cpu.extu_op = op_extu;
	cpu.mov_op = op_move;
	cpu.ldi_op = op_loadi;
	cpu.ldbu_op = op_ldbu;
	cpu.ldb_op = op_ldb;
	cpu.ldo_op = op_ldo;
	cpu.ldtu_op = op_ldtu;
	cpu.ldt_op = op_ldt;
	cpu.ldwu_op = op_ldwu;
	cpu.ldw_op = op_ldw;
	cpu.ldd_op = op_ldd;
	cpu.stb_op = op_stb;
	cpu.sto_op = op_sto;
	cpu.stt_op = op_stt;
	cpu.stw_op = op_stw;
	cpu.std_op = op_std;
#endif
#ifdef QUPLS40
	cpu.fileExt = ".asm";
	cpu.lea_op = op_lda;
	cpu.pagesize = 14;
	cpu.RIimmSize = 24;
	cpu.cache_line_size = 6;
	cpu.code_align = 6;					// power of two
	cpu.SupportsBand = false;
	cpu.SupportsBor = false;
	cpu.SupportsBBC = true;
	cpu.SupportsBBS = true;
	cpu.SupportsPop = false;
	cpu.SupportsPush = false;
	cpu.SupportsLink = false;
	cpu.SupportsUnlink = false;
	cpu.SupportsBitfield = false;
	cpu.SupportsLDM = false;
	cpu.SupportsSTM = false;
	cpu.SupportsPtrdif = false;
	cpu.SupportsEnter = false;
	cpu.SupportsLeave = false;
	cpu.SupportsIndexed = true;
	cpu.SupportsTrinary = true;
	cpu.Addsi = false;
	cpu.ext_op = op_ext;
	cpu.extu_op = op_extu;
	cpu.mov_op = op_mov;
	cpu.ldi_op = op_loadi;
	cpu.ldbu_op = op_ldbu;
	cpu.ldb_op = op_ldb;
	cpu.ldo_op = op_ldo;
	cpu.ldtu_op = op_ldtu;
	cpu.ldt_op = op_ldt;
	cpu.ldwu_op = op_ldwu;
	cpu.ldw_op = op_ldw;
	cpu.ldd_op = op_ldd;
	cpu.stb_op = op_stb;
	cpu.sto_op = op_sto;
	cpu.stt_op = op_stt;
	cpu.stw_op = op_stw;
	cpu.std_op = op_std;
#endif
#ifdef THOR
	cpu.fileExt = ".asm";
	cpu.lea_op = op_lea;
	cpu.pagesize = 14;
	cpu.cache_line_size = 6;
	cpu.code_align = 0;
	cpu.SupportsBand = false;
	cpu.SupportsBor = false;
	cpu.SupportsBBC = true;
	cpu.SupportsBBS = true;
	cpu.SupportsPop = false;
	cpu.SupportsPush = false;
	cpu.SupportsLink = false;
	cpu.SupportsUnlink = false;
	cpu.SupportsBitfield = false;
	cpu.SupportsLDM = false;
	cpu.SupportsSTM = false;
	cpu.SupportsPtrdif = false;
	cpu.SupportsEnter = false;
	cpu.SupportsLeave = false;
	cpu.SupportsIndexed = true;
	cpu.SupportsTrinary = false;
	cpu.Addsi = false;
	cpu.ext_op = op_ext;
	cpu.extu_op = op_extu;
	cpu.mov_op = op_mov;
	cpu.ldi_op = op_loadi;
	cpu.ldbu_op = op_ldbu;
	cpu.ldb_op = op_ldb;
	cpu.ldo_op = op_ldo;
	cpu.ldtu_op = op_ldtu;
	cpu.ldt_op = op_ldt;
	cpu.ldwu_op = op_ldwu;
	cpu.ldw_op = op_ldw;
	cpu.ldd_op = op_ldd;
	cpu.stb_op = op_stb;
	cpu.sto_op = op_sto;
	cpu.stt_op = op_stt;
	cpu.stw_op = op_stw;
	cpu.std_op = op_std;
#endif
#ifdef RISCV
	cpu.nregs = 32;
	cpu.pagesize = 12;
	cpu.RIimmSize = 12;
	cpu.cache_line_size = 6;
	cpu.code_align = 2;
	cpu.fileExt = ".r5a";
	cpu.sizeOfWord = 8;
	cpu.lea_op = op_la;
	cpu.SupportsBand = false;
	cpu.SupportsBor = false;
	cpu.SupportsBBC = true;
	cpu.SupportsBBS = true;
	cpu.SupportsPop = false;
	cpu.SupportsPush = false;
	cpu.SupportsLink = false;
	cpu.SupportsUnlink = false;
	cpu.SupportsBitfield = false;
	cpu.SupportsLDM = false;
	cpu.SupportsSTM = false;
	cpu.SupportsPtrdif = false;
	cpu.SupportsEnter = false;
	cpu.SupportsLeave = false;
	cpu.SupportsIndexed = true;
	cpu.SupportsTrinary = false;
	cpu.Addsi = false;
	cpu.ext_op = op_ext;
	cpu.extu_op = op_extu;
	cpu.mov_op = op_mv;
	cpu.ldi_op = op_loadi;
	cpu.ldbu_op = op_ldbu;
	cpu.ldb_op = op_ldb;
	cpu.ldo_op = op_ldo;
	cpu.ldtu_op = op_ldtu;
	cpu.ldt_op = op_ldt;
	cpu.ldwu_op = op_ldwu;
	cpu.ldw_op = op_ldw;
	cpu.ldd_op = op_ldd;
	cpu.stb_op = op_stb;
	cpu.sto_op = op_sto;
	cpu.stt_op = op_stt;
	cpu.stw_op = op_stw;
	cpu.std_op = op_std;
#endif
#ifdef BIGFOOT
	cpu.fileExt = ".bfa";
	cpu.lea_op = op_lda;
	cpu.pagesize = 14;
	cpu.RIimmSize = 24;
	cpu.cache_line_size = 6;
	cpu.code_align = 6;					// power of two
	cpu.SupportsBand = false;
	cpu.SupportsBor = false;
	cpu.SupportsBBC = false;
	cpu.SupportsBBS = false;
	cpu.SupportsPop = false;
	cpu.SupportsPush = false;
	cpu.SupportsLink = false;
	cpu.SupportsUnlink = false;
	cpu.SupportsBitfield = true;
	cpu.SupportsLDM = true;
	cpu.SupportsSTM = true;
	cpu.SupportsPtrdif = false;
	cpu.SupportsEnter = true;
	cpu.SupportsLeave = true;
	cpu.SupportsIndexed = true;
	cpu.SupportsTrinary = false;
	cpu.Addsi = false;
	cpu.ext_op = op_ext;
	cpu.extu_op = op_extu;
	cpu.mov_op = op_move;
	cpu.ldi_op = op_loadi;
	cpu.ldbu_op = op_ldbu;
	cpu.ldb_op = op_ldb;
	cpu.ldo_op = op_ldo;
	cpu.ldtu_op = op_ldtu;
	cpu.ldt_op = op_ldt;
	cpu.ldwu_op = op_ldwu;
	cpu.ldw_op = op_ldw;
	cpu.ldd_op = op_ldd;
	cpu.stb_op = op_stb;
	cpu.sto_op = op_sto;
	cpu.stt_op = op_stt;
	cpu.stw_op = op_stw;
	cpu.std_op = op_std;
#endif
#ifdef I386
	cpu.fileExt = ".asm";
	cpu.lea_op = op_lda;
	cpu.pagesize = 14;
	cpu.RIimmSize = 24;
	cpu.cache_line_size = 6;
	cpu.code_align = 6;					// power of two
	cpu.SupportsBand = false;
	cpu.SupportsBor = false;
	cpu.SupportsBBC = true;
	cpu.SupportsBBS = true;
	cpu.SupportsPop = false;
	cpu.SupportsPush = false;
	cpu.SupportsLink = false;
	cpu.SupportsUnlink = false;
	cpu.SupportsBitfield = false;
	cpu.SupportsLDM = false;
	cpu.SupportsSTM = false;
	cpu.SupportsPtrdif = false;
	cpu.SupportsEnter = false;
	cpu.SupportsLeave = false;
	cpu.SupportsIndexed = true;
	cpu.SupportsTrinary = false;
	cpu.Addsi = false;
	cpu.ext_op = op_ext;
	cpu.extu_op = op_extu;
	cpu.mov_op = op_move;
	cpu.ldi_op = op_loadi;
	cpu.ldbu_op = op_ldbu;
	cpu.ldb_op = op_ldb;
	cpu.ldo_op = op_ldo;
	cpu.ldtu_op = op_ldtu;
	cpu.ldt_op = op_ldt;
	cpu.ldwu_op = op_ldwu;
	cpu.ldw_op = op_ldw;
	cpu.ldd_op = op_ldd;
	cpu.stb_op = op_stb;
	cpu.sto_op = op_sto;
	cpu.stt_op = op_stt;
	cpu.stw_op = op_stw;
	cpu.std_op = op_std;
#endif
	cpu.InitRegs();

//	printf("c64 starting...\r\n");
	for (xx = 1; strchr("-/+", argv[xx][0]) && (xx < argc); xx++)
		options(argv[xx]);

	do {
		if (xx < argc) {
			if (PreProcessFile(argv[xx]) == -1)
				break;
			if (openfiles(argv[xx])) {
				lineno = 0;
				initsym();
	compiler.compile();
	AppendFiles();
	//				compile();
				summary();
//				MBlk::ReleaseAll();
//				ReleaseGlobalMemory();
				closefiles();
			}
    }
    dfs.printf((char *)"<CmdNext>Next on command line (%d).</CmdNext>\n", argc);
	} while (0);
	//getchar();
	dfs.printf("<Exit></Exit>\n");
	dfs.close();
 	exit(0);
	return (0);
}

int	options(char *s)
{
    int nn;
		int cnt;

		compiler.ipoll = false;
	if (s[1]=='o') {
        for (nn = 2; s[nn]; nn++) {
            switch(s[nn]) {
            case 'r':     opt_noregs = TRUE; break;
						case 'p':     ::opt_nopeep = TRUE; break;
            case 'x':     opt_noexpr = TRUE; break;
						case 'c':	  opt_nocgo = TRUE; break;
						case 's':		opt_size = TRUE; break;
						case 'l':	opt_loop_invariant = FALSE; break;
            }
        }
        if (nn==2) {
            opt_noregs = TRUE;
            ::opt_nopeep = TRUE;
            opt_noexpr = TRUE;
						opt_nocgo = TRUE;
            optimize = FALSE;
						opt_loop_invariant = FALSE;
        }
    }
	else if (s[1]=='f') {
		if (strcmp(&s[2],"no-exceptions")==0)
			compiler.exceptions = 0;
//		if (strcmp(&s[2],"farcode")==0)
//			farcode = 1;
		if (strncmp(&s[2], "poll",4) == 0) {
			compiler.ipoll = true;
			cnt = atoi(&s[6]);
			if (cnt > 0)
				compiler.pollCount = cnt;
		}
		if (strncmp(&s[2], "inline", 6) == 0) {
			compiler.autoInline = atoi(&s[8]);
		}
		if (strncmp(&s[2], "os_code", 7) == 0) {
			compiler.os_code = true;
		}
	}
	else if (s[1]=='a') {
		if (strncmp(&s[2], "ppend", 5) == 0)
			append = true;
		else
       address_bits = atoi(&s[2]);
    }
	else if (s[1]=='p') {
        if (strcmp(&s[2],"FISA64")==0) {
             gCpu = FISA64;
             regLR = 31;
             regSP = 30;
             regFP = 27;
             regGP = 26;
             use_gp = TRUE;
        }
				if (strcmp(&s[2], "riscv64") == 0) {
					cpu.sizeOfWord = 8;
					gCpu = 5;
					regLR = 1;
					regSP = 2;
					regFP = 8;
					regGP = 3;
					regZero = 0;
					compiler.nogcskips = true;
					cpu.fileExt = ".r5a";
					cpu.SupportsBand = false;
					cpu.SupportsBor = false;
					cpu.SupportsBBC = false;
					cpu.SupportsBBS = false;
					cpu.SupportsPop = false;
					cpu.SupportsPush = false;
					cpu.SupportsLink = false;
					cpu.SupportsUnlink = false;
					cpu.SupportsBitfield = false;
					cpu.SupportsLDM = false;
					cpu.SupportsSTM = false;
					cpu.SupportsPtrdif = false;
					cpu.SupportsEnter = false;
					cpu.SupportsLeave = false;
					cpu.SupportsIndexed = false;
					cpu.Addsi = true;
					cpu.mov_op = op_mv;
					cpu.lea_op = op_la;
					cpu.ldi_op = op_l;
					cpu.ldbu_op = op_lbu;
					cpu.ldb_op = op_lb;
					cpu.ldo_op = op_ld;
					cpu.ldtu_op = op_lwu;
					cpu.ldt_op = op_lw;
					cpu.ldwu_op = op_lhu;
					cpu.ldw_op = op_lh;
					cpu.stb_op = op_sb;
					cpu.sto_op = op_sd;
					cpu.stt_op = op_sw;
					cpu.stw_op = op_sh;
				}
				else if (strcmp(&s[2], "riscv32") == 0) {
					cpu.sizeOfWord = 4;
				}
	}
	else if (s[1]=='w')
		wcharSupport = 0;
	else if (s[1]=='v') {
         if (s[2]=='0')
             verbose = 0;
         else
             verbose = 1;
    }
	else if (s[1] == 'r') {
		if (s[2] == 'v') {
			opt_vreg = TRUE;
			cpu.SetVirtualRegisters();
			nregs = 1024;
		}
		else
		{
			cpu.SetRealRegisters();
		}
	}
  else if (s[1]=='S')
    mixedSource = TRUE;
	else if (s[0] == '+' && s[1] == 'b') {
		printf("ARPL v1.03 64-bit Compiler (C) 2024 Robert Finch\n");
		cg.banner();
	}
	return 0;
}

int PreProcessFile(char *nm)
{
	static char outname[1000];
	static char sysbuf[500];

	strcpy_s(outname, sizeof(outname), nm);
	makename_s(outname,sizeof(outname),(char *)".fpp");
//	snprintf(sysbuf, sizeof(sysbuf), "fpp /d /V -b %s %s", nm, outname);
	snprintf(sysbuf, sizeof(sysbuf), "fpp64 -b %s %s", nm, outname);
	_flushall();
	return system(sysbuf);
}

int openfiles(char *s)
{
	char *p;
	strcpy_s(irfile, sizeof(irfile), s);
	strcpy_s(infile,sizeof(infile),s);
        strcpy_s(listfile,sizeof(listfile),s);
				if (append)
					strcpy_s(outfile, sizeof(outfile), "arpl.asm");
				else
	        strcpy_s(outfile,sizeof(outfile),s);
  dbgfile = s;

		//strcpy(outfileG,s);
		_splitpath_s(
			s,
			(char *)NULL,
			(size_t)0,
			(char *)NULL,
			(size_t)0,
			nmspace[0],
			(size_t)sizeof(nmspace[0]),
			(char *)NULL,
			(size_t)0
			);
//		strcpy(nmspace[0],basename(s));
		p = strrchr(nmspace[0],'.');
		if (p)
			*p = '\0';
		makename_s(irfile, sizeof(irfile), (char *)".hir");
		makename_s(infile, sizeof(infile), (char *)".fpp");
        makename_s(listfile, sizeof(listfile), (char *)".lis");
        makename_s(outfile, sizeof(outfile), (char *)cpu.fileExt.c_str());
    dbgfile += ".xml";
		ifs = new std::ifstream();
		ifs->open(infile,std::ios::in);
/*
        if( (input = fopen(infile,"r")) == 0) {
				i = errno;
                printf(" cant open %s\n",infile);
                return 0;
                }
*/
/*
        ofl = creat(outfile,-1);
        if( ofl < 0 )
                {
                printf(" cant create %s\n",outfile);
                fclose(input);
                return 0;
                }
*/
        //oflg = _creat(outfileG,-1);
        //if( oflg < 0 )
        //        {
        //        printf(" cant create %s\n",outfileG);
        //        fclose(input);
        //        return 0;
        //        }
		if (append) {
			ofs.open(outfile, std::ios::out | std::ios::app);
		}
		else {
			ofs.open(outfile, std::ios::out | std::ios::trunc);
			ofs.name = outfile;
		}
		dfs.open(dbgfile.c_str(), std::ios::out | std::ios::trunc);
		//		irfs.open(irfile, std::ios::out | std::ios::trunc);
		irfs.level = 1;
		irfs.puts("ARPL Hex Intermediate File\n");
		dfs.level = 1;
		dfs.puts("<title>ARPL Compiler debug file</title>\n");
		dfs.level = 1;
		lfs.level = 1;
		ofs.level = 1;
/*
        if( (output = fdopen(ofl,"w")) == 0) {
                printf(" cant open %s\n",outfile);
                fclose(input);
                return 0;
                }
*/
        //if( (outputG = _fdopen(oflg,"w")) == 0) {
        //        printf(" cant open %s\n",outfileG);
        //        fclose(input);
        //        fclose(output);
        //        return 0;
        //        }
        try {
					lfs.open(listfile,std::ios::out|std::ios::trunc);
		}
		catch(...) {
			closefiles();
			return 0;
		}
/*
        if( (list = fopen(listfile,"w")) == 0) {
                printf(" cant open %s\n",listfile);
                fclose(input);
                fclose(output);
                //fclose(outputG);
                return 0;
                }
*/
        return 1;
}

void makename_s(char *s, size_t L, char *e)
{
	size_t n;

	n = strlen(s);
	while(s[n]!='.' && n >= 0) n--;
	if (n > 0)
		strcpy_s(&s[n],L-n-1,e);
	//while(*s != 0 && *s != '.')
 //       ++s;
 //   while(*s++ = *e++);
}

void summary()
{
//    if (verbose > 0)
  dfs.printf("<summary>\n");
    	printf("\n -- %d errors found.\n",total_errors);
    lfs.write((char *)"\f\n *** global scope typedef symbol table ***\n\n");
    ListTable(&gsyms[0],0);
    lfs.write((char *)"\n *** structures and unions ***\n\n");
    ListTable(&tagtable,0);
  dfs.printf("</summary>\n");
//	fflush(list);
}

void closefiles()
{ 
	dfs.printf("<closefiles>\n");
	ifs->close();
	delete ifs;
//	irfs.flush();
//	irfs.close();
	dfs.printf("A");
	lfs.close();
	dfs.printf("B");
	ofs.close();
	dfs.printf("C");
	dfs.printf("</closefiles>\n");
}

char *GetNamespace()
{
	return ((char *)currentFn->sym->name->c_str());
//	return nmspace[0];
//	return nmspace[incldepth];
}

char* GetPrivateNamespace()
{
	return nmspace[0];
	//	return nmspace[incldepth];
}
