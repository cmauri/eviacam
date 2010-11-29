## Process this file with automake to create Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	creavision \
	libwebcam \
	wxutil \
	wxcamwindow \
	po \
	doc \
	src \
	rpm

EXTRA_DIST = \
	autogen.sh \
	README.ca \
	README.es \
	INSTALL.es \
	AGRAIMENTS.txt \
	win32/eviacam.sln \
	win32/installer.iss \
	win32/eViacam.exe.manifest \
	win32/Info \
	eviacam.kdevelop \
	debian/changelog  \
	debian/compat  \
	debian/control \
	debian/copyright \
	debian/dirs \
	debian/docs \
	debian/rules \
	m4 

dist_pkgdata_DATA = THANKS

INCLUDES = \
	-I$(top_srcdir)/wxutil \
	-I$(top_srcdir)/creavision \
	-I$(top_srcdir)/libwebcam \
	-I$(top_srcdir)/wxcamwindow

maintainer-clean-local:
	-rm -rf win32/Release
	-rm -rf win32/Debug
	-rm -rf debian-build
	-rm -f win32/*.exe
	-rm -f win32/*.suo
	-rm -f win32/*.ncb
	-rm -rf autom4te.cache
	-rm -f eviacam.kdevelop.pcs
	-rm -f eviacam.kdevses
	-rm *~
	-rm creavision/*~
	-rm libwebcam/*~
	-rm wxutil/*~
	-rm wxcamwindow/*~
	-rm src/*~	

##
## Debian packages
##

## make a Debian binary package
deb: configure dist
	-$(RM) -r debian-build
	-mkdir debian-build
	cd debian-build ; \
	mv ../$(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)_$(VERSION).orig.tar.gz ; \
	tar xfvz $(PACKAGE)_$(VERSION).orig.tar.gz ; \
	cd $(PACKAGE)-$(VERSION) ; \
	dpkg-buildpackage -B -rfakeroot -uc ; \
	cd .. ; \
	$(RM) -r $(PACKAGE)-$(VERSION)

## make a Debian source package
deb-src: configure dist
	-$(RM) -r debian-src
	-mkdir debian-src
	cd debian-src ; \
	mv ../$(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)_$(VERSION).orig.tar.gz ; \
	tar xfvz $(PACKAGE)_$(VERSION).orig.tar.gz ; \
	cd $(PACKAGE)-$(VERSION) ; \
	dpkg-buildpackage -S -i -rfakeroot -uc -us ; \
	cd .. ; \
	$(RM) -r $(PACKAGE)-$(VERSION)

##
## RPM packages
##
rpm:	configure dist-bzip2
#	(cd rpm && $(MAKE) $(AM_MAKEFLAGS) Makefile *.spec) || exit 1
	(cd rpm && $(MAKE) $(AM_MAKEFLAGS) this) || exit 1
