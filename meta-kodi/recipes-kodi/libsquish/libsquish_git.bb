DESCRIPTION = "libCEC allows you in combination with the right hardware to control your device with your TV remote control. Utilising your existing HDMI cabling"
LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=2a51a796ca47e91336a4d198147ba58f"

PR = "r0"
inherit autotools-brokensep pkgconfig

SRC_URI = "git://github.com/OpenELEC/libsquish.git \
            file://0001-remove-installdir-from-config.patch \
            "

SRCREV = "52e7d93c5947f72380521116c05d97c528863ba8"

S = "${WORKDIR}/git"

do_install() {
  install -d ${D}/usr
  install -d ${D}/usr/include
  install -d ${D}/usr/lib
  install -d ${D}/usr/lib/pkgconfig  
  install -m 400 ${S}/squish.h ${D}/usr/include/squish.h
  install -m 400 ${S}/squish.pc ${D}/usr/lib/pkgconfig/squish.pc
  install -m 400 ${S}/libsquish.a ${D}/usr/lib/libsquish.a  
}

FILES_${PN} += "/usr/include/squish.h"
FILES_${PN} += "/usr/lib/pkgconfig/squish.pc"
FILES_${PN} += "/usr/lib/libsquish.a"