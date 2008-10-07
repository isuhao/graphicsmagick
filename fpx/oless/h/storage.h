//--------------------------------------------------------------- 
//
//  For conditions of distribution and use, see copyright notice
//  in Flashpix.h 
// 
//  Copyright (c) 1999 Digital Imaging Group, Inc.
// 
//  Contents:   This is the main file to include to use the reference
//              implementation.
//
//  Note:       Some of the defintions have been changed to TCHAR
//              instead of WCHAR, to add support for ANSI APIs.
//              TCHAR becomes WCHAR with _UNICODE defined, and char
//              otherwise. (See tchar.h for details).
//
//--------------------------------------------------------------- 


#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "ref.hxx"
#include "tchar.h"


/****** Storage Error Codes *************************************************/

// These are now coming from error.hxx

/****** Storage types *******************************************************/

#ifndef HUGEP
#define HUGEP
#endif

#define CWCSTORAGENAME 32

/* Storage instantiation modes */
#define STGM_DIRECT   0x00000000L

/* not used in ref, but defined here for error checking etc. */
#define STGM_TRANSACTED   0x00010000L
#define STGM_SIMPLE             0x08000000L
#define STGM_NOSCRATCH          0x00100000L

#define STGM_READ   0x00000000L
#define STGM_WRITE    0x00000001L
#define STGM_READWRITE    0x00000002L

#define STGM_SHARE_DENY_NONE  0x00000040L
#define STGM_SHARE_DENY_READ  0x00000030L
#define STGM_SHARE_DENY_WRITE 0x00000020L
#define STGM_SHARE_EXCLUSIVE  0x00000010L

#define STGM_PRIORITY   0x00040000L
#define STGM_DELETEONRELEASE  0x04000000L

#define STGM_CREATE   0x00001000L
#define STGM_CONVERT    0x00020000L
#define STGM_FAILIFTHERE  0x00000000L

/* Storage commit types */
typedef enum tagSTGC
{
    STGC_DEFAULT = 0,
    STGC_OVERWRITE  = 1,
    STGC_ONLYIFCURRENT  = 2,
    STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE = 4
} STGC;

/* Stream name block definitions */
typedef TCHAR FAR* FAR* SNB;

#ifndef _FILETIME_
#define _FILETIME_
typedef struct FARSTRUCT tagFILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;
#endif

/* Storage stat buffer */

typedef struct FARSTRUCT tagSTATSTG
{
    TCHAR FAR* pwcsName;
    DWORD type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    DWORD grfMode;
    DWORD grfLocksSupported;
    CLSID clsid;
    DWORD grfStateBits;
    DWORD reserved;
} STATSTG;


/* Storage element types */
typedef enum tagSTGTY
{
    STGTY_STORAGE   = 1,
    STGTY_STREAM    = 2,
    STGTY_LOCKBYTES = 3,
    STGTY_PROPERTY  = 4
} STGTY;

typedef enum tagSTREAM_SEEK
{
    STREAM_SEEK_SET = 0,
    STREAM_SEEK_CUR = 1,
    STREAM_SEEK_END = 2
} STREAM_SEEK;

typedef enum tagLOCKTYPE
{
    LOCK_WRITE      = 1,
    LOCK_EXCLUSIVE  = 2,
    LOCK_ONLYONCE   = 4
} LOCKTYPE;

typedef enum tagSTGMOVE
{
    STGMOVE_MOVE    = 0,
    STGMOVE_COPY    = 1
} STGMOVE;

typedef enum tagSTATFLAG
{
    STATFLAG_DEFAULT = 0,
    STATFLAG_NONAME = 1
} STATFLAG;


/****** Storage Enumerators *************************************************/

#define LPENUMSTATSTG        IEnumSTATSTG FAR*

#undef  INTERFACE
#define INTERFACE   IEnumSTATSTG

DECLARE_INTERFACE_(IEnumSTATSTG, IUnknown)
{
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    /* *** IENUMSTATSTG methods *** */
    STDMETHOD(Next) (THIS_ ULONG celt, STATSTG FAR * rgelt, ULONG FAR *pceltFetched) PURE;
    STDMETHOD(Skip) (THIS_ ULONG celt) PURE;
    STDMETHOD(Reset) (THIS) PURE;
    STDMETHOD(Clone) (THIS_ IEnumSTATSTG FAR *FAR *ppenm) PURE;
};


/****** ILockBytes Interface ************************************************/

#define LPLOCKBYTES     ILockBytes FAR*

