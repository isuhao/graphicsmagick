/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                             AAA   V   V  SSSSS                              %
%                            A   A  V   V  SS                                 %
%                            AAAAA  V   V   SSS                               %
%                            A   A   V V      SS                              %
%                            A   A    V    SSSSS                              %
%                                                                             %
%                                                                             %
%                    Read/Write ImageMagick Image Format.                     %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                 July 1992                                   %
%                                                                             %
%                                                                             %
%  Copyright 1999 E. I. du Pont de Nemours and Company                        %
%                                                                             %
%  Permission is hereby granted, free of charge, to any person obtaining a    %
%  copy of this software and associated documentation files ("ImageMagick"),  %
%  to deal in ImageMagick without restriction, including without limitation   %
%  the rights to use, copy, modify, merge, publish, distribute, sublicense,   %
%  and/or sell copies of ImageMagick, and to permit persons to whom the       %
%  ImageMagick is furnished to do so, subject to the following conditions:    %
%                                                                             %
%  The above copyright notice and this permission notice shall be included in %
%  all copies or substantial portions of ImageMagick.                         %
%                                                                             %
%  The software is provided "as is", without warranty of any kind, express or %
%  implied, including but not limited to the warranties of merchantability,   %
%  fitness for a particular purpose and noninfringement.  In no event shall   %
%  E. I. du Pont de Nemours and Company be liable for any claim, damages or   %
%  other liability, whether in an action of contract, tort or otherwise,      %
%  arising from, out of or in connection with ImageMagick or the use or other %
%  dealings in ImageMagick.                                                   %
%                                                                             %
%  Except as contained in this notice, the name of the E. I. du Pont de       %
%  Nemours and Company shall not be used in advertising or otherwise to       %
%  promote the sale, use or other dealings in ImageMagick without prior       %
%  written authorization from the E. I. du Pont de Nemours and Company.       %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/

