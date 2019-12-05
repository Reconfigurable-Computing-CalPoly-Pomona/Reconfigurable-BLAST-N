/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                         */
/*  \   \        Copyright (c) 2003-2013 Xilinx, Inc.                 */
/*  /   /        All Right Reserved.                                  */
/* /---/   /\                                                         */
/* \   \  /  \                                                        */
/*  \___\/\___\                                                       */
/**********************************************************************/


#include "iki.h"
#include <string.h>
#include <math.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                         */
/*  \   \        Copyright (c) 2003-2013 Xilinx, Inc.                 */
/*  /   /        All Right Reserved.                                  */
/* /---/   /\                                                         */
/* \   \  /  \                                                        */
/*  \___\/\___\                                                       */
/**********************************************************************/


#include "iki.h"
#include <string.h>
#include <math.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
typedef void (*funcp)(char *, char *);
extern void execute_2(char*, char *);
extern void execute_3(char*, char *);
extern void execute_4(char*, char *);
extern void execute_5(char*, char *);
extern void execute_6(char*, char *);
extern void execute_7(char*, char *);
extern void execute_8(char*, char *);
extern void execute_9(char*, char *);
extern void execute_35(char*, char *);
extern void execute_122(char*, char *);
extern void execute_37(char*, char *);
extern void execute_38(char*, char *);
extern void execute_39(char*, char *);
extern void execute_40(char*, char *);
extern void execute_41(char*, char *);
extern void execute_42(char*, char *);
extern void execute_130(char*, char *);
extern void execute_125(char*, char *);
extern void execute_127(char*, char *);
extern void execute_128(char*, char *);
extern void execute_129(char*, char *);
extern void vlog_transfunc_eventcallback(char*, char*, unsigned, unsigned, unsigned, char *);
extern void transaction_32(char*, char*, unsigned, unsigned, unsigned);
extern void vhdl_transfunc_eventcallback(char*, char*, unsigned, unsigned, unsigned, char *);
extern void transaction_37(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_38(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_39(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_44(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_52(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_53(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_57(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_58(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_59(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_60(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_61(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_62(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_63(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_64(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_65(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_71(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_72(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_73(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_74(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_75(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_76(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_77(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_78(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_79(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_85(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_86(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_87(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_88(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_89(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_90(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_91(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_92(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_93(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_99(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_100(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_101(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_102(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_103(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_104(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_105(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_106(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_107(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_113(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_114(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_115(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_116(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_117(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_118(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_119(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_120(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_121(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_127(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_128(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_129(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_130(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_131(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_132(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_133(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_134(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_135(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_141(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_142(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_143(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_144(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_145(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_146(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_147(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_148(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_149(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_155(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_156(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_157(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_158(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_159(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_160(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_161(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_162(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_163(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_169(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_170(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_171(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_172(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_173(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_174(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_175(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_176(char*, char*, unsigned, unsigned, unsigned);
extern void transaction_177(char*, char*, unsigned, unsigned, unsigned);
funcp funcTab[111] = {(funcp)execute_2, (funcp)execute_3, (funcp)execute_4, (funcp)execute_5, (funcp)execute_6, (funcp)execute_7, (funcp)execute_8, (funcp)execute_9, (funcp)execute_35, (funcp)execute_122, (funcp)execute_37, (funcp)execute_38, (funcp)execute_39, (funcp)execute_40, (funcp)execute_41, (funcp)execute_42, (funcp)execute_130, (funcp)execute_125, (funcp)execute_127, (funcp)execute_128, (funcp)execute_129, (funcp)vlog_transfunc_eventcallback, (funcp)transaction_32, (funcp)vhdl_transfunc_eventcallback, (funcp)transaction_37, (funcp)transaction_38, (funcp)transaction_39, (funcp)transaction_44, (funcp)transaction_52, (funcp)transaction_53, (funcp)transaction_57, (funcp)transaction_58, (funcp)transaction_59, (funcp)transaction_60, (funcp)transaction_61, (funcp)transaction_62, (funcp)transaction_63, (funcp)transaction_64, (funcp)transaction_65, (funcp)transaction_71, (funcp)transaction_72, (funcp)transaction_73, (funcp)transaction_74, (funcp)transaction_75, (funcp)transaction_76, (funcp)transaction_77, (funcp)transaction_78, (funcp)transaction_79, (funcp)transaction_85, (funcp)transaction_86, (funcp)transaction_87, (funcp)transaction_88, (funcp)transaction_89, (funcp)transaction_90, (funcp)transaction_91, (funcp)transaction_92, (funcp)transaction_93, (funcp)transaction_99, (funcp)transaction_100, (funcp)transaction_101, (funcp)transaction_102, (funcp)transaction_103, (funcp)transaction_104, (funcp)transaction_105, (funcp)transaction_106, (funcp)transaction_107, (funcp)transaction_113, (funcp)transaction_114, (funcp)transaction_115, (funcp)transaction_116, (funcp)transaction_117, (funcp)transaction_118, (funcp)transaction_119, (funcp)transaction_120, (funcp)transaction_121, (funcp)transaction_127, (funcp)transaction_128, (funcp)transaction_129, (funcp)transaction_130, (funcp)transaction_131, (funcp)transaction_132, (funcp)transaction_133, (funcp)transaction_134, (funcp)transaction_135, (funcp)transaction_141, (funcp)transaction_142, (funcp)transaction_143, (funcp)transaction_144, (funcp)transaction_145, (funcp)transaction_146, (funcp)transaction_147, (funcp)transaction_148, (funcp)transaction_149, (funcp)transaction_155, (funcp)transaction_156, (funcp)transaction_157, (funcp)transaction_158, (funcp)transaction_159, (funcp)transaction_160, (funcp)transaction_161, (funcp)transaction_162, (funcp)transaction_163, (funcp)transaction_169, (funcp)transaction_170, (funcp)transaction_171, (funcp)transaction_172, (funcp)transaction_173, (funcp)transaction_174, (funcp)transaction_175, (funcp)transaction_176, (funcp)transaction_177};
const int NumRelocateId= 111;

void relocate(char *dp)
{
	iki_relocate(dp, "xsim.dir/ScoreMatrixTb_behav/xsim.reloc",  (void **)funcTab, 111);
	iki_vhdl_file_variable_register(dp + 38728);
	iki_vhdl_file_variable_register(dp + 38784);


	/*Populate the transaction function pointer field in the whole net structure */
}

void sensitize(char *dp)
{
	iki_sensitize(dp, "xsim.dir/ScoreMatrixTb_behav/xsim.reloc");
}

void simulate(char *dp)
{
		iki_schedule_processes_at_time_zero(dp, "xsim.dir/ScoreMatrixTb_behav/xsim.reloc");
	// Initialize Verilog nets in mixed simulation, for the cases when the value at time 0 should be propagated from the mixed language Vhdl net

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 49096, 0, 1, 4, 5, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 49152, 0, 2, 6, 8, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 49208, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 49264, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 49320, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 49376, 0, 1, 6, 7, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 49432, 0, 1, 4, 5, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 49488, 0, 1, 14, 15, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 52456, 0, 1, 4, 5, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 52512, 0, 2, 3, 5, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 52568, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 52624, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 52680, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 52736, 0, 1, 4, 5, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 52792, 0, 1, 2, 3, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 52848, 0, 1, 12, 13, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 55816, 0, 1, 4, 5, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 55872, 0, 2, 0, 2, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 55928, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 55984, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 56040, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 56096, 0, 1, 2, 3, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 56152, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 56208, 0, 1, 10, 11, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 59176, 0, 1, 2, 3, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 59232, 0, 2, 6, 8, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 59288, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 59344, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 59400, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 59456, 0, 1, 14, 15, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 59512, 0, 1, 12, 13, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 59568, 0, 1, 22, 23, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 62536, 0, 1, 2, 3, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 62592, 0, 2, 3, 5, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 62648, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 62704, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 62760, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 62816, 0, 1, 12, 13, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 62872, 0, 1, 10, 11, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 62928, 0, 1, 20, 21, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 65896, 0, 1, 2, 3, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 65952, 0, 2, 0, 2, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 66008, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 66064, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 66120, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 66176, 0, 1, 10, 11, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 66232, 0, 1, 8, 9, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 66288, 0, 1, 18, 19, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 69256, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 69312, 0, 2, 6, 8, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 69368, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 69424, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 69480, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 69536, 0, 1, 22, 23, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 69592, 0, 1, 20, 21, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 69648, 0, 1, 30, 31, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 72616, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 72672, 0, 2, 3, 5, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 72728, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 72784, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 72840, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 72896, 0, 1, 20, 21, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 72952, 0, 1, 18, 19, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 73008, 0, 1, 28, 29, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46824, dp + 75976, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46784, dp + 76032, 0, 2, 0, 2, 3, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44352, dp + 76088, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44392, dp + 76144, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 44432, dp + 76200, 0, 1, 0, 1, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 76256, 0, 1, 18, 19, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 76312, 0, 1, 16, 17, 2, 1);

	iki_vlog_schedule_transaction_signal_fast_vhdl_value_time_0(dp + 46352, dp + 76368, 0, 1, 26, 27, 2, 1);
	iki_execute_processes();

	// Schedule resolution functions for the multiply driven Verilog nets that have strength
	// Schedule transaction functions for the singly driven Verilog nets that have strength

}
#include "iki_bridge.h"
void relocate(char *);

void sensitize(char *);

void simulate(char *);

extern SYSTEMCLIB_IMP_DLLSPEC void local_register_implicit_channel(int, char*);
extern void implicit_HDL_SCinstatiate();

extern SYSTEMCLIB_IMP_DLLSPEC int xsim_argc_copy ;
extern SYSTEMCLIB_IMP_DLLSPEC char** xsim_argv_copy ;

int main(int argc, char **argv)
{
    iki_heap_initialize("ms", "isimmm", 0, 2147483648) ;
    iki_set_sv_type_file_path_name("xsim.dir/ScoreMatrixTb_behav/xsim.svtype");
    iki_set_crvs_dump_file_path_name("xsim.dir/ScoreMatrixTb_behav/xsim.crvsdump");
    void* design_handle = iki_create_design("xsim.dir/ScoreMatrixTb_behav/xsim.mem", (void *)relocate, (void *)sensitize, (void *)simulate, 0, isimBridge_getWdbWriter(), 0, argc, argv);
     iki_set_rc_trial_count(100);
    (void) design_handle;
    return iki_simulate_design();
}
