/*
  Copyright (C) 2003, 2005 GraphicsMagick Group
  Copyright (C) 2002 ImageMagick Studio
 
  This program is covered by multiple licenses, which are described in
  Copyright.txt. You should have received a copy of Copyright.txt with this
  package; otherwise see http://www.graphicsmagick.org/www/Copyright.html.
 
  Windows NT Utility Methods for ImageMagick.
*/
#ifndef _MAGICK_NTBASE_H
#define _MAGICK_NTBASE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/delegate.h"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <io.h>
#include <process.h>
#include <errno.h>
#if defined(_DEBUG)
#include <crtdbg.h>
#endif

/*
  Define declarations.
*/

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define RW_OK 6
#define HAVE_VSNPRINTF 1
#define HAVE_TEMPNAM 1
#define HAVE_RAISE 1

// Define to support memory mapping files for improved performance
#define HAVE_MMAP_FILEIO 1

// Use Visual C++ C inline method extension to improve performance
#define inline __inline

#if !defined(chsize)
# if defined(__BORLANDC__)
#   define chsize(file,length) chsize(file,length)
# else
#   define chsize(file,length) _chsize(file,length)
# endif
#endif

#if !defined(tempnam)
# define tempnam _tempnam
#endif
#define vsnprintf _vsnprintf 
#if defined(_MT) && defined(WIN32)
#define SAFE_GLOBAL __declspec(thread)
#else
#define SAFE_GLOBAL
#endif

/*
  With Visual C++, Popen and pclose are available via _popen and _pclose.
  These are documented to work for console applications only.
*/
#define HAVE_POPEN 1
#define HAVE__POPEN 1
#define popen(command,mode)  _popen(command,mode)
#define HAVE_PCLOSE 1
#define HAVE__PCLOSE 1
#define pclose(stream)  _pclose(stream)

/*
  Visual C++ 7.0 supports GlobalMemoryStatusEx but MinGW headers and
  Visual C++ 6.0 lack support for it.

  _MSC_VER values:
    1100 MSVC 5.0
    1200 MSVC 6.0
    1300 MSVC 7.0
*/
#if defined(_VISUALC_) && (_MSC_VER >= 1300)
#  define HAVE_GLOBALMEMORYSTATUSEX 1
#endif

/*
  Windows provides Unix-style access() via _access()
 */
#define HAVE_ACCESS 1
#define access(path,mode) _access(path,mode)
/*
  Typedef declarations.
*/
typedef UINT (CALLBACK *LPFNDLLFUNC1)(DWORD,UINT);

#if !defined(XS_VERSION) /* Not in Perl extension */

/*
  ssize_t is the type returned by _read and _write.
  Recent MinGW compilers include this typedef by default.
 */
#if !defined(ssize_t) && !defined(__MINGW32__)
typedef long ssize_t;
#endif /* !defined(ssize_t) && !defined(__MINGW32__) */

#endif /* !defined(XS_VERSION) */


/*
  NT utilities routines.
*/

extern MagickExport char
  *NTGetLastError(void);

/*
  Ghostscript DLL support.
*/
extern MagickExport int
  NTGhostscriptDLL(char *path, int path_length),
  NTGhostscriptEXE(char *path, int path_length),
  NTGhostscriptFonts(char *path, int path_length),
  NTGhostscriptLoadDLL(void),
  NTGhostscriptUnLoadDLL(void);

#if defined(MAGICK_IMPLEMENTATION)
extern MagickExport const GhostscriptVectors
  *NTGhostscriptDLLVectors( void );
#endif /* defined(MAGICK_IMPLEMENTATION) */

/*
  Directory access functions
*/
#if !defined(HAVE_DIRENT_H) || defined(__MINGW32__)
struct dirent
{
  char
    d_name[2048];

  int
    d_namlen;
};

typedef struct _DIR
{
  HANDLE
    hSearch;

  WIN32_FIND_DATA
    Win32FindData;

  BOOL
    firsttime;

  struct dirent
    file_info;
} DIR;

extern MagickExport long
  NTtelldir(DIR *);

extern MagickExport struct dirent
  *NTreaddir(DIR *);

extern MagickExport DIR
  *NTopendir(char *);

extern MagickExport void
  NTclosedir(DIR *),
  NTseekdir(DIR *,long);

#define closedir(dir) NTclosedir(dir)
#define opendir(path) NTopendir(path)
#define readdir(entry) NTreaddir(entry)
#define seekdir(entry,position) NTseekdir(entry,position)
#define telldir(entry) NTtelldir(entry)
#endif /* !defined(HAVE_DIRENT_H) */

/*
  System functions.
*/
extern MagickExport int
  Exit(int),
  NTSystemComman(const char *);

#if !defined(XS_VERSION)  /* Not in Perl extension */

/*
  Precision timing functions.
*/
extern MagickExport double
  NTElapsedTime(void),
  NTUserTime(void);

/*
  Memory mapped file support.
*/
#define PROT_NONE       0x0  // pages can not be accessed
#define PROT_READ       0x1  // pages can be read
#define PROT_WRITE      0x2  // pages can be written
#define MAP_SHARED      0x1  // share changes
#define MAP_PRIVATE     0x2  // changes are private
#define MAP_NORESERVE   0x4  // do not reserve paging space
#define MAP_ANON        0x8  // anonymous mapping
#if !defined(MAP_FAILED)
#  define MAP_FAILED      ((void *) -1) // returned on error by mmap
#endif
#define MS_ASYNC        0x0  // asynchronous page sync
#define MS_SYNC         0x1  // synchronous page sync

extern MagickExport void
  *NTmmap(char *address,size_t length,int protection,int access,int file,
     magick_off_t offset);

extern MagickExport int
  NTftruncate(int filedes, off_t length),
  NTmsync(void *addr, size_t len, int flags),
  NTmunmap(void *addr, size_t len);

#define mmap(address,length,protection,access,file,offset) \
  NTmmap(address,length,protection,access,file,offset)
#define msync(addr,len,flags) NTmsync(addr,len,flags)
#define munmap(addr,len) NTmunmap(addr,len)

/*
  libltdl-like module loader wrappers
*/
#if !defined(HasLTDL)
extern MagickExport void
  *NTdlopen(const char *filename),
  *NTdlsym(void *handle, const char *name);

extern MagickExport int
  NTdlclose(void *handle),
  NTdlexit(void),
  NTdlinit(void),
  NTdlsetsearchpath(const char *);

extern MagickExport const char
  *NTdlerror(void);

#define lt_dlclose(handle) NTdlclose(handle)
#define lt_dlexit() NTdlexit()
#define lt_dlinit() NTdlinit()
#define lt_dlopen(filename) NTdlopen(filename)
#define lt_dlsetsearchpath(path) NTdlsetsearchpath(path)
#define lt_dlsym(handle,name) NTdlsym(handle,name)
#endif /* !defined(HasLTDL) */
  
extern MagickExport unsigned char
  *NTResourceToBlob(const char *);

extern MagickExport MagickPassFail
  NTGetExecutionPath(char *path);

extern MagickExport MagickBool
  IsWindows95(),
  NTKernelAPISupported(const char *name);

extern MagickExport void
  NTErrorHandler(const ExceptionType,const char *,const char *),
  NTWarningHandler(const ExceptionType,const char *,const char *);

#endif /* !XS_VERSION */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif /* !C++ */

#endif /* !_MAGICK_NTBASE_H */
