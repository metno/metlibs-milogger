#ifndef METLIBS_MILOGGER_VERSION_H
#define METLIBS_MILOGGER_VERSION_H

#define METLIBS_MILOGGER_VERSION_MAJOR 5
#define METLIBS_MILOGGER_VERSION_MINOR 1
#define METLIBS_MILOGGER_VERSION_PATCH 0

#define METLIBS_MILOGGER_VERSION_INT(major,minor,patch) \
    (1000000*major + 1000*minor + patch)
#define METLIBS_MILOGGER_VERSION_CURRENT_INT \
    METLIBS_MILOGGER_VERSION_INT(METLIBS_MILOGGER_VERSION_MAJOR, METLIBS_MILOGGER_VERSION_MINOR, METLIBS_MILOGGER_VERSION_PATCH)

#endif // METLIBS_MILOGGER_VERSION_H
