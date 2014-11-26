// Source file for image class



// Include files

#include "R2/R2.h"
#include "R2Pixel.h"
#include "R2Image.h"
#include "svd.h"



////////////////////////////////////////////////////////////////////////
// Constructors/Destructors
////////////////////////////////////////////////////////////////////////


R2Image::R2Image(void)
  : pixels(NULL),
    npixels(0),
    width(0),
    height(0)
{
}



R2Image::R2Image(const char *filename)
  : pixels(NULL),
    npixels(0),
    width(0),
    height(0)
{
  // Read image
  Read(filename);
}



R2Image::R2Image(int width, int height)
  : pixels(NULL),
    npixels(width * height),
    width(width),
    height(height)
{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);
}



R2Image::R2Image(int width, int height, const R2Pixel *p)
  : pixels(NULL),
    npixels(width * height),
    width(width),
    height(height)
{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = p[i];
}



R2Image::R2Image(const R2Image& image)
  : pixels(NULL),
    npixels(image.npixels),
    width(image.width),
    height(image.height)

{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = image.pixels[i];
}



R2Image::
~R2Image(void)
{
  // Free image pixels
  if (pixels) delete [] pixels;
}



R2Image& R2Image::
operator=(const R2Image& image)
{
  // Delete previous pixels
  if (pixels) { delete [] pixels; pixels = NULL; }

  // Reset width and height
  npixels = image.npixels;
  width = image.width;
  height = image.height;

  // Allocate new pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = image.pixels[i];

  // Return image
  return *this;
}


