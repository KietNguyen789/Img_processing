#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
using namespace cv;
/*Inputimg: Đây là tham số đầu vào cho hàm imread và đại diện cho đường dẫn tới tệp ảnh để  đọc.
cv::IMREAD_COLOR: Đây là một cờ (flag) đã được định nghĩa trong OpenCV để chỉ định chế độ đọc ảnh, như : cv::IMREAD_COLOR cho biết rằng ảnh sẽ được đọc có mỗi pixel là 3 kênh màu, cv::IMREAD_GRAYSCALE để đọc ảnh trong chế độ ảnh xám.
*/
// Lưu output gray image (Ma trận pixel) vào đường link Outputimg
// //Khai báo Outputimage có kiểu Mat(ma trận) có: chiều cao là height(số dòng ma trận), dài width(số cột ma trận), CV_8UC1 là mỗi pixel có 1 kênh màu kiểu unsigned char
/////////////////////// Chuyển đổi ảnh màu sang ảnh xám
//Input: const Mat& image: Ma tran anh mau 
//Output: Mat& Outputimage: Ma tran anh xam
int RgbToGrayScale( Mat image,  Mat& Outputimage)
{
	if (image.empty())
		return 0;
	
	int width = image.cols, height = image.rows;
	//width: chiều dài của image
	//height: chiều cao của image 
	int channels = image.channels();
	//channels: số kênh màu của image
	
	Mat out = Mat(height, width, CV_8UC1);
	for (int y = 0; y < height; y++)
	{
		unsigned char* pRow = image.ptr<unsigned char>(y);
		//khai báo con trỏ trỏ tới địa chỉ của phần tử đầu tiên của hàng y
		unsigned char* OutpRow = out.ptr<unsigned char>(y);
		for (int x = 0; x < width; x++, pRow += channels, OutpRow += 1)
		{
			*OutpRow = (unsigned char)((pRow[0] + pRow[1] + pRow[2]) / 3);

		}
	}
	Outputimage = out;
	namedWindow("Gray Scale Image");
	// Đặt tên cho cửa sổ trống, hữu ích khi muốn tùy chỉnh các thuộc tính của cửa sổ đồ họa như kích thước, độ phân giải, hoặc chế độ hiển thị khác
	imshow("Gray Scale Image", out);
	// Xuất ảnh hiện thị vào cùng của sổ tên Gray Scale Image
	waitKey(0);	
	return 1;
}
/////////////////////// Thay đổi độ sáng của ảnh
int ChangeBrightness(Mat image, Mat& Outputimage ,int bightness)
{
	if (image.empty())
		return 0;
	
	int width = image.cols, height = image.rows;
	int channels = image.channels();
	
	for (int y = 0; y < height; y++)
	{
		unsigned char* pRow = image.ptr<unsigned char>(y);
		unsigned char* outpRow = Outputimage.ptr<unsigned char>(y);
		for (int x = 0; x < width; x++, pRow += channels,outpRow+=channels)
		{
			int blue = pRow[0]+bightness;
			int green = pRow[1] + bightness;
			int red = pRow[2] + bightness;
			if (blue > 255)
				blue = 255;
			else if (blue < 0)
				blue = 0;
			if (red > 255)
				red = 255;
			else if (red < 0)
				red = 0;
			if (green > 255)
				green = 255;
			else if (green < 0)
				green = 0;
			outpRow[0] = blue;
			outpRow[1] = green;
			outpRow[2] = red;
			
			
		}
	
	}
	
	namedWindow("Change Brightness Image");
	imshow("Change Brightness Image", Outputimage);
	waitKey(0);
	return 1;
}
/////////////////////// Thay đổi độ tương phản của ảnh
int ChangeContrast(Mat image, Mat& Outputimage, double gramma)
{
	if (image.empty())
		return 0;

	int width = image.cols, height = image.rows;
	int channels = image.channels();
	
	image.convertTo(image, CV_64FC3);
	Outputimage.convertTo(Outputimage, CV_8UC3);
	// chuyển Ma trận image từ kiểu unsigned char sang kiểu double
	image = image / 255;

	for (int y = 0; y < height; y++)
	{
		double* pRow = image.ptr<double>(y);
		unsigned char* outpRow = Outputimage.ptr<unsigned char>(y);
		for (int x = 0; x < width; x++, pRow += channels, outpRow += channels)
		{
			outpRow[0] = (unsigned char)(pow(pRow[0],gramma)*255);
			// (pow(pRow[0],gramma)*255) chuẩn hóa về từ 0 - 255
			// (unsigned char)(pow(pRow[0],gramma)*255) ép kiểu sang unsigned char
			outpRow[1] = (unsigned char)(pow(pRow[1], gramma)*255);
			outpRow[2] = (unsigned char)(pow(pRow[2], gramma)*255);

		}
		
	}
	
	namedWindow("Change Contrast Image");
	imshow("Change Contrast Image", Outputimage);
	waitKey(0);
	return 1;
}


