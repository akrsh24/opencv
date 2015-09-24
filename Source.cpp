#include <cv.h>
#include<iostream>
#include <highgui.h>

using namespace std;
using namespace cv;

int main()
{
	Mat video,temp,cont,vidhsv,vidinrng,vidmorph,labels;                                      //data type for image(Mat),class
	//double index;
	VideoCapture cap(0);                            // initialising camera.
	cap.set(1,640);
	cap.set(2,480);
//cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
//cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	cap.open(0);
	vector<vector<Point> > cont_temp,cont_live,cont; 
    vector<Vec4i> hierarchy_temp,hierarchy_live; 
	namedWindow("control", CV_WINDOW_NORMAL); 
	int huelow = 0; 
  int huehigh = 179; 
  int satlow = 0; 
  int sathigh = 255; 
  int vallow = 0; 
  int valhigh = 255; 
  cvCreateTrackbar("LowH", "control", &huelow, 179); 
 cvCreateTrackbar("HighH", "control", &huehigh, 179); 
 cvCreateTrackbar("LowS", "control", &satlow, 255); 
 cvCreateTrackbar("HighS", "control", &sathigh, 255); 
 cvCreateTrackbar("LowV", "control", &vallow, 255); 
 cvCreateTrackbar("HighV", "control", &valhigh, 255); 


 namedWindow("control_TEMP", CV_WINDOW_NORMAL); 
	int huelowt = 0; 
  int huehight = 179; 
  int satlowt = 0; 
  int sathight = 255; 
  int vallowt = 0; 
  int valhight = 255; 
  cvCreateTrackbar("lowtH", "control_TEMP", &huelowt, 179); 
 cvCreateTrackbar("hightH", "control_TEMP", &huehight, 179); 
 cvCreateTrackbar("lowtS", "control_TEMP", &satlowt, 255); 
 cvCreateTrackbar("hightS", "control_TEMP", &sathight, 255); 
 cvCreateTrackbar("lowtV", "control_TEMP", &vallowt, 255); 
 cvCreateTrackbar("hightV", "control_TEMP", &valhight, 255); 
//video = imread("C:\\c2.jpg",CV_LOAD_IMAGE_COLOR);
 int shape=0;
 Mat element = getStructuringElement( MORPH_ELLIPSE,Size( 5, 5 ),Point( 0, 0 ) ); 
	while(1)
	{
		//cap>>video;
		cap.read(video);
		temp = imread("C:\\c1.jpg",CV_LOAD_IMAGE_COLOR);
		//Laplacian(video,lap,1);
		//imshow("lap",lap);
		labels=Mat::zeros(video.size(),CV_8UC1);
		//Org=temp.clone();
		//capturing temp.      
		//cout<<temp.size()<<endl;
		cvtColor(video,vidhsv,CV_BGR2HSV);// chnges frm bgr to hsv
		//inRange(temp,Scalar(huelowt,satlowt,vallowt),Scalar(huehight,sathight,valhight),temp);//B G R(tie)
		inRange(vidhsv,Scalar(huelow,satlow,vallow),Scalar(huehigh,sathigh,valhigh),vidinrng);
		inRange(temp,Scalar(0,0,0),Scalar(179,255,255),temp);
		imshow("in range image",vidinrng);
		//stereo vision  // red 0,0,255 bgr 0,255,0
		 // contours in the image */
		GaussianBlur(vidhsv,vidhsv,Size(7,7),1.5,1.5);

		findContours( temp.clone(), cont_temp, hierarchy_temp, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0, 0) ); 
		drawContours( temp, cont_temp,-1, Scalar(0,255,255), 4, 8, hierarchy_temp, 3, Point()); 
		Mat s = Mat::zeros(temp.size(),CV_8UC3);
		Mat p = Mat::zeros(video.size(),CV_8UC3);
		drawContours( s, cont_temp,1, Scalar(0,255,255), 4, 8, hierarchy_temp, 3, Point());
		imshow("cont_temp",s);
		erode(vidinrng,vidmorph,element);		//blur(thrushold,bl,Size(5,5));
		dilate(vidmorph,vidmorph,element);//store (a,b,c) a to b
		dilate(vidmorph,vidmorph,element);
		erode(vidmorph,vidmorph,element);//store (a,b,c) a to b
		Mat sp = Mat::zeros(video.size(),CV_8UC3);
		imshow("Filtered Contours",sp);
		//cout<<"emate = " <<cont_temp[3]<<endl;
		findContours( vidmorph.clone(), cont_live, hierarchy_live, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0, 0) );
		drawContours( p, cont_live,-1, Scalar(0,255,255), 4, 8, hierarchy_live, 3, Point());
		for(int l = 0 ; l < cont_live.size();++l)
		{
		  if(contourArea(cont_live[l])>10000)
			drawContours( sp, cont_live,l, Scalar(0,255,255), 4, 8, hierarchy_live, 3, Point());
		}
		imshow("live contours",p);
		for(unsigned int k=0;k<cont_live.size();k++)
		{
			//shape = 100;
			double index = 1;
			double index_temp;
			if(contourArea(cont_live[k])<10000)
				continue;
			for(unsigned int j=0;j<cont_temp.size();j++)
			{
			//findContours( noise.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0, 0) ); 
			//drawContours( temp, contours,3, Scalar(0,255,255), 4, 8, hierarchy, 3, Point()); 
				index_temp = matchShapes(cont_temp[j],cont_live[k],CV_CONTOURS_MATCH_I2,0);
				if(index_temp < index)
				{
					index = index_temp;
					shape = j;
				}
			}
			//calculate centroid
					Moments mymoments = moments(vidmorph,true);
					double m01 = mymoments.m01; //gives xoordinates of bot of y
					double m10 = mymoments.m10; //gives xoordinates of bot of x
					double m00 = mymoments.m00;//m00 gives area;
					int x=m10/m00;
					int y=m01/m00;
			
					if( shape==2 )
						putText(video,"rEctangle",Point(x,y),4,8,Scalar(110,101,145),1,2);
					else if( shape==3 )
						putText(video,"CIRCLe",Point(x,y),4,8,Scalar(110,101,145),1,2);
					else if( shape==0 )
						putText(video,"Arrow",Point(x,y),4,8,Scalar(110,101,145),1,2);// 0 for arrow
					else if( shape==4 )
						putText(video,"SQUARE",Point(x,y),4,8,Scalar(110,101,145),1,2);
					else if( shape==1 )
						putText(video,"TRAINGLE",Point(x,y),4,8,Scalar(110,101,145),1,2);// 1 triangle
		}	
	

		
	//		cout<<"value of x,y,index "<<x<<","<<y<<" , "<<endl;
			imshow("Processed Video",video);//reading temp variable
		imshow("Processed Template",temp);
//		imshow("bina noise ka",temp);	    
		//if((char)waitKey(30) == 27)	break;
		waitKey(1);
	}
	return 0;
}