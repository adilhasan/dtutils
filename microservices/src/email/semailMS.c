/**
 * \fn msiSEMail(msParam_t* xtoAddr, msParam_t* xsubjectLine, msParam_t* xbody, ruleExecInfo_t *rei)
 *
 * \brief Sends email
 * 
 * \module dtUtils
 * 
 * \author  Adil Hasan
 * \organisation DataTailor Ltd
 * \date    2013-07
 * 
 * \note   This microservice sends e-mail using the UNIX mail command. Attachments are not supported. The microservice is based on the core msiSendMail
 * microservice.
 *
 *
 * \param[in] xfromAddr - a msParam of type STR_MS_T which is the address of the sender.
 * \param[in] xtoAddr - a msParam of type STR_MS_T which is an address of the receiver.
 * \param[in] xsubjectLine - a msParam of type STR_MS_T which is a subject of the message.
 * \param[in] xbody - a msParam of type STR_MS_T which is a body of the message.
 * \param[in,out] rei - The RuleExecInfo structure that is automatically
 *    handled by the rule engine. The user does not include rei as a
 *    parameter in the rule invocation.
 *
 * \DolVarDependence none
 * \DolVarModified none
 * \iCatAttrDependence none
 * \iCatAttrModified none 
 * \sideeffect An e-mail is sent to the specified recipient.
 *
 * \return integer
 * \retval 0 on success
 * \pre none
 * \post none
 * \sa none
 *        Version:  1.0
 *        Created:  01/07/13 12:00:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Adil Hasan, 
 *   Organization:  DataTailor Ltd
 *
**/
//#include "reGlobalsExtern.h"
//#include "icatHighLevelRoutines.h"

#include "semailMS.h"

int msiSEMail(msParam_t* xfromAddr, msParam_t* xtoAddr, 
		msParam_t* xsubjectLine, msParam_t* xbody, 
		ruleExecInfo_t *rei)
{
  char *mailStr;
  char fName[100];
  char *t1, *t2;
  FILE *fd;
  char *toAddr;
  char *fromAddr;
  char *subjectLine;
  char *body;
  int status;

  fromAddr = (char *) xfromAddr->inOutStruct;
  toAddr = (char *) xtoAddr->inOutStruct;
  subjectLine = (char *) xsubjectLine->inOutStruct;
  body = (char *) xbody->inOutStruct;
  
  status = chkAddrStringForSystem(fromAddr);
  if (status) return(status);
  status = chkAddrStringForSystem(toAddr);
  if (status) return(status);
  status = checkStringForSystem(subjectLine);
  if (status) return(status);

    if (reTestFlag > 0 ) {
      if (reTestFlag == COMMAND_TEST_1) {
	fprintf(stdout,"  Sending Email\n     To:%s\n     Subject:%s\n     Body:%s\n",
		toAddr,subjectLine,body);
      }
      else if (reTestFlag == HTML_TEST_1) {
	fprintf(stdout,"Sending Email\n<UL>\n");
	fprintf(stdout,"<LI>To: %s\n",toAddr);
	fprintf(stdout,"<LI>subjectLine: %s\n",subjectLine);
	fprintf(stdout,"<LI>Body: %s\n",body); 
	fprintf(stdout,"</UL>\n");
      }
      else if (reTestFlag == LOG_TEST_1) 
	  rodsLog (LOG_NOTICE,"   Calling msiSEMail To:%s Subject %s\n",
		   toAddr,subjectLine);
      if (reLoopBackFlag > 0)
	return(0);
  }
    sprintf(fName,"mailFile%d.ml",getpid());
    fd = fopen(fName,"w");
    if (fd == NULL)
      return(FILE_CREATE_ERROR);
    t1 = body;
#ifdef solaris_platform
    if (subjectLine != NULL && strlen(subjectLine) > 0)
      fprintf(fd,"Subject:%s\n\n",subjectLine);
#endif
    while (t1 != NULL) {
      if ((t2 = strstr(t1,"\\n")) != NULL)
	*t2 = '\0';
      fprintf(fd,"%s\n",t1);
      if (t2 != NULL) {
	*t2 = '\\';
	t1 = t2+2;
      }
      else 
	t1 = NULL;
    }
    fclose(fd);
    mailStr = (char*)malloc (strlen(toAddr)+strlen(subjectLine)+100);
    if (mailStr == NULL) return SYS_MALLOC_ERR;

#ifdef solaris_platform
    sprintf(mailStr,"cat %s| mail  %s",fName,toAddr);
#else /* tested for linux - not sure how other platforms operate for subject */
    if (subjectLine != NULL && strlen(subjectLine) > 0)
      sprintf(mailStr,"cat %s| mail -r %s -s '%s'  %s",fName,fromAddr, 
		      subjectLine, toAddr);
    else
      sprintf(mailStr,"cat %s| mail -r %s %s",fName,fromAddr, toAddr);
#endif

    system(mailStr); 
    sprintf(mailStr,"rm %s",fName);
    system(mailStr);
    free(mailStr);
    return(0);
}

