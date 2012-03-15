FIND_PROGRAM(REC_BIN recordmydesktop)
if(REC_BIN)
   SET(RECORDMYDESKTOP_FOUND TRUE)
endif(REC_BIN)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RecordMyDesktop DEFAULT_MSG REC_BIN)
mark_as_advanced(REC_BIN)

