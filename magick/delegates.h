/*
  Methods to Read/Write/Invoke Delegates.
*/
#ifndef _MAGICK_DELEGATES_H
#define _MAGICK_DELEGATES_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*
  Delegate structure definitions.
*/
typedef struct _DelegateInfo
{
  char
    *filename,
    *decode,
    *encode,
    *commands;

  int
    mode;

  unsigned int
    restrain,
    spawn;

  struct _DelegateInfo
    *previous,
    *next;
} DelegateInfo;

/*
  Magick delegate methods.
*/
extern MagickExport char
  *GetDelegateCommand(const ImageInfo *,Image *,const char *,const char *);

extern MagickExport DelegateInfo
  *GetDelegateInfo(const char *,const char *,ExceptionInfo *exception),
  *SetDelegateInfo(DelegateInfo *);

extern MagickExport unsigned int
  InvokeDelegate(const ImageInfo *,Image *,const char *,const char *),
  ListDelegateInfo(FILE *,ExceptionInfo *);

extern MagickExport void
  DestroyDelegateInfo(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
