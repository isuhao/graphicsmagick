/*
  Image Compression/Decompression Methods.
*/
#ifndef _MAGICK_COMPRESS_H
#define _MAGICK_COMPRESS_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport unsigned int
  HuffmanDecodeImage(Image *),
  HuffmanEncodeImage(const ImageInfo *,Image *),
  Huffman2DEncodeImage(ImageInfo *,Image *),
  LZWEncodeImage(Image *,const unsigned int,unsigned char *),
  PackbitsEncodeImage(Image *,const unsigned int,unsigned char *),
  ZLIBEncodeImage(Image *,const unsigned long,const unsigned int,
    unsigned char *);

extern MagickExport void
  Ascii85Encode(Image *,const unsigned int),
  Ascii85Flush(Image *),
  Ascii85Initialize(Image *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
