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
    cpu_set_t *cpuset = (cpu_set_t*) p;
    struct timespec ts = {0,0};
    struct timespec te = {0,0};

#ifdef USE_AFFIN
    if(0 != pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), cpuset))
        fprintf(stderr,"%s: ERROR \n",__func__);
#endif
    clock_gettime(CLOCK_MONOTONIC, &ts);
    //DO WORK
    naive_ackermann(4,1);
    clock_gettime(CLOCK_MONOTONIC, &te);

    unsigned int msec = (te.tv_sec-ts.tv_sec)*1000 + (te.tv_nsec-ts.tv_nsec)/(1000*1000);
    fprintf(stderr,"%s %d ms \n",__func__,msec);
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
    cpu_set_t cpuset0;
    CPU_ZERO(&cpuset0);
    CPU_SET(0, &cpuset0);
    CPU_SET(2, &cpuset0);
    cpu_set_t cpuset1;
    CPU_ZERO(&cpuset1);
    //CPU_SET(1, &cpuset1);
    CPU_SET(3, &cpuset1);
    clock_gettime(CLOCK_MONOTONIC, &ts);
    pthread_create(&g_t0,NULL,&tT,&cpuset0);
    pthread_create(&g_t1,NULL,&tT,&cpuset1);
    pthread_join(g_t0,NULL);
    pthread_join(g_t1,NULL);
    clock_gettime(CLOCK_MONOTONIC, &te);
    unsigned int msec = (te.tv_sec-ts.tv_sec)*1000 + (te.tv_nsec-ts.tv_nsec)/(1000*1000);
    fprintf(stderr,"%s %d ms\n",__func__,msec);
//    return app.exec();
}
