// main.cpp
// Author: Travis Vanderstad

#include <stdio.h>
#include <string.h>
#include <chrono>

#include <zed/Mat.hpp>
#include <zed/Camera.hpp>
#include <zed/utils/GlobalDefine.hpp>

#include "PointCloud.hpp"
#include "CloudViewer.hpp"

using namespace sl::zed;
using namespace std;

int main() {
	Camera* camera = new Camera(HD720, 15.0);
	ERRCODE code = camera->init(MODE::PERFORMANCE, 0);

	if (code != SUCCESS) {
		cout << errcode2str(code) << endl;
		delete camera;
		return 1;
	}

	int width = camera->getImageSize().width;
	int height = camera->getImageSize().height;
	PointCloud *cloud = new PointCloud(width, height);
	CloudViewer *viewer = new CloudViewer();
	int key = ' ';
	Mat depth, imLeft;

	// application quits when user stikes 'q'
	while (key != 'q') {
		camera->setConfidenceThreshold(98); // parameter is reliability index ~[1,100] with 100 as no filtering
		camera->grab(SENSING_MODE::RAW);
		depth = camera->retrieveMeasure(MEASURE::DEPTH);
		imLeft = camera->retrieveImage(SIDE::LEFT);
		cloud->fill(imLeft.data, (float*) depth.data, camera->getParameters());
		cloud->genHeightMap(320, 640);
		viewer->AddData(cloud);

		// Update the value of key so that we can quit when the user strikes 'q'
		key = viewer->getKey();
	}

	delete camera;
	delete cloud;
	delete viewer;
	return 0;
}
