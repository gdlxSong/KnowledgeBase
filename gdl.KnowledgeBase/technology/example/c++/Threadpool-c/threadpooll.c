
#include "threadpool.h"

#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#include <pthread.h>
#include <errno.h>
#endif

#include <assert.h>
#ifdef _WIN32
#include <process.h>


#define FOREVER    -1
#define _thread_t  HANDLE
#define _thread_cond_t  CONDITION_VARIABLE
#define _thread_mutex_t CRITICAL_SECTION

#define tp_lock(pool) EnterCriticalSection(&(pool)->lock)
#define tp_unlock(pool) LeaveCriticalSection(&(pool)->lock)
#define tp_wake_one(cv) WakeConditionVariable(&(cv))
#define tp_wake_all(cv) WakeAllConditionVariable(&(cv))
#define tp_wait_cv(pool, cv, time) SleepConditionVariableCS(&(cv), &(pool)->lock, (time))

#define get_current_thread_id  GetCurrentThreadId

#define tp_get_last_error()   GetLastError()


//#define tp_syn_set(nptr, num) InterlockedExchange
//#define tp_syn_cmpset(nptr, cmp, num) InterlockedCompareExchange
#define tp_syn_add(nptr, num)   InterlockedExchangeAdd(nptr, num)
#else
#include <pthread.h>

static struct timespec gForever = { -1, -1 };
#define FOREVER &gForever
#define _thread_t  pthread_t
#define _thread_cond_t  pthread_cond_t
#define _thread_mutex_t pthread_mutex_t

#define tp_lock(pool) pthread_mutex_lock(&(pool)->lock)
#define tp_unlock(pool) pthread_mutex_unlock(&(pool)->lock)
#define tp_wake_one(cv) pthread_cond_signal(&(cv))
#define tp_wake_all(cv) pthread_cond_broadcast(&(cv))

#define tp_wait_cv(pool, cv, ti) do{      \
    struct timespec abstime = { 0 };        \
    abstime.tv_nsec = ((long)((ti) % 1000) * 1000000);        \
    abstime.tv_sec = ((time_t)(time(NULL) + (ti) / 1000));     \
    pthread_cond_timedwait(&(cv), &(pool)->lock, &abstime);        \
}while(0)

#define get_current_thread_id  pthread_self

#define tp_get_last_error()   errno

#define tp_syn_add(nptr, num)   __sync_add_and_fetch(nptr, num)



#endif


typedef unsigned long thread_ulong_t;
typedef int thread_bool_t;


typedef struct __HEAPELEMENT
{
    time_t dwValue;
    thread_ulong_t * pdwIndex;
    void * pObject;
}HEAPELEMENT;

typedef struct __MINHEAP
{
    thread_ulong_t dwIncrement;//�ռ䲻��ʱ,ÿ�ε������ٿռ�
    thread_ulong_t dwCount;//Ԫ������
    HEAPELEMENT * lpArrayElement;
}MINHEAP;

typedef struct __JOB
{
    thread_bool_t priority;//�Ƿ����ȴ���
    queue_hook_cb callback;//�ص�
    void * obj;//����һ��������ΪC++ thisָ��
    void * arg;//��������������ΪC++ class��Ա��������
    struct __JOB * next;
}JOB, *PJOB;

typedef struct __POOLTIMER
{
    thread_ulong_t index;//С���ѵ�λ��
    thread_ulong_t ref;//���ü���

    thread_ulong_t threadid;

    //��Ҫһ���¼�(HEVENT)����������������֤���ݵ��̰߳�ȫ,��Ϊ��timerִ��ʱ,��������
    //�����ʱ�����ͷŶ�ʱ���Ļ�,�ͻ��������,���Ե���ʱ����ʼʱ,������HEVENT
    //Ϊ���ź�, ��ʱ��ִ����Ϻ�,����Ϊ���ź�
    //void * signal;
    _thread_cond_t cv_kill;
    time_t timeout;//����
    timer_hook_cb callback;
    long event;
    void * arg;
}POOLTIMER;

