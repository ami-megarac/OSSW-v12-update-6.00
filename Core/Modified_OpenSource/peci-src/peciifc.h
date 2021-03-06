/*
 ****************************************************************
 **                                                            **
 **    (C)Copyright 2009-2015, American Megatrends Inc.        **
 **                                                            **
 **            All Rights Reserved.                            **
 **                                                            **
 **        5555 Oakbrook Pkwy Suite 200, Norcross,             **
 **                                                            **
 **        Georgia - 30093, USA. Phone-(770)-246-8600.         **
 **                                                            **
 ****************************************************************
 */

#ifndef __PECI_IFC_H__
#define __PECI_IFC_H__

/* Device Properties */
#define PECI_MAJOR		45
#define PECI_MINOR		0
#define PECI_MAX_DEVICES 	255
#define PECI_DEV_NAME		"PECI"

#define BYTE	1
#define WORD	2
#define DWORD	4
#define QWORD	8
#define DQWORD	16

/* IOCTL Command and structure */
#define PECI_ISSUE_CMD		_IOR('p', 0, int)
#define PECI_ISSUE_CMD_WITH_RETRIES	_IOR('p', 1, int)

#define PECI_DATA_BUF_SIZE	32 

typedef struct 
{
    unsigned char target;
    unsigned char dev_id;
    unsigned char domain;
    unsigned char write_len;
    unsigned char write_buffer[PECI_DATA_BUF_SIZE];
    unsigned char read_len;
    unsigned char read_buffer[PECI_DATA_BUF_SIZE];
    unsigned char AWFCS;
    unsigned char Xmit_Feedback;
    int status;
}__attribute__((packed)) peci_cmd_t;

union peci_data
{
    unsigned char peci_byte;         /*!< Byte data */
    unsigned short peci_word;        /*!< Word data */
    unsigned long peci_dword;        /*!< 4byte data */
    unsigned char peci_qword[8];     /*!< 8byte data */
    unsigned char peci_dqword[16];   /*!< 16byte data */
};

typedef struct
{
    unsigned char device_info;  /*!<Device information */
    unsigned char revision_no;  /*!<Revison Number supported by PECI */
    unsigned char reserved[6];  /*!<Reserved bytes */
}__attribute__((packed)) peci_getdib_res_t;

typedef struct
{
    unsigned char option;       /*!<Read option is performed for BYTE/WORD/DWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char index;        /*!<The encoding for the requested service */
    unsigned short parameter;   /*!<Specifies the parameter for data being requested */
}__attribute__((packed)) peci_rdpkgconfig_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
    union peci_data data;           /*!< Response data   */
}__attribute__((packed)) peci_rdpkgconfig_res_t;

typedef struct
{
    unsigned char option;       /*!<Write option is performed for BYTE/WORD/DWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char index;        /*!<The encoding for the requested service */
    unsigned short parameter;   /*!<Specifies the parameter for data being requested */
    union peci_data data;       /*!<Data to be written on the Index. */
}__attribute__((packed)) peci_wrpkgconfig_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
}__attribute__((packed)) peci_wrpkgconfig_res_t;

typedef struct
{
    unsigned char option;       /*!<Read option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char processor_id; /*!<IA MSR Space refers to a specific logical processor in CPU */
    unsigned short msr_addr;    /*!<Address of the MSR */
}__attribute__((packed)) peci_rdiamsr_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
    union peci_data data;            /*!< Response data */
}__attribute__((packed)) peci_rdiamsr_res_t;

typedef struct
{
    unsigned char option;       /*!<Read option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned short processor_id; /*!<IA MSR Space refers to a specific logical processor in CPU */
    unsigned short msr_addr;    /*!<Address of the MSR */
}__attribute__((packed)) peci_rdiamsrex_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
    union peci_data data;            /*!< Response data */
}__attribute__((packed)) peci_rdiamsrex_res_t;

typedef struct
{
    unsigned char option;       /*!<Write option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char processor_id; /*!<IA MSR Space refers to a specific logical processor in CPU */
    unsigned short msr_addr;    /*!<Address of the MSR */
    union peci_data data;	/*!<Data to be written on the Index. */
}__attribute__((packed)) peci_wriamsr_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
}__attribute__((packed)) peci_wriamsr_res_t;

