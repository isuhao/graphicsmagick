// This may look like C code, but it is really -*- C++ -*-
//
// Copyright Bob Friesenhahn, 1999, 2000, 2001, 2002
//
// Simple demo program for Magick++
//
// Concept and algorithms lifted from PerlMagick demo script written
// by John Christy.
//
// Max run-time size 60MB (as compared with 95MB for PerlMagick) under SPARC Solaris
//

#include <string>
#include <iostream>
#include <list>

#include <Magick++.h>

using namespace std;

using namespace Magick;

int main( int /*argc*/, char ** argv)
{

  // Initialize ImageMagick install location for Windows
  InitializeMagick(*argv);
  
  try {
    
    string srcdir("");
    if(getenv("SRCDIR") != 0)
      srcdir = getenv("SRCDIR");
    
    // Common font to use.
    string font = "Helvetica";

    list<Image> montage;

    {
      //
      // Read model & smile image.
      //
      cout << "Read images ..." << endl;

      Image model( srcdir + "model.miff" );
      model.label( "Magick++" );
      model.borderColor( "black" );
      model.backgroundColor( "black" );
    
      Image smile( srcdir + "smile.miff" );
      smile.label( "Smile" );
      smile.borderColor( "black" );
    
      //
      // Create image stack.
      //
      cout << "Creating thumbnails..." << endl;
    
      // Construct initial list containing five copies of a null image
      Image null;
      null.size( Geometry(70,70) );
      null.read( "NULL:black" );
      list<Image> images( 5, null );
    
      Image example = model;
    
      // Each of the following follow the pattern
      //  1. obtain reference to (own copy of) image
      //  2. apply label to image
      //  3. apply operation to image
      //  4. append image to container

      cout << "  add noise ..." << endl;
      example.label( "Add Noise" );
      example.addNoise( LaplacianNoise );
      images.push_back( example );

      cout << "  annotate ..." << endl;
      example = model;
      example.label( "Annotate" );
      example.density( "72x72" );
      example.fontPointsize( 18 );
      example.font( font );
      example.strokeColor( Color() );
      example.fillColor( "gold" );
      example.annotate( "Magick++", "+0+20", NorthGravity );
      images.push_back( example );

      cout << "  blur ..." << endl;
      example = model;
      example.label( "Blur" );
      example.blur( 0, 1.5 );
      images.push_back( example );

      cout << "  border ..." << endl;
      example = model;
      example.label( "Border" );
      example.borderColor( "gold" );
      example.border( Geometry(6,6) );
      images.push_back( example );

      cout << "  channel ..." << endl;
      example = model;
      example.label( "Channel" );
      example.channel( RedChannel );
      images.push_back( example );

      cout << "  charcoal ..." << endl;
      example = model;
      example.label( "Charcoal" );
      example.charcoal( );
      images.push_back( example );

      cout << "  composite ..." << endl;
      example = model;
      example.label( "Composite" );
      example.composite( smile, "+35+65", OverCompositeOp);
      images.push_back( example );

      cout << "  contrast ..." << endl;
      example = model;
      example.label( "Contrast" );
      example.contrast( false );
      images.push_back( example );

      cout << "  convolve ..." << endl;
      example = model;
      example.label( "Convolve" );
      {
        // 3x3 matrix
        const double kernel[] = { 1, 1, 1, 1, 4, 1, 1, 1, 1 };
        example.convolve( 3, kernel );
      }
      images.push_back( example );

      cout << "  crop ..." << endl;
      example = model;
      example.label( "Crop" );
      example.crop( "80x80+25+50" );
      images.push_back( example );

      cout << "  despeckle ..." << endl;
      example = model;
      example.label( "Despeckle" );
      example.despeckle( );
      images.push_back( example );

      cout << "  draw ..." << endl;
      example = model;
      example.label( "Draw" );
      example.fillColor(Color());
      example.strokeColor( "gold" );
      example.strokeWidth( 2 );
      example.draw( DrawableCircle( 60,90, 60,120 ) );
      images.push_back( example );

      cout << "  edge ..." << endl;
      example = model;
      example.label( "Detect Edges" );
      example.edge( );
      images.push_back( example );

      cout << "  emboss ..." << endl;
      example = model;
      example.label( "Emboss" );
      example.emboss( );
      images.push_back( example );

      cout << "  equalize ..." << endl;
      example = model;
      example.label( "Equalize" );
      example.equalize( );
      images.push_back( example );
    
      cout << "  explode ..." << endl;
      example = model;
      example.label( "Explode" );
      example.backgroundColor( "#000000FF" );
      example.implode( -1 );
      images.push_back( example );

      cout << "  flip ..." << endl;
      example = model;
      example.label( "Flip" );
      example.flip( );
      images.push_back( example );

      cout << "  flop ..." << endl;
      example = model;
      example.label( "Flop" );
      example.flop();
      images.push_back( example );

      cout << "  frame ..." << endl;
      example = model;
      example.label( "Frame" );
      example.frame( );
      images.push_back( example );

      cout << "  gamma ..." << endl;
      example = model;
      example.label( "Gamma" );
      example.gamma( 1.6 );
      images.push_back( example );

      cout << "  gaussian blur ..." << endl;
      example = model;
      example.label( "Gaussian Blur" );
      example.gaussianBlur( 0.0, 1.5 );
      images.push_back( example );
    
      cout << "  gradient ..." << endl;
      Image gradient;
      gradient.size( "130x194" );
      gradient.read( "gradient:#20a0ff-#ffff00" );
      gradient.label( "Gradient" );
      images.push_back( gradient );
    
      cout << "  grayscale ..." << endl;
      example = model;
      example.label( "Grayscale" );
      example.quantizeColorSpace( GRAYColorspace );
      example.quantize( );
      images.push_back( example );
    
      cout << "  implode ..." << endl;
      example = model;
      example.label( "Implode" );
      example.implode( 0.5 );
      images.push_back( example );

      cout << "  median filter ..." << endl;
      example = model;
      example.label( "Median Filter" );
      example.medianFilter( );
      images.push_back( example );

      cout << "  modulate ..." << endl;
      example = model;
      example.label( "Modulate" );
      example.modulate( 110, 110, 110 );
      images.push_back( example );

      cout << "  monochrome ..." << endl;
      example = model;
      example.label( "Monochrome" );
      example.quantizeColorSpace( GRAYColorspace );
      example.quantizeColors( 2 );
      example.quantizeDither( false );
      example.quantize( );
      images.push_back( example );
    
      cout << "  negate ..." << endl;
      example = model;
      example.label( "Negate" );
      example.negate( );
      images.push_back( example );
    
      cout << "  normalize ..." << endl;
      example = model;
      example.label( "Normalize" );
      example.normalize( );
      images.push_back( example );
    
      cout << "  oil paint ..." << endl;
      example = model;
      example.label( "Oil Paint" );
      example.oilPaint( );
      images.push_back( example );
    
      cout << "  plasma ..." << endl;
      Image plasma;
      plasma.size( "130x194" );
      plasma.read( "plasma:fractal" );
      plasma.label( "Plasma" );
      images.push_back( plasma );
    
      cout << "  quantize ..." << endl;
      example = model;
      example.label( "Quantize" );
      example.quantize( );
      images.push_back( example );
    
      cout << "  raise ..." << endl;
      example = model;
      example.label( "Raise" );
      example.raise( );
      images.push_back( example );
    
      cout << "  reduce noise ..." << endl;
      example = model;
      example.label( "Reduce Noise" );
      example.reduceNoise( );
      images.push_back( example );
    
      cout << "  resize ..." << endl;
      example = model;
      example.label( "Resize" );
      example.zoom( "50%" );
      images.push_back( example );
    
      cout << "  roll ..." << endl;
      example = model;
      example.label( "Roll" );
      example.roll( "+20+10" );
      images.push_back( example );
    
      cout << "  rotate ..." << endl;
      example = model;
      example.label( "Rotate" );
      example.rotate( 45 );
      example.transparent( "black" );
      images.push_back( example );

      cout << "  scale ..." << endl;
      example = model;
      example.label( "Scale" );
      example.scale( "60%" );
      images.push_back( example );
    
      cout << "  segment ..." << endl;
      example = model;
      example.label( "Segment" );
      example.segment( );
      images.push_back( example );
    
      cout << "  shade ..." << endl;
      example = model;
      example.label( "Shade" );
      example.shade( 30, 30, false );
      images.push_back( example );
    
      cout << "  sharpen ..." << endl;
      example = model;
      example.label("Sharpen");
      example.sharpen( 0.0, 1.0 );
      images.push_back( example );
    
      cout << "  shave ..." << endl;
      example = model;
      example.label("Shave");
      example.shave( Geometry( 10, 10) );
      images.push_back( example );
    
      cout << "  shear ..." << endl;
      example = model;
      example.label( "Shear" );
      example.shear( 45, 45 );
      example.transparent( "black" );
      images.push_back( example );
    
      cout << "  spread ..." << endl;
      example = model;
      example.label( "Spread" );
      example.spread( );
      images.push_back( example );
    
      cout << "  solarize ..." << endl;
      example = model;
      example.label( "Solarize" );
      example.solarize( );
      images.push_back( example );
    
      cout << "  swirl ..." << endl;
      example = model;
      example.backgroundColor( "#000000FF" );
      example.label( "Swirl" );
      example.swirl( 90 );
      images.push_back( example );
    
      cout << "  unsharp mask ..." << endl;
      example = model;
      example.label( "Unsharp Mask" );
      //           radius_, sigma_, amount_, threshold_
      example.unsharpmask( 0.0, 1.0, 1.0, 0.05);
      images.push_back( example );
    
      cout << "  wave ..." << endl;
      example = model;
      example.label( "Wave" );
      example.matte( true );
      example.backgroundColor( "#000000FF" );
      example.wave( 25, 150 );
      images.push_back( example );
    
      //
      // Create image montage.
      //
      cout <<  "Montage images..." << endl;

      for_each( images.begin(), images.end(), fontImage( font ) );
      for_each( images.begin(), images.end(), strokeColorImage( Color("#600") ) );

      MontageFramed montageOpts;
      montageOpts.geometry( "130x194+10+5>" );
      montageOpts.gravity( CenterGravity );
      montageOpts.borderColor( "green" );
      montageOpts.borderWidth( 1 );
      montageOpts.tile( "5x1000" );
      montageOpts.compose( OverCompositeOp );
      montageOpts.backgroundColor( "#ffffff" );
      montageOpts.font( font );
      montageOpts.pointSize( 18 );
      montageOpts.fillColor( "#600" );
      montageOpts.strokeColor( Color() );
      montageOpts.compose(OverCompositeOp);
      montageOpts.fileName( "Magick++ Demo" );
      montageImages( &montage, images.begin(), images.end(), montageOpts );
    }

    Image& montage_image = montage.front();
    {
      // Create logo image
      cout << "Adding logo image ..." << endl;
      Image logo( "logo:" );
      logo.crop( "461x455+98+0" );
      logo.zoom( "45%" );

      // Composite logo into montage image
      Geometry placement(0,0,(montage_image.columns()/2)-(logo.columns()/2),0);
      montage_image.composite( logo, placement, OverCompositeOp );
    }

    cout << "Writing image \"demo_out.miff\" ..." << endl;
    montage_image.matte( false );
    montage_image.compressType( RunlengthEncodedCompression );
    montage_image.write( "demo_out.miff" );

    // Uncomment following lines to display image to screen
    //    cout <<  "Display image..." << endl;
    //    montage_image.display();

  }
  catch( exception &error_ )
    {
      cout << "Caught exception: " << error_.what() << endl;
      return 1;
    }

  return 0;
}