void R2Image::
svdTest(void)
{
	// fit a 2D conic to five points
	R2Point p1(1.2,3.5);
	R2Point p2(2.1,2.2);
	R2Point p3(0.2,1.6);
	R2Point p4(0.0,0.5);
	R2Point p5(-0.2,4.2);

	// build the 5x6 matrix of equations
	double** linEquations = dmatrix(1,5,1,6);

	linEquations[1][1] = p1[0]*p1[0];
	linEquations[1][2] = p1[0]*p1[1];
	linEquations[1][3] = p1[1]*p1[1];
	linEquations[1][4] = p1[0];
	linEquations[1][5] = p1[1];
	linEquations[1][6] = 1.0;

	linEquations[2][1] = p2[0]*p2[0];
	linEquations[2][2] = p2[0]*p2[1];
	linEquations[2][3] = p2[1]*p2[1];
	linEquations[2][4] = p2[0];
	linEquations[2][5] = p2[1];
	linEquations[2][6] = 1.0;

	linEquations[3][1] = p3[0]*p3[0];
	linEquations[3][2] = p3[0]*p3[1];
	linEquations[3][3] = p3[1]*p3[1];
	linEquations[3][4] = p3[0];
	linEquations[3][5] = p3[1];
	linEquations[3][6] = 1.0;

	linEquations[4][1] = p4[0]*p4[0];
	linEquations[4][2] = p4[0]*p4[1];
	linEquations[4][3] = p4[1]*p4[1];
	linEquations[4][4] = p4[0];
	linEquations[4][5] = p4[1];
	linEquations[4][6] = 1.0;

	linEquations[5][1] = p5[0]*p5[0];
	linEquations[5][2] = p5[0]*p5[1];
	linEquations[5][3] = p5[1]*p5[1];
	linEquations[5][4] = p5[0];
	linEquations[5][5] = p5[1];
	linEquations[5][6] = 1.0;

	printf("\n Fitting a conic to five points:\n");
	printf("Point #1: %f,%f\n",p1[0],p1[1]);
	printf("Point #2: %f,%f\n",p2[0],p2[1]);
	printf("Point #3: %f,%f\n",p3[0],p3[1]);
	printf("Point #4: %f,%f\n",p4[0],p4[1]);
	printf("Point #5: %f,%f\n",p5[0],p5[1]);

	// compute the SVD
	double singularValues[7]; // 1..6
	double** nullspaceMatrix = dmatrix(1,6,1,6);
	svdcmp(linEquations, 5, 6, singularValues, nullspaceMatrix);

	// get the result
	printf("\n Singular values: %f, %f, %f, %f, %f, %f\n",singularValues[1],singularValues[2],singularValues[3],singularValues[4],singularValues[5],singularValues[6]);

	// find the smallest singular value:
	int smallestIndex = 1;
	for(int i=2;i<7;i++) if(singularValues[i]<singularValues[smallestIndex]) smallestIndex=i;

	// solution is the nullspace of the matrix, which is the column in V corresponding to the smallest singular value (which should be 0)
	printf("Conic coefficients: %f, %f, %f, %f, %f, %f\n",nullspaceMatrix[1][smallestIndex],nullspaceMatrix[2][smallestIndex],nullspaceMatrix[3][smallestIndex],nullspaceMatrix[4][smallestIndex],nullspaceMatrix[5][smallestIndex],nullspaceMatrix[6][smallestIndex]);

	// make sure the solution is correct:
	printf("Equation #1 result: %f\n",	p1[0]*p1[0]*nullspaceMatrix[1][smallestIndex] +
										p1[0]*p1[1]*nullspaceMatrix[2][smallestIndex] +
										p1[1]*p1[1]*nullspaceMatrix[3][smallestIndex] +
										p1[0]*nullspaceMatrix[4][smallestIndex] +
										p1[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	printf("Equation #2 result: %f\n",	p2[0]*p2[0]*nullspaceMatrix[1][smallestIndex] +
										p2[0]*p2[1]*nullspaceMatrix[2][smallestIndex] +
										p2[1]*p2[1]*nullspaceMatrix[3][smallestIndex] +
										p2[0]*nullspaceMatrix[4][smallestIndex] +
										p2[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	printf("Equation #3 result: %f\n",	p3[0]*p3[0]*nullspaceMatrix[1][smallestIndex] +
										p3[0]*p3[1]*nullspaceMatrix[2][smallestIndex] +
										p3[1]*p3[1]*nullspaceMatrix[3][smallestIndex] +
										p3[0]*nullspaceMatrix[4][smallestIndex] +
										p3[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	printf("Equation #4 result: %f\n",	p4[0]*p4[0]*nullspaceMatrix[1][smallestIndex] +
										p4[0]*p4[1]*nullspaceMatrix[2][smallestIndex] +
										p4[1]*p4[1]*nullspaceMatrix[3][smallestIndex] +
										p4[0]*nullspaceMatrix[4][smallestIndex] +
										p4[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	printf("Equation #5 result: %f\n",	p5[0]*p5[0]*nullspaceMatrix[1][smallestIndex] +
										p5[0]*p5[1]*nullspaceMatrix[2][smallestIndex] +
										p5[1]*p5[1]*nullspaceMatrix[3][smallestIndex] +
										p5[0]*nullspaceMatrix[4][smallestIndex] +
										p5[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	R2Point test_point(0.34,-2.8);

	printf("A point off the conic: %f\n",	test_point[0]*test_point[0]*nullspaceMatrix[1][smallestIndex] +
											test_point[0]*test_point[1]*nullspaceMatrix[2][smallestIndex] +
											test_point[1]*test_point[1]*nullspaceMatrix[3][smallestIndex] +
											test_point[0]*nullspaceMatrix[4][smallestIndex] +
											test_point[1]*nullspaceMatrix[5][smallestIndex] +
											nullspaceMatrix[6][smallestIndex]);

	return;
}


////////////////////////////////////////////////////////////////////////
// Filter Helper Functions
////////////////////////////////////////////////////////////////////////

// currently using c capabilities where array size must be known...
// need something different for other kernel sizes
void R2Image::applyKernelToTemp(int u, int v, double (&kernel)[3][3], R2Image &temp)
{

  for (int i = 1; i < temp.Width()-1; i++) {
    for (int j = 1;  j < temp.Height()-1; j++) {
      R2Pixel sumTotal;
      double totalWeight = 0.0;
      for(int k = 0; k < 3; ++k) {
        for(int l = 0; l < 3; ++l) {
          sumTotal += kernel[k][l]*Pixel(i+k-1, j+l-1);
          totalWeight += kernel[k][l];
        }
      }

      temp.Pixel(i,j) = sumTotal/totalWeight;
      // temp.Pixel(i,j).Clamp();
    }
  }
}

// replaces the pixels in this image with those in the temporary one
void R2Image::replaceWithTemp(R2Image &temp)
{
  for (int i = 1; i < width-1; i++) {
    for (int j = 1;  j < height-1; j++) {
      Pixel(i,j) = temp.Pixel(i,j);
      // Pixel(i,j).Clamp();
    }
  }
}

// converts a pixel to grayscale
void convertToGrayscale(R2Pixel &pixel)
{
  double avg = (pixel.Red() + pixel.Blue() + pixel.Green())/3.0;
  pixel = R2Pixel(avg, avg, avg, 1.0);
}

void R2Image::colorAroundPoint(int x, int y, int siz)
{
  // printf("%f\n",Rharris.Pixel(maxLocalX,maxLocalY).Red());
  R2Pixel indicatorPix = R2Pixel(1.0, 1.0, 1.0, 1.0);
  R2Pixel outerIndicatorPix = R2Pixel(0.0, 0.0, 0.0, 1.0);

  int iMin = x - siz;
  iMin = iMin < 0 ? 0 : iMin;
  int iMax = x + siz;
  iMax = iMax > width ? width : iMax;

  int jMin = y - siz;
  jMin = jMin < 0 ? 0 : jMin;
  int jMax = y + siz;
  jMax = jMax > height ? height : jMax;

  for (int i = iMin; i <= iMax; i++) {
    for (int j = jMin; j <= jMax; j++) {
      bool onEdge = i == iMin || j == jMin || i == iMax || j == jMax;
      Pixel(i,j) = onEdge ? outerIndicatorPix : indicatorPix;
    }
  }
}

void R2Image::line(int x0, int x1, int y0, int y1, float r, float g, float b)
{
  if(x0>3 && x0<width-3 && y0>3 && y0<height-3)
  {
    for(int x=x0-3;x<=x0+3;x++)
    {
      for(int y=y0-3;y<=y0+3;y++)
      {
        Pixel(x,y).Reset(r,g,b,1.0);
      }
    }
  }

  bool swap = false;
  if (fabs(y1 - y0) > fabs(x1 - x0))
  {
    int x = x0;
    x0 = y0;
    y0 = x;

    x = x1;
    x1 = y1;
    y1 = x;

    swap = true;
  }

  if(x0>x1)
  {
    int x=y1;
    y1=y0;
    y0=x;

    x=x1;
    x1=x0;
    x0=x;
  }

  int deltax = x1 - x0;
  int deltay = y1 - y0;
  float error = 0;
  float deltaerr = 0.0;
  if(deltax!=0) deltaerr =fabs(float(float((float) deltay) / deltax));    // Assume deltax != 0 (line is not vertical),
    // note that this division needs to be done in a way that preserves the fractional part
    int y = y0;

  for(int x=x0;x<=x1;x++)
  {
    if (swap) {
      Pixel(y,x).Reset(r,g,b,1.0);
    } else {
      Pixel(x,y).Reset(r,g,b,1.0);
    }
    error = error + deltaerr;
    if(error>=0.5)
    {
      if(deltay>0) y = y + 1;
      else y = y - 1;

      error = error - 1.0;
    }
  }
}

void R2Image::drawVectorFromPoint(ValPoint pt, ValPoint vec)
{
  R2Pixel shade = R2Pixel(0.0, 1.0, 0.0, 1.0);
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  int deltax = vec.x;
  int deltay = vec.y;
  double error = 0;
  double deltaerr = abs(deltay / deltax);    // Assume deltax != 0 (line is not vertical),
  // note that this division needs to be done in a way that preserves the fractional part
  int y = pt.y;
  for (int x = pt.x; x < pt.x + vec.x; x++) {
    Pixel(x,y) = shade;
    error = error + deltaerr;
    if (error >= 0.5) {
      y = y + 1;
      error = error - 1.0;
    }
  }

  // double dx = vec.x < 0 ? -1.0 : 1.0;
  // double dy = (double)(vec.y) / (double)(vec.x * dx);
  //
  // double exactX = (double) pt.x;
  // double exactY = (double) pt.y;
  //
  // int x = (int) exactX;
  // int y = (int) exactY;
  // do {
  //   Pixel(x+1,y) = shade;
  //   Pixel(x,y+1) = shade;
  //
  //   Pixel(x-1,y) = shade;
  //   Pixel(x,y-1) = shade;
  //
  //   exactX += dx;
  //   exactY += dy;
  //
  //   x = (int) exactX;
  //   y = (int) exactY;
  // } while( x > 0 && y > 0 && (x - pt.x > vec.x || y - pt.y > vec.y));
}

////////////////////////////////////////////////////////////////////////
// Image processing functions
// YOU IMPLEMENT THE FUNCTIONS IN THIS SECTION
////////////////////////////////////////////////////////////////////////

// Per-pixel Operations ////////////////////////////////////////////////

void R2Image::Brighten(double factor)
{
  // Brighten the image by multiplying each pixel component by the factor.
  // This is implemented for you as an example of how to access and set pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      Pixel(i,j) *= factor;
      Pixel(i,j).Clamp();
    }
  }
}

void R2Image::SobelX(void)
{
  // Apply the Sobel oprator to the image in X direction

  double sobelX[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}};

  R2Image temp(width, height);
  R2Pixel R2greyscale_pixel(0.5, 0.5, 0.5, 1.0);

  for (int i = 1; i < width-1; i++) {
    for (int j = 1;  j < height-1; j++) {
      R2Pixel sumTotal;
      for(int k = 0; k < 3; ++k) {
        for(int l = 0; l < 3; ++l) {
          sumTotal += sobelX[k][l]*Pixel(i+k-1, j+l-1);
        }
      }

      temp.Pixel(i,j) = sumTotal;// + R2greyscale_pixel;
      convertToGrayscale(temp.Pixel(i,j));
      // temp.Pixel(i,j).Clamp();
    }
  }

  replaceWithTemp(temp);
}

void R2Image::SobelY(void)
{
	// Apply the Sobel oprator to the image in Y direction

  static double sobelY[3][3] = {{-1, -2, -1},
                               { 0,  0,  0},
                               { 1,  2,  1}};

  R2Image temp(width, height);
  R2Pixel R2greyscale_pixel(0.5, 0.5, 0.5, 1.0);

  for (int i = 1; i < width-1; i++) {
    for (int j = 1;  j < height-1; j++) {
      R2Pixel sumTotal;
      for(int k = 0; k < 3; ++k) {
        for(int l = 0; l < 3; ++l) {
          sumTotal += sobelY[k][l]*Pixel(i+k-1, j+l-1);
        }
      }

      temp.Pixel(i,j) = sumTotal;// + R2greyscale_pixel;
      convertToGrayscale(temp.Pixel(i,j));
      // temp.Pixel(i,j).Clamp();
    }
  }

  for (int i = 1; i < width-1; i++) {
    for (int j = 1;  j < height-1; j++) {
      Pixel(i,j) = temp.Pixel(i,j);
      // Pixel(i,j).Clamp();
    }
  }
}

double laplacianOfGaussian(double x, double y)
{
  double sig = 2.0;
  double x2y2divsig = (pow(x,2)+pow(y,2))/(2*pow(sig,2));
  return 1/(M_PI*pow(sig,4)) * (x2y2divsig-1) * exp(-x2y2divsig);
}

// incomplete implementation, needs more work
void R2Image::LoG(void)
{
  // Apply the LoG (Laplacian of Gaussian) operator to the image
  int k = 3;
  double logKernel[3][3];
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < k; j ++) {
      logKernel[i][j] = laplacianOfGaussian(i,j);
      //fprintf(stderr, "LoG() weight: %f x: %i, y: %i\n", logKernel[i][j], i, j);
    }
  }

  R2Image temp(width, height);

  // applies calculated kernel to the temporary image (currently blank)
  applyKernelToTemp(k, k, logKernel, temp);

  replaceWithTemp(temp);

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "LoG() not implemented\n");
}



void R2Image::ChangeSaturation(double factor)
{
  // Changes the saturation of an image
  // Find a formula that changes the saturation without affecting the image brightness

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "ChangeSaturation(%g) not implemented\n", factor);
}

double gaussian2D(double x, double y, double sigma)
{
  double xval = pow(x,2)/(2*pow(sigma,2));
  double yval = pow(y,2)/(2*pow(sigma,2));
  return exp(-(xval + yval));
}

// Linear filtering ////////////////////////////////////////////////
void R2Image::Blur(double sigma)
{
  // Gaussian blur of the image. Separable solution is preferred
  int k = 3*(int)sigma;
  if (k%2 == 0) { k++; } // k should be odd for an evenly distributed kernel

  double gaussKernel[k][k];
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < k; j ++) {
      double x = (double)(i-k/2);
      double y = (double)(j-k/2);
      gaussKernel[i][j] = gaussian2D(x,y,sigma);
      // printf(" %f ",gaussKernel[i][j]);
    }
    // printf("\n");
  }
  // printf("starting computation\n");
  // temporary images for new blurred version
  R2Image temp(width, height);
  for (int y = 0; y < height; y++) {
    // bound range between 0 and the height
    int min = fmax(0, y-k/2);
    int max = fmin(height-1, y+k/2);
    for (int x = 0; x < width; x++) {
      // y direction
      double yweights = 0;
      // printf("x: %i y: %i min: %i max: %i  --  ",x,y,min,max);
      for (int ly = min; ly <= max; ly++) {
        int gausIdx = ly-y + k/2;
        float gaus = gaussKernel[k/2][gausIdx];
        temp.Pixel(x,y) += Pixel(x,ly)*gaus;
        yweights += gaus;
        // printf(" %i-%i->%f ",k,gausIdx,gaus);
      }
      // puts("");
      temp.Pixel(x,y) /= yweights;
    }
  }
  for (int x = 0; x < width; x++) {
    // remain constant across inner loop
    int min = fmax(0, x-k/2);
    int max = fmin(width-1, x+k/2);
    for (int y = 0; y < height; y++) {
      // x direction
      double xweights = 0;
      for (int lx = min; lx <= max; lx++) {
        int gausIdx = lx-x + k/2;
        float gaus = gaussKernel[gausIdx][k/2];
        Pixel(x,y) += temp.Pixel(lx,y)*gaus;
        xweights += gaus;
      }
      Pixel(x,y) /= xweights;
    }
  }
}



int vpCompare(const void * a, const void * b)
{
  double diff = (*(ValPoint*)a).val - (*(ValPoint*)b).val;
  return diff < 0 ? 1 : -1;
}

ValPoint* R2Image::topHarrisFeaturePoints(double sigma, int numPts)
{
  const R2Image thisTemp = *this;

  R2Image Ix_sq = R2Image(thisTemp);
  R2Image Iy_sq = R2Image(thisTemp);
  // create Ix and Iy values (not yet squared)
  Ix_sq.SobelX();
  Iy_sq.SobelY();

  R2Image Ix_Iy = R2Image(width, height);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      // create Ix*Iy values
      Ix_Iy.Pixel(x,y) = Ix_sq.Pixel(x,y) * Iy_sq.Pixel(x,y);

      // actually square the Ix and Iy values in those images
      Ix_sq.Pixel(x,y) = Ix_sq.Pixel(x,y)*Ix_sq.Pixel(x,y);
      Iy_sq.Pixel(x,y) = Iy_sq.Pixel(x,y)*Iy_sq.Pixel(x,y);
    }
  }

  // apply small blur to the the three temp images
  Ix_sq.Blur(sigma);
  Iy_sq.Blur(sigma);
  Ix_Iy.Blur(sigma);

  R2Image Rharris = R2Image(width,height);
  R2Pixel halfGray = R2Pixel(0.5, 0.5, 0.5, 0.0);

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      R2Pixel detA = Ix_sq.Pixel(x,y)*Iy_sq.Pixel(x,y) - Ix_Iy.Pixel(x,y)*Ix_Iy.Pixel(x,y);
      R2Pixel traceA = Ix_sq.Pixel(x,y) + Iy_sq.Pixel(x,y);
      double alpha = 0.04;

      Rharris.Pixel(x,y) = detA - alpha * (traceA * traceA);
      Rharris.Pixel(x,y) += halfGray;
      Rharris.Pixel(x,y).Clamp();

      // Pixel(x,y) = Rharris.Pixel(x,y);
    }
  }

  // creates an array of ValPoint structs to be sorted by value
  ValPoint *vals = new ValPoint[width*height];
  for (int x = 0; x < width; x ++) {
    for (int y = 0; y < height; y ++) {
      ValPoint curPoint;
      curPoint.val = Rharris.Pixel(x,y).Red();
      curPoint.x = x;
      curPoint.y = y;
      vals[x*height + y] = curPoint;
    }
  }


  // sorts ValPoint structs to find top 150
  qsort(vals, width*height, sizeof(ValPoint), vpCompare);

  ValPoint *topPoints = new ValPoint[numPts];
  int coloredCount = 0;
  for (int i = 0; coloredCount < numPts && i < width*height; i++) {
    ValPoint cur = vals[i];
    bool tooClose = false;
    for (int t = 0; t < coloredCount && !tooClose; t++) {
      int dx = topPoints[t].x - cur.x;
      int dy = topPoints[t].y - cur.y;
      int dist = sqrt(dx*dx + dy*dy);
      if (dist < 20) {
        tooClose = true;
      }
    }
    if(tooClose) {
      continue;
    }
    topPoints[coloredCount++] = cur;
  }
  delete vals;

  return topPoints;
}

