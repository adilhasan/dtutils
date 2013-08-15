#ifndef SHA1DATA_H
#define SHA1DATA_H

#include "apiHeaderAll.h"
#include <openssl/sha.h>

int msiSha1Data(msParam_t *infile, msParam_t *options, 
		msParam_t *sha1sum, ruleExecInfo_t *rei);
#endif /* SHA1DATA_H */
