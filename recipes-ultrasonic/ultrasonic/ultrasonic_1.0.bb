DESCRIPTION = "This is a simple Hello World recipe - uses a local source file"
HOMEPAGE = "https://www.coventry.ac.uk"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = "file://ultrasonic.c"
S = "${WORKDIR}"
do_compile() {
${CC} ${LDFLAGS} ultrasonic.c -o ultrasonic
}
do_install() {
install -d ${D}${bindir}
install -m 0755 ultrasonic ${D}${bindir}
}