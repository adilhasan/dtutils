This module contains rules and microservices for various tasks for the iRODS
data management system (http://www.irods.org).

Installation
1) Make sure you have download the iRODS source code
2) copy the dtUtils module to the <iRODS>/modules directory
3) Register the module: <iRODS>/scripts/configure --enable-dtUtils
4) Stop the iRODS server: <iRODS>/irodsctl stop
4) Rebuild iRODS (since this has new header files it means rebuilding the iRODS server code: make
5) Start the server; <iRODS>/irodsctl start

You should now be able to run the tests (you will need to modify the input arguments in the test rules.

Please let me know if you'd like new microservices or rules and I will try to add them. 

Adil Hasan 15/Aug/13 Created

Copyright (c) Adil Hasan 2013 DataTailor Ltd All Rights Reserved

