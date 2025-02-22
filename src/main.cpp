#include <iostream>
#include <opencv2/opencv.hpp>
#include <map>


using namespace cv;
using namespace std;



/**
 * @author Shance Zhao
 * @StudentID  24013122
*/

 struct PixelCompare
    {
        bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b)
        {
                          
            return a.second > b.second;
        }
    };


// encodingArray   number to char
const char encodingArray[64] = {
    ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'x', 'y', 'w','z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'X', 'Y', 'W', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'
};

/**
 * color to number  
 * this code is RGB!!!! can not use this code to tranfer !!!!! opencv use different code  BGR
 **/
int colorToBinaryRGB(const cv::Vec3b& color) {
    // 黑色black (0,0,0)  000
    // 蓝色blue (0,0,255)  001
    // 绿色green (0,255,0)  010
    // 青色cyan (0,255,255)  011
    // 红色red (255,0,0)  100
    // 洋红色magenta (255,0,255)  101
    // 黄色yellow (255,255,0)  110
    // 白色white (255,255,255)  111
    if (color == cv::Vec3b(0, 0, 0)) return 0;
    if (color == cv::Vec3b(0, 0, 255)) return 1;
    if (color == cv::Vec3b(0, 255, 0)) return 2;
    if (color == cv::Vec3b(0, 255, 255)) return 3;
    if (color == cv::Vec3b(255, 0, 0)) return 4;
    if (color == cv::Vec3b(255, 0, 255)) return 5;
    if (color == cv::Vec3b(255, 255, 0)) return 6;
    if (color == cv::Vec3b(255, 255, 255)) return 7;
    return -1; // not match return -1
}


//opencv use BGR
int colorToBinaryBGR(const cv::Vec3b& color){
    if (color == cv::Vec3b(0, 0, 0)) return 0;
    if (color == cv::Vec3b(255, 0, 0)) return 1;
    if (color == cv::Vec3b(0, 255, 0)) return 2;
    if (color == cv::Vec3b(255, 255, 0)) return 3;
    if (color == cv::Vec3b(0, 0, 255)) return 4;
    if (color == cv::Vec3b(255, 0, 255)) return 5;
    if (color == cv::Vec3b(0, 255, 255)) return 6;
    if (color == cv::Vec3b(255, 255, 255)) return 7;
    return -1; // not match return -1
}

//use this method can make the code more robust, but make be slower
int compareAndGetMaxCountPixel(Mat& image)
{

    map<int, int> pixelMap;

    // calculate all pixel's count
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            cv::Vec3b pixelValue = image.at<cv::Vec3b>(i, j);
            int binaryColor = colorToBinaryBGR(pixelValue);
            if(binaryColor == -1){
                continue;
            }

            auto it = pixelMap.find(binaryColor);
            if (it != pixelMap.end())
            {
                it->second++;
            }
            else
            {
                pixelMap[binaryColor] = 1; // add one more key value
            }
        }
    }

    // find the max one
    auto maxPixelIt = std::max_element(pixelMap.begin(), pixelMap.end(), PixelCompare());

    if (maxPixelIt != pixelMap.end())
    {
        // key (pixel)
        int pixelValue = maxPixelIt->first;
        // value (frequency)
        int frequency = maxPixelIt->second;

    }
    return maxPixelIt->first;
}



// combine to binary to 8 bit, and match the char
char binaryPairToChar(int binary1, int binary2) {
    if (binary1 < 0 || binary2 < 0 || binary1 > 7 || binary2 > 7) {
        return '?'; // wrong number
    }
    int i = (binary1 << 3) | binary2;
    return encodingArray[i];
}



// calculate Angle of two vector (degree)
double calculateAngle(const cv::Point2f& O, const cv::Point2f& from_A, const cv::Point2f& to_B) {
    //vector AO and BO
    cv::Point2f v1 = from_A - O;
    cv::Point2f v2 = to_B - O;
    
    // dot of vector
    double dot = v1.x * v2.x + v1.y * v2.y;

    //vector length
    double lengthAO = std::sqrt(v1.x * v1.x + v1.y * v1.y);
    double lengthBO = std::sqrt(v2.x * v2.x + v2.y * v2.y);

    double angle = std::acos(dot / (lengthAO * lengthBO));
    // Convert radians to degrees
    return angle * 180.0 / M_PI;
}



