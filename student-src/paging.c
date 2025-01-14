#include "paging.h"
#include "page_splitting.h"
#include "swapops.h"
#include "stats.h"

 /* The frame table pointer. You will set this up in system_init. */
fte_t *frame_table;

/* ----------------------------------------------------------------------------------
    Initialize the frame table.

    The frame table will be located at physical address 0 in our simulated
    memory. You will first assign the frame_table global variable to point to
    this location in memory. You should zero out the frame table, in case for
    any reason physical memory is not clean.

    You should then mark the first entry in the frame table as protected. We do
    this because we do not want our free frame allocator to give out the frame
    used by the frame table.
    -----------------------------------------------------------------------------------
*/
void system_init(void) {
    /*
     * 1. Set the frame table pointer to point to the first frame in physical
     * memory. Zero out the memory used by the frame table.
     *
     * Address "0" in memory will be used for the frame table. This table will
     * contain n frame table entries (fte_t), where n is the number of
     * frames in memory. The frame table will be useful later if we need to
     * evict pages during page faults.
     */

    frame_table = (fte_t*) &mem[0];
    memset(mem, 0, PAGE_SIZE);

    /*
     * 2. Mark the first frame table entry as protected.
     *
     * The frame table contains entries for all of physical memory,
     * however, there are some frames we never want to evict.
     * We mark these special pages as "protected" to indicate this.
     */

    frame_table[0].protected = 1;

}

/*  ----------------------------------------------------------------------------------
    This function gets called every time a new process is created.
    You will need to allocate a new page table for the process in memory using the
    free_frame function so that the process can store its page mappings. Then, you
    will need to store the PFN of this page table in the process's PCB.
    -----------------------------------------------------------------------------------
*/
void proc_init(pcb_t *proc) {
    /*
     * 1. Call the free frame allocator (free_frame) to return a free frame for
     * this process's page table. You should zero-out the memory.
     */

    pfn_t base_register = free_frame();
    memset(mem + (base_register * PAGE_SIZE), 0, PAGE_SIZE);


    /*
     * 2. Update the process's PCB with the frame number
     * of the newly allocated page table.
     *
     * Additionally, mark the frame's frame table entry as protected. You do not
     * want your page table to be accidentally evicted.
     */

    proc -> saved_ptbr = base_register;
    frame_table[base_register].protected = 1;

}

/*  ----------------------------------------------------------------------------------
    Swaps the currently running process on the CPU to another process.

    Every process has its own page table, as you allocated in proc_init. You will
    need to tell the processor to use the new process's page table.
    -----------------------------------------------------------------------------------
 */
void context_switch(pcb_t *proc) {
    PTBR = proc -> saved_ptbr;
}

/*  ----------------------------------------------------------------------------------
    Takes an input virtual address and returns the data from the corresponding
    physical memory address. The steps to do this are:

    1) Translate the virtual address into a physical address using the page table.
    2) Go into the memory and read/write the data at the translated address.

    Parameters:
        1) address     - The virtual address to be translated.
        2) rw          - 'r' if the access is a read, 'w' if a write
        3) data        - One byte of data to write to our memory, if the access is a write.
                         This byte will be NULL on read accesses.

    Return:
        The data at the address if the access is a read, or
        the data we just wrote if the access is a write.
    -----------------------------------------------------------------------------------
 */
uint8_t mem_access(vaddr_t address, char rw, uint8_t data) {


    /* Split the address and find the page table entry */
    uint16_t offset = vaddr_offset(address);
    vpn_t vpn = vaddr_vpn(address);                     
    pte_t* entry = (pte_t*) (mem + PTBR * PAGE_SIZE);   
    entry += vpn;

    /* If an entry is invalid, just page fault to allocate a page for the page table. */
    if (!entry -> valid) {
        page_fault(address);
    }

    /* Set the "referenced" bit to reduce the page's likelihood of eviction */
    frame_table[PTBR].referenced = 1;

    /*
        The physical address will be constructed like this:
        -------------------------------------
        |     PFN    |      Offset          |
        -------------------------------------
        where PFN is the value stored in the page table entry.
        We need to calculate the number of bits are in the offset.

        Create the physical address using your offset and the page
        table entry.
    */

    uint8_t* physical_address = (u_int8_t*) (mem + entry -> pfn * PAGE_SIZE + offset);

    /* Either read or write the data to the physical address
       depending on 'rw' */
    if (rw == 'r') {
        stats.reads++;
        return *physical_address;
    } else {
        stats.writes++;
        *physical_address = data;
        entry -> dirty = 1;
        return data;
    }
}

/*  ----------------------------------------------------------------------------------
    When a process exits, you need to free any pages previously occupied by the
    process. Otherwise, every time you closed and re-opened Microsoft Word, it
    would gradually eat more and more of your computer's usable memory.

    To free a process, you must clear the "mapped" bit on every page the process
    has mapped. If the process has swapped any pages to disk, you must call
    swap_free() using the page table entry pointer as a parameter.

    You must also clear the "protected" bits for the page table itself.
    -----------------------------------------------------------------------------------
*/
void proc_cleanup(pcb_t *proc) {
    /* Look up the process's page table */
    pte_t* entry = (pte_t*) (mem + proc->saved_ptbr * PAGE_SIZE);

    /* Iterate the page table and clean up each valid page */
    pte_t* ptr = entry;
    for (size_t i = 0; i < NUM_PAGES; i++) {
        if (ptr -> swap) swap_free(ptr);                      
        if (ptr -> valid) frame_table[ptr->pfn].mapped = 0;   
        ptr++; 
    }

    /* Free the page table itself in the frame table */
    frame_table[proc->saved_ptbr].protected = 0;

}
