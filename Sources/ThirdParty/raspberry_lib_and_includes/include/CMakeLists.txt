
set(EXPORT_HEADERS
    unicode/regex.h
    unicode/utypes.h
    unicode/umachine.h
    unicode/ptypes.h
    unicode/platform.h
    unicode/uconfig.h
    unicode/uvernum.h
    unicode/urename.h
    unicode/uversion.h
    unicode/utf.h
    unicode/utf8.h
    unicode/utf16.h
    unicode/utf_old.h
    unicode/uobject.h
    unicode/unistr.h
    unicode/rep.h
    unicode/std_string.h
    unicode/stringpiece.h
    unicode/bytestream.h
    unicode/ucasemap.h
    unicode/ustring.h
    unicode/putil.h
    unicode/uiter.h
    unicode/localpointer.h
    unicode/utext.h
    unicode/uchar.h
    unicode/chariter.h
    unicode/parseerr.h
    unicode/uregex.h
)

xdk_export_headers(rasp_headers ${EXPORT_HEADERS})
