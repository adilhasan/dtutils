# Example rule showing how the sha1 checksum can be computed and compared
# with a stored value.
# Input: *coll - the collection name of the data object
#        *data - the iRODS data object
#
acTestValidate {
	msiSha1Data("*coll/*data", "replNum=0", *outsha1);
	writeLine("stdout", "sha1 for *data 0 is *outsha1");
	msiMakeGenQuery("META_DATA_ATTR_NAME, META_DATA_ATTR_VALUE",
			"META_DATA_ATTR_NAME like 'SHA1%' and DATA_NAME = '*data' and COLL_NAME = '*coll'", *query);
	msiExecGenQuery(*query, *outKVs);
	foreach(*outKVs) {
		msiGetValByKey(*outKVs, "META_DATA_ATTR_VALUE", *shval);
	}
	if (*shval == *outsha1) {
		writeLine("stdout", "sha values ok!");
	}
}
input *coll="/adilZ/home/rods/test", *data="Makefile"
output ruleExecOut
