This module contains rules and microservices for various tasks for the iRODS
data management system.

Adil Hasan 15/Aug/13 Created

Copyright (c) Adil Hasan 2013 DataTailor Ltd All Rights Reserved


microservices                             Description
---------------------------------------------------------------------------
msiSha1Data(infile, option,               Microservice to compute the
sha1)                                     SHA1 checksum for an iRODS data
                                          object and return it as a string.
                                          Currently, the only option is 
                                          replNum=nnn (ie the replica number)
---------------------------------------------------------------------------


rules                                     Description
---------------------------------------------------------------------------
---------------------------------------------------------------------------


test                                     Description
---------------------------------------------------------------------------
testChksum.r                           An example test rule showing how
                                       the microservice can be used to
                                       compute and store the checksum value.

testValidate.r                         An example test rule showing how the
                                       microservice can be used to validate
                                       the stored checksum.
---------------------------------------------------------------------------

scripts                                    Description
---------------------------------------------------------------------------
---------------------------------------------------------------------------                