typedef struct __THREADPOOL{
    int thread_num;//��פ�߳���
    int thread_max_num;//����߳���
    int thread_cur_num;//��ǰ�߳���
    int queue_cur_num;//��ǰ������Ϣ��
    int queue_max_num;//��������Ϣ��
    thread_bool_t thread_work;//�߳��Ƿ��������
    thread_bool_t pool_close;//���ڹر��̳߳�
    _thread_t * pthreads;//�߳̾������
    thread_bool_t block_full;//Ĭ��TRUE ������ʱ,�����ȴ�. FALSE ʧ���˳�

    _thread_mutex_t lock;//��Ϣ��д��
    _thread_cond_t cv_not_empty;//������Ϣ�ǿ�
    _thread_cond_t cv_not_full;//������Ϣδ��
    _thread_cond_t cv_destroy;//ע��֪ͨ
    _thread_cond_t cv_create_thread;//�ȴ��̴߳������
    struct __JOB * queue_frist;//�����е�һ����Ϣ
    struct __JOB * queue_last;//���������һ����Ϣ

    //���ȶ����е����һ����Ϣλ��,�����������Ե���Ϣ,����queue_frist��ʼ����
    struct __JOB * queue_priority_last;

    JOB * job_mempool;//�����ڴ��,�����ظ������ڴ�
    struct __JOB * cache_frist;//��������һ��

    MINHEAP * time_heap;//timerС����

#define MAX_TIMER_COUNT   16
    POOLTIMER thread_timer[MAX_TIMER_COUNT];//��ඨ��N��timer
    int timer_count;//��ʱ������
    queue_hook_cb stable_cb;//�̶��Ļص�,����
};

static void empty(void * p, int size)
{
    while (size--)
    {
        ((char*)p)[size] = '\0';
    }
}

static time_t get_time()
{
    time_t ti = 0;
#ifdef _WIN32

    QueryPerformanceCounter((LARGE_INTEGER*)&ti);
    ti = ti / 1000;

#else

    struct timeval tival = { 0 };
    gettimeofday(&tival, NULL);

    ti += tival.tv_sec * 1000;
    ti += tival.tv_usec / 1000;
#endif

    return ti;
}

/*
������timer
*/

#define SET_ELEM_INDEX(pElem, Value)  ((pElem)->pdwIndex ? *(pElem)->pdwIndex = Value : 0)
#define UPDATE_ELEM_INDEX(lpElemArray, Index)  ((lpElemArray)[Index].pdwIndex ? *(lpElemArray)[Index].pdwIndex = (Index) : 0)

static MINHEAP * min_heap_new_(thread_ulong_t dwIncrement)
{
    if (0 == dwIncrement) return NULL;
    MINHEAP * pMinHeap = (MINHEAP *)malloc(sizeof(MINHEAP));
    memset(pMinHeap, 0, sizeof(MINHEAP));
    pMinHeap->dwIncrement = dwIncrement;

    pMinHeap->lpArrayElement = (HEAPELEMENT *)malloc(sizeof(HEAPELEMENT) * dwIncrement);

    if (NULL == pMinHeap->lpArrayElement)
    {
        free(pMinHeap);
        pMinHeap = NULL;
    }
    return pMinHeap;
}

static void min_heap_free_(MINHEAP * pMinHeap)
{
    if (pMinHeap->lpArrayElement)
    {
        free(pMinHeap->lpArrayElement);
    }
    free(pMinHeap);
}

static __inline thread_ulong_t min_heap_get_count_(MINHEAP * pMinHeap)
{
    return pMinHeap->dwCount;
}

static void min_heap_clear_(MINHEAP * pMinHeap)
{
    if (pMinHeap->dwCount > pMinHeap->dwIncrement)
    {
        //��Ҫ��С
        //��������С, ������һ���ɹ���
        pMinHeap->lpArrayElement = (HEAPELEMENT*)realloc(pMinHeap->lpArrayElement, pMinHeap->dwIncrement * sizeof(HEAPELEMENT));
        assert(pMinHeap->lpArrayElement && "min_heap_clear ��������Ԥ�ϵ����!");
    }

    pMinHeap->dwCount = 0;
    memset(pMinHeap->lpArrayElement, 0, pMinHeap->dwIncrement * sizeof(HEAPELEMENT));
}

static thread_bool_t min_heap_top_(MINHEAP * pMinHeap, HEAPELEMENT * pElem)
{
    if (NULL == pMinHeap || NULL == pElem)
    {
        assert(0 && "min_heap_top ������Ч��ָ��, ����!");
        return FALSE;
    }

    if (0 == pMinHeap->dwCount) return FALSE;

    *pElem = pMinHeap->lpArrayElement[0];

    return TRUE;
}

