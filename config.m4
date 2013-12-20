dnl config.m4 for extension shellinford

dnl Check PHP version:
AC_MSG_CHECKING(PHP version)
if test ! -z "$phpincludedir"; then
    PHP_VERSION=`grep 'PHP_VERSION ' $phpincludedir/main/php_version.h | sed -e 's/.*"\([[0-9\.]]*\)".*/\1/g' 2>/dev/null`
elif test ! -z "$PHP_CONFIG"; then
    PHP_VERSION=`$PHP_CONFIG --version 2>/dev/null`
fi

if test x"$PHP_VERSION" = "x"; then
    AC_MSG_WARN([none])
else
    PHP_MAJOR_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\1/g' 2>/dev/null`
    PHP_MINOR_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\2/g' 2>/dev/null`
    PHP_RELEASE_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\3/g' 2>/dev/null`
    AC_MSG_RESULT([$PHP_VERSION])
fi

if test $PHP_MAJOR_VERSION -lt 5; then
    AC_MSG_ERROR([need at least PHP 5 or newer])
fi

dnl Shellinford Extension
PHP_ARG_ENABLE(shellinford, whether to enable shellinford support,
[  --enable-shellinford      Enable shellinford support])

if test "$PHP_SHELLINFORD" != "no"; then

    dnl compiler C++:
    PHP_REQUIRE_CXX()

    dnl Source shellinford
    SHELLINFORD_SOURCES="shellinford/src/shellinford_bit_vector.cc shellinford/src/shellinford_fm_index.cc"

    if test -z "$SHELLINFORD_SOURCES"; then
        dnl Check for Shellinford header
        PHP_ARG_WITH(shellinford-includedir, for shellinford header,
        [ --with-shellinford-includedir=DIR shellinford header path], yes)

        if test "$PHP_SHELLINFORD_INCLUDEDIR" != "no" && test "$PHP_SHELLINFORD_INCLUDEDIR" != "yes"; then
            if test -r "$PHP_SHELLINFORD_INCLUDEDIR/shellinford_fm_index.h"; then
                SHELLINFORD_INCLUDES="$PHP_SHELLINFORD_INCLUDEDIR"
            else
                AC_MSG_ERROR([Can'|'t find shellinford headers under "$PHP_SHELLINFORD_INCLUDEDIR"])
           fi
        else
            SEARCH_PATH="/usr/local /usr"
            SEARCH_FOR="/include/shellinford_fm_index.h"
            if test -r $PHP_SHELLINFORD/$SEARCH_FOR; then
                SHELLINFORD_INCLUDES="$PHP_SHELLINFORD/include"
            else
                AC_MSG_CHECKING([for shellinford header files in default path])
                for i in $SEARCH_PATH ; do
                    if test -r $i/$SEARCH_FOR; then
                        SHELLINFORD_INCLUDES="$i/include"
                        AC_MSG_RESULT(found in $i)
                    fi
                done
            fi
        fi

        if test -z "$SHELLINFORD_INCLUDES"; then
            AC_MSG_RESULT([not found])
            AC_MSG_ERROR([Can'|'t find shellinford headers])
        fi

        PHP_ADD_INCLUDE($SHELLINFORD_INCLUDES)

        dnl Check for Shellinford library
        PHP_ARG_WITH(shellinford-libdir, for shellinford library,
        [ --with-shellinford-libdir=DIR shellinford library path], yes)

        LIBNAME=shellinford
        AC_MSG_CHECKING([for shellinford])
        AC_LANG_SAVE
        AC_LANG_CPLUSPLUS

        save_CPPFLAGS="$CPPFLAGS"
        shellinford_CPPFLAGS="-I$SHELLINFORD_INCLUDES"
        CPPFLAGS="$save_CPPFLAGS $shellinford_CPPFLAGS"

        save_LDFLAGS="$LDFLAGS"
        shellinford_LDFLAGS="-L$PHP_SHELLINFORD_LIBDIR -l$LIBNAME -lstdc++"
        LDFLAGS="$save_LDFLAGS $shellinford_LDFLAGS"

        AC_TRY_LINK(
        [
            #include "$SHELLINFORD_INCLUDES/shellinford_fm_index.h"
        ],[
            shellinford::fm_index fm(false);
        ],[
            AC_MSG_RESULT(yes)
            PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PHP_SHELLINFORD_LIBDIR, SHELLINFORD_SHARED_LIBADD)
            AC_DEFINE(HAVE_SHELLINFORDLIB,1,[ ])
        ],[
            AC_MSG_RESULT([error])
            AC_MSG_ERROR([wrong shellinford lib version or lib not found])
        ])
        CPPFLAGS="$save_CPPFLAGS"
        LDFLAGS="$save_LDFLAGS"
        AC_LANG_RESTORE
    else
        PHP_ADD_INCLUDE("shellinford/src")
        PHP_ADD_LIBRARY(stdc++, , SHELLINFORD_SHARED_LIBADD)
    fi

    PHP_SUBST(SHELLINFORD_SHARED_LIBADD)

    dnl PHP Extension
    PHP_NEW_EXTENSION(shellinford, shellinford.cpp fmindex.cpp exception.cpp $SHELLINFORD_SOURCES, $ext_shared)
fi

dnl coverage
PHP_ARG_ENABLE(coverage, whether to enable coverage support,
[  --enable-coverage     Enable coverage support], no, no)

if test "$PHP_COVERAGE" != "no"; then
    EXTRA_CFLAGS="--coverage"
    PHP_SUBST(EXTRA_CFLAGS)
fi
