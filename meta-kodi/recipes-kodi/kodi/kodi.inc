DESCRIPTION = "Kodi - Media player and entertainment hub"
LICENSE = "GPLv2"

SRC_URI = "git://github.com/xbmc/xbmc.git;branch=Isengard"
SRC_URI += "file://Fix_JsonSchemaBuilder_build.patch \
            file://0001-gettimeofday-fix-C-crosscompilation.patch "

LIC_FILES_CHKSUM = "file://LICENSE.GPL;md5=930e2a5f63425d8dd72dbd7391c43c46"

KODI_DEPENDENCY = " \
	python-distutils \
	python-textutils \
	python-sqlite3 \
	python-pickle \
	python-logging \
	python-elementtree \
	python-curses \
	python-compile \
	python-compiler \
	python-fcntl \
	python-shell \
	python-multiprocessing \
	python-subprocess \
	python-xmlrpc \
	python-netclient \
	python-netserver \
	python-unixadmin \
	python-compression \
	python-json \
	python-unittest \
	python-mmap \
	python-difflib \
	python-pprint \
	python-git \
	python-pkgutil \
	python-pycairo \
	python-mako \
	"

DEPENDS = " \
	${KODI_DEPENDENCY} \
	alsa-lib \
	boost \
	cairo \
	cmake \
	curl \
	dbus \
	doxygen-native \
	ffmpeg-kodi \
	flac \
	fontconfig \
	freetype \
	jasper \
	libass \
	libcdio \
	libmicrohttpd \
	libmodplug \
	libplist \
	libssh \
	libsquish \
	libtinyxml \
	libtinyxml-native \
	libvorbis \
	libxslt \
	lzo \
	mariadb \
	mpeg2dec \
	pango \
	python \
	rtmpdump \
	samba \
	swig \
	swig-native \
	taglib \
	tiff \
	udev \
	yajl \
	zlib \
"

DEPENDS_append = " virtual/kernel "

DEPENDS_append_mx6 = " libfslvpuwrap \
                       libcec-imx \
                     "

RDEPENDS_${PN} = "lsb \
                  ${PN}-bin \
                  ${KODI_DEPENDENCY} \
                 "

S = "${WORKDIR}/git/"

inherit autotools-brokensep lib_package pkgconfig gettext python-dir

PACKAGECONFIG_GL ?= "${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'opengl', '', d)}"
#PACKAGECONFIG_GL ?= "${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'gles2', '', d)}"

PACKAGECONFIG ?= " \
                  ${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'x11', '', d)} \
                  ${PACKAGECONFIG_GL} \
                  udev \
                  avahi \
                  airplay \
                 "

PACKAGECONFIG[x11]    = "--enable-x11,--disable-x11,virtual/libx11"
PACKAGECONFIG[opengl] = "--enable-gl,--disable-gl,virtual/libgl libglu glew"
PACKAGECONFIG[gles2]  = "--enable-gles ,--disable-gles , virtual/libgles2 virtual/egl "
PACKAGECONFIG[openmax]  = "--enable-openmax,--disable-openmax, libomxil"
PACKAGECONFIG[udev]   = "--enable-udev,--disable-udev,udev"
PACKAGECONFIG[libva]  = "--enable-vaapi,--disable-vaapi,libva libav"
PACKAGECONFIG[sdl]    = "--enable-joystick,--disable-joystick,libsdl"
PACKAGECONFIG[avahi]  = "--enable-avahi,--disable-avahi,avahi"
PACKAGECONFIG[libcec] = "--enable-libcec,--disable-libcec,libcec"
PACKAGECONFIG[airplay]    = "--enable-airplay --enable-airtunes,--disable-airplay --disable-airtunes, shairplay"
PACKAGECONFIG[pulseaudio] = "--enable-pulse,--disable-pulse,pulseaudio"

EXTRA_OECONF=" --disable-debug \
	--build=${BUILD_SYS} \
	--host=${HOST_SYS} \
	--target=${TARGET_SYS} \
	${@append_libtool_sysroot(d)} \
	--enable-optimizations \
	--disable-projectm \
	--disable-vdpau \
	--disable-texturepacker \
	--disable-mid \
	--disable-optical-drive \
	--disable-dvdcss \
	--disable-nfs \
	--disable-gtest \
	--disable-rsxs \
	LIBS=' -llzma ' \
           "
# to-do: add libnfs from openelec git

EXTRA_OECONF_append = "${@bb.utils.contains('MACHINE', 'mx6', ' --prefix=/imx6/xbmc', ' --prefix=/usr', d)}"

CXXFLAGS += " -I${STAGING_KERNEL_DIR}/include/uapi -I${STAGING_KERNEL_DIR}/include "
CFLAGS += " -I${STAGING_KERNEL_DIR}/include/uapi -I${STAGING_KERNEL_DIR}/include "

FILES_${PN} += "${datadir}/icons/hicolor"
FILES_${PN}_mx6 += " /imx6"

do_configure() {
   export PYTHON_EXTRA_LDFLAGS=""
   export PYTHON_EXTRA_LIBS="-lz"
   export PYTHON_VERSION="${PYTHON_BASEVERSION}"
   export PYTHON_NOVERSIONCHECK="no-check"
   export PYTHON_CPPFLAGS="-I/${STAGING_INCDIR}/${PYTHON_DIR}"
   export PYTHON_LDFLAGS="-L${STAGING_LIBDIR} -lpython${PYTHON_BASEVERSION}"
   export TINYXML_CFLAGS="-I/${STAGING_INCDIR}"
   export TINYXML_LIBS="-L${STAGING_LIBDIR} -ltinyxml"
   export SQUISH_CFLAGS="-I/${STAGING_INCDIR}"
   export SQUISH_LIBS="-L${STAGING_LIBDIR} -lsquish"
   cd ${S}
   sh bootstrap
   sh configure ${EXTRA_OECONF}
}

do_compile() {
   cd ${S}
   oe_runmake BUILDDIR=${BUILDDIR} DESTDIR=${D}
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"

FILES_${PN} += "${datadir}/xsessions/kodi.desktop"
