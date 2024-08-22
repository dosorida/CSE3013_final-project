#pragma once

#include "ofMain.h"

struct Star {
	float x; // ���� x ��ǥ
	float y; // ���� y ��ǥ
};

struct Edge {
	int first; // ����� ù ��° ���� �ε���
	int second; // ����� �� ��° ���� �ε���
};

struct Constellation {
	int numStars; // ���� ����
	vector<Star> stars; // ������ ��ǥ ����
	vector<Edge> edges; // ����� ������ �ε��� ����
	vector<float> costs; // ���� ���� �Ÿ� ����
	vector<int> parent; // ũ�罺Į �˰��򿡼� ���Ǵ� �θ� ��� ����
	vector<int> rank; // ũ�罺Į �˰��򿡼� ���Ǵ� ��ũ ����
	float minCost = 0; // �ּ� ���
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void createConstellation(); // ���ڸ� ���� �Լ�
	void resetConstellations(); // ������ ���ڸ� �ʱ�ȭ �Լ�
	void makeSet(Constellation& constellation, int v); // ���� ���� �Լ�
	int findSet(Constellation& constellation, int v); // v�� ���� ������ ��ǥ ��带 ã�� �Լ�
	void unionSets(Constellation& constellation, int a, int b); // �� ������ ��ġ�� �Լ�
	void drawStar(float x, float y, float radius, int numPoints); // ���� �׸��� �Լ�

private:
	vector<Constellation> constellations; // ������ ���ڸ����� ����
	float twinklingSpeed; // ���ڸ� ��¦�� ȿ���� �ӵ�
	float twinklingAlpha; // ���ڸ� ��¦�� ȿ���� ���� ��
	float offsetX; // ���ڸ� �׸��� �� x ������ �̵��� �Ÿ�
	float offsetY; // ���ڸ� �׸��� �� y ������ �̵��� �Ÿ�
};