static thread_bool_t min_heap_push_(MINHEAP * pMinHeap, const HEAPELEMENT * pElem)
{
    thread_ulong_t dwBytes = 0;
    HEAPELEMENT * lpArray = NULL;
    thread_ulong_t nIndex = 0;
    thread_ulong_t j = 0;
    if (NULL == pMinHeap || NULL == pElem)
    {
        assert(0 && "min_heap_push ������Ч��ָ��, ����!");
        return FALSE;
    }

    if (pElem->pdwIndex && -1 != *pElem->pdwIndex)
    {
        assert(0 && "min_heap_push Ԫ�ؿ����Ѿ������ڶ���, ����!");
        return FALSE;
    }

    if (pMinHeap->dwCount > 0 && 0 == pMinHeap->dwCount % pMinHeap->dwIncrement)
    {
        //�ڴ�պù���, ���ڻ�Ҫ����һ��, �Ǿ���Ҫ���·���ռ�
        dwBytes = sizeof(HEAPELEMENT) * (pMinHeap->dwCount + pMinHeap->dwIncrement);
        if (NULL == (lpArray = (HEAPELEMENT*)realloc(pMinHeap->lpArrayElement, dwBytes)))
        {
            return FALSE;
        }

        pMinHeap->lpArrayElement = lpArray;
        memset(pMinHeap->lpArrayElement + pMinHeap->dwCount, 0, sizeof(HEAPELEMENT) * pMinHeap->dwIncrement);
    }

    //�ڴ������Ѵ����, ���濪ʼ��ʽ����

    lpArray = pMinHeap->lpArrayElement;
    nIndex = pMinHeap->dwCount;//nIndexָ�������Ԫ�ص�λ�ã����������±꣬��ʼָ����Ԫ�����ڵĶ�βλ��

    lpArray[pMinHeap->dwCount++] = *pElem;//���β�����Ԫ��, ���������鳤��

    while (0 != nIndex)
    {
        j = (nIndex - 1) / 2; //jָ���±�ΪnIndex��Ԫ�ص�˫��
        if (pElem->dwValue >= lpArray[j].dwValue) //����Ԫ�ش��ڴ�����Ԫ�ص�˫�ף���Ƚϵ����������˳�ѭ��
            break;

        lpArray[nIndex] = lpArray[j]; //��˫��Ԫ�����Ƶ�������Ԫ�ص�λ��

        UPDATE_ELEM_INDEX(lpArray, nIndex);
        nIndex = j; //ʹ������λ�ñ�Ϊ��˫��λ�ã�������һ��ѭ��
    }
    lpArray[nIndex] = *pElem;//����Ԫ�ص���������λ��
    UPDATE_ELEM_INDEX(lpArray, nIndex);

    return TRUE;
}

static thread_bool_t min_heap_erase_(MINHEAP * pMinHeap, ULONG uIndex)
{
    thread_ulong_t dwBytes = 0;
    HEAPELEMENT * lpArray = NULL;
    //unsigned long uIndex = 0;//��nIndexָ�������Ԫ�ص�λ�ã���ʼָ��Ѷ�λ��
    unsigned long j = 0;//j��Ĭ��ָ�����, �����߱��ұߴ�,��ôָ���ұ�
    HEAPELEMENT Element;
    thread_bool_t bReAlloc = FALSE;

    if (NULL == pMinHeap)
    {
        assert(0 && "min_heap_erase ������Ч��ָ��, ����!");
        return FALSE;
    }

    if (uIndex > pMinHeap->dwCount) return FALSE;

    lpArray = pMinHeap->lpArrayElement;
    SET_ELEM_INDEX(&lpArray[uIndex], -1);//�Ѿ����Ƴ�

    if (0 == --pMinHeap->dwCount) return TRUE;

    Element = lpArray[pMinHeap->dwCount]; //����������ԭ��βԪ���ݴ�temp�У��Ա��������λ��
    j = 2 * uIndex + 1;//��jָ��nIndex������λ�ã���ʼָ���±�Ϊ1��λ��

    while (pMinHeap->dwCount - 1 >= j)//Ѱ�Ҵ�����Ԫ�ص�����λ�ã�ÿ��ʹ����Ԫ������һ�㣬����������Ϊ��ʱֹ
    {
        //�������Һ����ҽ�С��ʹjָ���Һ���
        if (pMinHeap->dwCount - 1 > j && lpArray[j].dwValue > lpArray[j + 1].dwValue)//����Ҵ�
            j++;//ָ���ұ�, ָ��С��λ��

        if (lpArray[j].dwValue >= Element.dwValue) //��temp�����С�ĺ��ӻ�С��������������˳�ѭ��
            break;

        lpArray[uIndex] = lpArray[j];//���򣬽�����Ԫ���Ƶ�˫��λ��
        UPDATE_ELEM_INDEX(lpArray, uIndex);
        uIndex = j; //��������λ�ñ�Ϊ���С�ĺ���λ��
        j = 2 * uIndex + 1;//��j��Ϊ�µĴ�����λ�õ�����λ�ã�������һ��ѭ��
    }

    lpArray[uIndex] = Element;
    UPDATE_ELEM_INDEX(lpArray, uIndex);

    if (pMinHeap->dwCount >= pMinHeap->dwIncrement && 0 == pMinHeap->dwCount % pMinHeap->dwIncrement)
    {
        //��ʱ�ڴ�պö��һ��, ���ڻ��Ƴ�һ��, �Ǿ����·���ռ�, ���ռ�ѹ������С
        dwBytes = sizeof(HEAPELEMENT) * (pMinHeap->dwCount);

        //��Ϊ����С�ռ�, ����realloc�ǲ���ʧ�ܵ�
        pMinHeap->lpArrayElement = (HEAPELEMENT*)realloc(pMinHeap->lpArrayElement, dwBytes);

        assert(lpArray == pMinHeap->lpArrayElement && "min_heap_erase ��������Ԥ�ϵ����!");
    }

    return TRUE;
}

