
# OPENCV_SRC = /usr/local/Cellar/opencv/4.1.0_2/include/opencv4

SRC_FILES += src/math/cv_math.c \
			 src/meanShift/cv_meanshift.c \
			 src/filters/cv_filter.c \
			 src/3rdParties/OpenCVShow.cpp

INCLUDE_PATH += -Isrc/math \
				-Isrc/meanShift \
				-Isrc/filters \
				-Isrc/ \
				-Isrc/3rdParties \
				-I/usr/local/Cellar/opencv/4.1.0_2/include/opencv4/

LIBS_SEARCH_PATH = -L/usr/local/Cellar/opencv/4.1.0_2/lib 
LIBS += -lopencv_gapi \
		-lopencv_stitching \
		-lopencv_aruco \
		-lopencv_bgsegm \
		-lopencv_bioinspired \
		-lopencv_ccalib \
		-lopencv_dnn_objdetect \
		-lopencv_dpm \
		-lopencv_face \
		-lopencv_freetype \
		-lopencv_fuzzy \
		-lopencv_hfs \
		-lopencv_img_hash \
		-lopencv_line_descriptor \
		-lopencv_quality \
		-lopencv_reg \
		-lopencv_rgbd \
		-lopencv_saliency \
		-lopencv_sfm \
		-lopencv_stereo \
		-lopencv_structured_light \
		-lopencv_phase_unwrapping \
		-lopencv_superres \
		-lopencv_optflow \
		-lopencv_surface_matching \
		-lopencv_tracking \
		-lopencv_datasets \
		-lopencv_text \
		-lopencv_dnn \
		-lopencv_plot \
		-lopencv_videostab \
		-lopencv_video \
		-lopencv_xfeatures2d \
		-lopencv_shape \
		-lopencv_ml \
		-lopencv_ximgproc \
		-lopencv_xobjdetect \
		-lopencv_objdetect \
		-lopencv_calib3d \
		-lopencv_features2d \
		-lopencv_highgui \
		-lopencv_videoio \
		-lopencv_imgcodecs \
		-lopencv_flann \
		-lopencv_xphoto \
		-lopencv_photo \
		-lopencv_imgproc \
		-lopencv_core

FILE_BUILD_NAME := opencv_api

CPPDEFINES = -DIRIS_DEBUG_MODE

# make commands
build:
	@g++ -std=c++11 $(INCLUDE_PATH) $(LIBS_SEARCH_PATH) $(LIBS) $(SRC_FILES) main.cpp -o $(FILE_BUILD_NAME) $(CPPDEFINES)

run:
	@./$(FILE_BUILD_NAME)

exe: build run