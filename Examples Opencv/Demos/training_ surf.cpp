//Performance measures calculations for report
	if (testing)
	{
		cout<<object.rows<<" "<<object.cols<<endl;

		//calculate integral image
		Mat iObject;
		integral(object, iObject);
		imshow("Good Matches", iObject);
		imwrite("/home/aphillips/Documents/PG/Examples Opencv/Demos/ee.jpg", iObject);
		cvWaitKey(0);

		//calculate number of interest points, computation time as f(minHess)
		int minHessVector[]={100, 500, 1000, 1500, 2000, 2500, 3000,
									3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500,
									8000, 8500, 9000, 9500, 10000};
		int minH;
		std::ofstream file;
		file.open("/home/aphillips/Documents/PG/Examples Opencv/Demos/nn.csv", std::ofstream::out);
		for (int i=0; i<20; i++)
		{
			minH=minHessVector[i];
			t = (double)getTickCount();
			SurfFeatureDetector detector(minH);
			detector.detect(object, kpObject);
			t = ((double)getTickCount() - t)/getTickFrequency();
			file<<minHess<<","<<kpObject.size()<<","<<t<<",";
			cout<<t<<" "<<kpObject.size()<<" "<<desObject.size()<<endl;

			t = (double)getTickCount();
			SurfDescriptorExtractor extractor;
			extractor.compute(object, kpObject, desObject);
			t = ((double)getTickCount() - t)/getTickFrequency();
			file<<t<<endl;
		}
		file.close();

	//Display keypoints on training image
	Mat interestPointObject=object;
	for (unsigned int i=0; i<kpObject.size();i++)
	{
		if(kpObject[i].octave)
		{
			circle(interestPointObject,kpObject[i].pt,kpObject[i].size,0);
			string octaveS;
			switch(kpObject[i].octave)
			{
			case 0:
				octaveS="0";
				break;
			case 1:
				octaveS='1';
				break;
			case 2:
				octaveS='2';
				break;
			default:
				break;

			}
			putText(interestPointObject, octaveS, kpObject[i].pt,
				    FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(0,0,250), 1, CV_AA);
		}

	}
	imshow("Good Matches",interestPointObject);
	imwrite("~/Examples Opencv/Demos/ff.jpg", interestPointObject);
	cvWaitKey(0);
	}