void R2Image::Harris(double sigma)
{
  // Harris corner detector. Make use of the previously developed filters, such as the Gaussian blur filter
	// Output should be 50% grey at flat regions, white at corners and black/dark near edges

  const int numPts = 150;

  ValPoint *topPoints = topHarrisFeaturePoints(sigma, numPts);

  for (int i = 0; i < numPts; i++) {
    ValPoint cur = topPoints[i];
    colorAroundPoint(cur.x, cur.y, 2);
  }

  delete topPoints;
}


void R2Image::Sharpen()
{
  // Sharpen an image using a linear filter.

  float s = 1.0;
  // Selected kernel.
  static float sharpen[3][3] = {{0, -s, 0},
                       {-s, 5*s, -s},
                       {0, -s, 0}};
  double red = 0.0;
  double green = 0.0;
  double blue = 0.0;

  R2Image temp(width, height);

  for (int i = 1; i < width-1; i++) {
    for (int j = 1;  j < height-1; j++) {
      red = 0.0;
      green = 0.0;
      blue = 0.0;

      for(int k = 0; k < 3; ++k) {
        for(int l = 0; l < 3; ++l) {
          // Compute weighted sums for each color value of the center pixel.
          red += sharpen[k][l]*Pixel(i+k-1,j+l-1).Red();
          green += sharpen[k][l]*Pixel(i+k-1,j+l-1).Green();
          blue += sharpen[k][l]*Pixel(i+k-1,j+l-1).Blue();

        }
      }
      temp.Pixel(i,j).SetRed(red);
      temp.Pixel(i,j).SetGreen(green);
      temp.Pixel(i,j).SetBlue(blue);

      temp.Pixel(i,j).Clamp();
    }
  }
  // Set the temporary values to the actual image.
  for (int i = 1; i < width-1; i++) {
    for (int j = 1;  j < height-1; j++) {
      Pixel(i,j) = temp.Pixel(i,j);
      Pixel(i,j).Clamp();
    }
  }
}

