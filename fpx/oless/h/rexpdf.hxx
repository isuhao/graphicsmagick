//---------------------------------------------------------------
//
//  For conditions of distribution and use, see copyright notice
//  in Flashpix.h 
// 
//  Copyright (c) 1999 Digital Imaging Group, Inc.
// 
//  Contents:	Root exposed docfile header
//
//  Classes:	CRootExposedDocFile
//
//---------------------------------------------------------------

#ifndef __REXPDF_HXX__
#define __REXPDF_HXX__

#include "../expdf.hxx"

//+--------------------------------------------------------------
//
//  Class:	CRootExposedDocFile (rpdf)
//
//  Purpose:	Root form of the public docfile
//
//  Interface:	See below
//
//---------------------------------------------------------------

class CRootExposedDocFile : public CExposedDocFile
{
public:
    CRootExposedDocFile(CDFBasis *pdfb);
    SCODE InitRoot(ILockBytes *plstBase,
		   DWORD const dwStartFlags,
		   DFLAGS const df,
		   SNBW snbExclude);

    virtual ~CRootExposedDocFile(void);

    STDMETHOD(Stat) (STATSTGW *pstatstg, DWORD grfStatFlag);
    
private:
    SCODE Init(ILockBytes *plstBase,
               SNBW snbExclude,
               DWORD const dwStartFlags);
};

#endif // #ifndef __REXPDF_HXX__