typedef struct
{
    unsigned char option;       /*!<Read option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned long pci_config_addr;    /*!<Address of the MSR */
}__attribute__((packed)) peci_rdpciconfig_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
    union peci_data data;            /*!< Response data */
}__attribute__((packed)) peci_rdpciconfig_res_t;

typedef struct
{
    unsigned char option;       /*!<Write option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned long pci_config_addr;    /*!<Address of the MSR */
    union peci_data data;	/*!<Data to be written on the Index. */
}__attribute__((packed)) peci_wrpciconfig_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
}__attribute__((packed)) peci_wrpciconfig_res_t;

typedef struct
{
    unsigned char option;       /*!<Read option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char opcode;    
    unsigned char subopcode;    
    unsigned char param0;
    unsigned short param1;
    unsigned char param2;
}__attribute__((packed)) peci_crashdump_disc_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
    union peci_data data;            /*!< Response data */
}__attribute__((packed)) peci_crashdump_disc_res_t;

typedef struct
{
    unsigned char option;       /*!<Read option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char opcode;    
    unsigned short param0;
    unsigned short param1;
    unsigned short param2;
}__attribute__((packed)) peci_crashdump_getframe_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
    union peci_data data;            /*!< Response data */
}__attribute__((packed)) peci_crashdump_getframe_res_t;

typedef struct
{
    unsigned char option;       /*!<Read option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char message_type;
    unsigned char segment;
    unsigned int pci_config_addr;    /*!<Address of the MSR */
}__attribute__((packed)) peci_rdendpointconfig_pci_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
    union peci_data data;            /*!< Response data */
}__attribute__((packed)) peci_rdendpointconfig_pci_res_t;

typedef struct
{
    unsigned char option;       /*!<Read option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char message_type;
    unsigned char bar;
    unsigned char address_type;
    unsigned char segment;
    unsigned char devicefun;
    unsigned char bus;
    unsigned long long pci_config_addr;    /*!<Address of the MSR */
}__attribute__((packed)) peci_rdendpointconfig_mmio_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
    union peci_data data;            /*!< Response data */
}__attribute__((packed)) peci_rdendpointconfig_mmio_res_t;

typedef struct
{
    unsigned char option;       /*!<Write option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char message_type;
    unsigned char segment;
    unsigned int pci_config_addr;    /*!<Address of the MSR */
    union peci_data data;	/*!<Data to be written on the Index. */
}__attribute__((packed)) peci_wrendpointconfig_pci_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
}__attribute__((packed)) peci_wrendpointconfig_pci_res_t;

typedef struct
{
    unsigned char option;       /*!<Write option is performed for BYTE/WORD/DWORD/QWORD */
    unsigned char host_id;      /*!<Host ID[7:1] and Retry[0] */
    unsigned char message_type;
    unsigned char bar;
    unsigned char address_type;
    unsigned char segment;
    unsigned char devicefun;
    unsigned char bus;
    unsigned long long pci_config_addr;    /*!<Address of the MSR */
    union peci_data data;	/*!<Data to be written on the Index. */
}__attribute__((packed)) peci_wrendpointconfig_mmio_req_t;

typedef struct
{
    unsigned char completion_code;  /*!< Completion code */
}__attribute__((packed)) peci_wrendpointconfig_mmio_res_t;


#define PECI_CTL_FILE	"/dev/peci"

void peci_cmd_enable_awfcs(peci_cmd_t* peci_cmd);

int peci_generic_cmd(char dev_id, char target, char domain, char xmit_feedback, char awfcs, char *write_buf, char write_len, char *read_buf, char read_len);

