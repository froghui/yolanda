#include <assert.h>
#include "channel_map.h"


int map_make_space(struct channel_map *map, int slot, int msize) {
    if (map->nentries <= slot) {
        int nentries = map->nentries ? map->nentries : 32;
        void **tmp;

        while (nentries <= slot)
            nentries <<= 1;

        tmp = (void **) realloc(map->entries, nentries * msize);
        if (tmp == NULL)
            return (-1);

        memset(&tmp[map->nentries], 0,
               (nentries - map->nentries) * msize);

        map->nentries = nentries;
        map->entries = tmp;
    }

    return (0);
}

void map_init(struct channel_map *map) {
    map->nentries = 0;
    map->entries = NULL;
}

void map_clear(struct channel_map *map) {
    if (map->entries != NULL) {
        int i;
        for (i = 0; i < map->nentries; ++i) {
            if (map->entries[i] != NULL)
                free(map->entries[i]);
        }
        free(map->entries);
        map->entries = NULL;
    }
    map->nentries = 0;
}