int doubleBoundedMin(int offset, int p1, int p2)
{
  int pMin = p1 < p2 ? p1 : p2;
  if(pMin + offset >= 0) {
    return offset;
  } else {
    // printf("pmin: %i\n",pMin);
    return -pMin;
  }
}

int doubleBoundedMax(int offset, int p1, int p2, int h1, int h2)
{
  int pMax = p1 > p2 ? p1 : p2;
  int hMin = h1 < h2 ? h1 : h2;
  if((pMax + offset) <= hMin) {
    return offset;
  } else {
    // printf("hMin: %i pMax: %i\n",hMin,pMax);
    return offset - (hMin - pMax);
  }
}

double sumSquaredDifferences(R2Image *image1, R2Image* image2, ValPoint pt1, ValPoint pt2, int sigma, bool logging)
{
  int minDx = doubleBoundedMin(-sigma/2, pt1.x, pt2.x);
  int maxDx = doubleBoundedMax(sigma/2, pt1.x, pt2.x, image1->Width(), image2->Width());

  int minDy = doubleBoundedMin(-sigma/2, pt1.y, pt2.y);
  int maxDy = doubleBoundedMax(sigma/2, pt1.y, pt2.y, image1->Height(), image2->Height());

  if(logging)
  {
    printf("pt1(%i,%i) pt2(%i,%i) min(%i,%i) max(%i,%i)\n",pt1.x,pt1.y,pt2.x,pt2.y,minDx, minDy, maxDx, maxDy);
  }
  double sum = 0.0;
  for (int x = minDx; x < maxDx; x++) {
    for (int y = minDy; y < maxDy; y++) {
      R2Pixel diff = image1->Pixel(pt1.x + x, pt1.y + y) - image2->Pixel(pt2.x + x, pt2.y + y);
      sum += diff.Red()*diff.Red() + diff.Green()*diff.Green() + diff.Blue()*diff.Blue();
    }
  }
  return sum;
}

double normalizedCrossCorrelation(R2Image *image1, R2Image* image2, ValPoint pt1, ValPoint pt2, int sigma)
{

  return 0.0;
}