#undef  INTERFACE
#define INTERFACE   ILockBytes

DECLARE_INTERFACE_(ILockBytes, IUnknown)
{
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    /* *** ILockBytes methods *** */
    STDMETHOD(ReadAt) (THIS_ ULARGE_INTEGER ulOffset,
             VOID HUGEP *pv,
             ULONG cb,
             ULONG FAR *pcbRead) PURE;
    STDMETHOD(WriteAt) (THIS_ ULARGE_INTEGER ulOffset,
              VOID const HUGEP *pv,
              ULONG cb,
              ULONG FAR *pcbWritten) PURE;
    STDMETHOD(Flush) (THIS) PURE;
    STDMETHOD(SetSize) (THIS_ ULARGE_INTEGER cb) PURE;
    STDMETHOD(LockRegion) (THIS_ ULARGE_INTEGER libOffset,
                 ULARGE_INTEGER cb,
                 DWORD dwLockType) PURE;
    STDMETHOD(UnlockRegion) (THIS_ ULARGE_INTEGER libOffset,
                   ULARGE_INTEGER cb,
                 DWORD dwLockType) PURE;
    STDMETHOD(Stat) (THIS_ STATSTG FAR *pstatstg, DWORD grfStatFlag) PURE;
};


/****** IStream Interface ***************************************************/


#define LPSTREAM        IStream FAR*

#undef  INTERFACE
#define INTERFACE   IStream

DECLARE_INTERFACE_(IStream, IUnknown)
{
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    /* *** IStream methods *** */
    STDMETHOD(Read) (THIS_ VOID HUGEP *pv,
         ULONG cb, ULONG FAR *pcbRead) PURE;
    STDMETHOD(Write) (THIS_ VOID const HUGEP *pv,
            ULONG cb,
            ULONG FAR *pcbWritten) PURE;
    STDMETHOD(Seek) (THIS_ LARGE_INTEGER dlibMove,
               DWORD dwOrigin,
               ULARGE_INTEGER FAR *plibNewPosition) PURE;
    STDMETHOD(SetSize) (THIS_ ULARGE_INTEGER libNewSize) PURE;
    STDMETHOD(CopyTo) (THIS_ IStream FAR *pstm,
             ULARGE_INTEGER cb,
             ULARGE_INTEGER FAR *pcbRead,
             ULARGE_INTEGER FAR *pcbWritten) PURE;
    STDMETHOD(Commit) (THIS_ DWORD grfCommitFlags) PURE;
    STDMETHOD(Revert) (THIS) PURE;
    STDMETHOD(LockRegion) (THIS_ ULARGE_INTEGER libOffset,
                 ULARGE_INTEGER cb,
                 DWORD dwLockType) PURE;
    STDMETHOD(UnlockRegion) (THIS_ ULARGE_INTEGER libOffset,
                 ULARGE_INTEGER cb,
                 DWORD dwLockType) PURE;
    STDMETHOD(Stat) (THIS_ STATSTG FAR *pstatstg, DWORD grfStatFlag) PURE;
    STDMETHOD(Clone)(THIS_ IStream FAR * FAR *ppstm) PURE;
};


/****** IStorage Interface **************************************************/

#define LPSTORAGE       IStorage FAR*

#undef  INTERFACE
#define INTERFACE   IStorage

