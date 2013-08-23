/*
 * =====================================================================================
 *
 *       Filename:  semailMS.h
 *
 *    Description:  Header file for the semailMS microservices that are used
 *    to send emails
 *
 *        Version:  1.0
 *        Created:  01/07/13 04:10:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Adil Hasan, 
 *   Organization:  DataTailor Ltd
 *
 * =====================================================================================
 */
#ifndef SEMAILMS_H
#define SEMAILMS_H

#include "apiHeaderAll.h"
int msiSEMail(msParam_t*, msParam_t*, msParam_t*, msParam_t*, 
		ruleExecInfo_t*);
int chkAddrStringForSystem(char*);
int processAddress(char*);
#endif /* SEMAILMS_H */