VectorOrigin* R2Image::translationVectorsToImage(ValPoint* topPoints, R2Image *otherImage, int numPts, int sigma, int maxSteps, bool indicators)
{
  VectorOrigin *translationVectors = new VectorOrigin[numPts];

  for (int ptIndex = 0; ptIndex < numPts; ptIndex++) {
    ValPoint curPt = topPoints[ptIndex];

    ValPoint minDiffPt = ValPoint(0,0,99999.0);
    ValPoint testPt = curPt;

    // This implementation spirals out from the original start point nievely. A better approach might look
    // at which direction yields the biggest improvement and follow that path.
    // it should also use the last value (or running average) of the previously found match vectors to speed up search.
    int x = 0, y = 0;
    int dx = 0, dy = -1;
    int steps = 0;
    while (x * y < maxSteps) {
      // adapted from answers at: http://stackoverflow.com/questions/398299/looping-in-a-spiral
      if (x == y || (x < 0 and x == -y) || (x > 0 && x == 1-y)) {
        int t = dx;
        dx = -dy;
        dy = t;
      }
      x += dx;
      y += dy;

      testPt.x = curPt.x + x;
      testPt.y = curPt.y + y;
      // skips points that are definitely outside of the range
      if(testPt.x < 0 || testPt.x > otherImage->Width() ||
        testPt.y < 0 || testPt.y > otherImage->Height() ||
        curPt.x < 0 || curPt.x > Width() ||
        curPt.y < 0 || curPt.y > Height()) {
          continue;
      }

      bool logging = false;
      double testDiff = sumSquaredDifferences(this, otherImage, curPt, testPt, sigma, logging);
      // double testDiff = normalizedCrossCorrelation(this, otherImage, curPt, testPt, sigma);

      if (testDiff < minDiffPt.val) {
        minDiffPt.x = testPt.x;
        minDiffPt.y = testPt.y;
        minDiffPt.val = testDiff;
      }
    }

    steps++;
    int curDx = curPt.x-minDiffPt.x;
    int curDy = curPt.y-minDiffPt.y;
    translationVectors[ptIndex] = VectorOrigin(curDx, curDy, curPt.x, curPt.y);

    if(indicators)
    {
      colorAroundPoint(curPt.x, curPt.y, 2);
      drawVectorFromPoint(curPt, ValPoint(curDx, curDy));
      colorAroundPoint(curPt.x + curDx, curPt.y + curDy, 2);
      printf("Vector: ( %i , %i ) from (%i, %i) with SSD: %f\n",curPt.x-minDiffPt.x, curPt.y-minDiffPt.y, curPt.x, curPt.y, minDiffPt.val);
    }
  }
  return translationVectors;
}

double2 averageVector(ValPoint* vectors, int numPts)
{
  double avgX = 0.0;
  double avgY = 0.0;
  for (int i = 0; i < numPts; i++) {
    avgX += (double)vectors[i].x;
    avgY += (double)vectors[i].y;
  }
  avgX /= (double)numPts;
  avgY /= (double)numPts;
  return double2(avgX, avgY);
}

void R2Image::blendOtherImageTranslated(R2Image * otherImage)
{
	// find at least 100 features on this image, and another 100 on the "otherImage". Based on these,
	// compute the matching translation (pixel precision is OK), and blend the translated "otherImage"
	// into this image with a 50% opacity.

  // number of feature points to attempt matching
  const int numPts = 100;
  // size of the examination kernel
  const int sigma = 15;
  // step size for the spiral search pattern for a match
  const int maxSteps = 10000;


  // calculates the top points from the harris filter and the translation between images based on those
  puts("calculating top Harris feature points...");
  ValPoint *topPoints = topHarrisFeaturePoints(sigma, numPts);
  puts("calculating translation vectors from feature points...");
  VectorOrigin *translationVectors = translationVectorsToImage(topPoints, otherImage, numPts, sigma, maxSteps, false);

  // calculate averages
  // double2 totalAvg = averageVector(translationVectors, numPts);

  // // calculate 2 standard deviations away
  // double stdDevLimX = 0.0;
  // double stdDevLimY = 0.0;
  // for (int i = 0; i < numPts; i++) {
  //   stdDevLimX += pow(translationVectors[i].x - totalAvg.x, 2);
  //   stdDevLimY += pow(translationVectors[i].y - totalAvg.y, 2);
  // }
  // stdDevLimX = sqrt(stdDevLimX/(double)(numPts - 1));
  // stdDevLimY = sqrt(stdDevLimY/(double)(numPts - 1));
  //
  // printf("found averages: (%f, %f), and 2*stdDevs: (%f, %f)\n",totalAvg.x, totalAvg.y, stdDevLimX, stdDevLimY);
  //
  // // calculate averages of numbers within t standard deviations of the mean
  // double avgDx = 0;
  // double avgDy = 0;
  // int usedPtCount = 0;
  // for (int i = 0; i < numPts; i++) {
  //   double curX = translationVectors[i].x;
  //   double curY = translationVectors[i].y;
  //   if (abs(curX - totalAvg.x) < stdDevLimX && abs(curY - totalAvg.y) < stdDevLimY) {
  //     avgDx += curX;
  //     avgDy += curY;
  //     usedPtCount++;
  //   }
  // }
  // avgDx /= (double)usedPtCount;
  // avgDy /= (double)usedPtCount;


  const int threshold = 5;

  double2 avg;
  int ransacCount = 0;
  int bestVecIndx = 0;
  int mostInliers = 0;
  while(ransacCount++ < 10) {
    int cur = rand()%numPts;

    int inliers = 0;
    for (int i = 0; i < numPts; i++) {
      int xDiff = translationVectors[i].xVec - translationVectors[cur].xVec;
      int yDiff = translationVectors[i].yVec - translationVectors[cur].yVec;
      if (abs(xDiff) < threshold && abs(yDiff) < threshold) {
        inliers++;
      }
    }
    if(inliers > mostInliers) {
      bestVecIndx = cur;
      mostInliers = inliers;
    }
  }
  printf("found %i inliers out of %i from vector (%i, %i)\n",mostInliers, numPts,translationVectors[bestVecIndx].xVec,translationVectors[bestVecIndx].yVec);
  for (int i = 0; i < numPts; i++) {
    int xDiff = translationVectors[i].xVec - translationVectors[bestVecIndx].xVec;
    int yDiff = translationVectors[i].yVec - translationVectors[bestVecIndx].yVec;
    // printf("(%i,%i)<=>(%i,%i) xDiff: %i yDiff:%i\n",translationVectors[i].xVec,translationVectors[i].yVec,translationVectors[bestVecIndx].xVec,translationVectors[bestVecIndx].yVec,xDiff, yDiff);
    if (abs(xDiff) < threshold && abs(yDiff) < threshold) { // gaurd statement for pixels too close
      line(translationVectors[i].xOrg,
           translationVectors[i].xOrg + translationVectors[i].xVec,
           translationVectors[i].yOrg,
           translationVectors[i].yOrg + translationVectors[i].yVec,
           0, 1, 0);
      colorAroundPoint(translationVectors[i].xOrg, translationVectors[i].yOrg, 2);
    }
  }

  // overlays the other image onto the current
  // printf("error-corrected translation vector: (%f, %f)\n",avg.x, avg.y);
  // int xShift = (int)floor(avg.x);
  // int yShift = (int)floor(avg.y);
  // for (int x = 0; x < Width(); x++) {
  //   for (int y = 0; y < Height(); y++) {
  //     int newX = x - xShift;
  //     int newY = y - yShift;
  //     if (newX < 0 || newY < 0 || newX > Width() || newY > Height()) {
  //       continue;
  //     }
  //     // Pixel(x,y) = (Pixel(x,y) + otherImage->Pixel(newX,newY)) / 2.0;
  //   }
  // }

	fprintf(stderr, "fit other image using translation and blend imageB over imageA\n");
	return;
}

