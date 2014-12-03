#include <QApplication>
#include <QQmlApplicationEngine>
#include <stdio.h>
#include "tThreads.hpp"

pthread_t g_t0;
pthread_t g_t1;

unsigned int naive_ackermann(unsigned int m, unsigned int n) {
    //calls++;
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return naive_ackermann(m - 1, 1);
    else
        return naive_ackermann(m - 1, naive_ackermann(m, n - 1));
}
#define USE_AFFIN
void* tT(void *p) {
    int affin = *((int*) p);
    struct timespec ts = {0,0};
    struct timespec te = {0,0};

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(affin, &cpuset);
#ifdef USE_AFFIN
    if(0 != pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset))
        fprintf(stderr,"%s: ERROR \n",__func__);
#endif
    clock_gettime(CLOCK_MONOTONIC, &ts);
    //DO WORK
    naive_ackermann(4,1);
    clock_gettime(CLOCK_MONOTONIC, &te);

    unsigned int msec = (te.tv_sec-ts.tv_sec)*1000 + (te.tv_nsec-ts.tv_nsec)/(1000*1000);
    fprintf(stderr,"%s %d ms (affin:%d)\n",__func__,msec,affin);
    return NULL;
}

int main(int argc, char *argv[])
{
/*
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
/*
/*
    tThreads *t = new tThreads(0,NULL);
    t->tJoin();
    delete(t);
*/
    struct timespec ts = {0,0};
    struct timespec te = {0,0};
    int affin0=0;
    int affin1=1;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    pthread_create(&g_t0,NULL,&tT,&affin0);
    pthread_create(&g_t1,NULL,&tT,&affin1);
    pthread_join(g_t0,NULL);
    pthread_join(g_t1,NULL);
    clock_gettime(CLOCK_MONOTONIC, &te);
    unsigned int msec = (te.tv_sec-ts.tv_sec)*1000 + (te.tv_nsec-ts.tv_nsec)/(1000*1000);
    fprintf(stderr,"%s %d ms\n",__func__,msec);
//    return app.exec();
}
