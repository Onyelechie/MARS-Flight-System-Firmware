#ifndef SD_CARD
#define SD_CARD

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_log.h"

class SD_FILESYSTEM {
    public:
        static void SDFS_initialize();
        
        static esp_err_t SDFS_write_file(const char *path, char *data);

        static esp_err_t SDFS__read_file(const char *path);

};

#endif //SD_CARD