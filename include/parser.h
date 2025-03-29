#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>  // FILE

#include "process.h"

// * Sets the time quantum (a global variable).
// 'data_fn' -- 'data filename'
// 'prc_lst' -- 'process list'
// 'nprc' -- 'process count'
void parse_prcs(char *data_fn, Process prc_lst[], int *nprc, int max_prc);

#endif