static thread_bool_t min_heap_pop_(MINHEAP * pMinHeap, HEAPELEMENT * pElem)
{
    if (TRUE == min_heap_top_(pMinHeap, pElem))
    {
        return min_heap_erase_(pMinHeap, 0);
    }

    return FALSE;
}

static int min_heap_popbat_(MINHEAP * pMinHeap, thread_ulong_t dwCutValue, HEAPELEMENT * pElem, int nCount)
{
    int nIndex = 0;
    if (NULL == pMinHeap || NULL == pElem)
    {
        assert(0 && "min_heap_popbat ������Ч��ָ��, ����!");
        return -1;
    }

    if (0 == pMinHeap->dwCount) return 0;

    //�����ϴ���������event��ȡ����
    for (nIndex = 0; nCount > nIndex; nIndex++)
    {
        if (pMinHeap->dwCount > 0 && dwCutValue >= pMinHeap->lpArrayElement[0].dwValue)
        {
            min_heap_pop_(pMinHeap, &pElem[nIndex]);
        }
        else
        {
            break;
        }
    }

    return nIndex;
}



/*
������timer������㷨, �������̳߳ص�ִ������
*/


static int threadpool_reset_timer_nolock(THREADPOOL pool, POOLTIMER * timer)
{
    int ret;
    HEAPELEMENT element = { 0 };

    if (TRUE == pool->pool_close)
    {
        return -1;
    }

    if (0 == timer->ref)
    {
        //��ʱ���Ѿ��ͷ�
        return 0;
    }

    if (-1 != timer->index)
    {
        //�ڻص��߳��ͷ�timer�������߳����������timer���ͻᷢ���������
        return 0;
    }

    element.dwValue += timer->timeout + get_time();
    element.pObject = timer;
    element.pdwIndex = &timer->index;

    ret = min_heap_push_(pool->time_heap, &element);
    timer->threadid = 0;

    return ret;
}

static int threadpool_get_timer_nolock(THREADPOOL pool, HEAPELEMENT * timer_array, time_t ti_cache, thread_ulong_t threadid)
{
    int count = 0;
    POOLTIMER * timer = NULL;

    empty(timer_array, sizeof(HEAPELEMENT) * MAX_TIMER_COUNT);
    count = min_heap_popbat_(pool->time_heap, ti_cache, timer_array, MAX_TIMER_COUNT);

    //���õ�ǰ�����߳�
    for (int i = 0; count > i; i++)
    {
        timer = timer_array[i].pObject;
        timer->threadid = threadid;
    }

    return count;
}


static JOB * alloc_job(THREADPOOL pool)
{
    JOB * job = NULL;

    assert(pool->cache_frist && "thread_pool ������в�����Ϊ�գ�");

    job = pool->cache_frist;
    pool->cache_frist = pool->cache_frist->next;

    return job;
}

static void free_job(THREADPOOL pool, JOB * job)
{
    job->next = pool->cache_frist;
    pool->cache_frist = job;
}