/////////////////////// áp dụng bộ lọc trung bình lên ảnh
int AverageFilter(Mat image, Mat& result, int kernelSize) {
	if (image.empty())
		return 0;
	int rows = image.rows;
	int cols = image.cols;
	int channels = image.channels();
	int numele = kernelSize * kernelSize;
	result.convertTo(result, CV_8UC3);
	if (kernelSize > 2)
	{
		if (kernelSize % 2 == 0)
		{
			kernelSize -= 1;
		}

		int halfKernelSize = kernelSize / 2;


		for (int i = halfKernelSize; i < rows - halfKernelSize; i += kernelSize - 1) {
			for (int j = halfKernelSize; j < cols - halfKernelSize; j += kernelSize - 1) {
				int sumR = 0;
				int sumG = 0;
				int sumB = 0;
				// duyệt các phần tử trong kernel
				for (int m = -halfKernelSize; m <= halfKernelSize; m++) {


					for (int n = -halfKernelSize; n <= halfKernelSize; n++) {
						int colIndex = j + n;
						int rowIndex = m + i;


						// trỏ con trỏ đến mảng ba phần tử blue, green, red
						unsigned char* pRow = image.ptr<unsigned char>((i + m), (j + n));
						sumR += pRow[2];
						sumG += pRow[1];
						sumB += pRow[0];


					}
				}

				float AverageR = sumR / numele;
				float AverageG = sumG / numele;
				float AverageB = sumB / numele;
				// gán giá trị tại vị trí tương ứng với Input lên Output
				for (int m = -halfKernelSize; m <= halfKernelSize; m++) {


					for (int n = -halfKernelSize; n <= halfKernelSize; n++) {
						int colIndex = j + n;
						int rowIndex = m + i;


						
						unsigned char* OutpRow = result.ptr<unsigned char>((i + m), (j + n));
						OutpRow[0] = AverageB;
						OutpRow[1] = AverageG;
						OutpRow[2] = AverageR;


					}
				}
			}
		}
		
		namedWindow("Average filter");
		imshow("Average filter", result);
		waitKey(0);
	}
	return 1;

}
/////////////////////// áp dụng bộ lọc trung vị lên ảnh
int MedianFilter(const cv::Mat& image, cv::Mat& result, int kernelSize) {
	if (image.empty())
		return 0;

	int rows = image.rows;
	int cols = image.cols;
	int channels = image.channels();
	int numele = kernelSize * kernelSize;
	int halfKernelSize = kernelSize / 2;

	for (int i = halfKernelSize; i < rows - halfKernelSize; i++) {
		for (int j = halfKernelSize; j < cols - halfKernelSize; j++) {
			std::vector<unsigned char> values(channels);

			for (int c = 0; c < channels; c++) {
				std::vector<unsigned char> windowValues;

				for (int m = -halfKernelSize; m <= halfKernelSize; m++) {
					for (int n = -halfKernelSize; n <= halfKernelSize; n++) {
						unsigned char value = image.at<cv::Vec3b>(i + m, j + n)[c];
						windowValues.push_back(value);
					}
				}

				std::sort(windowValues.begin(), windowValues.end());
				values[c] = windowValues[numele / 2];
			}

			for (int c = 0; c < channels; c++) {
				result.at<cv::Vec3b>(i, j)[c] = values[c];
			}
		}
	}
	imshow("out", result);
	waitKey();
	return 1;
}

