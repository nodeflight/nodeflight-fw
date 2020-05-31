#include "disk_access.h"

disk_access_t *disk_access[FF_VOLUMES];

void disk_access_init(
    void)
{
    int i;
    for (i = 0; i < FF_VOLUMES; i++) {
        disk_access[i] = NULL;
    }
}

int disk_access_register(
    int disk_id,
    disk_access_t *dacc)
{
    if (disk_id < 0 || disk_id >= FF_VOLUMES) {
        return -1;
    }
    if (disk_access[disk_id] != NULL) {
        return -1;
    }
    disk_access[disk_id] = dacc;
    return 0;
}

disk_access_t *disk_access_get(
    int disk_id)
{
    if (disk_id < 0 || disk_id >= FF_VOLUMES) {
        return NULL;
    }
    return disk_access[disk_id];
}
