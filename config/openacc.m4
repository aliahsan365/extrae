# AX_OPENACC
# --------------------
AC_DEFUN([AX_OPENACC],
[
	AC_ARG_WITH(openacc,
		AC_HELP_STRING(
			[--with-openacc@<:@=DIR@:>@],
			[Enable support for tracing OpenACC]
		),
		[openacc_path="${withval}"],
		[openacc_path="no"]
	)

	if test -z "${openacc_path}" ; then
		AC_MSG_ERROR([Cannot find OpenACC])
	fi

	enable_openacc="no"
	PGCC=""
	if test "${openacc_path}" != "no" ; then
		AC_MSG_CHECKING([for OPENACC])
		if test -d "${openacc_path}" ; then
			if test -x ${openacc_path}/bin/pgcc ; then
				PGCC=${openacc_path}/bin/pgcc
			fi
			if test "${PGCC}" != ""; then
				if test -r ${openacc_path}/include/acc_prof.h; then
					enable_openacc="yes"
					AC_MSG_RESULT(${openacc_path})
				else
					AC_MSG_ERROR([Cannot find the necessary header files in the OPENACC path])
				fi
			else
				AC_MSG_ERROR([Cannot find the OPENACC compiler in the given path])
			fi
		else
			AC_MSG_ERROR([The specified path for OPENACC does not exist])
		fi
		AX_FLAGS_SAVE()
		AX_FIND_INSTALLATION([OPENACC], [${openacc_path}], [openacc])
		AX_FLAGS_RESTORE()
	fi
	AC_SUBST(PGCC)
	AM_CONDITIONAL(WANT_OPENACC, test "${enable_openacc}" = "yes")
])

# AX_OPENACC_SHOW_CONFIGURATION
# --------------------
AC_DEFUN([AX_OPENACC_SHOW_CONFIGURATION],
[
	if test "${enable_openacc}" = "yes" ; then
		echo OpenACC instrumentation: yes, through LD_PRELOAD
		echo OpenACC version: ${OpenACC_version}
		echo -e \\\tOpenACC home: ${openacc_path}
	else
		echo OpenACC instrumentation: no
	fi
])