/*
  Include declarations.
*/
#include "magick.h"
#include "defines.h"
#include "proxy.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d A V S I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method ReadAVSImage reads an AVS X image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadAVSImage routine is:
%
%      image=ReadAVSImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Method ReadAVSImage returns a pointer to the image after
%      reading. A null image is returned if there is a memory shortage or if
%      the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
Export Image *ReadAVSImage(const ImageInfo *image_info)
{
  Image
    *image;

  Quantum
    blue,
    green,
    red;

  register int
    x,
    y;

  register long
    packets;

  register RunlengthPacket
    *q;

  unsigned long
    height,
    max_packets,
    width;

  unsigned short
    index;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image_info,image,ReadBinaryType);
  if (image->file == (FILE *) NULL)
    ReaderExit(FileOpenWarning,"Unable to open file",image);
  /*
    Read AVS image.
  */
  width=MSBFirstReadLong(image->file);
  height=MSBFirstReadLong(image->file);
  if ((width == (unsigned long) ~0) || (height == (unsigned long) ~0))
    ReaderExit(CorruptImageWarning,"Not a AVS image file",image);
  do
  {
    /*
      Initialize image structure.
    */
    image->columns=width;
    image->rows=height;
    if (image_info->ping)
      {
        CloseImage(image);
        return(image);
      }
    packets=0;
    max_packets=Max((image->columns*image->rows+4) >> 3,1);
    image->pixels=(RunlengthPacket *)
      AllocateMemory(max_packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      ReaderExit(ResourceLimitWarning,"Memory allocation failed",image);
    /*
      Convert AVS raster image to runlength-encoded packets.
    */
    q=image->pixels;
    SetRunlengthEncoder(q);
    for (y=0; y < (int) image->rows; y++)
    {
      for (x=0; x < (int) image->columns; x++)
      {
        index=UpScale(fgetc(image->file));
        image->matte|=index != Transparent;
        red=UpScale(fgetc(image->file));
        green=UpScale(fgetc(image->file));
        blue=UpScale(fgetc(image->file));
        if ((red == q->red) && (green == q->green) && (blue == q->blue) &&
            (index == q->index) && ((int) q->length < MaxRunlength))
          q->length++;
        else
          {
            if (packets != 0)
              q++;
            packets++;
            if (packets == (int) max_packets)
              {
                max_packets<<=1;
                image->pixels=(RunlengthPacket *) ReallocateMemory((char *)
                  image->pixels,max_packets*sizeof(RunlengthPacket));
                if (image->pixels == (RunlengthPacket *) NULL)
                  ReaderExit(ResourceLimitWarning,
                    "Memory allocation failed",image);
                q=image->pixels+packets-1;
              }
            q->red=red;
            q->green=green;
            q->blue=blue;
            q->index=index;
            q->length=0;
          }
      }
      if (image->previous == (Image *) NULL)
        if (QuantumTick(y,image->rows))
          ProgressMonitor(LoadImageText,y,image->rows);
    }
    SetRunlengthPackets(image,packets);
    /*
      Proceed to next image.
    */
    if (image_info->subrange != 0)
      if (image->scene >= (image_info->subimage+image_info->subrange-1))
        break;
    width=MSBFirstReadLong(image->file);
    height=MSBFirstReadLong(image->file);
    if ((width != (unsigned long) ~0) && (height != (unsigned long) ~0))
      {
        /*
          Allocate next image structure.
        */
        AllocateNextImage(image_info,image);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        image=image->next;
        ProgressMonitor(LoadImagesText,(unsigned int) ftell(image->file),
          (unsigned int) image->filesize);
      }
  } while ((width != (unsigned long) ~0) && (height != (unsigned long) ~0));
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e A V S I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method WriteAVSImage writes an image to a file in AVS X image format.
%
%  The format of the WriteAVSImage routine is:
%
%      status=WriteAVSImage(image_info,image)
%
%  A description of each parameter follows.
%
%    o status: Method WriteAVSImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%    o image:  A pointer to a Image structure.
%
%
*/
Export unsigned int WriteAVSImage(const ImageInfo *image_info,Image *image)
{
  int
    x,
    y;

  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *pixels;

  unsigned int
    scene;

  /*
    Open output image file.
  */
  OpenImage(image_info,image,WriteBinaryType);
  if (image->file == (FILE *) NULL)
    WriterExit(FileOpenWarning,"Unable to open file",image);
  scene=0;
  do
  {
    /*
      Write AVS header.
    */
    TransformRGBImage(image,RGBColorspace);
    MSBFirstWriteLong(image->columns,image->file);
    MSBFirstWriteLong(image->rows,image->file);
    /*
      Allocate memory for pixels.
    */
    pixels=(unsigned char *)
      AllocateMemory(image->columns*sizeof(RunlengthPacket));
    if (pixels == (unsigned char *) NULL)
      WriterExit(ResourceLimitWarning,"Memory allocation failed",image);
    /*
      Convert MIFF to AVS raster pixels.
    */
    x=0;
    y=0;
    p=image->pixels;
    q=pixels;
    for (i=0; i < (int) image->packets; i++)
    {
      for (j=0; j <= ((int) p->length); j++)
      {
        *q++=DownScale(image->matte ? p->index : Opaque);
        *q++=DownScale(p->red);
        *q++=DownScale(p->green);
        *q++=DownScale(p->blue);
        x++;
        if (x == (int) image->columns)
          {
            (void) fwrite((char *) pixels,1,q-pixels,image->file);
            if (image->previous == (Image *) NULL)
              if (QuantumTick(y,image->rows))
                ProgressMonitor(SaveImageText,y,image->rows);
            q=pixels;
            x=0;
            y++;
          }
      }
      p++;
    }
    FreeMemory((char *) pixels);
    if (image->next == (Image *) NULL)
      break;
    image->next->file=image->file;
    image=image->next;
    ProgressMonitor(SaveImagesText,scene++,GetNumberScenes(image));
  } while (image_info->adjoin);
  if (image_info->adjoin)
    while (image->previous != (Image *) NULL)
      image=image->previous;
  CloseImage(image);
  return(True);
}