DECLARE_INTERFACE_(IStorage, IUnknown)
{
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    /* *** IStorage methods *** */
    STDMETHOD(CreateStream) (THIS_ const TCHAR FAR* pwcsName,
                   DWORD grfMode,
                   DWORD reserved1,
                   DWORD reserved2,
                   IStream FAR *FAR *ppstm) PURE;
    STDMETHOD(OpenStream) (THIS_ const TCHAR FAR* pwcsName,
     void FAR *reserved1,
                 DWORD grfMode,
                 DWORD reserved2,
                 IStream FAR *FAR *ppstm) PURE;
    STDMETHOD(CreateStorage) (THIS_ const TCHAR FAR* pwcsName,
                DWORD grfMode,
                DWORD reserved1,
                DWORD reserved2,
                IStorage FAR *FAR *ppstg) PURE;
    STDMETHOD(OpenStorage) (THIS_ const TCHAR FAR* pwcsName,
                  IStorage FAR *pstgPriority,
                  DWORD grfMode,
                  SNB snbExclude,
                  DWORD reserved,
                  IStorage FAR *FAR *ppstg) PURE;
    STDMETHOD(CopyTo) (THIS_ DWORD ciidExclude,
           IID const FAR *rgiidExclude,
           SNB snbExclude,
           IStorage FAR *pstgDest) PURE;
    STDMETHOD(MoveElementTo) (THIS_ TCHAR const FAR* lpszName,
                IStorage FAR *pstgDest,
                              TCHAR const FAR* lpszNewName,
                              DWORD grfFlags) PURE;
    STDMETHOD(Commit) (THIS_ DWORD grfCommitFlags) PURE;
    STDMETHOD(Revert) (THIS) PURE;
    STDMETHOD(EnumElements) (THIS_ DWORD reserved1,
                 void FAR *reserved2,
                 DWORD reserved3,
                 IEnumSTATSTG FAR *FAR *ppenm) PURE;
    STDMETHOD(DestroyElement) (THIS_ const TCHAR FAR* pwcsName) PURE;
    STDMETHOD(RenameElement) (THIS_ const TCHAR FAR* pwcsOldName,
                const TCHAR FAR* pwcsNewName) PURE;
    STDMETHOD(SetElementTimes) (THIS_ const TCHAR FAR *lpszName,
                  FILETIME const FAR *pctime,
                                FILETIME const FAR *patime,
                                FILETIME const FAR *pmtime) PURE;
    STDMETHOD(SetClass) (THIS_ REFCLSID clsid) PURE;
    STDMETHOD(SetStateBits) (THIS_ DWORD grfStateBits, DWORD grfMask) PURE;
    STDMETHOD(Stat) (THIS_ STATSTG FAR *pstatstg, DWORD grfStatFlag) PURE;
};


/****** IRootStorage Interface **********************************************/

#define LPROOTSTORAGE       IRootStorage FAR*

#undef  INTERFACE
#define INTERFACE   IRootStorage

DECLARE_INTERFACE_(IRootStorage, IUnknown)
{
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    /* *** IRootStorage methods *** */
    STDMETHOD(SwitchToFile) (THIS_ LPTSTR lpstrFile) PURE;
};

/****** Storage API Prototypes ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

STDAPI StgCreateDocfile(const TCHAR FAR* pwcsName,
            DWORD grfMode,
            DWORD reserved,
            IStorage FAR * FAR *ppstgOpen);
STDAPI StgCreateDocfileOnILockBytes(ILockBytes FAR *plkbyt,
                    DWORD grfMode,
                    DWORD reserved,
                    IStorage FAR * FAR *ppstgOpen);
STDAPI StgOpenStorage(const TCHAR FAR* pwcsName,
              IStorage FAR *pstgPriority,
              DWORD grfMode,
              SNB snbExclude,
              DWORD reserved,
              IStorage FAR * FAR *ppstgOpen);
STDAPI StgOpenStorageOnILockBytes(ILockBytes FAR *plkbyt,
                  IStorage FAR *pstgPriority,
                  DWORD grfMode,
                  SNB snbExclude,
                  DWORD reserved,
                  IStorage FAR * FAR *ppstgOpen);
STDAPI StgIsStorageFile(const TCHAR FAR* pwcsName);
STDAPI StgIsStorageILockBytes(ILockBytes FAR* plkbyt);

/****** Malloc API's ********************************************/
interface IMalloc;
#define LPMALLOC  IMalloc FAR*

STDAPI CoGetMalloc( DWORD dwMemContext, LPMALLOC * ppMalloc );
STDAPI_(LPVOID) CoTaskMemAlloc( ULONG cb );
STDAPI_(void) CoTaskMemFree( void* pv );
STDAPI_(LPVOID) CoTaskMemRealloc( LPVOID pv, ULONG cb );

#ifdef __cplusplus
};
#endif

/****** IMalloc Interface **********************************************/
#undef  INTERFACE
#define INTERFACE IMalloc

DECLARE_INTERFACE_(IMalloc, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)  (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    /*** IMallic methods ***/
    STDMETHOD_(void*,Alloc)    ( ULONG cb) PURE;
    STDMETHOD_(void *,Realloc) ( void *pv, ULONG cb) PURE;    
    STDMETHOD_(void,Free)      ( void *pv) PURE;
    STDMETHOD_(ULONG,GetSize)  ( void *pv) PURE;
    STDMETHOD_(int,DidAlloc)   ( void *pv) PURE;
    STDMETHOD_(void,HeapMinimize)( void ) PURE;
};

#define MEMCTX_TASK 1  /* the only supported flag for CoGetMalloc */

#endif /* ifndef _STORAGE_H_ */
