#ifndef CONFIG_H
#define CONFIG_H

#define CORE_DEFAULT_PORT 8815
#define CORE_MYSQL_DEFAULT_PORT 3306
#define LOGGER_DROP_NORMAL_MESSAGE_PREFIX //if defined, messages of level 0 will not have [MSG] prefix
//#define NETWORK_HANDSHAKE_DUMP // will dump all handshake data received to console
//#define NETWORK_DATA_DUMP // will dump all plaintext data received to console

#define QT_NO_CAST_FROM_BYTEARRAY

#endif // CONFIG_H