static unsigned int TPSTDCALL threadpool_worker(void * arg)
{
    THREADPOOL pool = (THREADPOOL)arg;

    JOB * job = NULL;
    POOLTIMER * timer = NULL;
    HEAPELEMENT * timer_array = (HEAPELEMENT*)malloc(sizeof(HEAPELEMENT) * MAX_TIMER_COUNT);
    assert(timer_array && "threadpool_function �������ڴ�������!");

    int timer_count = 0;//������timer����

    HEAPELEMENT element = { 0 };
    time_t ti_cache = 0;
    thread_ulong_t timeout = -1;
    thread_ulong_t last_error = 0;
    int handle_count = 0;//�����ۼ�

    thread_ulong_t threadid = get_current_thread_id();
    thread_bool_t is_permanent_thread = TRUE;//�Ƿ�פ�߳�

    tp_lock(pool);
    if (pool->thread_num)
    {
        is_permanent_thread = pool->thread_num > pool->thread_cur_num ? TRUE : FALSE;
        tp_wake_one(pool->cv_create_thread);
    }
    tp_unlock(pool);

    while (pool->thread_work)
    {
    BEGIN:
        timeout = -1;
        timer_count = 0;

        tp_lock(pool);
        //ͨ��iotime.key.data�Ƿ�ΪNULL, ����֪��һ����ʱ���Ƿ񱻴���
        //������Ļ�����ȡһ���µ�

        if (pool->timer_count && handle_count > pool->thread_cur_num && pool->queue_cur_num)
        {
            //��ʱ�����ȼ��ܵͣ�������ܶ��ʱ������ÿ��������߳��������ϵ�������м��һ�ζ�ʱ��
            if (TRUE == min_heap_top_(pool->time_heap, &element))
            {
                ti_cache = get_time();
                if (ti_cache >= element.dwValue)//ʱ�䵽
                {
                    if (timer_count = threadpool_get_timer_nolock(pool, timer_array, ti_cache, threadid))
                    {
                        goto FINISH;
                    }
                }
            }
        }

        while (0 == pool->queue_cur_num)
        {
            if (FALSE == is_permanent_thread || FALSE == pool->thread_work)
            {
                pool->thread_cur_num--;
                tp_unlock(pool);
                goto BYEBYE;
            }
            else if (TRUE == min_heap_top_(pool->time_heap, &element))
            {
                ti_cache = get_time();
                timeout = (thread_ulong_t)max(0, element.dwValue - ti_cache);

                if (0 == timeout)//ʱ�䵽
                {
                    if (timer_count = threadpool_get_timer_nolock(pool, timer_array, ti_cache, threadid))
                    {
                        goto FINISH;
                    }
                }
            }

            if (FALSE == tp_wait_cv(pool, pool->cv_not_empty, timeout))
            {
                last_error = tp_get_last_error();
                if (ERROR_TIMEOUT == last_error || WAIT_TIMEOUT == last_error)
                {
                    ti_cache = get_time();
                    if (timer_count = threadpool_get_timer_nolock(pool, timer_array, ti_cache, threadid))
                    {
                        goto FINISH;
                    }
                }
                tp_unlock(pool);
                fprintf(stderr, "threadpool_function δ֪��last error = %d", last_error);
                goto BEGIN;
            }
        }

        job = pool->queue_frist;

        if (pool->queue_frist == pool->queue_priority_last)
        {
            //���һ����������ȡ��
            pool->queue_priority_last = NULL;
        }

        pool->queue_frist = job->next;
        pool->queue_cur_num--;

        if (0 == pool->queue_cur_num)
        {
            pool->queue_frist = NULL;
            pool->queue_last = NULL;

            //����ִ�����ˣ� ���˳�
            if (TRUE == pool->pool_close)
            {
                pool->thread_work = FALSE;
                tp_wake_one(pool->cv_not_empty);
            }
        }
        else if ((pool->queue_cur_num + 1) == pool->queue_max_num)
        {
            tp_wake_one(pool->cv_not_empty);
            tp_wake_all(pool->cv_not_full);
        }
        else/*not full and not empty*/
        {
            tp_wake_one(pool->cv_not_empty);//֪ͨ����һ���߳�
        }

    FINISH:
        tp_unlock(pool);

        if (timer_count)
        {
            //��������timer
            while (timer_count--)//������ִ��
            {
                timer = timer_array[timer_count].pObject;

                //��һ������Ҫ��arg���ܼ��ݸ������������thisָ��
                timer->callback(timer->arg, timer->event);

                //ͨ��threadidId ���� key ���ж϶����Ƿ���ɾ��,������threadid�ж����
                //���timer��ɾ��������Ϊ�µĶ�ʱ��, threadid������NULL, Ҳ�����������߳�, 
                //���������Ǳ��߳�, ��Ϊ���߳���������

                if (timer->threadid == threadid)//��ʹ���Ҳ���ж���ȷ
                {
                    tp_wake_one(timer->cv_kill);

                    tp_lock(pool);
                    threadpool_reset_timer_nolock(pool, timer);
                    tp_unlock(pool);
                }

                handle_count = 0;//����
            }
        }
        else
        {
            //������������
            handle_count++;//�����ۼ�
            job->callback(job->obj, job->arg);
            tp_lock(pool);
            free_job(pool, job);
            tp_unlock(pool);
        }
    }/*while(pool->thread_work)*/

BYEBYE:
    free(timer_array);
    if (0 == pool->thread_cur_num)//�˴�����Ҫ����
    {
        //�������һ���߳�
        tp_wake_one(pool->cv_destroy);//֪ͨ�ر�
    }
    else
    {
        //�����߳�δ�ر�
        tp_wake_one(pool->cv_not_empty);
    }

    return 0;
}