void R2Image::blendOtherImageHomography(R2Image * otherImage)
{
	// find at least 100 features on this image, and another 100 on the "otherImage". Based on these,
	// compute the matching homography, and blend the transformed "otherImage" into this image with a 50% opacity.

  // number of feature points to attempt matching
  const int numPts = 100;
  // size of the examination kernel
  // const int sigma = 15;

  // ValPoint *topPoints = topHarrisFeaturePoints(sigma, numPts);
  // ValPoint *translationVectors = new ValPoint[numPts];

  for (int ptIndex = 0; ptIndex < numPts; ptIndex++) {

  }

	fprintf(stderr, "fit other image using a homography and blend imageB over imageA\n");
	return;
}

////////////////////////////////////////////////////////////////////////
// I/O Functions
////////////////////////////////////////////////////////////////////////

int R2Image::
Read(const char *filename)
{
  // Initialize everything
  if (pixels) { delete [] pixels; pixels = NULL; }
  npixels = width = height = 0;

  // Parse input filename extension
  char *input_extension;
  if (!(input_extension = (char*)strrchr(filename, '.'))) {
    fprintf(stderr, "Input file has no extension (e.g., .jpg).\n");
    return 0;
  }

  // Read file of appropriate type
  if (!strncmp(input_extension, ".bmp", 4)) return ReadBMP(filename);
  else if (!strncmp(input_extension, ".ppm", 4)) return ReadPPM(filename);
  else if (!strncmp(input_extension, ".jpg", 4)) return ReadJPEG(filename);
  else if (!strncmp(input_extension, ".jpeg", 5)) return ReadJPEG(filename);

  // Should never get here
  fprintf(stderr, "Unrecognized image file extension");
  return 0;
}



int R2Image::
Write(const char *filename) const
{
  // Parse input filename extension
  char *input_extension;
  if (!(input_extension = (char*)strrchr(filename, '.'))) {
    fprintf(stderr, "Input file has no extension (e.g., .jpg).\n");
    return 0;
  }

  // Write file of appropriate type
  if (!strncmp(input_extension, ".bmp", 4)) return WriteBMP(filename);
  else if (!strncmp(input_extension, ".ppm", 4)) return WritePPM(filename, 1);
  else if (!strncmp(input_extension, ".jpg", 5)) return WriteJPEG(filename);
  else if (!strncmp(input_extension, ".jpeg", 5)) return WriteJPEG(filename);

  // Should never get here
  fprintf(stderr, "Unrecognized image file extension");
  return 0;
}



////////////////////////////////////////////////////////////////////////
// BMP I/O
////////////////////////////////////////////////////////////////////////

#if (RN_OS == RN_LINUX) && !WIN32

