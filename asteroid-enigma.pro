TARGET = asteroid-enigma
CONFIG += asteroidapp

SOURCES +=	main.cpp \
			enigma.cpp \
			scrypt/sha256.c \
			scrypt/warnp.c \
			scrypt/crypto_scrypt.c \ 
			arduinolibs/libraries/Crypto/SHA256.cpp \
			arduinolibs/libraries/Crypto/Hash.cpp \
			arduinolibs/libraries/Crypto/BLAKE2b.cpp \
			arduinolibs/libraries/Crypto/Crypto.cpp \
			arduinolibs/libraries/Crypto/HKDF.cpp
			
RESOURCES +=   resources.qrc
OTHER_FILES += main.qml  


lupdate_only{ SOURCES += i18n/asteroid-enigma.desktop.h }
TRANSLATIONS = $$files(i18n/$$TARGET.*.ts)

HEADERS += \
	    enigma.h \ 
	    scrypt/crypto_scrypt.h \
		scrypt/scrypt-kdf.h \
		scrypt/sha256.h \
		scrypt/sysendian.h \
		scrypt/warnp.h \
		arduinolibs/libraries/Crypto/SHA256.h \
		arduinolibs/libraries/Crypto/Hash.h \
		arduinolibs/libraries/Crypto/BLAKE2b.h \
		arduinolibs/libraries/Crypto/Crypto.h \
		arduinolibs/libraries/Crypto/HKDF.h	
		    
CONFIG += qmltypes
QML_IMPORT_NAME = com.usagi.asteroidos.enigma
QML_IMPORT_MAJOR_VERSION = 1


