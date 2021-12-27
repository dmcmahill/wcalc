include config.mk
include ${DEPMK}

.PHONY: show-license
show-license:
	@${ECHO} "${LICENSE} ${NAME}-${VER}"

.PHONY: config-variables
config-variables:
	@${ECHO} "gtk_win32_runtime=${RUNTIMEDIR}"
	@${ECHO} "gtk_win32_devel=${DEVTIMEDIR}"

