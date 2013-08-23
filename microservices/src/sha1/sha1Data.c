/*
 * @file sha1Data.c
 *
 */
/* Copyright (c), Adil Hasan, DataTailor Ltd */

/**
 * \fn msiSha1Data(msParam_t *infile, msParam_t *options, 
 * msParam_t *sha1sum, ruleExecInfo_t *rei)
 *
 * \brief This microservice computes the sha1 hash for a file.
 *
 * \module dtUtils
 *
 * \since 3.3
 *
 * \author Adil Hasan
 * \date Aug 2013
 *
 * \usage See dtUtils/test
 *
 * \param[in]   infile - the input file
 * \param[in]  replnum - the replica number of the resource to checksum
 * \param[out] sha1sum - the computed SHA1 hash value
**/

#include "sha1Data.h"

int msiSha1Data(msParam_t *infile, msParam_t *replnum, 
		msParam_t *sha1sum, ruleExecInfo_t *rei) {

	SHA_CTX inf;
	unsigned char hash[SHA_DIGEST_LENGTH];
	char hash_str[SHA_DIGEST_LENGTH];
	unsigned int i;
	char *outBadKeyWd;
	int validKwFlags;
	int bufferSize = 4*1024*1024;

	rsComm_t *rsComm;
	dataObjInp_t dataObjInp, *pdataObjInp;
	openedDataObjInp_t dataObjReadInp, *pdataObjReadInp;
	openedDataObjInp_t dataObjCloseInp, *pdataObjCloseInp;
	bytesBuf_t *dataObjReadBuff = NULL;
	int bytesRead = -1;
	int l1desc = -1;
        
	RE_TEST_MACRO("      Calling msiSha1Data");
	if (rei == NULL || rei->rsComm == NULL) {
		rodsLog(LOG_ERROR, "msiSha1Data: rei or rsComm NULL");
		return (SYS_INTERNAL_NULL_INPUT_ERR);
	}

	rsComm = rei->rsComm;
	rei->status = parseMspForDataObjInp(infile, &dataObjInp,
			&pdataObjInp, 1);

	validKwFlags = REPL_NUM_FLAG;
	if ((rei->status = parseMsKeyValStrForDataObjInp (replnum,
					pdataObjInp, KEY_WORD_KW,
					validKwFlags, &outBadKeyWd)) < 0) {
		if (outBadKeyWd != NULL) {
			rodsLogAndErrorMsg (LOG_ERROR, &rsComm->rError, 
					rei->status,
					"msiSha1Data: input keyWd - %s error. status = %d", 
					outBadKeyWd, rei->status);
			free (outBadKeyWd);
		} else {
			rodsLogAndErrorMsg (LOG_ERROR, &rsComm->rError, 
					rei->status,
					"msiSha1Data: input msKeyValStr error. status = %d",
					rei->status);
		}
		return (rei->status);
	}

	l1desc = rsDataObjOpen(rsComm, pdataObjInp);
	if (l1desc < 0) {
		rodsLogAndErrorMsg(LOG_ERROR, &rsComm->rError, 
				l1desc,
				"msiSha1Data: rsDataObjOpen failed for %s status = %d", 
				dataObjInp.objPath, l1desc);
		return(l1desc);
	}
        memset(&dataObjReadInp, 0, sizeof(dataObjReadInp));
	dataObjReadInp.l1descInx = l1desc;
	pdataObjReadInp = &dataObjReadInp;

	SHA1_Init(&inf);
	while (bytesRead != 0) {
		pdataObjReadInp->len = bufferSize;
		dataObjReadBuff = (bytesBuf_t *) malloc(sizeof(bytesBuf_t));
		memset(dataObjReadBuff, 0, sizeof(bytesBuf_t));
                bytesRead = rsDataObjRead(rsComm, pdataObjReadInp,
				dataObjReadBuff);
		if (bytesRead < 0) {
			rodsLogAndErrorMsg(LOG_ERROR, &rsComm->rError, 
					bytesRead, "msiSha1Data: rsDataObjRead failed status = %d", 
					bytesRead);
			return(bytesRead);
		}
		SHA1_Update(&inf, dataObjReadBuff->buf, dataObjReadBuff->len);
	}
        memset(&dataObjCloseInp, 0, sizeof(dataObjCloseInp));
        dataObjCloseInp.l1descInx = l1desc;
        pdataObjCloseInp = &dataObjCloseInp;
        rei->status = rsDataObjClose(rsComm, pdataObjCloseInp);	

	SHA1_Final(hash, &inf);

	for (i=0; i<sizeof(hash)/sizeof(hash[0]); i++) {
		sprintf(&hash_str[i*2], "%02x", hash[i]);
	}
	
	fillStrInMsParam(sha1sum, hash_str);

	return (rei->status);
}
