/*
  Graphic Gems - Graphic Support Methods.
*/
#ifndef _MAGICK_GEM_H
#define _MAGICK_GEM_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*
  Graphic gems define declarations.
*/
extern MagickExport double
  ExpandAffine(const AffineMatrix *),
  Permutate(const long,const long);

extern MagickExport int
  GetOptimalKernelWidth(const double,const double),
  GetOptimalKernelWidth1D(const double,const double),
  GetOptimalKernelWidth2D(const double,const double);

extern MagickExport inline PixelPacket
  AlphaComposite(const PixelPacket *,const unsigned long,const PixelPacket *,
    const unsigned long);

extern MagickExport PixelPacket
  InterpolateColor(const Image *,const double,const double,ExceptionInfo *);

extern MagickExport inline Quantum
  Downscale(const unsigned long),
  Intensity(const PixelPacket *),
  Upscale(const unsigned long);

extern MagickExport Quantum
  GenerateNoise(const Quantum,const NoiseType);

extern MagickExport inline unsigned int
  ColorMatch(const PixelPacket *,const PixelPacket *,const double);

extern MagickExport inline unsigned short
  XDownscale(const unsigned long),
  XUpscale(const unsigned long);

extern MagickExport void
  Contrast(const int,Quantum *,Quantum *,Quantum *),
  HSLTransform(const double,const double,const double,Quantum *,Quantum *,
    Quantum *),
  Hull(const long,const long,const unsigned long,const unsigned long,Quantum *,
    Quantum *,const int),
  IdentityAffine(AffineMatrix *),
  Modulate(const double,const double,const double,Quantum *,Quantum *,
    Quantum *),
  TransformHSL(const Quantum,const Quantum,const Quantum,double *,double *,
    double *),
  Upsample(const unsigned long,const unsigned long,const unsigned long,
    unsigned char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