/////////////////////// hàm tích chập
void myfilter2D(int* image, int image_width, int image_height, int* kernel, int kernel_width, int kernel_height, int* result) {
	int padding_width = kernel_width / 2;
	int padding_height = kernel_height / 2;

	for (int i = padding_height; i < image_height - padding_height; i++) {
		for (int j = padding_width; j < image_width - padding_width; j++) {
			int value = 0;
			for (int k = -padding_height; k <= padding_height; k++) {
				for (int l = -padding_width; l <= padding_width; l++) {
					int image_index = (i + k) * image_width + (j + l);
					int kernel_index = (k + padding_height) * kernel_width + (l + padding_width);
					value += image[image_index] * kernel[kernel_index];
				}
			}
			int result_index = i * image_width + j;
			result[result_index] = value;
		}
	}
}
/////////////////////// tạo gauss kernel
void generateGaussfilter(Mat& GKernel, int kernelsize, double sigma)
{
	double r, s = 2.0 * sigma * sigma;
	int halfkernel = kernelsize / 2;
	double sum = 0.0;
	GKernel= Mat(kernelsize, kernelsize, CV_64FC1);
	// Tạo một kernel trống dài rộng kernelsize kiểu double
	for (int x = -halfkernel; x <= halfkernel; x++) {
		double* pRow = GKernel.ptr<double>(x + halfkernel);
		for (int y = -halfkernel; y <= halfkernel; y++) {
			r = sqrt(x * x + y * y);
			pRow[y + halfkernel] = exp(-(r * r) / s) / (CV_PI * s);
			sum += pRow[y + halfkernel];
		}
	}
	
	for (int i = 0; i < kernelsize; i++) {
		double* pRow = GKernel.ptr<double>(i);
		for (int j = 0; j < kernelsize; j++)
			pRow[j] /= sum;
	}
}
// Them ria de khong lam mat pixel
void applyfilter(const cv::Mat& inputImage, int kernelSize, const Mat& kernel, Mat& filteredImage)
{
	//cv::Mat paddedImage;
	filteredImage = Mat(inputImage.rows,inputImage.cols,CV_64FC1);
	//int borderSize = kernelSize / 2;
	//cv::copyMakeBorder(inputImage, paddedImage, borderSize, borderSize, borderSize, borderSize, cv::BORDER_REPLICATE);
	// Áp dụng bộ lọc bằng tích chập
	
	try
	{
		// ... Contents of your main
		filter2D(inputImage, filteredImage, CV_64FC1, kernel);
	}
	catch (cv::Exception& e)
	{
		std::cout << e.msg << std::endl; // output exception message
	}
	
	
	
}
/////////////////////// chuan hoa gia tri
void normalizeMatto8UC1(Mat& inputMat)
{
	double minVal, maxVal;
	Point minI, maxI;
	minMaxLoc(inputMat, &minVal, &maxVal, &minI, &maxI);
	inputMat = inputMat / maxVal * 255;
	inputMat.convertTo(inputMat, CV_8UC1);
}