typedef struct tagBITMAPFILEHEADER {
  unsigned short int bfType;
  unsigned int bfSize;
  unsigned short int bfReserved1;
  unsigned short int bfReserved2;
  unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  unsigned int biSize;
  int biWidth;
  int biHeight;
  unsigned short int biPlanes;
  unsigned short int biBitCount;
  unsigned int biCompression;
  unsigned int biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBTRIPLE {
  unsigned char rgbtBlue;
  unsigned char rgbtGreen;
  unsigned char rgbtRed;
} RGBTRIPLE;

typedef struct tagRGBQUAD {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD;

#endif

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

#define BMP_BF_TYPE 0x4D42 /* word BM */
#define BMP_BF_OFF_BITS 54 /* 14 for file header + 40 for info header (not sizeof(), but packed size) */
#define BMP_BI_SIZE 40 /* packed size of info header */


static unsigned short int WordReadLE(FILE *fp)
{
  // Read a unsigned short int from a file in little endian format
  unsigned short int lsb, msb;
  lsb = getc(fp);
  msb = getc(fp);
  return (msb << 8) | lsb;
}



static void WordWriteLE(unsigned short int x, FILE *fp)
{
  // Write a unsigned short int to a file in little endian format
  unsigned char lsb = (unsigned char) (x & 0x00FF); putc(lsb, fp);
  unsigned char msb = (unsigned char) (x >> 8); putc(msb, fp);
}



static unsigned int DWordReadLE(FILE *fp)
{
  // Read a unsigned int word from a file in little endian format
  unsigned int b1 = getc(fp);
  unsigned int b2 = getc(fp);
  unsigned int b3 = getc(fp);
  unsigned int b4 = getc(fp);
  return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



static void DWordWriteLE(unsigned int x, FILE *fp)
{
  // Write a unsigned int to a file in little endian format
  unsigned char b1 = (x & 0x000000FF); putc(b1, fp);
  unsigned char b2 = ((x >> 8) & 0x000000FF); putc(b2, fp);
  unsigned char b3 = ((x >> 16) & 0x000000FF); putc(b3, fp);
  unsigned char b4 = ((x >> 24) & 0x000000FF); putc(b4, fp);
}



static int LongReadLE(FILE *fp)
{
  // Read a int word from a file in little endian format
  int b1 = getc(fp);
  int b2 = getc(fp);
  int b3 = getc(fp);
  int b4 = getc(fp);
  return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



static void LongWriteLE(int x, FILE *fp)
{
  // Write a int to a file in little endian format
  char b1 = (x & 0x000000FF); putc(b1, fp);
  char b2 = ((x >> 8) & 0x000000FF); putc(b2, fp);
  char b3 = ((x >> 16) & 0x000000FF); putc(b3, fp);
  char b4 = ((x >> 24) & 0x000000FF); putc(b4, fp);
}



int R2Image::
ReadBMP(const char *filename)
{
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  /* Read file header */
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = WordReadLE(fp);
  bmfh.bfSize = DWordReadLE(fp);
  bmfh.bfReserved1 = WordReadLE(fp);
  bmfh.bfReserved2 = WordReadLE(fp);
  bmfh.bfOffBits = DWordReadLE(fp);

  /* Check file header */
  assert(bmfh.bfType == BMP_BF_TYPE);
  /* ignore bmfh.bfSize */
  /* ignore bmfh.bfReserved1 */
  /* ignore bmfh.bfReserved2 */
  assert(bmfh.bfOffBits == BMP_BF_OFF_BITS);

  /* Read info header */
  BITMAPINFOHEADER bmih;
  bmih.biSize = DWordReadLE(fp);
  bmih.biWidth = LongReadLE(fp);
  bmih.biHeight = LongReadLE(fp);
  bmih.biPlanes = WordReadLE(fp);
  bmih.biBitCount = WordReadLE(fp);
  bmih.biCompression = DWordReadLE(fp);
  bmih.biSizeImage = DWordReadLE(fp);
  bmih.biXPelsPerMeter = LongReadLE(fp);
  bmih.biYPelsPerMeter = LongReadLE(fp);
  bmih.biClrUsed = DWordReadLE(fp);
  bmih.biClrImportant = DWordReadLE(fp);

  // Check info header
  assert(bmih.biSize == BMP_BI_SIZE);
  assert(bmih.biWidth > 0);
  assert(bmih.biHeight > 0);
  assert(bmih.biPlanes == 1);
  assert(bmih.biBitCount == 24);  /* RGB */
  assert(bmih.biCompression == BI_RGB);   /* RGB */
  int lineLength = bmih.biWidth * 3;  /* RGB */
  if ((lineLength % 4) != 0) lineLength = (lineLength / 4 + 1) * 4;
  assert(bmih.biSizeImage == (unsigned int) lineLength * (unsigned int) bmih.biHeight);

  // Assign width, height, and number of pixels
  width = bmih.biWidth;
  height = bmih.biHeight;
  npixels = width * height;

  // Allocate unsigned char buffer for reading pixels
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = bmih.biSizeImage;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Read buffer
  fseek(fp, (long) bmfh.bfOffBits, SEEK_SET);
  if (fread(buffer, 1, bmih.biSizeImage, fp) != bmih.biSizeImage) {
    fprintf(stderr, "Error while reading BMP file %s", filename);
    return 0;
  }

  // Close file
  fclose(fp);

  // Allocate pixels for image
  pixels = new R2Pixel [ width * height ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Assign pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      double b = (double) *(p++) / 255;
      double g = (double) *(p++) / 255;
      double r = (double) *(p++) / 255;
      R2Pixel pixel(r, g, b, 1);
      SetPixel(i, j, pixel);
    }
  }

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return success
  return 1;
}



int R2Image::
WriteBMP(const char *filename) const
{
  // Open file
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Compute number of bytes in row
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;

  // Write file header
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = BMP_BF_TYPE;
  bmfh.bfSize = BMP_BF_OFF_BITS + rowsize * height;
  bmfh.bfReserved1 = 0;
  bmfh.bfReserved2 = 0;
  bmfh.bfOffBits = BMP_BF_OFF_BITS;
  WordWriteLE(bmfh.bfType, fp);
  DWordWriteLE(bmfh.bfSize, fp);
  WordWriteLE(bmfh.bfReserved1, fp);
  WordWriteLE(bmfh.bfReserved2, fp);
  DWordWriteLE(bmfh.bfOffBits, fp);

  // Write info header
  BITMAPINFOHEADER bmih;
  bmih.biSize = BMP_BI_SIZE;
  bmih.biWidth = width;
  bmih.biHeight = height;
  bmih.biPlanes = 1;
  bmih.biBitCount = 24;       /* RGB */
  bmih.biCompression = BI_RGB;    /* RGB */
  bmih.biSizeImage = rowsize * (unsigned int) bmih.biHeight;  /* RGB */
  bmih.biXPelsPerMeter = 2925;
  bmih.biYPelsPerMeter = 2925;
  bmih.biClrUsed = 0;
  bmih.biClrImportant = 0;
  DWordWriteLE(bmih.biSize, fp);
  LongWriteLE(bmih.biWidth, fp);
  LongWriteLE(bmih.biHeight, fp);
  WordWriteLE(bmih.biPlanes, fp);
  WordWriteLE(bmih.biBitCount, fp);
  DWordWriteLE(bmih.biCompression, fp);
  DWordWriteLE(bmih.biSizeImage, fp);
  LongWriteLE(bmih.biXPelsPerMeter, fp);
  LongWriteLE(bmih.biYPelsPerMeter, fp);
  DWordWriteLE(bmih.biClrUsed, fp);
  DWordWriteLE(bmih.biClrImportant, fp);

  // Write image, swapping blue and red in each pixel
  int pad = rowsize - width * 3;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      const R2Pixel& pixel = (*this)[i][j];
      double r = 255.0 * pixel.Red();
      double g = 255.0 * pixel.Green();
      double b = 255.0 * pixel.Blue();
      if (r >= 255) r = 255;
      if (g >= 255) g = 255;
      if (b >= 255) b = 255;
      fputc((unsigned char) b, fp);
      fputc((unsigned char) g, fp);
      fputc((unsigned char) r, fp);
    }

    // Pad row
    for (int i = 0; i < pad; i++) fputc(0, fp);
  }

  // Close file
  fclose(fp);

  // Return success
  return 1;
}



////////////////////////////////////////////////////////////////////////
// PPM I/O
////////////////////////////////////////////////////////////////////////

int R2Image::
ReadPPM(const char *filename)
{
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Read PPM file magic identifier
  char buffer[128];
  if (!fgets(buffer, 128, fp)) {
    fprintf(stderr, "Unable to read magic id in PPM file");
    fclose(fp);
    return 0;
  }

  // skip comments
  int c = getc(fp);
  while (c == '#') {
    while (c != '\n') c = getc(fp);
    c = getc(fp);
  }
  ungetc(c, fp);

  // Read width and height
  if (fscanf(fp, "%d%d", &width, &height) != 2) {
    fprintf(stderr, "Unable to read width and height in PPM file");
    fclose(fp);
    return 0;
  }

  // Read max value
  double max_value;
  if (fscanf(fp, "%lf", &max_value) != 1) {
    fprintf(stderr, "Unable to read max_value in PPM file");
    fclose(fp);
    return 0;
  }

  // Allocate image pixels
  pixels = new R2Pixel [ width * height ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for PPM file");
    fclose(fp);
    return 0;
  }

  // Check if raw or ascii file
  if (!strcmp(buffer, "P6\n")) {
    // Read up to one character of whitespace (\n) after max_value
    int c = getc(fp);
    if (!isspace(c)) putc(c, fp);

    // Read raw image data
    // First ppm pixel is top-left, so read in opposite scan-line order
    for (int j = height-1; j >= 0; j--) {
      for (int i = 0; i < width; i++) {
        double r = (double) getc(fp) / max_value;
        double g = (double) getc(fp) / max_value;
        double b = (double) getc(fp) / max_value;
        R2Pixel pixel(r, g, b, 1);
        SetPixel(i, j, pixel);
      }
    }
  }
  else {
    // Read asci image data
    // First ppm pixel is top-left, so read in opposite scan-line order
    for (int j = height-1; j >= 0; j--) {
      for (int i = 0; i < width; i++) {
	// Read pixel values
	int red, green, blue;
	if (fscanf(fp, "%d%d%d", &red, &green, &blue) != 3) {
	  fprintf(stderr, "Unable to read data at (%d,%d) in PPM file", i, j);
	  fclose(fp);
	  return 0;
	}

	// Assign pixel values
	double r = (double) red / max_value;
	double g = (double) green / max_value;
	double b = (double) blue / max_value;
        R2Pixel pixel(r, g, b, 1);
        SetPixel(i, j, pixel);
      }
    }
  }

  // Close file
  fclose(fp);

  // Return success
  return 1;
}



int R2Image::
WritePPM(const char *filename, int ascii) const
{
  // Check type
  if (ascii) {
    // Open file
    FILE *fp = fopen(filename, "w");
    if (!fp) {
      fprintf(stderr, "Unable to open image file: %s", filename);
      return 0;
    }

    // Print PPM image file
    // First ppm pixel is top-left, so write in opposite scan-line order
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    for (int j = height-1; j >= 0 ; j--) {
      for (int i = 0; i < width; i++) {
        const R2Pixel& p = (*this)[i][j];
        int r = (int) (255 * p.Red());
        int g = (int) (255 * p.Green());
        int b = (int) (255 * p.Blue());
        fprintf(fp, "%-3d %-3d %-3d  ", r, g, b);
        if (((i+1) % 4) == 0) fprintf(fp, "\n");
      }
      if ((width % 4) != 0) fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    // Close file
    fclose(fp);
  }
  else {
    // Open file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
      fprintf(stderr, "Unable to open image file: %s", filename);
      return 0;
    }

    // Print PPM image file
    // First ppm pixel is top-left, so write in opposite scan-line order
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    for (int j = height-1; j >= 0 ; j--) {
      for (int i = 0; i < width; i++) {
        const R2Pixel& p = (*this)[i][j];
        int r = (int) (255 * p.Red());
        int g = (int) (255 * p.Green());
        int b = (int) (255 * p.Blue());
        fprintf(fp, "%c%c%c", r, g, b);
      }
    }

    // Close file
    fclose(fp);
  }

  // Return success
  return 1;
}



////////////////////////////////////////////////////////////////////////
// JPEG I/O
////////////////////////////////////////////////////////////////////////


// #define USE_JPEG
#ifdef USE_JPEG
  extern "C" {
#   define XMD_H // Otherwise, a conflict with INT32
#   undef FAR // Otherwise, a conflict with windows.h
#   include "jpeg/jpeglib.h"
  };
#endif



int R2Image::
ReadJPEG(const char *filename)
{
#ifdef USE_JPEG
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Initialize decompression info
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, fp);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  // Remember image attributes
  width = cinfo.output_width;
  height = cinfo.output_height;
  npixels = width * height;
  int ncomponents = cinfo.output_components;

  // Allocate pixels for image
  pixels = new R2Pixel [ npixels ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Allocate unsigned char buffer for reading image
  int rowsize = ncomponents * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = rowsize * height;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for JPEG file");
    fclose(fp);
    return 0;
  }

  // Read scan lines
  // First jpeg pixel is top-left, so read pixels in opposite scan-line order
  while (cinfo.output_scanline < cinfo.output_height) {
    int scanline = cinfo.output_height - cinfo.output_scanline - 1;
    unsigned char *row_pointer = &buffer[scanline * rowsize];
    jpeg_read_scanlines(&cinfo, &row_pointer, 1);
  }

  // Free everything
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  // Close file
  fclose(fp);

  // Assign pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      double r, g, b, a;
      if (ncomponents == 1) {
        r = g = b = (double) *(p++) / 255;
        a = 1;
      }
      else if (ncomponents == 1) {
        r = g = b = (double) *(p++) / 255;
        a = 1;
        p++;
      }
      else if (ncomponents == 3) {
        r = (double) *(p++) / 255;
        g = (double) *(p++) / 255;
        b = (double) *(p++) / 255;
        a = 1;
      }
      else if (ncomponents == 4) {
        r = (double) *(p++) / 255;
        g = (double) *(p++) / 255;
        b = (double) *(p++) / 255;
        a = (double) *(p++) / 255;
      }
      else {
        fprintf(stderr, "Unrecognized number of components in jpeg image: %d\n", ncomponents);
        return 0;
      }
      R2Pixel pixel(r, g, b, a);
      SetPixel(i, j, pixel);
    }
  }

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return success
  return 1;
