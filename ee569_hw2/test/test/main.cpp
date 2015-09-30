#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    const int rows = 5;
    const int cols = 3;
    
    Mat points(rows, cols, CV_32F, Scalar(0));
    Mat mean, covar, invcovar;
    
    Mat p1(1,cols,CV_32F,Scalar(0));
    
    Mat p2(1,cols,CV_32F,Scalar(0));
    
    float x[rows][cols] = { {20, 55, 119}, {123, 333, 11},  {113, 321, 11},  {103, 313, 191},  {123, 3433, 1100} };
    
    for (int i = 0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            points.at<float>(i,j)=x[i][j];
        }
        
    }
    p1 = points(Range(0,1), Range::all());
    
    p2 = points(Range(1,2), Range::all());
    
    cout<<"input matrix: \n"<<points<<endl<<endl;
    
    calcCovarMatrix(points, covar, mean, CV_COVAR_NORMAL+CV_COVAR_ROWS, -1);
    
    cout << "mean: " << endl;
    cout << mean <<endl<< endl;
    
    covar = covar/(points.rows-1);
    // above mentioned posts have followed this technique to obtain covariance matrix and they seemed to
    // get the right covariance matrix.
    cout<<"covar matrix:\n"<<covar<<endl<<endl;
    
    invert(covar, invcovar, DECOMP_SVD);
    // Official Documentation site refers that calculating inverse
    // covariance matrix with DECOMP_SVD flag is more accurate
    cout<<"inverse covar matrix:\n"<<invcovar<<endl<<endl;
    cout<<"Mahalanobis distance between:"<<endl<<endl;
    
    cout<<"p1"<<p1<<endl;
    cout<<"and"<<endl;
    cout<<"p2"<<p2<<endl;
    cout<<"is: "<<endl;
    cout<< Mahalanobis( p1, p2, invcovar)<<endl<<endl;
    return 0;
}