/////////////////////// Áp dụng bộ lọc gauss
cv::Mat applyGaussianFilter(const cv::Mat& inputImage, int kernelSize, double sigma)
{
	
	cv::Mat kernel,result;
	// Tạo gauss kernel
	generateGaussfilter(kernel, kernelSize, sigma);
	applyfilter(inputImage, kernelSize,kernel,result);
	normalizeMatto8UC1(result);
	namedWindow("Gauss");
	imshow("Gauss", result);
	waitKey(0);
	return result;
}
/////////////////////// Áp dụng bộ lọc sobel kích thước 3 x 3
void sobel(const Mat& inputimage, Mat& sobelX, Mat& sobelY)
{
	double kernelXData[3][3] = {
		{-1,0,1},
		{-2,0,2},
		{-1,0,1}
	};
	double kernelYData[3][3] = {
		{-1,-2,-1},
		{0,0,0},
		{1,2,1}
	};
	Mat sobelXk = Mat(3, 3,CV_64FC1, kernelXData);
	Mat sobelYk = Mat(3, 3,CV_64FC1, kernelYData);
	applyfilter(inputimage, 3, sobelXk, sobelX);
	applyfilter(inputimage, 3, sobelYk, sobelY);
	normalizeMatto8UC1(sobelX);
	normalizeMatto8UC1(sobelY);

}
void n_m_suppression_loaibocanhday(Mat eMat, const Mat& Angle, Mat& result)
{
	result = eMat.clone();
	int height = eMat.rows;
	int width = eMat.cols;
	try {
		Mat Direction = Mat(Angle.rows, Angle.cols, CV_64FC1);
		Direction = Angle * 180 / 3.14;
		int unit = sizeof(double);
		for (int i = 0; i < height; i++)
		{
			double* pRow = Direction.ptr<double>(i);
			for (int j = 0; j < width; j++)
			{

				if (pRow[j] < 0)
					pRow[j] += 180;
			}
		}
		for (int i = 0; i < height; i++)
		{
			double* pRowGoc = Direction.ptr<double>(i);
			double* pRowDoLon = eMat.ptr<double>(i);
			double* pRowDoLonr = result.ptr<double>(i);
			for (int j = 0; j < width; j++)
			{
				int q = 255;
				int r = 255;
				if ((0 <= pRowGoc[j] < 20) || (157 <= pRowGoc[j] <= 180))
				{

					q = pRowDoLon[j + 1];
					r = pRowDoLon[j - 1];

				}
				else if (20 <= pRowGoc[j] < 67)
				{
					double* new_rowq = pRowDoLon + width * unit;
					double* new_rowr = pRowDoLon - width * unit;
					q = new_rowq[j - 1];
					r = new_rowr[j + 1];

				}
				else if (67 <= pRowGoc[j] < 112)
				{
					double* new_rowq = pRowDoLon + width * unit;
					double* new_rowr = pRowDoLon - width * unit;
					q = new_rowq[j];
					r = new_rowr[j];
				}
				else if (112 <= pRowGoc[j] < 157)
				{
					double* new_rowq = pRowDoLon - width * unit;
					double* new_rowr = pRowDoLon + width * unit;
					q = new_rowq[j - 1];
					r = new_rowr[j + 1];
				}
				// khong la lon nhat trong lan can 8
				if (pRowDoLon[j] < q or pRowDoLon[j] < r)
					pRowDoLonr[j] = 0;

			}
		}
		
	}
	catch (Exception& e)
	{
		std::cout << e.msg << std::endl;
	}
	
} ///////////////////////  Tính độ lớn vector gradient
Mat ComputeEfunction(Mat Matx, Mat Maty)
{

	int nrow = Matx.rows;
	int ncol = Matx.cols;
	Mat result = Mat(nrow, ncol, CV_64FC1);
	for (int i = 0; i < nrow; i++)
	{
		unsigned char* pRowx = Matx.ptr<unsigned char>(i);
		unsigned char* pRowy = Maty.ptr<unsigned char>(i);
		double* pRowr = result.ptr<double>(i);
		for (int j = 0; j < ncol; j++)
		{
			pRowr[j] = sqrt(pow(pRowx[j], 2) + pow(pRowy[j], 2));
			//std::cout << pRowr[j] << " ";
			if (pRowr[j] > 255)
				pRowr[j] = 255;
		}
	}

	return result;
}
/////////////////////// tính số đo góc hợp bởi vector gradient phương x và phương y 
Mat ComputeTheltafunction(Mat Matx, Mat Maty)
{
	int nrow = Matx.rows;
	int ncol = Maty.cols;
	Mat result = Mat(nrow, ncol, CV_64FC1);
	for (int i = 0; i < nrow; i++)
	{
		unsigned char* pRowx = Matx.ptr<unsigned char>(i);
		unsigned char* pRowy = Maty.ptr<unsigned char>(i);
		double* pRowr = result.ptr<double>(i);
		for (int j = 0; j < ncol; j++)
		{
			pRowr[j] = atan2(pRowy[j], pRowx[j]);
		   
		}
	}
	return result;
}
/////////////////////// tìm cạnh mạnh, yếu
void clibbing(Mat& result, int low, int high)
{
	int rows = result.rows;
	int cols = result.cols;
	for (int i = 0; i < rows; i++)
	{
		double* pRow = result.ptr<double>(i);
		for (int j = 0; j < cols; j++)
		{
			if (pRow[j] > high)
			{
				pRow[j] = 255;
			}
			else if (pRow[j] >= low && pRow[j] <= high)
			{
				pRow[j] = 50;
			}
		}
	}
	

}

