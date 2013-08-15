# Example test rule showing how the sha1 checksum can be computed
# and stored as AVU user-defined metadata.
# Input: *coll - the collection part of the iRODS data object
#        *data - the iRODS data object
#
acTest {
	msiSha1Data("*coll/*data", "replNum=0", *outsha1);
	writeLine("stdout", "sha1 for *data 0 is *outsha1");
	msiAddKeyVal(*shaKV, "SHA1-r0", "*outsha1");
	msiSetKeyValuePairsToObj(*shaKV, "*coll/*data", "-d");
}
input *coll="/adilZ/home/rods/test", *data="Makefile"
output ruleExecOut
