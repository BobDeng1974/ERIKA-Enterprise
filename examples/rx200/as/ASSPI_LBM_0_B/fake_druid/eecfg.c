#include "ee.h"



/***************************************************************************
 *
 * Kernel ( CPU 0 )
 *
 **************************************************************************/
    /* Definition of task's body */
    DeclareTask(TaskJobError);
    DeclareTask(TaskJobOk);
    DeclareTask(TaskSeqError);
    DeclareTask(TaskSeqOk);

    const EE_THREAD_PTR EE_hal_thread_body[EE_MAX_TASK] = {
        &FuncTaskJobError,		/* thread TaskJobError */
        &FuncTaskJobOk,		/* thread TaskJobOk */
        &FuncTaskSeqError,		/* thread TaskSeqError */
        &FuncTaskSeqOk 		/* thread TaskSeqOk */

    };

    /* ready priority */
    const EE_TYPEPRIO EE_th_ready_prio[EE_MAX_TASK] = {
        0x2U,		/* thread TaskJobError */
        0x1U,		/* thread TaskJobOk */
        0x2U,		/* thread TaskSeqError */
        0x1U 		/* thread TaskSeqOk */
    };

    /* dispatch priority */
    const EE_TYPEPRIO EE_th_dispatch_prio[EE_MAX_TASK] = {
        0x2U,		/* thread TaskJobError */
        0x1U,		/* thread TaskJobOk */
        0x2U,		/* thread TaskSeqError */
        0x1U 		/* thread TaskSeqOk */
    };

    /* thread status */
    #if defined(__MULTI__) || defined(__WITH_STATUS__)
        EE_TYPESTATUS EE_th_status[EE_MAX_TASK] = {
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY
        };
    #endif

    /* next thread */
    EE_TID EE_th_next[EE_MAX_TASK] = {
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL
    };

    EE_TYPEPRIO EE_th_nact[EE_MAX_TASK];
    /* The first stacked task */
    EE_TID EE_stkfirst = EE_NIL;

    /* The first task into the ready queue */
    EE_TID EE_rqfirst  = EE_NIL;

    /* system ceiling */
    EE_TYPEPRIO EE_sys_ceiling= 0x0000U;