/* PECI Command API */
int peci_cmd_ping(char dev_id, char target);
int peci_cmd_read_temp(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len, char *read_buffer);
int peci_cmd_get_dib(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len, unsigned char *pecigetdibres);
int peci_cmd_rdpkgconfig(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_rdpkgconfig_req_t *req, peci_rdpkgconfig_res_t *res);
int peci_cmd_wrpkgconfig(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len, peci_wrpkgconfig_req_t *req, peci_wrpkgconfig_res_t *res);
int peci_cmd_rdiamsr(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len, peci_rdiamsr_req_t *req, peci_rdiamsr_res_t *res);
int peci_cmd_wriamsr(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_wriamsr_req_t *req, peci_wriamsr_res_t *res);
int peci_cmd_rdpciconfig(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_rdpciconfig_req_t *req, peci_rdpciconfig_res_t *res);
int peci_cmd_wrpciconfig(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_wrpciconfig_req_t *req, peci_wrpciconfig_res_t *res);
int peci_cmd_rdpciconfiglocal(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_rdpciconfig_req_t *req, peci_rdpciconfig_res_t *res);
int peci_cmd_wrpciconfiglocal(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_wrpciconfig_req_t *req, peci_wrpciconfig_res_t *res);
int peci_cmd_rdiamsrex(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_rdiamsrex_req_t *req, peci_rdiamsrex_res_t *res);
int peci_cmd_crashdump_discovery(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_crashdump_disc_req_t *req, peci_crashdump_disc_res_t *res);
int peci_cmd_crashdump_getframe(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_crashdump_getframe_req_t *req, peci_crashdump_getframe_res_t *res);
int peci_cmd_rdendpointconfig_pci(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_rdendpointconfig_pci_req_t *req, peci_rdendpointconfig_pci_res_t *res);
int peci_cmd_rdendpointconfig_mmio(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_rdendpointconfig_mmio_req_t *req, peci_rdendpointconfig_mmio_res_t *res);
int peci_cmd_wrendpointconfig_pci(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_wrendpointconfig_pci_req_t *req,peci_wrendpointconfig_pci_res_t *res);
int peci_cmd_wrendpointconfig_mmio(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, peci_wrendpointconfig_mmio_req_t *req, peci_wrendpointconfig_mmio_res_t *res);

/* IPMI PECI Command API */
int peci_cmd_ping_ipmi(char dev_id, char target);
int peci_cmd_read_temp_ipmi(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len, char *read_buffer);
int peci_cmd_get_dib_ipmi(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len, unsigned char *pecigetdibres);
int peci_cmd_rdpkgconfig_ipmi(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len, unsigned char *pecirdpkgreq, unsigned char *pecirdpkgres);
int peci_cmd_wrpkgconfig_ipmi(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len, unsigned char *peciwrpkgreq, unsigned char *peciwrpkgres);
int peci_cmd_rdiamsr_ipmi(char dev_id, char target, char domain, char xmit_feedback, char awfcs, int *read_len,  unsigned char *pecirdiamsrreq, unsigned char *pecirdiamsrres);
int peci_cmd_wriamsr_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *peciwriamsrreq, unsigned char *peciwriamsrres);
int peci_cmd_rdpciconfig_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *pecirdpcireq, unsigned char *pecirdpcires);
int peci_cmd_wrpciconfig_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *peciwrpcireq, unsigned char *peciwrpcires);
int peci_cmd_rdpciconfiglocal_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char*pecirdpcilocalreq, unsigned char *pecirdpcilocalres);
int peci_cmd_wrpciconfiglocal_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *peciwrpcilocalreq, unsigned char *peciwrpcilocalres);
int peci_cmd_rdiamsrex_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *pecirdiamsrexreq, unsigned char *pecirdiamsrexres);
int peci_cmd_crashdump_discovery_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *pecicrashdumpreq, unsigned char *pecicrashdumpres);
int peci_cmd_crashdump_getframe_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *pecicrashdumpreq, unsigned char *pecicrashdumpres);
int peci_cmd_rdendpointconfig_pci_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *pecirdendpointreq, unsigned char *pecirdendpointres);
int peci_cmd_rdendpointconfig_mmio_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *pecirdendpointreq, unsigned char *pecirdendpointres);
int peci_cmd_wrendpointconfig_pci_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char *peciwrendpointreq, unsigned char *peciwrendpointres);
int peci_cmd_wrendpointconfig_mmio_ipmi(char dev_id, char target, char domain, char Xmit_Feedback, char awfcs, int *read_len, unsigned char* peciwrendpointreq, unsigned char *peciwrendpointres);
#endif
