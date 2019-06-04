/*  Alunos: Cláudio Padilha da Silva, RGA: 201719040036
            Caio Riyousuke Miyada Tokunaga, RGA: 201719040028
            Rodrigo Schio Wengenroth Silva, RGA: 201719040010    
           
    Estrutura de diretórios: A pasta raiz é a pagging_simulator. Dentro desta o arquivo simulator.c é a main. É quem controla a execução, chama as 
    threads dos escalonador e do creator e crias as estruturas (disco, memória, processos, timer e a fila). As threads do shiper, do pagger e do timer 
    são criadas internamente (em outras threads). Ainda dentro do diretório pagging_simulator, os componente do sistema (cada estrutura e thread) citados 
    anteriormente foram divididos em 3 categorias (diretórios): hardware, data_structure e software_resource. Cada componente tem o seu diretório contendo o seu 
    arquivo.c e o seu arquivo.h. Os arquivos possuem comentários explicando o funcionamento e características de cada componente.

    Compilação: Dentro do diretório pagging_simulator existe um make file para compilar todos os arquivos e gerar um executável chamado "simulator" dentro do
    próprio diretório. Digit simplesmente make no terminal, dentro do diretório do arquivo make file.

    Execução: Dentro do diretório pagging_simulator após o make, irá aparecer o executável simulator. Digite simplesmente ./simulator no terminal para rodar.

    Entrada: Implementação que recebe números inteiros positivos, separados por espaços como parâmetros de entrada via linha de comando, seguindo as 
    especificações do enunciado do trabalho.
*/ 

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "./hardware/cpu/cpu.h"
#include "./hardware/memory/memory.h"
#include "./hardware/disc/disc.h"
#include "./data_structure/frame/frame.h"
#include "./data_structure/page/page.h"
#include "./data_structure/process/process.h"
#include "./data_structure/queue/queue.h"
#include "./software_resource/creator/creator.h"
#include "./software_resource/pager/pager.h"
#include "./software_resource/scheduler/scheduler.h"
#include "./software_resource/shipper/shipper.h"
#include "./software_resource/timer/timer.h"


int main ()
{
    int seed;
    int nFrames;
    int nProcesses;
    int tq;
    int pid;
    int numPgs;
    int creation;
    int burst;
    process * p = NULL;

    if (scanf("%d %d %d %d", &seed, &nFrames, &nProcesses, &tq))
    {}  // avoids warning

    disc * d = newDisc();
    memory * m = newMemmory(nFrames);
    timerS * t = newTimer(tq, nProcesses);                                                       // Creates disc, memory, ready queue and timer.
    queue * ready = newQueue();

    struct tm * currentTime;
    time_t segundos;
    time(&segundos);   
    currentTime = localtime(&segundos);
    printf("Time: %d:%d:%d - Inicio da observacao.\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);

    schArgs * schAr = newSchArgs (t, ready, m, d, seed);
    pthread_t sch;
    pthread_create(&sch, NULL, roundRobin, (void *) schAr);                                        // Creates RR scheduler.

    for (int i = 0; i < nProcesses; i++)
    {
        if (scanf("%d %d %d %d", &pid, &numPgs, &creation, &burst))
        {} // avoids warning

        p = newProcess (pid, numPgs, creation, burst, d);

        crArgs * crAr = newCrArgs(p, ready);
        pthread_t cr;
        pthread_create(&cr, NULL, creator, (void *) crAr);                                      // Creates one thread for each process. puts process on ready queue.
    }

    pthread_mutex_lock(&t->lock);
    pthread_cond_wait(&t->endCond, &t->lock);                           // Waits for timer signal to end the execution
    pthread_mutex_unlock(&t->lock);


    time(&segundos);   
    currentTime = localtime(&segundos);
    printf("Time: %d:%d:%d - O numero de falha de paginas foi %d.\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, m->pgFaults);

    time(&segundos);   
    currentTime = localtime(&segundos);
    printf("Time: %d:%d:%d - Termino da observacao.\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
    
    return 0;
}