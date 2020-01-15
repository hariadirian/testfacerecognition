#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#include "jackylib.h"
#include <iostream>

namespace OpenCV_faceRecognition {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace cv;
	using namespace std;
	using namespace jacky_lib;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  ptbVideo;
	protected:

	protected:
	private: System::Windows::Forms::Button^  btnStart;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ptbVideo = (gcnew System::Windows::Forms::PictureBox());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ptbVideo))->BeginInit();
			this->SuspendLayout();
			// 
			// ptbVideo
			// 
			this->ptbVideo->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->ptbVideo->Location = System::Drawing::Point(12, 12);
			this->ptbVideo->Name = L"ptbVideo";
			this->ptbVideo->Size = System::Drawing::Size(464, 330);
			this->ptbVideo->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->ptbVideo->TabIndex = 0;
			this->ptbVideo->TabStop = false;
			this->ptbVideo->Click += gcnew System::EventHandler(this, &MainForm::ptbVideo_Click);
			// 
			// btnStart
			// 
			this->btnStart->Location = System::Drawing::Point(482, 12);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(75, 23);
			this->btnStart->TabIndex = 1;
			this->btnStart->Text = L"Start";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &MainForm::btnStart_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(566, 354);
			this->Controls->Add(this->btnStart);
			this->Controls->Add(this->ptbVideo);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ptbVideo))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e) {
		VideoCapture videoCap;
		videoCap.open(0); 
		Mat frame;
		mat2picture mat2bmp;
		/** Global variables */
		string face_cascade_name = "haarcascade_frontalface_alt.xml";
		string eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
		string nose_cascade_name = "haarcascade_nosez.xml";
		string mouth_cascade_name = "haarcascade_mouth.xml";
		CascadeClassifier face_cascade;
		CascadeClassifier eyes_cascade;
		CascadeClassifier nose_cascade;
		CascadeClassifier mouth_cascade;
		// Load the cascades
		face_cascade.load(face_cascade_name);
		eyes_cascade.load(eyes_cascade_name);
		nose_cascade.load(nose_cascade_name);
		mouth_cascade.load(mouth_cascade_name);
		while (true)
		{
			Mat frame;
			videoCap.read(frame);
			//imshow("Video by OpenCV", frame);
			// Face & eye detection 

			//Image pre-processing
			resize(frame, frame, cv::Size(640, 480));
			std::vector<Rect> faces;
			Mat frame_gray;

			cvtColor(frame, frame_gray, CV_BGR2GRAY);
			equalizeHist(frame_gray, frame_gray);

			//-- Detect faces
			
			face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

			for (size_t i = 0; i < faces.size(); i++)
			{
				cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
				ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(0, 255, 0), 4, 8, 0);

				Mat faceROI = frame_gray(faces[i]);
				//imshow("face", faceROI);
				std::vector<Rect> eyes;
				std::vector<Rect> nose;
				std::vector<Rect> mouth;

				Rect mouthROI((int)faces[i].width / 4,(int)(faces[i].height/4)*2.9 ,(int)faces[i].width / 2, (int)faces[i].height / 5);
				Rect noseROI((int)faces[i].width / 4, (int)(faces[i].height / 2), (int)faces[i].width / 2, (int)faces[i].height / 4);
				imshow("nose",faceROI(noseROI));
				imshow("mouth", faceROI(mouthROI));

				//-- In each face, detect eyes,nose,mouth
				eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
				for (size_t j = 0; j < eyes.size(); j++)
				{
					cv::Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
					int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
					circle(frame, center, radius, Scalar(255, 0, 0), 3, 8, 0);
				}


				nose_cascade.detectMultiScale(faceROI(noseROI), nose, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
				for (size_t j = 0; j < nose.size(); j++)
				{
					cv::Point center(faces[i].x + nose[j].x +noseROI.x+ nose[j].width*0.5, faces[i].y + nose[j].y + noseROI.y+ nose[j].height*0.5);
					int radius = cvRound((nose[j].width + nose[j].height)*0.25);
					circle(frame, center, radius, Scalar(0, 255, 85), 3, 8, 0);
				}

				mouth_cascade.detectMultiScale(faceROI(mouthROI), mouth, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
				for (size_t j = 0; j < mouth.size(); j++)
				{
					cv::Point center(faces[i].x + mouth[j].x + mouthROI.x + mouth[j].width*0.5, faces[i].y + mouth[j].y + mouthROI.y + mouth[j].height*0.5);
					int radius = cvRound((mouth[j].width + mouth[j].height)*0.25);
					circle(frame, center, radius, Scalar(85, 100, 255), 2, 8, 0);
				}

			}

			//show on picture box
			ptbVideo->Image = mat2bmp.Mat2Bimap(frame);
			ptbVideo->Refresh();
			if (waitKey(10) == 27)
			{
				break;
			}

		}

	}
	private: System::Void ptbVideo_Click(System::Object^ sender, System::EventArgs^ e) {
	}
};
}
