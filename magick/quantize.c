/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%           QQQ   U   U   AAA   N   N  TTTTT  IIIII   ZZZZZ  EEEEE            %
%          Q   Q  U   U  A   A  NN  N    T      I        ZZ  E                %
%          Q   Q  U   U  AAAAA  N N N    T      I      ZZZ   EEEEE            %
%          Q  QQ  U   U  A   A  N  NN    T      I     ZZ     E                %
%           QQQQ   UUU   A   A  N   N    T    IIIII   ZZZZZ  EEEEE            %
%                                                                             %
%                                                                             %
%         Methods to Reduce the Number of Unique Colors in an Image           %
%                                                                             %
%                                                                             %
%                           Software Design                                   %
%                             John Cristy                                     %
%                              July 1992                                      %
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
%  Realism in computer graphics typically requires using 24 bits/pixel to
%  generate an image.  Yet many graphic display devices do not contain
%  the amount of memory necessary to match the spatial and color
%  resolution of the human eye.  The QUANTIZE program takes a 24 bit
%  image and reduces the number of colors so it can be displayed on
%  raster device with less bits per pixel.  In most instances, the
%  quantized image closely resembles the original reference image.
%
%  A reduction of colors in an image is also desirable for image
%  transmission and real-time animation.
%
%  Method Quantize takes a standard RGB or monochrome images and quantizes
%  them down to some fixed number of colors.
%
%  For purposes of color allocation, an image is a set of n pixels, where
%  each pixel is a point in RGB space.  RGB space is a 3-dimensional
%  vector space, and each pixel, pi,  is defined by an ordered triple of
%  red, green, and blue coordinates, (ri, gi, bi).
%
%  Each primary color component (red, green, or blue) represents an
%  intensity which varies linearly from 0 to a maximum value, cmax, which
%  corresponds to full saturation of that color.  Color allocation is
%  defined over a domain consisting of the cube in RGB space with
%  opposite vertices at (0,0,0) and (cmax,cmax,cmax).  QUANTIZE requires
%  cmax = 255.
%
%  The algorithm maps this domain onto a tree in which each node
%  represents a cube within that domain.  In the following discussion
%  these cubes are defined by the coordinate of two opposite vertices:
%  The vertex nearest the origin in RGB space and the vertex farthest
%  from the origin.
%
%  The tree's root node represents the the entire domain, (0,0,0) through
%  (cmax,cmax,cmax).  Each lower level in the tree is generated by
%  subdividing one node's cube into eight smaller cubes of equal size.
%  This corresponds to bisecting the parent cube with planes passing
%  through the midpoints of each edge.
%
%  The basic algorithm operates in three phases: Classification,
%  Reduction, and Assignment.  Classification builds a color
%  description tree for the image.  Reduction collapses the tree until
%  the number it represents, at most, the number of colors desired in the
%  output image.  Assignment defines the output image's color map and
%  sets each pixel's color by reclassification in the reduced tree.
%  Our goal is to minimize the numerical discrepancies between the original
%  colors and quantized colors (quantization error).
%
%  Classification begins by initializing a color description tree of
%  sufficient depth to represent each possible input color in a leaf.
%  However, it is impractical to generate a fully-formed color
%  description tree in the classification phase for realistic values of
%  cmax.  If colors components in the input image are quantized to k-bit
%  precision, so that cmax= 2k-1, the tree would need k levels below the
%  root node to allow representing each possible input color in a leaf.
%  This becomes prohibitive because the tree's total number of nodes is
%  1 + sum(i=1,k,8k).
%
%  A complete tree would require 19,173,961 nodes for k = 8, cmax = 255.
%  Therefore, to avoid building a fully populated tree, QUANTIZE: (1)
%  Initializes data structures for nodes only as they are needed;  (2)
%  Chooses a maximum depth for the tree as a function of the desired
%  number of colors in the output image (currently log2(colormap size)).
%
%  For each pixel in the input image, classification scans downward from
%  the root of the color description tree.  At each level of the tree it
%  identifies the single node which represents a cube in RGB space
%  containing the pixel's color.  It updates the following data for each
%  such node:
%
%    n1: Number of pixels whose color is contained in the RGB cube
%    which this node represents;
%
%    n2: Number of pixels whose color is not represented in a node at
%    lower depth in the tree;  initially,  n2 = 0 for all nodes except
%    leaves of the tree.
%
%    Sr, Sg, Sb: Sums of the red, green, and blue component values for
%    all pixels not classified at a lower depth. The combination of
%    these sums and n2  will ultimately characterize the mean color of a
%    set of pixels represented by this node.
%
%    E: The distance squared in RGB space between each pixel contained
%    within a node and the nodes' center.  This represents the quantization
%    error for a node.
%
%  Reduction repeatedly prunes the tree until the number of nodes with
%  n2 > 0 is less than or equal to the maximum number of colors allowed
%  in the output image.  On any given iteration over the tree, it selects
%  those nodes whose E count is minimal for pruning and merges their
%  color statistics upward. It uses a pruning threshold, Ep, to govern
%  node selection as follows:
%
%    Ep = 0
%    while number of nodes with (n2 > 0) > required maximum number of colors
%      prune all nodes such that E <= Ep
%      Set Ep to minimum E in remaining nodes
%
%  This has the effect of minimizing any quantization error when merging
%  two nodes together.
%
%  When a node to be pruned has offspring, the pruning procedure invokes
%  itself recursively in order to prune the tree from the leaves upward.
%  n2,  Sr, Sg,  and  Sb in a node being pruned are always added to the
%  corresponding data in that node's parent.  This retains the pruned
%  node's color characteristics for later averaging.
%
%  For each node, n2 pixels exist for which that node represents the
%  smallest volume in RGB space containing those pixel's colors.  When n2
%  > 0 the node will uniquely define a color in the output image. At the
%  beginning of reduction,  n2 = 0  for all nodes except a the leaves of
%  the tree which represent colors present in the input image.
%
%  The other pixel count, n1, indicates the total number of colors
%  within the cubic volume which the node represents.  This includes n1 -
%  n2  pixels whose colors should be defined by nodes at a lower level in
%  the tree.
%
%  Assignment generates the output image from the pruned tree.  The
%  output image consists of two parts: (1)  A color map, which is an
%  array of color descriptions (RGB triples) for each color present in
%  the output image;  (2)  A pixel array, which represents each pixel as
%  an index into the color map array.
%
%  First, the assignment phase makes one pass over the pruned color
%  description tree to establish the image's color map.  For each node
%  with n2  > 0, it divides Sr, Sg, and Sb by n2 .  This produces the
%  mean color of all pixels that classify no lower than this node.  Each
%  of these colors becomes an entry in the color map.
%
%  Finally,  the assignment phase reclassifies each pixel in the pruned
%  tree to identify the deepest node containing the pixel's color.  The
%  pixel's value in the pixel array becomes the index of this node's mean
%  color in the color map.
%
%  For efficiency, QUANTIZE requires that the reference image be in a
%  run-length encoded format.
%
%  This method is based on a similar algorithm written by Paul Raveling.
%
%
*/

/*
  Include declarations.
*/
#include "magick.h"
#include "defines.h"

/*
  Define declarations.
*/
#define CacheShift  (QuantumDepth-6)
#define ExceptionQueueLength  16
#define MaxNodes  266817

/*
  Typdef declarations.
*/
typedef struct _ExceptionPacket
{
  double
    red,
    green,
    blue;
} ExceptionPacket;