/*
�������ṩ���ⲿ����Ľӿ�
*/

THREADPOOL threadpool_create(unsigned int thread_num, unsigned int max_thread_num, unsigned int queue_max_num)
{
    THREADPOOL pool = NULL;

    if (thread_num > (((unsigned int)~((unsigned int)0)) >> 1)
        || max_thread_num > (((unsigned int)~((unsigned int)0)) >> 1)
        || queue_max_num > (((unsigned int)~((unsigned int)0)) >> 1)) return NULL;

    pool = (THREADPOOL)malloc(sizeof(struct __THREADPOOL));
    if (NULL == pool) return NULL;
    empty(pool, sizeof(struct __THREADPOOL));

    pool->thread_num = thread_num;
    pool->thread_max_num = (0 == thread_num ? 0 : max(thread_num, max_thread_num));
    pool->thread_cur_num = 0;
    pool->queue_cur_num = 0;
    pool->queue_max_num = queue_max_num;
    pool->queue_frist = NULL;
    pool->queue_last = NULL;
    pool->thread_work = TRUE;
    pool->pool_close = FALSE;
    pool->block_full = TRUE;

    for (int i = 0; MAX_TIMER_COUNT > i; i++)
    {
#ifdef _WIN32
        InitializeConditionVariable(&pool->thread_timer[i].cv_kill);
#else
        pthread_cond_init(&pool->thread_timer[i].cv_kill, NULL);
#endif
    }

    pool->time_heap = min_heap_new_(MAX_TIMER_COUNT + 1);//ûɶ������Ԥ��һ����λ����

    if (NULL == pool->time_heap)
    {
        free(pool);
        return NULL;
    }

    pool->job_mempool = (JOB*)malloc(sizeof(JOB) * (pool->queue_max_num + pool->thread_max_num + 1));

    if (NULL == pool->job_mempool)
    {
        min_heap_free_(pool->time_heap);
        free(pool);
        return NULL;
    }

    //�� 1 ��Ϊ�˼���queue����, ��Ϊqueue��ʱ��poolû�м�¼�߳���, 
    //����ȷʵqueue������ĳ���߳�, ��ռ����һ���ڴ�ռ�
    empty(pool->job_mempool, sizeof(JOB) * (pool->queue_max_num + pool->thread_max_num + 1));

    int queue_count = pool->queue_max_num + pool->thread_max_num;
    pool->cache_frist = pool->job_mempool;
    for (int i = 0; queue_count > i; i++)
    {
        pool->job_mempool[i].next = &pool->job_mempool[i + 1];
    }

#ifdef _WIN32

    InitializeCriticalSection(&pool->lock);//�ٽ���
    InitializeConditionVariable(&pool->cv_not_empty);
    InitializeConditionVariable(&pool->cv_not_full);
    InitializeConditionVariable(&pool->cv_destroy);
    InitializeConditionVariable(&pool->cv_create_thread);
    if (thread_num)
    {
        pool->pthreads = (_thread_t*)malloc(sizeof(_thread_t) * pool->thread_num);

        for (int i = 0; pool->thread_num > i; i++)
        {
            tp_lock(pool);
            //_beginthread���Զ��رվ���� ex����

            pool->pthreads[i] = (_thread_t)_beginthreadex(NULL, 0, threadpool_worker, (LPVOID)pool, 0, 0);

            if (INVALID_HANDLE_VALUE != pool->pthreads[i])
            {
                pool->thread_cur_num++;
                tp_wait_cv(pool, pool->cv_create_thread, FOREVER);
            }

            tp_unlock(pool);
        }
    }

#else
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cv_not_empty, NULL);
    pthread_cond_init(&pool->cv_not_full, NULL);
    pthread_cond_init(&pool->cv_destroy, NULL);
    pthread_cond_init(&pool->cv_create_thread, NULL);
    if (thread_num)
    {
        pool->pthreads = (_thread_t*)malloc(sizeof(_thread_t) * pool->thread_num);

        for (int i = 0; pool->thread_num > i; i++)
        {
            tp_lock(pool);
            if (0 == pthread_create(&pool->pthreads[i], NULL, threadpool_worker, (void *)pool))
            {
                pool->thread_cur_num++;
                tp_wait_cv(pool, pool->cv_create_thread, FOREVER);
            }

            tp_unlock(pool);
        }
    }

