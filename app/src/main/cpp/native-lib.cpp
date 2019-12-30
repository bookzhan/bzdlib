#include <jni.h>
#include <string>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <iostream>
#include "common/BZLogUtil.h"
#include "common/bz_time.h"

using namespace dlib;
using namespace std;

int test_face_landmark() {
// We need a face detector.  We will use this to get bounding boxes for
    // each face in an image.
    frontal_face_detector detector = get_frontal_face_detector();
    // And we also need a shape_predictor.  This is the tool that will predict face
    // landmark positions given an image and face bounding box.  Here we are just
    // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
    // as a command line argument.

    int64_t startTime = getCurrentTime();
    shape_predictor sp;
    deserialize("/sdcard/bzmedia/shape_predictor_68_face_landmarks.dat") >> sp;
    BZLogUtil::logD("deserialize 耗时=%lld", getCurrentTime() - startTime);

    array2d<rgb_pixel> img;
//    load_image(img, "/sdcard/bzmedia/IMG_25.jpg");
    load_image(img, "/sdcard/bzmedia/IMG_25_small.jpg");

    // Now tell the face detector to give us a list of bounding boxes
    // around all the faces in the image.
    startTime = getCurrentTime();
    std::vector<rectangle> dets = detector(img);
    BZLogUtil::logD("detector 耗时=%lld", getCurrentTime() - startTime);

    BZLogUtil::logD("Number of faces detected: %lld", dets.size());

    // Now we will go ask the shape_predictor to tell us the pose of
    // each face we detected.
    std::vector<full_object_detection> shapes;
    for (unsigned long j = 0; j < dets.size(); ++j) {
        startTime = getCurrentTime();
        full_object_detection shape = sp(img, dets[j]);
        BZLogUtil::logD("sp 耗时=%lld", getCurrentTime() - startTime);

        BZLogUtil::logD("number of parts: %d", shape.num_parts());
        BZLogUtil::logD("pixel position of first part: x=%ld y=%ld", shape.part(0).x(),
                        shape.part(0).y());
        BZLogUtil::logD("pixel position of second part: x=%ld y=%ld", shape.part(1).x(),
                        shape.part(1).y());
        shapes.push_back(shape);
    }

    BZLogUtil::logD("---finish---");
    return 0;
}


extern "C" JNIEXPORT jint JNICALL
Java_com_luoye_dlibdemo_MainActivity_nativeTest(
        JNIEnv *env,
        jobject /* this */) {

    test_face_landmark();

    return 0;
}
