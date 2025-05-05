#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
// Kernel operation using input operator of size 3*3
cv::Mat GetSobel(cv::Mat image, cv::Mat Sobel, int width, int height) {
    // Khởi tạo ma trận I_d height hàng width cột kiểu float giá trị mõi phần tử là 0
    cv::Mat I_d = cv::Mat::zeros(height, width, CV_32FC1);

    // For every pixel in the image
    for (int rows = 1; rows < height - 2; ++rows) {
        for (int cols = 1; cols < width - 2; ++cols) {
            // Run the Sobel kernel for each pixel
            for (int ind = 0; ind < 3; ++ind) {
                for (int ite = 0; ite < 3; ++ite) {
                    // fix unsigned char
                    I_d.at<float>(rows, cols) += Sobel.at<float>(ind, ite) * image.at<unsigned char>(rows + ind - 1, cols + ite - 1);
                }
            }
        }
    }

    return I_d;
}
// Tính định thức ma trận 2 x 2
float Caldet(Mat& input)
{
    float* ptrrow1 = input.ptr<float>(0);
    float* ptrrow2 = input.ptr<float>(1);
    return ptrrow1[0] * ptrrow2[1] - ptrrow1[1] * ptrrow2[0];
}
float Caltrace(Mat& input)
{
    float* ptrrow1 = input.ptr<float>(0);
    float* ptrrow2 = input.ptr<float>(1);
    return ptrrow1[0] + ptrrow2[1];
}
void printMat(Mat input)
{
    for (int i = 0; i < input.rows; i++)
    {
        //float* ptr = input.ptr<float>(i);
        for (int j = 0; j < input.cols; j++)
        {
            cout << input.at<float>(i,j) <<" ";
        }
        cout << endl;
    }
}
// Method implements the Harris Corner Detection algorithm
cv::Mat HarrisCornerDetection(cv::Mat image) {
    // The two Sobel operators - for x and y direction
    float sobelX[3][3] = { { -1, 0, 1},{ -2, 0, 2},{ -1, 0, 1} };
    float sobelY[3][3] = { { -1, -2, -1},{  0, 0, 0},{ 1, 2, 1} };
    cv::Mat SobelX = Mat(3, 3, CV_32FC1, sobelX);
    cv::Mat SobelY = Mat(3, 3, CV_32FC1, sobelY);
    //printMat(SobelX);
    //printMat(SobelY);
    cout << endl;
    int h = image.rows;
    int w = image.cols;
    // X and Y derivative of image using Sobel operator
    Mat ImgX = GetSobel(image, SobelX, w, h);
   // printMat(ImgX);
    Mat ImgY = GetSobel(image, SobelY, w, h);
    for (int ind1 = 0; ind1 < h; ++ind1) {
        for (int ind2 = 0; ind2 < w; ++ind2) {
            // tính giá trị tuyệt đối độ thay đổi (đạo hàm)
            if (ImgY.at<float>(ind1, ind2) < 0) {
                ImgY.at<float>(ind1, ind2) *= -1;
            }
            if (ImgX.at<float>(ind1, ind2) < 0) {
                ImgX.at<float>(ind1, ind2) *= -1;
            }
        }
    }
    // Tính ma trận bình phương đạo hàm theo phương x
    cv::Mat ImgX_2 = ImgX.mul(ImgX);
    // Tính ma trận bình phương đạo hàm theo phương x
    cv::Mat ImgY_2 = ImgY.mul(ImgY);
    // Tính ma trận đạo hàm phương x nhân phương y
    cv::Mat ImgXY = ImgX.mul(ImgY);
    // Tính ma trận đạo hàm phương y nhân phương x
    cv::Mat ImgYX = ImgY.mul(ImgX);

    // Use Gaussian Blur
    double Sigma = 1.4;
    cv::Size kernelSize(3, 3);
    // nhân hệ số cửa số gauss cho M
    cv::GaussianBlur(ImgX_2, ImgX_2, kernelSize, Sigma);
    cv::GaussianBlur(ImgY_2, ImgY_2, kernelSize, Sigma);
    cv::GaussianBlur(ImgXY, ImgXY, kernelSize, Sigma);
    cv::GaussianBlur(ImgYX, ImgYX, kernelSize, Sigma);

    float alpha = 0.06;
    cv::Mat R = cv::Mat::zeros(h, w, CV_32F);

    // For every pixel find the corner strength
    for (int row = 0; row < h; ++row) {
        for (int col = 0; col < w; ++col) {
            cv::Mat M_bar = (cv::Mat_<float>(2, 2) << ImgX_2.at<float>(row, col), ImgXY.at<float>(row, col), ImgYX.at<float>(row, col), ImgY_2.at<float>(row, col));
            // áp dụng công thức tính R
            R.at<float>(row, col) =Caldet(M_bar) - (alpha * std::pow(Caltrace(M_bar), 2));
        }
    }
    return R;
}

int main(int argc, char* argv[]) {
    char* func = argv[1];
    char* input = argv[2];
    char* output = argv[3];
    const char* sign = "-harris";
    if (!strcmp(func, sign))
    {
        cv::Mat firstimage = cv::imread(input, cv::IMREAD_GRAYSCALE);
        int h = firstimage.rows;
        int w = firstimage.cols;
        double Sigma = 1.4;
        cv::Size kernelSize(3, 3);
        // Convert image toMã tiếp theo:
        cv::Mat bgr;
        cv::cvtColor(firstimage, bgr, cv::COLOR_GRAY2BGR);
        cv::GaussianBlur(firstimage, firstimage, kernelSize, Sigma);
        // Corner detection
        cv::Mat R = HarrisCornerDetection(firstimage);

        // Empirical Parameter
        // This parameter will need tuning based on the use-case
        float CornerStrengthThreshold = 600000;

        // Plot detected corners on image
        int radius = 1;
        cv::Scalar color(0, 255, 0);  // Green
        int thickness = 1;
        cout << R.rows << " " << R.cols << endl;
        std::vector<cv::Point> PointList;
        // Look for Corner strengths above the threshold
        for (int row = 0; row < h; ++row) {
            for (int col = 0; col < w; ++col) {
                if (R.at<float>(row, col) > CornerStrengthThreshold) {
                    float max = R.at<float>(row, col);

                    // tìm cực trị địa phương
                    bool skip = false;
                    for (int nrow = 0; nrow < 5; ++nrow) {
                        for (int ncol = 0; ncol < 5; ++ncol) {
                            if (row + nrow - 2 < h && col + ncol - 2 < w && row + nrow - 2 > 0 && col + ncol - 2 > 0) {
                                if (R.at<float>(row + nrow - 2, col + ncol - 2) > max) {
                                    skip = true;
                                    break;
                                }
                            }
                        }
                        if (skip) {
                            break;
                        }
                    }
                    // nếu điểm đang xét thỏa ngưỡng và là cực trị địa phương lân cận kernel 5 x 5 thì chọn điểm đó
                    if (!skip) {
                        cv::circle(bgr, cv::Point(col, row), radius, color, thickness);
                        PointList.push_back(cv::Point(col, row));
                    }
                }
            }
        }

        // Display image indicating corners and save it
        cv::imshow("Corners", bgr);
        cv::imwrite(output, bgr);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    

    return 0;
}