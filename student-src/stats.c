#include "paging.h"
#include "stats.h"

/* The stats. See the definition in stats.h. */
stats_t stats;

/*  --------------------------------- PROBLEM 9 --------------------------------------
    Calculate any remaining statistics to print out.

    You will need to include code to increment many of these stats in
    the functions you have written for other parts of the project.

    Use this function to calculate any remaining stats, such as the
    average access time (AAT).

    You may find the #defines in the stats.h file useful.
    -----------------------------------------------------------------------------------
*/
void compute_stats() {
    stats.accesses = stats.writes + stats.reads;
    double mem_read_time = stats.accesses * MEMORY_READ_TIME;
    double disk_read_time = stats.page_faults * DISK_PAGE_READ_TIME;
    double disk_writeback_time = stats.writebacks * DISK_PAGE_WRITE_TIME;
    stats.aat = (mem_read_time + disk_read_time + disk_writeback_time) / stats.accesses;
}
