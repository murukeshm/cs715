/*-----------------------------------------------------------------------------
 *  "gcc-plugin.h" must be the FIRST file to be included 
 *-----------------------------------------------------------------------------*/
#include "gcc-plugin.h"

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
//#include "tm_p.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "toplev.h"
#include "gimple-pretty-print.h"
#include "cgraph.h"
#include "coretypes.h"
#include "tree.h"
#include "diagnostic.h"
#include "timevar.h"
#include "alloc-pool.h"
#include "params.h"
#include "ggc.h"
#include "vec.h"

/*-----------------------------------------------------------------------------
 *  Each plugin MUST define this global int to assert compatibility with GPL; 
 *  else the compiler throws a fatal error 
 *-----------------------------------------------------------------------------*/
int plugin_is_GPL_compatible;
static unsigned int inter_gimple_manipulation (void);
static int process_function(void);
/*-----------------------------------------------------------------------------
 *  Structure of the pass we want to insert, identical to a regular ipa pass
 *-----------------------------------------------------------------------------*/
struct simple_ipa_opt_pass pass_plugin = {
{
    SIMPLE_IPA_PASS,
    "total_stmt_count",               /*  name */
    NULL,                            /*  gate */
    inter_gimple_manipulation,	  /* execute */
    NULL,                         /*  sub */
    NULL,                         /*  next */
    0,                            /*  static pass number */
    TV_INTEGRATION,               /*  tv_id */
    0,                            /*  properties required */
    0,                            /*  properties provided */
    0,                            /*  properties destroyed */
    0,                            /*  todo_flags start */
    0                             /*  todo_flags end */
  }
};



/*-----------------------------------------------------------------------------
 *  This structure provides the information about inserting the pass in the
 *  pass manager. 
 *-----------------------------------------------------------------------------*/
struct register_pass_info pass_info = {
  &(pass_plugin.pass),            /* Address of new pass, here, the 'struct
                                     opt_pass' field of 'simple_ipa_opt_pass'
                                     defined above */
  "pta",                          /* Name of the reference pass for hooking up
                                     the new pass.  */
  0,                              /* Insert the pass at the specified instance
                                     number of the reference pass. Do it for
                                     every instance if it is 0. */
  PASS_POS_INSERT_BEFORE           /* how to insert the new pass: before,
                                     after, or replace. Here we are inserting
                                     a pass names 'plug' after the pass named
                                     'pta' */
};




/*-----------------------------------------------------------------------------
 *  plugin_init is the first function to be called after the plugin is loaded
 *-----------------------------------------------------------------------------*/
int plugin_init(struct plugin_name_args *plugin_info,struct plugin_gcc_version *version)
{

  /*-----------------------------------------------------------------------------
   * Plugins are activiated using this callback 
   *-----------------------------------------------------------------------------*/
   register_callback (
      plugin_info->base_name,     /* char *name: Plugin name, could be any
                                     name. plugin_info->base_name gives this
                                     filename */
      PLUGIN_PASS_MANAGER_SETUP,  /* int event: The event code. Here, setting
                                     up a new pass */
      NULL,                       /* The function that handles event */
      &pass_info);                /* plugin specific data */

  return 0;
}

static unsigned int inter_gimple_manipulation (void)
{
	struct cgraph_node *node;
	for (node = cgraph_nodes; node; node = node->next)
	{
		if (!gimple_has_body_p (node->decl) || node->clone_of)
			continue;
		push_cfun (DECL_STRUCT_FUNCTION (node->decl));
		process_function();
		pop_cfun();
	}
		
	return 0;
}		

int process_function (void)
{
	cfun
	int i = 0;
	basic_block bb;
	gimple_stmt_iterator si;
	FOR_ALL_BB (bb) 
	{
		for (si = gsi_start_bb (bb); !gsi_end_p (si); gsi_next(&si))
		{
			gimple stmt = gsi_stmt (si);
			if (CALL_EXPR == gimple_expr_code(stmt))
			{
				fprintf(dump_file, "%d\t", i);
				tree fn (gimple_call_fndecl(stmt));
				if (!fn)
					fprintf(stderr, "NULL.\n");
				else if (gimple_has_body_p (fn))
				{
					push_cfun(DECL_STRUCT_FUNCTION (fn));
					i += process_function();
				}
				else
					i++;
			}
			else
			{
				i++;
				fprintf(dump_file, "%d\t", i);
				print_gimple_stmt (dump_file, stmt, 0, TDF_SLIM);		
			}
		}
	}
	return i;
}
/*
	basic_block bb;
	FOR_ALL_BB(bb)
	{
		gimple_stmt_iterator si;
		for (si = gsi_start_bb (bb); !gsi_end_p (si); gsi_next (&si))
		{
			gimple stmt = gsi_stmt (si);
			if (CALL_EXPR == gimple_expr_code(stmt))
			{
				i++;
				tree fn = gimple_call_fn(stmt);
				if (fn == NULL)
					fprintf(stderr, "NULL.\n");
			}
			else
			{
				fprintf(dump_file, "%d\t", i++);
				print_gimple_stmt (dump_file, stmt, 0, TDF_SLIM);		
			}
		}
	}
*/	
