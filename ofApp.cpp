#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofEnableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	//ofRotateY(ofGetFrameNum() * 1.44);

	float phi_deg_step = 0.1;
	float theta_deg_step = 0.1;

	float R = 230;
	float r = R * 0.3;

	float threshold_1 = 0.5 - 0.0125;
	float threshold_2 = 0.5 + 0.0125;

	for (int i = 0; i < 4; i++) {

		ofMesh mesh;

		ofColor face_color;
		face_color.setHsb(ofMap(i, 0, 4, 0, 255), 200, 255);

		auto noise_seed = ofRandom(100);

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

				auto noise_location = this->make_point(R, r, theta_deg, phi_deg) * 0.01;
				auto noise_value = ofNoise(noise_location.x, noise_location.y, noise_location.z, noise_seed + ofGetFrameNum() * 0.01);
				if (noise_value <= threshold_1 || noise_value >= threshold_2) { continue; }

				float alpha = ofMap(abs(noise_value - 0.5), 0, 0.0125, 255, 0);

				auto index = mesh.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));

				mesh.addVertices(vertices);

				mesh.addIndex(index + 0); mesh.addIndex(index + 1); mesh.addIndex(index + 3);
				mesh.addIndex(index + 0); mesh.addIndex(index + 3); mesh.addIndex(index + 2);

				for (auto& vertex : vertices) {

					mesh.addColor(ofColor(face_color, alpha));
				}
			}
		}

		mesh.draw();
	}

	/*
	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v, float scale) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u) * scale) * cos(v);
	auto y = (R + r * cos(u) * scale) * sin(v);
	auto z = r * sin(u) * scale;

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}