#endif

    //���������ȴ��̴߳������, ��ʵ�ܿ��
    return pool;
}

thread_bool_t threadpool_destroy(THREADPOOL pool)
{
    tp_lock(pool);

    if (TRUE == pool->pool_close)//��ֹ��ε���threadpool_destroy����
    {
        tp_unlock(pool);
        return FALSE;
    }

    //if (pool->time_heap) time_heap_clear(pool->time_heap);
    pool->pool_close = TRUE;

    while (pool->thread_cur_num)
    {
        tp_wait_cv(pool, pool->cv_destroy, FOREVER);
    }

    tp_unlock(pool);

#ifdef _WIN32
    if (pool->thread_num)
    {
        //WaitForMultipleObjects(pool->thread_num, pool->pthreads, TRUE, FROEVER);
        for (int i = 0; pool->thread_num > i; i++)
        {
            CloseHandle(pool->pthreads[i]);
        }
    }

    DeleteCriticalSection(&pool->lock);
#else

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cv_not_empty);
    pthread_cond_destroy(&pool->cv_not_full);
    pthread_cond_destroy(&pool->cv_destroy);

#endif

    if (pool->pthreads) free(pool->pthreads);
    if (pool->time_heap) min_heap_free_(pool->time_heap);
    if (pool->job_mempool) free(pool->job_mempool);
    if (pool) free(pool);

    return TRUE;
}

int threadpool_add_job(THREADPOOL pool, thread_bool_t priority, queue_hook_cb callback, void * obj, void *arg)
{
    int ret = 0;
    JOB * newjob = NULL;
    _thread_t tid = 0;

    tp_lock(pool);

    if (FALSE == pool->block_full && pool->queue_cur_num == pool->queue_max_num)
    {
        goto END;
    }

    while (FALSE == pool->pool_close && pool->queue_cur_num == pool->queue_max_num)
    {
        //fprintf(stderr, "threadpool_add_job ���������ﵽ�嶥[%d], ����ȴ�ģʽ", pool->queue_max_num);
        tp_wait_cv(pool, pool->cv_not_full, FOREVER);
        //fprintf(stderr, "threadpool_add_job �ȴ�������, ��ǰ������[%d]", pool->queue_cur_num);
    }

    if (TRUE == pool->pool_close)
    {
        ret = -1;
        goto END;
    }

    newjob = alloc_job(pool);
    newjob->priority = priority;
    newjob->callback = callback;
    newjob->obj = obj;
    newjob->arg = arg;
    newjob->next = NULL;

    if (pool->queue_cur_num == 0)
    {
        pool->queue_frist = newjob;
        pool->queue_last = newjob;
        if (TRUE == priority)//���ȴ���
        {
            pool->queue_priority_last = pool->queue_frist;
        }
        tp_wake_one(pool->cv_not_empty);
    }
    else
    {
        if (TRUE == priority)//���ȴ���ǰ������
        {
            if (NULL == pool->queue_priority_last)
            {
                newjob->next = pool->queue_frist;
                pool->queue_frist = newjob;
            }
            else
            {
                //pool->queue_priority_last->next ָ���������û�����ȼ���
                newjob->next = pool->queue_priority_last->next;//�嵽nextǰ��
                pool->queue_priority_last->next = newjob;//�嵽nextǰ��
            }

            if (pool->queue_last == pool->queue_priority_last)
            {
                //Ŀǰ���е����񶼾������ȼ�,
                //Ҳ����˵pool->queue_lastҲָ������������,������Ҫ�޸�����ֵ
                pool->queue_last = newjob;
            }

            pool->queue_priority_last = newjob;//lastָ���µ�����
        }
        else
        {
            pool->queue_last->next = newjob;
            pool->queue_last = newjob;
        }

    }

    ret = ++pool->queue_cur_num;

    if (pool->queue_cur_num == pool->queue_max_num && pool->thread_max_num > pool->thread_cur_num)
    {

        //fprintf(stderr, "threadpool_add_job ������������[%d], ����һ�����߳�", pool->queue_cur_num);
#ifdef _WIN32

        tid = (_thread_t)_beginthreadex(NULL, 0, threadpool_worker, (LPVOID)pool, 0, 0);

        if (INVALID_HANDLE_VALUE == tid)
        {
            pool->thread_cur_num++;
            CloseHandle(tid);
            tp_wait_cv(pool, pool->cv_destroy, FOREVER);
        }

#else

        if (0 == pthread_create(&tid, NULL, threadpool_worker, (void *)pool))
        {
            pool->thread_cur_num++;
            tp_wait_cv(pool, pool->cv_destroy, FOREVER);
        }
#endif
    }

END:
    tp_unlock(pool);
    return ret;
}