#else
  fprintf(stderr, "JPEG not supported");
  return 0;
#endif
}




int R2Image::
WriteJPEG(const char *filename) const
{
#ifdef USE_JPEG
  // Open file
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Initialize compression info
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, fp);
  cinfo.image_width = width; 	/* image width and height, in pixels */
  cinfo.image_height = height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  cinfo.dct_method = JDCT_ISLOW;
  jpeg_set_defaults(&cinfo);
  cinfo.optimize_coding = TRUE;
  jpeg_set_quality(&cinfo, 75, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  // Allocate unsigned char buffer for reading image
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = rowsize * height;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for JPEG file");
    fclose(fp);
    return 0;
  }

  // Fill buffer with pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      const R2Pixel& pixel = (*this)[i][j];
      int r = (int) (255 * pixel.Red());
      int g = (int) (255 * pixel.Green());
      int b = (int) (255 * pixel.Blue());
      if (r > 255) r = 255;
      if (g > 255) g = 255;
      if (b > 255) b = 255;
      *(p++) = r;
      *(p++) = g;
      *(p++) = b;
    }
  }



  // Output scan lines
  // First jpeg pixel is top-left, so write in opposite scan-line order
  while (cinfo.next_scanline < cinfo.image_height) {
    int scanline = cinfo.image_height - cinfo.next_scanline - 1;
    unsigned char *row_pointer = &buffer[scanline * rowsize];
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

  // Free everything
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  // Close file
  fclose(fp);

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return number of bytes written
  return 1;
#else
  fprintf(stderr, "JPEG not supported");
  return 0;
#endif
}