int chkAddrStringForSystem(char *inString) {
	char c;
	int MAX_EMAIL_ADDR = 100;
	char *firstOccurence;
	char *lastOccurence;
	char **eAddress = (char**)malloc(MAX_EMAIL_ADDR*sizeof(*eAddress));
	char **eAddressStart = eAddress;

	// Check the string is within the RFC limit
	if (strlen(inString) > 256 || strlen(inString) == 0) {
		return (USER_INPUT_STRING_ERR);
	}

	c = *inString;

	// Do not allow emails with '@' in the name or domain
	// unless there is a ',' indicating multiple email addresses
	firstOccurence = strchr(inString, '@');
        lastOccurence = strrchr(inString, '@');
	if (firstOccurence != lastOccurence) {
		if (!strchr(inString, ',')) {
			return (USER_INPUT_STRING_ERR);
		}
	}

	// Split the address string into separate email addresses
	char *inString2 = strdup(inString);
	char *res = strtok(inString2, ",");
	while (res != NULL) {
		*eAddress = strdup(res);
		res = strtok(NULL, ",");
		*eAddress++;
	}

	// Now check each email address
	int arrayLen = eAddress - eAddressStart;
	int count;
	int ok = 0;
	for (count = 0; count < arrayLen; count++) {
		ok = processAddress(eAddressStart[count]);
		if (ok == 0) {
			return (USER_INPUT_STRING_ERR);
		}
	}
	return 0;
}

int processAddress(char inString[]) {
	char *name = (char*)malloc(strlen(inString)*sizeof(char));
	char *domain;
	char c, n, d;
	int nameOK = 0;
	int domainOK = 0;
	int failure = 0;
	int success = 1;
	int first = 1;
	int firstBlank = 0;
	int acount = 0;
	char *inStringO = strdup(inString);

	// Split the address into a name and domain
	for (c=*inString; c != '\0'; c=*(++inString)) {
		if (c == '@') {
			strncpy(name, inStringO, acount);
			name[acount] = '\0';
			*(++inString);
			domain = strdup(inString);
			break;
		}
		acount++;
	}

	// Check the name string to see if it conforms roughly with the
	// standard
	for (c=*name; c != '\0'; c=*(++name)) {
		if ((first || firstBlank) && isspace(c)) {
			firstBlank = 1;
			first = 0;
			continue;
		}
		first = 0;
		firstBlank = 0;
		if (isalnum(c)|| c == '.' || c == '_' || c== '%' || c== '+' || c== '-') {
			nameOK = 1;
		} else {
			nameOK = 0;
			break;
		}
	}

	// If the name fails the check just return failure 
	// (no point in going further)
	if (nameOK == 0) {
		return failure;
	}

	// For the domain we need some characters before the 
	// first dot and up to 4 characters after the last 
	// dot: .com, .uk, .no etc
	char *domainW = strdup(domain);
	char *res = strtok(domainW, ".");
	char *outp;
	while (res != NULL) {
		res = strtok(NULL, ".");
		if (res != NULL) {
			outp = strdup(res);
		}
	}
	int countDom = 0;
	for (c=*outp; c != '\0'; c=*(++outp)) {
		if (isalpha(c)) {
			domainOK = 1;
			++countDom;
		} else {
			domainOK = 0;
			break;
		}
	}
	if (!(domainOK == 1 && countDom <= 4 && countDom >= 2)) {
		return failure;
	}

	// Check the rest of the domain and see if it conforms
	for (c=*domain; c != '\0'; c=*(++domain)) {
		if (isalnum(c)|| c == '.' || c == '-') {
			domainOK = 1;
		} else {
			domainOK = 0;
			break;
		}
	}
	if (domainOK == 0) {
		return failure;
	}
	return success;	
}
