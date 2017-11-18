DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)

if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

BBBIT_CONFIG_INCLUDES=-I$(builddir)/config
BBBIT_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

BBBIT_INCLUDES += -I$(srcdir)/secp256k1/include
BBBIT_INCLUDES += -I$(srcdir)/univalue/include

LIBBBBIT_SERVER=libbbbit_server.a
LIBBBBIT_WALLET=libbbbit_wallet.a
LIBBBBIT_COMMON=libbbbit_common.a
LIBBBBIT_CLI=libbbbit_cli.a
LIBBBBIT_UTIL=libbbbit_util.a
LIBBBBIT_CRYPTO=crypto/libbbbit_crypto.a
LIBBBBITQT=qt/libbbbitqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBUNIVALUE=univalue/libunivalue.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
  
$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libbbbit_crypto.a \
  libbbbit_util.a \
  libbbbit_common.a \
  libbbbit_server.a \
  libbbbit_cli.a
if ENABLE_WALLET
BBBIT_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libbbbit_wallet.a
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += libbbbit_zmq.a
endif

if BUILD_BBBIT_LIBS
lib_LTLIBRARIES = libbbbitconsensus.la
LIBBBBIT_CONSENSUS=libbbbitconsensus.la
else
LIBBBBIT_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_BBBITD
  bin_PROGRAMS += bbbitd
endif

if BUILD_BBBIT_UTILS
  bin_PROGRAMS += bbbit-cli bbbit-tx
if ENABLE_WALLET
  bin_PROGRAMS += wallet-utility
endif
endif

.PHONY: FORCE check-symbols check-security
# bbbit core #
BBBIT_CORE_H = \
  addressindex.h \
  spentindex.h \
  addrman.h \
  alert.h \
  amount.h \
  arith_uint256.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  core_io.h \
  core_memusage.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  key.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  net.h \
  netbase.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  prevector.h \
  primitives/block.h \
  primitives/transaction.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/script_error.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  serialize.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utilstrencodings.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  versionbits.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libbbbit_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between bbbitd and bbbit-qt
libbbbit_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libbbbit_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbbbit_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  dbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(BBBIT_CORE_H)

if ENABLE_ZMQ
LIBBBBIT_ZMQ=libbbbit_zmq.a

libbbbit_zmq_a_CPPFLAGS = $(BBBIT_INCLUDES) $(ZMQ_CFLAGS)
libbbbit_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbbbit_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between bbbitd and bbbit-qt, but only linked
# when wallet enabled
libbbbit_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES)
libbbbit_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbbbit_wallet_a_SOURCES = \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  policy/rbf.cpp \
  $(BBBIT_CORE_H)

# crypto primitives library
crypto_libbbbit_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_CONFIG_INCLUDES)
crypto_libbbbit_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbbbit_crypto_a_SOURCES = \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# common: shared between bbbitd, and bbbit-qt and non-server tools
libbbbit_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES)
libbbbit_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbbbit_common_a_SOURCES = \
  amount.cpp \
  arith_uint256.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  consensus/merkle.cpp \
  core_read.cpp \
  core_write.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_error.cpp \
  script/sign.cpp \
  script/standard.cpp \
  $(BBBIT_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libbbbit_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES)
libbbbit_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbbbit_util_a_SOURCES = \
  support/pagelocker.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpcprotocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(BBBIT_CORE_H)

if GLIBC_BACK_COMPAT
libbbbit_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between bbbit-cli and bbbit-qt
libbbbit_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES)
libbbbit_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbbbit_cli_a_SOURCES = \
  rpcclient.cpp \
  $(BBBIT_CORE_H)

nodist_libbbbit_util_a_SOURCES = $(srcdir)/obj/build.h
#

# bbbitd binary #
bbbitd_SOURCES = bbbitd.cpp
bbbitd_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES)
bbbitd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
bbbitd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
bbbitd_SOURCES += bbbitd-res.rc
endif

bbbitd_LDADD = \
  $(LIBBBBIT_SERVER) \
  $(LIBBBBIT_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBBBBIT_UTIL) \
  $(LIBBBBIT_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_ZMQ
bbbitd_LDADD += $(LIBBBBIT_ZMQ) $(ZMQ_LIBS)
endif

if ENABLE_WALLET
bbbitd_LDADD += libbbbit_wallet.a
endif

bbbitd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)

# bbbit-cli binary #
bbbit_cli_SOURCES = bbbit-cli.cpp
bbbit_cli_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES) $(EVENT_CFLAGS)
bbbit_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
bbbit_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

# wallet-utility binary #
if ENABLE_WALLET
wallet_utility_SOURCES = wallet-utility.cpp
wallet_utility_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES) $(EVENT_CFLAG)
wallet_utility_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
wallet_utility_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)
endif

if TARGET_WINDOWS
bbbit_cli_SOURCES += bbbit-cli-res.rc
endif

bbbit_cli_LDADD = \
  $(LIBBBBIT_CLI) \
  $(LIBUNIVALUE) \
  $(LIBBBBIT_UTIL)

bbbit_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
if ENABLE_WALLET
wallet_utility_LDADD = libbbbit_wallet.a $(LIBBBBIT_COMMON) $(LIBBBBIT_CRYPTO) $(LIBSECP256K1) $(LIBBBBIT_UTIL) $(BOOST_LIBS) $(BDB_LIBS) $(CRYPTO_LIBS)
endif

#

# bbbit-tx binary #
bbbit_tx_SOURCES = bbbit-tx.cpp
bbbit_tx_CPPFLAGS = $(AM_CPPFLAGS) $(BBBIT_INCLUDES)
bbbit_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
bbbit_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
bbbit_tx_SOURCES += bbbit-tx-res.rc
endif

bbbit_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBBBBIT_COMMON) \
  $(LIBBBBIT_UTIL) \
  $(LIBBBBIT_CRYPTO) \
  $(LIBSECP256K1)

bbbit_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# bbbitconsensus library #
if BUILD_BBBIT_LIBS
include_HEADERS = script/bbbitconsensus.h
libbbbitconsensus_la_SOURCES = \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  hash.cpp \
  primitives/transaction.cpp \
  pubkey.cpp \
  script/bbbitconsensus.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libbbbitconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libbbbitconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libbbbitconsensus_la_LIBADD = $(LIBSECP256K1)
libbbbitconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_BBBIT_INTERNAL
libbbbitconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
