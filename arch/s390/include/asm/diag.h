/*
 * s390 diagnose functions
 *
 * Copyright IBM Corp. 2007
 * Author(s): Michael Holzheu <holzheu@de.ibm.com>
 */

#ifndef _ASM_S390_DIAG_H
#define _ASM_S390_DIAG_H

#include <linux/if_ether.h>

/*
 * Diagnose 10: Release page range
 */
static inline void diag10_range(unsigned long start_pfn, unsigned long num_pfn)
{
	unsigned long start_addr, end_addr;

	start_addr = start_pfn << PAGE_SHIFT;
	end_addr = (start_pfn + num_pfn - 1) << PAGE_SHIFT;

	asm volatile(
		"0:	diag	%0,%1,0x10\n"
		"1:\n"
		EX_TABLE(0b, 1b)
		EX_TABLE(1b, 1b)
		: : "a" (start_addr), "a" (end_addr));
}

/*
 * Diagnose 14: Input spool file manipulation
 */
extern int diag14(unsigned long rx, unsigned long ry1, unsigned long subcode);

/*
 * Diagnose 210: Get information about a virtual device
 */
struct diag210 {
	u16 vrdcdvno;	/* device number (input) */
	u16 vrdclen;	/* data block length (input) */
	u8 vrdcvcla;	/* virtual device class (output) */
	u8 vrdcvtyp;	/* virtual device type (output) */
	u8 vrdcvsta;	/* virtual device status (output) */
	u8 vrdcvfla;	/* virtual device flags (output) */
	u8 vrdcrccl;	/* real device class (output) */
	u8 vrdccrty;	/* real device type (output) */
	u8 vrdccrmd;	/* real device model (output) */
	u8 vrdccrft;	/* real device feature (output) */
} __attribute__((packed, aligned(4)));

extern int diag210(struct diag210 *addr);

enum diag26c_sc {
	DIAG26C_MAC_SERVICES = 0x00000030
};

enum diag26c_version {
	DIAG26C_VERSION2 = 0x00000002	/* z/VM 5.4.0 */
};

#define DIAG26C_GET_MAC	0x0000
struct diag26c_mac_req {
	u32	resp_buf_len;
	u32	resp_version;
	u16	op_code;
	u16	devno;
	u8	res[4];
};

struct diag26c_mac_resp {
	u32	version;
	u8	mac[ETH_ALEN];
	u8	res[2];
} __aligned(8);

int diag26c(void *req, void *resp, enum diag26c_sc subcode);

#endif /* _ASM_S390_DIAG_H */
