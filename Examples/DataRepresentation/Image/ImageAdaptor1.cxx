/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageAdaptor1.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif


// Software Guide : BeginLatex
//
// This example illustrates the use of the classes \doxygen{ImageAdaptor} and
// \doxygen{PixelAccessor}.  Here we show how ImageAdaptor can be used to
// cast the pixel type of an image.  In particular, an image of pixel type
// \code{unsigned char} is \emph{adapted} to make it look as and image of
// pixel type \code{float}.
// 
// \index{itk::ImageAdaptor!Instantiation}
// \index{itk::ImageAdaptor!Header}
//
// The first step required to use image adaptors is to include the relevant
// headers.  
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkImage.h"
#include "itkImageAdaptor.h"
// Software Guide : EndCodeSnippet


#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageFileReader.h"


//  Software Guide : BeginLatex
//
//  Then, a class should be defined for specifying the pixel conversion.
//  This class is called in general a \doxygen{PixelAccessor}. Note that
//  valid operations are those that only require the value of the input
//  pixel. Neighborhood operations are not possible with the use of
//  PixelAccessors. The PixelAccessor class must provide methods \code{Set()}
//  and \code{Get()}, and define the types \code{InternalType} and
//  \code{ExternalType}. The \code{InternalType} corresponds to the pixel
//  type of the image to be adapted (in the current example this is
//  \code{unsigned char}). The \code{ExternalType} corresponds to the pixel
//  type we desire to emulate with the ImageAdaptor (in this case this is
//  \code{float}).
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
class CastPixelAccessor  
{
public:
  typedef unsigned char InternalType;
  typedef float         ExternalType;

  static void Set(InternalType & output, const ExternalType & input) 
    {
      output = static_cast<InternalType>( input );
    }

  static ExternalType Get( const InternalType & input ) 
    {
      return static_cast<ExternalType>( input );
    }
};
// Software Guide : EndCodeSnippet


//-------------------------
//
//   Main code
//
//-------------------------

int main( int argc, char *argv[] ) 
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << "ImageAdaptor1   inputFileName" << std::endl;
    return -1;
    }


//  Software Guide : BeginLatex
//
//  The CastPixelAccessor class defined above simply applies a
//  \code{static\_cast} to the pixel values. We use now this pixel accessor
//  for instantiating the image adaptor in the lines below.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef unsigned char  InputPixelType;
  const   unsigned int   Dimension = 2;
  typedef itk::Image< InputPixelType, Dimension >   ImageType;

  typedef itk::ImageAdaptor< ImageType, CastPixelAccessor > ImageAdaptorType;
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  An object of this type is now constructed using the standart \code{New()}
//  method and assigning the result to a \code{SmartPointer}.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  We create a reader whose output will have the appropiate type for the
//  \emph{adapted} image type.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef itk::ImageFileReader< ImageType >   ReaderType;
  ReaderType::Pointer reader = ReaderType::New();  
// Software Guide : EndCodeSnippet


  reader->SetFileName( argv[1] );
  reader->Update();


//  Software Guide : BeginLatex
//
//  and now connect the output of the reader as input to the image adaptor.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  adaptor->SetImage( reader->GetOutput() );
// Software Guide : EndCodeSnippet
 

//  Software Guide : BeginLatex
//
//  Finally, we can visit the image using an iterator instantiated for the
//  output image type of the adaptor. For example, the following code
//  computes the sum of pixel values.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef itk::ImageRegionIteratorWithIndex< ImageAdaptorType >  IteratorType;
  IteratorType  it( adaptor, adaptor->GetBufferedRegion() );

  double sum = 0.0;
  it.GoToBegin();
  while( !it.IsAtEnd() )
    {
    float value = it.Get();
    sum += value;
    ++it;
    }
// Software Guide : EndCodeSnippet


std::cout << "Sum of pixels is: " << sum << std::endl;


//  Software Guide : BeginLatex
//
// In this case, the iterator is simply visiting all the pixels, reading their
// values and accumulating them in the sum. The key concept is that
// access to pixels is performed as if the pixel type is \code{float}. 
//
// Note that the \code{adaptor} is used \emph{as if} it was an image, not as a
// filter. ImageAdaptors provide the same API of the \doxygen{Image} class.
//
//  Software Guide : EndLatex 


  return 0;
}