thread_bool_t threadpool_block_full(THREADPOOL pool, thread_bool_t block)
{
    //�������,������
    pool->block_full = block;
    return TRUE;
}

thread_bool_t threadpool_active(THREADPOOL pool)
{
    if (pool)
    {
        //���û���������ⲿ���ö�ʱ���󣬽��޷����е�һ�λ��ѣ����Լ���һ�£�ʹ��ʱ����Ч
        //��ʵһ�㲻̫����ʹ�øú�������Ȼ��Ҳ������loopǰ���ö�ʱ���������Ͳ���Ҫ�ú�����
        tp_wake_one(pool->cv_not_empty);
        return TRUE;
    }
    return FALSE;
}

THREADTIMER threadpool_set_timer(THREADPOOL pool, time_t timeout, timer_hook_cb callback, void * arg, long event)
{
    POOLTIMER * timer = NULL;
    _thread_cond_t cv_kill = { 0 };
    int tid = 0;

    tp_lock(pool);

    //��Ϊ�̳߳��ǲ���Ƶ������ɾ��ʱ����,����������forѭ��,����Ҫ̫��Ч���㷨
    if (FALSE == pool->pool_close && sizeof(pool->thread_timer) > pool->timer_count)
    {
        for (int i = 0; sizeof(pool->thread_timer) > i; i++)
        {
            if (0 == pool->thread_timer[i].ref)
            {
                timer = &pool->thread_timer[i];
                pool->timer_count++;
                tid = i + 1;
                break;
            }
        }
    }

    if (timer)
    {
        timer->timeout = timeout;
        timer->callback = callback;
        timer->event = event;
        timer->arg = arg;
        timer->ref = 1;
        timer->index = -1;
        threadpool_reset_timer_nolock(pool, timer);
    }

    tp_unlock(pool);

    return (THREADTIMER)tid;
}

thread_bool_t threadpool_kill_timer(THREADPOOL pool, THREADTIMER tid)
{
    int ret = FALSE;
    _thread_cond_t cv_kill;
    POOLTIMER * timer = NULL;

    if (tid > MAX_TIMER_COUNT) return FALSE;

    tp_lock(pool);

    timer = &pool->thread_timer[tid - 1];
    if (timer->ref)
    {
        cv_kill = timer->cv_kill;
        if (timer->threadid && timer->threadid != get_current_thread_id())
        {
            //timer�ص��߳���ɾ����ʱ��,��ô�û�Ӧ��֪����α�֤�̰߳�ȫ
            //ֱ��ɾ���Ϳ�����, ����ǵ�ǰ�߳�, ��ôҪ��timer�ص�����,
            //����ɾ����ʱ��
            tp_wait_cv(pool, timer->cv_kill, FOREVER);
        }

        min_heap_erase_(pool->time_heap, timer->index);

        empty(timer, sizeof(POOLTIMER));
        timer->cv_kill = cv_kill;//��ԭһ��,����������

        pool->timer_count--;

        ret = TRUE;
    }

    tp_unlock(pool);

    return ret;
}


/*
��չ�Ķ��нӿ�
*/
HQUEUE queue_create(unsigned int max_message)
{
    return (HQUEUE)threadpool_create(0, 0, max_message);
}

unsigned int queue_dispatch(HQUEUE queue)
{
    ((THREADPOOL)queue)->thread_work = TRUE;
    tp_syn_add(&((THREADPOOL)queue)->thread_cur_num, 1);
    return threadpool_worker((void*)queue);
}

int queue_post(HQUEUE queue, queue_hook_cb callback, void * obj, void *arg)
{
    return threadpool_add_job((THREADPOOL)queue, FALSE, callback, obj, arg);
}

thread_bool_t queue_break(HQUEUE queue)
{
    ((THREADPOOL)queue)->thread_work = FALSE;
    return TRUE;
}

thread_bool_t queue_destroy(HQUEUE queue)
{
    return threadpool_destroy((THREADPOOL)queue);
}

HQTIMER queue_set_timer(HQUEUE queue, time_t timeout, timer_hook_cb callback, void * arg, long event)
{
    return (HQTIMER)threadpool_set_timer((THREADPOOL)queue, timeout, callback, arg, event);
}

thread_bool_t queue_kill_timer(HQUEUE queue, HQTIMER timer)
{
    return threadpool_kill_timer((THREADPOOL)queue, (THREADTIMER)timer);
}