Mat rotateAndScaleImage2show(Mat image, const vector<Vec3f>& circles) {
    if (circles.size() < 3) return image; 
    
 
    // distances of three circles
    double dist01 = norm(Point2f(circles[0][0], circles[0][1]) - Point2f(circles[1][0], circles[1][1]));
    double dist02 = norm(Point2f(circles[0][0], circles[0][1]) - Point2f(circles[2][0], circles[2][1]));
    double dist12 = norm(Point2f(circles[1][0], circles[1][1]) - Point2f(circles[2][0], circles[2][1]));

    // find the right angle with the longest distance
    Point2f A;
    if (dist01 > dist02 && dist01 > dist12) {
        A = Point2f(circles[2][0], circles[2][1]);
    } else if (dist02 > dist01 && dist02 > dist12) {
        A = Point2f(circles[1][0], circles[1][1]);
    } else {
        A = Point2f(circles[0][0], circles[0][1]);
    }

    
    // 2.use the longest line to find the middle point，as B
    int maxDistIndex1, maxDistIndex2;
    double maxDist = 0;
    for (int i = 0; i < circles.size(); ++i) {
        for (int j = i + 1; j < circles.size(); ++j) {
            double dist = norm(Point2f(circles[i][0], circles[i][1]) - Point2f(circles[j][0], circles[j][1]));
            if (dist > maxDist) {
                maxDist = dist;
                maxDistIndex1 = i;
                maxDistIndex2 = j;
            }
        }
    }

    // get B point(the middle of the image)
    Point2f B((circles[maxDistIndex1][0] + circles[maxDistIndex2][0]) / 2,
              (circles[maxDistIndex1][1] + circles[maxDistIndex2][1]) / 2);

    

    // calculate right angle side
    double d = sqrt((maxDist* maxDist)/2);

    double a_pos_x = B.x- d/2;
    double a_pos_y = B.y + d/2;

// 3. Rotate with point B as the center of the circle so that the three circles 
//return to the top left, bottom left, and bottom right positions (i.e., right Angle point A is at the bottom left).
     
    double angle = calculateAngle(B, A, Point2f(a_pos_x, a_pos_y));  

    //check lockwise or not 
    if(a_pos_y> A.y && a_pos_x < A.x){
        angle = -angle;
    }else if(a_pos_y<A.y && a_pos_x < A.x){
        angle = -angle;
    }


    
    //4. calculate the scale with right angle side
    double barcodeWidth = (d/41)*47;
    double width = image.rows> image.cols ? image.cols: image.rows;

    double sc = width/barcodeWidth;

    // getRotationMatrix2D use unclockwise
    Mat rotationMatrix = getRotationMatrix2D(B, angle, sc);

    Mat rotated;
    warpAffine(image, rotated, rotationMatrix, image.size());
        
    return rotated;
}



// Function to analyze barcode colors and decode characters
string analyzeBarcode(Mat barcodeImage) {

    double squareSize = barcodeImage.rows/47.000; //should 47.00  if use 47 would change to int type
   
    std::string barcodeText;

    vector<int> array;
      
    for (int i = 0; i < 47; i++) { //Y
        int squareY = i * squareSize ;
        int squareMiddleY = squareY + squareSize / 2;
        
        
        for (int j = 0; j < 47; j++) {//X

            if(i < 6 && j < 6){
                continue;// top-right
            }
            if(i > 40 && j < 6){
                continue;// bottom-left
            }

            if(j>40 && i > 40){
                continue;// bottom-right
            }

            int squareX = j * squareSize ;
            int squareMiddleX = squareX + squareSize/2;  //middle of squre


            cv::Vec3b pixelValue = barcodeImage.at<cv::Vec3b>(squareMiddleY, squareMiddleX);

            int binary1 = colorToBinaryBGR(pixelValue);
            if(binary1 == -1){
                continue;
            }
            array.push_back(binary1);

           
            
        }
    }

   
    // two square a time to decode

    for (int i = 0; i < array.size(); i += 2)
    {
        char ch = binaryPairToChar(array[i], array[i + 1]);

        if (ch == '?')
        {
            continue;
        }
        barcodeText.push_back(ch);
    }
    cout<<barcodeText<<endl;
    return barcodeText;
}


vector<Vec3f>  detect_circle_2_show(Mat & barcodeImage){
    vector<Vec3f> circles;


    //need to transfer to grayscale image before finding the circles
    Mat gray;
    // medianBlur ( barcodeImage , barcodeImage , 5 );
    cvtColor(barcodeImage, gray, COLOR_BGR2GRAY);
    

    //add one more gaussian blur step
    GaussianBlur(gray, gray, Size(5, 5), 2,2);
    
    Mat circle_image = Mat::zeros(barcodeImage.size(), CV_8UC3);

    HoughCircles (gray, circles , HOUGH_GRADIENT, 1 , 20 , 100 , 30 , 20 , 30 );  //  this paramter can detect circle point perfectly without blur like: circle size:3 (y,x):(70.5,70.5) (y,x):(728.5,70.5)  (y,x):(727.5,727.5)
    // HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 100, 50, 30, 20, 100);
    
    for(int i = 0;i < circles.size(); i++){
        Vec3f c = circles[i];
        circle(circle_image, Point(c[0],c[1]),c[2],Scalar(255,0,0),4,1);

    }
    return circles;

}


//split channels and try to remove the noise
Mat removeNoise(Mat image){
   
    
    
    std::vector<cv::Mat> bgr_planes;
    cv::split(image, bgr_planes);
    
    // BGR   not   RGB !!!!!!
    cv::Mat binaryB, binaryG, binaryR;
    cv::threshold(bgr_planes[0], binaryB, 127, 255, cv::THRESH_BINARY);
    cv::threshold(bgr_planes[1], binaryG, 127, 255, cv::THRESH_BINARY);
    cv::threshold(bgr_planes[2], binaryR, 127, 255, cv::THRESH_BINARY);
    
    //merge again
    std::vector<cv::Mat> binary_planes = {binaryB, binaryG, binaryR};
    cv::Mat binaryImageColor;
    cv::merge(binary_planes, binaryImageColor);

    return binaryImageColor;

}

int main(int argc, char** argv) {


    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <ImageFile>" << std::endl;
        return -1;
    }

    cv::Mat barcodeImage;
    barcodeImage = cv::imread(argv[1], cv::IMREAD_COLOR); 

    if (barcodeImage.empty()) {
        std::cout << "Could not read the image file" << std::endl;
        return -1;
    }

    // Detect circles in the image
    vector<Vec3f> cs = detect_circle_2_show(barcodeImage);

    Mat formattedImage = rotateAndScaleImage2show(barcodeImage, cs);

    formattedImage= removeNoise(formattedImage);

    string decodedText = analyzeBarcode(formattedImage);

    return -1;
}