void Canny_sobel(const cv::Mat& inputImage, Mat& out, double sigma=1.0 )
{
	int nrow = inputImage.rows;
	int ncol = inputImage.cols;
	Mat sobelX, sobelY;
	Mat eMat , theltaMat;
	Mat image = Mat(nrow, ncol, CV_64FC1);
	Mat result;
	// chuyển đổi ảnh đầu vào sang ảnh xám
	cvtColor(inputImage, image, COLOR_BGR2GRAY);
	// ap dụng bộ lọc gauss
	// must declare before assign, because an empty 
	Mat gaussimage = Mat(nrow, ncol, CV_64FC1);
	// buoc 1 ap dung bo loc gauss loai nhieu
	gaussimage = applyGaussianFilter(image, 3, sigma);
	// ap dung bo loc sobel tim bien canh theo phuong x va y
	sobel(gaussimage, sobelX, sobelY);
	eMat = ComputeEfunction(sobelX, sobelY);
	theltaMat = ComputeTheltafunction(sobelX, sobelY);
	// loai bo canh kep
	n_m_suppression_loaibocanhday(eMat, theltaMat,result);
	// Tim bien
	clibbing(result, 50, 100);
	normalizeMatto8UC1(result);
	imshow("Edge Canny sobel", result);
	waitKey();
	out = result;
	
	
	
}
/////////////////////// Áp dụng bộ lọc laplace
void laplace(const Mat& image, Mat& result)
{

	double laplace[3][3] = {
		{0,1,0},
		{1,-4,1},
		{0,1,0}
	};
	Mat kernel = Mat(3, 3, CV_64FC1, laplace);
	result = Mat(image.rows, image.cols, CV_64FC1);
	applyfilter(image, 3, kernel, result);
	normalizeMatto8UC1(result);

}
void LaplaceEdgeDetection(const cv::Mat& inputImage, Mat&out)
{
	double sigma = 1.7;
	int nrow = inputImage.rows;
	int ncol = inputImage.cols;
	Mat image;
	cvtColor(inputImage, image, COLOR_BGR2GRAY);
	// buoc 1 ap dung bo loc gauss loai nhieu
	Mat gaussimage = Mat(nrow, ncol, CV_64FC1);
	gaussimage = applyGaussianFilter(image, 3, sigma);
	// ap dung bo loc laplace tim bien canh theo phuong x va y
	laplace(image,out);
	imshow("Edge Detection laplace", out);
	waitKey(0);

}
#include<stdio.h>
#include<string.h>
int main(int argc, char* argv[])
{
	char* func = argv[1];
	char* input = argv[2];
	char* output = argv[3];	
	char* m = argv[4];
	//std::cout << argv[1] << std::endl;
	//std::cout << argv[2] << std::endl;
	//std::cout << argv[3] << std::endl;
	const char* grayscale = "-rgb2gray";
	const char* bright = "-brightness";
	const char* constrast = "-constrast";
	const char* average = "-avg";
	const char* median = "-med";
	const char* gauss = "-gau";
	const char* sobel = "-sobel";
	const char* laplace = "-laplace";
	Mat src = imread(input, IMREAD_COLOR);
	Mat dst ;
	
	if (!strcmp(func, grayscale))
	{
		RgbToGrayScale(src, dst);
		
	}
	if (!strcmp(func, bright))
	{
		double n = (int)atof(m);
		ChangeBrightness(src, dst,n);
	}
	if (!strcmp(func, constrast))
	{
		double n = atof(m);
		ChangeContrast(src, dst,n);
	}
	if (!strcmp(func, average))
	{
		double n = atof(m);
		AverageFilter(src, dst,(int)n);
	}
	if (!strcmp(func, median))
	{
		double n = atof(m);
		MedianFilter(src, dst,(int)n);
	}
	if (!strcmp(func,gauss))
	{
		double n = atof(m);
		dst = applyGaussianFilter(src, 3, n);
	}
	if (!strcmp(func,sobel))
	{
		Canny_sobel(src, dst);
		
	}
	if (!strcmp(func,laplace))
	{
		LaplaceEdgeDetection(src,dst);
	}
	imwrite(output, dst);
	return 0;

}