typedef struct _NodeInfo
{
  double
    number_unique,
    total_red,
    total_green,
    total_blue,
    quantization_error;

  unsigned int
    color_number;

  unsigned char
    id,
    level,
    census;

  struct _NodeInfo
    *parent,
    *child[8];
} NodeInfo;

typedef struct _Nodes
{
  NodeInfo
    nodes[NodesInAList];

  struct _Nodes
    *next;
} Nodes;

typedef struct _CubeInfo
{
  NodeInfo
    *root;

  unsigned int
    depth;

  unsigned long
    colors;

  PixelPacket
    color,
    *colormap;

  double
    distance,
    pruning_threshold,
    next_pruning_threshold,
    *squares;

  unsigned int
    nodes,
    free_nodes,
    color_number;

  NodeInfo
    *next_node;

  Nodes
    *node_queue;

  int
    x,
    y,
    *cache;

  ExceptionPacket
    error[ExceptionQueueLength];

  double
    weights[ExceptionQueueLength];

  const QuantizeInfo
    *quantize_info;
} CubeInfo;

/*
  Const declarations.
*/
const long
  MaxColormapSize = 65535L;

/*
  Method prototypes.
*/
static NodeInfo
  *GetNodeInfo(CubeInfo *,const unsigned int,const unsigned int,NodeInfo *);

static unsigned int
  DitherImage(CubeInfo *,Image *);

