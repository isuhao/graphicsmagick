/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                CCCC   OOO   N   N  V   V  EEEEE  RRRR   TTTTT               %
%               C      O   O  NN  N  V   V  E      R   R    T                 %
%               C      O   O  N N N  V   V  EEE    RRRR     T                 %
%               C      O   O  N  NN   V V   E      R R      T                 %
%                CCCC   OOO   N   N    V    EEEEE  R  R     T                 %
%                                                                             %
%                                                                             %
%                Convert an image from one format to another.                 %
%                                                                             %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                April 1992                                   %
%                                                                             %
%                                                                             %
%  Copyright (C) 2000 ImageMagick Studio, a non-profit organization dedicated %
%  to making software imaging solutions freely available.                     %
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
%  ImageMagick Studio be liable for any claim, damages or other liability,    %
%  whether in an action of contract, tort or otherwise, arising from, out of  %
%  or in connection with ImageMagick or the use or other dealings in          %
%  ImageMagick.                                                               %
%                                                                             %
%  Except as contained in this notice, the name of the ImageMagick Studio     %
%  shall not be used in advertising or otherwise to promote the sale, use or  %
%  other dealings in ImageMagick without prior written authorization from the %
%  ImageMagick Studio.                                                        %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Convert converts an input file using one image format to an output file
%  with a differing image format.
%
%  The convert program syntax is:
%
%  Usage: convert [options ...] file [ files... ] output_file
%
%  Where options include:
%    -adjoin              join images into a single multi-image file
%    -affine matrix       drawing transform matrix
%    -antialias           remove pixel-aliasing
%    -append              append an image sequence
%    -average             average an image sequence
%    -blur order          apply a filter to blur the image
%    -border geometry     surround image with a border of color
%    -bordercolor color   border color
%    -box color           color for annotation bounding box
%    -cache threshold     number of megabytes available to the pixel cache
%    -charcoal order      simulate a charcoal drawing
%    -coalesce            merge a sequence of images
%    -colorize value      colorize the image with the fill color
%    -colors value        preferred number of colors in the image
%    -colorspace type     alternate image colorspace
%    -comment string      annotate image with comment
%    -compress type       type of image compression
%    -contrast            enhance or reduce the image contrast
%    -crop geometry       preferred size and location of the cropped image
%    -cycle amount        cycle the image colormap
%    -deconstruct         break down an image sequence into constituent parts
%    -delay value         display the next image after pausing
%    -density geometry    vertical and horizontal density of the image
%    -depth value         depth of the image
%    -despeckle           reduce the speckles within an image"
%    -display server      obtain image or font from this X server
%    -dispose method      GIF disposal method
%    -dither              apply Floyd/Steinberg error diffusion to image
%    -draw string         annotate the image with a graphic primitive
%    -edge order          apply a filter to detect edges in the image
%    -emboss order        emboss an image
%    -enhance             apply a digital filter to enhance a noisy image
%    -equalize            perform histogram equalization to an image
%    -fill color          color to use when filling a graphic primitive
%    -filter type         use this filter when resizing an image
%    -flip                flip image in the vertical direction
%    -flop                flop image in the horizontal direction
%    -font name           X11 font for displaying text
%    -frame geometry      surround image with an ornamental border
%    -fuzz distance       colors within this distance are considered equal
%    -gamma value         level of gamma correction
%    -gaussian geometry   gaussian blur an image
%    -geometry geometry   perferred size or location of the image
%    -gravity type        vertical and horizontal text placement
%    -implode amount      implode image pixels about the center
%    -intent type         Absolute, Perceptual, Relative, or Saturation
%    -interlace type      None, Line, Plane, or Partition
%    -label name          assign a label to an image
%    -layer type          Red, Green, Blue, Matte
%    -linewidth value     width of line in pixels
%    -loop iterations     add Netscape loop extension to your GIF animation
%    -map filename        transform image colors to match this set of colors
%    -matte               store matte channel if the image has one
%    -median order        apply a median filter to the image
%    -modulate value      vary the brightness, saturation and hue
%    -monochrome          transform image to black and white
%    -morph value         morph an image sequence
%    -mosaic              create an mosaic from an image sequence
%    -negate              replace every pixel with its complementary color
%    -noise               add or reduce noise in an image
%    -normalize           transform image to span the full range of colors
%    -opaque color        change this color to the fill color
%    -page geometry       size and location of an image canvas
%    -paint radius        simulate an oil painting
%    -ping                efficiently determine image characteristics
%    -pointsize value     pointsize of Postscript font
%    -preview type        image preview type
%    -profile filename    add ICC or IPTC information profile to image
%    -quality value       JPEG/MIFF/PNG compression level
%    -raise value         lighten/darken image edges to create a 3-D effect
%    -region geometry     apply options to a portion of the image
%    -roll geometry       roll an image vertically or horizontally
%    -rotate degrees      apply Paeth rotation to the image
%    -sample geometry     scale image with pixel sampling
%    -scale geometry      resize image
%    -scene value         image scene number
%    -seed value          pseudo-random number generator seed value
%    -segment values      segment an image
%    -shade degrees       shade the image using a distant light source
%    -sharpen order       apply a filter to sharpen the image
%    -shear geometry      slide one edge of the image along the X or Y axis
%    -size geometry       width and height of image
%    -solarize threshold  negate all pixels above the threshold level
%    -spread amount       displace image pixels by a random amount
%    -stroke color        color to use when stroking a graphic primitive
%    -swirl degrees       swirl image pixels about the center
%    -texture filename    name of texture to tile onto the image background
%    -threshold value     threshold the image
%    -transparent color   make this color transparent within the image
%    -treedepth value     depth of the color storage_class tree
%    -units type          PixelsPerInch, PixelsPerCentimeter, or Undefined
%    -verbose             print detailed information about the image
%    -view                FlashPix viewing transforms
%    -wave geometry       alter an image along a sine wave
%
%  By default, the image format of `file' is determined by its magic
%  number.  To specify a particular image format, precede the filename
%  with an image format name and a colon (i.e. ps:image) or specify the
%  image type as the filename suffix (i.e. image.ps).  Specify 'file' as
%  '-' for standard input or output.
%
%
*/

/*
  Forward reference
*/
void ConcatenateImages(int argc,char **argv);
/*
  Include declarations.
*/
#if defined(CONVERT_MAIN)
static int convert_main(int argc,char **argv,
  const char *header_data,const int header_length)
#else
#include "magick/magick.h"
#include "magick/defines.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U s a g e                                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure Usage displays the program usage;
%
%  The format of the Usage method is:
%
%      void Usage(const char *client_name)
%
%    o client_name: a character string representing the name of the client
%      program.
%
%
*/
static void Usage(const char *client_name)
{
  const char
    **p;

  static const char
    *options[]=
    {
      "-adjoin              join images into a single multi-image file",
      "-affine matrix       drawing transform matrix",
      "-antialias           remove pixel-aliasing",
      "-append              append an image sequence",
      "-average             average an image sequence",
      "-blur order          apply a filter to blur the image",
      "-border geometry     surround image with a border of color",
      "-bordercolor color   border color",
      "-box color           color for annotation bounding box",
      "-cache threshold     megabytes of memory available to the pixel cache",
      "-charcoal order      simulate a charcoal drawing",
      "-coalesce            merge a sequence of images",
      "-colorize value      colorize the image with the fill color",
      "-colors value        preferred number of colors in the image",
      "-colorspace type     alternate image colorspace",
      "-comment string      annotate image with comment",
      "-compress type       type of image compression",
      "-contrast            enhance or reduce the image contrast",
      "-crop geometry       preferred size and location of the cropped image",
      "-cycle amount        cycle the image colormap",
      "-delay value         display the next image after pausing",
      "-deconstruct         break down an image sequence into constituent parts",
      "-density geometry    vertical and horizontal density of the image",
      "-depth value         depth of the image",
      "-despeckle           reduce the speckles within an image",
      "-display server      obtain image or font from this X server",
      "-dispose method      GIF disposal method",
      "-dither              apply Floyd/Steinberg error diffusion to image",
      "-draw string         annotate the image with a graphic primitive",
      "-edge order          apply a filter to detect edges in the image",
      "-emboss order        emboss an image",
      "-enhance             apply a digital filter to enhance a noisy image",
      "-equalize            perform histogram equalization to an image",
      "-fill color          color to use when filling a graphic primitive",
      "-filter type         use this filter when resizing an image",
      "-flip                flip image in the vertical direction",
      "-flop                flop image in the horizontal direction",
      "-font name           X11 font for displaying text",
      "-frame geometry      surround image with an ornamental border",
      "-fuzz distance       colors within this distance are considered equal",
      "-gamma value         level of gamma correction",
      "-geometry geometry   perferred size or location of the image",
      "-gaussian geometry   gaussian blur an image",
      "-gravity type        vertical and horizontal text placement",
      "-implode amount      implode image pixels about the center",
      "-intent type         Absolute, Perceptual, Relative, or Saturation",
      "-interlace type      None, Line, Plane, or Partition",
      "-label name          assign a label to an image",
      "-layer type          Red, Green, Blue, Matte",
      "-linewidth value     width of line in pixels",
      "-label name          assign a label to an image",
      "-loop iterations     add Netscape loop extension to your GIF animation",
      "-map filename        transform image colors to match this set of colors",
      "-matte               store matte channel if the image has one",
      "-median order        apply a median filter to the image",
      "-modulate value      vary the brightness, saturation, and hue",
      "-monochrome          transform image to black and white",
      "-morph value         morph an image sequence",
      "-mosaic              create an mosaic from an image sequence",
      "-negate              replace every pixel with its complementary color ",
      "-noise               add or reduce noise in an image",
      "-normalize           transform image to span the full range of colors",
      "-opaque color        change this color to the fill color",
      "-page geometry       size and location of an image canvas",
      "-paint radius        simulate an oil painting",
      "-ping                efficiently determine image characteristics",
      "-pointsize value     pointsize of Postscript font",
      "-preview type        image preview type",
      "-profile filename    add ICC or IPTC information profile to image",
      "-quality value       JPEG/MIFF/PNG compression level",
      "-raise value         lighten/darken image edges to create a 3-D effect",
      "-region geometry     apply options to a portion of the image",
      "-roll geometry       roll an image vertically or horizontally",
      "-rotate degrees      apply Paeth rotation to the image",
      "-sample geometry     scale image with pixel sampling",
      "-scale geometry      resize image",
      "-scene value         image scene number",
      "-segment values      segment an image",
      "-seed value          pseudo-random number generator seed value",
      "-shade degrees       shade the image using a distant light source",
      "-sharpen order       apply a filter to sharpen the image",
      "-shear geometry      slide one edge of the image along the X or Y axis",
      "-size geometry       width and height of image",
      "-solarize threshold  negate all pixels above the threshold level",
      "-spread amount       displace image pixels by a random amount",
      "-stroke color        color to use when stoking a graphic primitive",
      "-swirl degrees       swirl image pixels about the center",
      "-texture filename    name of texture to tile onto the image background",
      "-threshold value     threshold the image",
      "-transparent color   make this color transparent within the image",
      "-treedepth value     depth of the color storage_class tree",
      "-units type          Inch, Centimeter, or Undefined",
      "-verbose             print detailed information about the image",
      "-view                FlashPix viewing transforms",
      "-wave geometry       alter an image along a sine wave",
      (char *) NULL
    };

  (void) printf("Version: %.1024s\n",MagickVersion);
  (void) printf("Copyright: %.1024s\n\n",MagickCopyright);
  (void) printf("Usage: %.1024s [options ...] file [ files... ] output_file\n",
    client_name);
  (void) printf("\nWhere options include:\n");
  for (p=options; *p != (char *) NULL; p++)
    (void) printf("  %.1024s\n",*p);
  (void) printf(
    "\nBy default, the image format of `file' is determined by its magic\n");
  (void) printf(
    "number.  To specify a particular image format, precede the filename\n");
  (void) printf(
    "with an image format name and a colon (i.e. ps:image) or specify the\n");
  (void) printf(
    "image type as the filename suffix (i.e. image.ps).  Specify 'file' as\n");
  (void) printf("'-' for standard input or output.\n");
  ListMagickInfo((FILE *) NULL);
  ListDelegateInfo((FILE *) NULL);
  Exit(0);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  M a i n                                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/
