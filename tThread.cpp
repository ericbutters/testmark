#include <stdio.h>
#include "tThreads.hpp"

/*
tThreads::tThreads(int aff, void *p) {
    fprintf(stderr, "%s \n", __func__);
    if(pthread_create(&m_t, NULL, (this->tT), p)) {
        fprintf(stderr, "Error creating thread\n");
    }
}

static void* tThreads::tT(void *p) {
    fprintf(stderr, "%s \n", __func__);
    return NULL;
}

int tThreads::tJoin(){
    return pthread_join(m_t,NULL);
}
*/
