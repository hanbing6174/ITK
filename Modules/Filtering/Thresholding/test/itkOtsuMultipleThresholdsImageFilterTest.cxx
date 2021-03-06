/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImageFileReader.h"
#include "itkOtsuMultipleThresholdsImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

int itkOtsuMultipleThresholdsImageFilterTest(int argc, char* argv[] )
{
  if( argc < 6 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputImageFile";
    std::cerr << " numberOfHistogramBins";
    std::cerr << " numberOfThresholds";
    std::cerr << " labelOffset";
    std::cerr << " [valleyEmphasis]";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  using InputPixelType = short;
  using InternalPixelType = unsigned short;
  using OutputPixelType = unsigned char;

  using InputImageType = itk::Image< InputPixelType,  2 >;
  using InternalImageType = itk::Image< InternalPixelType, 2>;
  using OutputImageType = itk::Image< OutputPixelType, 2 >;

  using ReaderType = itk::ImageFileReader< InputImageType >;
  using FilterType = itk::OtsuMultipleThresholdsImageFilter< InputImageType, InternalImageType >;
  using RescaleType = itk::RescaleIntensityImageFilter< InternalImageType, OutputImageType >;
  using WriterType = itk::ImageFileWriter< OutputImageType >;

  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  RescaleType::Pointer rescaler = RescaleType::New();
  WriterType::Pointer writer = WriterType::New();

  itk::SimpleFilterWatcher watcher(filter);

  // Set up the reader
  reader->SetFileName( argv[1] );

  // Set up the filter parameters.
  filter->SetInput( reader->GetOutput() );
  filter->SetNumberOfHistogramBins (atoi(argv[3]));
  filter->SetNumberOfThresholds( atoi(argv[4]) );
  filter->SetLabelOffset( atoi(argv[5]) );
  if( argc > 6 )
  {
    filter->SetValleyEmphasis(atoi(argv[6]));
  }
  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

  // Test GetMacros
  unsigned long numberOfHistogramBins = filter->GetNumberOfHistogramBins();
  std::cout << "filter->GetNumberOfHistogramBins(): "
            << numberOfHistogramBins
            << std::endl;
  unsigned long numberOfThresholds = filter->GetNumberOfThresholds();
  std::cout << "filter->GetNumberOfThresholds(): "
            << numberOfThresholds
            << std::endl;
  OutputPixelType labelOffset = filter->GetLabelOffset();
  std::cout << "filter->GetLabelOffset(): "
            << static_cast<itk::NumericTraits<OutputPixelType>::PrintType>( labelOffset )
            << std::endl;
  FilterType::ThresholdVectorType thresholds = filter->GetThresholds();
  std::cout << "filter->GetThresholds(): ";
  for (double threshold : thresholds)
    {
    std::cout << itk::NumericTraits<FilterType::InputPixelType>::PrintType(threshold) << " ";
    }
  std::cout << std::endl;

  // Rescale the image so that it can be seen.  The output of the
  // filter contains labels that are numbered sequentially, so the
  // image looks nearly uniform unless there are a large number of labels.
  rescaler->SetInput( filter->GetOutput() );
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  // Write out the test image
  writer->SetFileName( argv[2] );
  writer->SetInput( rescaler->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