int main(int argc,char **argv)
#endif
{
#define NotInitialized  (unsigned int) (~0)

  char
    *client_name,
    *filename,
    *option;

  double
    sans;

  ExceptionInfo
    exception;

  Image
    *image,
    *next_image;

  ImageInfo
    *image_info;

  int
    append,
    x;

  register Image
    *p;

  register int
    i;

  unsigned int
    average,
    coalesce,
    deconstruct,
    morph,
    mosaic,
    global_colormap,
    scene,
    status;

  /*
    Initialize command line arguments.
  */
#if !defined(CONVERT_MAIN)
  ReadCommandlLine(argc,&argv);
  MagickIncarnate(*argv);
#endif
  client_name=SetClientName((char *) NULL);
  status=ExpandFilenames(&argc,&argv);
  if (status == False)
    MagickError(ResourceLimitError,"Memory allocation failed",(char *) NULL);
  if (argc < 3)
    Usage(client_name);
  /*
    Set defaults.
  */
  append=0;
  average=False;
  coalesce=False;
  deconstruct=False;
  GetExceptionInfo(&exception);
  morph=0;
  mosaic=False;
  filename=(char *) NULL;
  global_colormap=False;
  image=(Image *) NULL;
  image_info=CloneImageInfo((ImageInfo *) NULL);
  (void) strcpy(image_info->filename,argv[argc-1]);
  SetImageInfo(image_info,True);
  if (strcmp(image_info->magick,"CACHE") == 0)
    SetCacheThreshold(0);
  option=(char *) NULL;
  scene=0;
  /*
    Parse command-line arguments.
  */
  if ((argc > 2) && (LocaleCompare("-concatenate",argv[1]) == 0))
    ConcatenateImages(argc,argv);
  for (i=1; i < (argc-1); i++)
  {
    option=argv[i];
    if ((Extent(option) < 2) || ((*option != '-') && (*option != '+')))
      {
        /*
          Read input image.
        */
        filename=argv[i];
        (void) strcpy(image_info->filename,filename);
        next_image=ReadImage(image_info,&exception);
        if (next_image == (Image *) NULL)
          {
            MagickWarning(exception.severity,exception.message,
              exception.qualifier);
            continue;
          }
        status=MogrifyImages(image_info,i,argv,&next_image);
        if (status == False)
          CatchImageException(next_image);
        if (image == (Image *) NULL)
          image=next_image;
        else
          {
            /*
              Link image into image list.
            */
            for (p=image; p->next != (Image *) NULL; p=p->next);
            next_image->previous=p;
            p->next=next_image;
          }
      }
    else
      switch(*(option+1))
      {
        case 'a':
        {
          if (LocaleNCompare("adjoin",option+1,2) == 0)
            {
              image_info->adjoin=(*option == '-');
              break;
            }
          if (LocaleNCompare("affine",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing matrix",option);
                }
              break;
            }
          if (LocaleNCompare("antialias",option+1,3) == 0)
            {
              image_info->antialias=(*option == '-');
              break;
            }
          if (LocaleNCompare("append",option+1,2) == 0)
            {
              append=(*option) == '-' ? 1 : -1;
              break;
            }
          if (LocaleNCompare("average",option+1,2) == 0)
            {
              average=(*option == '-');
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'b':
        {
          if (LocaleNCompare("background",option+1,5) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing background color",option);
                  (void) QueryColorDatabase(argv[i],
                    &image_info->background_color);
                }
              break;
            }
          if (LocaleNCompare("blur",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing order",option);
                }
              break;
            }
          if (LocaleNCompare("border",option+1,7) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("bordercolor",option+1,7) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing border color",option);
                  (void) QueryColorDatabase(argv[i],&image_info->border_color);
                }
              break;
            }
          if (LocaleNCompare("box",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing box color",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'c':
        {
          if (LocaleNCompare("cache",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing threshold",option);
                }
              SetCacheThreshold(atoi(argv[i]));
              break;
            }
          if (LocaleNCompare("charcoal",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing order",option);
                }
              break;
            }
          if (LocaleNCompare("coalesce",option+1,3) == 0)
            {
              coalesce=(*option == '-');
              break;
            }
          if (LocaleNCompare("colorize",option+1,7) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing value",option);
                }
              break;
            }
          if (LocaleNCompare("colors",option+1,7) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing colors",option);
                }
              break;
            }
          if (LocaleNCompare("colorspace",option+1,7) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  image_info->colorspace=UndefinedColorspace;
                  if (LocaleCompare("cmyk",option) == 0)
                    image_info->colorspace=CMYKColorspace;
                  if (LocaleCompare("gray",option) == 0)
                    image_info->colorspace=GRAYColorspace;
                  if (LocaleCompare("ohta",option) == 0)
                    image_info->colorspace=OHTAColorspace;
                  if (LocaleCompare("rgb",option) == 0)
                    image_info->colorspace=RGBColorspace;
                  if (LocaleCompare("srgb",option) == 0)
                    image_info->colorspace=sRGBColorspace;
                  if (LocaleCompare("transparent",option) == 0)
                    image_info->colorspace=TransparentColorspace;
                  if (LocaleCompare("xyz",option) == 0)
                    image_info->colorspace=XYZColorspace;
                  if (LocaleCompare("ycbcr",option) == 0)
                    image_info->colorspace=YCbCrColorspace;
                  if (LocaleCompare("ycc",option) == 0)
                    image_info->colorspace=YCCColorspace;
                  if (LocaleCompare("yiq",option) == 0)
                    image_info->colorspace=YIQColorspace;
                  if (LocaleCompare("ypbpr",option) == 0)
                    image_info->colorspace=YPbPrColorspace;
                  if (LocaleCompare("yuv",option) == 0)
                    image_info->colorspace=YUVColorspace;
                  if (image_info->colorspace == UndefinedColorspace)
                    MagickError(OptionError,"Invalid colorspace type",option);
                }
              break;
            }
          if (LocaleNCompare("comment",option+1,4) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing comment",option);
                }
              break;
            }
          if (LocaleNCompare("compress",option+1,5) == 0)
            {
              image_info->compression=NoCompression;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  image_info->compression=UndefinedCompression;
                  if (LocaleCompare("None",option) == 0)
                    image_info->compression=NoCompression;
                  if (LocaleCompare("BZip",option) == 0)
                    image_info->compression=BZipCompression;
                  if (LocaleCompare("Fax",option) == 0)
                    image_info->compression=FaxCompression;
                  if (LocaleCompare("Group4",option) == 0)
                    image_info->compression=Group4Compression;
                  if (LocaleCompare("JPEG",option) == 0)
                    image_info->compression=JPEGCompression;
                  if (LocaleCompare("LZW",option) == 0)
                    image_info->compression=LZWCompression;
                  if (LocaleCompare("RunlengthEncoded",option) == 0)
                    image_info->compression=RunlengthEncodedCompression;
                  if (LocaleCompare("Zip",option) == 0)
                    image_info->compression=ZipCompression;
                  if (image_info->compression == UndefinedCompression)
                    MagickError(OptionError,"Invalid compression type",option);
                }
              break;
            }
          if (LocaleNCompare("contrast",option+1,3) == 0)
            break;
          if (LocaleNCompare("crop",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("cycle",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing amount",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'd':
        {
          if (LocaleNCompare("deconstruct",option+1,3) == 0)
            {
              deconstruct=(*option == '-');
              break;
            }
          if (LocaleNCompare("delay",option+1,3) == 0)
            {
              CloneString(&image_info->delay,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing delay",option);
                  image_info->delay=PostscriptGeometry(argv[i]);
                }
              break;
            }
          if (LocaleNCompare("density",option+1,3) == 0)
            {
              CloneString(&image_info->density,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                  (void) CloneString(&image_info->density,argv[i]);
                }
              break;
            }
          if (LocaleNCompare("depth",option+1,3) == 0)
            {
              image_info->depth=QuantumDepth;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing image depth",option);
                  image_info->depth=atoi(argv[i]);
                }
              break;
            }
          if (LocaleNCompare("despeckle",option+1,3) == 0)
            break;
          if (LocaleCompare("display",option+1) == 0)
            {
              CloneString(&image_info->server_name,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing server name",option);
                  (void) CloneString(&image_info->server_name,argv[i]);
                }
              break;
            }
          if (LocaleNCompare("dispose",option+1,5) == 0)
            {
              CloneString(&image_info->dispose,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing method",option);
                  image_info->dispose=PostscriptGeometry(argv[i]);
                }
              break;
            }
          if (LocaleNCompare("dither",option+1,3) == 0)
            {
              image_info->dither=(*option == '-');
              break;
            }
          if (LocaleNCompare("draw",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing primitive",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'e':
        {
          if (LocaleNCompare("edge",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing order",option);
                }
              break;
            }
          if (LocaleNCompare("emboss",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing order",option);
                }
              break;
            }
          if (LocaleNCompare("enhance",option+1,2) == 0)
            break;
          if (LocaleNCompare("equalize",option+1,2) == 0)
            break;
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'f':
        {
          if (LocaleCompare("fill",option+1) == 0)
            {
              (void) QueryColorDatabase("none",&image_info->fill);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing fill color",option);
                  (void) QueryColorDatabase(argv[i],&image_info->fill);
                }
              break;
            }
          if (LocaleCompare("fill",option+1) == 0)
            {
              i++;
              if (i == argc)
                MagickError(OptionError,"Missing fill color",option);
              QueryColorDatabase(argv[i],&image_info->fill);
              break;
            }
          if (LocaleNCompare("filter",option+1,4) == 0)
            {
              if (*option == '-')
                {
                  FilterTypes
                    filter;

                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  filter=UndefinedFilter;
                  if (LocaleCompare("Point",option) == 0)
                    filter=PointFilter;
                  if (LocaleCompare("Box",option) == 0)
                    filter=BoxFilter;
                  if (LocaleCompare("Triangle",option) == 0)
                    filter=TriangleFilter;
                  if (LocaleCompare("Hermite",option) == 0)
                    filter=HermiteFilter;
                  if (LocaleCompare("Hanning",option) == 0)
                    filter=HanningFilter;
                  if (LocaleCompare("Hamming",option) == 0)
                    filter=HammingFilter;
                  if (LocaleCompare("Blackman",option) == 0)
                    filter=BlackmanFilter;
                  if (LocaleCompare("Gaussian",option) == 0)
                    filter=GaussianFilter;
                  if (LocaleCompare("Quadratic",option) == 0)
                    filter=QuadraticFilter;
                  if (LocaleCompare("Cubic",option) == 0)
                    filter=CubicFilter;
                  if (LocaleCompare("Catrom",option) == 0)
                    filter=CatromFilter;
                  if (LocaleCompare("Mitchell",option) == 0)
                    filter=MitchellFilter;
                  if (LocaleCompare("Lanczos",option) == 0)
                    filter=LanczosFilter;
                  if (LocaleCompare("Bessel",option) == 0)
                    filter=BesselFilter;
                  if (LocaleCompare("Sinc",option) == 0)
                    filter=SincFilter;
                  if (filter == UndefinedFilter)
                    MagickError(OptionError,"Invalid filter type",option);
                }
              break;
            }
          if (LocaleNCompare("flip",option+1,3) == 0)
            break;
          if (LocaleNCompare("flop",option+1,3) == 0)
            break;
          if (LocaleNCompare("font",option+1,2) == 0)
            {
              CloneString(&image_info->font,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing font name",option);
                  (void) CloneString(&image_info->font,argv[i]);
                }
              break;
            }
          if (LocaleNCompare("frame",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("fuzz",option+1,2) == 0)
            {
              image_info->fuzz=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing distance",option);
                  image_info->fuzz=atoi(argv[i]);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'g':
        {
          if (LocaleNCompare("gamma",option+1,3) == 0)
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                MagickError(OptionError,"Missing value",option);
              break;
            }
          if (LocaleCompare("gaussian",option+1) == 0)
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%d",&x))
                MagickError(OptionError,"Missing width",option);
              break;
            }
          if (LocaleNCompare("geometry",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("gravity",option+1,2) == 0)
            {
              GravityType
                gravity;

              gravity=ForgetGravity;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  if (LocaleCompare("NorthWest",option) == 0)
                    gravity=NorthWestGravity;
                  if (LocaleCompare("North",option) == 0)
                    gravity=NorthGravity;
                  if (LocaleCompare("NorthEast",option) == 0)
                    gravity=NorthEastGravity;
                  if (LocaleCompare("West",option) == 0)
                    gravity=WestGravity;
                  if (LocaleCompare("Center",option) == 0)
                    gravity=CenterGravity;
                  if (LocaleCompare("East",option) == 0)
                    gravity=EastGravity;
                  if (LocaleCompare("SouthWest",option) == 0)
                    gravity=SouthWestGravity;
                  if (LocaleCompare("South",option) == 0)
                    gravity=SouthGravity;
                  if (LocaleCompare("SouthEast",option) == 0)
                    gravity=SouthEastGravity;
                  if (gravity == ForgetGravity)
                    MagickError(OptionError,"Invalid gravity type",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'h':
        {
          if (LocaleNCompare("help",option+1,2) == 0)
            {
              Usage(client_name);
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'i':
        {
          if (LocaleNCompare("implode",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing amount",option);
                }
              break;
            }
          if (LocaleNCompare("filter",option+1,5) == 0)
            {
              if (*option == '-')
                {
                  RenderingIntent
                    rendering_intent;

                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  rendering_intent=UndefinedIntent;
                  if (LocaleCompare("Absolute",option) == 0)
                    rendering_intent=AbsoluteIntent;
                  if (LocaleCompare("Perceptual",option) == 0)
                    rendering_intent=PerceptualIntent;
                  if (LocaleCompare("Relative",option) == 0)
                    rendering_intent=RelativeIntent;
                  if (LocaleCompare("Saturation",option) == 0)
                    rendering_intent=SaturationIntent;
                  if (rendering_intent == UndefinedIntent)
                    MagickError(OptionError,"Invalid intent type",option);
                }
              break;
            }
          if (LocaleNCompare("interlace",option+1,5) == 0)
            {
              image_info->interlace=NoInterlace;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  image_info->interlace=UndefinedInterlace;
                  if (LocaleCompare("None",option) == 0)
                    image_info->interlace=NoInterlace;
                  if (LocaleCompare("Line",option) == 0)
                    image_info->interlace=LineInterlace;
                  if (LocaleCompare("Plane",option) == 0)
                    image_info->interlace=PlaneInterlace;
                  if (LocaleCompare("Partition",option) == 0)
                    image_info->interlace=PartitionInterlace;
                  if (image_info->interlace == UndefinedInterlace)
                    MagickError(OptionError,"Invalid interlace type",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'l':
        {
          if (LocaleNCompare("label",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing label name",option);
                }
              break;
            }
          if (LocaleNCompare("layer",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  LayerType
                    layer;

                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  layer=UndefinedLayer;
                  if (LocaleCompare("Red",option) == 0)
                    layer=RedLayer;
                  if (LocaleCompare("Green",option) == 0)
                    layer=GreenLayer;
                  if (LocaleCompare("Blue",option) == 0)
                    layer=BlueLayer;
                  if (LocaleCompare("Matte",option) == 0)
                    layer=MatteLayer;
                  if (layer == UndefinedLayer)
                    MagickError(OptionError,"Invalid layer type",option);
                }
              break;
            }
          if (LocaleNCompare("linewidth",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing size",option);
                }
              break;
            }
          if (LocaleNCompare("loop",option+1,2) == 0)
            {
              image_info->iterations=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing iterations",option);
                  image_info->iterations=PostscriptGeometry(argv[i]);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'm':
        {
          if (LocaleNCompare("map",option+1,3) == 0)
            {
              global_colormap=(*option == '+');
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing file name",option);
                }
              break;
            }
          if (LocaleCompare("matte",option+1) == 0)
            break;
          if (LocaleNCompare("mattecolor",option+1,6) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing matte color",option);
                  (void) QueryColorDatabase(argv[i],&image_info->matte_color);
                }
              break;
            }
          if (LocaleNCompare("median",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing radius",option);
                }
              break;
            }
          if (LocaleNCompare("modulate",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing value",option);
                }
              break;
            }
          if (LocaleNCompare("monochrome",option+1,4) == 0)
            {
              image_info->monochrome=(*option == '-');
              break;
            }
          if (LocaleNCompare("morph",option+1,3) == 0)
            {
              morph=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing frames",option);
                  morph=atoi(argv[i]);
                }
              break;
            }
          if (LocaleNCompare("mosaic",option+1,3) == 0)
            {
              mosaic=(*option == '-');
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'n':
        {
          if (LocaleNCompare("negate",option+1,3) == 0)
            break;
          if (LocaleNCompare("noise",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing value",option);
                }
              if (*option == '+')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  if ((LocaleCompare("Uniform",option) != 0) &&
                      (LocaleCompare("Gaussian",option) != 0) &&
                      (LocaleCompare("Multiplicative",option) != 0) &&
                      (LocaleCompare("Impulse",option) != 0) &&
                      (LocaleCompare("Laplacian",option) != 0) &&
                      (LocaleCompare("Poisson",option) != 0))
                    MagickError(OptionError,"Invalid noise type",option);
                }
              break;
            }
          if (LocaleNCompare("normalize",option+1,3) == 0)
            break;
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'o':
        {
          if (LocaleNCompare("opaque",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing opaque color",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'p':
        {
          if (LocaleNCompare("page",option+1,3) == 0)
            {
              CloneString(&image_info->page,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing page geometry",option);
                  image_info->page=PostscriptGeometry(argv[i]);
                }
              break;
            }
          if (LocaleNCompare("paint",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing radius",option);
                }
              break;
            }
          if (LocaleCompare("pen",option+1) == 0)
            {
              (void) QueryColorDatabase("none",&image_info->fill);
              (void) QueryColorDatabase("none",&image_info->stroke);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing fill color",option);
                  (void) QueryColorDatabase(argv[i],&image_info->fill);
                  (void) QueryColorDatabase(argv[i],&image_info->stroke);
                }
              break;
            }
          if (LocaleNCompare("ping",option+1,2) == 0)
            {
              image_info->ping=(*option == '-');
              break;
            }
          if (LocaleNCompare("pointsize",option+1,2) == 0)
            {
              image_info->pointsize=12;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing size",option);
                  image_info->pointsize=atof(argv[i]);
                }
              break;
            }
          if (LocaleNCompare("preview",option+1,3) == 0)
            {
              image_info->preview_type=UndefinedPreview;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  image_info->preview_type=UndefinedPreview;
                  if (LocaleCompare("Rotate",option) == 0)
                    image_info->preview_type=RotatePreview;
                  if (LocaleCompare("Shear",option) == 0)
                    image_info->preview_type=ShearPreview;
                  if (LocaleCompare("Roll",option) == 0)
                    image_info->preview_type=RollPreview;
                  if (LocaleCompare("Hue",option) == 0)
                    image_info->preview_type=HuePreview;
                  if (LocaleCompare("Saturation",option) == 0)
                    image_info->preview_type=SaturationPreview;
                  if (LocaleCompare("Brightness",option) == 0)
                    image_info->preview_type=BrightnessPreview;
                  if (LocaleCompare("Gamma",option) == 0)
                    image_info->preview_type=GammaPreview;
                  if (LocaleCompare("Spiff",option) == 0)
                    image_info->preview_type=SpiffPreview;
                  if (LocaleCompare("Dull",option) == 0)
                    image_info->preview_type=DullPreview;
                  if (LocaleCompare("Grayscale",option) == 0)
                    image_info->preview_type=GrayscalePreview;
                  if (LocaleCompare("Quantize",option) == 0)
                    image_info->preview_type=QuantizePreview;
                  if (LocaleCompare("Despeckle",option) == 0)
                    image_info->preview_type=DespecklePreview;
                  if (LocaleCompare("ReduceNoise",option) == 0)
                    image_info->preview_type=ReduceNoisePreview;
                  if (LocaleCompare("AddNoise",option) == 0)
                    image_info->preview_type=AddNoisePreview;
                  if (LocaleCompare("Sharpen",option) == 0)
                    image_info->preview_type=SharpenPreview;
                  if (LocaleCompare("Blur",option) == 0)
                    image_info->preview_type=BlurPreview;
                  if (LocaleCompare("Threshold",option) == 0)
                    image_info->preview_type=ThresholdPreview;
                  if (LocaleCompare("EdgeDetect",option) == 0)
                    image_info->preview_type=EdgeDetectPreview;
                  if (LocaleCompare("Spread",option) == 0)
                    image_info->preview_type=SpreadPreview;
                  if (LocaleCompare("Shade",option) == 0)
                    image_info->preview_type=ShadePreview;
                  if (LocaleCompare("Raise",option) == 0)
                    image_info->preview_type=RaisePreview;
                  if (LocaleCompare("Segment",option) == 0)
                    image_info->preview_type=SegmentPreview;
                  if (LocaleCompare("Solarize",option) == 0)
                    image_info->preview_type=SolarizePreview;
                  if (LocaleCompare("Swirl",option) == 0)
                    image_info->preview_type=SwirlPreview;
                  if (LocaleCompare("Implode",option) == 0)
                    image_info->preview_type=ImplodePreview;
                  if (LocaleCompare("Wave",option) == 0)
                    image_info->preview_type=WavePreview;
                  if (LocaleCompare("OilPaint",option) == 0)
                    image_info->preview_type=OilPaintPreview;
                  if (LocaleCompare("CharcoalDrawing",option) == 0)
                    image_info->preview_type=CharcoalDrawingPreview;
                  if (LocaleCompare("JPEG",option) == 0)
                    image_info->preview_type=JPEGPreview;
                  if (image_info->preview_type == UndefinedPreview)
                    MagickError(OptionError,"Invalid interlace type",option);
                }
              break;
            }
          if (LocaleNCompare("process",option+1,4) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing argument",option);
                }
              break;
            }
          if (LocaleNCompare("profile",option+1,4) == 0)
            {
              i++;
              if (i == argc)
                MagickError(OptionError,"Missing profile",option);
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'q':
        {
          if (LocaleNCompare("quality",option+1,2) == 0)
            {
              image_info->quality=75;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing quality",option);
                  image_info->quality=atoi(argv[i]);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'r':
        {
          if (LocaleNCompare("raise",option+1,2) == 0)
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%d",&x))
                MagickError(OptionError,"Missing bevel width",option);
              break;
            }
          if (LocaleNCompare("region",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("roll",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("rotate",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing degrees",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 's':
        {
          if (LocaleNCompare("sample",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("scale",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("scene",option+1,3) == 0)
            {
              scene=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing scene number",option);
                }
              scene=atoi(argv[i]);
              break;
            }
          if (LocaleNCompare("seed",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing value",option);
                }
              srand((unsigned int) atoi(argv[i]));
              break;
            }
          if (LocaleNCompare("segment",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing threshold",option);
                }
              break;
            }
          if (LocaleNCompare("shade",option+1,5) == 0)
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%d",&x))
                MagickError(OptionError,"Missing azimuth",option);
              break;
            }
          if (LocaleNCompare("sharpen",option+1,5) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing order",option);
                }
              break;
            }
          if (LocaleNCompare("shear",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing geometry",option);
                }
              break;
            }
          if (LocaleNCompare("size",option+1,2) == 0)
            {
              CloneString(&image_info->size,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    MagickError(OptionError,"Missing geometry",option);
                  (void) CloneString(&image_info->size,argv[i]);
                }
              break;
            }
          if (LocaleNCompare("solarize",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing threshold",option);
                }
              break;
            }
          if (LocaleNCompare("spread",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing amount",option);
                }
              break;
            }
          if (LocaleCompare("stroke",option+1) == 0)
            {
              (void) QueryColorDatabase("none",&image_info->stroke);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing color",option);
                  (void) QueryColorDatabase(argv[i],&image_info->stroke);
                }
              break;
            }
          if (LocaleNCompare("swirl",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%lf",&sans))
                    MagickError(OptionError,"Missing degrees",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 't':
        {
          if (LocaleNCompare("texture",option+1,5) == 0)
            {
              CloneString(&image_info->texture,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing filename",option);
                  (void) CloneString(&image_info->texture,argv[i]);
                }
              break;
            }
          if (LocaleNCompare("threshold",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing value",option);
                }
              break;
            }
          if (LocaleNCompare("transparent",option+1,7) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing transparent color",option);
                }
              break;
            }
          if (LocaleNCompare("treedepth",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    MagickError(OptionError,"Missing depth",option);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'u':
        {
          if (LocaleNCompare("units",option+1,3) == 0)
            {
              image_info->units=UndefinedResolution;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing type",option);
                  option=argv[i];
                  image_info->units=UndefinedResolution;
                  if (LocaleCompare("PixelsPerInch",option) == 0)
                    image_info->units=PixelsPerInchResolution;
                  if (LocaleCompare("PixelsPerCentimeter",option) == 0)
                    image_info->units=PixelsPerCentimeterResolution;
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'v':
        {
          if (LocaleNCompare("verbose",option+1,2) == 0)
            {
              image_info->verbose=(*option == '-');
              break;
            }
          if (LocaleNCompare("view",option+1,3) == 0)
            {
              CloneString(&image_info->view,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    MagickError(OptionError,"Missing view transform",option);
                  (void) CloneString(&image_info->view,argv[i]);
                }
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case 'w':
        {
          if (LocaleCompare("wave",option+1) == 0)
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%d",&x))
                MagickError(OptionError,"Missing amplitude",option);
              break;
            }
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
        case '?':
        {
          Usage(client_name);
          break;
        }
        default:
        {
          MagickError(OptionError,"Unrecognized option",option);
          break;
        }
      }
  }
  if ((i != (argc-1)) || (image == (Image *) NULL))
    MagickError(OptionError,"Missing an image file name",(char *) NULL);
  /*
    Write images.
  */
  if ((Extent(option) > 2) && ((*option == '-') || (*option == '+')))
    {
      status=MogrifyImages(image_info,i,argv,&image);
      if (status == False)
        CatchImageException(image);
    }
  while (image->previous != (Image *) NULL)
    image=image->previous;
  if (append != 0)
    {
      Image
        *append_image;

      /*
        Append an image sequence.
      */
      append_image=AppendImages(image,append == 1,&exception);
      if (append_image != (Image *) NULL)
        {
          DestroyImages(image);
          image=append_image;
        }
    }
  if (average)
    {
      Image
        *average_image;

      /*
        Average an image sequence.
      */
      average_image=AverageImages(image,&exception);
      if (average_image != (Image *) NULL)
        {
          DestroyImages(image);
          image=average_image;
        }
    }
  if (coalesce)
    {
      Image
        *coalesce_image;

      /*
        Coalesce an image sequence.
      */
      coalesce_image=CoalesceImages(image,&exception);
      if (coalesce_image != (Image *) NULL)
        {
          DestroyImages(image);
          image=coalesce_image;
        }
    }
  if (deconstruct)
    {
      Image
        *deconstruct_image;

      /*
        Deconstruct an image sequence.
      */
      deconstruct_image=DeconstructImages(image,&exception);
      if (deconstruct_image != (Image *) NULL)
        {
          DestroyImages(image);
          image=deconstruct_image;
        }
    }
  if (morph != 0)
    {
      Image
        *morph_image;

      /*
        Morph an image sequence.
      */
      morph_image=MorphImages(image,morph,&exception);
      if (morph_image != (Image *) NULL)
        {
          DestroyImages(image);
          image=morph_image;
        }
    }
  if (mosaic)
    {
      Image
        *mosaic_image;

      /*
        Create an image mosaic.
      */
      mosaic_image=MosaicImages(image,&exception);
      if (mosaic_image != (Image *) NULL)
        {
          DestroyImages(image);
          image=mosaic_image;
        }
    }
  if (global_colormap)
    (void) MapImages(image,(Image *) NULL,image_info->dither);
  /*
    Write converted image.
  */
  (void) strcpy(image_info->filename,argv[i]);
  for (p=image; p != (Image *) NULL; p=p->next)
  {
    (void) strcpy(p->filename,argv[i]);
    p->scene=scene++;
  }
  SetImageInfo(image_info,True);
  for (p=image; p != (Image *) NULL; p=p->next)
  {
#if defined(CONVERT_MAIN)
    if (header_data != (char *) NULL)
      fwrite((char *) header_data,1,header_length,stdout);
#endif
    status=WriteImage(image_info,p);
    if (status == False)
      CatchImageException(p);
    if (image_info->adjoin)
      break;
  }
  if (image_info->verbose)
    DescribeImage(image,stderr,False);
  DestroyImages(image);
  DestroyImageInfo(image_info);
#if !defined(CONVERT_MAIN)
  FreeMemory((void **) &argv);
  Exit(0);
  return(False);
#else
  return(True);
#endif
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   C o n c a t e n t a t e I m a g e s                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method ConcatentateImages reads each file in sequence and writes it to a
%  single file.  It is required by the delegates subsystem.
%
%  The format of the ConcatentateImages method is:
%
%      void ConcatenateImages(int argc,char **argv)
%
%  A description of each parameter follows:
%
%    o argc: Specifies a pointer to an integer describing the number of
%      elements in the argument vector.
%
%    o argv: Specifies a pointer to a text array containing the command line
%      arguments.
%
%
*/
void ConcatenateImages(int argc,char **argv)
{
  FILE
    *input,
    *output;

  register int
    c,
    i;

  /*
    Open output file.
  */
  output=fopen(argv[argc-1],"wb");
  if (output == (FILE *) NULL)
    MagickError(FileOpenError,"Unable to open file",argv[argc-1]);
  for (i=2; i < (argc-1); i++)
  {
    input=fopen(argv[i],"rb");
    if (input == (FILE *) NULL)
      {
        MagickWarning(FileOpenWarning,"Unable to open file",argv[i]);
        continue;
      }
    for (c=fgetc(input); c != EOF; c=fgetc(input))
      (void) fputc((char) c,output);
    (void) fclose(input);
    (void) remove(argv[i]);
  }
  (void) fclose(output);
  Exit(0);
}

