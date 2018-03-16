DESCRIPTION = "Hello world program"
PR = "r0"
LICENSE = "MYLICESECLOSED"
SRC_URI = "file://myhelloworld.c \
           file://README.txt"




do_compile() {
        ${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/myhelloworld.c -o myhelloworld
}

do_install() {
        install -m 0755 -d ${D}${bindir} ${D}${docdir}/myhelloworld
        install -m 0644 ${S}/myhelloworld ${D}${bindir}
        install -m 0644 ${WORKDIR}/README.txt ${D}${docdir}/myhelloworld
}
