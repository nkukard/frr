## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = subdir-objects 1.12
include common.am

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/lib -I$(top_builddir) -I$(top_builddir)/lib
AM_CFLAGS = $(WERROR)
DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\" -DCONFDATE=$(CONFDATE)
LIBCAP = @LIBCAP@

EXTRA_DIST =
BUILT_SOURCES =
CLEANFILES =

examplesdir = $(exampledir)

bin_PROGRAMS =
sbin_PROGRAMS =
noinst_PROGRAMS =
noinst_HEADERS =
noinst_LIBRARIES =
lib_LTLIBRARIES =
module_LTLIBRARIES =
pkginclude_HEADERS =
nodist_pkginclude_HEADERS =
dist_examples_DATA =

## libtool, the self-made GNU scourge
## ... this should fix relinking
## ... and AUTOMAKE_DUMMY is needed to prevent automake from treating this
##     as overriding the normal targets...
$(AUTOMAKE_DUMMY)install-moduleLTLIBRARIES: install-libLTLIBRARIES
$(AUTOMAKE_DUMMY)install-binPROGRAMS: install-libLTLIBRARIES
$(AUTOMAKE_DUMMY)install-sbinPROGRAMS: install-libLTLIBRARIES

include lib/subdir.am
include zebra/subdir.am
include qpb/subdir.am
include fpm/subdir.am

include ripd/subdir.am
include ripngd/subdir.am
include ospfd/subdir.am
include ospf6d/subdir.am
include ospfclient/subdir.am
include isisd/subdir.am
include nhrpd/subdir.am
include ldpd/subdir.am
include babeld/subdir.am
include eigrpd/subdir.am

SUBDIRS = . @LIBRFP@ @RFPTEST@ \
	 @BGPD@ \
         @PIMD@ \
	 @WATCHFRR@ @VTYSH@ @DOC@ \
         @SOLARIS@ tests tools

DIST_SUBDIRS = . bgpd \
	  watchfrr vtysh doc tests \
	  solaris pimd bgpd/rfp-example/librfp \
	  bgpd/rfp-example/rfptest tools \
	  # end

if PKGSRC
rcdir=@pkgsrcrcdir@
rc_SCRIPTS = \
	pkgsrc/bgpd.sh \
	pkgsrc/ospf6d.sh \
	pkgsrc/ospfd.sh \
	pkgsrc/ripd.sh \
	pkgsrc/ripngd.sh \
	pkgsrc/zebra.sh \
	# end
endif

EXTRA_DIST += \
	REPORTING-BUGS \
	SERVICES \
	aclocal.m4 \
	update-autotools \
	m4/README.txt \
	\
	python/clidef.py \
	python/clippy/__init__.py \
	\
	redhat/frr.init \
	redhat/frr.service \
	redhat/daemons \
	redhat/frr.logrotate \
	redhat/frr.pam \
	redhat/frr.spec \
	redhat/README.rpm_build.md \
	\
	snapcraft/snapcraft.yaml \
	snapcraft/README.snap_build.md \
	snapcraft/README.usage.md \
	snapcraft/extra_version_info.txt \
	snapcraft/scripts \
	snapcraft/defaults \
	snapcraft/helpers \
	snapcraft/snap \
	\
	tools/multiple-bgpd.sh \
	tools/rrcheck.pl \
	tools/rrlookup.pl \
	tools/zc.pl \
	tools/zebra.el \
	\
	vtysh/Makefile.am \
	vtysh/Makefile.in \
	# end

ACLOCAL_AMFLAGS = -I m4

noinst_HEADERS += defaults.h