static void
  ClosestColor(CubeInfo *,const NodeInfo *),
  DefineColormap(CubeInfo *,NodeInfo *),
  HilbertCurve(CubeInfo *,Image *,const int,const unsigned int),
  PruneLevel(CubeInfo *,const NodeInfo *),
  Reduction(CubeInfo *,const unsigned int);

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   A s s i g n m e n t                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method Assignment generates the output image from the pruned tree.  The
%  output image consists of two parts: (1)  A color map, which is an
%  array of color descriptions (RGB triples) for each color present in
%  the output image;  (2)  A pixel array, which represents each pixel as
%  an index into the color map array.
%
%  First, the assignment phase makes one pass over the pruned color
%  description tree to establish the image's color map.  For each node
%  with n2  > 0, it divides Sr, Sg, and Sb by n2 .  This produces the
%  mean color of all pixels that classify no lower than this node.  Each
%  of these colors becomes an entry in the color map.
%
%  Finally,  the assignment phase reclassifies each pixel in the pruned
%  tree to identify the deepest node containing the pixel's color.  The
%  pixel's value in the pixel array becomes the index of this node's mean
%  color in the color map.
%
%  The format of the Assignment method is:
%
%      unsigned int Assignment(CubeInfo *cube_info,Image *image)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int Assignment(CubeInfo *cube_info,Image *image)
{
#define AssignImageText  "  Assigning image colors...  "

  IndexPacket
    index;

  int
    y;

  register IndexPacket
    *indexes;

  register int
    x;

  register const NodeInfo
    *node_info;

  register PixelPacket
    *q;

  unsigned int
    dither,
    id;

  /*
    Allocate image colormap.
  */
  if (image->colormap == (PixelPacket *) NULL)
    image->colormap=(PixelPacket *)
      AllocateMemory(cube_info->colors*sizeof(PixelPacket));
  else
    image->colormap=(PixelPacket *) ReallocateMemory((char *) image->colormap,
      cube_info->colors*sizeof(PixelPacket));
  if (image->colormap == (PixelPacket *) NULL)
    ThrowBinaryException(ResourceLimitWarning,"Unable to quantize image",
      "Memory allocation failed");
  cube_info->colormap=image->colormap;
  cube_info->colors=0;
  DefineColormap(cube_info,cube_info->root);
  if ((cube_info->quantize_info->colorspace != TransparentColorspace) &&
      (image->colorspace != CMYKColorspace))
    image->class=PseudoClass;
  image->colors=cube_info->colors;
  /*
    Create a reduced color image.
  */
  dither=cube_info->quantize_info->dither;
  if (dither)
    dither=DitherImage(cube_info,image);
  if (!dither)
    for (y=0; y < (int) image->rows; y++)
    {
      q=GetPixelCache(image,0,y,image->columns,1);
      if (q == (PixelPacket *) NULL)
        break;
      indexes=GetIndexes(image->cache);
      for (x=0; x < (int) image->columns; x++)
      {
        /*
          Identify the deepest node containing the pixel's color.
        */
        node_info=cube_info->root;
        for (index=MaxTreeDepth-1; (int) index > 0; index--)
        {
          id=((DownScale(q->red) >> index) & 0x01) << 2 |
             ((DownScale(q->green) >> index) & 0x01) << 1 |
             ((DownScale(q->blue) >> index) & 0x01);
          if ((node_info->census & (1 << id)) == 0)
            break;
          node_info=node_info->child[id];
        }
        /*
          Find closest color among siblings and their children.
        */
        cube_info->color.red=q->red;
        cube_info->color.green=q->green;
        cube_info->color.blue=q->blue;
        cube_info->distance=3.0*(MaxRGB+1)*(MaxRGB+1);
        ClosestColor(cube_info,node_info->parent);
        index=cube_info->color_number;
        if (image->class == PseudoClass)
          indexes[x]=index;
        if (!cube_info->quantize_info->measure_error)
          {
            q->red=image->colormap[index].red;
            q->green=image->colormap[index].green;
            q->blue=image->colormap[index].blue;
          }
        q++;
      }
      if (!SyncPixelCache(image))
        break;
      if (QuantumTick(y,image->rows))
        ProgressMonitor(AssignImageText,y,image->rows);
    }
  if ((cube_info->quantize_info->number_colors == 2) &&
      (cube_info->quantize_info->colorspace == GRAYColorspace))
    {
      unsigned int
        polarity;

      /*
        Monochrome image.
      */
      polarity=Intensity(image->colormap[0]) > Intensity(image->colormap[1]);
      image->colormap[polarity].red=0;
      image->colormap[polarity].green=0;
      image->colormap[polarity].blue=0;
      image->colormap[!polarity].red=MaxRGB;
      image->colormap[!polarity].green=MaxRGB;
      image->colormap[!polarity].blue=MaxRGB;
    }
  if (cube_info->quantize_info->measure_error)
    {
      QuantizationError(image);
      SyncImage(image);
    }
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   C l a s s i f i c a t i o n                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method Classification begins by initializing a color description tree
%  of sufficient depth to represent each possible input color in a leaf.
%  However, it is impractical to generate a fully-formed color
%  description tree in the classification phase for realistic values of
%  cmax.  If colors components in the input image are quantized to k-bit
%  precision, so that cmax= 2k-1, the tree would need k levels below the
%  root node to allow representing each possible input color in a leaf.
%  This becomes prohibitive because the tree's total number of nodes is
%  1 + sum(i=1,k,8k).
%
%  A complete tree would require 19,173,961 nodes for k = 8, cmax = 255.
%  Therefore, to avoid building a fully populated tree, QUANTIZE: (1)
%  Initializes data structures for nodes only as they are needed;  (2)
%  Chooses a maximum depth for the tree as a function of the desired
%  number of colors in the output image (currently log2(colormap size)).
%
%  For each pixel in the input image, classification scans downward from
%  the root of the color description tree.  At each level of the tree it
%  identifies the single node which represents a cube in RGB space
%  containing It updates the following data for each such node:
%
%    n1 : Number of pixels whose color is contained in the RGB cube
%    which this node represents;
%
%    n2 : Number of pixels whose color is not represented in a node at
%    lower depth in the tree;  initially,  n2 = 0 for all nodes except
%    leaves of the tree.
%
%    Sr, Sg, Sb : Sums of the red, green, and blue component values for
%    all pixels not classified at a lower depth. The combination of
%    these sums and n2  will ultimately characterize the mean color of a
%    set of pixels represented by this node.
%
%    E: The distance squared in RGB space between each pixel contained
%    within a node and the nodes' center.  This represents the quantization
%    error for a node.
%
%  The format of the Classification method is:
%
%      unsigned int Classification(CubeInfo *cube_info,Image *image)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int Classification(CubeInfo *cube_info,Image *image)
{
#define ClassifyImageText  "  Classifying image colors...  "

  double
    bisect[MaxTreeDepth+1],
    mid_red,
    mid_green,
    mid_blue;

  int
    y;

  register double
    distance_squared,
    *squares;

  register int
    i,
    x;

  register NodeInfo
    *node_info;

  register PixelPacket
    *p;

  register unsigned int
    index;

  unsigned int
    id,
    level;

  bisect[0]=(MaxRGB+1.0)/2.0;
  for (i=1; i <= MaxTreeDepth; i++)
    bisect[i]=0.5*bisect[i-1];
  squares=cube_info->squares;
  cube_info->root->quantization_error+=
    3.0*(MaxRGB/2.0)*(MaxRGB/2.0)*image->columns*image->rows;
  for (y=0; y < (int) image->rows; y++)
  {
    p=GetPixelCache(image,0,y,image->columns,1);
    if (p == (PixelPacket *) NULL)
      break;
    if ((int) cube_info->nodes > MaxNodes)
      {
        /*
          Prune one level if the color tree is too large.
        */
        PruneLevel(cube_info,cube_info->root);
        cube_info->depth--;
      }
    for (x=0; x < (int) image->columns; x++)
    {
      /*
        Start at the root and descend the color cube tree.
      */
      node_info=cube_info->root;
      index=MaxTreeDepth-1;
      mid_red=MaxRGB/2.0;
      mid_green=MaxRGB/2.0;
      mid_blue=MaxRGB/2.0;
      for (level=1; level <= cube_info->depth; level++)
      {
        id=((DownScale(p->red) >> index) & 0x01) << 2 |
           ((DownScale(p->green) >> index) & 0x01) << 1 |
           ((DownScale(p->blue) >> index) & 0x01);
        mid_red+=id & 4 ? bisect[level] : -bisect[level];
        mid_green+=id & 2 ? bisect[level] : -bisect[level];
        mid_blue+=id & 1 ? bisect[level] : -bisect[level];
        if (node_info->child[id] == (NodeInfo *) NULL)
          {
            /*
              Set colors of new node to contain pixel.
            */
            node_info->census|=1 << id;
            node_info->child[id]=GetNodeInfo(cube_info,id,level,node_info);
            if (node_info->child[id] == (NodeInfo *) NULL)
              ThrowBinaryException(ResourceLimitWarning,
                "Unable to quantize image","Memory allocation failed");
            if (level == cube_info->depth)
              cube_info->colors++;
          }
        node_info=node_info->child[id];
        if (level != MaxTreeDepth)
          {
            /*
              Approximate the quantization error represented by this node.
            */
            distance_squared=squares[(int) (p->red-mid_red)]+
              squares[(int) (p->green-mid_green)]+
              squares[(int) (p->blue-mid_blue)];
            node_info->quantization_error+=distance_squared;
          }
        index--;
      }
      /*
        Sum RGB for this leaf for later derivation of the mean cube color.
      */
      node_info->number_unique++;
      node_info->total_red+=p->red;
      node_info->total_green+=p->green;
      node_info->total_blue+=p->blue;
      p++;
    }
    if (QuantumTick(y,image->rows))
      ProgressMonitor(ClassifyImageText,y,image->rows);
  }
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   C l o n e Q u a n t i z e I n f o                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method CloneQuantizeInfo makes a duplicate of the given quantize info
%  structure, or if quantize info is NULL, a new one.
%
%  The format of the CloneQuantizeInfo method is:
%
%      QuantizeInfo *CloneQuantizeInfo(const QuantizeInfo *quantize_info)
%
%  A description of each parameter follows:
%
%    o clone_info: Method CloneQuantizeInfo returns a duplicate of the given
%      quantize info, or if image info is NULL a new one.
%
%    o quantize_info: a structure of type info.
%
%
*/
Export QuantizeInfo *CloneQuantizeInfo(const QuantizeInfo *quantize_info)
{
  QuantizeInfo
    *clone_info;

  clone_info=(QuantizeInfo *) AllocateMemory(sizeof(QuantizeInfo));
  if (clone_info == (QuantizeInfo *) NULL)
    MagickError(ResourceLimitError,"Unable to clone quantize info",
      "Memory allocation failed");
  if (quantize_info == (QuantizeInfo *) NULL)
    {
      GetQuantizeInfo(clone_info);
      return(clone_info);
    }
  *clone_info=(*quantize_info);
  return(clone_info);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   C l o s e s t C o l o r                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method ClosestColor traverses the color cube tree at a particular node
%  and determines which colormap entry best represents the input color.
%
%  The format of the ClosestColor method is:
%
%      void ClosestColor(CubeInfo *cube_info,const NodeInfo *node_info)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o node_info: The address of a structure of type NodeInfo which points to a
%      node in the color cube tree that is to be pruned.
%
%
*/
static void ClosestColor(CubeInfo *cube_info,const NodeInfo *node_info)
{
  register unsigned int
    id;

  if (cube_info->distance != 0.0)
    {
      /*
        Traverse any children.
      */
      if (node_info->census != 0)
        for (id=0; id < MaxTreeDepth; id++)
          if (node_info->census & (1 << id))
            ClosestColor(cube_info,node_info->child[id]);
      if (node_info->number_unique != 0)
        {
          register PixelPacket
            *color;

          register double
            distance_squared,
            *squares;

          /*
            Determine if this color is "closest".
          */
          squares=cube_info->squares;
          color=cube_info->colormap+node_info->color_number;
          distance_squared=squares[(int) (color->red-cube_info->color.red)]+
            squares[(int) (color->green-cube_info->color.green)]+
            squares[(int) (color->blue-cube_info->color.blue)];
          if (distance_squared < cube_info->distance)
            {
              cube_info->distance=distance_squared;
              cube_info->color_number=node_info->color_number;
            }
        }
    }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   D e f i n e C o l o r m a p                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method DefineColormap traverses the color cube tree and notes each
%  colormap entry.  A colormap entry is any node in the color cube tree where
%  the number of unique colors is not zero.
%
%  The format of the DefineColormap method is:
%
%      DefineColormap(CubeInfo *cube_info,NodeInfo *node_info)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o node_info: The address of a structure of type NodeInfo which points to a
%      node in the color cube tree that is to be pruned.
%
%
*/
static void DefineColormap(CubeInfo *cube_info,NodeInfo *node_info)
{
  register double
    number_unique;

  register unsigned int
    id;

  /*
    Traverse any children.
  */
  if (node_info->census != 0)
    for (id=0; id < MaxTreeDepth; id++)
      if (node_info->census & (1 << id))
        DefineColormap(cube_info,node_info->child[id]);
  if (node_info->number_unique != 0)
    {
      /*
        Colormap entry is defined by the mean color in this cube.
      */
      number_unique=node_info->number_unique;
      cube_info->colormap[cube_info->colors].red=
        (node_info->total_red+number_unique/2)/number_unique;
      cube_info->colormap[cube_info->colors].green=
        (node_info->total_green+number_unique/2)/number_unique;
      cube_info->colormap[cube_info->colors].blue=
        (node_info->total_blue+number_unique/2)/number_unique;
      node_info->color_number=cube_info->colors++;
    }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   D e s t r o y C u b e I n f o                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method DestroyCubeInfo deallocates memory associated with an image.
%
%  The format of the DestroyCubeInfo method is:
%
%      DestroyCubeInfo(CubeInfo *cube_info)
%
%  A description of each parameter follows:
%
%    o cube_info: The address of a structure of type CubeInfo.
%
%
*/
static void DestroyCubeInfo(CubeInfo *cube_info)
{
  register Nodes
    *nodes;

  /*
    Release color cube tree storage.
  */
  do
  {
    nodes=cube_info->node_queue->next;
    FreeMemory(cube_info->node_queue);
    cube_info->node_queue=nodes;
  } while (cube_info->node_queue != (Nodes *) NULL);
  cube_info->squares-=MaxRGB;
  FreeMemory(cube_info->squares);
  if (!cube_info->quantize_info->dither)
    return;
  /*
    Free memory resources.
  */
  FreeMemory(cube_info->cache);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   D e s t r o y Q u a n t i z e I n f o                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method DestroyQuantizeInfo deallocates memory associated with an
%  QuantizeInfo structure.
%
%  The format of the DestroyQuantizeInfo method is:
%
%      DestroyQuantizeInfo(QuantizeInfo *quantize_info)
%
%  A description of each parameter follows:
%
%    o quantize_info: Specifies a pointer to an QuantizeInfo structure.
%
%
*/
Export void DestroyQuantizeInfo(QuantizeInfo *quantize_info)
{
  assert(quantize_info != (QuantizeInfo *) NULL);
  FreeMemory((QuantizeInfo *) quantize_info);
  quantize_info=(QuantizeInfo *) NULL;
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   D i t h e r                                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method Dither distributes the difference between an original image and the
%  corresponding color reduced algorithm to neighboring pixels along a Hilbert
%  curve.
%
%  The format of the Dither method is:
%
%      unsigned int Dither(CubeInfo *cube_info,Image *image,
%        const unsigned int direction)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%    o direction:  This unsigned direction describes which direction
%      to move to next to follow the Hilbert curve.
%
*/
static unsigned int Dither(CubeInfo *cube_info,Image *image,
  const unsigned int direction)
{
  double
    blue_error,
    green_error,
    red_error;

  IndexPacket
    index;

  Quantum
    blue,
    green,
    red;

  register IndexPacket
    *indexes;

  register CubeInfo
    *p;

  register int
    i;

  register PixelPacket
    *q;

  p=cube_info;
  if ((p->x >= 0) && (p->x < (int) image->columns) &&
      (p->y >= 0) && (p->y < (int) image->rows))
    {
      /*
        Distribute error.
      */
      q=GetPixelCache(image,p->x,p->y,1,1);
      if (q == (PixelPacket *) NULL)
        return(False);
      indexes=GetIndexes(image->cache);
      red_error=q->red;
      green_error=q->green;
      blue_error=q->blue;
      for (i=0; i < ExceptionQueueLength; i++)
      {
        red_error+=p->error[i].red*p->weights[i];
        green_error+=p->error[i].green*p->weights[i];
        blue_error+=p->error[i].blue*p->weights[i];
      }
      red=(red_error < 0) ? 0 :
        (red_error > MaxRGB) ? MaxRGB : red_error+0.5;
      green=(green_error < 0) ? 0 :
        (green_error > MaxRGB) ? MaxRGB : green_error+0.5;
      blue=(blue_error < 0) ? 0 :
	(blue_error > MaxRGB) ? MaxRGB : blue_error+0.5;
      i=(blue >> CacheShift) << 12 | (green >> CacheShift) << 6 |
        (red >> CacheShift);
      if (p->cache[i] < 0)
        {
          register NodeInfo
            *node_info;

          register unsigned int
            id;

          /*
            Identify the deepest node containing the pixel's color.
          */
          node_info=p->root;
          for (index=MaxTreeDepth-1; (int) index > 0; index--)
          {
            id=((DownScale(red) >> index) & 0x01) << 2 |
               ((DownScale(green) >> index) & 0x01) << 1 |
               ((DownScale(blue) >> index) & 0x01);
            if ((node_info->census & (1 << id)) == 0)
              break;
            node_info=node_info->child[id];
          }
          /*
            Find closest color among siblings and their children.
          */
          p->color.red=red;
          p->color.green=green;
          p->color.blue=blue;
          p->distance=3.0*(MaxRGB+1)*(MaxRGB+1);
          ClosestColor(p,node_info->parent);
          p->cache[i]=p->color_number;
        }
      /*
        Assign pixel to closest colormap entry.
      */
      index=p->cache[i];
      if (image->class == PseudoClass)
        *indexes=index;
      if (!cube_info->quantize_info->measure_error)
        {
          q->red=image->colormap[index].red;
          q->green=image->colormap[index].green;
          q->blue=image->colormap[index].blue;
        }
      if (!SyncPixelCache(image))
        return(False);
      /*
        Propagate the error as the last entry of the error queue.
      */
      for (i=0; i < (ExceptionQueueLength-1); i++)
        p->error[i]=p->error[i+1];
      p->error[i].red=(int) (red-image->colormap[index].red);
      p->error[i].green=(int) (green-image->colormap[index].green);
      p->error[i].blue=(int) (blue-image->colormap[index].blue);
    }
  switch (direction)
  {
    case WestGravity: p->x--; break;
    case EastGravity: p->x++; break;
    case NorthGravity: p->y--; break;
    case SouthGravity: p->y++; break;
  }
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   D i t h e r I m a g e                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method DitherImage distributes the difference between an original image and
%  the corresponding color reduced algorithm to neighboring pixels along a
%  Hilbert curve.  DitherImage returns True if the image is dithered
%  otherwise False.
%
%  This algorithm is strongly based on a similar algorithm by Thiadmer
%  Riemersma.
%
%  The format of the DitherImage method is:
%
%      unsigned int DitherImage(CubeInfo *cube_info,Image *image)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int DitherImage(CubeInfo *cube_info,Image *image)
{
  register int
    i;

  unsigned int
    depth;

  /*
    Initialize error queue.
  */
  for (i=0; i < ExceptionQueueLength; i++)
  {
    cube_info->error[i].red=0.0;
    cube_info->error[i].green=0.0;
    cube_info->error[i].blue=0.0;
  }
  /*
    Distribute quantization error along a Hilbert curve.
  */
  cube_info->x=0;
  cube_info->y=0;
  i=image->columns > image->rows ? image->columns : image->rows;
  for (depth=1; i != 0; depth++)
    i>>=1;
  HilbertCurve(cube_info,image,depth-1,NorthGravity);
  (void) Dither(cube_info,image,ForgetGravity);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   G e t C u b e I n f o                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method GetCubeInfo initialize the Cube data structure.
%
%  The format of the GetCubeInfo method is:
%
%      unsigned int GetCubeInfo(CubeInfo *cube_info,
%        const QuantizeInfo *quantize_info,int depth)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o quantize_info: Specifies a pointer to an QuantizeInfo structure.
%
%    o depth: Normally, this integer value is zero or one.  A zero or
%      one tells Quantize to choose a optimal tree depth of Log4(number_colors).
%      A tree of this depth generally allows the best representation of the
%      reference image with the least amount of memory and the fastest
%      computational speed.  In some cases, such as an image with low color
%      dispersion (a few number of colors), a value other than
%      Log4(number_colors) is required.  To expand the color tree completely,
%      use a value of 8.
%
%
*/
static unsigned int GetCubeInfo(CubeInfo *cube_info,
  const QuantizeInfo *quantize_info,int depth)
{
  double
    weight;

  register int
    i;

  /*
    Initialize tree to describe color cube_info.
  */
  cube_info->node_queue=(Nodes *) NULL;
  cube_info->nodes=0;
  cube_info->free_nodes=0;
  if (depth > MaxTreeDepth)
    depth=MaxTreeDepth;
  if (depth < 2)
    depth=2;
  cube_info->depth=depth;
  /*
    Initialize root node.
  */
  cube_info->root=GetNodeInfo(cube_info,0,0,(NodeInfo *) NULL);
  cube_info->squares=(double *)
    AllocateMemory((MaxRGB+MaxRGB+1)*sizeof(double));
  if ((cube_info->root == (NodeInfo *) NULL) ||
      (cube_info->squares == (double *) NULL))
    return(False);
  cube_info->root->parent=cube_info->root;
  cube_info->root->quantization_error=0.0;
  cube_info->colors=0;
  cube_info->squares+=MaxRGB;
  for (i=(-MaxRGB); i <= MaxRGB; i++)
    cube_info->squares[i]=i*i;
  cube_info->quantize_info=quantize_info;
  if (!cube_info->quantize_info->dither)
    return(True);
  /*
    Initialize dither resources.
  */
  cube_info->cache=(int *) AllocateMemory((1 << 18)*sizeof(int));
  if (cube_info->cache == (int *) NULL)
    return(False);
  /*
    Initialize color cache.
  */
  for (i=0; i < (1 << 18); i++)
    cube_info->cache[i]=(-1);
  /*
    Distribute weights along a curve of exponential decay.
  */
  weight=1.0;
  for (i=0; i < ExceptionQueueLength; i++)
  {
    cube_info->weights[ExceptionQueueLength-i-1]=1.0/weight;
    weight*=exp(log((double) MaxRGB+1.0)/(ExceptionQueueLength-1.0));
  }
  /*
    Normalize the weighting factors.
  */
  weight=0.0;
  for (i=0; i < ExceptionQueueLength; i++)
    weight+=cube_info->weights[i];
  for (i=0; i < ExceptionQueueLength; i++)
    cube_info->weights[i]/=weight;
  if (QuantumDepth == 16)
    for (i=0; i < ExceptionQueueLength; i++)
      cube_info->weights[i]/=256.0;
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   G e t N o d e I n f o                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method GetNodeInfo allocates memory for a new node in the color cube
%  tree and presets all fields to zero.
%
%  The format of the GetNodeInfo method is:
%
%      NodeInfo *GetNodeInfo(CubeInfo *cube_info,const unsigned int id,
%        const unsigned int level,NodeInfo *parent)
%
%  A description of each parameter follows.
%
%    o node: The GetNodeInfo method returns this integer address.
%
%    o id: Specifies the child number of the node.
%
%    o level: Specifies the level in the classification the node resides.
%
%
*/
static NodeInfo *GetNodeInfo(CubeInfo *cube_info,const unsigned int id,
  const unsigned int level,NodeInfo *parent)
{
  register int
    i;

  NodeInfo
    *node_info;

  if (cube_info->free_nodes == 0)
    {
      Nodes
        *nodes;

      /*
        Allocate a new nodes of nodes.
      */
      nodes=(Nodes *) AllocateMemory(sizeof(Nodes));
      if (nodes == (Nodes *) NULL)
        return((NodeInfo *) NULL);
      nodes->next=cube_info->node_queue;
      cube_info->node_queue=nodes;
      cube_info->next_node=nodes->nodes;
      cube_info->free_nodes=NodesInAList;
    }
  cube_info->nodes++;
  cube_info->free_nodes--;
  node_info=cube_info->next_node++;
  node_info->parent=parent;
  for (i=0; i < MaxTreeDepth; i++)
    node_info->child[i]=(NodeInfo *) NULL;
  node_info->id=id;
  node_info->level=level;
  node_info->census=0;
  node_info->number_unique=0;
  node_info->quantization_error=0.0;
  node_info->total_red=0.0;
  node_info->total_green=0.0;
  node_info->total_blue=0.0;
  return(node_info);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   G e t Q u a n t i z e I n f o                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method GetQuantizeInfo initializes the QuantizeInfo structure.
%
%  The format of the GetQuantizeInfo method is:
%
%      GetQuantizeInfo(QuantizeInfo *quantize_info)
%
%  A description of each parameter follows:
%
%    o quantize_info: Specifies a pointer to a QuantizeInfo structure.
%
%
*/
Export void GetQuantizeInfo(QuantizeInfo *quantize_info)
{
  assert(quantize_info != (QuantizeInfo *) NULL);
  quantize_info->number_colors=0;
  quantize_info->tree_depth=0;
  quantize_info->dither=False;
  quantize_info->colorspace=RGBColorspace;
  quantize_info->measure_error=False;
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   H i l b e r t C u r v e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method HilbertCurve s a space filling curve that visits every point in a
%  square grid with any power of 2.  Hilbert is useful in dithering due to
%  the coherence between neighboring pixels.  Here, the quantization error is
%  distributed along the Hilbert curve.
%
%  The format of the HilbertCurve method is:
%
%      void HilbertCurve(CubeInfo *cube_info,Image *image,const int level,
%        const unsigned int direction)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%    o direction:  This unsigned direction describes which direction
%      to move to next to follow the Hilbert curve.
%
%
*/
static void HilbertCurve(CubeInfo *cube_info,Image *image,const int level,
  const unsigned int direction)
{
  if (level == 1)
    {
      switch (direction)
      {
        case WestGravity:
        {
          Dither(cube_info,image,EastGravity);
          Dither(cube_info,image,SouthGravity);
          Dither(cube_info,image,WestGravity);
          break;
        }
        case EastGravity:
        {
          Dither(cube_info,image,WestGravity);
          Dither(cube_info,image,NorthGravity);
          Dither(cube_info,image,EastGravity);
          break;
        }
        case NorthGravity:
        {
          Dither(cube_info,image,SouthGravity);
          Dither(cube_info,image,EastGravity);
          Dither(cube_info,image,NorthGravity);
          break;
        }
        case SouthGravity:
        {
          Dither(cube_info,image,NorthGravity);
          Dither(cube_info,image,WestGravity);
          Dither(cube_info,image,SouthGravity);
          break;
        }
        default:
          break;
      }
      return;
    }
  switch (direction)
  {
    case WestGravity:
    {
      HilbertCurve(cube_info,image,level-1,NorthGravity);
      Dither(cube_info,image,EastGravity);
      HilbertCurve(cube_info,image,level-1,WestGravity);
      Dither(cube_info,image,SouthGravity);
      HilbertCurve(cube_info,image,level-1,WestGravity);
      Dither(cube_info,image,WestGravity);
      HilbertCurve(cube_info,image,level-1,SouthGravity);
      break;
    }
    case EastGravity:
    {
      HilbertCurve(cube_info,image,level-1,SouthGravity);
      Dither(cube_info,image,WestGravity);
      HilbertCurve(cube_info,image,level-1,EastGravity);
      Dither(cube_info,image,NorthGravity);
      HilbertCurve(cube_info,image,level-1,EastGravity);
      Dither(cube_info,image,EastGravity);
      HilbertCurve(cube_info,image,level-1,NorthGravity);
      break;
    }
    case NorthGravity:
    {
      HilbertCurve(cube_info,image,level-1,WestGravity);
      Dither(cube_info,image,SouthGravity);
      HilbertCurve(cube_info,image,level-1,NorthGravity);
      Dither(cube_info,image,EastGravity);
      HilbertCurve(cube_info,image,level-1,NorthGravity);
      Dither(cube_info,image,NorthGravity);
      HilbertCurve(cube_info,image,level-1,EastGravity);
      break;
    }
    case SouthGravity:
    {
      HilbertCurve(cube_info,image,level-1,EastGravity);
      Dither(cube_info,image,NorthGravity);
      HilbertCurve(cube_info,image,level-1,SouthGravity);
      Dither(cube_info,image,WestGravity);
      HilbertCurve(cube_info,image,level-1,SouthGravity);
      Dither(cube_info,image,SouthGravity);
      HilbertCurve(cube_info,image,level-1,WestGravity);
      break;
    }
    default:
      break;
  }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   M a p I m a g e                                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  MapImage replaces the colors of an image with the closest color from
%  a reference image.
%
%  The format of the MapImage method is:
%
%      unsigned int MapImage(Image *image,Image *map_image,
%        const unsigned int dither)
%
%  A description of each parameter follows:
%
%    o image: Specifies a pointer to an Image structure.
%
%    o map_image: Specifies a pointer to a Image structure.  Reduce
%      image to a set of colors represented by this image.
%
%    o dither: Set this integer value to something other than zero to
%      dither the quantized image.
%
%
*/
Export unsigned int MapImage(Image *image,Image *map_image,
  const unsigned int dither)
{
  CubeInfo
    cube_info;

  QuantizeInfo
    quantize_info;

  unsigned int
    status;

  assert(image != (Image *) NULL);
  if (map_image == (Image *) NULL)
    return(False);
  /*
    Initialize color cube.
  */
  GetQuantizeInfo(&quantize_info);
  quantize_info.dither=dither;
  quantize_info.colorspace=image->matte ? TransparentColorspace : RGBColorspace;
  status=GetCubeInfo(&cube_info,&quantize_info,8);
  if (status == False)
    ThrowBinaryException(ResourceLimitWarning,"Unable to map image",
      "Memory allocation failed");
  status=Classification(&cube_info,map_image);
  if (status != False)
    {
      /*
        Classify image colors from the reference image.
      */
      quantize_info.number_colors=cube_info.colors;
      status=Assignment(&cube_info,image);
    }
  DestroyCubeInfo(&cube_info);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   M a p I m a g e s                                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  MapImages replaces the colors of a sequence of images with the closest
%  color from a reference image.
%
%  The format of the MapImage method is:
%
%      unsigned int MapImages(Image *images,Image *map_image,
%        const unsigned int dither)
%
%  A description of each parameter follows:
%
%    o image: Specifies a pointer to a set of Image structures.
%
%    o map_image: Specifies a pointer to a Image structure.  Reduce
%      image to a set of colors represented by this image.
%
%    o dither: Set this integer value to something other than zero to
%      dither the quantized image.
%
%
*/
Export unsigned int MapImages(Image *images,Image *map_image,
  const unsigned int dither)
{
  CubeInfo
    cube_info;

  Image
    *image;

  QuantizeInfo
    quantize_info;

  unsigned int
    status;

  assert(images != (Image *) NULL);
  if (images->next == (Image *) NULL)
    {
      /*
        Handle a single image with MapImage.
      */
      status=MapImage(images,map_image,dither);
      return(status);
    }
  GetQuantizeInfo(&quantize_info);
  quantize_info.dither=dither;
  if (map_image == (Image *) NULL)
    {
      /*
        Create a global colormap for an image sequence.
      */
      for (image=images; image != (Image *) NULL; image=image->next)
        if (image->matte)
          quantize_info.colorspace=TransparentColorspace;
      status=QuantizeImages(&quantize_info,images);
      return(status);
    }
  /*
    Classify image colors from the reference image.
  */
  status=GetCubeInfo(&cube_info,&quantize_info,8);
  if (status == False)
    ThrowBinaryException(ResourceLimitWarning,"Unable to map image sequence",
      "Memory allocation failed");
  status=Classification(&cube_info,map_image);
  if (status != False)
    {
      /*
        Classify image colors from the reference image.
      */
      quantize_info.number_colors=cube_info.colors;
      for (image=images; image != (Image *) NULL; image=image->next)
      {
        quantize_info.colorspace=
          image->matte ? TransparentColorspace : RGBColorspace;
        status=Assignment(&cube_info,image);
        if (status == False)
          break;
      }
    }
  DestroyCubeInfo(&cube_info);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   O r d e r e d D i t h e r I m a g e                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method OrderedDitherImage uses the ordered dithering technique of
%  reducing color images to monochrome using positional information to retain
%  as much information as possible.
%
%  The format of the OrderedDitherImage method is:
%
%      unsigned int OrderedDitherImage(Image *image)
%
%  A description of each parameter follows.
%
%    o image: Specifies a pointer to an Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int OrderedDitherImage(Image *image)
{
#define DitherImageText  "  Dithering image...  "

  static Quantum
    DitherMatrix[8][8] =
    {
      { UpScale(  0), UpScale(192), UpScale( 48), UpScale(240),
        UpScale( 12), UpScale(204), UpScale( 60), UpScale(252) },
      { UpScale(128), UpScale( 64), UpScale(176), UpScale(112),
        UpScale(140), UpScale( 76), UpScale(188), UpScale(124) },
      { UpScale( 32), UpScale(224), UpScale( 16), UpScale(208),
        UpScale( 44), UpScale(236), UpScale( 28), UpScale(220) },
      { UpScale(160), UpScale( 96), UpScale(144), UpScale( 80),
        UpScale(172), UpScale(108), UpScale(156), UpScale( 92) },
      { UpScale(  8), UpScale(200), UpScale( 56), UpScale(248),
        UpScale(  4), UpScale(196), UpScale( 52), UpScale(244) },
      { UpScale(136), UpScale( 72), UpScale(184), UpScale(120),
        UpScale(132), UpScale( 68), UpScale(180), UpScale(116) },
      { UpScale( 40), UpScale(232), UpScale( 24), UpScale(216),
        UpScale( 36), UpScale(228), UpScale( 20), UpScale(212) },
      { UpScale(168), UpScale(104), UpScale(152), UpScale( 88),
        UpScale(164), UpScale(100), UpScale(148), UpScale( 84) }
    };

  IndexPacket
    index;

  int
    y;

  PixelPacket
    *colormap;

  register IndexPacket
    *indexes;

  register int
    x;

  register PixelPacket
    *q;

  /*
    Initialize colormap.
  */
  NormalizeImage(image);
  image->class=PseudoClass;
  image->colors=2;
  colormap=(PixelPacket *) AllocateMemory(image->colors*sizeof(PixelPacket));
  if (colormap == (PixelPacket *) NULL)
    ThrowBinaryException(ResourceLimitWarning,"Unable to dither image",
      "Memory allocation failed");
  if (image->colormap != (PixelPacket *) NULL)
    FreeMemory(image->colormap);
  image->colormap=colormap;
  image->colormap[0].red=0;
  image->colormap[0].green=0;
  image->colormap[0].blue=0;
  image->colormap[1].red=MaxRGB;
  image->colormap[1].green=MaxRGB;
  image->colormap[1].blue=MaxRGB;
  /*
    Dither image with the ordered dithering technique.
  */
  for (y=0; y < (int) image->rows; y++)
  {
    q=GetPixelCache(image,0,y,image->columns,1);
    if (q == (PixelPacket *) NULL)
      break;
    indexes=GetIndexes(image->cache);
    for (x=0; x < (int) image->columns; x++)
    {
      index=Intensity(*q) > DitherMatrix[y & 0x07][x & 0x07] ? 1 : 0;
      indexes[x]=index;
      q->red=image->colormap[index].red;
      q->green=image->colormap[index].green;
      q->blue=image->colormap[index].blue;
      q++;
    }
    if (!SyncPixelCache(image))
      break;
    if (QuantumTick(y,image->rows))
      ProgressMonitor(DitherImageText,y,image->rows);
  }
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   P r u n e C h i l d                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method PruneChild deletes the given node and merges its statistics into
%  its parent.
%
%  The format of the PruneSubtree method is:
%
%      PruneChild(CubeInfo *cube_info,const NodeInfo *node_info)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o node_info: pointer to node in color cube tree that is to be pruned.
%
%
*/
static void PruneChild(CubeInfo *cube_info,const NodeInfo *node_info)
{
  NodeInfo
    *parent;

  register int
    id;

  /*
    Traverse any children.
  */
  if (node_info->census != 0)
    for (id=0; id < MaxTreeDepth; id++)
      if (node_info->census & (1 << id))
        PruneChild(cube_info,node_info->child[id]);
  /*
    Merge color statistics into parent.
  */
  parent=node_info->parent;
  parent->census&=~(1 << node_info->id);
  parent->number_unique+=node_info->number_unique;
  parent->total_red+=node_info->total_red;
  parent->total_green+=node_info->total_green;
  parent->total_blue+=node_info->total_blue;
  cube_info->nodes--;
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+  P r u n e L e v e l                                                        %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method PruneLevel deletes all nodes at the bottom level of the color
%  tree merging their color statistics into their parent node.
%
%  The format of the PruneLevel method is:
%
%      PruneLevel(CubeInfo *cube_info,const NodeInfo *node_info)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o node_info: pointer to node in color cube tree that is to be pruned.
%
%
*/
static void PruneLevel(CubeInfo *cube_info,const NodeInfo *node_info)
{
  register int
    id;

  /*
    Traverse any children.
  */
  if (node_info->census != 0)
    for (id=0; id < MaxTreeDepth; id++)
      if (node_info->census & (1 << id))
        PruneLevel(cube_info,node_info->child[id]);
  if (node_info->level == cube_info->depth)
    PruneChild(cube_info,node_info);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  Q u a n t i z a t i o n E r r o r                                          %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method QuantizationError measures the difference between the original
%  and quantized images.  This difference is the total quantization error.
%  The error is computed by summing over all pixels in an image the distance
%  squared in RGB space between each reference pixel value and its quantized
%  value.  These values are computed:
%
%    o mean_error_per_pixel:  This value is the mean error for any single
%      pixel in the image.
%
%    o normalized_mean_square_error:  This value is the normalized mean
%      quantization error for any single pixel in the image.  This distance
%      measure is normalized to a range between 0 and 1.  It is independent
%      of the range of red, green, and blue values in the image.
%
%    o normalized_maximum_square_error:  Thsi value is the normalized
%      maximum quantization error for any single pixel in the image.  This
%      distance measure is normalized to a range between 0 and 1.  It is
%      independent of the range of red, green, and blue values in your image.
%
%
%  The format of the QuantizationError method is:
%
%      unsigned int QuantizationError(Image *image)
%
%  A description of each parameter follows.
%
%    o image: The address of a byte (8 bits) array of run-length
%      encoded pixel data of your reference image.  The sum of the
%      run-length counts in the reference image must be equal to or exceed
%      the number of pixels.
%
%
*/
Export unsigned int QuantizationError(Image *image)
{
  CubeInfo
    cube_info;

  double
    distance_squared,
    maximum_error_per_pixel,
    *squares,
    total_error;

  IndexPacket
    index;

  int
    y;

  register IndexPacket
    *indexes;

  register int
    i,
    x;

  register PixelPacket
    *p;

  /*
    Initialize measurement.
  */
  assert(image != (Image *) NULL);
  image->total_colors=GetNumberColors(image,(FILE *) NULL);
  image->mean_error_per_pixel=0;
  image->normalized_mean_error=0.0;
  image->normalized_maximum_error=0.0;
  if (image->class == DirectClass)
    return(True);
  cube_info.squares=(double *) AllocateMemory((MaxRGB+MaxRGB+1)*sizeof(double));
  if (cube_info.squares == (double *) NULL)
    ThrowBinaryException(ResourceLimitWarning,"Unable to measure error",
      "Memory allocation failed");
  cube_info.squares+=MaxRGB;
  for (i=(-MaxRGB); i <= MaxRGB; i++)
    cube_info.squares[i]=i*i;
  /*
    For each pixel, collect error statistics.
  */
  squares=cube_info.squares;
  maximum_error_per_pixel=0;
  total_error=0;
  for (y=0; y < (int) image->rows; y++)
  {
    p=GetPixelCache(image,0,y,image->columns,1);
    if (p == (PixelPacket *) NULL)
      break;
    indexes=GetIndexes(image->cache);
    for (x=0; x < (int) image->columns; x++)
    {
      index=indexes[x];
      distance_squared=squares[(int) (p->red-image->colormap[index].red)]+
        squares[(int) (p->green-image->colormap[index].green)]+
        squares[(int) (p->blue-image->colormap[index].blue)];
      total_error+=distance_squared;
      if (distance_squared > maximum_error_per_pixel)
        maximum_error_per_pixel=distance_squared;
      p++;
    }
  }
  /*
    Compute final error statistics.
  */
  image->mean_error_per_pixel=total_error/(image->columns*image->rows);
  image->normalized_mean_error=image->mean_error_per_pixel/
    (3.0*(MaxRGB+1)*(MaxRGB+1));
  image->normalized_maximum_error=maximum_error_per_pixel/
    (3.0*(MaxRGB+1)*(MaxRGB+1));
  cube_info.squares-=MaxRGB;
  FreeMemory(cube_info.squares);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  Q u a n t i z e I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method QuantizeImage analyzes the colors within a reference image and
%  chooses a fixed number of colors to represent the image.  The goal of the
%  algorithm is to minimize the difference between the input and output image
%  while minimizing the processing time.
%
%  The format of the QuantizeImage method is:
%
%      unsigned int QuantizeImage(const QuantizeInfo *quantize_info,
%        Image *image)
%
%  A description of each parameter follows:
%
%    o quantize_info: Specifies a pointer to an QuantizeInfo structure.
%
%    o image: Specifies a pointer to a Image structure.
%
*/
Export unsigned int QuantizeImage(const QuantizeInfo *quantize_info,
  Image *image)
{
  CubeInfo
    cube_info;

  int
    depth;

  unsigned int
    number_colors,
    status;

  assert(image != (Image *) NULL);
  if ((quantize_info->number_colors == 2) && quantize_info->dither &&
      (quantize_info->colorspace == GRAYColorspace))
    {
      status=OrderedDitherImage(image);
      return(status);
    }
  number_colors=quantize_info->number_colors;
  if (number_colors == 0)
    number_colors=MaxRGB+1;
  if ((int) number_colors > MaxColormapSize)
    number_colors=MaxColormapSize;
  depth=quantize_info->tree_depth;
  if (depth == 0)
    {
      unsigned int
        colors;

      /*
        Depth of color classification tree is: Log4(colormap size)+2.
      */
      colors=number_colors;
      for (depth=1; colors != 0; depth++)
        colors>>=2;
      if (quantize_info->dither)
        depth--;
      if (image->class == PseudoClass)
        depth+=2;
    }
  /*
    Initialize color cube.
  */
  status=GetCubeInfo(&cube_info,quantize_info,depth);
  if (status == False)
    ThrowBinaryException(ResourceLimitWarning,"Unable to quantize image",
      "Memory allocation failed");
  if (quantize_info->colorspace != RGBColorspace)
    RGBTransformImage(image,quantize_info->colorspace);
  status=Classification(&cube_info,image);
  if (status != False)
    {
      /*
        Reduce the number of colors in the continuous tone image.
      */
      if (number_colors < cube_info.colors)
        Reduction(&cube_info,number_colors);
      status=Assignment(&cube_info,image);
      if (quantize_info->colorspace != RGBColorspace)
        TransformRGBImage(image,quantize_info->colorspace);
    }
  DestroyCubeInfo(&cube_info);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   Q u a n t i z e I m a g e s                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  QuantizeImages analyzes the colors within a set of reference images and
%  chooses a fixed number of colors to represent the set.  The goal of the
%  algorithm is to minimize the difference between the input and output images
%  while minimizing the processing time.
%
%  The format of the QuantizeImages method is:
%
%      unsigned int QuantizeImages(const QuantizeInfo *quantize_info,
%        Image *images)
%
%  A description of each parameter follows:
%
%    o quantize_info: Specifies a pointer to an QuantizeInfo structure.
%
%    o images: Specifies a pointer to a list of Image structures.
%
%
*/
Export unsigned int QuantizeImages(const QuantizeInfo *quantize_info,
  Image *images)
{
  CubeInfo
    cube_info;

  int
    depth;

  MonitorHandler
    handler;

  Image
    *image;

  register int
    i;

  unsigned int
    number_colors,
    number_images,
    status;

  assert(images != (Image *) NULL);
  if (images->next == (Image *) NULL)
    {
      /*
        Handle a single image with QuantizeImage.
      */
      status=QuantizeImage(quantize_info,images);
      return(status);
    }
  number_colors=quantize_info->number_colors;
  if (number_colors == 0)
    number_colors=MaxRGB+1;
  if ((int) number_colors > MaxColormapSize)
    number_colors=MaxColormapSize;
  depth=quantize_info->tree_depth;
  if (depth == 0)
    {
      int
        pseudo_class;

      unsigned int
        colors;

      /*
        Depth of color classification tree is: Log4(colormap size)+2.
      */
      colors=number_colors;
      for (depth=1; colors != 0; depth++)
        colors>>=2;
      if (quantize_info->dither)
        depth--;
      pseudo_class=True;
      for (image=images; image != (Image *) NULL; image=image->next)
        pseudo_class|=(image->class == PseudoClass);
      if (pseudo_class)
        depth+=2;
    }
  /*
    Initialize color cube.
  */
  status=GetCubeInfo(&cube_info,quantize_info,depth);
  if (status == False)
    ThrowBinaryException(ResourceLimitWarning,
      "Unable to quantize image sequence","Memory allocation failed");
  image=images;
  for (i=0; image != (Image *) NULL; i++)
  {
    if (quantize_info->colorspace != RGBColorspace)
      RGBTransformImage(image,quantize_info->colorspace);
    image=image->next;
  }
  number_images=i;
  image=images;
  for (i=0; image != (Image *) NULL; i++)
  {
    handler=SetMonitorHandler((MonitorHandler) NULL);
    status=Classification(&cube_info,image);
    if (status == False)
      break;
    image=image->next;
    (void) SetMonitorHandler(handler);
    ProgressMonitor(ClassifyImageText,i,number_images);
  }
  if (status != False)
    {
      /*
        Reduce the number of colors in the continuous tone image sequence.
      */
      Reduction(&cube_info,number_colors);
      image=images;
      for (i=0; image != (Image *) NULL; i++)
      {
        handler=SetMonitorHandler((MonitorHandler) NULL);
        status=Assignment(&cube_info,image);
        if (status == False)
          break;
        if (quantize_info->colorspace != RGBColorspace)
          TransformRGBImage(image,quantize_info->colorspace);
        image=image->next;
        (void) SetMonitorHandler(handler);
        ProgressMonitor(AssignImageText,i,number_images);
      }
    }
  DestroyCubeInfo(&cube_info);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   R e d u c e                                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method Reduce traverses the color cube tree and prunes any node whose
%  quantization error falls below a particular threshold.
%
%  The format of the Reduce method is:
%
%      Reduce(CubeInfo *cube_info,const NodeInfo *node_info)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o node_info: pointer to node in color cube tree that is to be pruned.
%
%
*/
static void Reduce(CubeInfo *cube_info,const NodeInfo *node_info)
{
  register unsigned int
    id;

  /*
    Traverse any children.
  */
  if (node_info->census != 0)
    for (id=0; id < MaxTreeDepth; id++)
      if (node_info->census & (1 << id))
        Reduce(cube_info,node_info->child[id]);
  if (node_info->quantization_error <= cube_info->pruning_threshold)
    PruneChild(cube_info,node_info);
  else
    {
      /*
        Find minimum pruning threshold.
      */
      if (node_info->number_unique > 0)
        cube_info->colors++;
      if (node_info->quantization_error < cube_info->next_pruning_threshold)
        cube_info->next_pruning_threshold=node_info->quantization_error;
    }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   R e d u c t i o n                                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method Reduction repeatedly prunes the tree until the number of nodes
%  with n2 > 0 is less than or equal to the maximum number of colors allowed
%  in the output image.  On any given iteration over the tree, it selects
%  those nodes whose E value is minimal for pruning and merges their
%  color statistics upward. It uses a pruning threshold, Ep, to govern
%  node selection as follows:
%
%    Ep = 0
%    while number of nodes with (n2 > 0) > required maximum number of colors
%      prune all nodes such that E <= Ep
%      Set Ep to minimum E in remaining nodes
%
%  This has the effect of minimizing any quantization error when merging
%  two nodes together.
%
%  When a node to be pruned has offspring, the pruning procedure invokes
%  itself recursively in order to prune the tree from the leaves upward.
%  n2,  Sr, Sg,  and  Sb in a node being pruned are always added to the
%  corresponding data in that node's parent.  This retains the pruned
%  node's color characteristics for later averaging.
%
%  For each node, n2 pixels exist for which that node represents the
%  smallest volume in RGB space containing those pixel's colors.  When n2
%  > 0 the node will uniquely define a color in the output image. At the
%  beginning of reduction,  n2 = 0  for all nodes except a the leaves of
%  the tree which represent colors present in the input image.
%
%  The other pixel count, n1, indicates the total number of colors
%  within the cubic volume which the node represents.  This includes n1 -
%  n2  pixels whose colors should be defined by nodes at a lower level in
%  the tree.
%
%  The format of the Reduction method is:
%
%      Reduction(CubeInfo *cube_info,const unsigned int number_colors)
%
%  A description of each parameter follows.
%
%    o cube_info: A pointer to the Cube structure.
%
%    o number_colors: This integer value indicates the maximum number of
%      colors in the quantized image or colormap.  The actual number of
%      colors allocated to the colormap may be less than this value, but
%      never more.
%
%
*/
static void Reduction(CubeInfo *cube_info,const unsigned int number_colors)
{
#define ReduceImageText  "  Reducing image colors...  "

  unsigned int
    span;

  span=(unsigned int) cube_info->colors;
  cube_info->next_pruning_threshold=0.0;
  while (cube_info->colors > number_colors)
  {
    cube_info->pruning_threshold=cube_info->next_pruning_threshold;
    cube_info->next_pruning_threshold=cube_info->root->quantization_error;
    cube_info->colors=0;
    Reduce(cube_info,cube_info->root);
    ProgressMonitor(ReduceImageText,span-cube_info->colors,
      span-number_colors+1);
  }
}
